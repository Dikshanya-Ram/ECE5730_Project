#ifndef BATTLESHIP_MAIN_H
#define BATTLESHIP_MAIN_H

/**
 *
 * HARDWARE CONNECTIONS
 *  - GPIO 16 ---> VGA Hsync
 *  - GPIO 17 ---> VGA Vsync
 *  - GPIO 18 ---> 470 ohm resistor ---> VGA Green
 *  - GPIO 19 ---> 330 ohm resistor ---> VGA Green
 *  - GPIO 20 ---> 330 ohm resistor ---> VGA Blue
 *  - GPIO 21 ---> 330 ohm resistor ---> VGA Red
 *  - RP2040 GND ---> VGA GND

 * Joystick CONNECTIONS
 *  - GND      -->  Pin 1
 *  - GPIO 28  -->  330 ohms  --> Pin 2 (move to down)
 *  - GPIO 27  -->  330 ohms  --> Pin 3 (move to up)
 *  - GPIO 26  -->  330 ohms  --> Pin 4 (move to left)
 *  - GPIO 22  -->  330 ohms  --> Pin 5 (move to )

 * RESOURCES USED
 *
 */

// Include the VGA grahics library
#include "vga256_graphics.h"

// Include standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "fix15.h"

// Include Pico libraries
#include "pico/stdlib.h"
#include "pico/divider.h"
#include "pico/multicore.h"

// Include hardware libraries
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "hardware/clocks.h"
#include "hardware/pll.h"

// Include protothreads
// #include "pt_cornell_rp2040_v1_1_1.h"

// dma include library
#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/spi.h"

// Battleship
#include "BattleshipGame.h"
#include "vga_displayElements.h"
#include "picow_udp_beacon.h"
// #include "connect.h"

//========= IRQ Elements==============
#include "hardware/sync.h"

// IRQ: Low-level alarm infrastructure we'll be using
#define ALARM_NUM 0
#define ALARM_IRQ TIMER_IRQ_0
#define DELAY 20000 // in microseconds (0.02s for button checking)

// GPIO for timing the ISR
#define ISR_GPIO 2

//Sound IRQ
#define SOUND_ALARM_NUM 1
#define SOUND_ALARM_IRQ TIMER_IRQ_1
#define SOUND_DELAY 89 // in microseconds (0.02s for button checking)

// GPIO for timing the ISR
#define SOUND_ISR_GPIO 3

// ======== DMA audio ===================
//Sound
#include "splash.h"
#include "boom.h"

// =================== PINS ==================

#define LED_PIN 25
#define BUT_PIN_Y 18 // LEFT
#define BUT_PIN_R 19 // RIGHT

#define JOY_RIGHT 22
#define JOY_LEFT 26
#define JOY_UP 27
#define JOY_DOWN 28

// uS per frame
#define FRAME_RATE 33000 // 33000

enum PlayerState
{
  YT,
  RR,
  RA
}; // Your Turn, Ready Response, Ready Attack
PlayerState my_state = YT;
PlayerState opponent_state = RA;

// =========================== DMA related setting =================
// Number of samples per period in sine table
#define sine_table_size 256

// Sine table
int raw_sin[sine_table_size];

// Table of values to be sent to DAC
unsigned short DAC_data[sine_table_size];
unsigned short splash_data[splash_audio_len];
unsigned short boom_data[boom_audio_len];

// Pointer to the address of the DAC data table
// unsigned short *address_pointer = &DAC_data[0];
unsigned short *splash_pointer = &splash_data[0];
unsigned short *boom_pointer = &boom_data[0];

// A-channel, 1x, active
#define DAC_config_chan_A 0b0011000000000000

// SPI configurations
#define PIN_MISO 4
#define PIN_CS 5
#define PIN_SCK 6
#define PIN_MOSI 7
#define SPI_PORT spi0

// Number of DMA transfers per event
const uint32_t transfer_count = sine_table_size;

#define STRIDE_LENGTH 10

void spawnCursor(fix15 *x, fix15 *y, fix15 *vx, fix15 *vy);
void checkPOS(fix15 *x, fix15 *y, fix15 *vx, fix15 *vy);
Coordinate8 isInMYGRID(int pos_x, int pos_y);
Coordinate8 isInOtherGRID(int pos_x, int pos_y);
void drawTextforGameState(PlayerState my_state_1);
#endif /* BATTLESHIP_MAIN_H */