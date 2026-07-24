# Arduino-hybrid-obstacle-car
Hybrid autonomous/manual obstacle-avoiding robot with Arduino
# Hybrid Obstacle-Avoiding Robot

A two-wheel robot car built on an Arduino UNO that runs in two modes:
- **Autonomous** — drives forward, detects obstacles with an ultrasonic sensor, 
  stops, backs up, and turns to avoid them.
- **Manual** — driven directly via an IR remote control.

A single button on the remote toggles between the two modes at any time.

## Status: In Progress

- ✅ Chassis assembled (two-layer 2WD kit)
- ✅ Motors confirmed working via standalone test sketch
- ✅ Ultrasonic sensor confirmed giving correct distance readings standalone
- ✅ Autonomous obstacle-avoidance logic written and tested
- ✅ IR remote codes mapped (see table below)
- ✅ Combined autonomous + manual sketch
- 🔲 GitHub demo video - in process

This is being built and documented in public as it's developed reflecting on 
real progress and real bugs, not a finished product.

## Hardware

| Component | Source |
|---|---|
| Arduino UNO R3 | Elegoo Super Starter Kit |
| L293D Motor Driver IC | Elegoo Super Starter Kit |
| HC-SR04 Ultrasonic Sensor | Elegoo Super Starter Kit |
| IR Receiver + Remote | Elegoo Super Starter Kit |
| 2x TT DC Motors, 2x drive wheels, 1x caster wheel, battery box | 2WD chassis kit |

## Pin Map

| Pin | Function |
|---|---|
| 8, 9 | Right motor direction |
| 10 | Right motor speed (PWM) |
| 12, 13 | Left motor direction |
| 11 | Left motor speed (PWM) |
| 5 | Ultrasonic trigger |
| 6 | Ultrasonic echo |
| 4 | IR receiver signal |

## Manual Mode — Remote Button Map

| Remote Button | Function | IR Code |
|---|---|---|
| Top-left red | Toggle Autonomous/Manual | `0xBA45FF00` |
| 2 | Forward | `0xE718FF00` |
| 8 | Backward | `0xAD52FF00` |
| 4 | Turn Left | `0xF708FF00` |
| 6 | Turn Right | `0xA55AFF00` |
| 5 | Stop | `0xE31CFF00` |

## How It Works

**Autonomous mode:** continuously measures distance via the ultrasonic sensor. 
If the path is clear, drives forward. If an obstacle is within 15cm, stops, 
reverses briefly, picks a random direction, turns, and resumes.

**Manual mode:** motors are driven directly by remote button presses with no 
sensor input used.

**Mode switching:** the IR receiver is checked every loop cycle regardless of 
current mode. One dedicated button flips a mode flag; all other buttons only 
act while in manual mode.

## Known Issues / Debug Log

- July 14 — Original combined sketch: motors confirmed working standalone and 
  sensor confirmed working standalone, but motors did not move when both 
  subsystems ran together. Suspected cause: shared power rail between motors 
  and Arduino logic causing brownout under motor current draw — not yet confirmed.
- July 16 — Rewrote combined sketch from scratch with IR mode-switching added. 
  Testing in progress.

## Software

- Arduino IDE
- [IRremote library](https://github.com/Arduino-IRremote/Arduino-IRremote) 
  by Armin Joachimsmeyer (v3.x/4.x)


## Roadmap (V2)

- Servo-mounted scanning sensor for smarter turn decisions (instead of random)
- LCD1602 display showing current mode and live distance reading
