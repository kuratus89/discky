#pragma once
#include "discky.h"
#include <stddef.h>
#include "stdlib.h"

# define pushVector(vec , data) \ (*(typeof(data)*)pushVec(vec) = data) 
#define MAX(a,b)({typeof(a) tempa =(a); typeof(b) tempb = (b) ; tempa>tempb ? tempa : tempb;})
#define MIN(a,b)({typeof(a) tempa = (a) ; typeof(b) tempb =(b) ; tempa<tempb ? tempa :tempb;})
#define SWAP(a,b)({typeof(a) temp = (a) ; a = b ; b = temp;})

typedef struct{
    void* vector;
    int capacity;
    int count;
    int size;
}Vec;

void iniVec(Vec *vector , size_t elementSize);
void* pushVec(Discky* discky , Vec *vector);
void* getVecElement(Discky* discky ,const Vec* vector , int x);
void resizeVec(Discky* discky , Vec* vector , int x);
void allocBuffer(Screen** screen);