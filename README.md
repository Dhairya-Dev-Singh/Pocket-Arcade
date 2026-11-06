# POCKET ARCADE ESP32 Handheld Gaming Console

this is the complete codebase and build documentation for my pocket arcade handheld gaming console. it includes the esp32 code, circuit connections, cad models and setup information you need to build your own compact gaming console using an esp32 development board, 2.4 inch tft display, joystick and physical buttons

the main idea behind this project was to make my own handheld gaming console instead of buying one. i wanted something inspired by the classic gameboy but with my own design, controls and games. i decided to use an esp32 as the main controller and a tft display for the interface.

the esp32 handles the game logic, reads the joystick and button inputs and controls the different parts of the console. the tft display provides the visuals while the joystick and buttons are used to navigate the menu and play games.

as the project developed i added four games which are snake, pong, tetris and space invaders. i also made a custom boot screen, main menu, settings screen and designed my own 3d printed enclosure instead of using a ready-made case.

<img width="1200" height="1600" alt="image" src="https://github.com/user-attachments/assets/23ea4ae1-7cc8-4173-b1db-1bbd130aed1f" />
<img width="1600" height="1200" alt="image" src="https://github.com/user-attachments/assets/098f8223-c4a3-4bbc-b7b0-c9457d00a654" />
<img width="1600" height="1200" alt="image" src="https://github.com/user-attachments/assets/021d29fa-f2b7-44a7-bc3f-bd1ce1ca8add" />
<img width="1600" height="1200" alt="image" src="https://github.com/user-attachments/assets/fe0414ca-5d5a-45a7-8c50-e1692886bf79" />


## Features

- compact esp32 based handheld gaming console
- 2.4 inch 240x320 tft display
- custom pocket arcade boot screen
- animated loading sequence
- custom main menu interface
- joystick based menu navigation
- four physical control buttons
- snake game with adjustable speed
- snake countdown before gameplay
- snake score and game over system
- pong game against a computer opponent
- pong scoring system with first to 5 points
- tetris with all seven tetromino shapes
- piece rotation and collision detection
- line clearing and scoring system
- tetris levels with increasing speed
- space invaders with moving enemy formations
- player shooting and enemy bullets
- shields, lives and score counter
- multiple enemy waves
- pause and game over screens
- buzzer for sound effects
- rgb led for visual feedback
- custom designed cad models for the display and joystick
- self designed handheld enclosure
- 3d printed enclosure with internal mountings
- removable back cover with screw holes
- custom soldered wiring instead of a permanent breadboard setup

## BOM Bill of Materials

