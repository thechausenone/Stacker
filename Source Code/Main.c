#include "Main.h"

//=================================================================
//=====================GLOBAL VARIABLES============================
//=================================================================
t_block* block = (t_block*)0x10000000;
t_stack* stack = (t_stack*)0x10002567;
OS_MUT sem_speed, sem_pos, sem_button, sem_display_state;
OS_TID tid_move_block, tid_update_speed, tid_drop_block, tid_display_state, tid_update_LED, tid_end_game, tid_init_tasks;
U32 display_state = 0, old_state = 0, score = 0;
bool from_pot = 0, end_game_flag = 0, check_isr = 0, spawn_pos = 0;
//S32 spawn_pos = 1;

//=================================================================
//=======================INTERRUPTS================================
//=================================================================
//interrupt for int0 button
void EINT3_IRQHandler(void) {
  //clears the ISR
  LPC_GPIOINT->IO2IntClr |= (1 << 10);
  
  //set a "flag" so that drop_block can begin
  check_isr = 1;
}

//=================================================================
//===================(2)DROP BLOCK=================================
//=================================================================
//task that drops the block onto the stack
__task void drop_block() {
	S32 block_xi, block_xf;
	S32 stack_xi, stack_xf;
	
	while(1){
		os_evt_wait_or(0x0030, 0xFFFF);{
			os_mut_wait(&sem_pos, 0xffff);{
        
        //initialize position variables
				block_xi = block->x;
				block_xf = block->x + block->length;
				stack_xi = stack->x;
				stack_xf = stack->x + stack->length;
				
				//clear the previous block before updating values
				draw_to_LCD('B', 0);
				
				//lose condition #1
				if (block_xi > stack_xf || block_xf < stack_xi) {
					printf("OVERHANG LOSS\nblock_xi = %d\nstack_xf = %d\nblock_xf = %d\nstack_xi = %d\n", block_xi, stack_xf, block_xf, stack_xi);
          end_game_flag = 0;
          os_evt_set(0x0300, tid_end_game);
				}
				
				//block either overhangs on the left or is perfectly placed on the stack
				else if (stack_xf - block_xi >= 0 && block_xi >= stack_xi) {
					//update stack
					stack->length = stack_xf - block_xi;
					stack->x = stack_xf - stack->length;
          stack->y = stack->y - 8;
				}
				
				//block overhangs on the right of the stack or is perfectly placed on the stack
				else if (block_xf - stack_xi >=0 && block_xf <= stack_xf) {			
					//update stack
					stack->length = block_xf - stack_xi;
					//stack x-pos stays the same
          stack->y = stack->y - 8;
				}
				
				//update block
				block->length = stack->length;
				block->y = stack->y - 8;
				score = score + ((block->length)/8)*(block->speed);
			
				//update which side the new block spawns
				//spawn_pos = spawn_block(spawn_pos);
				spawn_block();
				
				//lose condition #2
				if (block->length < 8){
					end_game_flag = 0;
          os_evt_set(0x0300, tid_end_game);
				}
        
				//draw the new block and new stack
				draw_to_LCD('B', 1);
				draw_to_LCD('S', 1);      
        
        //check win condition
        if (stack->y < 16){
          end_game_flag = 1;
          os_evt_set(0x0300, tid_end_game);
        }
        
			}os_mut_release(&sem_pos);
		}os_evt_clr(0x0030, tid_drop_block);
	}
}

//=================================================================
//=====================(3)UPDATE LEDs==============================
//=================================================================
//task that updates what is currently displayed on the LEDs
__task void update_LED() {
	while(1) {
		os_evt_wait_or(0x0003, 0xffff);{
			os_mut_wait(&sem_display_state, 0xffff); {
				
				//displays current score 
				if (display_state == 0){
          LED_display(score);
          draw_stat(score, 0);
        }

				//displays speed for an instant (from potentiometer)
				else if (display_state == 1 && from_pot == 1){
					LED_display(block->speed);
          draw_stat(block->speed, 1);
          
          //pause the game so the user can see the current speed
					timer_delay(1000);
					from_pot = 0;
					display_state = old_state;
				}
				
				//displays speed (from scrolling)
				else{
          LED_display(block->speed);
          draw_stat(block->speed, 1);
        }

			} os_mut_release(&sem_display_state);
		}os_evt_clr(0x0003, tid_update_LED);
	}
}
//=================================================================
//==============(4)UPDATE SPEED (potentiometer)====================
//=================================================================
//task that updates the speed of the block
__task void update_speed() {
	U32 old_speed;
	while(1) {
		os_mut_wait(&sem_speed, 0xffff); {
			os_mut_wait(&sem_display_state, 0xffff); {
        
				//old speed is used to display state needs to be updated
				old_speed = block->speed;
				
				//total rotation of dial = 0 -> 4095, 4095/5 = 819 per interval
				block->speed = ceil(potentiometer_read()/819) + 1;
				
				//check if display state needs to be updated
				if (block->speed != old_speed){
					old_state = display_state;
					display_state = 1;
					from_pot = 1;
				}
				
        //when speed is changed, display the change on the LEDs right away
				os_evt_set(0x0003, tid_update_LED);		
				
			} os_mut_release(&sem_display_state);
		} os_mut_release(&sem_speed);
	}
}

