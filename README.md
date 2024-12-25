# Minecraft-Revamped

## Description

Minecraft-Revamped is a Minecraft clone (that I am remaking because my last one was ... eh ... https://github.com/anakin004/Minecraft-Clone), developed in C++ with focus on performance optimization, multithreading, and real-time rendering.

## Features

- **Custom Terrain Generation:** Utilizing algorithms like 3d Simplex Noise, introducing caves compared to my previous clone, and gives computation benefits with perlin noise being more costly
- **Multithreading Support:** Efficiently handles terrain and chunk generation with thread pooling 
- **Rendering:** Uses OpenGL 
- **Testing:** Uses ImGui for runtime debuging, testing, and analysis
- **Networking:** Going to support multiplayer, currently looking at implimenting https://github.com/ValveSoftware/GameNetworkingSockets, I have some src files with netowrking already, I am using that as a refrence from my previous clone
- **Models:** Using assimp, my previous project handled model loading just fine for now

## Installation

### Prerequisites

- C++ compiler (MSVC recommended)
- glfw and assimp libraries (will grow)

##Demo
