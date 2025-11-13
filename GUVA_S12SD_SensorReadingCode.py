# ----------------------------------------------------------------------------------
# OVERVIEW (my own notes)
# This code reads the GUVAS12SD UV sensor using a microcontroller's ADC.
# I'm using a Raspberry Pi Pico running MicroPython.
# I still have not tested this, as I do not have the physical device yet but this si generated code that should be able to read it. Further information is in the drive, component, sensors, and firt choice
# What it's doing:
# - Reads the analog voltage from the sensor on pin GP26 (ADC0).
# - Converts raw ADC (0–65535) into a real voltage using:
#       volts = (raw / 65535) * 3.3
#   because the Pico's ADC uses 3.3V as reference.
# - Converts voltage into UV Index using the sensor's approximation:
#       UV_Index = volts / 0.1
#   (so 0.5V ≈ UV Index 5)
# - Outputs voltage + UV index repeatedly so I can rotate the sensor
#   and see which angle gives the highest UV reading.
#
# If I change microcontrollers:
# - Change the ADC pin in machine.ADC(...)
# - Change ADC_MAX based on resolution (4095 for 12-bit, 65535 for read_u16)
# - Change VREF if the MCU uses a different analog reference voltage
#
# Recommended filename:
#       GUVA_S12SD_SensorReadingCode.py
# ----------------------------------------------------------------------------------

import machine
import time

# Using GP26 (ADC0) on the Raspberry Pi Pico
adc = machine.ADC(26)

# Sensor + Pico powered at 3.3V
VREF = 3.3

# read_u16() returns 0–65535 → 16-bit range
ADC_MAX = 65535

def read_uv(samples=10, delay=0.003):
    total = 0
    for _ in range(samples):
        total += adc.read_u16()      # raw ADC reading (0–65535)
        time.sleep(delay)

    avg_raw = total / samples

    # Convert to volts using:
    # volts = (avg_raw / ADC_MAX) * VREF
    volts = (avg_raw / ADC_MAX) * VREF

    # Convert volts → UV Index using:
    # UV ≈ volts / 0.1   (datasheet rule)
    uv_index = volts / 0.1

    return volts, uv_index


print("Reading GUVAS12SD UV sensor...")

while True:
    volts, uv = read_uv()
    print(f"Voltage: {volts:.3f} V    UV Index ≈ {uv:.2f}")
    time.sleep(0.5)
