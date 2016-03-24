CXX=g++
CC=gcc
CXXFLAGS=-ggdb `pkg-config --cflags --libs opencv`
CCFLAGS=-lGL -lGLU -lglut -lm
all: Rubix Config
Rubix:Rubix_main.o
	$(CC) $^ $(CCFLAGS) -o $@
Config:cubeconfig.o 
	$(CXX) $^ $(CXXFLAGS) -o $@

