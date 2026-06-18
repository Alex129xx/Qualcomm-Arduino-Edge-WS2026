# SPDX-FileCopyrightText: Copyright (C) Electronic Cats
#
# SPDX-License-Identifier: MPL-2.0

from arduino.app_utils import *

import json
import threading
import time
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from urllib.parse import urlparse

COMMANDS = {
    "warmer": ("Warmer-light", "warmer_light"),
    "cooler": ("Cooler-light", "cooler_light"),
    "brighter": ("Brighter", "brighter"),
    "dimmer": ("Dimmer", "dimmer"),
    "reset": ("Reset demo", "reset_demo"),
    "clear": ("Clear matrix", "clear_matrix"),
}

state_lock = threading.Lock()
state = {
    "last_command": "None",
    "count": 0,
    "started_at": time.time(),
    "events": [],
    "last_error": None,
}


def record_event(key, ok, error=None):
    label = COMMANDS.get(key, (key, None))[0]
    item = {
        "time": time.strftime("%H:%M:%S"),
        "key": key,
        "label": label,
        "ok": ok,
        "error": error,
    }
    with state_lock:
        state["last_command"] = label
        state["count"] += 1
        state["last_error"] = error
        state["events"].insert(0, item)
        del state["events"][8:]


def snapshot():
    with state_lock:
        data = dict(state)
    data["uptime_seconds"] = int(time.time() - data["started_at"])
    data["commands"] = {key: value[0] for key, value in COMMANDS.items()}
    return data


def run_command(key):
    command = COMMANDS.get(key)
    if command is None:
        return False, "Unknown command: " + key

    label, bridge_name = command
    try:
        Bridge.call(bridge_name)
    except Exception as exc:
        error = repr(exc)
        print("Command failed:", key, error)
        record_event(key, False, error)
        return False, error

    print("Demo command:", label, "->", bridge_name)
    record_event(key, True)
    return True, None


def run_demo_sequence():
    for key in ("warmer", "brighter", "cooler", "dimmer", "reset"):
        run_command(key)
        time.sleep(1.1)


class WebHandler(BaseHTTPRequestHandler):
    def send_text(self, status, text, content_type="text/html; charset=utf-8"):
        body = text.encode("utf-8")
        self.send_response(status)
        self.send_header("Content-Type", content_type)
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def do_GET(self):
        path = urlparse(self.path).path.rstrip("/") or "/"

        if path == "/api/status":
            self.send_text(200, json.dumps(snapshot()), "application/json; charset=utf-8")
            return

        if path == "/api/demo":
            threading.Thread(target=run_demo_sequence, daemon=True).start()
            self.send_text(200, render_page("Demo sequence started"))
            return

        if path.startswith("/cmd/"):
            key = path.split("/")[-1]
            ok, error = run_command(key)
            message = "OK: " + key if ok else "ERROR: " + str(error)
            self.send_text(200 if ok else 400, render_page(message))
            return

        self.send_text(200, render_page())

    def log_message(self, format, *args):
        return


def render_page(message=""):
    data = snapshot()
    buttons = "".join(
        '<p><a class="button" href="/cmd/{key}">{label}</a></p>'.format(key=key, label=value[0])
        for key, value in COMMANDS.items()
    )
    events = json.dumps(data["events"], indent=2)
    return """<!doctype html>
<html>
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>UNO Q EchoGlow Demo</title>
  <style>
    body {{ font-family: sans-serif; max-width: 760px; margin: 32px auto; padding: 0 18px; }}
    .card {{ border: 1px solid #ddd; border-radius: 14px; padding: 18px; margin: 16px 0; }}
    .button {{ display: block; border: 1px solid #999; border-radius: 10px; padding: 14px; text-decoration: none; color: #111; }}
    pre {{ background: #111; color: #eee; padding: 12px; border-radius: 10px; overflow-x: auto; }}
  </style>
</head>
<body>
  <h1>UNO Q EchoGlow Demo</h1>
  <p>No microphone or NeoPixel strip required. These buttons simulate the EchoGlow voice commands.</p>
  <div class="card">
    <h2>Status</h2>
    <p><b>{message}</b></p>
    <p>Last command: <b>{last}</b></p>
    <p>Command count: {count}</p>
    <p>Uptime: {uptime}s</p>
  </div>
  <div class="card">
    <h2>Commands</h2>
    {buttons}
    <p><a class="button" href="/api/demo">Run full demo sequence</a></p>
  </div>
  <div class="card">
    <h2>Recent events</h2>
    <pre>{events}</pre>
  </div>
</body>
</html>""".format(
        message=message,
        last=data["last_command"],
        count=data["count"],
        uptime=data["uptime_seconds"],
        buttons=buttons,
        events=events,
    )


def start_web_server():
    server = ThreadingHTTPServer(("0.0.0.0", 7000), WebHandler)
    print("UNO Q EchoGlow Demo server started on port 7000")
    print("Open http://<UNO_Q_IP>:7000 in your browser")
    server.serve_forever()


def loop():
    time.sleep(1)


threading.Thread(target=start_web_server, daemon=True).start()
App.run(user_loop=loop)
