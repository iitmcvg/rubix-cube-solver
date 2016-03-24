/*
    Rubik's cube solver
    Copyright (C) 2012 Alessio Serraino

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    For any question you can email me: serrainoalessio@gmail.com

*/


#define STARTFULLSCREEN 1



#define KEY_TOGGLEFULLSCREEN 'k'
#define KEY_STOP             'q'


#if (KEY_TOGGLEFULLSCREEN == 'u'|| KEY_TOGGLEFULLSCREEN == 'U' ||       \
     KEY_TOGGLEFULLSCREEN == 'd'|| KEY_TOGGLEFULLSCREEN == 'D' ||       \
     KEY_TOGGLEFULLSCREEN == 'l'|| KEY_TOGGLEFULLSCREEN == 'L' ||       \
     KEY_TOGGLEFULLSCREEN == 'r'|| KEY_TOGGLEFULLSCREEN == 'R' ||       \
     KEY_TOGGLEFULLSCREEN == 'f'|| KEY_TOGGLEFULLSCREEN == 'F' ||       \
     KEY_TOGGLEFULLSCREEN == 'b'|| KEY_TOGGLEFULLSCREEN == 'B' )

#error "Toggle fullscreen command conflicts with another command"
#endif
#if (KEY_STOP == 'u'|| KEY_STOP == 'U' ||                               \
     KEY_STOP == 'd'|| KEY_STOP == 'D' ||                               \
     KEY_STOP == 'l'|| KEY_STOP == 'L' ||                               \
     KEY_STOP == 'r'|| KEY_STOP == 'R' ||                               \
     KEY_STOP == 'f'|| KEY_STOP == 'F' ||                               \
     KEY_STOP == 'b'|| KEY_STOP == 'B' )

#error "Stop command conflicts with another command"
#endif

#ifdef WIN32
    #define WIN_OS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if (defined KEY_TOOGLEFULLSCREEN) || (defined KEY_STOP)
    #include <ctype.h>
#endif
#include <math.h>
#include <time.h>

#ifdef WIN_OS
    #include <windows.h>
#endif
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <unistd.h>

#define ESCAPE 27

#define bool  int
#define true  1
#define false 0
#define bits 32
#define PI 3.14159265359


#define BK_RED    0.00
#define BK_GREEN  0.00
#define BK_BLUE   0.00


#define RED       0xFF0000
#define ORANGE    0xFF7700
#define YELLOW    0xFFFF00
#define GREEN     0x00FF00
#define LIGHTBLUE 0x00FFFF
#define BLUE      0x0000FF
#define FUCSIA    0xFF00FF
#define BLACK     0x000000
#define DARKGRAY  0x555555
#define WHITE     0xFFFFFF




#define ANY_COLOR 0x7

#define COLOR(x) ((colors >> (x<<2)) & 0x0000000F)

#define DIR_UP                  1
#define DIR_DOWN                2
#define DIR_LEFT                3
#define DIR_RIGHT               4
#define CLOCKWISE               5
#define COUNTERCLOCKWISE        6
#define S_180                   7
#define NONE                    0


#define FACE_UP    1
#define FACE_DOWN  2
#define FACE_LEFT  3
#define FACE_RIGHT 4
#define FACE_FRONT 5
#define FACE_BACK  6


#define MOVE_NONE 0
#define MOVE_U  0x10
#define MOVE_D  0x11
#define MOVE_F  0x12
#define MOVE_B  0x13
#define MOVE_L  0x14
#define MOVE_R  0x15
#define MOVE_Ui 0x30
#define MOVE_Di 0x31
#define MOVE_Fi 0x32
#define MOVE_Bi 0x33
#define MOVE_Li 0x34
#define MOVE_Ri 0x35
#define MOVE_U2 0x20
#define MOVE_D2 0x21
#define MOVE_F2 0x22
#define MOVE_B2 0x23
#define MOVE_L2 0x24
#define MOVE_R2 0x25

#ifndef WIN_OS

#define TXI_COLOR_BLACK   "\033[90m"
#define TXI_COLOR_RED     "\033[91m"
#define TXI_COLOR_GREEN   "\033[92m"
#define TXI_COLOR_YELLOW  "\033[93m"
#define TXI_COLOR_BLUE    "\033[94m"
#define TXI_COLOR_MAGENTA "\033[95m"
#define TXI_COLOR_CYAN    "\033[96m"
#define TXI_COLOR_WHITE   "\033[97m"

#define BKI_COLOR_BLACK   "\033[100m"
#define BKI_COLOR_RED     "\033[101m"
#define BKI_COLOR_GREEN   "\033[102m"
#define BKI_COLOR_YELLOW  "\033[103m"
#define BKI_COLOR_BLUE    "\033[104m"
#define BKI_COLOR_MAGENTA "\033[105m"
#define BKI_COLOR_CYAN    "\033[106m"
#define BKI_COLOR_WHITE   "\033[107m"

#define TX_COLOR_BLACK   "\033[30m"
#define TX_COLOR_RED     "\033[31m"
#define TX_COLOR_GREEN   "\033[32m"
#define TX_COLOR_YELLOW  "\033[33m"
#define TX_COLOR_BLUE    "\033[34m"
#define TX_COLOR_MAGENTA "\033[35m"
#define TX_COLOR_CYAN    "\033[36m"
#define TX_COLOR_WHITE   "\033[37m"

#define BK_COLOR_BLACK   "\033[40m"
#define BK_COLOR_RED     "\033[41m"
#define BK_COLOR_GREEN   "\033[42m"
#define BK_COLOR_YELLOW  "\033[43m"
#define BK_COLOR_BLUE    "\033[44m"
#define BK_COLOR_MAGENTA "\033[45m"
#define BK_COLOR_CYAN    "\033[46m"
#define BK_COLOR_WHITE   "\033[47m"

#define COLOR_DEFAULT    "\033[0;0m"

#endif


int window;

struct __face {
    unsigned int center: 3;
    unsigned int n: 3;
    unsigned int s: 3;
    unsigned int e: 3;
    unsigned int w: 3;
    unsigned int nw: 3;
    unsigned int ne: 3;
    unsigned int se: 3;
    unsigned int sw: 3;
};
struct __cube {
    struct __face up;
    struct __face down;
    struct __face left;
    struct __face right;
    struct __face front;
    struct __face back;
};
typedef struct __cube cube;

cube MainCube;
char * moves = NULL;
char * movesbuffer = NULL;
int movesbufferpos = 0;
int colors;


GLfloat LightAmbient[]  = {0.0f, 0.0f, 0.0f, 0.0f};
GLfloat LightDiffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat LightPosition[] = {3.0f, 4.0f, 5.0f, 1.0f};

GLfloat xAngle = 30.0f,
        yAngle = 40.0f,
        xAngleIncrement = 0.0f,
        yAngleIncrement = 0.0f;
int oldx, oldy;
int incx, incy;
int leftdown;

int CurrentMove = MOVE_NONE;
float INCREMENT = 10.0f;

bool fullscreen;
bool moving;


char* mystrcat(const char*, const char*);
char* m_reverse(const char*);
int rotate_face(struct __face *, int);
int rotate_cube(cube *, int);
int do_move(cube *, int);
int do_moves(cube *, char *);
int is_of_type(cube, char *);
char getcolor(int);
int getcolor_i(int);
int getid(char);
void cube_to_string(char *, cube);
void type_to_string(char *, cube);
void type_from_string(cube *, const char *);
int cube_from_string(cube *, const char *);
void getcube(cube *);
void new_cube(cube *, char * s);
void optimize();
int moveslen();
int mov_len(const char*);
void scramble(cube*, int);
int not_found_solution();
int solve(cube);

void NextMove();

