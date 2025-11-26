FlightSim – Simple Multi-Aircraft Simulator and Collision Detection

FlightSim is a small simulation library written in C++ that simulates the motion of multiple aircraft in 2.5D space (x, y, altitude), performs fuel consumption, and collision detection.

The project is specifically designed to demonstrate **automated test writing**, **testable architecture**, and **TDD principles**.

## Features

- Modeling of multiple aircraft with:
- position (`x, y, z`),
- speed (`vx, vy`),
- heading (for informational purposes only),
- fuel amount

- Simple physics engine:
- Linear motion (position = position + speed * dt)
- Simple fuel consumption model (proportional to throttle and speed)
- Collision detection:
- Horizontal and vertical minimum distance limits
- Multiple collision detection simultaneously
- Automatic collision detection:
- Potential conflict list generated at each simulation step
- Autopilot / Waypoint tracking:
- Waypoint list giving the aircraft target altitude and speed
- Simple P-control for climb rate and throttle adjustment
- Switching to next target upon approaching waypoint
- Simple CLI demo:
- Head-on scenario (two aircraft approaching each other)
