#include "Main.h"

//=================================================================
//=====================GLOBAL VARIABLES============================
//=================================================================
t_block* block = (t_block*)0x10000000;
t_stack* stack = (t_stack*)0x10002567;
OS_MUT sem_speed, sem_pos, sem_button;
OS_TID tid_move_block, tid_update_speed, tid_drop_block;
int state = 0;

//=================================================================
//=======================INTERRUPTS================================
//=================================================================
// //interrupt for int0 button
// void EINT3_IRQHandler(void) {
// 	//need to account for the fact that holding down needs to be reset to 0
// 	LPC_GPIOINT->IO2IntClr |= (1 << 10);
// 	printf("interrupt detected\n");
// 	os_evt_set(0x0001, tid_drop_block);
// }

//=================================================================
//=====================DROP BLOCK==================================
//=================================================================
__task void drop_block() {
	int block_xi, block_xf;
	int stack_xi, stack_xf;
	
	while(1){
		printf("dropblock initialized\n");
		os_evt_wait_or(0x0001, 0xFFFF);{
			os_mut_wait(&sem_pos, 0xffff);{
				printf("entered dropblock\n");
				block_xf = block->x;
				block_xi = block->x + block->length;
				stack_xf = stack->x;
				stack_xi = stack->x + stack->length;
				
				//clear the previous block before updating values
				draw_to_LCD('B', 0);
				
				//block is all the way to the left
				if (block_xi > stack_xf || block_xf < stack_xi) {
					printf("GAME OVER!\n");
				}
				
				//block either overhangs on the left or is perfectly placed on the stack
				else if (stack_xf - block_xi >= 0 && block_xi >= stack_xi) {
					//update stack
					stack->length = stack_xf - block_xi;
					stack->x = stack_xf - stack->length;
					stack->height = stack->height + block->height;
					
					//update block
					block->length = stack->length;
					block->x = 240 - block->length;
					block->y = stack->height;
				}
				
				//block overhangs on the right of the stack
				else if (block_xf - stack_xi >=0 && block_xf <= stack_xf) {			
					//update stack (stack x-pos stays the same)
					stack->length = block_xf - stack_xi;
					stack->height = stack->height + block->height;
					
					//update block
					block->length = stack->length;
					block->x = 240 - block->length;
					block->y = stack->height;
				}
				
				//draw new block and new stack
				draw_to_LCD('B', 1);
				draw_to_LCD('S', 1);
			}os_mut_release(&sem_pos);
		}os_evt_set(0x0002, tid_drop_block);
		printf("exited dropblock\n");
	}
}

//=================================================================
//==========================UPDATE SPEED===========================
//=================================================================
//task that updates the speed of the block
__task void update_speed() {
	while(1) {
		os_mut_wait(&sem_speed, 0xffff); {
			//total rotation of dial = 0 -> 4095, 4095/20 = 205 per interval
			block->speed = ceil(potentiometer_read()/205) + 1;
		} os_mut_release(&sem_speed);
	}
}
//=================================================================
//==========================MOVE BLOCK=============================
//=================================================================
//task that moves the block back and forth on the LCD
__task void move_block(){
  
  bool direction;//going to the right is 1, left is 0
  S32 movement_value, iter;
  direction = 1;
  movement_value = -1;

  while(1){
		if(iter == 0)
			os_mut_wait(&sem_speed, 0xffff);{
				os_mut_wait(&sem_pos, 0xffff);{
					//clears the previous instance of the block
					draw_to_LCD('B', 0);
					 
					//check if the block's direction needs to be reversed
					//currently going to the right
					if (block->x <= 0 && direction == 1) {
						direction = 0;
						movement_value = 1;
					}
					
					//currently going to the left
					else if (block->x >= 240-block->length && direction == 0) {
						direction = 1;
						movement_value = -1;
					}

					//updates the position of the block
					block->x = block->x + movement_value;
					
					//draws the new instance of the block
					draw_to_LCD('B', 1);
					
					//wait for a bit until the block's can be updated again
					timer_delay(50/block->speed);
					iter++;
				} os_mut_release(&sem_pos);
			} if (iter>50){iter = 0;os_mut_release(&sem_speed);}
  }
}

//=================================================================
//=========================INIT TASKS==============================
//=================================================================
//task that is responsible for initializing all other tasks
__task void init_tasks() {
    //initialize muti
    os_mut_init (&sem_speed);
    
    //create the tasks
    tid_move_block = os_tsk_create(move_block, 1);
		tid_update_speed = os_tsk_create(update_speed, 1);
		tid_drop_block = os_tsk_create(drop_block, 1);
	
    //delete itself
    os_tsk_delete_self();
}

//=================================================================
//=======================MAIN FUNCTION=============================
//=================================================================
int main() {
    printf("Begin Game");
    init_game();
    os_sys_init(init_tasks);
    return 0;
}
