# Stacker
A game created using the Keil Microcontroller.

## Design Proposal
The Keil Microcontroller, and its attached peripherals, will be used as a platform for the development
of an interactive game. The idea of the game revolves around the consecutive stacking of rectangular
blocks, where the end goal is to achieve the highest stack possible. Initially, the user is supplied with a
single rectangular block that will travel across the screen in a periodic manner. Once the user decides
to place the block down, the stack will have been created and the game will commence. A subsequent
block will be given to the user which floats above the stack and it is required that the user promptly
place the new block down as well. Any section of the new block that causes overhang with respect to
the top of the stack will be removed. The leftover size of the new block then becomes the new size for
incoming blocks. As such, it is desired that the user places the blocks perfectly on top of each other. If
the user is not able to place a block on the stack, the game is then declared to be over. Additional
features in the game include being able to adjust the difficulty of the game in real time and having the
ability to toggle through a display that shows different stats to the user.

## How are the peripherals used to meet the design requirements
1. The **LCD screen** will be an interface that allows the user to see the dynamic of the game.

2. The **push button** is a peripheral that has two states and will be used to control the most
important action in the game. When pressed, the button will switch to a state of 1, allowing a
new block to be set into position. The button will switch back to a state of 0 when the button is
released or when button has been held for over 1 second.

3. The **LEDs** will be used to display various stats to the user in binary, with the least significant
bit starting from the rightmost LED. Some of the stats that are going to be displayed are: the
user's current score, the current difficulty level of game, and the user’s current highscore.

4. To account for the multiple statistics, the direction that the **joystick** peripheral is moving in will
be used to toggle what is currently be displayed on the LEDs.

5. The **potentiometer** will dictate the difficulty of the game, or in other words, the speed at
which the blocks travel across the screen. As the potentiometer is rotated more, the difficulty
will increase. It should be noted that an increase in difficulty will result in a multiplier to the
user’s score to appear.

## What are the tasks and how are they going to interact with each other?
1. A task which constantly updates the current position of the block to be placed (this will be the
task that the game will begin on)

2. A task that places a new block down into position and changes the size of the subsequent
block if necessary. If the new block is placed unsuccessfully, then Task 6 will be run,
otherwise Task 3 will be run. This will only fire when an ISR is activated as a result of a push
button being hit.

3. A task that updates the user’s current score after a new block is successfully placed or will
update what is currently being displayed on the LEDs based on the current toggle selection.
Task 1 should be run after this task is completed in normal cases.

4. A task that updates the current difficulty of the game and score multiplier as well. This task
should pause the game for a second and prompt Task 2 to toggle the display to the difficulty
setting. This task will only activate when an ISR is sent due to the potentiometer being
activated.

5. A task that pauses the game for a second and calls on Task 2 to change the current display
mode of the LEDs. This task will only activate when an ISR is sent due to the joystick being
activated.

6. A task that resets the game when the game over condition is met during Task 2.

## Initial Gameplay Mockup
![stacker design proposal](https://github.com/thechausenone/Stacker/blob/master/design_proposal_diagram.png)

