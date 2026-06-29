# UNO Q Voice Matrix Workshop

This branch adapts the original UNO Q EchoGlow example for a simpler hardware setup:

- USB microphone for keyword spotting
- Arduino UNO Q onboard LED matrix for visual feedback
- No external NeoDriver, NeoPixel strip, or analog microphone setup required

The app detects voice commands with Arduino App Lab's `keyword_spotting` Brick. When a keyword is detected, the Python app calls the microcontroller through the Bridge, and the sketch shows a matching icon on the onboard LED matrix. When no command is active, the matrix keeps showing a heart icon.

## Bricks Used

- `keyword_spotting` — detects sound patterns and triggers an event when a keyword is matched.

## Hardware and Software Requirements

### Hardware

- Arduino UNO Q
- USB microphone connected to the UNO Q

### Software

- Arduino App Lab
- A custom Edge Impulse keyword spotting model with the labels used by `python/main.py`

The current Python app accepts both the English labels and the pinyin labels below:

| Action | English label | Pinyin label |
|---|---|---|
| Warmer light | `Warmer-light` | `nuan_yidian` |
| Cooler light | `Cooler-light` | `leng_yidian` |
| Brighter | `Brighter` | `liang_yidian` |
| Dimmer | `Dimmer` | `an_yidian` |

If the App Lab keyword runner logs show only:

```text
['background', 'hey_arduino', 'other']
```

then the default Hey Arduino model is being used, and the four command callbacks will not trigger.

## Setup on the UNO Q

Clone this branch on the Arduino UNO Q:

```bash
git clone -b learn https://github.com/Alex129xx/Qualcomm-Arduino-Edge-WS2026
cd Qualcomm-Arduino-Edge-WS2026
```

If you already cloned the repository, update it instead:

```bash
cd /home/arduino/Qualcomm-Arduino-Edge-WS2026
git checkout learn
git pull origin learn
```

Deploy the App Lab example with the USB microphone setup script:

```bash
sudo Software/setup-uno-q-usb-voice-matrix-applab.sh
```

This script checks for the USB microphone, removes an old `/etc/asound.conf` if present, and copies `Software/uno-q-echoglow` to:

```text
/home/arduino/ArduinoApps/uno-q-echoglow
```

The old analog microphone setup script has been removed from this branch because this version uses a USB microphone.

## How to Use the Example

1. Connect the USB microphone to the Arduino UNO Q.
2. Open Arduino App Lab.
3. Connect to the board using Network Mode.
4. Open `UNO Q Voice Matrix`.
5. Click the Play button in the top right corner.
6. Say one of the voice commands into the USB microphone.

| Keyword | LED Matrix Action |
|---|---|
| `Warmer-light` or `nuan_yidian` | Shows a sun icon |
| `Cooler-light` or `leng_yidian` | Shows a snowflake icon |
| `Brighter` or `liang_yidian` | Shows a plus icon |
| `Dimmer` or `an_yidian` | Shows a minus icon |
| No command | Keeps showing a heart icon |

## How it Works

### Python side (`python/main.py`)

- `ALSAMicrophone(device="plughw:CARD=Audio,DEV=0", shared=False)` uses the connected USB microphone.
- `KeywordSpotting(mic=mic)` initializes keyword spotting with that microphone.
- `spotter.on_detect(...)` registers callbacks for the English and pinyin labels.
- `Bridge.call(...)` notifies the microcontroller which command was detected.

### Microcontroller side (`sketch/sketch.ino`)

- `Arduino_LED_Matrix matrix;` controls the onboard LED matrix.
- `matrix.setGrayscaleBits(3)` enables 3-bit grayscale values from 0 to 7.
- The official LED Matrix Painter example treats the UNO Q onboard matrix as an 8-row x 13-column grayscale buffer.
- This sketch follows that drawing path and displays frames with `matrix.draw(frame)` instead of `matrix.renderBitmap(...)` or packed `loadFrame(...)` data.
- `Bridge.provide(...)` exposes handlers to the Python side.
- `warmer_light()`, `cooler_light()`, `brighter()`, and `dimmer()` show different LED matrix icons, then return to the idle heart icon.

## Notes

- The onboard LED matrix is monochrome, so `Warmer-light` and `Cooler-light` are represented by different icons instead of different colors.
- If App Lab cannot find the USB microphone, check the board with `arecord -l` and then verify the ALSA microphone device in `python/main.py`.
- If the icons still look rotated or mirrored on your board orientation, adjust the drawing helper functions in `sketch/sketch.ino`.
