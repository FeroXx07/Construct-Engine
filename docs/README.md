# Construct-Engine

## Description
A 3D game engine for the Game Enginge subject in CITM 2022 Game Dev course.
The engine is made by SDL, OpenGl and other 3rd party open source libraries.
All the licenses can be found in the license folder of the repository.

[Github repository](https://github.com/FeroXx07/Construct-Engine)

## Team

**Ali Hassan Shahid** - Solo developer

<img src="https://raw.githubusercontent.com/FeroXx07/Construct-Engine/main/docs/images/AliHassanShahidPhoto.jpg" width="100" height="150">

## Contents of the engine
The engine uses SDL + OpenGL environment, for the interface management it uses ImGUI and other 3rd parties libraries like Glew, JsonParse fro nlohmann and math
libraries like GLM and MathGeoLib.

## New contents, Physics Sub-System
The engine uses Bullet physics library to implement some basic collisions and forces managment. It has three primitive shapes: Box, Spehere and Cylinder
and three constraints available: Point to Point, Hinge and Slider.

![In-Editor](https://raw.githubusercontent.com/FeroXx07/Construct-Engine/main/docs/gifs/In_Editor.gif)

![In-Game](https://raw.githubusercontent.com/FeroXx07/Construct-Engine/main/docs/gifs/In_Game.gif)

# Video showcase

[![Alt text](https://img.youtube.com/vi/T6JH3_sUVHQ/0.jpg)](https://www.youtube.com/watch?v=T6JH3_sUVHQ)

## How to use? + Controls
- “WASD” fps-like movement
- While Right clicking free look around
- Mouse wheel zooms in and out.
- Pressing “f” focuses the camera to the geometry.
- Holding SHIFT duplicates movement speed.
- Drag and slide the in the component inspector window to change Transform data like Position, Scaling and Rotation.
- Press "L" key to delete the scene.
- Press "K" key to save the scene.
- Press "SPACE BAR" key to load the saved the scene.
- Press "1" key to shoot spheres when on execution.

## License
MIT License

Copyright (c) 2022 Ali Hassan Shahid

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
