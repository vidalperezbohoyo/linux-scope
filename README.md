# linux-scope
A embeeded linux sniper scope with several cool features.

## Compilation
Clone **fbg** library:

Generate static library:
cd /fbg
mkdir lib
gcc -c ../src/lodepng/lodepng.c ../src/nanojpeg/nanojpeg.c ../src/fbgraphics.c ../custom_backend/fbdev/fbg_fbdev.c -I../src -I. -I../custom_backend/fbdev -O2 -Wall
ar rcs lib.a lodepng.o nanojpeg.o fbgraphics.o fbg_fbdev.o

Clone this project:

Compile it:
