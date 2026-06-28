# Copyright (c) 2026, Arduino. All rights reserved.
# This source code is licensed under the BSD-3-Clause license found in the
# LICENSE file in the root directory of this source tree.

from arduino.app_utils import *
from arduino.app_peripherals.microphone import ALSAMicrophone
from arduino.app_bricks.keyword_spotting import KeywordSpotting

# Light control commands

def warmer_light():
    print("Detected: Warmer-light", flush=True)
    Bridge.call("warmer_light")


def cooler_light():
    print("Detected: Cooler-light", flush=True)
    Bridge.call("cooler_light")


def dimmer():
    print("Detected: Dimmer", flush=True)
    Bridge.call("dimmer")


def brighter():
    print("Detected: Brighter", flush=True)
    Bridge.call("brighter")

# Main application
mic = ALSAMicrophone(device="plughw:CARD=Audio,DEV=0", shared=False)
spotter = KeywordSpotting(mic=mic)

# Register the callbacks for the detected keywords
spotter.on_detect("Warmer-light", warmer_light)
spotter.on_detect("nuan_yidian", warmer_light)

spotter.on_detect("Cooler-light", cooler_light)
spotter.on_detect("leng_yidian", cooler_light)

spotter.on_detect("Brighter", brighter)
spotter.on_detect("liang_yidian", brighter)

spotter.on_detect("Dimmer", dimmer)
spotter.on_detect("an_yidian", dimmer)

# Start the application
App.run()
