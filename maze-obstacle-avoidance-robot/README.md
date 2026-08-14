# Maze Obstacle Avoidance Robot

An autonomous maze/obstacle-avoidance robot built for BRAC University's
CSE-350 course. A servo-mounted HC-SR04 ultrasonic sensor scans for walls and
obstacles, and the robot changes direction to keep moving through an unknown
maze without human input.

![Assembled robot chassis](images/robot-assembled.jpg)

## Overview

- **Sensing:** single HC-SR04 ultrasonic sensor on a servo, scanning
  center/right/left to judge clearance on each side
- **Drive:** two DC motors via an L298N dual H-bridge driver, differential
  steering
- **Control:** started on an Arduino Uno, moved to an **ESP32** for the final
  build after the Uno's Vin pin was damaged mid-project
- **Logic:** stop within a distance threshold, scan both sides, move toward
  the clearer path (see [Project Status](#project-status) — the final
  left/right decision step is not yet wired up)
- **Cost:** under $60 USD in components

## Project Status

This repo documents two stages of the same project:

| | Design stage | Final build |
|---|---|---|
| Doc | [`docs/DESIGN_DOCUMENT.md`](docs/DESIGN_DOCUMENT.md) | [`docs/PROJECT_REPORT.md`](docs/PROJECT_REPORT.md) |
| Microcontroller | Arduino Uno | ESP32 |
| Detail level | Full technical write-up: sensing math, servo scanning, motor logic, calibration, BOM | Final, as-tested IEEE-style report |

The reconstructed firmware in [`firmware/`](firmware/maze_solver_arduino) is
the Arduino version, since that's what the design document's code snippet
(and the surrounding pin-logic descriptions) came from. **The obstacle
scanning works, but the decision logic that compares left/right clearance
and actually steers the robot was never finished** — it's marked as a
`TODO` in the code rather than filled in with guessed behavior. See
["Challenge 4"](docs/DESIGN_DOCUMENT.md#implementation-challenges-and-solutions)
in the design doc for details, and [Future Work](#future-work) below.

## Hardware

| Component | Qty | Notes |
|---|---|---|
| Microcontroller | 1 | Arduino Uno (design) / ESP32 (final build) |
| Motor driver | 1 | L298N dual H-bridge |
| Ultrasonic sensor | 1 | HC-SR04, servo-mounted |
| Servo motor | 1 | Sweeps the sensor between 0°/90°/180° |
| DC geared motors | 2 | Low-RPM (~60 RPM) for controllability |
| Battery | 1 | 7.4V 2S LiPo (design) / 11.1V Li-ion (final build) |
| Chassis + caster wheel | 1 | Symmetrical, holds all components |
| Jumper wires + power switch | 1 set | |

Full rationale for each part is in the
[design document's BOM](docs/DESIGN_DOCUMENT.md#component-specification-bom).

## Circuit

<img src="images/circuit-diagram-arduino.jpeg" alt="Arduino circuit diagram" width="480">
<img src="images/circuit-diagram-esp32.png" alt="ESP32 circuit diagram" width="480">

Left: Arduino design-stage wiring. Right: final ESP32 wiring.

## Firmware

[`firmware/maze_solver_arduino/maze_solver_arduino.ino`](firmware/maze_solver_arduino/maze_solver_arduino.ino)

- Drives forward, checking distance every 500 ms (non-blocking, via `millis()`)
- Stops and scans right/left with the servo when an obstacle is within 15 cm
- Motor control functions (`moveForward`, `stopMotor`, `moveRight`,
  `moveLeft`) were reconstructed from the design document's pin-logic
  description, since the code pasted into the original report cut off
  before them — see the comments at the top of the file for exactly what
  was rebuilt vs. original.

**To run it:** open the `.ino` file in the Arduino IDE, install the
`Servo` library (bundled with the IDE), wire up per the circuit diagram
above, and upload to an Arduino Uno.

## Repo Structure

```
.
├── README.md                          <- you are here
├── docs/
│   ├── DESIGN_DOCUMENT.md             <- detailed Arduino-stage design report
│   └── PROJECT_REPORT.md              <- final ESP32-stage IEEE-style paper
├── firmware/
│   └── maze_solver_arduino/
│       └── maze_solver_arduino.ino    <- reconstructed, complete sketch
└── images/
    ├── robot-assembled.jpg
    ├── circuit-diagram-arduino.jpeg
    └── circuit-diagram-esp32.png
```

## Future Work

- Complete the left/right decision logic (compare `lval`/`rval`, steer
  toward the clearer side, handle both-blocked case)
- Port the firmware to the ESP32 as actually used in the final build (no
  ESP32 code was captured in either source report)
- Wireless monitoring over ESP32 Wi-Fi
- See the [design document](docs/DESIGN_DOCUMENT.md#future-improvements-and-extensions)
  for a longer list (PID motor control, sensor fusion, mapping, etc.)

## Team

- Sabiq Zahid — 22201283
- Imtiaz Salam Jami — 21301649
- Md. Nabil Mohtashim — 22201281

BRAC University, Department of Computer Science and Engineering — CSE-350

## License

No license has been specified yet. Add a `LICENSE` file (e.g. MIT) if you
want to make reuse terms explicit before others build on this.
