Michael Robertson
mirob2005@gmail.com, miro2005@csu.fullerton.edu
SID: 892-32-2629
Thesis Test Implementation using OpenGL and GLSL
Start Date: 5/31/12
Updated: 7/24/12


Assignment was completed using Visual Studio 2008 Pro on Windows 7 64-bit.
Included is all .cpp, .h, and .txt to independently run using any compiler.

All files must be in the same directory to be found.

* The project extends my Light_Wave implementation using OpenGL and GLSL rather than a raytracer.

* This test implementation will attempt to model the global illumination in the scene more like a wave rather than a point light source

* FPS is shown at the bottom of the screen


TODO:
1) (DONE) Add Shadows to the scene using shadow maps
2) Add commands to move objects in the scene
3) Add Light_Wave logic

COMMAND LINE (currently):

LIGHT_WAVE_GL.exe

-> No Additional commandline arguments needed
-> No Additional dependencies required, now using GLee to avoid the need to add GLEW32.lib

KEYBOARD CONTROLS:
ESC, 'q' - QUIT
UP ARROW - move light forward
DOWN ARROW - move light back
LEFT ARROW - move light to the left
RIGHT ARROW - move light to the right

shift+UP ARROW  = move light up
shift+DOWN ARROW  = move light down

W - move camera forward
S - move camera back
D - rotate camera to the right (pos rotation around y)
A - rotate camera to the left (neg rotation around y)
shift+W - move camera up
shift+S - move camera down

R - reset camera rotations
T - reset camera mvmts