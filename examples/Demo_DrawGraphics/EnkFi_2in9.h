#ifndef _ENKFI_2IN9_H_
#define _ENKFI_2IN9_H_

#include <Arduino.h>
#include <stdlib.h>
#include <SPI.h>

// Display Pins
#define DISPLAY_SCK_PIN  12
#define DISPLAY_MOSI_PIN 11
#define DISPLAY_CS_PIN   10
#define DISPLAY_RST_PIN  14
#define DISPLAY_DC_PIN   13
#define DISPLAY_BUSY_PIN 46

// Display resolution
#define ENKFI_WIDTH       128
#define ENKFI_HEIGHT      296

void ENKFI_Config(void);
void ENKFI_SPI(uint8_t data);
void ENKFI_Init(void);
void ENKFI_Clear(void);
void ENKFI_Display(uint8_t *Imageblack, uint8_t *Imagered);
void ENKFI_DisplayBLK(uint8_t *Imageblack);
void ENKFI_DisplayRED(uint8_t *Imagered);
void ENKFI_Sleep(void);

#endif
