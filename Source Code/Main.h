#ifndef MAIN_H_
#define MAIN_H_
#include "type.h"

//=================================================================
//=====================DEFINITIONS=================================
//=================================================================
#define W White
#define B Black

typedef struct block{
    U32 height:10;//8
    U32 length:10;//80
    U32 speed:10;
    U32 x:10;
    U32 y:10;
}t_block;

typedef struct stack {
    U32 length:10;//80
    U32 height:10;
    U16 top_row[80];
    U32 x:10;
    U32 y:10;
}t_stack;

//=================================================================
//=====================FUNCTION SIGNATURES=========================
//=================================================================
void init(void);
void create_block(U32 h, U32 l, U32 s, U32 x, U32 y);
void create_stack(U32 h, U32 l, U32 x, U32 y);

#endif
