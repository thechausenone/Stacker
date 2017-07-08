#include <stdint.h>
#include "led.h"
#include <stdbool.h>
#include <rtl.h>
#include "joystick.h"
#include "potentiometer.h"
#include <stdio.h>
#include "timer.h"
#include "glcd_scroll.h"
#include "glcd.h"
#include "Main.h"
#include "type.h"

//=================================================================
//=====================GLOBAL VARIABLES============================
//=================================================================
  t_block* block = (t_block*)0x10000000;
  t_stack* block_stack = (t_stack*)0x10000007;

//=================================================================
//=====================FUNCTIONS===================================
//=================================================================
//initializes the values for the block
void create_block(U32 h, U32 l, U32 s, U32 x, U32 y){
    block->height = h;
    block->length = l;
    block->speed = s;
    block->x = x;
    block->y = y;
}

void print_stuff(){
  printf("==============STACK==========\n");
  printf("height: %d\n",block_stack->height);
  printf("length: %d\n",block_stack->length);
  printf("x: %d\n",block_stack->x);
  printf("y: %d\n\n",block_stack->y);
  
  printf("==============BLOCK==========\n");
  printf("height: %d\n",block->height);
  printf("length: %d\n",block->length);
  printf("speed: %d\n",block->speed);
  printf("x: %d\n",block->x);
  printf("y: %d\n\n",block->y);
}

//initializes the values for the stack
void create_stack(U32 h, U32 l, U32 x, U32 y){
    int i;
    block_stack->height = h;
    block_stack->length = l;
  
    for (i = 0; i < 8*80; i++)
        block_stack->top_row[i] = W;
        
    block_stack->x = x;
    block_stack->y = y;
    
    return;
}

//initializes all required peripherals and other stuff
void init() {
    //other
    //SystemInit();
  
    //peripherals
    GLCD_Init();
    GLCD_Clear(B);
    
    LED_setup();
    pushbutton_setup();
    joystick_setup(); //left: 8, up: 16, right: 32, down: 64, select: 1, default: 0
    potentiometer_setup(); // has to be in a task
    
    //gameplay objects
    create_block(8, 80, 5, 0, block_stack->height + 16);
    create_stack(8, 80, 80, 0);
  
    GLCD_Bitmap(block_stack->x, block_stack->y, block_stack->length, block_stack->height, (unsigned char*)(block_stack->top_row));
    print_stuff();
    
}



int main() {
  printf("hi11\n");
    init();
    printf("hi\n");
    return 0;
      
  //add vertical scroll in pixels values being 8 
}


    
