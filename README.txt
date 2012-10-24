Michael Robertson
mirob2005@gmail.com, miro2005@csu.fullerton.edu
SID: 892-32-2629
Thesis Implementation using OpenGL and GLSL
Start Date: 5/31/12
Updated: 10/24/12


Assignment was completed using Visual Studio 2008 Pro on Windows 7 64-bit.
Included is all .cpp, .h, and .txt to independently run using any compiler.

All files must be in the same directory to be found.

* The project extends my Light_Wave implementation using OpenGL and GLSL rather than a raytracer.

* This test implementation will attempt to model the global illumination in the scene more like a wave rather than a point light source

* FPS is shown in the title bar


TODO:
1) (DONE) Add Shadows to the scene using shadow maps
2) (DONE) Add commands to move objects in the scene
3) (DONE) Add Light_Wave logic
4) (DONE) Add Specular
5) (DONE) Add Indirect Shadows
6) Code changes?
7) Thesis Writing

COMMAND LINE (currently):

LIGHT_WAVE_GL.exe

-> No Additional commandline arguments needed
-> No Additional dependencies required, now using GLee to avoid the need to add GLEW32.lib

KEYBOARD CONTROLS:
ESC, 'q' - QUIT
H - Help Menu
UP ARROW - move light forward
DOWN ARROW - move light back
LEFT ARROW - move light to the left
RIGHT ARROW - move light to the right

0,1,2 - select object: camera, sphere 1, sphere2, respectively (Default: camera)
W - move object forward
S - move object back
D - rotate object to the right (pos rotation around y)
A - rotate object to the left (neg rotation around y)
shift+W - move object up
shift+S - move object down

R - reset camera
T - reset spheres
G - Alternate between shaders on and shaders off with VPLs visible

I - Take SnapShot
M - Take continuous snapshots