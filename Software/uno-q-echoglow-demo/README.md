# UNO Q EchoGlow Demo Mode

This is a no-extra-hardware demo for the UNO Q EchoGlow workshop.

The full EchoGlow project uses an analog microphone, Edge Impulse keyword spotting, a NeoDriver board, and a NeoPixel strip. This demo keeps the same command names and the same RouterBridge flow, but replaces real voice detection with a small web control panel.

## What it demonstrates

- `Warmer-light`
- `Cooler-light`
- `Brighter`
- `Dimmer`

When you click a button in the browser, the Python side calls the same Bridge handler name that the real keyword spotting app uses. The Arduino sketch then updates the onboard LED Matrix and briefly flashes the built-in LED as a small confirmation indicator.

```text
Browser button
    -> Python web server
    -> Bridge.call("warmer_light") / Bridge.call("cooler_light") / ...
    -> Arduino sketch
    -> onboard LED Matrix + built-in LED
```

## Expected demo behavior

| Button | Onboard LED Matrix feedback | Built-in LED |
|---|---|---|
| `Warmer-light` | Sun / warm-light icon | Short confirmation pulse |
| `Cooler-light` | Snowflake / cool-light icon | Short confirmation pulse |
| `Brighter` | Large bright icon | Short confirmation pulse |
| `Dimmer` | Small dim icon | Short confirmation pulse |
| `Reset demo` | Heart idle screen | Off |
| `Clear matrix` | Matrix off | Off |

The built-in LED is not the main display for this demo. The primary feedback should be the LED Matrix icon.

## Hardware required

- Arduino UNO Q
- USB-C cable or network connection

No microphone, NeoDriver, or NeoPixel strip is required for this demo.

## How to run

1. Open Arduino App Lab.
2. Import or open `Software/uno-q-echoglow-demo`.
3. Click **Run**.
4. Wait for the output:

```text
UNO Q EchoGlow Demo server started on port 7000
Open http://<UNO_Q_IP>:7000 in your browser
```

5. Open this address from your computer:

```text
http://<UNO_Q_IP>:7000/
```

For example:

```text
http://172.16.80.177:7000/
```

## Files

```text
uno-q-echoglow-demo/
├─ app.yaml
├─ python/
│  └─ main.py
└─ sketch/
   ├─ heart_frames.h
   ├─ sketch.ino
   └─ sketch.yaml
```

## How this differs from the full EchoGlow app

The full app uses `keyword_spotting` and an analog microphone. Its Python side registers callbacks such as:

```python
spotter.on_detect("Warmer-light", lambda: Bridge.call("warmer_light"))
```

This demo uses web buttons instead, but keeps the same Arduino Bridge handler names:

```python
Bridge.call("warmer_light")
Bridge.call("cooler_light")
Bridge.call("brighter")
Bridge.call("dimmer")
```

That means you can understand the command flow first, then replace the web buttons with real voice detection later.

## Troubleshooting

If the webpage does not open, confirm that `app.yaml` exposes port `7000`:

```yaml
ports:
  - 7000
```

If the page opens but the LED Matrix does not respond, check the App Lab output for Bridge or sketch compilation errors.
