# Sequencer

## Overview
This project is a simple music sequencer designed to allow users to create and play back sequences of tones. The sequencer is based on the FRDM-KL05Z microcontroller platform and features an LCD display, a Waveshare speaker, and a 4x4 keypad for user interaction.

![image](https://github.com/user-attachments/assets/2ecb8c00-500e-47cc-bf09-9cbceacb3a55)

## Features
- **User-controlled sequence playback**
- **Recording mode for modifying sequences**
- **Adjustable BPM (tempo) when recording is disabled**
- **Volume control**
- **Supports different waveform types: sine, saw, and triangle**
- **Real-time feedback on an LCD display**

## How It Works
Upon powering on, the user can:
- Start the sequence
- Enter recording mode
- Clear the sequence
- Change the active sound
- Increase/decrease BPM (only when recording is off)
- Adjust volume

## Hardware Components
### LCD Display
Displays:
- Current recording status
- Playback tempo
- Active waveform type
- Sequence state
  
![image](https://github.com/user-attachments/assets/80e72eed-5c83-4ed4-aa9e-360f3cc4e499)

### Waveshare Speaker
- Outputs generated sound
- Allows for volume adjustments

![image](https://github.com/user-attachments/assets/916001d4-a4b6-4208-a3db-451a558f483f)

### 4x4 Keypad
- Used to control playback and recording
- First three keys: start sequence, toggle recording, and clear sequence
- Middle rows: modify sequence in recording mode, adjust BPM when recording is off
- Bottom row: select waveform type (sine, saw, triangle)

![image](https://github.com/user-attachments/assets/faf4177f-4628-43f0-8417-6d77e1b3f4a8)

### FRDM-KL05Z Microcontroller
- Main processing unit managing all functionalities

![image](https://github.com/user-attachments/assets/16d1545b-c2dc-41c5-90d1-df629c6cb81e)

## Source Files
- **main.c** - Main program file, handles initialization and interrupts.
- **Sequencer.c / .h** - Manages sequence logic, timing, and button handling.
- **Display.c / .h** - Updates and controls the LCD screen.
- **klaw.c / .h** - Manages keypad inputs.
- **DAC.c / .h** - Initializes and controls the digital-to-analog converter.
- **i2c.c / .h** - Handles I2C communication.
- **lcd1602.c / .h** - Controls the LCD module.
- **leds.c / .h** - Debugging utility using onboard LEDs.

## Planned Improvements
- Expand waveform selection options
- Implement an option to save/load sequences
- Add an external storage option for extended sequences
- Improve graphical interface for LCD display

## Author
Jakub Konior

## License
This project is open-source and available for educational use.

