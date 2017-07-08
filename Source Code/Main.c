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

#define BG White
#define FG Blue

OS_TID t_test;

void test_peripherals(){
        
    unsigned short cross_bitmap[] = {FG, FG, FG, FG, FG, FG ,FG ,FG ,FG,
                                     FG, FG, FG, FG, FG, FG ,FG ,FG ,FG,
                                     FG, FG, FG, FG, FG, FG ,FG ,FG ,FG,
                                     FG, FG, FG, FG, FG, FG ,FG ,FG ,FG,
                                     FG, FG, FG, FG, FG, FG ,FG ,FG ,FG,
                                     FG, FG, FG, FG, FG, FG ,FG ,FG ,FG,
                                     FG, FG, FG, FG, FG, FG ,FG ,FG ,FG,
                                     FG, FG, FG, FG, FG, FG ,FG ,FG ,FG };
    
//     //TEST LED
//     LED_setup();
//     LED_display(27);// 0->255
    
//     //TEST PUSHBUTTON
//      pushbutton_setup();
//     while (1) {
//         printf("Value: %d \n", pushbutton_read());
//     }
    
//     //TEST JOYSTICK
//     //left: 8, up: 16, right: 32, down: 64, select: 1, default: 0
//     joystick_setup();
//     while (1) {
//         printf("Value: %d \n", joystick_read());
//     }
    
//     //TEST POTENTIOMETER    
//     potentiometer_setup(); // has to be in a task
//     while (1) {
//         printf("Value: %d \n", potentiometer_read());
//     }

//     //TEST TIMER
//     timer_setup(); //microseconds
//     pushbutton_setup();
//     while (1) {
//         printf("Value: %d \n", timer_read());
//         if (pushbutton_read()==0)
//             timer_reset();
//     }

    //TEST LCD 
    //init_scroll();
    //append_char('A');
    //refresh_lcd();
                  
    SystemInit();
    GLCD_Init();
    GLCD_Clear(BG);

    while(1) {
    GLCD_Bitmap(160, 120, 9, 8, (unsigned char*)cross_bitmap);
    GLCD_Bitmap(160-9, 120, 9, 8, (unsigned char*)cross_bitmap);
    }
}

__task void task_test(void) {
 
 test_peripherals();
    
}

int main() {
    
    printf("main\n");
    os_sys_init (task_test);
    
    return 0;
      
}