char *mystrcat(const char* str1, const char* str2){
    char *newstr;
    int i,
        newSize;
    i = (str1 != NULL)?strlen(str1):0;
    newSize = sizeof(char)*((str2 != NULL)?(strlen(str2)+i):(i));
    if((newstr =  (char *)realloc(NULL, newSize + 1)) == NULL){
        fprintf(stderr, "Error: Could not allocate desired size");
        abort();
    }
    if (str1 != NULL)
        strcpy(newstr, str1);
    if (str2 != NULL)
        strcpy((char*)((int)newstr + i), str2);
    newstr[newSize] = '\0';

    return newstr;
}
char * m_reverse(const char * in){
    char * movelist;
    movelist = (char*)malloc(sizeof(char));
    movelist[0] = '\0';
    int pos = 0;
    while(in[pos] != '\0'){
        if (in[pos+1] == '2'){
                switch(in[pos]){
                case 'U': movelist = mystrcat("U2", movelist); break;
                case 'D': movelist = mystrcat("D2", movelist); break;
                case 'L': movelist = mystrcat("L2", movelist); break;
                case 'R': movelist = mystrcat("R2", movelist); break;
                case 'F': movelist = mystrcat("F2", movelist); break;
                case 'B': movelist = mystrcat("B2", movelist);
                }
                pos+=2;
        } else if (in[pos + 1] == '\'' || in[pos+1] == 'i'){
                switch(in[pos]){
                case 'U': movelist = mystrcat("U", movelist); break;
                case 'D': movelist = mystrcat("D", movelist); break;
                case 'L': movelist = mystrcat("L", movelist); break;
                case 'R': movelist = mystrcat("R", movelist); break;
                case 'F': movelist = mystrcat("F", movelist); break;
                case 'B': movelist = mystrcat("B", movelist);
                }
                pos+=2;
        } else {
                switch(in[pos]){
                case 'U': movelist = mystrcat("U'", movelist); break;
                case 'D': movelist = mystrcat("D'", movelist); break;
                case 'L': movelist = mystrcat("L'", movelist); break;
                case 'R': movelist = mystrcat("R'", movelist); break;
                case 'F': movelist = mystrcat("F'", movelist); break;
                case 'B': movelist = mystrcat("B'", movelist);
                }
                pos++;
        }
    }
    return movelist;
}
void rot_moves(char * moveslist, int direction){
    int k, len = strlen(moveslist);
    switch (direction){
        case DIR_UP:
        for (k = 0; k < len; k++){
            switch (moveslist[k]){
                case 'U':
                moveslist[k] = 'B';
                break;
                case 'D':
                moveslist[k] = 'F';
                break;

                case 'F':
                moveslist[k] = 'U';
                break;
                case 'B':
                moveslist[k] = 'D';
                break;
                default:
                break;
            }
        }
        break;
        case DIR_DOWN:
        for (k = 0; k < len; k++){
            switch (moveslist[k]){
                case 'U':
                moveslist[k] = 'F';
                break;
                case 'D':
                moveslist[k] = 'B';
                break;

                case 'F':
                moveslist[k] = 'D';
                break;
                case 'B':
                moveslist[k] = 'U';
                break;
                default:
                break;
            }
        }
        break;
        case DIR_LEFT:
        for (k = 0; k < len; k++){
            switch (moveslist[k]){

                case 'L':
                moveslist[k] = 'B';
                break;
                case 'R':
                moveslist[k] = 'F';
                break;
                case 'F':
                moveslist[k] = 'L';
                break;
                case 'B':
                moveslist[k] = 'R';
                break;
                default:
                break;
            }
        }
        break;
        case DIR_RIGHT:
        for (k = 0; k < len; k++){
            switch (moveslist[k]){

                case 'L':
                moveslist[k] = 'F';
                break;
                case 'R':
                moveslist[k] = 'B';
                break;
                case 'F':
                moveslist[k] = 'R';
                break;
                case 'B':
                moveslist[k] = 'L';
                break;
                default:
                break;
            }
        }
        break;
        case CLOCKWISE:
        for (k = 0; k < len; k++){
            switch (moveslist[k]){
                case 'U':
                moveslist[k] = 'R';
                break;
                case 'D':
                moveslist[k] = 'L';
                break;
                case 'L':
                moveslist[k] = 'U';
                break;
                case 'R':
                moveslist[k] = 'D';
                break;

                default:
                break;
            }
        }
        break;
        case COUNTERCLOCKWISE:
        for (k = 0; k < len; k++){
            switch (moveslist[k]){
                case 'U':
                moveslist[k] = 'L';
                break;
                case 'D':
                moveslist[k] = 'R';
                break;
                case 'L':
                moveslist[k] = 'D';
                break;
                case 'R':
                moveslist[k] = 'U';
                break;

                default:
                break;
            }
        }
        break;
        default:
        return;
    }
}
int rotate_face(struct __face * f, int direction){
        switch(direction){
        case CLOCKWISE:{
                struct __face tmpface;
                tmpface.center = f->center;
                tmpface.n = f->w;
                tmpface.s = f->e;
                tmpface.e = f->n;
                tmpface.w = f->s;
                tmpface.ne = f->nw;
                tmpface.nw = f->sw;
                tmpface.se = f->ne;
                tmpface.sw = f->se;
                (*f) = tmpface;
        }
        break;
        case COUNTERCLOCKWISE:{
                struct __face tmpface;
                tmpface.center = f->center;
                tmpface.n = f->e;
                tmpface.s = f->w;
                tmpface.e = f->s;
                tmpface.w = f->n;
                tmpface.ne = f->se;
                tmpface.nw = f->ne;
                tmpface.se = f->sw;
                tmpface.sw = f->nw;
                (*f) = tmpface;
        }
        break;
        case S_180:{
                struct __face tmpface;
                tmpface.center = f->center;
                tmpface.n = f->s;
                tmpface.s = f->n;
                tmpface.e = f->w;
                tmpface.w = f->e;
                tmpface.ne = f->sw;
                tmpface.nw = f->se;
                tmpface.se = f->nw;
                tmpface.sw = f->ne;
                (*f) = tmpface;
        }
        }
        return 0;
}
int rotate_cube(cube * c, int direction){
        switch(direction){
        case DIR_UP:{
                struct __face tmpface;
                cube tmpcube;
                tmpcube.up = c->front;
                tmpcube.front = c->down;
                tmpface = c->back;
                rotate_face(&tmpface, S_180);
                tmpcube.down = tmpface;
                tmpface = c->up;
                rotate_face(&tmpface, S_180);
                tmpcube.back = tmpface;
                tmpface = c->right;
                rotate_face(&tmpface, CLOCKWISE);
                tmpcube.right = tmpface;
                tmpface = c->left;
                rotate_face(&tmpface, COUNTERCLOCKWISE);
                tmpcube.left = tmpface;
                (*c) = tmpcube;
        }
        break;
        case DIR_DOWN:{
                struct __face tmpface;
                cube tmpcube;
                tmpcube.down = c->front;
                tmpcube.front = c->up;
                tmpface = c->back;
                rotate_face(&tmpface, S_180);
                tmpcube.up = tmpface;
                tmpface = c->down;
                rotate_face(&tmpface, S_180);
                tmpcube.back = tmpface;
                tmpface = c->right;
                rotate_face(&tmpface, COUNTERCLOCKWISE);
                tmpcube.right = tmpface;
                tmpface = c->left;
                rotate_face(&tmpface, CLOCKWISE);
                tmpcube.left = tmpface;
                (*c) = tmpcube;
        }
        break;
        case DIR_LEFT:{
                struct __face tmpface;
                cube tmpcube;
                tmpcube.front = c->right;
                tmpcube.left = c->front;
                tmpcube.back = c->left;
                tmpcube.right = c->back;
                tmpface = c->up;
                rotate_face(&tmpface, CLOCKWISE);
                tmpcube.up = tmpface;
                tmpface = c->down;
                rotate_face(&tmpface, COUNTERCLOCKWISE);
                tmpcube.down = tmpface;
                (*c) = tmpcube;
        }
        break;
        case DIR_RIGHT:{
                struct __face tmpface;
                cube tmpcube;
                tmpcube.front = c->left;
                tmpcube.left = c->back;
                tmpcube.back = c->right;
                tmpcube.right = c->front;
                tmpface = c->up;
                rotate_face(&tmpface, COUNTERCLOCKWISE);
                tmpcube.up = tmpface;
                tmpface = c->down;
                rotate_face(&tmpface, CLOCKWISE);
                tmpcube.down = tmpface;
                (*c) = tmpcube;
        } break;
        case CLOCKWISE:{
                struct __face tmpface;
                cube tmpcube;
                tmpface = c->front;
                rotate_face(&tmpface, CLOCKWISE);
                tmpcube.front = tmpface;
                tmpface = c->back;
                rotate_face(&tmpface, COUNTERCLOCKWISE);
                tmpcube.back = tmpface;
                tmpface = c->left;
                rotate_face(&tmpface, CLOCKWISE);
                tmpcube.up = tmpface;
                tmpface = c->up;
                rotate_face(&tmpface, CLOCKWISE);
                tmpcube.right = tmpface;
                tmpface = c->right;
                rotate_face(&tmpface, CLOCKWISE);
                tmpcube.down = tmpface;
                tmpface = c->down;
                rotate_face(&tmpface, CLOCKWISE);
                tmpcube.left = tmpface;
                (*c) = tmpcube;
        } break;
        case COUNTERCLOCKWISE:{
                struct __face tmpface;
                cube tmpcube;
                tmpface = c->front;
                rotate_face(&tmpface, COUNTERCLOCKWISE);
                tmpcube.front = tmpface;
                tmpface = c->back;
                rotate_face(&tmpface, CLOCKWISE);
                tmpcube.back = tmpface;
                tmpface = c->right;
                rotate_face(&tmpface, COUNTERCLOCKWISE);
                tmpcube.up = tmpface;
                tmpface = c->down;
                rotate_face(&tmpface, COUNTERCLOCKWISE);
                tmpcube.right = tmpface;
                tmpface = c->left;
                rotate_face(&tmpface, COUNTERCLOCKWISE);
                tmpcube.down = tmpface;
                tmpface = c->up;
                rotate_face(&tmpface, COUNTERCLOCKWISE);
                tmpcube.left = tmpface;
                (*c) = tmpcube;
        }
        }
        return 0;
}
int do_move(cube * c, int move){

    switch(move){
    case MOVE_U:{
        cube newcube;
        newcube = (*c);
        rotate_face(&(newcube.up), CLOCKWISE);
        newcube.front.nw = c->right.nw;
        newcube.front.n  = c->right.n;
        newcube.front.ne = c->right.ne;
        newcube.left.nw  = c->front.nw;
        newcube.left.n   = c->front.n;
        newcube.left.ne  = c->front.ne;
        newcube.back.nw  = c->left.nw;
        newcube.back.n   = c->left.n;
        newcube.back.ne  = c->left.ne;
        newcube.right.nw = c->back.nw;
        newcube.right.n  = c->back.n;
        newcube.right.ne = c->back.ne;
        (*c) = newcube;
        moves = mystrcat(moves, "U");
    } break;
    case MOVE_Ui:{
        cube newcube;
        newcube = (*c);
        rotate_face(&(newcube.up), COUNTERCLOCKWISE);
        newcube.front.nw = c->left.nw;
        newcube.front.n  = c->left.n;
        newcube.front.ne = c->left.ne;
        newcube.left.nw  = c->back.nw;
        newcube.left.n   = c->back.n;
        newcube.left.ne  = c->back.ne;
        newcube.back.nw  = c->right.nw;
        newcube.back.n   = c->right.n;
        newcube.back.ne  = c->right.ne;
        newcube.right.nw = c->front.nw;
        newcube.right.n  = c->front.n;
        newcube.right.ne = c->front.ne;
        (*c) = newcube;
        moves = mystrcat(moves, "U'");
    } break;
    case MOVE_U2:{
        cube newcube;
        newcube = (*c);
        rotate_face(&(newcube.up), S_180);
        newcube.front.nw = c->back.nw;
        newcube.front.n  = c->back.n;
        newcube.front.ne = c->back.ne;
        newcube.left.nw  = c->right.nw;
        newcube.left.n   = c->right.n;
        newcube.left.ne  = c->right.ne;
        newcube.back.nw  = c->front.nw;
        newcube.back.n   = c->front.n;
        newcube.back.ne  = c->front.ne;
        newcube.right.nw = c->left.nw;
        newcube.right.n  = c->left.n;
        newcube.right.ne = c->left.ne;
        (*c) = newcube;
        moves = mystrcat(moves, "U2");
    } break;
    case MOVE_D:{
        cube newcube;
        newcube = (*c);
        rotate_face(&(newcube.down), CLOCKWISE);
        newcube.front.sw = c->left.sw;
        newcube.front.s  = c->left.s;
        newcube.front.se = c->left.se;
        newcube.left.sw  = c->back.sw;
        newcube.left.s   = c->back.s;
        newcube.left.se  = c->back.se;
        newcube.back.sw  = c->right.sw;
        newcube.back.s   = c->right.s;
        newcube.back.se  = c->right.se;
        newcube.right.sw = c->front.sw;
        newcube.right.s  = c->front.s;
        newcube.right.se = c->front.se;
        (*c) = newcube;
        moves = mystrcat(moves, "D");
    } break;
    case MOVE_Di:{
        cube newcube;
        newcube = (*c);
        rotate_face(&(newcube.down), COUNTERCLOCKWISE);
        newcube.front.sw = c->right.sw;
        newcube.front.s  = c->right.s;
        newcube.front.se = c->right.se;
        newcube.left.sw  = c->front.sw;
        newcube.left.s   = c->front.s;
        newcube.left.se  = c->front.se;
        newcube.back.sw  = c->left.sw;
        newcube.back.s   = c->left.s;
        newcube.back.se  = c->left.se;
        newcube.right.sw = c->back.sw;
        newcube.right.s  = c->back.s;
        newcube.right.se = c->back.se;
        (*c) = newcube;
        moves = mystrcat(moves, "D'");
    } break;
    case MOVE_D2:{
        cube newcube;
        newcube = (*c);
        rotate_face(&(newcube.down), S_180);
        newcube.front.sw = c->back.sw;
        newcube.front.s  = c->back.s;
        newcube.front.se = c->back.se;
        newcube.left.sw  = c->right.sw;
        newcube.left.s   = c->right.s;
        newcube.left.se  = c->right.se;
        newcube.back.sw  = c->front.sw;
        newcube.back.s   = c->front.s;
        newcube.back.se  = c->front.se;
        newcube.right.sw = c->left.sw;
        newcube.right.s  = c->left.s;
        newcube.right.se = c->left.se;
        (*c) = newcube;
        moves = mystrcat(moves, "D2");
    } break;
    case MOVE_R:{
        cube newcube;
        newcube = (*c);
        rotate_face(&(newcube.right), CLOCKWISE);
        newcube.up.ne = c->front.ne;
        newcube.up.e  = c->front.e;
        newcube.up.se = c->front.se;
        newcube.front.ne = c->down.ne;
        newcube.front.e  = c->down.e;
        newcube.front.se = c->down.se;
        newcube.down.ne  = c->back.sw;
        newcube.down.e   = c->back.w;
        newcube.down.se  = c->back.nw;
        newcube.back.nw  = c->up.se;
        newcube.back.w   = c->up.e;
        newcube.back.sw  = c->up.ne;
        (*c) = newcube;
        moves = mystrcat(moves, "R");
    } break;
    case MOVE_Ri:{
        cube newcube;
        newcube = (*c);
        rotate_face(&(newcube.right), COUNTERCLOCKWISE);
        newcube.up.ne = c->back.sw;
        newcube.up.e  = c->back.w;
        newcube.up.se = c->back.nw;
        newcube.front.ne = c->up.ne;
        newcube.front.e  = c->up.e;
        newcube.front.se = c->up.se;
        newcube.down.ne  = c->front.ne;
        newcube.down.e   = c->front.e;
        newcube.down.se  = c->front.se;
        newcube.back.nw  = c->down.se;
        newcube.back.w   = c->down.e;
        newcube.back.sw  = c->down.ne;
        (*c) = newcube;
        moves = mystrcat(moves, "R'");
    } break;
    case MOVE_R2:{
        cube newcube;
        newcube = (*c);
        rotate_face(&(newcube.right), S_180);
        newcube.up.ne = c->down.ne;
        newcube.up.e  = c->down.e;
        newcube.up.se = c->down.se;
        newcube.front.ne = c->back.sw;
        newcube.front.e  = c->back.w;
        newcube.front.se = c->back.nw;
        newcube.down.ne  = c->up.ne;
        newcube.down.e   = c->up.e;
        newcube.down.se  = c->up.se;
        newcube.back.nw  = c->front.se;
        newcube.back.w   = c->front.e;
        newcube.back.sw  = c->front.ne;
        (*c) = newcube;
        moves = mystrcat(moves, "R2");
    } break;
    case MOVE_L:{
        cube newcube;
        newcube = (*c);
        rotate_face(&(newcube.left), CLOCKWISE);
        newcube.up.nw = c->back.se;
        newcube.up.w  = c->back.e;
        newcube.up.sw = c->back.ne;
        newcube.front.nw = c->up.nw;
        newcube.front.w  = c->up.w;
        newcube.front.sw = c->up.sw;
        newcube.down.nw  = c->front.nw;
        newcube.down.w   = c->front.w;
        newcube.down.sw  = c->front.sw;
        newcube.back.ne  = c->down.sw;
        newcube.back.e   = c->down.w;
        newcube.back.se  = c->down.nw;
        (*c) = newcube;
        moves = mystrcat(moves, "L");
    } break;
    case MOVE_Li:{
        cube newcube;
        newcube = (*c);
        rotate_face(&(newcube.left), COUNTERCLOCKWISE);
        newcube.up.nw = c->front.nw;
        newcube.up.w  = c->front.w;
        newcube.up.sw = c->front.sw;
        newcube.front.nw = c->down.nw;
        newcube.front.w  = c->down.w;
        newcube.front.sw = c->down.sw;
        newcube.down.nw  = c->back.se;
        newcube.down.w   = c->back.e;
        newcube.down.sw  = c->back.ne;
        newcube.back.ne  = c->up.sw;
        newcube.back.e   = c->up.w;
        newcube.back.se  = c->up.nw;
        (*c) = newcube;
        moves = mystrcat(moves, "L'");
    } break;
    case MOVE_L2:{
        cube newcube;
        newcube = (*c);
        rotate_face(&(newcube.left), S_180);
        newcube.up.nw = c->down.nw;
        newcube.up.w  = c->down.w;
        newcube.up.sw = c->down.sw;
        newcube.front.nw = c->back.se;
        newcube.front.w  = c->back.e;
        newcube.front.sw = c->back.ne;
        newcube.down.nw  = c->up.nw;
        newcube.down.w   = c->up.w;
        newcube.down.sw  = c->up.sw;
        newcube.back.ne  = c->front.sw;
        newcube.back.e   = c->front.w;
        newcube.back.se  = c->front.nw;
        (*c) = newcube;
        moves = mystrcat(moves, "L2");
    } break;
    case MOVE_F:{
        cube newcube;
        newcube = (*c);
        rotate_face(&(newcube.front), CLOCKWISE);
        newcube.up.sw = c->left.se;
        newcube.up.s  = c->left.e;
        newcube.up.se = c->left.ne;
        newcube.right.nw = c->up.sw;
        newcube.right.w  = c->up.s;
        newcube.right.sw = c->up.se;
        newcube.down.nw  = c->right.sw;
        newcube.down.n   = c->right.w;
        newcube.down.ne  = c->right.nw;
        newcube.left.se  = c->down.ne;
        newcube.left.e   = c->down.n;
        newcube.left.ne  = c->down.nw;
        (*c) = newcube;
        moves = mystrcat(moves, "F");
    } break;
    case MOVE_Fi:{
        cube newcube;
        newcube = (*c);
        rotate_face(&(newcube.front), COUNTERCLOCKWISE);
        newcube.up.sw = c->right.nw;
        newcube.up.s  = c->right.w;
        newcube.up.se = c->right.sw;
        newcube.right.nw = c->down.ne;
        newcube.right.w  = c->down.n;
        newcube.right.sw = c->down.nw;
        newcube.down.nw  = c->left.ne;
        newcube.down.n   = c->left.e;
        newcube.down.ne  = c->left.se;
        newcube.left.se  = c->up.sw;
        newcube.left.e   = c->up.s;
        newcube.left.ne  = c->up.se;
        (*c) = newcube;
        moves = mystrcat(moves, "F'");
    } break;
    case MOVE_F2:{
        cube newcube;
        newcube = (*c);
        rotate_face(&(newcube.front), S_180);
        newcube.up.sw = c->down.ne;
        newcube.up.s  = c->down.n;
        newcube.up.se = c->down.nw;
        newcube.right.nw = c->left.se;
        newcube.right.w  = c->left.e;
        newcube.right.sw = c->left.ne;
        newcube.down.nw  = c->up.se;
        newcube.down.n   = c->up.s;
        newcube.down.ne  = c->up.sw;
        newcube.left.se  = c->right.nw;
        newcube.left.e   = c->right.w;
        newcube.left.ne  = c->right.sw;
        (*c) = newcube;
        moves = mystrcat(moves, "F2");
    } break;
    case MOVE_B:{
        cube newcube;
        newcube = (*c);
        rotate_face(&(newcube.back), CLOCKWISE);
        newcube.up.nw = c->right.ne;
        newcube.up.n  = c->right.e;
        newcube.up.ne = c->right.se;
        newcube.right.ne = c->down.se;
        newcube.right.e  = c->down.s;
        newcube.right.se = c->down.sw;
        newcube.down.sw  = c->left.nw;
        newcube.down.s   = c->left.w;
        newcube.down.se  = c->left.sw;
        newcube.left.sw  = c->up.nw;
        newcube.left.w   = c->up.n;
        newcube.left.nw  = c->up.ne;
        (*c) = newcube;
        moves = mystrcat(moves, "B");
    } break;
    case MOVE_Bi:{
        cube newcube;
        newcube = (*c);
        rotate_face(&(newcube.back), COUNTERCLOCKWISE);
        newcube.up.nw = c->left.sw;
        newcube.up.n  = c->left.w;
        newcube.up.ne = c->left.nw;
        newcube.right.ne = c->up.nw;
        newcube.right.e  = c->up.n;
        newcube.right.se = c->up.ne;
        newcube.down.sw  = c->right.se;
        newcube.down.s   = c->right.e;
        newcube.down.se  = c->right.ne;
        newcube.left.sw  = c->down.se;
        newcube.left.w   = c->down.s;
        newcube.left.nw  = c->down.sw;
        (*c) = newcube;
        moves = mystrcat(moves, "B'");
    } break;
    case MOVE_B2:{
        cube newcube;
        newcube = (*c);
        rotate_face(&(newcube.back), S_180);
        newcube.up.nw = c->down.se;
        newcube.up.n  = c->down.s;
        newcube.up.ne = c->down.sw;
        newcube.right.ne = c->left.sw;
        newcube.right.e  = c->left.w;
        newcube.right.se = c->left.nw;
        newcube.down.sw  = c->up.ne;
        newcube.down.s   = c->up.n;
        newcube.down.se  = c->up.nw;
        newcube.left.sw  = c->right.ne;
        newcube.left.w   = c->right.e;
        newcube.left.nw  = c->right.se;
        (*c) = newcube;
        moves = mystrcat(moves, "B2");
    } break;
    default:
        return -1;
    }
    return 0;
}
int do_moves(cube * c, char * movelist){
    int k = 0;
    while(movelist[k] != '\0'){
        if (movelist[k+1] == '2'){
                switch(movelist[k]){
                case 'U': do_move(c, MOVE_U2); break;
                case 'D': do_move(c, MOVE_D2); break;
                case 'L': do_move(c, MOVE_L2); break;
                case 'R': do_move(c, MOVE_R2); break;
                case 'F': do_move(c, MOVE_F2); break;
                case 'B': do_move(c, MOVE_B2);
                }
                k+=2;
        } else if (movelist[k + 1] == '\'' || movelist[k+1] == 'i'){
                switch(movelist[k]){
                case 'U': do_move(c, MOVE_Ui); break;
                case 'D': do_move(c, MOVE_Di); break;
                case 'L': do_move(c, MOVE_Li); break;
                case 'R': do_move(c, MOVE_Ri); break;
                case 'F': do_move(c, MOVE_Fi); break;
                case 'B': do_move(c, MOVE_Bi);
                }
                k+=2;
        } else {
                switch(movelist[k]){
                case 'U': do_move(c, MOVE_U); break;
                case 'D': do_move(c, MOVE_D); break;
                case 'L': do_move(c, MOVE_L); break;
                case 'R': do_move(c, MOVE_R); break;
                case 'F': do_move(c, MOVE_F); break;
                case 'B': do_move(c, MOVE_B);
                }
                k++;
        }
    }
    return 0;
}
int is_of_type(cube c, char * typestr){
    unsigned char cstr[55], k;
    type_to_string((char *)cstr, c);
    for (k = 0; k < 54; k++){
          if (typestr[k] == (unsigned char)'-')
                continue;
          if (typestr[k] != cstr[k])
                return 0;
    }
    return 1;
}
char getcolor(int t){
        if (COLOR(0) == t)
                return 'R';
        else if (COLOR(1) == t)
                return 'O';
        else if (COLOR(2) == t)
                return 'Y';
        else if (COLOR(3) == t)
                return 'G';
        else if (COLOR(4) == t)
                return 'B';
        else if (COLOR(5) == t)
                return 'W';
        else
                return ' ';
}
int getcolor_i(int t){
        if (COLOR(0) == t)
                return RED;
        else if (COLOR(1) == t)
                return ORANGE;
        else if (COLOR(2) == t)
                return YELLOW;
        else if (COLOR(3) == t)
                return GREEN;
        else if (COLOR(4) == t)
                return BLUE;
        else if (COLOR(5) == t)
                return WHITE;
        else
                return DARKGRAY;
}
void exchange_colors(cube * c, int c1, int c2, int c3, int c4, int c5, int c6){
    char scube[100];
    int k, len;
    type_to_string(scube, *c);
    len = strlen(scube);
    for (k = 0; k < len; k++){
        switch(scube[k]){
            case '1':
                scube[k] = c1 + '0';
                break;
            case '2':
                scube[k] = c2 + '0';
                break;
            case '3':
                scube[k] = c3 + '0';
                break;
            case '4':
                scube[k] = c4 + '0';
                break;
            case '5':
                scube[k] = c5 + '0';
                break;
            case '6':
                scube[k] = c6 + '0';
                break;
            case '0':
                break;

            default:
                return;
        }
    }
    type_from_string(c, scube);
}
void rot_and_swap(cube *c, int face){


    switch(face){

        case FACE_DOWN:
            rotate_cube(c, CLOCKWISE);
            rotate_cube(c, CLOCKWISE);
            exchange_colors(c, 6, 4, 3, 2, 5, 1);
        break;
        case FACE_LEFT:
            rotate_cube(c, CLOCKWISE);
            exchange_colors(c, 4, 1, 3, 6, 5, 2);
        break;
        case FACE_RIGHT:
            rotate_cube(c, COUNTERCLOCKWISE);
            exchange_colors(c, 2, 6, 3, 1, 5, 4);
        break;
        case FACE_FRONT:
            rotate_cube(c, DIR_UP);
            exchange_colors(c, 5, 2, 1, 4, 6, 3);
        break;
        case FACE_BACK:
            rotate_cube(c, DIR_DOWN);
            exchange_colors(c, 3, 2, 6, 4, 1, 5);
        break;
        default:
        return;
    }
}
int getid(char c){
        switch(c){
                case 'W':
                        return COLOR(5);
                case 'B':
                        return COLOR(4);
                case 'G':
                        return COLOR(3);
                case 'Y':
                        return COLOR(2);
                case 'O':
                        return COLOR(1);
                case 'R':
                        return COLOR(0);
                default:
                        return -1;
                }
}
void cube_to_string(char * str, cube c){
    sprintf(str,
        "   %c%c%c\n"
                "   %c%c%c\n"
                "   %c%c%c\n"
                "%c%c%c%c%c%c%c%c%c%c%c%c\n"
                "%c%c%c%c%c%c%c%c%c%c%c%c\n"
                "%c%c%c%c%c%c%c%c%c%c%c%c\n"
                "   %c%c%c\n"
                "   %c%c%c\n"
                "   %c%c%c",
                getcolor(c.up.nw), getcolor(c.up.n), getcolor(c.up.ne),
                getcolor(c.up.w), getcolor(c.up.center), getcolor(c.up.e),
                getcolor(c.up.sw), getcolor(c.up.s), getcolor(c.up.se),
                getcolor(c.left.nw), getcolor(c.left.n), getcolor(c.left.ne), getcolor(c.front.nw), getcolor(c.front.n), getcolor(c.front.ne), getcolor(c.right.nw), getcolor(c.right.n), getcolor(c.right.ne), getcolor(c.back.nw), getcolor(c.back.n), getcolor(c.back.ne),
                getcolor(c.left.w), getcolor(c.left.center), getcolor(c.left.e), getcolor(c.front.w), getcolor(c.front.center), getcolor(c.front.e), getcolor(c.right.w), getcolor(c.right.center), getcolor(c.right.e), getcolor(c.back.w), getcolor(c.back.center), getcolor(c.back.e),
                getcolor(c.left.sw), getcolor(c.left.s), getcolor(c.left.se), getcolor(c.front.sw), getcolor(c.front.s), getcolor(c.front.se), getcolor(c.right.sw), getcolor(c.right.s), getcolor(c.right.se), getcolor(c.back.sw), getcolor(c.back.s), getcolor(c.back.se),
                getcolor(c.down.nw), getcolor(c.down.n), getcolor(c.down.ne),
                getcolor(c.down.w), getcolor(c.down.center), getcolor(c.down.e),
                getcolor(c.down.sw), getcolor(c.down.s), getcolor(c.down.se)
                );
}
void type_to_string(char * str, cube c){
    sprintf(str,
                   "%d%d%d"
                   "%d%d%d"
                   "%d%d%d"
                "%d%d%d%d%d%d%d%d%d%d%d%d"
                "%d%d%d%d%d%d%d%d%d%d%d%d"
                "%d%d%d%d%d%d%d%d%d%d%d%d"
                   "%d%d%d"
                   "%d%d%d"
                   "%d%d%d",
                c.up.nw,    c.up.n,         c.up.ne,
                c.up.w,     c.up.center,    c.up.e,
                c.up.sw,    c.up.s,         c.up.se,
                c.left.nw,  c.left.n,       c.left.ne,
                c.left.w,   c.left.center,  c.left.e,
                c.left.sw,  c.left.s,       c.left.se,
                c.front.nw, c.front.n,      c.front.ne,
                c.front.w,  c.front.center, c.front.e,
                c.front.sw, c.front.s,      c.front.se,
                c.right.nw, c.right.n,      c.right.ne,
                c.right.w,  c.right.center, c.right.e,
                c.right.sw, c.right.s,      c.right.se,
                c.back.nw,  c.back.n,       c.back.ne,
                c.back.w,   c.back.center,  c.back.e,
                c.back.sw,  c.back.s,       c.back.se,
                c.down.nw,  c.down.n,       c.down.ne,
                c.down.w,   c.down.center,  c.down.e,
                c.down.sw,  c.down.s,       c.down.se
                );
}
void type_from_string(cube * c, const char * type){
        c->up.nw =  type[0] - '0';
        c->up.n =  type[1] - '0';
        c->up.ne =  type[2] - '0';
        c->up.w =  type[3] - '0';
        c->up.center =  type[4] - '0';
        c->up.e =  type[5] - '0';
        c->up.sw =  type[6] - '0';
        c->up.s =  type[7] - '0';
        c->up.se =  type[8] - '0';

        c->left.nw =  type[9] - '0';
        c->left.n =  type[10] - '0';
        c->left.ne =  type[11] - '0';
        c->left.w =  type[12] - '0';
        c->left.center =  type[13] - '0';
        c->left.e =  type[14] - '0';
        c->left.sw =  type[15] - '0';
        c->left.s =  type[16] - '0';
        c->left.se =  type[17] - '0';

        c->front.nw =  type[18] - '0';
        c->front.n =  type[19] - '0';
        c->front.ne =  type[20] - '0';
        c->front.w =  type[21] - '0';
        c->front.center =  type[22] - '0';
        c->front.e =  type[23] - '0';
        c->front.sw =  type[24] - '0';
        c->front.s =  type[25] - '0';
        c->front.se =  type[26] - '0';

        c->right.nw =  type[27] - '0';
        c->right.n =  type[28] - '0';
        c->right.ne =  type[29] - '0';
        c->right.w =  type[30] - '0';
        c->right.center =  type[31] - '0';
        c->right.e =  type[32] - '0';
        c->right.sw =  type[33] - '0';
        c->right.s =  type[34] - '0';
        c->right.se =  type[35] - '0';

        c->back.nw =  type[36] - '0';
        c->back.n =  type[37] - '0';
        c->back.ne =  type[38] - '0';
        c->back.w =  type[39] - '0';
        c->back.center =  type[40] - '0';
        c->back.e =  type[41] - '0';
        c->back.sw =  type[42] - '0';
        c->back.s =  type[43] - '0';
        c->back.se =  type[44] - '0';

        c->down.nw =  type[45] - '0';
        c->down.n =  type[46] - '0';
        c->down.ne =  type[47] - '0';
        c->down.w =  type[48] - '0';
        c->down.center =  type[49] - '0';
        c->down.e =  type[50] - '0';
        c->down.sw =  type[51] - '0';
        c->down.s =  type[52]- '0';
        c->down.se =  type[53] - '0';
}
int cube_from_string(cube * c, const char * user_in){
    int k;

        for(k = 0; k < 54; k++){
                switch(user_in[k]){
                case 'W':
                case 'B':
                case 'G':
                case 'Y':
                case 'O':
                case 'R':
                  continue;
                default:
                    printf("entered not valid characrer\n");
                        return -1;
                }
        }
        colors = 0;

        switch(user_in[4]){
                case 'R':
                        colors |= 0x00000001;
                        break;
                case 'O':
                        colors |= 0x00000010;
                        break;
                case 'Y':
                        colors |= 0x00000100;
                        break;
                case 'G':
                        colors |= 0x00001000;
                        break;
                case 'B':
                        colors |= 0x00010000;
                        break;
                case 'W':
                        colors |= 0x00100000;
        }
        switch(user_in[13]){
                case 'R':
                        colors |= 0x00000002;
                        break;
                case 'O':
                        colors |= 0x00000020;
                        break;
                case 'Y':
                        colors |= 0x00000200;
                        break;
                case 'G':
                        colors |= 0x00002000;
                        break;
                case 'B':
                        colors |= 0x00020000;
                        break;
                case 'W':
                        colors |= 0x00200000;
        }
        switch(user_in[22]){
                case 'R':
                        colors |= 0x00000003;
                        break;
                case 'O':
                        colors |= 0x00000030;
                        break;
                case 'Y':
                        colors |= 0x00000300;
                        break;
                case 'G':
                        colors |= 0x00003000;
                        break;
                case 'B':
                        colors |= 0x00030000;
                        break;
                case 'W':
                        colors |= 0x00300000;
        }
        switch(user_in[31]){
                case 'R':
                        colors |= 0x00000004;
                        break;
                case 'O':
                        colors |= 0x00000040;
                        break;
                case 'Y':
                        colors |= 0x00000400;
                        break;
                case 'G':
                        colors |= 0x00004000;
                        break;
                case 'B':
                        colors |= 0x00040000;
                        break;
                case 'W':
                        colors |= 0x00400000;
        }
        switch(user_in[40]){
                case 'R':
                        colors |= 0x00000005;
                        break;
                case 'O':
                        colors |= 0x00000050;
                        break;
                case 'Y':
                        colors |= 0x00000500;
                        break;
                case 'G':
                        colors |= 0x00005000;
                        break;
                case 'B':
                        colors |= 0x00050000;
                        break;
                case 'W':
                        colors |= 0x00500000;
        }
        switch(user_in[49]){
                case 'R':
                        colors |= 0x00000006;
                        break;
                case 'O':
                        colors |= 0x00000060;
                        break;
                case 'Y':
                        colors |= 0x00000600;
                        break;
                case 'G':
                        colors |= 0x00006000;
                        break;
                case 'B':
                        colors |= 0x00060000;
                        break;
                case 'W':
                        colors |= 0x00600000;
        }
        for (k = 0; k < 6; k++)
                if (COLOR(k) == 0){
                    printf("entered not valid centers, two or more centers have the same color\n");
                        return 1;
                }

        c->up.nw = getid(user_in[0]);
        c->up.n = getid(user_in[1]);
        c->up.ne = getid(user_in[2]);
        c->up.w = getid(user_in[3]);
        c->up.center = getid(user_in[4]);
        c->up.e = getid(user_in[5]);
        c->up.sw = getid(user_in[6]);
        c->up.s = getid(user_in[7]);
        c->up.se = getid(user_in[8]);

        c->left.nw = getid(user_in[9]);
        c->left.n = getid(user_in[10]);
        c->left.ne = getid(user_in[11]);
        c->left.w = getid(user_in[12]);
        c->left.center = getid(user_in[13]);
        c->left.e = getid(user_in[14]);
        c->left.sw = getid(user_in[15]);
        c->left.s = getid(user_in[16]);
        c->left.se = getid(user_in[17]);

        c->front.nw = getid(user_in[18]);
        c->front.n = getid(user_in[19]);
        c->front.ne = getid(user_in[20]);
        c->front.w = getid(user_in[21]);
        c->front.center = getid(user_in[22]);
        c->front.e = getid(user_in[23]);
        c->front.sw = getid(user_in[24]);
        c->front.s = getid(user_in[25]);
        c->front.se = getid(user_in[26]);

        c->right.nw = getid(user_in[27]);
        c->right.n = getid(user_in[28]);
        c->right.ne = getid(user_in[29]);
        c->right.w = getid(user_in[30]);
        c->right.center = getid(user_in[31]);
        c->right.e = getid(user_in[32]);
        c->right.sw = getid(user_in[33]);
        c->right.s = getid(user_in[34]);
        c->right.se = getid(user_in[35]);

        c->back.nw = getid(user_in[36]);
        c->back.n = getid(user_in[37]);
        c->back.ne = getid(user_in[38]);
        c->back.w = getid(user_in[39]);
        c->back.center = getid(user_in[40]);
        c->back.e = getid(user_in[41]);
        c->back.sw = getid(user_in[42]);
        c->back.s = getid(user_in[43]);
        c->back.se = getid(user_in[44]);

        c->down.nw = getid(user_in[45]);
        c->down.n = getid(user_in[46]);
        c->down.ne = getid(user_in[47]);
        c->down.w = getid(user_in[48]);
        c->down.center = getid(user_in[49]);
        c->down.e = getid(user_in[50]);
        c->down.sw = getid(user_in[51]);
        c->down.s = getid(user_in[52]);
        c->down.se = getid(user_in[53]);
        return 0;
}
void printcube(cube c){
    char tmp[81];
    int k;
    cube_to_string(tmp, c);
#ifdef WIN_OS
    HANDLE Out = GetStdHandle(STD_OUTPUT_HANDLE);
    for (k = 0; k < 81; k++){
        switch(tmp[k]){
                case ' ':
                        SetConsoleTextAttribute(Out, 0);
                        printf("  ");
                        break;
                case 'R':
                        SetConsoleTextAttribute(Out, BACKGROUND_RED | BACKGROUND_INTENSITY);
                        printf("  ");
                        break;
                case 'O':
                        SetConsoleTextAttribute(Out, BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY);
                        printf("%c%c", 177, 177);
                        break;
                case 'Y':
                        SetConsoleTextAttribute(Out, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
                        printf("  ");
                        break;
                case 'G':
                        SetConsoleTextAttribute(Out, BACKGROUND_GREEN | BACKGROUND_INTENSITY);
                        printf("  ");
                        break;
                case 'B':
                        SetConsoleTextAttribute(Out, BACKGROUND_BLUE | BACKGROUND_INTENSITY);
                        printf("  ");
                        break;
                case 'W':
                        SetConsoleTextAttribute(Out, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
                        printf("  ");
                        break;
                case '\n':
                        SetConsoleTextAttribute(Out, 0);
                        printf("\n");
        }
    }
    SetConsoleTextAttribute(Out, 0);
    SetConsoleTextAttribute(Out, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
    for (k = 0; k < 81; k++){
        switch(tmp[k]){
                case ' ':
                        printf(BK_COLOR_BLACK TX_COLOR_BLACK);
                        printf("  ");
                        break;
                case 'R':
                        printf(BKI_COLOR_RED);
                        printf("  ");
                        break;
                case 'O':
                        printf(BKI_COLOR_RED TXI_COLOR_YELLOW);
                        printf("##");
                        break;
                case 'Y':
                        printf(BKI_COLOR_YELLOW);
                        printf("  ");
                        break;
                case 'G':
                        printf(BKI_COLOR_GREEN);
                        printf("  ");
                        break;
                case 'B':
                        printf(BKI_COLOR_BLUE);
                        printf("  ");
                        break;
                case 'W':
                        printf(BKI_COLOR_WHITE);
                        printf("  ");
                        break;
                case '\n':
                        printf(COLOR_DEFAULT);
                        printf("\n");
        }
    }
    printf(COLOR_DEFAULT);
#endif
    printf("\n");
}
void getcube(cube * c){



}
void new_cube(cube * c, char s[54]){
    cube_from_string(c, s);
}
void optimize(){
    int c, n, len, lastmoveslen;
    char * newmoves = NULL, tmp[3], lastmove;
    do {
        newmoves = (char*)realloc(newmoves, 1);
        newmoves[0] = '\0';
        lastmoveslen = moveslen();
        for (len = strlen(moves), c = n = 0, lastmove = '\0'; c < len; c++){
            switch(moves[c]){
                case 'U':
                case 'D':
                case 'L':
                case 'R':
                case 'F':
                case 'B':
                    if (lastmove == '\0') {
                        lastmove = moves[c];
                        n++;
                    } else if (lastmove == moves[c]){
                        n++;
                    } else {
                        n %= 4;
                        if (n < 0)
                            n += 4;
                        switch(n){
                            case 1:
                                tmp[0] = lastmove;
                                tmp[1] = '\0';
                                newmoves = mystrcat(newmoves, tmp);
                                break;
                            case 2:
                                tmp[0] = lastmove;
                                tmp[1] = '2';
                                tmp[2] = '\0';
                                newmoves = mystrcat(newmoves, tmp);
                                break;
                            case 3:
                                tmp[0] = lastmove;
                                tmp[1] = '\'';
                                tmp[2] = '\0';
                                newmoves = mystrcat(newmoves, tmp);
                                break;
                        }
                        lastmove = moves[c];
                        n=1;
                    }
                    break;

                case '\'':
                case 'i':
                    n-=2;
                    break;

                case '2':
                    n++;
                    break;
            }
        }
        n %= 4;
        if (n < 0)
            n += 4;
        switch(n){
            case 1:
                tmp[0] = lastmove;
                tmp[1] = '\0';
                newmoves = mystrcat(newmoves, tmp);
                break;
            case 2:
                tmp[0] = lastmove;
                tmp[1] = '2';
                tmp[2] = '\0';
                newmoves = mystrcat(newmoves, tmp);
                break;
            case 3:
                tmp[0] = lastmove;
                tmp[1] = '\'';
                tmp[2] = '\0';
                newmoves = mystrcat(newmoves, tmp);
                break;
        }
        moves = (char*)realloc(moves, strlen(newmoves) + 1);
        strcpy(moves, newmoves);
    } while (lastmoveslen - moveslen());
    free(newmoves);
    return;
}
int moveslen(){
    int count, pos, len = strlen(moves);
    for (pos = count = 0; pos < len; pos++){
        switch(moves[pos]){
            case 'F':
            case 'B':
            case 'L':
            case 'R':
            case 'U':
            case 'D':
                count++;
        }
    }
    return count;
}
int mov_len(const char * str){
    int count, pos, len = strlen(str);
    for (pos = count = 0; pos < len; pos++){
        switch(str[pos]){
            case 'F':
            case 'B':
            case 'L':
            case 'R':
            case 'U':
            case 'D':
                count++;
        }
    }
    return count;
}
void scramble(cube * c, int nmoves){
    int m, rnd;
    char last = 0;
    moves = (char *)realloc(moves, 1);
    moves[0] = 0;
    srand(time(NULL) - rand());
    for (m = 0; m < nmoves; m++){
        switch(last){
        case 0:
            rnd = rand() % 18;
            switch(rnd){
            case 0:
                    do_move(c, MOVE_U);
                    last = 'U';
                    break;
            case 1:
                    do_move(c, MOVE_D);
                    last = 'D';
                    break;
            case 2:
                    do_move(c, MOVE_L);
                    last = 'L';
                    break;
            case 3:
                    do_move(c, MOVE_R);
                    last = 'R';
                    break;
            case 4:
                    do_move(c, MOVE_F);
                    last = 'F';
                    break;
            case 5:
                    do_move(c, MOVE_B);
                    last = 'B';
                    break;
            case 6:
                    do_move(c, MOVE_Ui);
                    last = 'U';
                    break;
            case 7:
                    do_move(c, MOVE_Di);
                    last = 'D';
                    break;
            case 8:
                    do_move(c, MOVE_Li);
                    last = 'L';
                    break;
            case 9:
                    do_move(c, MOVE_Ri);
                    last = 'R';
                    break;
            case 10:
                    do_move(c, MOVE_Fi);
                    last = 'F';
                    break;
            case 11:
                    do_move(c, MOVE_Bi);
                    last = 'B';
                    break;
            case 12:
                    do_move(c, MOVE_U2);
                    last = 'U';
                    break;
            case 13:
                    do_move(c, MOVE_D2);
                    last = 'D';
                    break;
            case 14:
                    do_move(c, MOVE_L2);
                    last = 'L';
                    break;
            case 15:
                    do_move(c, MOVE_R2);
                    last = 'R';
                    break;
            case 16:
                    do_move(c, MOVE_F2);
                    last = 'F';
                    break;
            case 17:
                    do_move(c, MOVE_B2);
                    last = 'B';
                    break;
            }
            break;
        case 'U':
            rnd = rand() % 15;
            switch(rnd){
            case 0:
                    do_move(c, MOVE_D);
                    last = 'D';
                    break;
            case 1:
                    do_move(c, MOVE_L);
                    last = 'L';
                    break;
            case 2:
                    do_move(c, MOVE_R);
                    last = 'R';
                    break;
            case 3:
                    do_move(c, MOVE_F);
                    last = 'F';
                    break;
            case 4:
                    do_move(c, MOVE_B);
                    last = 'B';
                    break;
            case 5:
                    do_move(c, MOVE_Di);
                    last = 'D';
                    break;
            case 6:
                    do_move(c, MOVE_Li);
                    last = 'L';
                    break;
            case 7:
                    do_move(c, MOVE_Ri);
                    last = 'R';
                    break;
            case 8:
                    do_move(c, MOVE_Fi);
                    last = 'F';
                    break;
            case 9:
                    do_move(c, MOVE_Bi);
                    last = 'B';
                    break;
            case 10:
                    do_move(c, MOVE_D2);
                    last = 'D';
                    break;
            case 11:
                    do_move(c, MOVE_L2);
                    last = 'L';
                    break;
            case 12:
                    do_move(c, MOVE_R2);
                    last = 'R';
                    break;
            case 13:
                    do_move(c, MOVE_F2);
                    last = 'F';
                    break;
            case 14:
                    do_move(c, MOVE_B2);
                    last = 'B';
                    break;
            }
            break;
        case 'D':
            rnd = rand() % 15;
            switch(rnd){
            case 0:
                    do_move(c, MOVE_U);
                    last = 'U';
                    break;
            case 1:
                    do_move(c, MOVE_L);
                    last = 'L';
                    break;
            case 2:
                    do_move(c, MOVE_R);
                    last = 'R';
                    break;
            case 3:
                    do_move(c, MOVE_F);
                    last = 'F';
                    break;
            case 4:
                    do_move(c, MOVE_B);
                    last = 'B';
                    break;
            case 5:
                    do_move(c, MOVE_Ui);
                    last = 'U';
                    break;
            case 6:
                    do_move(c, MOVE_Li);
                    last = 'L';
                    break;
            case 7:
                    do_move(c, MOVE_Ri);
                    last = 'R';
                    break;
            case 8:
                    do_move(c, MOVE_Fi);
                    last = 'F';
                    break;
            case 9:
                    do_move(c, MOVE_Bi);
                    last = 'B';
                    break;
            case 10:
                    do_move(c, MOVE_U2);
                    last = 'U';
                    break;
            case 11:
                    do_move(c, MOVE_L2);
                    last = 'L';
                    break;
            case 12:
                    do_move(c, MOVE_R2);
                    last = 'R';
                    break;
            case 13:
                    do_move(c, MOVE_F2);
                    last = 'F';
                    break;
            case 14:
                    do_move(c, MOVE_B2);
                    last = 'B';
                    break;
            }
            break;
        case 'L':
            rnd = rand() % 15;
            switch(rnd){
            case 0:
                    do_move(c, MOVE_U);
                    last = 'U';
                    break;
            case 1:
                    do_move(c, MOVE_D);
                    last = 'D';
                    break;
            case 2:
                    do_move(c, MOVE_R);
                    last = 'R';
                    break;
            case 3:
                    do_move(c, MOVE_F);
                    last = 'F';
                    break;
            case 4:
                    do_move(c, MOVE_B);
                    last = 'B';
                    break;
            case 5:
                    do_move(c, MOVE_Ui);
                    last = 'U';
                    break;
            case 6:
                    do_move(c, MOVE_Di);
                    last = 'D';
                    break;
            case 7:
                    do_move(c, MOVE_Ri);
                    last = 'R';
                    break;
            case 8:
                    do_move(c, MOVE_Fi);
                    last = 'F';
                    break;
            case 9:
                    do_move(c, MOVE_Bi);
                    last = 'B';
                    break;
            case 10:
                    do_move(c, MOVE_U2);
                    last = 'U';
                    break;
            case 11:
                    do_move(c, MOVE_D2);
                    last = 'D';
                    break;
            case 12:
                    do_move(c, MOVE_R2);
                    last = 'R';
                    break;
            case 13:
                    do_move(c, MOVE_F2);
                    last = 'F';
                    break;
            case 14:
                    do_move(c, MOVE_B2);
                    last = 'B';
                    break;
            }
            break;
        case 'R':
            rnd = rand() % 15;
            switch(rnd){
            case 0:
                    do_move(c, MOVE_U);
                    last = 'U';
                    break;
            case 1:
                    do_move(c, MOVE_D);
                    last = 'D';
                    break;
            case 2:
                    do_move(c, MOVE_L);
                    last = 'L';
                    break;
            case 3:
                    do_move(c, MOVE_F);
                    last = 'F';
                    break;
            case 4:
                    do_move(c, MOVE_B);
                    last = 'B';
                    break;
            case 5:
                    do_move(c, MOVE_Ui);
                    last = 'U';
                    break;
            case 6:
                    do_move(c, MOVE_Di);
                    last = 'D';
                    break;
            case 7:
                    do_move(c, MOVE_Li);
                    last = 'L';
                    break;
            case 8:
                    do_move(c, MOVE_Fi);
                    last = 'F';
                    break;
            case 9:
                    do_move(c, MOVE_Bi);
                    last = 'B';
                    break;
            case 10:
                    do_move(c, MOVE_U2);
                    last = 'U';
                    break;
            case 11:
                    do_move(c, MOVE_D2);
                    last = 'D';
                    break;
            case 12:
                    do_move(c, MOVE_L2);
                    last = 'L';
                    break;
            case 13:
                    do_move(c, MOVE_F2);
                    last = 'F';
                    break;
            case 14:
                    do_move(c, MOVE_B2);
                    last = 'B';
                    break;
            }
            break;
        case 'F':
            rnd = rand() % 15;
            switch(rnd){
            case 0:
                    do_move(c, MOVE_U);
                    last = 'U';
                    break;
            case 1:
                    do_move(c, MOVE_D);
                    last = 'D';
                    break;
            case 2:
                    do_move(c, MOVE_L);
                    last = 'L';
                    break;
            case 3:
                    do_move(c, MOVE_R);
                    last = 'R';
                    break;
            case 4:
                    do_move(c, MOVE_B);
                    last = 'B';
                    break;
            case 5:
                    do_move(c, MOVE_Ui);
                    last = 'U';
                    break;
            case 6:
                    do_move(c, MOVE_Di);
                    last = 'D';
                    break;
            case 7:
                    do_move(c, MOVE_Li);
                    last = 'L';
                    break;
            case 8:
                    do_move(c, MOVE_Ri);
                    last = 'R';
                    break;
            case 9:
                    do_move(c, MOVE_Bi);
                    last = 'B';
                    break;
            case 10:
                    do_move(c, MOVE_U2);
                    last = 'U';
                    break;
            case 11:
                    do_move(c, MOVE_D2);
                    last = 'D';
                    break;
            case 12:
                    do_move(c, MOVE_L2);
                    last = 'L';
                    break;
            case 13:
                    do_move(c, MOVE_R2);
                    last = 'R';
                    break;
            case 14:
                    do_move(c, MOVE_B2);
                    last = 'B';
                    break;
            }
            break;
        case 'B':
            rnd = rand() % 15;
            switch(rnd){
            case 0:
                    do_move(c, MOVE_U);
                    last = 'U';
                    break;
            case 1:
                    do_move(c, MOVE_D);
                    last = 'D';
                    break;
            case 2:
                    do_move(c, MOVE_L);
                    last = 'L';
                    break;
            case 3:
                    do_move(c, MOVE_R);
                    last = 'R';
                    break;
            case 4:
                    do_move(c, MOVE_F);
                    last = 'F';
                    break;
            case 5:
                    do_move(c, MOVE_Ui);
                    last = 'U';
                    break;
            case 6:
                    do_move(c, MOVE_Di);
                    last = 'D';
                    break;
            case 7:
                    do_move(c, MOVE_Li);
                    last = 'L';
                    break;
            case 8:
                    do_move(c, MOVE_Ri);
                    last = 'R';
                    break;
            case 9:
                    do_move(c, MOVE_Fi);
                    last = 'F';
                    break;
            case 10:
                    do_move(c, MOVE_U2);
                    last = 'U';
                    break;
            case 11:
                    do_move(c, MOVE_D2);
                    last = 'D';
                    break;
            case 12:
                    do_move(c, MOVE_L2);
                    last = 'L';
                    break;
            case 13:
                    do_move(c, MOVE_R2);
                    last = 'R';
                    break;
            case 14:
                    do_move(c, MOVE_F2);
                    last = 'F';
                    break;
            }
            break;
        }
    }
    printf("\nScrambling\n"
           "%s\n", moves);
}
int not_found_solution(){
    printf("--ERROR: Not found a solution, your cube was disassembled and reassembled uncorrectly.\n");
    getchar();
    return 0;
}
int _solve(cube c){
    int t = 0;
    moves = (char *)realloc(moves, 1);
    moves[0] = 0;



    if (is_of_type(c, "-1-111-1--2--2-----3--3-----4--4-----5--5--------6----"))
        goto corner_first_layer;



    if (is_of_type(c, "---11-----2--2--------3--------4--------5--------6----"));
    else if (is_of_type(c, "---21-----1--2--------3--------4--------5--------6----"))
        do_moves(&c, "LU'FU");
    else if (is_of_type(c, "----1--1-----2-----2--3--------4--------5--------6----"))
        do_moves(&c, "F2D'L2");
    else if (is_of_type(c, "----1--2-----2-----1--3--------4--------5--------6----"))
        do_moves(&c, "F'L'");
    else if (is_of_type(c, "----11-------2--------3-----2--4--------5--------6----"))
        do_moves(&c, "R'F2L'");
    else if (is_of_type(c, "----12-------2--------3-----1--4--------5--------6----"))
        do_moves(&c, "UF'U'L'");
    else if (is_of_type(c, "-1--1--------2--------3--------4-----2--5--------6----"))
        do_moves(&c, "B2DL2");
    else if (is_of_type(c, "-2--1--------2--------3--------4-----1--5--------6----"))
        do_moves(&c, "BL");
    else if (is_of_type(c, "----1--------22------13--------4--------5--------6----"))
        do_moves(&c, "L'");
    else if (is_of_type(c, "----1--------21------23--------4--------5--------6----"))
        do_moves(&c, "U'FU");
    else if (is_of_type(c, "----1--------2--------31------24--------5--------6----"))
        do_moves(&c, "F2L'");
    else if (is_of_type(c, "----1--------2--------32------14--------5--------6----"))
        do_moves(&c, "U'F'U");
    else if (is_of_type(c, "----1--------2--------3--------42------15--------6----"))
        do_moves(&c, "U2R'U2");
    else if (is_of_type(c, "----1--------2--------3--------41------25--------6----"))
        do_moves(&c, "UBU'");
    else if (is_of_type(c, "----1-------22--------3--------4--------51-------6----"))
        do_moves(&c, "L");
    else if (is_of_type(c, "----1-------12--------3--------4--------52-------6----"))
        do_moves(&c, "UB'U'");
    else if (is_of_type(c, "----1--------2--2-----3--------4--------5-------16----"))
        do_moves(&c, "L2");
    else if (is_of_type(c, "----1--------2--1-----3--------4--------5-------26----"))
        do_moves(&c, "DFL'");
    else if (is_of_type(c, "----1--------2--------3--2-----4--------5-----1--6----"))
        do_moves(&c, "D'L2");
    else if (is_of_type(c, "----1--------2--------3--1-----4--------5-----2--6----"))
        do_moves(&c, "FL'");
    else if (is_of_type(c, "----1--------2--------3--------4--2-----5--------61---"))
        do_moves(&c, "D2L2");
    else if (is_of_type(c, "----1--------2--------3--------4--1-----5--------62---"))
        do_moves(&c, "D'FL'");
    else if (is_of_type(c, "----1--------2--------3--------4--------5--2-----6--1-"))
        do_moves(&c, "DL2");
    else if (is_of_type(c, "----1--------2--------3--------4--------5--1-----6--2-"))
        do_moves(&c, "B'L");
    else{
        not_found_solution();
        goto notfoundsolution;
    }


    if (is_of_type(c, "---11--1--2--2-----3--3--------4--------5--------6----"));
    else if (is_of_type(c, "---11--3--2--2-----1--3--------4--------5--------6----"))
        do_moves(&c, "F'UL'U'");
    else if (is_of_type(c, "---111----2--2--------3-----3--4--------5--------6----"))
        do_moves(&c, "R2D'F2");
    else if (is_of_type(c, "---113----2--2--------3-----1--4--------5--------6----"))
        do_moves(&c, "R'F'");
    else if (is_of_type(c, "-1-11-----2--2--------3--------4-----3--5--------6----"))
        do_moves(&c, "B'R2F'");
    else if (is_of_type(c, "-3-11-----2--2--------3--------4-----1--5--------6----"))
        do_moves(&c, "UR'U'F'");
    else if (is_of_type(c, "---11-----2--21------33--------4--------5--------6----"))
        do_moves(&c, "F");
    else if (is_of_type(c, "---11-----2--23------13--------4--------5--------6----"))
        do_moves(&c, "UL'U'");
    else if (is_of_type(c, "---11-----2--2--------33------14--------5--------6----"))
        do_moves(&c, "F'");
    else if (is_of_type(c, "---11-----2--2--------31------34--------5--------6----"))
        do_moves(&c, "U'RU");
    else if (is_of_type(c, "---11-----2--2--------3--------41------35--------6----"))
        do_moves(&c, "R2F'");
    else if (is_of_type(c, "---11-----2--2--------3--------43------15--------6----"))
        do_moves(&c, "U'R'U");
    else if (is_of_type(c, "---11-----2-12--------3--------4--------53-------6----"))
        do_moves(&c, "U2B'U2");
    else if (is_of_type(c, "---11-----2-32--------3--------4--------51-------6----"))
        do_moves(&c, "L'DLF2");
    else if (is_of_type(c, "---11-----2--2--------3--3-----4--------5-----1--6----"))
        do_moves(&c, "F2");
    else if (is_of_type(c, "---11-----2--2--------3--1-----4--------5-----3--6----"))
        do_moves(&c, "DRF'");
    else if (is_of_type(c, "---11-----2--2--------3--------4--3-----5--------61---"))
        do_moves(&c, "D'F2");
    else if (is_of_type(c, "---11-----2--2--------3--------4--1-----5--------63---"))
        do_moves(&c, "RF'");
    else if (is_of_type(c, "---11-----2--2--------3--------4--------5--3-----6--1-"))
        do_moves(&c, "D2F2");
    else if (is_of_type(c, "---11-----2--2--------3--------4--------5--1-----6--3-"))
        do_moves(&c, "D'RF'");
    else if (is_of_type(c, "---11-----2--2--3-----3--------4--------5-------16----"))
        do_moves(&c, "DF2");
    else if (is_of_type(c, "---11-----2--2--1-----3--------4--------5-------36----"))
        do_moves(&c, "L'FL");
    else {
        not_found_solution();
        goto notfoundsolution;
    }


    if (is_of_type(c, "---111-1--2--2-----3--3-----4--4--------5--------6----"));
    else if (is_of_type(c, "---114-1--2--2-----3--3-----1--4--------5--------6----"))
        do_moves(&c, "R'UF'U'");
    else if (is_of_type(c, "-1-11--1--2--2-----3--3--------4-----4--5--------6----"))
        do_moves(&c, "B'U'BU");
    else if (is_of_type(c, "-4-11--1--2--2-----3--3--------4-----1--5--------6----"))
        do_moves(&c, "B'R'");
    else if (is_of_type(c, "---11--1--2--24----3-13--------4--------5--------6----"))
        do_moves(&c, "F2RF2");
    else if (is_of_type(c, "---11--1--2--21----3-43--------4--------5--------6----"))
        do_moves(&c, "UFU'");
    else if (is_of_type(c, "---11--1--2--2-----3--31------44--------5--------6----"))
        do_moves(&c, "R");
    else if (is_of_type(c, "---11--1--2--2-----3--34------14--------5--------6----"))
        do_moves(&c, "UF'U'");
    else if (is_of_type(c, "---11--1--2--2-----3--3--------44------15--------6----"))
        do_moves(&c, "R'");
    else if (is_of_type(c, "---11--1--2--2-----3--3--------41------45--------6----"))
        do_moves(&c, "U'BU");
    else if (is_of_type(c, "---11--1--2-42-----3--3--------4--------51-------6----"))
        do_moves(&c, "B2R'");
    else if (is_of_type(c, "---11--1--2-12-----3--3--------4--------54-------6----"))
        do_moves(&c, "U'B'U");
    else if (is_of_type(c, "---11--1--2--2-----3--3--------4--4-----5--------61---"))
        do_moves(&c, "R2");
    else if (is_of_type(c, "---11--1--2--2-----3--3--------4--1-----5--------64---"))
        do_moves(&c, "DBR'");
    else if (is_of_type(c, "---11--1--2--2-----3--3--------4--------5--4-----6--1-"))
        do_moves(&c, "D'R2");
    else if (is_of_type(c, "---11--1--2--2-----3--3--------4--------5--1-----6--4-"))
        do_moves(&c, "BR'");
    else if (is_of_type(c, "---11--1--2--2--1--3--3--------4--------5-------46----"))
        do_moves(&c, "D'BR'");
    else if (is_of_type(c, "---11--1--2--2--4--3--3--------4--------5-------16----"))
        do_moves(&c, "D2R2");
    else if (is_of_type(c, "---11--1--2--2-----3--3--4-----4--------5-----1--6----"))
        do_moves(&c, "DR2");
    else if (is_of_type(c, "---11--1--2--2-----3--3--1-----4--------5-----4--6----"))
        do_moves(&c, "D2BR'");
    else {
        not_found_solution();
        goto notfoundsolution;
    }


    if (is_of_type(c, "-1-111-1--2--2-----3--3-----4--4-----5--5--------6----"));
    else if (is_of_type(c, "-5-111-1--2--2-----3--3-----4--4-----1--5--------6----"))
        do_moves(&c, "URU'B");
    else if (is_of_type(c, "---111-1--2--25----3-13-----4--4--------5--------6----"))
        do_moves(&c, "U'L'U");
    else if (is_of_type(c, "---111-1--2--21----3-53-----4--4--------5--------6----"))
        do_moves(&c, "U2FU2");
    else if (is_of_type(c, "---111-1--2--2-----3--35----4-14--------5--------6----"))
        do_moves(&c, "U2F'U2");
    else if (is_of_type(c, "---111-1--2--2-----3--31----4-54--------5--------6----"))
        do_moves(&c, "URU'");
    else if (is_of_type(c, "---111-1--2--2-----3--3-----4--41------55--------6----"))
        do_moves(&c, "B");
    else if (is_of_type(c, "---111-1--2--2-----3--3-----4--45------15--------6----"))
        do_moves(&c, "UR'U'");
    else if (is_of_type(c, "---111-1--2-12-----3--3-----4--4--------55-------6----"))
        do_moves(&c, "B'");
    else if (is_of_type(c, "---111-1--2-52-----3--3-----4--4--------51-------6----"))
        do_moves(&c, "U'LU");
    else if (is_of_type(c, "---111-1--2--2-----3--3--5--4--4--------5-----1--6----"))
        do_moves(&c, "D2B2");
    else if (is_of_type(c, "---111-1--2--2-----3--3--1--4--4--------5-----5--6----"))
        do_moves(&c, "DR'BR");
    else if (is_of_type(c, "---111-1--2--2-----3--3-----4--4--5-----5--------61---"))
        do_moves(&c, "DB2");
    else if (is_of_type(c, "---111-1--2--2-----3--3-----4--4--1-----5--------65---"))
        do_moves(&c, "R'BR");
    else if (is_of_type(c, "---111-1--2--2-----3--3-----4--4--------5--5-----6--1-"))
        do_moves(&c, "B2");
    else if (is_of_type(c, "---111-1--2--2-----3--3-----4--4--------5--1-----6--5-"))
        do_moves(&c, "DLB'L'");
    else if (is_of_type(c, "---111-1--2--2--5--3--3-----4--4--------5-------16----"))
        do_moves(&c, "D'B2");
    else if (is_of_type(c, "---111-1--2--2--1--3--3-----4--4--------5-------56----"))
        do_moves(&c, "LB'L'");
    else {
        not_found_solution();
        goto notfoundsolution;
    }

corner_first_layer:
    if (is_of_type(c, "111111111222-2----333-3----444-4----555-5--------6----"))
        goto edges_second_layer;

    if (is_of_type(c, "-1-11111--22-2----33--3-----4--4-----5--5--------6----"));
    else if (is_of_type(c, "-1-11121--23-2----13--3-----4--4-----5--5--------6----"))
        do_moves(&c, "LDR'D2RL'");
    else if (is_of_type(c, "-1-11131--21-2----23--3-----4--4-----5--5--------6----"))
        do_moves(&c, "LD'FL2F'L'");
    else if (is_of_type(c, "-1-111-1--2--2---2-3--3-1---4--4-----5--5----3---6----"))
        do_moves(&c, "F'D'F");
    else if (is_of_type(c, "-1-111-1--2--2---3-3--3-2---4--4-----5--5----1---6----"))
        do_moves(&c, "DR2F2R2F2");
    else if (is_of_type(c, "-1-111-1--2--2---1-3--3-3---4--4-----5--5----2---6----"))
        do_moves(&c, "LDL'");
    else if (is_of_type(c, "-1-111-11-2--2-----32-3----34--4-----5--5--------6----"))
        do_moves(&c, "LFDF'D2L'");
    else if (is_of_type(c, "-1-111-12-2--2-----33-3----14--4-----5--5--------6----"))
        do_moves(&c, "LR'D'RL'");
    else if (is_of_type(c, "-1-111-13-2--2-----31-3----24--4-----5--5--------6----"))
        do_moves(&c, "FDF2D2F");
    else if (is_of_type(c, "-1-111-1--2--2-----3--3---3-4--4-2---5--5------1-6----"))
        do_moves(&c, "R2F2R2F2");
    else if (is_of_type(c, "-1-111-1--2--2-----3--3---1-4--4-3---5--5------2-6----"))
        do_moves(&c, "DF'D2F");
    else if (is_of_type(c, "-1-111-1--2--2-----3--3---2-4--4-1---5--5------3-6----"))
        do_moves(&c, "LD'L'");
    else if (is_of_type(c, "-1-111-1--2--2-----3--3-----4--4---3-5--5-2------6---1"))
        do_moves(&c, "DL2FL2F'");
    else if (is_of_type(c, "-1-111-1--2--2-----3--3-----4--4---1-5--5-3------6---2"))
        do_moves(&c, "F'D2F");
    else if (is_of_type(c, "-1-111-1--2--2-----3--3-----4--4---2-5--5-1------6---3"))
        do_moves(&c, "LD2L'");
    else if (is_of_type(c, "-11111-1--2--2-----3--3-----42-4----35--5--------6----"))
        do_moves(&c, "L2B2L2B2");
    else if (is_of_type(c, "-12111-1--2--2-----3--3-----43-4----15--5--------6----"))
        do_moves(&c, "LB'D2BL'");
    else if (is_of_type(c, "-13111-1--2--2-----3--3-----41-4----25--5--------6----"))
        do_moves(&c, "RF'D2FR'");
    else if (is_of_type(c, "11-111-1-32--2-----3--3-----4--4-----52-5--------6----"))
        do_moves(&c, "FL2F'L'D'L'");
    else if (is_of_type(c, "31-111-1-22--2-----3--3-----4--4-----51-5--------6----"))
        do_moves(&c, "BF'DFB'");
    else if (is_of_type(c, "21-111-1-12--2-----3--3-----4--4-----53-5--------6----"))
        do_moves(&c, "L'D'L2D2L'");
    else if (is_of_type(c, "-1-111-1--2--2-2---3--3-----4--4-----5--5---3----6-1--"))
        do_moves(&c, "L2FL2F'");
    else if (is_of_type(c, "-1-111-1--2--2-3---3--3-----4--4-----5--5---1----6-2--"))
        do_moves(&c, "F'DF");
    else if (is_of_type(c, "-1-111-1--2--2-1---3--3-----4--4-----5--5---2----6-3--"))
        do_moves(&c, "DF'D'F");
    else {
        not_found_solution();
        goto notfoundsolution;
    }


    if (is_of_type(c, "-1-111111-22-2----333-3----44--4-----5--5--------6----"));
    else if (is_of_type(c, "-1-111114-22-2----331-3----34--4-----5--5--------6----"))
        do_moves(&c, "RF'R'F2DF'");
    else if (is_of_type(c, "-1-111113-22-2----334-3----14--4-----5--5--------6----"))
        do_moves(&c, "FDB'D2BF'");
    else if (is_of_type(c, "-1-11111--22-2----33--3---4-4--4-3---5--5------1-6----"))
        do_moves(&c, "DR2F'R2F");
    else if (is_of_type(c, "-1-11111--22-2----33--3---1-4--4-4---5--5------3-6----"))
        do_moves(&c, "FDF'");
    else if (is_of_type(c, "-1-11111--22-2----33--3---3-4--4-1---5--5------4-6----"))
        do_moves(&c, "R'D'R");
    else if (is_of_type(c, "-1-11111--22-2----33--3-----4--4---4-5--5-3------6---1"))
        do_moves(&c, "R2F'R2F");
    else if (is_of_type(c, "-1-11111--22-2----33--3-----4--4---1-5--5-4------6---3"))
        do_moves(&c, "DR'D2R");
    else if (is_of_type(c, "-1-11111--22-2----33--3-----4--4---3-5--5-1------6---4"))
        do_moves(&c, "FD'F'");
    else if (is_of_type(c, "-1111111--22-2----33--3-----43-4----45--5--------6----"))
        do_moves(&c, "RDRBR2B'");
    else if (is_of_type(c, "-1411111--22-2----33--3-----41-4----35--5--------6----"))
        do_moves(&c, "RDR2D2R");
    else if (is_of_type(c, "-1311111--22-2----33--3-----44-4----15--5--------6----"))
        do_moves(&c, "FB'D'BF'");
    else if (is_of_type(c, "11-11111-422-2----33--3-----4--4-----53-5--------6----"))
        do_moves(&c, "R2B2R2B2");
    else if (is_of_type(c, "41-11111-322-2----33--3-----4--4-----51-5--------6----"))
        do_moves(&c, "BR'D2RB'");
    else if (is_of_type(c, "31-11111-122-2----33--3-----4--4-----54-5--------6----"))
        do_moves(&c, "FL'D2LF'");
    else if (is_of_type(c, "-1-11111--22-2-3--33--3-----4--4-----5--5---4----6-1--"))
        do_moves(&c, "FL'DLF'");
    else if (is_of_type(c, "-1-11111--22-2-1--33--3-----4--4-----5--5---3----6-4--"))
        do_moves(&c, "FD2F'");
    else if (is_of_type(c, "-1-11111--22-2-4--33--3-----4--4-----5--5---1----6-3--"))
        do_moves(&c, "R'D2R");
    else if (is_of_type(c, "-1-11111--22-2---433--3-3---4--4-----5--5----1---6----"))
        do_moves(&c, "LF2L'F2L'");
    else if (is_of_type(c, "-1-11111--22-2---133--3-4---4--4-----5--5----3---6----"))
        do_moves(&c, "R'DR");
    else if (is_of_type(c, "-1-11111--22-2---333--3-1---4--4-----5--5----4---6----"))
        do_moves(&c, "DR'D'R");
    else {
        not_found_solution();
        goto notfoundsolution;
    }



    if (is_of_type(c, "-11111111-22-2----333-3----444-4----55--5--------6----"));
    else if (is_of_type(c, "-14111111-22-2----333-3----445-4----15--5--------6----"))
        do_moves(&c, "RDL'D2LR'");
    else if (is_of_type(c, "-15111111-22-2----333-3----441-4----45--5--------6----"))
        do_moves(&c, "LB2L'BD2B");
    else if (is_of_type(c, "-1-111111-22-2----333-3----44--4---4-5--5-1------6---5"))
        do_moves(&c, "B'D'B");
    else if (is_of_type(c, "-1-111111-22-2----333-3----44--4---5-5--5-4------6---1"))
        do_moves(&c, "DB2R'B2R");
    else if (is_of_type(c, "-1-111111-22-2----333-3----44--4---1-5--5-5------6---4"))
        do_moves(&c, "RDR'");
    else if (is_of_type(c, "11-111111522-2----333-3----44--4-----54-5--------6----"))
        do_moves(&c, "LB'L2D'LB");
    else if (is_of_type(c, "41-111111122-2----333-3----44--4-----55-5--------6----"))
        do_moves(&c, "RL'D'LR'");
    else if (is_of_type(c, "51-111111422-2----333-3----44--4-----51-5--------6----"))
        do_moves(&c, "BDB2D2B");
    else if (is_of_type(c, "-1-111111-22-2-5--333-3----44--4-----5--5---1----6-4--"))
        do_moves(&c, "DB'D2B");
    else if (is_of_type(c, "-1-111111-22-2-4--333-3----44--4-----5--5---5----6-1--"))
        do_moves(&c, "B2R'B2R");
    else if (is_of_type(c, "-1-111111-22-2-1--333-3----44--4-----5--5---4----6-5--"))
        do_moves(&c, "RD'R'");
    else if (is_of_type(c, "-1-111111-22-2---1333-3-5--44--4-----5--5----4---6----"))
        do_moves(&c, "B'D2B");
    else if (is_of_type(c, "-1-111111-22-2---4333-3-1--44--4-----5--5----5---6----"))
        do_moves(&c, "RD2R'");
    else if (is_of_type(c, "-1-111111-22-2---5333-3-4--44--4-----5--5----1---6----"))
        do_moves(&c, "RF'RFR2");
    else if (is_of_type(c, "-1-111111-22-2----333-3---144--4-5---5--5------4-6----"))
        do_moves(&c, "B'DB");
    else if (is_of_type(c, "-1-111111-22-2----333-3---444--4-1---5--5------5-6----"))
        do_moves(&c, "DB'D'B");
    else if (is_of_type(c, "-1-111111-22-2----333-3---544--4-4---5--5------1-6----"))
        do_moves(&c, "FR2F'R2F'");
    else {
        not_found_solution();
        goto notfoundsolution;
    }


    if (is_of_type(c, "111111111222-2----333-3----444-4----555-5--------6----"));
    else if (is_of_type(c, "211111111522-2----333-3----444-4----551-5--------6----"))
        do_moves(&c, "LB'L'B2DB'");
    else if (is_of_type(c, "511111111122-2----333-3----444-4----552-5--------6----"))
        do_moves(&c, "BLB2L'DB'");
    else if (is_of_type(c, "-11111111-22-2-1--333-3----444-4----55--5---5----6-2--"))
        do_moves(&c, "L'D'L");
    else if (is_of_type(c, "-11111111-22-2-2--333-3----444-4----55--5---1----6-5--"))
        do_moves(&c, "BDB'");
    else if (is_of_type(c, "-11111111-22-2-5--333-3----444-4----55--5---2----6-1--"))
        do_moves(&c, "BLB2L'B'");
    else if (is_of_type(c, "-11111111-22-2---5333-3-1--444-4----55--5----2---6----"))
        do_moves(&c, "BD'B'");
    else if (is_of_type(c, "-11111111-22-2---2333-3-5--444-4----55--5----1---6----"))
        do_moves(&c, "F'L2FL2F");
    else if (is_of_type(c, "-11111111-22-2---1333-3-2--444-4----55--5----5---6----"))
        do_moves(&c, "DL'D2L");
    else if (is_of_type(c, "-11111111-22-2----333-3---1444-4-2--55--5------5-6----"))
        do_moves(&c, "L'D2L");
    else if (is_of_type(c, "-11111111-22-2----333-3---5444-4-1--55--5------2-6----"))
        do_moves(&c, "BD2B'");
    else if (is_of_type(c, "-11111111-22-2----333-3---2444-4-5--55--5------1-6----"))
        do_moves(&c, "BR'BRB2");
    else if (is_of_type(c, "-11111111-22-2----333-3----444-4---255--5-5------6---1"))
        do_moves(&c, "LU2R'U2L'");
    else if (is_of_type(c, "-11111111-22-2----333-3----444-4---155--5-2------6---5"))
        do_moves(&c, "L'DL");
    else if (is_of_type(c, "-11111111-22-2----333-3----444-4---555--5-1------6---2"))
        do_moves(&c, "DL'D'L");
    else {
        not_found_solution();
        goto notfoundsolution;
    }



edges_second_layer:
    if (is_of_type(c, "111111111222222---333333---444444---555555-------6----"))
        goto cross_third_layer;

    if (is_of_type(c, "111111111222-22---33333----444-4----555-5--------6----"));
    else if (is_of_type(c, "111111111222-23---33323----444-4----555-5--------6----"))
        do_moves(&c, "R2U2B'L2U'LU'R2F'");
    else if (is_of_type(c, "111111111222-2----333-33---44424----555-5--------6----"))
        do_moves(&c, "L2DU'F2U");
    else if (is_of_type(c, "111111111222-2----333-32---44434----555-5--------6----"))
        do_moves(&c, "FDRF'R'F'");
    else if (is_of_type(c, "111111111222-2----333-3----444-43---55525--------6----"))
        do_moves(&c, "L2D'FUB2LU'F'");
    else if (is_of_type(c, "111111111222-2----333-3----444-42---55535--------6----"))
        do_moves(&c, "R2D'UF2U'");
    else if (is_of_type(c, "11111111122222----333-3----444-4----555-53-------6----"))
        do_moves(&c, "U'L2UD'B2");
    else if (is_of_type(c, "11111111122232----333-3----444-4----555-52-------6----"))
        do_moves(&c, "LFLF'D'L'");
    else if (is_of_type(c, "111111111222-2----333-3--3-444-4----555-5-----2--6----"))
        do_moves(&c, "F2D2FD2F2");
    else if (is_of_type(c, "111111111222-2----333-3--2-444-4----555-5-----3--6----"))
        do_moves(&c, "DL2D2LD2L2");
    else if (is_of_type(c, "111111111222-2--2-333-3----444-4----555-5-------36----"))
        do_moves(&c, "L2D2L'D2L2");
    else if (is_of_type(c, "111111111222-2--3-333-3----444-4----555-5-------26----"))
        do_moves(&c, "DF2D2FD2F2");
    else if (is_of_type(c, "111111111222-2----333-3----444-4----555-5--3-----6--2-"))
        do_moves(&c, "F2D2F'D2F2");
    else if (is_of_type(c, "111111111222-2----333-3----444-4----555-5--2-----6--3-"))
        do_moves(&c, "DL2D2L'D2L2");
    else if (is_of_type(c, "111111111222-2----333-3----444-4--2-555-5--------63---"))
        do_moves(&c, "L2D2LD2L2");
    else if (is_of_type(c, "111111111222-2----333-3----444-4--3-555-5--------62---"))
        do_moves(&c, "DF2D2F'D2F2");
    else {
        not_found_solution();
        goto notfoundsolution;
    }


    if (is_of_type(c, "111111111222-22---333333---44444----555-5--------6----"));
    else if (is_of_type(c, "111111111222-22---333334---44434----555-5--------6----"))
        do_moves(&c, "L2U2B'D2R'D'R'U2F'");
    else if (is_of_type(c, "111111111222-22---33333----444-44---55535--------6----"))
        do_moves(&c, "UR2DU'B2");
    else if (is_of_type(c, "111111111222-22---33333----444-43---55545--------6----"))
        do_moves(&c, "RDBR'B'R'");
    else if (is_of_type(c, "111111111222422---33333----444-4----555-53-------6----"))
        do_moves(&c, "U'B2UD'R2");
    else if (is_of_type(c, "111111111222322---33333----444-4----555-54-------6----"))
        do_moves(&c, "R'F2U2L'U2F'DF'");
    else if (is_of_type(c, "111111111222-22---33333--3-444-4----555-5-----4--6----"))
        do_moves(&c, "L2U2B'R2U2L2F'");
    else if (is_of_type(c, "111111111222-22---33333--4-444-4----555-5-----3--6----"))
        do_moves(&c, "DL2U2RU2L2");
    else if (is_of_type(c, "111111111222-22---33333----444-4--4-555-5--------63---"))
        do_moves(&c, "L2U2RU2L2");
    else if (is_of_type(c, "111111111222-22---33333----444-4--3-555-5--------64---"))
        do_moves(&c, "R'D2RD2FDF'");
    else if (is_of_type(c, "111111111222-22---33333----444-4----555-5--3-----6--4-"))
        do_moves(&c, "FU2RD2R'U2F'");
    else if (is_of_type(c, "111111111222-22---33333----444-4----555-5--4-----6--3-"))
        do_moves(&c, "DR2D2R'D2R2");
    else if (is_of_type(c, "111111111222-22-4-33333----444-4----555-5-------36----"))
        do_moves(&c, "R2D2R'D2R2");
    else if (is_of_type(c, "111111111222-22-3-33333----444-4----555-5-------46----"))
        do_moves(&c, "FU2RDR'U2F'");
    else {
        not_found_solution();
        goto notfoundsolution;
    }

    if (is_of_type(c, "111111111222-22---333333---444444---55555--------6----"));
    else if (is_of_type(c, "111111111222-22---333333---444445---55545--------6----"))
        do_moves(&c, "R2D2BR2B'D2R'DR'");
    else if (is_of_type(c, "111111111222422---333333---44444----555-55-------6----"))
        do_moves(&c, "F2U2B2U2F2");
    else if (is_of_type(c, "111111111222522---333333---44444----555-54-------6----"))
        do_moves(&c, "BDLB'L'B'");
    else if (is_of_type(c, "111111111222-22---333333---44444--4-555-5--------65---"))
        do_moves(&c, "F2U2L'B2U2F2R'");
    else if (is_of_type(c, "111111111222-22---333333---44444--5-555-5--------64---"))
        do_moves(&c, "D'F2U2B'U2F2");
    else if (is_of_type(c, "111111111222-22---333333---44444----555-5--5-----6--4-"))
        do_moves(&c, "F2U2BU2F2");
    else if (is_of_type(c, "111111111222-22---333333---44444----555-5--4-----6--5-"))
        do_moves(&c, "B'D2BD2RDR'");
    else if (is_of_type(c, "111111111222-22-4-333333---44444----555-5-------56----"))
        do_moves(&c, "RU2BD2B'U2R'");
    else if (is_of_type(c, "111111111222-22-5-333333---44444----555-5-------46----"))
        do_moves(&c, "DF2U2B'U2F2");
    else if (is_of_type(c, "111111111222-22---333333-4-44444----555-5-----5--6----"))
        do_moves(&c, "RU2BDB'U2R'");
    else if (is_of_type(c, "111111111222-22---333333-5-44444----555-5-----4--6----"))
        do_moves(&c, "F2U2B'U2F2");
    else {
        not_found_solution();
        goto notfoundsolution;
    }


    if (is_of_type(c, "111111111222222---333333---444444---555555-------6----"));
    else if (is_of_type(c, "111111111222522---333333---444444---555552-------6----"))
        do_moves(&c, "B2D2LB2L'D2B'DB'");
    else if (is_of_type(c, "111111111222-22-5-333333---444444---55555-------26----"))
        do_moves(&c, "R'B2D'BDB2R");
    else if (is_of_type(c, "111111111222-22-2-333333---444444---55555-------56----"))
        do_moves(&c, "FL2DLD'L2F'");
    else if (is_of_type(c, "111111111222-22---333333-2-444444---55555-----5--6----"))
        do_moves(&c, "RF2UL'U'F2R'");
    else if (is_of_type(c, "111111111222-22---333333-5-444444---55555-----2--6----"))
        do_moves(&c, "F'D'F2UL2U'F'");
    else if (is_of_type(c, "111111111222-22---333333---444444-5-55555--------62---"))
        do_moves(&c, "F'R2U'BUR2F");
    else if (is_of_type(c, "111111111222-22---333333---444444-2-55555--------65---"))
        do_moves(&c, "RF2UL2U'F2R'");
    else if (is_of_type(c, "111111111222-22---333333---444444---55555--5-----6--2-"))
        do_moves(&c, "F'R2U'B'UR2F");
    else if (is_of_type(c, "111111111222-22---333333---444444---55555--2-----6--5-"))
        do_moves(&c, "FL2DL'D'L2F'");
    else {
        not_found_solution();
        goto notfoundsolution;
    }

    for (t = 0; t < 4; t++){
        do_move(&c, MOVE_D);
        if (is_of_type(c, "111111111222222-6-333333---444444-6-555555-6--6--6----") ||
            is_of_type(c, "111111111222222---333333-6-444444---555555------666-6-")){
            not_found_solution();
            goto notfoundsolution;
        }
    }
    t=0;
cross_third_layer:
    if (is_of_type(c, "111111111222222---333333---444444---555555----6-666-6-"))
        goto corners_orientation_third_layer;
    else if (is_of_type(c, "111111111222222---333333---444444-6-555555-6--6-66----"))
        do_moves(&c, "BDRD'R'B'");
    else if (is_of_type(c, "111111111222222-6-333333---444444---555555-6--6--66---"))
        do_moves(&c, "LDBD'B'L'");
    else if (is_of_type(c, "111111111222222-6-333333-6-444444---555555-------66-6-"))
        do_moves(&c, "FDLD'L'F'");
    else if (is_of_type(c, "111111111222222---333333-6-444444-6-555555------66--6-"))
        do_moves(&c, "RDFD'F'R'");
    else{
        do_moves(&c, "BDRD'R'B'");
        goto cross_third_layer;
    }

corners_orientation_third_layer:
    if (is_of_type(c, "111111111222222---333333---444444---555555---666666666"))
        goto fix_corners_third_layer;
    else if (is_of_type(c, "111111111222222---333333--6444444--6555555--666-666-6-"))
        do_moves(&c, "FD2F'D'FD'F'");
    else if (is_of_type(c, "1111111112222226--333333---4444446--5555556--66-666-6-"))
        do_moves(&c, "BD'F'DB'D'F");
    else if (is_of_type(c, "111111111222222--6333333---444444--6555555--6-66666-6-"))
        do_moves(&c, "B'DFD'BDF'");
    else if (is_of_type(c, "1111111112222226--3333336--444444---5555556---66666-6-"))
        do_moves(&c, "F'D2FDF'DF");
    else if (is_of_type(c, "1111111112222226--3333336--4444446--555555----6-666-66"))
        do_moves(&c, "FDF'DFD2F'");
    else if (is_of_type(c, "111111111222222--6333333--6444444---555555--6-6-666-66"))
        do_moves(&c, "L'DRD'LDR'");
    else if (is_of_type(c, "111111111222222--6333333--6444444--6555555----6-66666-"))
        do_moves(&c, "F'D'FD'F'D2F");
    else if (is_of_type(c, "111111111222222---3333336--4444446--5555556---6-66666-"))
        do_moves(&c, "RDR'DRD2R'");
    else if (is_of_type(c, "111111111222222---333333--6444444---5555556--66-66666-"))
        do_moves(&c, "RFL'F'R'FLF'");
    else if (is_of_type(c, "1111111112222226--333333---444444--6555555---666666-6-"))
        do_moves(&c, "BRF'R'B'RFR'");
    else if (is_of_type(c, "111111111222222---3333336--444444---555555--6-66666-66"))
        do_moves(&c, "L'F'RFLF'R'F");
    else if (is_of_type(c, "111111111222222--6333333---4444446--555555----6-666666"))
        do_moves(&c, "LBL'FLB'L'F'");
    else if (is_of_type(c, "1111111112222226--333333--6444444---555555---66-666-66"))
        do_moves(&c, "BRFR'B'RF'R'");
    else if (is_of_type(c, "111111111222222---333333---4444446--555555--666-666-66"))
        do_moves(&c, "L'F'R'FLF'RF");
    else if (is_of_type(c, "111111111222222---3333336--444444--6555555----6666666-"))
        do_moves(&c, "RF'R'BRFR'B'");
    else if (is_of_type(c, "111111111222222--6333333---444444---5555556---6666666-"))
        do_moves(&c, "RFLF'R'FL'F'");
    else if (is_of_type(c, "111111111222222---3333336-6444444---555555----6-666666"))
        do_moves(&c, "R2B2R'F2RB2R'F2R'");
    else if (is_of_type(c, "111111111222222---333333---4444446-6555555---66-66666-"))
        do_moves(&c, "F2UF'D2FU'F'D2F'");
    else if (is_of_type(c, "111111111222222---333333---444444---5555556-6666666-6-"))
        do_moves(&c, "R2UR'D2RU'R'D2R'");
    else if (is_of_type(c, "1111111112222226-6333333---444444---555555----66666-66"))
        do_moves(&c, "F2R2F'L2FR2F'L2F'");
    else if (is_of_type(c, "1111111112222226-6333333--6444444---5555556---6-666-6-"))
        do_moves(&c, "R'D2R2DR2DR2D2R'");
    else if (is_of_type(c, "1111111112222226--3333336-6444444--6555555----6-666-6-"))
        do_moves(&c, "B'D2B2DB2DB2D2B'");
    else if (is_of_type(c, "111111111222222---3333336--4444446-6555555--6-6-666-6-"))
        do_moves(&c, "L'D2L2DL2DL2D2L'");
    else if (is_of_type(c, "111111111222222--6333333---4444446--5555556-6-6-666-6-"))
        do_moves(&c, "F'D2F2DF2DF2D2F'");
    else if (is_of_type(c, "111111111222222---3333336-6444444---5555556-6-6-666-6-"))
        do_moves(&c, "B'D'BD'B'D'FD'BDF'");
    else if (is_of_type(c, "1111111112222226-6333333---4444446-6555555----6-666-6-"))
        do_moves(&c, "L'D'LD'L'D'RD'LDR'");
    else {
        not_found_solution();
        goto notfoundsolution;
    }

fix_corners_third_layer:
    if (is_of_type(c, "1111111112222222-23333333-34444444-45555555-5666666666"))
        goto fix_edges_third_layer;
    else if (is_of_type(c, "1111111112222225-23333333-54444442-35555554-4666666666"))
        do_moves(&c, "RF'RB2R'FRB2R2");
    else if (is_of_type(c, "1111111112222224-23333333-44444445-55555552-3666666666"))
        do_moves(&c, "R2B2R'F'RB2R'FR'");
    else if (is_of_type(c, "1111111112222225-53333332-34444444-25555553-4666666666"))
        do_moves(&c, "L2B2LFL'B2LF'L");
    else if (is_of_type(c, "1111111112222223-43333335-34444444-55555552-2666666666"))
        do_moves(&c, "B2L2B'R'BL2B'RB'");
    else if (is_of_type(c, "1111111112222224-53333332-24444443-45555555-3666666666"))
        do_moves(&c, "F2L2FRF'L2FR'F");
    else if (is_of_type(c, "1111111112222223-33333334-54444442-45555555-2666666666"))
        do_moves(&c, "F'RF'L2FR'F'L2F2");
    else if (is_of_type(c, "1111111112222222-43333335-24444443-35555554-5666666666"))
        do_moves(&c, "R2F2RBR'F2RB'R");
    else if (is_of_type(c, "1111111112222222-33333334-44444445-25555553-5666666666"))
        do_moves(&c, "F2R2F'L'FR2F'LF'");
    else if (is_of_type(c, "1111111112222223-53333332-44444445-35555554-2666666666"))
        do_moves(&c, "BD'FU2F'DB'FD'BU2B'DF'");
    else if (is_of_type(c, "1111111112222225-33333334-24444443-55555552-4666666666"))
        do_moves(&c, "LD'RU2R'DL'RD'LU2L'DR'");
    else{
        if (t == 3){
            not_found_solution();
            goto notfoundsolution;
        }
        do_moves(&c, "D");
        t++;
        goto fix_corners_third_layer;
    }
    t = 0;

fix_edges_third_layer:

    if (is_of_type(c, "111111111222222222333333333444444444555555555666666666"));
    else if (is_of_type(c, "111111111222222242333333333444444454555555525666666666"))
        do_moves(&c, "B2DLR'B2L'RDB2");
    else if (is_of_type(c, "111111111222222252333333333444444424555555545666666666"))
        do_moves(&c, "B2D'LR'B2L'RD'B2");
    else if (is_of_type(c, "111111111222222252333333323444444444555555535666666666"))
        do_moves(&c, "L2D'B'FL2BF'D'L2");
    else if (is_of_type(c, "111111111222222232333333353444444444555555525666666666"))
        do_moves(&c, "L2DB'FL2BF'DL2");
    else if (is_of_type(c, "111111111222222242333333323444444434555555555666666666"))
        do_moves(&c, "F2D'L'RF2LR'D'F2");
    else if (is_of_type(c, "111111111222222232333333343444444424555555555666666666"))
        do_moves(&c, "F2DL'RF2LR'DF2");
    else if (is_of_type(c, "111111111222222222333333353444444434555555545666666666"))
        do_moves(&c, "R2D'BF'R2B'FD'R2");
    else if (is_of_type(c, "111111111222222222333333343444444454555555535666666666"))
        do_moves(&c, "R2DBF'R2B'FDR2");
    else if (is_of_type(c, "111111111222222242333333353444444424555555535666666666"))
        do_moves(&c, "L'R'D2LRBFD2B'F'");
    else if (is_of_type(c, "111111111222222252333333343444444434555555525666666666"))
        do_moves(&c, "BR'B'LRB'LR'B'RBL2D");
    else if (is_of_type(c, "111111111222222232333333323444444454555555545666666666"))
        do_moves(&c, "B2LFL'BF'L'BFL'F'LD");
    else {
        not_found_solution();
        goto notfoundsolution;
    }

    optimize();
    t = moveslen();
notfoundsolution:
    return t;
}
int solve(cube Start){
    cube CStart = Start;
    int sol, best;
    char * BestSolution = NULL;

    sol = best = _solve(CStart);
    BestSolution = (char*)realloc(BestSolution, strlen(moves) + 1);
    strcpy(BestSolution, moves);

    CStart = Start;
    rot_and_swap(&CStart, FACE_DOWN);
    sol = _solve(CStart);
    if (sol < best){
        rot_moves(moves, COUNTERCLOCKWISE);
        rot_moves(moves, COUNTERCLOCKWISE);
        BestSolution = (char*)realloc(BestSolution, strlen(moves) + 1);
        strcpy(BestSolution, moves);
        best = sol;
    }

    CStart = Start;
    rot_and_swap(&CStart, FACE_LEFT);
    sol = _solve(CStart);
    if (sol < best){
        rot_moves(moves, COUNTERCLOCKWISE);
        BestSolution = (char*)realloc(BestSolution, strlen(moves) + 1);
        strcpy(BestSolution, moves);
        best = sol;
    }

    CStart = Start;
    rot_and_swap(&CStart, FACE_RIGHT);
    sol = _solve(CStart);
    if (sol < best){
        rot_moves(moves, CLOCKWISE);
        BestSolution = (char*)realloc(BestSolution, strlen(moves) + 1);
        strcpy(BestSolution, moves);
        best = sol;
    }

    CStart = Start;
    rot_and_swap(&CStart, FACE_FRONT);
    sol = _solve(CStart);
    if (sol < best){
        rot_moves(moves, DIR_DOWN);
        BestSolution = (char*)realloc(BestSolution, strlen(moves) + 1);
        strcpy(BestSolution, moves);
        best = sol;
    }

    CStart = Start;
    rot_and_swap(&CStart, FACE_BACK);
    sol = _solve(CStart);
    if (sol < best){
        rot_moves(moves, DIR_UP);
        BestSolution = (char*)realloc(BestSolution, strlen(moves) + 1);
        strcpy(BestSolution, moves);
        best = sol;
    }

    moves = (char *)realloc(moves, strlen(BestSolution) + 1);
    strcpy(moves, BestSolution);

    printf("\n\n"
           "+-----------------------------------+\n"
           "|   FOUND A SOLUTION IN %3d MOVES   |\n"
           "+-----------------------------------+\n"
           "%s\n"
           "-------------------------------------\n",
           moveslen(), moves);
    return best;
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height) {

    if (height == 0){
        height = 1;
    }

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
int InitGL(GLvoid){
    glShadeModel(GL_SMOOTH);
    glClearColor((GLfloat)BK_RED, (GLfloat)BK_GREEN, (GLfloat)BK_BLUE, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  LightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  LightDiffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    return 1;
}
int Color(int color){
    GLfloat r = (float)((color >> 16) & 0x000000FF) / 255.0,
            g = (float)((color >>  8) & 0x000000FF) / 255.0,
            b = (float)( color        & 0x000000FF) / 255.0;
    glColor3f(r, g, b);

    return 0;
}
int DrawCube(int u, int d, int l, int r, int f, int b){
    int c;
    glPushMatrix();
    glBegin(GL_QUADS);

        Color(f);
        glNormal3f( 0.0f, 0.0f, 1.0f);
        glVertex3d( 0.45f,  0.45f,  0.5f);
        glVertex3d(-0.45f,  0.45f,  0.5f);
        glVertex3d(-0.45f, -0.45f,  0.5f);
        glVertex3d( 0.45f, -0.45f,  0.5f);

        Color(b);
        glNormal3f( 0.0f, 0.0f, -1.0f);
        glVertex3d( 0.45f,  0.45f, -0.5f);
        glVertex3d(-0.45f,  0.45f, -0.5f);
        glVertex3d(-0.45f, -0.45f, -0.5f);
        glVertex3d( 0.45f, -0.45f, -0.5f);

        Color(u);
        glNormal3f( 0.0f, 1.0f, 0.0f);
        glVertex3d( 0.45f,  0.5f, -0.45f);
        glVertex3d(-0.45f,  0.5f, -0.45f);
        glVertex3d(-0.45f,  0.5f,  0.45f);
        glVertex3d( 0.45f,  0.5f,  0.45f);

        Color(d);
        glNormal3f( 0.0f, -1.0f, 0.0f);
        glVertex3d( 0.45f, -0.5f, -0.45f);
        glVertex3d(-0.45f, -0.5f, -0.45f);
        glVertex3d(-0.45f, -0.5f,  0.45f);
        glVertex3d( 0.45f, -0.5f,  0.45f);

        Color(r);
        glNormal3f( 1.0f, 0.0f, 0.0f);
        glVertex3d( 0.5f,  0.45f, -0.45f);
        glVertex3d( 0.5f, -0.45f, -0.45f);
        glVertex3d( 0.5f, -0.45f,  0.45f);
        glVertex3d( 0.5f,  0.45f,  0.45f);

        Color(l);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3d(-0.5f,  0.45f, -0.45f);
        glVertex3d(-0.5f, -0.45f, -0.45f);
        glVertex3d(-0.5f, -0.45f,  0.45f);
        glVertex3d(-0.5f,  0.45f,  0.45f);
    glEnd();
    Color(DARKGRAY);
    for(c = 0; c < 12; c++){
        if (c > 8)
            glRotatef(90.0f, 1.0, 0.0, 0.0);
        else if (c == 8)
            glRotatef(90.0f, 0.0, 0.0, 1.0);
        else if (c == 4)
            glRotatef(180.0f, 1.0, 0.0, 0.0);
        else
            glRotatef(90.0f, 0.0, 1.0, 0.0);
        glBegin(GL_QUADS);

            glNormal3f( 0.0f, 1.0f, 1.0f);
            glVertex3d( 0.45f,  0.45f,  0.5f);
            glVertex3d(-0.45f,  0.45f,  0.5f);
            glVertex3d(-0.45f,  0.5f,  0.45f);
            glVertex3d( 0.45f,  0.5f,  0.45f);
        glEnd();
    }

    for (c = 0; c < 8; c++){
        if (c == 4)
            glRotatef(180.0f, 1.0, 0.0, 0.0);
        else
            glRotatef(90.0f, 0.0, 1.0, 0.0);
        glBegin(GL_TRIANGLES);

            glNormal3f( 1.0f, 1.0f, 1.0f);
            glVertex3d( 0.45f,  0.45f,  0.5f);
            glVertex3d( 0.45f,  0.5f,  0.45f);
            glVertex3d( 0.5f,  0.45f,  0.45f);
        glEnd();
    }
    glPopMatrix();

    return 0;
}
void DrawGLScene(GLvoid){
    static float alpha = -45.000;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0, 0.0, -6.0);
    glRotatef(xAngle, 1.0, 0.0, 0.0);
    glRotatef(yAngle, 0.0, 1.0, 0.0);

    switch(CurrentMove){
    case MOVE_NONE: {
        glTranslatef( 1.0, -1.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.ne), DARKGRAY, getcolor_i(MainCube.right.sw), getcolor_i(MainCube.front.se), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.n), DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.s), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.nw), getcolor_i(MainCube.left.se), DARKGRAY, getcolor_i(MainCube.front.sw), DARKGRAY);
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.w), getcolor_i(MainCube.front.e), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.center), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.e), DARKGRAY, getcolor_i(MainCube.front.w), DARKGRAY);
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.se), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.nw), getcolor_i(MainCube.front.ne), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.n), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.sw), DARKGRAY, getcolor_i(MainCube.left.ne), DARKGRAY, getcolor_i(MainCube.front.nw), DARKGRAY);

        glTranslatef(2.0, -2.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.e), DARKGRAY, getcolor_i(MainCube.right.s), DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.w), getcolor_i(MainCube.left.s), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.center), DARKGRAY, DARKGRAY);
        glTranslatef(-2.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.center), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.e), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.n), DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.w), DARKGRAY, getcolor_i(MainCube.left.n), DARKGRAY, DARKGRAY, DARKGRAY);

        glTranslatef(2.0, -2.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.se), DARKGRAY, getcolor_i(MainCube.right.se), DARKGRAY, getcolor_i(MainCube.back.sw));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.s));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.sw), getcolor_i(MainCube.left.sw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.se));
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.e), DARKGRAY, getcolor_i(MainCube.back.w));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.center));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.w), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.e));
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.ne), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.ne), DARKGRAY, getcolor_i(MainCube.back.nw));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.n), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.n));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.nw), DARKGRAY, getcolor_i(MainCube.left.nw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.ne));
    } break;
    case MOVE_B: {
        glTranslatef( 1.0, -1.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.ne), DARKGRAY, getcolor_i(MainCube.right.sw), getcolor_i(MainCube.front.se), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.n), DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.s), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.nw), getcolor_i(MainCube.left.se), DARKGRAY, getcolor_i(MainCube.front.sw), DARKGRAY);
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.w), getcolor_i(MainCube.front.e), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.center), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.e), DARKGRAY, getcolor_i(MainCube.front.w), DARKGRAY);
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.se), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.nw), getcolor_i(MainCube.front.ne), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.n), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.sw), DARKGRAY, getcolor_i(MainCube.left.ne), DARKGRAY, getcolor_i(MainCube.front.nw), DARKGRAY);

        glTranslatef(2.0, -2.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.e), DARKGRAY, getcolor_i(MainCube.right.s), DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.w), getcolor_i(MainCube.left.s), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.center), DARKGRAY, DARKGRAY);
        glTranslatef(-2.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.center), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.e), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.n), DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.w), DARKGRAY, getcolor_i(MainCube.left.n), DARKGRAY, DARKGRAY, DARKGRAY);

        glTranslatef(1.0, -1.0, 0.0);
        glRotatef(sin(alpha*PI/90)*45+45.0, 0.0, 0.0, 1.0);

        glTranslatef(1.0, -1.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.se), DARKGRAY, getcolor_i(MainCube.right.se), DARKGRAY, getcolor_i(MainCube.back.sw));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.s));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.sw), getcolor_i(MainCube.left.sw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.se));
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.e), DARKGRAY, getcolor_i(MainCube.back.w));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.center));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.w), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.e));
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.ne), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.ne), DARKGRAY, getcolor_i(MainCube.back.nw));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.n), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.n));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.nw), DARKGRAY, getcolor_i(MainCube.left.nw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.ne));
        alpha += INCREMENT;
        if (alpha >= 45.0){
            alpha = -45.0;
            do_moves(&MainCube, "B");
            CurrentMove = MOVE_NONE;
        }
        } break;
    case MOVE_B2: {
        glTranslatef( 1.0, -1.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.ne), DARKGRAY, getcolor_i(MainCube.right.sw), getcolor_i(MainCube.front.se), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.n), DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.s), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.nw), getcolor_i(MainCube.left.se), DARKGRAY, getcolor_i(MainCube.front.sw), DARKGRAY);
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.w), getcolor_i(MainCube.front.e), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.center), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.e), DARKGRAY, getcolor_i(MainCube.front.w), DARKGRAY);
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.se), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.nw), getcolor_i(MainCube.front.ne), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.n), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.sw), DARKGRAY, getcolor_i(MainCube.left.ne), DARKGRAY, getcolor_i(MainCube.front.nw), DARKGRAY);

        glTranslatef(2.0, -2.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.e), DARKGRAY, getcolor_i(MainCube.right.s), DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.w), getcolor_i(MainCube.left.s), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.center), DARKGRAY, DARKGRAY);
        glTranslatef(-2.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.center), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.e), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.n), DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.w), DARKGRAY, getcolor_i(MainCube.left.n), DARKGRAY, DARKGRAY, DARKGRAY);

        glTranslatef(1.0, -1.0, 0.0);
        glRotatef(sin(alpha*PI/90)*90+90.0, 0.0, 0.0, 1.0);

        glTranslatef(1.0, -1.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.se), DARKGRAY, getcolor_i(MainCube.right.se), DARKGRAY, getcolor_i(MainCube.back.sw));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.s));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.sw), getcolor_i(MainCube.left.sw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.se));
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.e), DARKGRAY, getcolor_i(MainCube.back.w));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.center));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.w), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.e));
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.ne), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.ne), DARKGRAY, getcolor_i(MainCube.back.nw));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.n), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.n));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.nw), DARKGRAY, getcolor_i(MainCube.left.nw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.ne));
        alpha += INCREMENT;
        if (alpha >= 45.0){
            alpha = -45.0;
            do_moves(&MainCube, "B2");
            CurrentMove = MOVE_NONE;
        }
        } break;
    case MOVE_Bi: {
        glTranslatef( 1.0, -1.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.ne), DARKGRAY, getcolor_i(MainCube.right.sw), getcolor_i(MainCube.front.se), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.n), DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.s), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.nw), getcolor_i(MainCube.left.se), DARKGRAY, getcolor_i(MainCube.front.sw), DARKGRAY);
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.w), getcolor_i(MainCube.front.e), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.center), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.e), DARKGRAY, getcolor_i(MainCube.front.w), DARKGRAY);
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.se), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.nw), getcolor_i(MainCube.front.ne), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.n), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.sw), DARKGRAY, getcolor_i(MainCube.left.ne), DARKGRAY, getcolor_i(MainCube.front.nw), DARKGRAY);

        glTranslatef(2.0, -2.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.e), DARKGRAY, getcolor_i(MainCube.right.s), DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.w), getcolor_i(MainCube.left.s), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.center), DARKGRAY, DARKGRAY);
        glTranslatef(-2.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.center), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.e), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.n), DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.w), DARKGRAY, getcolor_i(MainCube.left.n), DARKGRAY, DARKGRAY, DARKGRAY);

        glTranslatef(1.0, -1.0, 0.0);
        glRotatef(-sin(alpha*PI/90)*45.0 -45.0, 0.0, 0.0, 1.0);

        glTranslatef(1.0, -1.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.se), DARKGRAY, getcolor_i(MainCube.right.se), DARKGRAY, getcolor_i(MainCube.back.sw));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.s));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.sw), getcolor_i(MainCube.left.sw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.se));
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.e), DARKGRAY, getcolor_i(MainCube.back.w));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.center));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.w), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.e));
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.ne), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.ne), DARKGRAY, getcolor_i(MainCube.back.nw));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.n), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.n));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.nw), DARKGRAY, getcolor_i(MainCube.left.nw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.ne));
        alpha += INCREMENT;
        if (alpha >= 45.0){
            alpha = -45.0;
            do_moves(&MainCube, "B'");
            CurrentMove = MOVE_NONE;
        }
        } break;
    case MOVE_F: {
        glTranslatef(1.0, -1.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.se), DARKGRAY, getcolor_i(MainCube.right.se), DARKGRAY, getcolor_i(MainCube.back.sw));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.s));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.sw), getcolor_i(MainCube.left.sw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.se));
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.e), DARKGRAY, getcolor_i(MainCube.back.w));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.center));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.w), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.e));
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.ne), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.ne), DARKGRAY, getcolor_i(MainCube.back.nw));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.n), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.n));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.nw), DARKGRAY, getcolor_i(MainCube.left.nw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.ne));

        glTranslatef(2.0, -2.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.e), DARKGRAY, getcolor_i(MainCube.right.s), DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.w), getcolor_i(MainCube.left.s), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.center), DARKGRAY, DARKGRAY);
        glTranslatef(-2.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.center), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.e), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.n), DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.w), DARKGRAY, getcolor_i(MainCube.left.n), DARKGRAY, DARKGRAY, DARKGRAY);

        glTranslatef(1.0, -1.0, 0.0);
        glRotatef(-sin(alpha*PI/90)*45.0 - 45.0, 0.0, 0.0, 1.0);

        glTranslatef( 1.0, -1.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.ne), DARKGRAY, getcolor_i(MainCube.right.sw), getcolor_i(MainCube.front.se), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.n), DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.s), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.nw), getcolor_i(MainCube.left.se), DARKGRAY, getcolor_i(MainCube.front.sw), DARKGRAY);
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.w), getcolor_i(MainCube.front.e), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.center), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.e), DARKGRAY, getcolor_i(MainCube.front.w), DARKGRAY);
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.se), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.nw), getcolor_i(MainCube.front.ne), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.n), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.sw), DARKGRAY, getcolor_i(MainCube.left.ne), DARKGRAY, getcolor_i(MainCube.front.nw), DARKGRAY);
        alpha += INCREMENT;
        if (alpha >= 45.0){
            alpha = -45.0;
            do_moves(&MainCube, "F");
            CurrentMove = MOVE_NONE;
        }
        break;
    }
    case MOVE_F2: {
        glTranslatef(1.0, -1.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.se), DARKGRAY, getcolor_i(MainCube.right.se), DARKGRAY, getcolor_i(MainCube.back.sw));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.s));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.sw), getcolor_i(MainCube.left.sw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.se));
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.e), DARKGRAY, getcolor_i(MainCube.back.w));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.center));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.w), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.e));
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.ne), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.ne), DARKGRAY, getcolor_i(MainCube.back.nw));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.n), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.n));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.nw), DARKGRAY, getcolor_i(MainCube.left.nw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.ne));

        glTranslatef(2.0, -2.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.e), DARKGRAY, getcolor_i(MainCube.right.s), DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.w), getcolor_i(MainCube.left.s), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.center), DARKGRAY, DARKGRAY);
        glTranslatef(-2.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.center), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.e), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.n), DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.w), DARKGRAY, getcolor_i(MainCube.left.n), DARKGRAY, DARKGRAY, DARKGRAY);

        glTranslatef(1.0, -1.0, 0.0);
        glRotatef(-sin(alpha*PI/90)*90.0 - 90.0, 0.0, 0.0, 1.0);

        glTranslatef( 1.0, -1.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.ne), DARKGRAY, getcolor_i(MainCube.right.sw), getcolor_i(MainCube.front.se), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.n), DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.s), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.nw), getcolor_i(MainCube.left.se), DARKGRAY, getcolor_i(MainCube.front.sw), DARKGRAY);
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.w), getcolor_i(MainCube.front.e), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.center), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.e), DARKGRAY, getcolor_i(MainCube.front.w), DARKGRAY);
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.se), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.nw), getcolor_i(MainCube.front.ne), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.n), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.sw), DARKGRAY, getcolor_i(MainCube.left.ne), DARKGRAY, getcolor_i(MainCube.front.nw), DARKGRAY);
        alpha += INCREMENT;
        if (alpha >= 45.0){
            alpha = -45.0;
            do_moves(&MainCube, "F2");
            CurrentMove = MOVE_NONE;
        }
        break;
    }
    case MOVE_Fi: {
        glTranslatef(1.0, -1.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.se), DARKGRAY, getcolor_i(MainCube.right.se), DARKGRAY, getcolor_i(MainCube.back.sw));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.s));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.sw), getcolor_i(MainCube.left.sw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.se));
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.e), DARKGRAY, getcolor_i(MainCube.back.w));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.center));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.w), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.e));
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.ne), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.ne), DARKGRAY, getcolor_i(MainCube.back.nw));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.n), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.n));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.nw), DARKGRAY, getcolor_i(MainCube.left.nw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.ne));

        glTranslatef(2.0, -2.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.e), DARKGRAY, getcolor_i(MainCube.right.s), DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.w), getcolor_i(MainCube.left.s), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.center), DARKGRAY, DARKGRAY);
        glTranslatef(-2.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.center), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.e), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.n), DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.w), DARKGRAY, getcolor_i(MainCube.left.n), DARKGRAY, DARKGRAY, DARKGRAY);

        glTranslatef(1.0, -1.0, 0.0);
        glRotatef(sin(alpha*PI/90)*45.0 + 45.0, 0.0, 0.0, 1.0);

        glTranslatef( 1.0, -1.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.ne), DARKGRAY, getcolor_i(MainCube.right.sw), getcolor_i(MainCube.front.se), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.n), DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.s), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.nw), getcolor_i(MainCube.left.se), DARKGRAY, getcolor_i(MainCube.front.sw), DARKGRAY);
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.w), getcolor_i(MainCube.front.e), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.center), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.e), DARKGRAY, getcolor_i(MainCube.front.w), DARKGRAY);
        glTranslatef(2.0, 1.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.se), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.nw), getcolor_i(MainCube.front.ne), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.n), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.sw), DARKGRAY, getcolor_i(MainCube.left.ne), DARKGRAY, getcolor_i(MainCube.front.nw), DARKGRAY);
        alpha += INCREMENT;
        if (alpha >= 45.0){
            alpha = -45.0;
            do_moves(&MainCube, "F'");
            CurrentMove = MOVE_NONE;
        }
        break;
    }
    case MOVE_U: {
        glTranslatef(1.0, -1.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.se), DARKGRAY, getcolor_i(MainCube.right.se), DARKGRAY, getcolor_i(MainCube.back.sw));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.s));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.sw), getcolor_i(MainCube.left.sw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.se));
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.e), DARKGRAY, getcolor_i(MainCube.right.s), DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.w), getcolor_i(MainCube.left.s), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.ne), DARKGRAY, getcolor_i(MainCube.right.sw), getcolor_i(MainCube.front.se), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.n), DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.s), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.nw), getcolor_i(MainCube.left.se), DARKGRAY, getcolor_i(MainCube.front.sw), DARKGRAY);

        glTranslatef(2.0, 1.0, -2.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.e), DARKGRAY, getcolor_i(MainCube.back.w));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.center));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.w), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.e));
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.center), DARKGRAY, DARKGRAY);
        glTranslatef(-2.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.center), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.w), getcolor_i(MainCube.front.e), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.center), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.e), DARKGRAY, getcolor_i(MainCube.front.w), DARKGRAY);

        glTranslatef(1.0, 0.0, -1.0);
        glRotatef(-sin(alpha*PI/90)*45.0 - 45.0, 0.0, 1.0, 0.0);

        glTranslatef(1.0, 1.0, -1.0);
        DrawCube(getcolor_i(MainCube.up.ne), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.ne), DARKGRAY, getcolor_i(MainCube.back.nw));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.n), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.n));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.nw), DARKGRAY, getcolor_i(MainCube.left.nw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.ne));
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.e), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.n), DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.w), DARKGRAY, getcolor_i(MainCube.left.n), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.se), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.nw), getcolor_i(MainCube.front.ne), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.n), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.sw), DARKGRAY, getcolor_i(MainCube.left.ne), DARKGRAY, getcolor_i(MainCube.front.nw), DARKGRAY);

        alpha += INCREMENT;
        if (alpha >= 45.0){
            alpha = -45.0;
            do_moves(&MainCube, "U");
            CurrentMove = MOVE_NONE;
        }
        break;
    }
    case MOVE_U2: {
        glTranslatef(1.0, -1.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.se), DARKGRAY, getcolor_i(MainCube.right.se), DARKGRAY, getcolor_i(MainCube.back.sw));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.s));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.sw), getcolor_i(MainCube.left.sw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.se));
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.e), DARKGRAY, getcolor_i(MainCube.right.s), DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.w), getcolor_i(MainCube.left.s), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.ne), DARKGRAY, getcolor_i(MainCube.right.sw), getcolor_i(MainCube.front.se), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.n), DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.s), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.nw), getcolor_i(MainCube.left.se), DARKGRAY, getcolor_i(MainCube.front.sw), DARKGRAY);

        glTranslatef(2.0, 1.0, -2.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.e), DARKGRAY, getcolor_i(MainCube.back.w));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.center));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.w), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.e));
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.center), DARKGRAY, DARKGRAY);
        glTranslatef(-2.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.center), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.w), getcolor_i(MainCube.front.e), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.center), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.e), DARKGRAY, getcolor_i(MainCube.front.w), DARKGRAY);

        glTranslatef(1.0, 0.0, -1.0);
        glRotatef(-sin(alpha*PI/90)*90.0 - 90.0, 0.0, 1.0, 0.0);

        glTranslatef(1.0, 1.0, -1.0);
        DrawCube(getcolor_i(MainCube.up.ne), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.ne), DARKGRAY, getcolor_i(MainCube.back.nw));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.n), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.n));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.nw), DARKGRAY, getcolor_i(MainCube.left.nw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.ne));
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.e), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.n), DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.w), DARKGRAY, getcolor_i(MainCube.left.n), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.se), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.nw), getcolor_i(MainCube.front.ne), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.n), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.sw), DARKGRAY, getcolor_i(MainCube.left.ne), DARKGRAY, getcolor_i(MainCube.front.nw), DARKGRAY);

        alpha += INCREMENT;
        if (alpha >= 45.0){
            alpha = -45.0;
            do_moves(&MainCube, "U2");
            CurrentMove = MOVE_NONE;
        }
        break;
    }
    case MOVE_Ui: {
        glTranslatef(1.0, -1.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.se), DARKGRAY, getcolor_i(MainCube.right.se), DARKGRAY, getcolor_i(MainCube.back.sw));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.s));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.sw), getcolor_i(MainCube.left.sw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.se));
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.e), DARKGRAY, getcolor_i(MainCube.right.s), DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.w), getcolor_i(MainCube.left.s), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.ne), DARKGRAY, getcolor_i(MainCube.right.sw), getcolor_i(MainCube.front.se), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.n), DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.s), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.nw), getcolor_i(MainCube.left.se), DARKGRAY, getcolor_i(MainCube.front.sw), DARKGRAY);

        glTranslatef(2.0, 1.0, -2.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.e), DARKGRAY, getcolor_i(MainCube.back.w));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.center));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.w), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.e));
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.center), DARKGRAY, DARKGRAY);
        glTranslatef(-2.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.center), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.w), getcolor_i(MainCube.front.e), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.center), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.e), DARKGRAY, getcolor_i(MainCube.front.w), DARKGRAY);

        glTranslatef(1.0, 0.0, -1.0);
        glRotatef(sin(alpha*PI/90)*45.0 + 45.0, 0.0, 1.0, 0.0);

        glTranslatef(1.0, 1.0, -1.0);
        DrawCube(getcolor_i(MainCube.up.ne), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.ne), DARKGRAY, getcolor_i(MainCube.back.nw));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.n), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.n));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.nw), DARKGRAY, getcolor_i(MainCube.left.nw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.ne));
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.e), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.n), DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.w), DARKGRAY, getcolor_i(MainCube.left.n), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.se), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.nw), getcolor_i(MainCube.front.ne), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.n), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.sw), DARKGRAY, getcolor_i(MainCube.left.ne), DARKGRAY, getcolor_i(MainCube.front.nw), DARKGRAY);

        alpha += INCREMENT;
        if (alpha >= 45.0){
            alpha = -45.0;
            do_moves(&MainCube, "U'");
            CurrentMove = MOVE_NONE;
        }
        break;
    }
    case MOVE_D: {
        glTranslatef(1.0, 1.0, -1.0);
        DrawCube(getcolor_i(MainCube.up.ne), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.ne), DARKGRAY, getcolor_i(MainCube.back.nw));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.n), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.n));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.nw), DARKGRAY, getcolor_i(MainCube.left.nw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.ne));
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.e), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.n), DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.w), DARKGRAY, getcolor_i(MainCube.left.n), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.se), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.nw), getcolor_i(MainCube.front.ne), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.n), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.sw), DARKGRAY, getcolor_i(MainCube.left.ne), DARKGRAY, getcolor_i(MainCube.front.nw), DARKGRAY);

        glTranslatef(2.0, -1.0, -2.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.e), DARKGRAY, getcolor_i(MainCube.back.w));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.center));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.w), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.e));
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.center), DARKGRAY, DARKGRAY);
        glTranslatef(-2.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.center), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.w), getcolor_i(MainCube.front.e), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.center), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.e), DARKGRAY, getcolor_i(MainCube.front.w), DARKGRAY);

        glTranslatef(1.0, 0.0, -1.0);
        glRotatef(sin(alpha*PI/90)*45.0 + 45.0, 0.0, 1.0, 0.0);

        glTranslatef(1.0, -1.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.se), DARKGRAY, getcolor_i(MainCube.right.se), DARKGRAY, getcolor_i(MainCube.back.sw));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.s));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.sw), getcolor_i(MainCube.left.sw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.se));
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.e), DARKGRAY, getcolor_i(MainCube.right.s), DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.w), getcolor_i(MainCube.left.s), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.ne), DARKGRAY, getcolor_i(MainCube.right.sw), getcolor_i(MainCube.front.se), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.n), DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.s), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.nw), getcolor_i(MainCube.left.se), DARKGRAY, getcolor_i(MainCube.front.sw), DARKGRAY);


        alpha += INCREMENT;
        if (alpha >= 45.0){
            alpha = -45.0;
            do_moves(&MainCube, "D");
            CurrentMove = MOVE_NONE;
        }
        break;
    }
    case MOVE_D2: {
        glTranslatef(1.0, 1.0, -1.0);
        DrawCube(getcolor_i(MainCube.up.ne), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.ne), DARKGRAY, getcolor_i(MainCube.back.nw));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.n), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.n));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.nw), DARKGRAY, getcolor_i(MainCube.left.nw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.ne));
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.e), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.n), DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.w), DARKGRAY, getcolor_i(MainCube.left.n), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.se), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.nw), getcolor_i(MainCube.front.ne), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.n), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.sw), DARKGRAY, getcolor_i(MainCube.left.ne), DARKGRAY, getcolor_i(MainCube.front.nw), DARKGRAY);

        glTranslatef(2.0, -1.0, -2.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.e), DARKGRAY, getcolor_i(MainCube.back.w));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.center));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.w), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.e));
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.center), DARKGRAY, DARKGRAY);
        glTranslatef(-2.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.center), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.w), getcolor_i(MainCube.front.e), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.center), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.e), DARKGRAY, getcolor_i(MainCube.front.w), DARKGRAY);

        glTranslatef(1.0, 0.0, -1.0);
        glRotatef(sin(alpha*PI/90)*90.0 + 90.0, 0.0, 1.0, 0.0);

        glTranslatef(1.0, -1.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.se), DARKGRAY, getcolor_i(MainCube.right.se), DARKGRAY, getcolor_i(MainCube.back.sw));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.s));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.sw), getcolor_i(MainCube.left.sw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.se));
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.e), DARKGRAY, getcolor_i(MainCube.right.s), DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.w), getcolor_i(MainCube.left.s), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.ne), DARKGRAY, getcolor_i(MainCube.right.sw), getcolor_i(MainCube.front.se), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.n), DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.s), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.nw), getcolor_i(MainCube.left.se), DARKGRAY, getcolor_i(MainCube.front.sw), DARKGRAY);


        alpha += INCREMENT;
        if (alpha >= 45.0){
            alpha = -45.0;
            do_moves(&MainCube, "D2");
            CurrentMove = MOVE_NONE;
        }
        break;
    }
    case MOVE_Di: {
        glTranslatef(1.0, 1.0, -1.0);
        DrawCube(getcolor_i(MainCube.up.ne), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.ne), DARKGRAY, getcolor_i(MainCube.back.nw));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.n), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.n));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.nw), DARKGRAY, getcolor_i(MainCube.left.nw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.ne));
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.e), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.n), DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.w), DARKGRAY, getcolor_i(MainCube.left.n), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.se), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.nw), getcolor_i(MainCube.front.ne), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.n), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(getcolor_i(MainCube.up.sw), DARKGRAY, getcolor_i(MainCube.left.ne), DARKGRAY, getcolor_i(MainCube.front.nw), DARKGRAY);

        glTranslatef(2.0, -1.0, -2.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.e), DARKGRAY, getcolor_i(MainCube.back.w));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.center));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.w), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.e));
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.center), DARKGRAY, DARKGRAY);
        glTranslatef(-2.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.center), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.w), getcolor_i(MainCube.front.e), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.center), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.e), DARKGRAY, getcolor_i(MainCube.front.w), DARKGRAY);

        glTranslatef(1.0, 0.0, -1.0);
        glRotatef(-sin(alpha*PI/90)*45.0 - 45.0, 0.0, 1.0, 0.0);

        glTranslatef(1.0, -1.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.se), DARKGRAY, getcolor_i(MainCube.right.se), DARKGRAY, getcolor_i(MainCube.back.sw));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.s));
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.sw), getcolor_i(MainCube.left.sw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.se));
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.e), DARKGRAY, getcolor_i(MainCube.right.s), DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.w), getcolor_i(MainCube.left.s), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(2.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.ne), DARKGRAY, getcolor_i(MainCube.right.sw), getcolor_i(MainCube.front.se), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.n), DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.s), DARKGRAY);
        glTranslatef(-1.0, 0.0, 0.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.nw), getcolor_i(MainCube.left.se), DARKGRAY, getcolor_i(MainCube.front.sw), DARKGRAY);


        alpha += INCREMENT;
        if (alpha >= 45.0){
            alpha = -45.0;
            do_moves(&MainCube, "D'");
            CurrentMove = MOVE_NONE;
        }
        break;
    }
    case MOVE_R: {
        glTranslatef(-1.0, -1.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.sw), getcolor_i(MainCube.left.sw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.se));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.w), getcolor_i(MainCube.left.s), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.nw), getcolor_i(MainCube.left.se), DARKGRAY, getcolor_i(MainCube.front.sw), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.w), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.e));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.center), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.e), DARKGRAY, getcolor_i(MainCube.front.w), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(getcolor_i(MainCube.up.nw), DARKGRAY, getcolor_i(MainCube.left.nw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.ne));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.w), DARKGRAY, getcolor_i(MainCube.left.n), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.sw), DARKGRAY, getcolor_i(MainCube.left.ne), DARKGRAY, getcolor_i(MainCube.front.nw), DARKGRAY);

        glTranslatef(1.0, -2.0, -2.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.s));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.n), DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.s), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.center));
        glTranslatef(0.0, 0.0, 2.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.center), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(getcolor_i(MainCube.up.n), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.n));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.n), DARKGRAY);


        glTranslatef(0.0, -1.0, -1.0);
        glRotatef(-sin(alpha*PI/90)*45.0 - 45.0, 1.0, 0.0, 0.0);


        glTranslatef(1.0, -1.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.se), DARKGRAY, getcolor_i(MainCube.right.se), DARKGRAY, getcolor_i(MainCube.back.sw));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.e), DARKGRAY, getcolor_i(MainCube.right.s), DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.ne), DARKGRAY, getcolor_i(MainCube.right.sw), getcolor_i(MainCube.front.se), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.e), DARKGRAY, getcolor_i(MainCube.back.w));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.center), DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.w), getcolor_i(MainCube.front.e), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(getcolor_i(MainCube.up.ne), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.ne), DARKGRAY, getcolor_i(MainCube.back.nw));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.e), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.n), DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.se), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.nw), getcolor_i(MainCube.front.ne), DARKGRAY);

        alpha += INCREMENT;
        if (alpha >= 45.0){
            alpha = -45.0;
            do_moves(&MainCube, "R");
            CurrentMove = MOVE_NONE;
        }
        break;
    }
    case MOVE_R2: {
        glTranslatef(-1.0, -1.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.sw), getcolor_i(MainCube.left.sw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.se));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.w), getcolor_i(MainCube.left.s), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.nw), getcolor_i(MainCube.left.se), DARKGRAY, getcolor_i(MainCube.front.sw), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.w), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.e));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.center), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.e), DARKGRAY, getcolor_i(MainCube.front.w), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(getcolor_i(MainCube.up.nw), DARKGRAY, getcolor_i(MainCube.left.nw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.ne));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.w), DARKGRAY, getcolor_i(MainCube.left.n), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.sw), DARKGRAY, getcolor_i(MainCube.left.ne), DARKGRAY, getcolor_i(MainCube.front.nw), DARKGRAY);

        glTranslatef(1.0, -2.0, -2.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.s));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.n), DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.s), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.center));
        glTranslatef(0.0, 0.0, 2.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.center), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(getcolor_i(MainCube.up.n), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.n));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.n), DARKGRAY);


        glTranslatef(0.0, -1.0, -1.0);
        glRotatef(-sin(alpha*PI/90)*90.0 - 90.0, 1.0, 0.0, 0.0);


        glTranslatef(1.0, -1.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.se), DARKGRAY, getcolor_i(MainCube.right.se), DARKGRAY, getcolor_i(MainCube.back.sw));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.e), DARKGRAY, getcolor_i(MainCube.right.s), DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.ne), DARKGRAY, getcolor_i(MainCube.right.sw), getcolor_i(MainCube.front.se), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.e), DARKGRAY, getcolor_i(MainCube.back.w));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.center), DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.w), getcolor_i(MainCube.front.e), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(getcolor_i(MainCube.up.ne), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.ne), DARKGRAY, getcolor_i(MainCube.back.nw));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.e), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.n), DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.se), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.nw), getcolor_i(MainCube.front.ne), DARKGRAY);

        alpha += INCREMENT;
        if (alpha >= 45.0){
            alpha = -45.0;
            do_moves(&MainCube, "R2");
            CurrentMove = MOVE_NONE;
        }
        break;
    }
    case MOVE_Ri: {
        glTranslatef(-1.0, -1.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.sw), getcolor_i(MainCube.left.sw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.se));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.w), getcolor_i(MainCube.left.s), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.nw), getcolor_i(MainCube.left.se), DARKGRAY, getcolor_i(MainCube.front.sw), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.w), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.e));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.center), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.e), DARKGRAY, getcolor_i(MainCube.front.w), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(getcolor_i(MainCube.up.nw), DARKGRAY, getcolor_i(MainCube.left.nw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.ne));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.w), DARKGRAY, getcolor_i(MainCube.left.n), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.sw), DARKGRAY, getcolor_i(MainCube.left.ne), DARKGRAY, getcolor_i(MainCube.front.nw), DARKGRAY);

        glTranslatef(1.0, -2.0, -2.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.s));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.n), DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.s), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.center));
        glTranslatef(0.0, 0.0, 2.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.center), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(getcolor_i(MainCube.up.n), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.n));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.n), DARKGRAY);


        glTranslatef(0.0, -1.0, -1.0);
        glRotatef(sin(alpha*PI/90)*45.0 + 45.0, 1.0, 0.0, 0.0);


        glTranslatef(1.0, -1.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.se), DARKGRAY, getcolor_i(MainCube.right.se), DARKGRAY, getcolor_i(MainCube.back.sw));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.e), DARKGRAY, getcolor_i(MainCube.right.s), DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.ne), DARKGRAY, getcolor_i(MainCube.right.sw), getcolor_i(MainCube.front.se), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.e), DARKGRAY, getcolor_i(MainCube.back.w));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.center), DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.w), getcolor_i(MainCube.front.e), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(getcolor_i(MainCube.up.ne), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.ne), DARKGRAY, getcolor_i(MainCube.back.nw));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.e), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.n), DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.se), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.nw), getcolor_i(MainCube.front.ne), DARKGRAY);

        alpha += INCREMENT;
        if (alpha >= 45.0){
            alpha = -45.0;
            do_moves(&MainCube, "R'");
            CurrentMove = MOVE_NONE;
        }
        break;
    }
    case MOVE_L: {
        glTranslatef(1.0, -1.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.se), DARKGRAY, getcolor_i(MainCube.right.se), DARKGRAY, getcolor_i(MainCube.back.sw));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.e), DARKGRAY, getcolor_i(MainCube.right.s), DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.ne), DARKGRAY, getcolor_i(MainCube.right.sw), getcolor_i(MainCube.front.se), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.e), DARKGRAY, getcolor_i(MainCube.back.w));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.center), DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.w), getcolor_i(MainCube.front.e), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(getcolor_i(MainCube.up.ne), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.ne), DARKGRAY, getcolor_i(MainCube.back.nw));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.e), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.n), DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.se), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.nw), getcolor_i(MainCube.front.ne), DARKGRAY);

        glTranslatef(-1.0, -2.0, -2.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.s));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.n), DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.s), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.center));
        glTranslatef(0.0, 0.0, 2.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.center), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(getcolor_i(MainCube.up.n), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.n));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.n), DARKGRAY);


        glTranslatef(0.0, -1.0, -1.0);
        glRotatef(sin(alpha*PI/90)*45.0 + 45.0, 1.0, 0.0, 0.0);


        glTranslatef(-1.0, -1.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.sw), getcolor_i(MainCube.left.sw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.se));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.w), getcolor_i(MainCube.left.s), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.nw), getcolor_i(MainCube.left.se), DARKGRAY, getcolor_i(MainCube.front.sw), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.w), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.e));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.center), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.e), DARKGRAY, getcolor_i(MainCube.front.w), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(getcolor_i(MainCube.up.nw), DARKGRAY, getcolor_i(MainCube.left.nw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.ne));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.w), DARKGRAY, getcolor_i(MainCube.left.n), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.sw), DARKGRAY, getcolor_i(MainCube.left.ne), DARKGRAY, getcolor_i(MainCube.front.nw), DARKGRAY);


        alpha += INCREMENT;
        if (alpha >= 45.0){
            alpha = -45.0;
            do_moves(&MainCube, "L");
            CurrentMove = MOVE_NONE;
        }
        break;
    }
    case MOVE_L2: {
        glTranslatef(1.0, -1.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.se), DARKGRAY, getcolor_i(MainCube.right.se), DARKGRAY, getcolor_i(MainCube.back.sw));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.e), DARKGRAY, getcolor_i(MainCube.right.s), DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.ne), DARKGRAY, getcolor_i(MainCube.right.sw), getcolor_i(MainCube.front.se), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.e), DARKGRAY, getcolor_i(MainCube.back.w));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.center), DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.w), getcolor_i(MainCube.front.e), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(getcolor_i(MainCube.up.ne), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.ne), DARKGRAY, getcolor_i(MainCube.back.nw));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.e), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.n), DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.se), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.nw), getcolor_i(MainCube.front.ne), DARKGRAY);

        glTranslatef(-1.0, -2.0, -2.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.s));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.n), DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.s), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.center));
        glTranslatef(0.0, 0.0, 2.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.center), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(getcolor_i(MainCube.up.n), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.n));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.n), DARKGRAY);


        glTranslatef(0.0, -1.0, -1.0);
        glRotatef(sin(alpha*PI/90)*90.0 + 90.0, 1.0, 0.0, 0.0);


        glTranslatef(-1.0, -1.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.sw), getcolor_i(MainCube.left.sw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.se));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.w), getcolor_i(MainCube.left.s), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.nw), getcolor_i(MainCube.left.se), DARKGRAY, getcolor_i(MainCube.front.sw), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.w), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.e));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.center), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.e), DARKGRAY, getcolor_i(MainCube.front.w), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(getcolor_i(MainCube.up.nw), DARKGRAY, getcolor_i(MainCube.left.nw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.ne));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.w), DARKGRAY, getcolor_i(MainCube.left.n), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.sw), DARKGRAY, getcolor_i(MainCube.left.ne), DARKGRAY, getcolor_i(MainCube.front.nw), DARKGRAY);


        alpha += INCREMENT;
        if (alpha >= 45.0){
            alpha = -45.0;
            do_moves(&MainCube, "L2");
            CurrentMove = MOVE_NONE;
        }
        break;
    }
    case MOVE_Li: {
        glTranslatef(1.0, -1.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.se), DARKGRAY, getcolor_i(MainCube.right.se), DARKGRAY, getcolor_i(MainCube.back.sw));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.e), DARKGRAY, getcolor_i(MainCube.right.s), DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.ne), DARKGRAY, getcolor_i(MainCube.right.sw), getcolor_i(MainCube.front.se), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.e), DARKGRAY, getcolor_i(MainCube.back.w));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.center), DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.w), getcolor_i(MainCube.front.e), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(getcolor_i(MainCube.up.ne), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.ne), DARKGRAY, getcolor_i(MainCube.back.nw));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.e), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.n), DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.se), DARKGRAY, DARKGRAY, getcolor_i(MainCube.right.nw), getcolor_i(MainCube.front.ne), DARKGRAY);

        glTranslatef(-1.0, -2.0, -2.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.s));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.n), DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.s), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.center));
        glTranslatef(0.0, 0.0, 2.0);
        DrawCube(DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.center), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(getcolor_i(MainCube.up.n), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.n));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.center), DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.s), DARKGRAY, DARKGRAY, DARKGRAY, getcolor_i(MainCube.front.n), DARKGRAY);


        glTranslatef(0.0, -1.0, -1.0);
        glRotatef(-sin(alpha*PI/90)*45.0 - 45.0, 1.0, 0.0, 0.0);


        glTranslatef(-1.0, -1.0, -1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.sw), getcolor_i(MainCube.left.sw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.se));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.w), getcolor_i(MainCube.left.s), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, getcolor_i(MainCube.down.nw), getcolor_i(MainCube.left.se), DARKGRAY, getcolor_i(MainCube.front.sw), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.w), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.e));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.center), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(DARKGRAY, DARKGRAY, getcolor_i(MainCube.left.e), DARKGRAY, getcolor_i(MainCube.front.w), DARKGRAY);
        glTranslatef(0.0, 1.0, -2.0);
        DrawCube(getcolor_i(MainCube.up.nw), DARKGRAY, getcolor_i(MainCube.left.nw), DARKGRAY, DARKGRAY, getcolor_i(MainCube.back.ne));
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.w), DARKGRAY, getcolor_i(MainCube.left.n), DARKGRAY, DARKGRAY, DARKGRAY);
        glTranslatef(0.0, 0.0, 1.0);
        DrawCube(getcolor_i(MainCube.up.sw), DARKGRAY, getcolor_i(MainCube.left.ne), DARKGRAY, getcolor_i(MainCube.front.nw), DARKGRAY);


        alpha += INCREMENT;
        if (alpha >= 45.0){
            alpha = -45.0;
            do_moves(&MainCube, "L'");
            CurrentMove = MOVE_NONE;
        }
        break;
    }
    }
    xAngle+=xAngleIncrement;
    yAngle+=yAngleIncrement;
    NextMove();
    glutSwapBuffers();
}
void NextMove(){
    if ((CurrentMove == MOVE_NONE) && (movesbuffer != NULL)){
        if (movesbuffer[movesbufferpos] == '\0') {
            moving = false;
            return;
       } if (movesbuffer[movesbufferpos+1] == '2'){
                switch(movesbuffer[movesbufferpos]){
                case 'U': CurrentMove = MOVE_U2; break;
                case 'D': CurrentMove = MOVE_D2; break;
                case 'L': CurrentMove = MOVE_L2; break;
                case 'R': CurrentMove = MOVE_R2; break;
                case 'F': CurrentMove = MOVE_F2; break;
                case 'B': CurrentMove = MOVE_B2;
                }
                movesbufferpos+=2;
                moving = true;
        } else if (movesbuffer[movesbufferpos + 1] == '\'' || movesbuffer[movesbufferpos+1] == 'i'){
                switch(movesbuffer[movesbufferpos]){
                case 'U': CurrentMove = MOVE_Ui; break;
                case 'D': CurrentMove = MOVE_Di; break;
                case 'L': CurrentMove = MOVE_Li; break;
                case 'R': CurrentMove = MOVE_Ri; break;
                case 'F': CurrentMove = MOVE_Fi; break;
                case 'B': CurrentMove = MOVE_Bi;
                }
                movesbufferpos+=2;
                moving = true;
        } else {
                switch(movesbuffer[movesbufferpos]){
                case 'U': CurrentMove = MOVE_U; break;
                case 'D': CurrentMove = MOVE_D; break;
                case 'L': CurrentMove = MOVE_L; break;
                case 'R': CurrentMove = MOVE_R; break;
                case 'F': CurrentMove = MOVE_F; break;
                case 'B': CurrentMove = MOVE_B;
                }
                movesbufferpos++;
                moving = true;
        }
    }
}
void DoMoves(char * c){
    movesbuffer = (char *)realloc(movesbuffer, strlen(c) + 1);
    strcpy(movesbuffer, c);
    movesbufferpos = 0;
    return;
}




