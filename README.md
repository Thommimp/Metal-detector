# Metal-detector

Arduino code for metal detector project.

## Overview

**Note:** This repository contains only the Arduino code for a DFT (Discrete Fourier Transform) metal detector as part of a larger hardware project. It does not include hardware schematics, enclosure design, or other components—just the code that runs on the microcontroller.

The code demonstrates how to use an Arduino to detect metal objects based on changes in signal frequency and amplitude, using DFT-based signal processing.

## Features

- **DFT Metal Detection:** Implements a metal detector algorithm using Discrete Fourier Transform principles.
- **Arduino Compatibility:** Designed to run on standard Arduino boards.
- **Signal Processing:** Analyzes coil signals to identify metallic objects.
- **Simple Hardware Interface:** Uses basic components like coils, resistors, and capacitors.

## How It Works

The metal detector works by sending a signal through a coil and measuring the response. When a metallic object is near the coil, it changes the signal's frequency and amplitude. The Arduino samples these signals and processes them using a DFT algorithm to detect these changes, providing a simple indication (e.g., LED or buzzer) when metal is detected.


## Author

Thomas Pedersen
