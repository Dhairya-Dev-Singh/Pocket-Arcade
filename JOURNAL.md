---
title: "1"
author: "Dhairya"
created_at: "2026-11-06"
---

# 2026-11-05: Journalssss

**Total time spent: 3 hours**

after getting the main development work documented on github i also wanted to organise the progress i had made throughout the project. i went back through the different stages of building pocket arcade, starting from the initial idea and hardware setup all the way to the games, cad design, soldering and assembly.

i started writing separate journals for each major stage so that the development process would be easier to follow. i included what i worked on, the problems i faced and the changes i made along the way.

i also wanted to make sure the journals followed the actual order of development instead of just listing the final features. some parts took more effort than expected, especially fixing the display, debugging the games and designing the enclosure, so i made sure those stages were documented separately.

this took some time because i had to go through the work and explain each stage clearly without missing important details.
![image.png](https://cdn.hackclub.com/01a1acc5-2910-7ff6-994c-96929b735a50/image.png)
after finishing the journals i had a much more complete record of how pocket arcade was developed, including both the successful parts and the problems i encountered during the build.

# 2026-11-05: setting up the github repository and writing the readme

**Total time spent: 4 hours**

after finishing the wiring diagram i started working on the github repository for pocket arcade. i wanted to keep all the project files in one place so that anyone interested in the console could understand how it was built and even recreate it themselves.

i started by setting up the repository and organising the project files. i wanted the code, wiring diagram and other design-related files to be easy to find instead of having everything mixed together.
<img width="1334" height="1048" alt="image" src="https://github.com/user-attachments/assets/a123b680-c1da-4232-a87d-ea5e3e8dc232" />
then i started writing the README. i wanted it to explain what pocket arcade actually is, what components i used and what features i had managed to implement. i added information about the ESP32, TFT display, joystick, buttons, buzzer and RGB LED, along with an overview of the four games.

i also included the hardware pin connections and wiring information so that someone going through the repository could understand how everything was connected. i wanted the README to be useful as documentation rather than just a short description of the project.

writing it took some thought because i had to explain both the hardware and software parts clearly. i also wanted the repository to look organised and make it easy for someone to navigate through the different files.

after setting everything up and writing the README, i had a proper github repository for pocket arcade where i could document the project and share the work.

# 2026-11-05: making the wiring diagram

**Total time spent: 3 hours**

along with the hardware work i also wanted to document the wiring properly on github so that anyone looking at the project could understand how all the components were connected.

i started making the wiring diagram, but it turned out to be more confusing than i expected. there were so many connections between the esp32, display, joystick, four buttons, buzzer and rgb led. some components also shared common vcc and gnd connections, so i had to be careful about how i represented everything.

i spent time organising the connections and making sure the diagram actually matched the way i had wired the console. i didn't want to upload something that looked good but had incorrect pin connections or confusing power lines.

it took a while to arrange everything clearly because i wanted the diagram to be understandable even for someone who hadn't seen the actual hardware.
![image.png](https://cdn.hackclub.com/01a1acc2-f0ba-7f9a-94da-0cb59e573bf8/image.png)
after working through the connections i managed to put together the wiring diagram for the github repository so that the hardware setup would be easier to follow.

# 2026-11-05: Starting the assembly

**Total time spent: 3 hours**

after getting all the enclosure parts 3d printed and preparing the electronics, i finally started assembling pocket arcade. i had all the main parts ready and was excited to see how everything would fit together in the actual enclosure.
![image.png](https://cdn.hackclub.com/01a1acba-636c-7ccd-8fc5-c09ed0a5d513/image.png)
i started placing the components into their respective mounts and checking how they aligned with the openings in the case. the display was one of the most important parts to install, so i tried fitting it into its position in the enclosure.

however, while placing the display i accidentally applied too much pressure on it and the display broke. i hadn't expected this to happen, and it stopped the assembly process before i could finish putting everything together.
![image.png](https://cdn.hackclub.com/01a1acba-830a-7b38-847f-63f7020f1eab/image.png)
it was frustrating because the enclosure and other parts were finally ready everything wwas done a gameboy ready to be asssembled but .......

# 2026-11-05: Soldering the electronics

**Total time spent: 4 hours**

while the enclosure parts were being 3d printed i started working on the electronics. until now i had everything connected on a breadboard, but i wanted to make the wiring more permanent and compact so it could eventually fit inside the console.

i first removed the breadboard connections and started soldering the wires directly. there were quite a few connections to make, especially for the four physical buttons. i soldered the individual wires for each button and made sure they were connected to the correct pins.
![image.png](https://cdn.hackclub.com/01a1acb3-74e7-7a05-8866-91cfff52c4f2/image.png)
after that i worked on organising the power connections. instead of running separate power and ground wires everywhere, i joined the common vcc connections together and did the same with all the gnd connections. this helped reduce the amount of wiring and made the overall setup more organised.
![image.png](https://cdn.hackclub.com/01a1acb4-0ab5-75fb-b078-b6155f697f91/image.png)
it took a while because there were so many wires to handle and i had to be careful not to mix up the connections while soldering. i also checked the joints and wiring as i went so that i wouldn't end up with loose connections later.
after around a few hours of soldering i had moved the electronics away from the breadboard setup and made the connections much more permanent, getting them ready for installation inside the enclosure.

# 2026-11-05: refining the enclosure design

**Total time spent: 5 hours**

after creating the basic shape of the console i started working on the smaller details of the enclosure. the main body was taking shape but it still needed proper mounting points and a way to hold all the components securely.
![image.png](https://cdn.hackclub.com/01a1aca4-bded-7236-a20b-3c22776e1d30/image.png)
i added mountings for the internal components so that the display, joystick and other parts could be positioned properly inside the case. i also worked on the back cover because i wanted the console to be accessible from the inside without making the enclosure a single closed piece.
![image.png](https://cdn.hackclub.com/01a1aca6-5385-7d3b-bfa4-7448591d2be1/image.png)
another important part was adding the screw holes. i had to position them carefully so that the back cover could be attached securely to the main body. i also made some adjustments to the dimensions and alignment while refining the model.

there are still a few small refinements left to do, but the main enclosure structure, mountings, back cover and screw holes are now designed. once the remaining details are finished i can move on to checking the complete model and preparing it for printing.

# 2026-11-05:  designing the main enclosure

**Total time spent: 2.5 hours**

after getting the joystick mount and display model corrected i started working on the main enclosure of pocket arcade. this was one of the more complex parts of the project because i wanted the console to have a proper handheld gaming device shape, similar to a gameboy, while still fitting all my components inside.

i started by planning the overall shape and dimensions of the console. i had to think about where the display and joystick would sit, how the buttons would be positioned and how much space would be needed inside for the esp32 and the wiring.
![image.png](https://cdn.hackclub.com/01a1aca3-72dd-7a9e-bad8-dd024ad555d4/image.png)
i used my display and joystick cad models as references while working on the enclosure. this helped me make sure that the important components would fit into the design instead of creating the body first and trying to fit everything afterwards.
![image.png](https://cdn.hackclub.com/01a1aca3-ace6-7c8f-bdd8-4c57f944e5db/image.png)
i also had to consider the thickness of the enclosure and the space needed around the components for mounting. getting all these things right took some planning because i wanted the console to be compact but not so small that assembling or using it would become difficult.
this was the starting point for the main enclosure design and i knew i would need to refine several parts before the model was ready to print.

# 2026-11-05: testing the printed parts and making adjustments

**Total time spent: 2.5 hours**

after getting the joystick mount and display template printed i started checking them against the actual components. the main purpose was to make sure the dimensions in my cad models matched the real parts before using them in the final enclosure.
![image.png](https://cdn.hackclub.com/01a1aca2-389f-7b81-91c9-3b6db0a7175a/image.png)
while testing i noticed that some dimensions were slightly off. a few measurements didn't match the actual components properly, so the fit wasn't exactly how i wanted it. i went back to the cad models and corrected the dimensions based on the measurements from the physical parts.

i also checked the positioning and clearances so that the joystick and display would sit properly without being too tight or leaving unnecessary gaps. this was important because even small measurement errors could cause problems later when assembling the console.

after making the adjustments i had more accurate models that i could use as references for designing the main enclosure.
![image.png](https://cdn.hackclub.com/01a1aca2-5041-725a-92c5-cd126ae18465/image.png)

# 2026-11-05: making my own cad models for the display and joystick

**Total time spent: 2 hours**

after getting the software and games working i started moving towards the physical design of the console. i wanted to make a proper enclosure that would hold all the components securely and still be comfortable to use.
![Screenshot_2026-11-05_at_10.26.43_AM.png](https://cdn.hackclub.com/01a1aa8a-3119-7df9-9ac7-200eb5eeaa65/Screenshot_2026-11-05_at_10.26.43_AM.png)
![Screenshot_2026-11-05_at_10.30.14_AM.png](https://cdn.hackclub.com/01a1aa8a-5a73-726b-81cf-76df7f35b439/Screenshot_2026-11-05_at_10.30.14_AM.png)
i started with the joystick mount and designed my own model for it. i wanted it to fit the actual joystick module properly instead of downloading a random model and trying to adjust my design around it.
![Screenshot_2026-11-05_at_11.03.39_AM.png](https://cdn.hackclub.com/01a1aa8a-7944-7174-911a-5465cbd6054b/Screenshot_2026-11-05_at_11.03.39_AM.png)
i also worked on making my own model of the tft display. the reason for this was that there are quite a lot of variations of these displays even when they look almost identical. the pcb size, mounting holes, screen position and other details can be different, so finding an exact model online was taking too much time.

instead i decided to measure my own display and recreate it in cad. i also printed a template to check the dimensions and see whether the model matched the actual display properly.

i wanted to get these two parts right before designing the full enclosure because even a small difference in their dimensions could cause problems during assembly. the joystick mount and display model will be used as references for the rest of the console design.

# 2026-11-05: putting everything together and testing

**Total time spent: 3 hours**

after finishing all four games i started bringing everything together into one complete program. until now i had been working on the games individually but now i needed to make sure they all worked properly from the same main menu.

i combined the different game functions and checked whether each game could be opened and exited without causing problems. i also tested the buttons because some of them had different uses depending on whether i was in the menu or playing a game.
![image.png](https://cdn.hackclub.com/01a1aa7e-3b21-79dd-ba1e-f3e209180e63/image.png)
one thing i had to be careful about was making sure that values from one game didn't affect another game. i also checked that the display was being updated properly when switching between screens and that old graphics from the previous game weren't left behind.
![image.png](https://cdn.hackclub.com/01a1aa7e-0e23-732a-8e22-9bd08c18c3db/image.png)
i went through each game again and tested the controls, scoring, restart options and game over screens. i also checked the overall flow from turning on the console to reaching the menu and starting a game.

there were a few small things that needed adjusting during testing, so i made changes and tested them again to make sure everything was working together properly.

after this i had a working version of pocket arcade with the boot screen, main menu, settings and all four games accessible from the same console.

# 2026-11-05: making space invaders

**Total time spent: 6 hours**

after getting tetris working i decided to add one more game which was space invaders. i wanted to make it a bit different from the other games, with enemies moving together across the screen and the player having to shoot them before they reach the bottom.
![image.png](https://cdn.hackclub.com/01a1aa7c-ba17-735e-8f6a-6854f8af1d3e/image.png)
i started by creating the alien formation and making it move from side to side. when the enemies reached the edge of the screen they would change direction and move a little further down. i also added the player at the bottom and used the joystick to move it left and right.
![image.png](https://cdn.hackclub.com/01a1aa7c-dd1f-7c64-aa7f-68c60c6df7ce/image.png)
then i worked on the shooting system. the player could shoot using the a button while the enemies could also fire bullets back. i added shields for protection, along with lives and a score counter so there was more to the game than just shooting the aliens.

one thing i had to pay attention to was updating the enemies and bullets without constantly clearing and redrawing the entire screen. otherwise the game would flicker and wouldn't look smooth. i worked on updating individual elements instead so the movement looked better.
![image.png](https://cdn.hackclub.com/01a1aa7d-3041-77a2-bdde-75e73f2ca3c0/image.png)
i also added waves so that once all the enemies were defeated a new formation would appear. i tested the movement, shooting and collisions several times to make sure the game was playable and that the different elements weren't interfering with each other.

after making the changes i got the main gameplay working with enemy movement, shooting, shields, lives and scoring.

# 2026-11-05: making the tetris game

**Total time spent: 5 hours**

after pong i started working on tetris. this was a bit more complicated because i had to handle different shapes, their movement and how they fit together on the game board.
![image.png](https://cdn.hackclub.com/01a1aa7a-da3d-70a4-891e-0f3ab25dbd70/image.png)
i started by creating the different tetromino shapes and making them move down the screen. i added controls to move the pieces left and right and rotate them while they were falling. i also had to work on the collision logic so that pieces would stop when they reached the bottom or landed on top of other blocks.
![image.png](https://cdn.hackclub.com/01a1aa7a-f8ad-7de4-9a22-e7cf4240be94/image.png)
![image.png](https://cdn.hackclub.com/01a1aa7b-202c-76a1-ab9a-7f07e843e28b/image.png)
one of the trickier parts was rotation. sometimes a piece would rotate near the edge of the board and end up going outside the playing area. i had to work on the rotation and wall handling so that the pieces would stay inside the board whenever possible.
![image.png](https://cdn.hackclub.com/01a1aa7b-584e-77b2-8397-d4830af47379/image.png)

then i added line clearing, where a completed row disappears and the blocks above it move down. i also added scoring and levels so the game gradually gets faster as the player progresses.

i spent quite a bit of time testing different pieces and checking what happened when they landed in different positions. i also worked on the pause and game over screens and made sure the game could be restarted properly.
![image.png](https://cdn.hackclub.com/01a1aa7b-a096-7c79-b0d4-e20247717333/image.png)
after making several changes to the movement and collision logic i finally got the main gameplay working the way i wanted.

# 2026-11-05: making the pong game

**Total time spent: 4.5 hours**

after finishing snake i moved on to pong. i wanted to make it a player versus computer game so that i could play against the console instead of needing another person to control the second paddle.
![image.png](https://cdn.hackclub.com/01a1aa79-a8bd-7e21-8b55-f51cf1fd48de/image.png)
i started by making the ball move around the screen and added the two paddles on opposite sides. i used the joystick to control my paddle and worked on making the computer move its paddle based on the ball's position.
![image.png](https://cdn.hackclub.com/01a1aa79-c7a0-7698-8f87-6a19231903b3/image.png)
![image.png](https://cdn.hackclub.com/01a1aa79-e633-7105-b363-e3e4d67600e9/image.png)
one of the things i had to get right was the ball movement and collision detection. the ball needed to bounce properly when it hit the top and bottom boundaries or either paddle. i also had to make sure the computer opponent wasn't too difficult or too easy to play against.

i added a scoring system where the first player to reach 5 points wins. while testing i also made changes to how the ball and paddles were updated on the display because i wanted the game to run smoothly without the screen constantly flickering.
![image.png](https://cdn.hackclub.com/01a1aa7a-09da-74b8-b04d-5e376434ff52/image.png)
after testing the controls and different ball movements i got the game working with the computer opponent and the scoring system. i also made sure the game could be exited and returned to the main menu.

# 2026-11-05: Snake Gameee

**Total time spent: 4 hours**

after getting the main menu working i started with the first game which was snake. i wanted to make the usual snake game where the player controls the snake using the joystick and tries to eat the food without hitting its own tail or the boundaries.

i first worked on the basic movement and getting the snake to move in the four directions. but while testing i noticed that the controls sometimes felt delayed and the snake would occasionally die even when it hadn't actually hit the wall or its own body. i had to go back and check the movement and collision logic because something wasn't being handled properly.
![image.png](https://cdn.hackclub.com/01a1aa77-38b7-7030-8c4e-ed14c6a954fb/image.png)
i also wanted to add a proper setup screen instead of starting the game immediately. so i added a play option and a speed setting where the player could choose how fast the snake moves. after pressing play the game shows a 3 2 1 countdown before starting.
![image.png](https://cdn.hackclub.com/01a1aa77-f805-7293-96fe-60e33dd6feaf/image.png)
there were also some display issues during the game. the game over message would sometimes appear while the snake was still visible and moving, and the boundary and food weren't always showing correctly. i fixed these by adjusting how the game screen was drawn and making sure the old screen elements were cleared at the right time.
![image.png](https://cdn.hackclub.com/01a1aa78-2f94-7ef5-b0c3-a422efe8ae2f/image.png)
i also added a red boundary around the playing area and made sure the snake only dies when it touches the boundary or its own tail. after a lot of testing and changes i finally got the game working properly with the speed settings and restart option.
![image.png](https://cdn.hackclub.com/01a1aa78-82da-7cbb-bb12-10c7952049fc/image.png)


# 2026-11-05: making the main menu

**Total time spent: 2 hours**

after finishing the boot screen i started working on the main menu. i wanted the menu to look simple but still have that retro gaming console feel. i added the pocket arcade title at the top and created a list of games that could be selected using the joystick.

i added snake, pong, tetris, space invaders and settings to the menu. i then worked on the navigation so that moving the joystick up and down would change the selected option. i also added the button controls to open a game or go back to the menu.
![image.png](https://cdn.hackclub.com/01a1aa76-0a0b-7d53-91d5-2b062c68cb06/image.png)
one thing i had to be careful about was making sure the menu and games worked properly together. i didn't want the menu controls to interfere with the controls inside a game or have the console get stuck on a screen after exiting a game.

i made a few changes to the selection highlighting and screen updates so it was easier to see which option was selected. after testing the navigation a few times i got the menu working and could open the different games from one place.

# 2026-11-05: Making of the boot screen animation

**Total time spent: 3 hours**

once the display and all the components were working properly i started working on the boot screen. i wanted the console to feel like an actual gaming device when it starts up instead of just showing a blank screen or jumping straight into the menu.
i made an intro screen with the fluxo bots name and the pocket arcade title. i also added an initializing message and a loading bar which fills up before showing the system ready message.
while making this i had to work on the positioning of the text and the loading bar so everything would fit properly on the display. i also had to make sure the screen was cleared correctly when moving from the boot screen to the main menu so that old text or graphics wouldn't remain visible.
![image.png](https://cdn.hackclub.com/01a1aa50-c0d8-7ea1-918c-b3f67354957e/image.png)
![Screenshot_2026-11-04_at_10.26.38_PM.png](https://cdn.hackclub.com/01a1aa76-8fc5-7ab8-adc6-faa582db6ae8/Screenshot_2026-11-04_at_10.26.38_PM.png)
after a few adjustments i got the boot screen working the way i wanted and it now gives the console a proper startup sequence.

# 2026-11-05: testing all the components

**Total time spent: 1.5 hours**

after getting the display working i started testing all the other components individually to make sure everything was working properly before moving ahead with the software.

i tested the joystick and checked the x and y values to see if they were changing correctly when i moved it in different directions. i also checked the joystick press button and made sure it was being detected.
![image.png](https://cdn.hackclub.com/01a1aa4c-99eb-7261-8d9a-57c3f9fca9b1/image.png)
then i tested all the physical buttons including a, b, start and select. i wanted to make sure each button was responding properly because these would be used for controlling the games and navigating through the menu.
![image.png](https://cdn.hackclub.com/01a1aa50-9352-726a-b6cf-a1f0e3897051/image.png)
i also tested the buzzer to check whether it could produce sounds and checked the rgb led by testing its different colour channels. this helped me confirm that all the output components were working too.

after testing everything separately i had a much better idea of how each component was behaving and could move on to building the actual console software without worrying about basic hardware issues.

# 2026-11-05: fixing the display issues

**Total time spent: 3 hours**

after connecting the display i started testing it properly but i noticed that it wasn't working as expected. only part of the screen was updating while the bottom portion sometimes remained unchanged.
![image.png](https://cdn.hackclub.com/01a1aa45-1a69-7203-a98a-1c2b18c75ba5/image.png)
i tried changing the display rotation and testing it in both portrait and landscape mode but the same issue kept happening. i also tried different colours and simple display tests to figure out whether it was a software issue or something related to the display configuration.
![image.png](https://cdn.hackclub.com/01a1aa45-7fe0-7d6c-8f5b-d5565d0d36a0/image.png)
![image.png](https://cdn.hackclub.com/01a1aa45-a626-7934-8301-22646ad839f1/image.png)
at one point i even tried splitting the screen into two parts and updating the top and bottom separately to see if i could get the entire display working. it took quite a few attempts because the display would sometimes update partially or leave parts of the previous screen unchanged.
![image.png](https://cdn.hackclub.com/01a1aa45-e7d9-765c-a0dd-74b0e4620964/image.png)
i kept testing different configurations and checking the display setup until i was able to get it working properly. so the issues was it was not 2nd copy it was 9 or 19th copyyy foollish me who bout it i had to change the user setup .h in teft library and write a custom one for this display 
#define ILI9341_2_DRIVER
#define TFT_RGB_ORDER TFT_BGR
#define TFT_INVERSION_ON

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF

#define TFT_WIDTH  240
#define TFT_HEIGHT 320

#define TFT_MOSI 23
#define TFT_MISO 19
#define TFT_SCLK 18

#define TFT_CS   22
#define TFT_DC   21
#define TFT_RST  4

#define LOAD_FONT8
#define LOAD_GFXFF

#define SPI_FREQUENCY  27000000


if yall wan t u can use it 

![image.png](https://cdn.hackclub.com/01a1aa46-1ae7-7fcb-b43b-cdb067f5a19f/image.png)


# 2026-11-05: getting the hardware connected

**Total time spent: 3 hours**

after figuring out what i wanted to make, i started working on the actual hardware. i connected the esp32 to the tft display, joystick, buttons, buzzer and rgb led.
![image.png](https://cdn.hackclub.com/01a1aa43-7ad8-7a66-82b4-92622849ae55/image.png)
this part took a bit of time because i had to make sure all the connections were correct and that i wasn't using conflicting gpio pins. i also had to plan the wiring properly since there were quite a few components and i didn't want the connections to get messy.
![image.png](https://cdn.hackclub.com/01a1aa43-9fdb-7bc5-bde2-5bf58021e3ac/image.png)
once everything was connected i started testing the individual components. i checked whether the display was responding, whether the joystick values were changing properly and whether the buttons were being detected. i also tested the buzzer and rgb led to make sure they were working.

there were a few things i had to check and fix along the way, especially with the pin connections and getting all the components to work together. i didn't want to start building the games before making sure the basic hardware was working properly.
after testing i was is dispersion display is either broken or its 2nd copy

# 2026-11-05: coming up with the idea

**Total time spent: 2 hours**

i wanted to make my own handheld gaming console for a while so i finally decided to start working on it. i wanted something like a gameboy but with my own design and games.

![image.png](https://cdn.hackclub.com/01a1aa3c-979e-73e3-99ba-06bb68bf7353/image.png)

at first i was just figuring out what all i could fit into it. i decided to use an esp32 with a tft display and a joystick for controls. i also wanted proper buttons, sound and a few games instead of making it run just one game.
i started planning the games i wanted to add. snake, pong and tetris were the first ones i had in mind. later i also decided to add space invaders.
i knew the software would probably take some time because i wanted all the games to work from one menu and use the same controls. i also had to think about how the components would fit inside a small case.i decided to get the electronics and games working first and then work on the enclosure. i didn't want to finalise the case before actually checking the display, joystick and other components because the dimensions and layout might change.

