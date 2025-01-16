#include "EnkFi_2in9.h"

// Configure E-Paper GPIO pins
void ENKFI_Config(void)
{
    pinMode(DISPLAY_BUSY_PIN,  INPUT);
    pinMode(DISPLAY_RST_PIN , OUTPUT);
    pinMode(DISPLAY_DC_PIN  , OUTPUT);
    
    pinMode(DISPLAY_SCK_PIN, OUTPUT);
    pinMode(DISPLAY_MOSI_PIN, OUTPUT);
    pinMode(DISPLAY_CS_PIN , OUTPUT);

    digitalWrite(DISPLAY_CS_PIN , HIGH);
    digitalWrite(DISPLAY_SCK_PIN, LOW);
}

/******************************************************************************
function:
      SPI read and write
******************************************************************************/
void ENKFI_SPI(uint8_t data)
{
    //SPI.beginTransaction(spi_settings);
    digitalWrite(DISPLAY_CS_PIN, 0);

    for (int i = 0; i < 8; i++)
    {
        if ((data & 0x80) == 0) digitalWrite(DISPLAY_MOSI_PIN, 0); 
        else                    digitalWrite(DISPLAY_MOSI_PIN, 1);

        data <<= 1;
        digitalWrite(DISPLAY_SCK_PIN, 1);     
        digitalWrite(DISPLAY_SCK_PIN, 0);
    }

    //SPI.transfer(data);
    digitalWrite(DISPLAY_CS_PIN, 1);
    //SPI.endTransaction(); 
}

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void ENKFI_Reset(void)
{
    digitalWrite(DISPLAY_RST_PIN, 1);
    delay(200);
    digitalWrite(DISPLAY_RST_PIN, 0);
    delay(2);
    digitalWrite(DISPLAY_RST_PIN, 1);
    delay(200);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void ENKFI_SendCommand(uint8_t Reg)
{
    digitalWrite(DISPLAY_DC_PIN, 0);
    digitalWrite(DISPLAY_CS_PIN, 0);
    ENKFI_SPI(Reg);
    digitalWrite(DISPLAY_CS_PIN, 1);
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
static void ENKFI_SendData(uint8_t Data)
{
    digitalWrite(DISPLAY_DC_PIN, 1);
    digitalWrite(DISPLAY_CS_PIN, 0);
    ENKFI_SPI(Data);
    digitalWrite(DISPLAY_CS_PIN, 1);
}

/******************************************************************************
function :	Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
static void ENKFI_ReadBusy(void)
{
    Serial.print("e-Paper busy\r\n");
    while(digitalRead(DISPLAY_BUSY_PIN) == 1) {      //0: busy, 1: idle
        delay(20);
    }
    Serial.print("e-Paper busy release\r\n");
}

static void ENKFI_TurnOnDisplay(void)
{
	ENKFI_SendCommand(0x20); 
	ENKFI_ReadBusy();
}

/******************************************************************************
function :	Setting the display window
parameter:
******************************************************************************/
static void ENKFI_SetWindows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend)
{
    ENKFI_SendCommand(0x44); // SET_RAM_X_ADDRESS_START_END_POSITION
    ENKFI_SendData((Xstart>>3) & 0xFF);
    ENKFI_SendData((Xend>>3) & 0xFF);
	
    ENKFI_SendCommand(0x45); // SET_RAM_Y_ADDRESS_START_END_POSITION
    ENKFI_SendData(Ystart & 0xFF);
    ENKFI_SendData((Ystart >> 8) & 0xFF);
    ENKFI_SendData(Yend & 0xFF);
    ENKFI_SendData((Yend >> 8) & 0xFF);
}

/******************************************************************************
function :	Set Cursor
parameter:
******************************************************************************/
static void ENKFI_SetCursor(uint16_t Xstart, uint16_t Ystart)
{
    ENKFI_SendCommand(0x4E); // SET_RAM_X_ADDRESS_COUNTER
    ENKFI_SendData(Xstart & 0xFF);

    ENKFI_SendCommand(0x4F); // SET_RAM_Y_ADDRESS_COUNTER
    ENKFI_SendData(Ystart & 0xFF);
    ENKFI_SendData((Ystart >> 8) & 0xFF);
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void ENKFI_Init(void)
{
	ENKFI_Reset();

	ENKFI_ReadBusy();
	ENKFI_SendCommand(0x12);    
	ENKFI_ReadBusy();
	
	ENKFI_SendCommand(0x00);    
	ENKFI_SendData(0x27);
	ENKFI_SendData(0x01);
	ENKFI_SendData(0x00);
	
	ENKFI_SendCommand(0x11);    
	ENKFI_SendData(0x03);

	ENKFI_SetWindows(0, 0, ENKFI_WIDTH-1, ENKFI_HEIGHT-1);
	ENKFI_SetCursor(0, 0);
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void ENKFI_Clear(void)
{    
    uint16_t Width, Height;
    Width = (ENKFI_WIDTH % 8 == 0)? (ENKFI_WIDTH / 8 ): (ENKFI_WIDTH / 8 + 1);
    Height = ENKFI_HEIGHT;

    ENKFI_SendCommand(0x24);
    for (uint16_t j = 0; j < Height; j++) {
        for (uint16_t i = 0; i < Width; i++) {
            ENKFI_SendData(0Xff);
        }
    }
    ENKFI_SendCommand(0x26);
    for (uint16_t j = 0; j < Height; j++) {
        for (uint16_t i = 0; i < Width; i++) {
            ENKFI_SendData(0X00);
        }
    }

	ENKFI_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void ENKFI_Display(uint8_t *Imageblack,uint8_t *Imagered)
{
    uint16_t Width, Height;
    Width = (ENKFI_WIDTH % 8 == 0)? (ENKFI_WIDTH / 8 ): (ENKFI_WIDTH / 8 + 1);
    Height = ENKFI_HEIGHT;

    ENKFI_SendCommand(0x24);
    for (uint16_t j = 0; j < Height; j++) {
        for (uint16_t i = 0; i < Width; i++) {
            ENKFI_SendData(Imageblack[i + j * Width]);
        }
    }
    
    ENKFI_SendCommand(0x26);
    for (uint16_t j = 0; j < Height; j++) {
        for (uint16_t i = 0; i < Width; i++) {
            ENKFI_SendData(~Imagered[i + j * Width]);
        }
    }

	ENKFI_TurnOnDisplay();
}

/******************************************************************************/
void ENKFI_DisplayBLK(uint8_t *Imageblack)
{
    uint16_t Width, Height;
    Width = (ENKFI_WIDTH % 8 == 0)? (ENKFI_WIDTH / 8 ): (ENKFI_WIDTH / 8 + 1);
    Height = ENKFI_HEIGHT;

    ENKFI_SendCommand(0x24);
    for (uint16_t j = 0; j < Height; j++) {
        for (uint16_t i = 0; i < Width; i++) {
            ENKFI_SendData(Imageblack[i + j * Width]);
        }
    }

  ENKFI_TurnOnDisplay();
}

/******************************************************************************/
void ENKFI_DisplayRED(uint8_t *Imagered)
{
    uint16_t Width, Height;
    Width = (ENKFI_WIDTH % 8 == 0)? (ENKFI_WIDTH / 8 ): (ENKFI_WIDTH / 8 + 1);
    Height = ENKFI_HEIGHT;

    ENKFI_SendCommand(0x26);
    for (uint16_t j = 0; j < Height; j++) {
        for (uint16_t i = 0; i < Width; i++) {
            ENKFI_SendData(~Imagered[i + j * Width]);
        }
    }

  ENKFI_TurnOnDisplay();
}


/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void ENKFI_Sleep(void)
{
  	ENKFI_SendCommand(0x10);  // Deep sleep
  	ENKFI_SendData(0x01);
}
