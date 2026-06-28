# UNO Q Voice Matrix Workshop

This branch adapts the original UNO Q EchoGlow example for a simpler hardware setup:

- USB microphone for keyword spotting
- Arduino UNO Q onboard LED matrix for visual feedback

The app detects the keywords `Warmer-light`, `Cooler-light`, `Dimmer`, and `Brighter` with Arduino App Lab's `keyword_spotting` Brick. When a keyword is detected, the Python app calls the microcontroller through the Bridge, and the sketch shows a matching pattern on the onboard LED matrix.

## Bricks Used

- `keyword_spotting` — detects sound patterns and triggers an event when a keyword is matched.

## Hardware and Software Requirements

### Hardware

- Arduino UNO Q
- USB microphone connected to the UNO Q
- No external NeoDriver or NeoPixel strip required

### Software

- Arduino App Lab
- This app requires a custom Edge Impulse keyword spotting model with the labels:
Warmer-light, Cooler-light, Dimmer, Brighter.

    If the App Lab keyword runner logs show only:
    ['background', 'hey_arduino', 'other']

    then the default Hey Arduino model is being used, and the four command callbacks will not trigger.

## Setup

This USB microphone version does not require the analog microphone setup script. The original script is still present in the repository for the analog microphone + NeoPixel version, but this branch is intended to run with the default USB microphone path used by App Lab.

Clone this branch on the Arduino UNO Q:

```bash
git clone -b learn-usb-mic-led-matrix https://github.com/Alex129xx/Qualcomm-Arduino-Edge-WS2026
cd Qualcomm-Arduino-Edge-WS2026
```

If you need to copy the example into the Arduino App Lab examples directory manually, copy `Software/uno-q-echoglow` into the examples location used by your board firmware.

## How to Use the Example

### Hardware Setup

1. Connect the USB microphone to the Arduino UNO Q.
2. No Qwiic NeoDriver or NeoPixel strip is needed.
3. Use the onboard LED matrix for feedback.

### Launch the App

1. Open Arduino App Lab.
2. Connect to the board using Network Mode.
3. Open `UNO Q Voice Matrix`.
4. Click the Play button in the top right corner.
5. Say one of the voice commands into the USB microphone.

| Keyword | LED Matrix Action |
|---|---|
| `Warmer-light` | Blinks a warm/sun-style pattern |
| `Cooler-light` | Blinks a cool/snowflake-style pattern |
| `Brighter` | Blinks a full-bright matrix pattern |
| `Dimmer` | Shows a small dim center pattern |

## How it Works

### Python side (`python/main.py`)

- `spotter = KeywordSpotting()` initializes keyword spotting with App Lab's default microphone path, intended here for a USB microphone.
- `spotter.on_detect(...)` registers callbacks for each keyword.
- `Bridge.call(...)` notifies the microcontroller which keyword was detected.

### Microcontroller side (`sketch/sketch.ino`)

- `Arduino_LED_Matrix matrix;` controls the onboard LED matrix.
- `Bridge.provide(...)` exposes handlers to the Python side.
- `warmer_light()`, `cooler_light()`, `brighter()`, and `dimmer()` show different LED matrix patterns.

## Notes

- This branch removes the external NeoDriver/NeoPixel dependency from the sketch.
- The onboard LED matrix is monochrome, so `Warmer-light` and `Cooler-light` are represented by different patterns instead of different colors.
- If App Lab cannot find the USB microphone automatically, check the board with `arecord -l` and then explicitly configure the ALSA microphone device in `python/main.py`.
