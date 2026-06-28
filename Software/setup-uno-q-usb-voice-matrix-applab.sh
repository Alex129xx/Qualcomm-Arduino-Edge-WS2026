#!/usr/bin/env bash
set -euo pipefail

APP_NAME="uno-q-echoglow"
APP_DIR="/home/arduino/ArduinoApps/${APP_NAME}"
ASOUND_CONF="/etc/asound.conf"

info() { echo "[INFO] $*"; }
ok() { echo "[OK] $*"; }
warn() { echo "[WARN] $*"; }
fail() { echo "[FAIL] $*" >&2; exit 1; }

preflight_checks() {
  [ "$(id -u)" -eq 0 ] || fail "Run as root: sudo $0"

  for cmd in docker arecord; do
    command -v "$cmd" >/dev/null 2>&1 || fail "Required command not found: $cmd"
  done

  id arduino >/dev/null 2>&1 || fail "User 'arduino' not found"

  arecord -l | grep -q "UGREEN\|USB Audio\|card 0: Audio" || {
    warn "USB microphone not obviously detected."
    warn "Current capture devices:"
    arecord -l || true
    fail "Plug in the USB microphone and try again."
  }

  ok "Preflight OK"
}

ensure_no_asound_conf() {
  if [ -f "${ASOUND_CONF}" ]; then
    cp -a "${ASOUND_CONF}" "${ASOUND_CONF}.bak.$(date +%Y%m%d_%H%M%S)"
    rm -f "${ASOUND_CONF}"
    ok "Removed ${ASOUND_CONF}; backup kept"
  else
    ok "${ASOUND_CONF} absent"
  fi
}

deploy_app() {
  local script_dir
  script_dir="$(dirname "$(realpath "$0")")"

  local src="${script_dir}/uno-q-echoglow"

  [ -d "$src" ] || fail "App folder not found: $src"

  mkdir -p "/home/arduino/ArduinoApps"
  rm -rf "$APP_DIR"
  cp -r "$src" "$APP_DIR"
  chown -R arduino:arduino "$APP_DIR"

  ok "App deployed to $APP_DIR"
}

verify() {
  echo ""
  info "Verification"

  arecord -l | grep -q "UGREEN\|USB Audio\|card 0: Audio" \
    && ok "USB microphone detected" \
    || warn "USB microphone not detected"

  test ! -f "${ASOUND_CONF}" \
    && ok "/etc/asound.conf absent" \
    || warn "/etc/asound.conf still exists"

  test -f "${APP_DIR}/app.yaml" \
    && ok "App present: ${APP_DIR}" \
    || warn "App not found: ${APP_DIR}"

  echo ""
  info "Expected Python microphone device:"
  echo '  plughw:CARD=Audio,DEV=0'
}

main() {
  preflight_checks
  ensure_no_asound_conf
  deploy_app
  verify
}

main "$@"
