# Disable Turn Animation

An SKSE plugin for Skyrim Special Edition (SE/AE).

## Overview
The primary purpose of this plugin is to **completely remove ALL turn animations** in Skyrim.

It intercepts and blocks vanilla turn animations (`TurnLeft`, `TurnRight` events) that cause the character to stumble and rotate their feet whenever you move the mouse. Furthermore, it **flawlessly handles tricky exception cases** that are normally difficult to intercept:
1. The brief transition moment when equipping or unequipping a weapon.
2. The moment when the character decelerates and comes to a complete stop.

Even during these exception timings, it injects `turnStop` events to smoothly prevent the character from playing unwanted turn animations under any circumstances.

## Requirements
- Skyrim Special Edition (1.5.97) or Anniversary Edition (1.6.x+)
- [SKSE64](https://skse.silverlock.org/)
- **[Address Library for SKSE Plugins](https://www.nexusmods.com/skyrimspecialedition/mods/32444)**: Required

## Installation
1. Install the archive via your preferred mod manager (MO2, Vortex, etc.), OR
2. Manually copy the `DisableTurnAnimation.dll` file into your `Data/SKSE/Plugins` folder.

## Compatibility
* **True Directional Movement (TDM)**: Fully compatible. The camera remains decoupled, and the plugin only intervenes in turn suppression logic.
* Built using **CommonLibSSE-NG**, meaning it natively supports both SE and AE versions out of the box (no specific version checking needed).

---

### Technical Details
This plugin is built using C++ and SKSE.
- It utilizes the **Address Library** via REL::ID to avoid hardcoded memory addresses, ensuring maximum compatibility across game versions.
- To ensure **Thread Safety** in a multithreaded environment, `std::atomic` variables are used to synchronize the state between the event graph and the main character update loop.

**Author**: Smooth
