#ifndef GAME_OBJECTS_H__
#define GAME_OBJECTS_H__
#include "type.h"
#include <rtl.h>

//=================================================================
//======================TYPE DEFS==================================
//=================================================================
typedef struct block{
    U32 height:10;//8
    U32 length:10;//80
    U32 speed:10;
    U16 clear_block[8*80];
    U16 set_block[8*80];
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
//======================VARIABLES==================================
//=================================================================
extern t_block* block;
extern t_stack* stack;

#endif
