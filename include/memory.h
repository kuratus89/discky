// #pragma once
// #include "types.h"
// #include <stddef.h>
// #include "stdlib.h"

// #define MAX(a,b)({__typeof__(a) tempa =(a); __typeof__(b) tempb = (b) ; tempa>tempb ? tempa : tempb;})
// #define MIN(a,b)({__typeof__(a) tempa = (a) ; __typeof__(b) tempb =(b) ; tempa<tempb ? tempa :tempb;})
// #define SWAP(a,b)({__typeof__(a) temp = (a) ; a = b ; b = temp;})

// struct Vec{
//     void* vector;
//     int capacity;
//     int count;
//     int size;
// };

// void iniVec(Vec *vector , size_t elementSize);
// void* pushVec(Discky* discky , Vec *vector);
// void* getVecElement(Discky* discky ,const Vec* vector , int x);
// void resizeVec(Discky* discky , Vec* vector , int x);
// void allocBuffer(Screen** screen);