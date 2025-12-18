# CocoaGDK (Game Development Kit)
CocoaGDK is a foundation layer to help with the creation of games.

## Philosophy of CocoaGDK
* Native solutions (Helps reduce size of this kit and provides better control)
* Portability (Built in C and encouraged to be ported to other languages for the users needs)
* Usage as a foundation and not a framework (Help encourage the user to get the first difficult aspects of game development out of the way)

## CocoaGDK Features
* Platform layer (Windowing, Input, Utils)
* Rolled math libraries (Vec2, Vec3, Mat4, Utils)
* OpenGL layer

## CocoaGDK Platforms
* MacOS
* Windows (Not fully supported)

## What is planned?
* Audio System (XAudio2, CoreAudio, ALSA)
* Socket System (Winsock, BSD)
* Graphics expansions (Vulkan, Metal, DirectX, WebGPU)
* Input expansions (Controllers, Joysticks, etc..)
* VR layer (OpenXR)

## What isn't planned?
* Scene graph/management (Implement your own, or integrate an existing library)
* Asset pipelines (Build whatever you feel fits)
* Physics engine (Too many edge cases, integrate or build your own)
* UI systems (Use the renderers or integrate one)

**Why?** These features are opinionated and are unnecessary to implement, other reasons may also include C being tricky to implement with, and are generally better off being implemented in other languages like C++
