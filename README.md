# Vision-Based Target Tracking for RoboMaster Vehicle

## Project Overview

This project was developed as part of the RoboMasters robotics competition, which is a high-level robot combat contest. Participants design and build robots capable of completing assigned tasks, including automatic and manual targeting systems.

The system focuses on using computer vision to recognize and track red and blue targets in real time and control a dual-axis gimbal for aiming.

---

## Functional Goals

### Basic Functionality
- The system captures images from a camera and detects stationary or slow-moving enemy targets.
- On command, it locks onto the target and aims the gimbal accordingly.

### Extended Functionality
- Real-time tracking of moving targets even when both the friendly and enemy robots are in motion.
- The gimbal continuously follows the enemy target without manual intervention.
- Performance goal: achieve at least 20–30 FPS (i.e., <50ms per frame).
- The gimbal should stabilize within 3 oscillations when locking onto a target.

---

## System Design

### System Architecture

The system consists of three main modules:
1. **Image Recognition Module**
2. **Motion Control Module**
3. **Serial Communication Module**

A high-level overview:
- The camera captures images for the Image Recognition Module.
- The target's position relative to the image center is computed.
- The Motion Control Module maps image coordinates to relative angles for the gimbal.
- The Serial Communication Module encodes the angle data and transmits it to the gimbal controller.

### Image Recognition

- Two functions: **detection** and **tracking**.
- Detection is used at initialization or when tracking fails.
- Tracking is performed using the CamShift algorithm after a successful detection.

### Motion Control

- A linear function maps image coordinates to angles.
- Additional logic handles conditions like lock detection and gimbal stalling.

### Serial Communication

- Floats (angles) are encoded into byte streams.
- A checksum byte is appended to ensure transmission reliability.

---

## Design Alternatives

### Option 1: Detection Only
- Used in early official demos.
- Each frame is processed independently using color segmentation.
- Time complexity: O(n²) for connected domain construction.
- Sensitive to image noise and color complexity.

### Option 2: Detection + Tracking (Adopted)
- Uses detection for initialization and re-locking.
- CamShift for continuous tracking.
- Achieves ~25 FPS during testing.
- A Kalman filter variant was also tested, but not adopted due to limited time.

---

## Testing and Performance

- System reached 25 FPS with minimal gimbal oscillation.
- Accurately tracked targets in both static and dynamic scenes.

---

## Appendix: Source Code Structure

- `camshift.cpp` — Main application
- `Tracking.cpp / .h` — CamShift-based tracking logic
- `MyComClass.cpp / .h` — Serial communication class

---

## Dependencies

- OpenCV
- Windows Serial Communication API
- Standard C++ Libraries

---

## Build Instructions

1. Use Visual Studio (recommended) or g++ on Windows with appropriate serial libs.
2. Ensure OpenCV is installed and linked.
3. Compile all `.cpp` files in the `src/` directory.

---

## License

This project is provided for educational and research purposes. Please cite the original authors if reused.
