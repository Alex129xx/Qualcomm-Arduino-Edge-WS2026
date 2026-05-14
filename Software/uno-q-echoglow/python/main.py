# SPDX-FileCopyrightText: Copyright (C) Electronic Cats
#
# SPDX-License-Identifier: MPL-2.0

import subprocess

from arduino.app_utils import *
from arduino.app_peripherals.microphone import ALSAMicrophone
from arduino.app_bricks.keyword_spotting import KeywordSpotting


class _ArduinoQAnalogMic(ALSAMicrophone):
    """Re-applies the 9 ALSA mixer controls before every PCM open.

    The Qualcomm LPASS (Q6ASM) on the UNO Q drops the MultiMedia3 routing
    mixer to off on every PCM close, and WirePlumber's session manager
    applies the same reset shortly after boot. Without this re-init, the
    first open often succeeds but auto-reconnect retries fail with
    "Invalid argument [plughw:CARD=0,DEV=2]".
    """

    _MIXER_CONTROLS = (
        ("TX DEC0 MUX",                        "SWR_MIC"),
        ("TX SMIC MUX0",                       "SWR_MIC1"),
        ("ADC2 MUX",                           "INP2"),
        ("ADC2 Switch",                        "1"),
        ("ADC2 Volume",                        "8"),
        ("ADC2_MIXER Switch",                  "1"),
        ("TX_DEC0 Volume",                     "82"),
        ("TX_AIF1_CAP Mixer DEC0",             "1"),
        ("MultiMedia3 Mixer TX_CODEC_DMA_TX_3", "1"),
    )

    def _open_microphone(self) -> None:
        for name, value in self._MIXER_CONTROLS:
            subprocess.run(
                ["amixer", "-c", "0", "cset", f"name={name}", value],
                capture_output=True, check=False, timeout=2,
            )
        super()._open_microphone()


mic = _ArduinoQAnalogMic(device="plughw:CARD=ArduinoImolaHPH,DEV=2", shared=False)
spotter = KeywordSpotting(mic=mic)
spotter.on_detect("Warmer-light", lambda: Bridge.call("warmer_light"))
spotter.on_detect("Cooler-light", lambda: Bridge.call("cooler_light"))
spotter.on_detect("Dimmer", lambda: Bridge.call("dimmer"))
spotter.on_detect("Brighter", lambda: Bridge.call("brighter"))

App.run()
