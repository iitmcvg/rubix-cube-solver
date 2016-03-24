# rubix-cube-solver
A repository for Rubix cube solving tool.

## Source
cubeconfig.cpp
Reads in 6 images corresponding to faces of the cube and identifies the colour configuration of the cube

Rubix_main.c
Program to solve the rubix cube given a string of length 54 characters(composed of W,G,R,B,O,Y)

## Dependencies
OpenCV 3.0.0

## Trial
To compile Rubix_main.c run: 
gcc Rubix_main.c -lGL -lGLU -lglut -lm -o Rubix

If you don't have OpenGL and GLUT type `sudo apt-get install freeglut3-dev` to install everything you need

To compile cubeconfig.cpp run: 
```
$ g++ -ggdb cubeconfig.cpp `pkg-config --cflags --libs opencv`
```

Run:
```
$ ./RubiksSolver.sh
```
Then enter path of the 6 images of the cube in the shown order:


















Press spacebar to solve the cube