void KeyPress(unsigned char key, int x, int y)
{
    usleep(100);
    if (key == ESCAPE)
    {
        printf("\n"
               "Goodbye!\n");
        glutDestroyWindow(window);
        exit(0);
    }
#ifdef KEY_TOGGLEFULLSCREEN
    if (key == tolower((char)KEY_TOGGLEFULLSCREEN) || key == toupper((char)KEY_TOGGLEFULLSCREEN)){
        if (fullscreen) {
            glutReshapeWindow(640, 480);
            fullscreen = false;
        } else {
            glutFullScreen();
            fullscreen = true;
        }
        return;
    }
#endif
#ifdef KEY_STOP
    if (key == tolower((char)KEY_STOP) || key == toupper((char)KEY_STOP)){

        DoMoves("");
        return;
    }
#endif
    if (key >= '1' && key <='9'){
        INCREMENT=(key-'0')*2.0;
    }
    if (moving)
        return;
    if (key == 's' || key == 'S'){
        cube CopyOfMainCube = MainCube;
        scramble(&CopyOfMainCube, 25);

//        printcube(CopyOfMainCube);
        char tmp_s[100];
        cube_to_string(tmp_s, CopyOfMainCube);
        printf("\n%s\n", tmp_s);
        DoMoves(moves);
        return;
    }
    if (key == ' '){
        cube CopyOfMainCube = MainCube;
        solve(CopyOfMainCube);
        DoMoves(moves);
        return;
    }
    switch(key){
        case 'u':
            DoMoves("U");
            break;
        case 'd':
            DoMoves("D");
            break;
        case 'l':
            DoMoves("L");
            break;
        case 'r':
            DoMoves("R");
            break;
        case 'f':
            DoMoves("F");
            break;
        case 'b':
            DoMoves("B");
            break;

        case 'U':
            DoMoves("U'");
            break;
        case 'D':
            DoMoves("D'");
            break;
        case 'L':
            DoMoves("L'");
            break;
        case 'R':
            DoMoves("R'");
            break;
        case 'F':
            DoMoves("F'");
            break;
        case 'B':
            DoMoves("B'");
            break;

        case 'U' - '@':
            DoMoves("U2");
            break;
        case 'D' - '@':
            DoMoves("D2");
            break;
        case 'L' - '@':
            DoMoves("L2");
            break;
        case 'R' - '@':
            DoMoves("R2");
            break;
        case 'F' - '@':
            DoMoves("F2");
            break;
        case 'B' - '@':
            DoMoves("B2");
            break;
        case 1:
            DoMoves("F' U B' R' U F2 U2 F' U' F U2 D B' D' R2 B2 U'");
    }
    if (key == 'k'){
        cube cpCube = MainCube;
        rot_and_swap(&cpCube, FACE_FRONT);
        solve(cpCube);
        rot_moves(moves, DIR_DOWN);
        DoMoves(moves);
        return;
    }
}
void mouseMoviment(int x, int y)
{
    if (leftdown){
        yAngle += (x-oldx)*0.2;
        xAngle += (y-oldy)*0.2;
        incx = oldx;
        incy = oldy;
        oldx = x;
        oldy = y;
    }
}