| component | quantity | purpose | purchase link | price |
|-----------|:--------:|---------|:-------------:|------:|
| ESP32 development board CP2102 30-pin | 1 | main controller and game processing | [buy here](https://robocraze.com/products/esp32-development-board-with-cp2102-wifi-bluetooth-dual-core-30-pin?variant=53806381531360) | $4.75 |
| 2.4 inch SPI TFT display 240x320 | 1 | game graphics and user interface | [buy here](https://robu.in/product/2-4-inch-spi-interface-240x320-touch-screen-tft-display-module/) | $7.50 |
| PS2 joystick module | 1 | game controls and menu navigation | [buy here](https://robu.in/product/joystick-module-ps2-breakout-sensor/) | $0.50 |
| tactile push buttons 12x12mm | 1 pack (5 pieces) | A, B, START and SELECT controls | [buy here](https://robocraze.com/products/tactile-push-button-switch-4-pin-12x12x5mm-pack-of-5?variant=48070563070176) | $0.10 |
| active buzzer module | 1 | sound effects and game feedback | [buy here](https://robocraze.com/products/pcb-mounted-active-buzzer-module?variant=51266183561440) | $0.30 |
| RGB LED module | 1 | visual feedback and status indication | [buy here](https://robocraze.com/products/rgb-led-module-for-boards-compatible-with-arduino?variant=40194394685593) | $0.30 |
| jumper wires | as required | initial prototyping and connections | none | — |
| solder and connecting wires | as required | permanent hardware connections | none | — |
| USB cable | 1 | programming and powering the ESP32 | none | — |
| 3d printed enclosure | 1 | holds and protects all the components | self designed | — |

**estimated component cost: $13.45**

the prices above are the prices i used when preparing the bom and may change depending on the seller. the total does not include the enclosure printing, solder, wires or other miscellaneous materials.

## Circuit Diagram

i have used the following pin connections for the main pocket arcade hardware. you can use them as a reference while connecting your own components.

### Main Connections

<img width="1334" height="1048" alt="image" src="https://github.com/user-attachments/assets/608bb83c-e5a6-4a2b-a4b1-ac23738f83a8" />

### TFT Display to ESP32

| TFT display | ESP32 |
|-------------|-------|
| VCC | 3.3V |
| GND | GND |
| CS | GPIO 22 |
| RESET | GPIO 4 |
| DC / RS | GPIO 21 |
| MOSI / SDA | GPIO 23 |
| MISO / SDO | GPIO 19 |
| SCK / CLK | GPIO 18 |
| LED / BL | 3.3V |

the display uses SPI communication. the backlight is connected directly to 3.3V, so its brightness is not controlled through the code.

### Joystick to ESP32

| joystick | ESP32 |
|----------|-------|
| VCC | 3.3V |
| GND | GND |
| VRx | GPIO 34 |
| VRy | GPIO 35 |
| SW | GPIO 32 |

the joystick provides analog x and y values which are used for movement and menu navigation. its built-in push button is also connected to the esp32.

### Control Buttons to ESP32

| button | ESP32 |
|--------|-------|
| A | GPIO 13 |
| B | GPIO 14 |
| START | GPIO 27 |
| SELECT | GPIO 26 |

each button is connected between its GPIO pin and GND. the code uses the esp32 internal pull-up configuration to detect button presses.

### Buzzer to ESP32

| buzzer | ESP32 |
|--------|-------|
| Positive (+) | GPIO 25 |
| Negative (-) | GND |

the buzzer is used to produce sound effects and feedback during gameplay.

### RGB LED to ESP32

| RGB LED channel | ESP32 |
|-----------------|-------|
| Red | GPIO 15 |
| Green | GPIO 2 |
| Blue | GPIO 33 |

the RGB channels are controlled individually and use active-HIGH logic.

all components share a common ground. make sure the power connections are correct and check the voltage requirements of your modules before powering the circuit.

## How to Build This

step 1 first get all the components mentioned in the bom. the main parts required are the esp32 development board, 2.4 inch tft display, joystick module, four buttons, buzzer and rgb led.

step 2 connect the tft display to the esp32 using the pin connections mentioned above. make sure the SPI pins match the display configuration used in the code.

step 3 connect the joystick module to the esp32. connect VRx to GPIO 34, VRy to GPIO 35 and SW to GPIO 32. connect its VCC and GND properly.

step 4 connect the four physical buttons to GPIO 13, GPIO 14, GPIO 27 and GPIO 26. each button should connect its assigned GPIO to GND when pressed.

step 5 connect the buzzer to GPIO 25 and the RGB LED channels to GPIO 15, GPIO 2 and GPIO 33.

step 6 once all the connections are checked, connect the esp32 to your computer using a USB cable. make sure the board is detected properly before uploading the code.

step 7 open Arduino IDE and install the required libraries. the project uses TFT_eSPI for the display interface. make sure the TFT_eSPI configuration matches the display driver and pin definitions used in the project.

step 8 select the correct ESP32 board and port in Arduino IDE and upload the pocket arcade code.

step 9 after uploading, check whether the boot screen and main menu appear correctly. test the joystick and all four buttons before starting the games.

step 10 open each game and test its controls, scoring, restart and game over behaviour. also check the buzzer and RGB LED.

step 11 once the electronics are working properly, prepare the enclosure. use the CAD models as references while positioning the display, joystick and other components.

step 12 finally install the electronics inside the 3d printed enclosure and secure the back cover using screws. be careful while fitting the display because applying too much pressure can damage the screen.

## Main Menu

the main menu is the starting point of pocket arcade. i wanted the interface to feel like an actual handheld gaming console instead of just showing a list of games on a display.

the menu uses the joystick for navigation and the buttons for selecting options and going back. i used a dark background with green highlights and white text to give it a simple retro gaming look.

the main menu includes

- snake
- pong
- tetris
- space invaders
- settings

each game opens its own screen and has its own controls and gameplay logic. the menu also makes it easier to add more games or features in the future without changing the entire console interface.

## Snake

snake was the first game i implemented for pocket arcade. the idea is to control the snake using the joystick and collect food while avoiding the boundaries and its own tail.

i added a separate setup screen so the player can choose the snake speed before starting. after pressing play, the game shows a 3 2 1 countdown before gameplay begins.

the game includes

- joystick based snake movement
- adjustable snake speed
- food generation
- score tracking
- red boundary around the playing area
- collision detection with the wall and snake's own tail
- game over screen
- restart option

during development i had to fix some issues with delayed controls and incorrect collision detection. i also worked on the screen updates because sometimes the food or boundary would not appear correctly and old game over messages would remain on the display.

after making changes to the movement and drawing logic i got the game working with the speed settings, countdown and restart system.

## Pong

pong is a player versus computer game where the player controls one paddle using the joystick while the computer controls the other paddle.

the ball moves around the screen and bounces off the boundaries and paddles. the objective is to score points by getting the ball past the opponent's paddle.

the game includes

- player controlled paddle
- computer controlled opponent
- ball movement and paddle collision
- top and bottom boundary collision
- scoring system
- first to 5 points wins
- game over and return to menu

one of the important parts was getting the ball collision and movement to work properly. i also adjusted how the screen elements were updated so the game could run smoothly without unnecessary flickering.

## Tetris

tetris is one of the more complex games in pocket arcade because it involves different shapes, rotation and collision handling.

the player controls falling tetrominoes and tries to complete horizontal rows. whenever a row is completed it disappears and the blocks above it move down.

the game includes

- all seven tetromino shapes
- left and right movement
- piece rotation
- collision detection
- wall handling
- automatic piece movement
- line clearing
- score tracking
- levels and increasing fall speed
- pause screen
- game over and restart

one of the trickier parts was rotation near the edges of the board. i had to adjust the rotation and collision logic so pieces would stay within the playing area whenever possible.

## Space Invaders

space invaders is an arcade style shooting game where the player has to defeat enemy formations before they reach the bottom of the screen.

the player is positioned at the bottom and can move left and right using the joystick. the A button is used to shoot at the enemies while the enemy formation moves across the screen and gradually moves down.

the game includes

- moving enemy formations
- direction changes at screen edges
- player movement using joystick
- player shooting
- enemy bullets
- shields for protection
- lives and score counter
- multiple enemy waves
- pause and game over states

i also worked on updating individual game elements instead of constantly clearing and redrawing the entire screen. this helped reduce flickering and made the movement smoother.

## Settings

the settings option is included in the main menu to provide a separate place for console options. snake also has its own setup screen where the player can select the game speed before starting.

the interface is designed so that additional settings can be added later without needing to redesign the whole menu.

## Code

the main project uses the following components and concepts

- Arduino framework for ESP32
- TFT_eSPI library for the TFT display
- SPI communication for the display
- analog input reading for joystick movement
- digital input handling for buttons
- internal pull-up configuration for button inputs
- buzzer control for sound effects
- RGB LED channel control
- game state management
- collision detection and scoring systems
- custom menu and screen drawing functions

the pin configuration used by the project is

```cpp
#define JOY_X 34
#define JOY_Y 35
#define JOY_SW 32

#define BTN_A 13
#define BTN_B 14
#define BTN_START 27
#define BTN_SELECT 26

#define BUZZER 25

#define LED_R 15
#define LED_G 2
#define LED_B 33

#define TFT_MOSI 23
#define TFT_MISO 19
#define TFT_SCLK 18
#define TFT_CS 22
#define TFT_DC 21
#define TFT_RST 4
```

the TFT_eSPI library also needs to be configured for the correct display driver and SPI pins. the display setup used in this project is based on the ILI9341 configuration.

if you change any of the hardware connections, make sure to update the corresponding pin definitions and display configuration in the code.

## Known Issues

1. the display backlight is connected directly to 3.3V so brightness cannot currently be controlled through software
2. the joystick may need calibration depending on its analog readings and resting position
3. button inputs should be checked for proper pull-up behaviour and unwanted repeated presses
4. the display requires the correct TFT_eSPI driver and configuration
5. incorrect SPI connections can cause a blank screen or incorrect display output
6. the RGB LED wiring must match the active-HIGH logic used in the code
7. the enclosure dimensions and component alignment should be checked before final assembly
8. the display needs to be handled carefully while fitting it into the enclosure
9. the final wiring should be checked for loose solder joints and short circuits

## CAD Models

i designed my own CAD models for the display and joystick instead of relying on random models from the internet. there are quite a few variations of these modules, so i wanted the models to match the actual dimensions of my components.

i also designed the main handheld enclosure with internal mountings, screw holes and a removable back cover. the overall design is inspired by classic handheld gaming consoles.

<img width="1620" height="1472" alt="image" src="https://github.com/user-attachments/assets/f62b4ac9-9a0b-4644-b47a-56f01dff60a6" />
<img width="1922" height="1522" alt="image" src="https://github.com/user-attachments/assets/3b5e760e-b7d1-4a45-9839-c88dd47e8e70" />
<img width="3024" height="1964" alt="image" src="https://github.com/user-attachments/assets/d75f80aa-f8bc-40b1-ac1a-863c9757d697" />


## Working Principle

the basic idea behind pocket arcade is to use the ESP32 as the main controller and combine multiple games into one compact handheld device.

the joystick and buttons provide user input while the TFT display shows the menu, game graphics and different screens. the ESP32 continuously reads the controls and runs the logic for whichever game is currently active.

the overall structure looks something like this

```text
                 ESP32
                   |
        +----------+----------+
        |          |          |
      DISPLAY   JOYSTICK    BUTTONS
        |          |          |
        +----------+----------+
                   |
               MAIN MENU
                   |
       +-----------+-----------+
       |           |           |
      SNAKE       PONG       TETRIS
                               |
                         SPACE INVADERS

                   |
             BUZZER + RGB LED
```

when the console starts, the boot screen is displayed first and then the main menu appears. the player can move through the menu using the joystick and select a game using the buttons.

once a game is selected, the ESP32 runs that game's logic and updates the display accordingly. the buttons and joystick are interpreted differently depending on which game is active.

this allows four different games to run on the same hardware without needing separate controllers or displays.

## Final Result

the whole idea behind pocket arcade was to build my own handheld gaming console inspired by the classic gameboy. i started with an ESP32, a small TFT display and a joystick, and gradually turned them into a console with its own interface, controls and games.

the project now brings together snake, pong, tetris and space invaders along with a custom boot screen, main menu, sound effects and RGB lighting. i also designed the enclosure myself so that the electronics could fit into a compact handheld body.

it was a good way to learn more about embedded programming, display interfaces, game logic, input handling, collision detection, soldering and CAD design. one of the biggest challenges was getting all the different parts to work together while keeping the console small enough to use comfortably.

there is still room to improve the enclosure and add more features, but the main goal was to create a handheld console that i designed and programmed myself.

pocket arcade is an ongoing personal project and the codebase can be used as a starting point for anyone interested in making their own ESP32 based handheld gaming console.