//=================================================================
//===============(5)UPDATE DISPLAY STATE (joystick)================
//=================================================================
//task that updates the LEDs' current display state
__task void update_display_state() {
	while(1) {
		os_mut_wait(&sem_display_state, 0xffff); {
      
      //if UP is actuated on the joystick
			if (joystick_read() == 16)
        display_state = 1;

      //if DOWN is actuated on the joystick
			else if(joystick_read() == 64) 
				display_state = 0;

      //go and update the LEDs with the new state
      os_evt_set(0x0003, tid_update_LED);
			
		} os_mut_release(&sem_display_state);
	}
}

//=================================================================
//=======================(6)END GAME===============================
//=================================================================
//task that stops the game and directs the user to either the win or lose screen 
__task void end_game() {
  while(1) {
    os_evt_wait_or(0x0300, 0xFFFF); {
      //delete all tasks
      os_tsk_delete(tid_move_block);
      os_tsk_delete(tid_update_speed);
      os_tsk_delete(tid_drop_block);
      os_tsk_delete(tid_display_state);
      os_tsk_delete(tid_update_LED);
     
      //reset screen
      GLCD_Clear(B);

      //display the lose or win screen
      draw_text(score, end_game_flag);
      
      //waits for the user to hard reset
      while(1) {}
    }
  }
}

//=================================================================
//=====================FXN:INIT TASKS==============================
//=================================================================
//function that is responsible for initializing all other tasks
void init_tasks() {
    //initialize semaphores
    os_mut_init (&sem_speed);
		os_mut_init (&sem_pos);
		os_mut_init (&sem_button);
		os_mut_init (&sem_display_state);
    
    //create the tasks
		tid_update_speed = os_tsk_create(update_speed, 1);
		tid_drop_block = os_tsk_create(drop_block, 1);
		tid_display_state = os_tsk_create(update_display_state, 1);
		tid_update_LED = os_tsk_create(update_LED, 1);
		tid_end_game = os_tsk_create(end_game, 1);
}

//=================================================================
//======================(1)MOVE BLOCK==============================
//=================================================================
//task that moves the block back and forth on the LCD
__task void move_block(){
  //initialize and set all local variables
  S32 direction, iter, bounce_count, movement_value;
  direction = 0;
  iter = 0;
  bounce_count = 0;
	movement_value = -1;
	
	//set the task ID for move_block & initialize all other tasks
	tid_move_block = os_tsk_self();
	init_tasks();
	
  while(1){
    //only acquire sem_speed on 1st iteration and every 10 after that (avoid multiple waits on an owned semaphore)
    if (iter == 0)
			os_mut_wait(&sem_speed, 0xffff);{
				os_mut_wait(&sem_pos, 0xffff);{
          
					//clears the previous instance of the block
					draw_to_LCD('B', 0);
					 
// 					//check if the block's direction needs to be reversed
// 					if (direction != reverse_direction(direction)){
// 						bounce_count++;
// 						direction = direction * -1;
// 						//spawn_pos = spawn_pos * -1;
// 					}
					
						//currently going to the right (==1)
					if (block->x <= 0 && direction == 1){
						direction = 0;
						bounce_count++;
						movement_value  = 1;
					}						
						
					//currently going to the left (==0)
					else if (block->x >= 240-block->length && direction == 0){
						direction = 1;
						bounce_count++;
						movement_value = -1;
					}

          //game over when the block moves back and forth two times
          if (bounce_count == 4) {
            bounce_count = 0;
            os_evt_set(0x0300, tid_end_game);
          }
          
					//updates the position of the block
					block->x = block->x + movement_value;//sub for direction
					
					//draws the new instance of the block
					draw_to_LCD('B', 1);
					
					//when push button is acutated, drop the block
          if (check_isr == 1){
            bounce_count = 0;
            check_isr = 0;
            os_evt_set(0x0030, tid_drop_block);
          }
					
					//wait for a bit until the block's can be updated again
					timer_delay(50/(block->speed*4));
					iter++;
    
				} os_mut_release(&sem_pos);
        //sem_speed is not released to update_speed task until iteration 10
			} if (iter>10){iter = 0;os_mut_release(&sem_speed);}
  }
}

//=================================================================
//=======================MAIN FUNCTION=============================
//=================================================================
int main() {
    printf("Begin Game\n");
    init_peripherals();
    init_game();
		os_sys_init(move_block);
    return 0;
}
