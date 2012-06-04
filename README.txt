Michael Robertson
mirob2005@gmail.com, miro2005@csu.fullerton.edu
SID: 892-32-2629
Thesis Test Implementation using OpenGL and GLSL
Start Date: 5/31/12


Assignment was completed using Visual Studio 2008 Pro on Windows 7 64-bit.
Included is all .cpp, .h, and .txt to independently run using any compiler.

All files must be in the same directory to be found.

* The project extends my Light_Wave implementation using OpenGL and GLSL rather than a raytracer.

* This test implementation will attempt to model the global illumination in the scene more like a wave rather than a point light source


* RayTracer_GLSL_Compare.jpg shows current OpenGL output in comparison to the rayTracer output

* FPS is shown at the bottom of the screen


TODO:
1) Add Shadows to the scene using shadow maps
2) Add Light_Wave logic

COMMAND LINE (currently):

LIGHT_WAVE_GL.exe -x diffuse.vs -f diffuse.fs

KEYBOARD CONTROLS:
ESC, 'q' - QUIT
UP ARROW - move camera forward
DOWN ARROW - move camera back
LEFT ARROW - rotate camera to the left (neg rotation around y)
RIGHT ARROW - rotate camera to the right (pos rotation around y)

'r' - reset scene