void mouseClick(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON){
        if (state == GLUT_DOWN){

            oldx = incx = x;
            oldy = incy = y;
            xAngleIncrement = 0.0f;
            yAngleIncrement = 0.0f;
            leftdown = 1;
        }
        else if (state == GLUT_UP){

            leftdown = 0;
            yAngleIncrement = (x-incx)*0.2;
            xAngleIncrement = (y-incy)*0.2;
        }
    }
}

int main(int argc, char **argv)
{
  if(argc != 2) {
    printf("Usage: ./prog <string>\n");
    exit(1);
  }
  new_cube(&MainCube, argv[1]);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
  glutInitWindowSize(640, 480);
  glutInitWindowPosition(0, 0);
  window = glutCreateWindow("Rubiks");
  glutDisplayFunc(&DrawGLScene);
#if STARTFULLSCREEN
  glutFullScreen();
  fullscreen = true;
#else
  fullscreen = false;
#endif
  glutIdleFunc(&DrawGLScene);
  glutReshapeFunc(&ReSizeGLScene);
  glutKeyboardFunc(&KeyPress);
  glutMouseFunc(&mouseClick);
  glutMotionFunc(&mouseMoviment);
  InitGL();
  printf("Welcome!\n");
  glutMainLoop();

  return 0;
}
