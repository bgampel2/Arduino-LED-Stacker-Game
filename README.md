# Arduino Stacker Game
A fully featured Stacker arcade game built with Arduino Uno and four 8x8 LED matrix displays.
[![Stacker Game Demo](https://img.youtube.com/vi/-oeyFDvVNB8/0.jpg)](https://youtube.com/shorts/-oeyFDvVNB8)

## About
Stacker is a classic arcade game where the player must stack blocks as they move across 
the screen, trying to align each layer perfectly with the one below. This project recreates 
that experience on a tower of four 8x8 LED matrix displays controlled by an Arduino Uno.

Built from scratch in C++ using the LedControl library, the game features custom display 
mapping, bitmap font rendering, procedural animations, and reactive sound effects through 
a passive buzzer — all running on a single Arduino Uno.

## Features
- 4x 8x8 LED matrix display tower (32x8 pixel play field)
- Smooth block movement with wall bouncing
- Block trimming mechanic — only overlapping dots survive each stack
- Progressive speed increase as you stack higher
- Random direction changes after row 10 to increase difficulty
- Block shrinks every 10 consecutive perfect stacks
- Hit flash animation when you lose a dot
- Startup animation — random pixel fill on power on
- Game over animation — ominous snake crawl with GAME OVER text
- Win animation — celebratory snake with WIN text
- Passive buzzer sound effects for stack, perfect, hit, win, and game over
- 8x8 bitmap font library for all 26 letters

## Components
- Arduino Uno
- 4x MAX7219 8x8 LED Matrix Display Modules
- 1x Passive Buzzer
- 1x Push Button
- Jumper Wires
- USB Cable (for power and programming)

## Wiring

### LED Matrix Displays
The four displays are daisy-chained together and connected to the Arduino as follows:

| MAX7219 Pin | Arduino Pin |
|-------------|-------------|
| VCC         | 5V          |
| GND         | GND         |
| DIN         | Pin 12      |
| CLK         | Pin 10      |
| CS          | Pin 11      |

The displays are chained by connecting the DOUT of each display to the DIN of the next 
one. Only the first display in the chain connects to the Arduino.

### Button
| Button Pin | Arduino Pin |
|------------|-------------|
| Pin 1      | Pin 2       |
| Pin 2      | GND         |

The button uses the Arduino's internal pull-up resistor (INPUT_PULLUP) so no external 
resistor is needed.

### Passive Buzzer
| Buzzer Pin | Arduino Pin |
|------------|-------------|
| Positive   | Pin 8       |
| Negative   | GND         |

## Dependencies

### LedControl
- **Install:** Arduino IDE → Sketch → Include Library → Manage Libraries → search "LedControl"
- **URL:** https://www.arduino.cc/reference/en/libraries/ledcontrol/

### pitches.h
- Not a library — just a header file containing note frequency definitions
- Add pitches.h as a second tab in your Arduino sketch
- **Download:** https://gist.github.com/mikeputnam/2820675

## How to Run
1. Install the LedControl library via Arduino IDE Library Manager
2. Download pitches.h and add it as a second tab in your Arduino sketch
3. Wire all components as described above
4. Upload STACKERGAME_v2.ino to your Arduino Uno
5. The startup animation plays automatically on power on

## How to Play
- Press the button to stack the moving block onto the previous row
- Only the dots that overlap with the row below are kept
- The block speeds up as you stack higher
- After row 10, the block may randomly reverse direction
- Every 10 consecutive perfect stacks your block shrinks by 1 dot
- Stack all 32 rows to win

## Author
Made by Bernie Gampel — Computer Engineering Student, March 2026  
GitHub: [bgampel2](https://github.com/bgampel2)
