# rubix-cube-solver
A repository for Rubix cube solving tool.

## Source
cubeconfig.cpp: 
Reads in 6 images corresponding to faces of the cube and identifies the colour configuration of the cube

Rubix_main.c: 
Program to solve the rubix cube given a string of length 54 characters(composed of W,G,R,B,O,Y)

## Dependencies
* OpenCV 3.0.0
* OpenGL
* GLUT

## Trial
If you don't have OpenGL and GLUT type `sudo apt-get install freeglut3-dev` to install everything you need

Run: 
```
$ make all
$./RubiksSolver.sh /path/to/1 /path/to/2 /path/to/3 /path/to/4 /path/to/5 /path/to/6
```

Enter path of the 6 images of the cube in the shown order:
<br><br>
<img src = "https://raw.githubusercontent.com/iitmcvg/rubix-cube-solver/master/Images/sample.jpg" width = "40%" />
<br><br>

Press spacebar to solve the cube

##Sample Run
```
$ make all
$ ./RubiksSolver.sh Images/1.jpg Images/2.jpg Images/3.jpg Images/4.jpg Images/5.jpg Images/6.jpg
```
Output:
<br><br>
<img src = "https://raw.githubusercontent.com/iitmcvg/rubix-cube-solver/master/review/Screenshot from 2016-03-24 22:29:21.png" width = "40%" />
<br><br>

  
