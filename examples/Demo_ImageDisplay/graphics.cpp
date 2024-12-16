#include "graphics.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h> //memset()
#include <math.h>

GRAPHICS Graphics;

/******************************************************************************
function: Create Image
parameter:
    image   :   Pointer to the image cache
    width   :   The width of the picture
    Height  :   The height of the picture
    Color   :   Whether the picture is inverted
******************************************************************************/
void Graphics_NewImage(uint8_t *image, uint16_t Width, uint16_t Height, uint16_t Rotate, uint16_t Color)
{
    Graphics.Image = NULL;
    Graphics.Image = image;

    Graphics.WidthMemory = Width;
    Graphics.HeightMemory = Height;
    Graphics.Color = Color;    
    Graphics.Scale = 2;
    Graphics.WidthByte = (Width % 8 == 0)? (Width / 8 ): (Width / 8 + 1);
    Graphics.HeightByte = Height;    
//    printf("WidthByte = %d, HeightByte = %d\r\n", Graphics.WidthByte, Graphics.HeightByte);
//    printf(" EPD_WIDTH / 8 = %d\r\n",  122 / 8);
   
    Graphics.Rotate = Rotate;
    Graphics.Mirror = MIRROR_NONE;
    
    if(Rotate == ROTATE_0 || Rotate == ROTATE_180) {
        Graphics.Width = Width;
        Graphics.Height = Height;
    } else {
        Graphics.Width = Height;
        Graphics.Height = Width;
    }
}

/******************************************************************************
function: Select Image
parameter:
    image : Pointer to the image cache
******************************************************************************/
void Graphics_SelectImage(uint8_t *image)
{
    Graphics.Image = image;
}

/******************************************************************************
function: Select Image Rotate
parameter:
    Rotate : 0,90,180,270
******************************************************************************/
void Graphics_SetRotate(uint16_t Rotate)
{
    if(Rotate == ROTATE_0 || Rotate == ROTATE_90 || Rotate == ROTATE_180 || Rotate == ROTATE_270) {
        // printf("Set image Rotate %d\r\n", Rotate);
        Graphics.Rotate = Rotate;
    } else {
        printf("rotate = 0, 90, 180, 270\r\n");
    }
}

/******************************************************************************
function:	Select Image mirror
parameter:
    mirror   :Not mirror,Horizontal mirror,Vertical mirror,Origin mirror
******************************************************************************/
void Graphics_SetMirroring(uint8_t mirror)
{
    if(mirror == MIRROR_NONE || mirror == MIRROR_HORIZONTAL || 
        mirror == MIRROR_VERTICAL || mirror == MIRROR_ORIGIN) {
        // printf("mirror image x:%s, y:%s\r\n",(mirror & 0x01)? "mirror":"none", ((mirror >> 1) & 0x01)? "mirror":"none");
        Graphics.Mirror = mirror;
    } else {
        printf("mirror should be MIRROR_NONE, MIRROR_HORIZONTAL, \
        MIRROR_VERTICAL or MIRROR_ORIGIN\r\n");
    }    
}

void Graphics_SetScale(uint8_t scale)
{
    if(scale == 2){
        Graphics.Scale = scale;
        Graphics.WidthByte = (Graphics.WidthMemory % 8 == 0)? (Graphics.WidthMemory / 8 ): (Graphics.WidthMemory / 8 + 1);
    }
	else if(scale == 4) {
        Graphics.Scale = scale;
        Graphics.WidthByte = (Graphics.WidthMemory % 4 == 0)? (Graphics.WidthMemory / 4 ): (Graphics.WidthMemory / 4 + 1);
    }
	else if(scale == 7) {//Only applicable with 5in65 e-Paper
		Graphics.Scale = 7;
		Graphics.WidthByte = (Graphics.WidthMemory % 2 == 0)? (Graphics.WidthMemory / 2 ): (Graphics.WidthMemory / 2 + 1);
	}
	else {
        printf("Set Scale Input parameter error\r\n");
        printf("Scale Only support: 2 4 7\r\n");
    }
}
/******************************************************************************
function: Draw Pixels
parameter:
    Xpoint : At point X
    Ypoint : At point Y
    Color  : Graphicsed colors
******************************************************************************/
void Graphics_SetPixel(uint16_t Xpoint, uint16_t Ypoint, uint16_t Color)
{
    if(Xpoint > Graphics.Width || Ypoint > Graphics.Height){
        printf("Exceeding display boundaries\r\n");
        return;
    }      
    uint16_t X, Y;
    switch(Graphics.Rotate) {
    case 0:
        X = Xpoint;
        Y = Ypoint;  
        break;
    case 90:
        X = Graphics.WidthMemory - Ypoint - 1;
        Y = Xpoint;
        break;
    case 180:
        X = Graphics.WidthMemory - Xpoint - 1;
        Y = Graphics.HeightMemory - Ypoint - 1;
        break;
    case 270:
        X = Ypoint;
        Y = Graphics.HeightMemory - Xpoint - 1;
        break;
    default:
        return;
    }
    
    switch(Graphics.Mirror) {
    case MIRROR_NONE:
        break;
    case MIRROR_HORIZONTAL:
        X = Graphics.WidthMemory - X - 1;
        break;
    case MIRROR_VERTICAL:
        Y = Graphics.HeightMemory - Y - 1;
        break;
    case MIRROR_ORIGIN:
        X = Graphics.WidthMemory - X - 1;
        Y = Graphics.HeightMemory - Y - 1;
        break;
    default:
        return;
    }

    if(X > Graphics.WidthMemory || Y > Graphics.HeightMemory){
        printf("Exceeding display boundaries\r\n");
        return;
    }
    
    if(Graphics.Scale == 2){
        uint32_t Addr = X / 8 + Y * Graphics.WidthByte;
        uint8_t Rdata = Graphics.Image[Addr];
        if(Color == BLACK)
            Graphics.Image[Addr] = Rdata & ~(0x80 >> (X % 8));
        else
            Graphics.Image[Addr] = Rdata | (0x80 >> (X % 8));
    }else if(Graphics.Scale == 4){
        uint32_t Addr = X / 4 + Y * Graphics.WidthByte;
        Color = Color % 4;//Guaranteed color scale is 4  --- 0~3
        uint8_t Rdata = Graphics.Image[Addr];
        
        Rdata = Rdata & (~(0xC0 >> ((X % 4)*2)));
        Graphics.Image[Addr] = Rdata | ((Color << 6) >> ((X % 4)*2));
    }else if(Graphics.Scale == 7){
			uint16_t Width = Graphics.WidthMemory*3%8 == 0 ? Graphics.WidthMemory*3/8 : Graphics.WidthMemory*3/8+1;
			uint32_t Addr = (Xpoint * 3) / 8 + Ypoint * Width;
			uint8_t shift, Rdata, Rdata2;
			shift = (Xpoint+Ypoint*Graphics.HeightMemory) % 8;

			switch(shift) {
				case 0 :
					Rdata = Graphics.Image[Addr] & 0x1f;
					Rdata = Rdata | ((Color << 5) & 0xe0);
					Graphics.Image[Addr] = Rdata;
					break;
				case 1 :
					Rdata = Graphics.Image[Addr] & 0xe3;
					Rdata = Rdata | ((Color << 2) & 0x1c);
					Graphics.Image[Addr] = Rdata;
					break;
				case 2 :
					Rdata = Graphics.Image[Addr] & 0xfc;
					Rdata2 = Graphics.Image[Addr + 1] & 0x7f;
					Rdata = Rdata | ((Color >> 1) & 0x03);
					Rdata2 = Rdata2 | ((Color << 7) & 0x80);
					Graphics.Image[Addr] = Rdata;
					Graphics.Image[Addr + 1] = Rdata2;
					break;
				case 3 :
					Rdata = Graphics.Image[Addr] & 0x8f;
					Rdata = Rdata | ((Color << 4) & 0x70);
					Graphics.Image[Addr] = Rdata;
					break;
				case 4 :
					Rdata = Graphics.Image[Addr] & 0xf1;
					Rdata = Rdata | ((Color << 1) & 0x0e);
					Graphics.Image[Addr] = Rdata;
					break;
				case 5 :
					Rdata = Graphics.Image[Addr] & 0xfe;
					Rdata2 = Graphics.Image[Addr + 1] & 0x3f;
					Rdata = Rdata | ((Color >> 2) & 0x01);
					Rdata2 = Rdata2 | ((Color << 6) & 0xc0);
					Graphics.Image[Addr] = Rdata;
					Graphics.Image[Addr + 1] = Rdata2;
					break;
				case 6 :
					Rdata = Graphics.Image[Addr] & 0xc7;
					Rdata = Rdata | ((Color << 3) & 0x38);
					Graphics.Image[Addr] = Rdata;
					break;
				case 7 :
					Rdata = Graphics.Image[Addr] & 0xf8;
					Rdata = Rdata | (Color & 0x07);
					Graphics.Image[Addr] = Rdata;
					break;						
			}	
		}
}

/******************************************************************************
function: Clear the color of the picture
parameter:
    Color : Graphicsed colors
******************************************************************************/
void Graphics_Clear(uint16_t Color)
{
    if(Graphics.Scale == 2 || Graphics.Scale == 4) {
		for (uint16_t Y = 0; Y < Graphics.HeightByte; Y++) {
			for (uint16_t X = 0; X < Graphics.WidthByte; X++ ) {//8 pixel =  1 byte
				uint32_t Addr = X + Y*Graphics.WidthByte;
				Graphics.Image[Addr] = Color;
			}
		}
	}
	if(Graphics.Scale == 7) {
		Color = (uint8_t)Color;
		uint16_t Width = (Graphics.WidthMemory * 3 % 8 == 0)? (Graphics.WidthMemory * 3 / 8 ): (Graphics.WidthMemory * 3 / 8 + 1);
		for (uint16_t Y = 0; Y < Graphics.HeightByte; Y++) {
			for (uint16_t X = 0; X < Width; X++ ) {
				uint32_t Addr = X + Y * Width;
				if((X + Y * Width)%3 == 0) 
					Graphics.Image[Addr] = ((Color<<5) | (Color<<2) | (Color>>1));							
				else if((X + Y * Width)%3 == 1) 
					Graphics.Image[Addr] = ((Color<<7) | (Color<<4) | (Color<<1) | (Color>>2));				
				else if((X + Y * Width)%3 == 2)
					Graphics.Image[Addr] = ((Color<<6) | (Color<<3) |  Color);				
			}
		}
	}
}

/******************************************************************************
function: Clear the color of a window
parameter:
    Xstart : x starting point
    Ystart : Y starting point
    Xend   : x end point
    Yend   : y end point
    Color  : Graphicsed colors
******************************************************************************/
void Graphics_ClearWindows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t Color)
{
    uint16_t X, Y;
    for (Y = Ystart; Y < Yend; Y++) {
        for (X = Xstart; X < Xend; X++) {//8 pixel =  1 byte
            Graphics_SetPixel(X, Y, Color);
        }
    }
}

/******************************************************************************
function: Draw Point(Xpoint, Ypoint) Fill the color
parameter:
    Xpoint		: The Xpoint coordinate of the point
    Ypoint		: The Ypoint coordinate of the point
    Color		: Graphicsed color
    Dot_Pixel	: point size
    Dot_Style	: point Style
******************************************************************************/
void Graphics_DrawPoint(uint16_t Xpoint, uint16_t Ypoint, uint16_t Color,
                     DOT_PIXEL Dot_Pixel, DOT_STYLE Dot_Style)
{
    if (Xpoint > Graphics.Width || Ypoint > Graphics.Height) {
        printf("Graphics_DrawPoint Input exceeds the normal display range\r\n");
        return;
    }

    int16_t XDir_Num , YDir_Num;
    if (Dot_Style == DOT_FILL_AROUND) {
        for (XDir_Num = 0; XDir_Num < 2 * Dot_Pixel - 1; XDir_Num++) {
            for (YDir_Num = 0; YDir_Num < 2 * Dot_Pixel - 1; YDir_Num++) {
                if(Xpoint + XDir_Num - Dot_Pixel < 0 || Ypoint + YDir_Num - Dot_Pixel < 0)
                    break;
                // printf("x = %d, y = %d\r\n", Xpoint + XDir_Num - Dot_Pixel, Ypoint + YDir_Num - Dot_Pixel);
                Graphics_SetPixel(Xpoint + XDir_Num - Dot_Pixel, Ypoint + YDir_Num - Dot_Pixel, Color);
            }
        }
    } else {
        for (XDir_Num = 0; XDir_Num <  Dot_Pixel; XDir_Num++) {
            for (YDir_Num = 0; YDir_Num <  Dot_Pixel; YDir_Num++) {
                Graphics_SetPixel(Xpoint + XDir_Num - 1, Ypoint + YDir_Num - 1, Color);
            }
        }
    }
}

/******************************************************************************
function: Draw a line of arbitrary slope
parameter:
    Xstart ：Starting Xpoint point coordinates
    Ystart ：Starting Xpoint point coordinates
    Xend   ：End point Xpoint coordinate
    Yend   ：End point Ypoint coordinate
    Color  ：The color of the line segment
    Line_width : Line width
    Line_Style: Solid and dotted lines
******************************************************************************/
void Graphics_DrawLine(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend,
                    uint16_t Color, DOT_PIXEL Line_width, LINE_STYLE Line_Style)
{
    if (Xstart > Graphics.Width || Ystart > Graphics.Height ||
        Xend > Graphics.Width || Yend > Graphics.Height) {
        printf("Graphics_DrawLine Input exceeds the normal display range\r\n");
        return;
    }

    uint16_t Xpoint = Xstart;
    uint16_t Ypoint = Ystart;
    int dx = (int)Xend - (int)Xstart >= 0 ? Xend - Xstart : Xstart - Xend;
    int dy = (int)Yend - (int)Ystart <= 0 ? Yend - Ystart : Ystart - Yend;

    // Increment direction, 1 is positive, -1 is counter;
    int XAddway = Xstart < Xend ? 1 : -1;
    int YAddway = Ystart < Yend ? 1 : -1;

    //Cumulative error
    int Esp = dx + dy;
    char Dotted_Len = 0;

    for (;;) {
        Dotted_Len++;
        //Graphicsed dotted line, 2 point is really virtual
        if (Line_Style == LINE_STYLE_DOTTED && Dotted_Len % 3 == 0) {
            //printf("LINE_DOTTED\r\n");
            Graphics_DrawPoint(Xpoint, Ypoint, IMAGE_BACKGROUND, Line_width, DOT_STYLE_DFT);
            Dotted_Len = 0;
        } else {
            Graphics_DrawPoint(Xpoint, Ypoint, Color, Line_width, DOT_STYLE_DFT);
        }
        if (2 * Esp >= dy) {
            if (Xpoint == Xend)
                break;
            Esp += dy;
            Xpoint += XAddway;
        }
        if (2 * Esp <= dx) {
            if (Ypoint == Yend)
                break;
            Esp += dx;
            Ypoint += YAddway;
        }
    }
}

/******************************************************************************
function: Draw a rectangle
parameter:
    Xstart ：Rectangular  Starting Xpoint point coordinates
    Ystart ：Rectangular  Starting Xpoint point coordinates
    Xend   ：Rectangular  End point Xpoint coordinate
    Yend   ：Rectangular  End point Ypoint coordinate
    Color  ：The color of the Rectangular segment
    Line_width: Line width
    Draw_Fill : Whether to fill the inside of the rectangle
******************************************************************************/
void Graphics_DrawRectangle(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend,
                         uint16_t Color, DOT_PIXEL Line_width, DRAW_FILL Draw_Fill)
{
    if (Xstart > Graphics.Width || Ystart > Graphics.Height ||
        Xend > Graphics.Width || Yend > Graphics.Height) {
        printf("Input exceeds the normal display range\r\n");
        return;
    }

    if (Draw_Fill) {
        uint16_t Ypoint;
        for(Ypoint = Ystart; Ypoint < Yend; Ypoint++) {
            Graphics_DrawLine(Xstart, Ypoint, Xend, Ypoint, Color , Line_width, LINE_STYLE_SOLID);
        }
    } else {
        Graphics_DrawLine(Xstart, Ystart, Xend, Ystart, Color, Line_width, LINE_STYLE_SOLID);
        Graphics_DrawLine(Xstart, Ystart, Xstart, Yend, Color, Line_width, LINE_STYLE_SOLID);
        Graphics_DrawLine(Xend, Yend, Xend, Ystart, Color, Line_width, LINE_STYLE_SOLID);
        Graphics_DrawLine(Xend, Yend, Xstart, Yend, Color, Line_width, LINE_STYLE_SOLID);
    }
}

/******************************************************************************
function: Use the 8-point method to draw a circle of the
            specified size at the specified position->
parameter:
    X_Center  ：Center X coordinate
    Y_Center  ：Center Y coordinate
    Radius    ：circle Radius
    Color     ：The color of the ：circle segment
    Line_width: Line width
    Draw_Fill : Whether to fill the inside of the Circle
******************************************************************************/
void Graphics_DrawCircle(uint16_t X_Center, uint16_t Y_Center, uint16_t Radius,
                      uint16_t Color, DOT_PIXEL Line_width, DRAW_FILL Draw_Fill)
{
    if (X_Center > Graphics.Width || Y_Center >= Graphics.Height) {
        printf("Graphics_DrawCircle Input exceeds the normal display range\r\n");
        return;
    }

    //Draw a circle from(0, R) as a starting point
    int16_t XCurrent, YCurrent;
    XCurrent = 0;
    YCurrent = Radius;

    //Cumulative error,judge the next point of the logo
    int16_t Esp = 3 - (Radius << 1 );

    int16_t sCountY;
    if (Draw_Fill == DRAW_FILL_FULL) {
        while (XCurrent <= YCurrent ) { //Realistic circles
            for (sCountY = XCurrent; sCountY <= YCurrent; sCountY ++ ) {
                Graphics_DrawPoint(X_Center + XCurrent, Y_Center + sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//1
                Graphics_DrawPoint(X_Center - XCurrent, Y_Center + sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//2
                Graphics_DrawPoint(X_Center - sCountY, Y_Center + XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//3
                Graphics_DrawPoint(X_Center - sCountY, Y_Center - XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//4
                Graphics_DrawPoint(X_Center - XCurrent, Y_Center - sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//5
                Graphics_DrawPoint(X_Center + XCurrent, Y_Center - sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//6
                Graphics_DrawPoint(X_Center + sCountY, Y_Center - XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//7
                Graphics_DrawPoint(X_Center + sCountY, Y_Center + XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);
            }
            if (Esp < 0 )
                Esp += 4 * XCurrent + 6;
            else {
                Esp += 10 + 4 * (XCurrent - YCurrent );
                YCurrent --;
            }
            XCurrent ++;
        }
    } else { //Draw a hollow circle
        while (XCurrent <= YCurrent ) {
            Graphics_DrawPoint(X_Center + XCurrent, Y_Center + YCurrent, Color, Line_width, DOT_STYLE_DFT);//1
            Graphics_DrawPoint(X_Center - XCurrent, Y_Center + YCurrent, Color, Line_width, DOT_STYLE_DFT);//2
            Graphics_DrawPoint(X_Center - YCurrent, Y_Center + XCurrent, Color, Line_width, DOT_STYLE_DFT);//3
            Graphics_DrawPoint(X_Center - YCurrent, Y_Center - XCurrent, Color, Line_width, DOT_STYLE_DFT);//4
            Graphics_DrawPoint(X_Center - XCurrent, Y_Center - YCurrent, Color, Line_width, DOT_STYLE_DFT);//5
            Graphics_DrawPoint(X_Center + XCurrent, Y_Center - YCurrent, Color, Line_width, DOT_STYLE_DFT);//6
            Graphics_DrawPoint(X_Center + YCurrent, Y_Center - XCurrent, Color, Line_width, DOT_STYLE_DFT);//7
            Graphics_DrawPoint(X_Center + YCurrent, Y_Center + XCurrent, Color, Line_width, DOT_STYLE_DFT);//0

            if (Esp < 0 )
                Esp += 4 * XCurrent + 6;
            else {
                Esp += 10 + 4 * (XCurrent - YCurrent );
                YCurrent --;
            }
            XCurrent ++;
        }
    }
}

/******************************************************************************
function: Show English characters
parameter:
    Xpoint           ：X coordinate
    Ypoint           ：Y coordinate
    Acsii_Char       ：To display the English characters
    Font             ：A structure pointer that displays a character size
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
void Graphics_DrawChar(uint16_t Xpoint, uint16_t Ypoint, const char Acsii_Char,
                    sFONT* Font, uint16_t Color_Foreground, uint16_t Color_Background)
{
    uint16_t Page, Column;

    if (Xpoint > Graphics.Width || Ypoint > Graphics.Height) {
        printf("Graphics_DrawChar Input exceeds the normal display range\r\n");
        return;
    }

    uint32_t Char_Offset = (Acsii_Char - ' ') * Font->Height * (Font->Width / 8 + (Font->Width % 8 ? 1 : 0));
    const unsigned char *ptr = &Font->table[Char_Offset];

    for (Page = 0; Page < Font->Height; Page ++ ) {
        for (Column = 0; Column < Font->Width; Column ++ ) {

            //To determine whether the font background color and screen background color is consistent
            if (FONT_BACKGROUND == Color_Background) { //this process is to speed up the scan
                if (*ptr & (0x80 >> (Column % 8)))
                    Graphics_SetPixel(Xpoint + Column, Ypoint + Page, Color_Foreground);
                    // Graphics_DrawPoint(Xpoint + Column, Ypoint + Page, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
            } else {
                if (*ptr & (0x80 >> (Column % 8))) {
                    Graphics_SetPixel(Xpoint + Column, Ypoint + Page, Color_Foreground);
                    // Graphics_DrawPoint(Xpoint + Column, Ypoint + Page, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                } else {
                    Graphics_SetPixel(Xpoint + Column, Ypoint + Page, Color_Background);
                    // Graphics_DrawPoint(Xpoint + Column, Ypoint + Page, Color_Background, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                }
            }
            //One pixel is 8 bits
            if (Column % 8 == 7)
                ptr++;
        }// Write a line
        if (Font->Width % 8 != 0)
            ptr++;
    }// Write all
}

/******************************************************************************
function:	Display the string
parameter:
    Xstart           ：X coordinate
    Ystart           ：Y coordinate
    pString          ：The first address of the English string to be displayed
    Font             ：A structure pointer that displays a character size
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
void Graphics_DrawString(uint16_t Xstart, uint16_t Ystart, const char * pString,
                         sFONT* Font, uint16_t Color_Foreground, uint16_t Color_Background)
{
    uint16_t Xpoint = Xstart;
    uint16_t Ypoint = Ystart;

    if (Xstart > Graphics.Width || Ystart > Graphics.Height) {
        printf("Graphics_DrawString Input exceeds the normal display range\r\n");
        return;
    }

    while (* pString != '\0') {
        //if X direction filled , reposition to(Xstart,Ypoint),Ypoint is Y direction plus the Height of the character
        if ((Xpoint + Font->Width ) > Graphics.Width ) {
            Xpoint = Xstart;
            Ypoint += Font->Height;
        }

        // If the Y direction is full, reposition to(Xstart, Ystart)
        if ((Ypoint  + Font->Height ) > Graphics.Height ) {
            Xpoint = Xstart;
            Ypoint = Ystart;
        }
        Graphics_DrawChar(Xpoint, Ypoint, * pString, Font, Color_Background, Color_Foreground);

        //The next character of the address
        pString ++;

        //The next word of the abscissa increases the font of the broadband
        Xpoint += Font->Width;
    }
}


/******************************************************************************
function:	Display nummber
parameter:
    Xstart           ：X coordinate
    Ystart           : Y coordinate
    Nummber          : The number displayed
    Font             ：A structure pointer that displays a character size
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
#define  ARRAY_LEN 255
void Graphics_DrawNum(uint16_t Xpoint, uint16_t Ypoint, int32_t Nummber,
                   sFONT* Font, uint16_t Color_Foreground, uint16_t Color_Background)
{

    int16_t Num_Bit = 0, Str_Bit = 0;
    uint8_t Str_Array[ARRAY_LEN] = {0}, Num_Array[ARRAY_LEN] = {0};
    uint8_t *pStr = Str_Array;

    if (Xpoint > Graphics.Width || Ypoint > Graphics.Height) {
        printf("Graphics_DisNum Input exceeds the normal display range\r\n");
        return;
    }

    //Converts a number to a string
    while (Nummber) {
        Num_Array[Num_Bit] = Nummber % 10 + '0';
        Num_Bit++;
        Nummber /= 10;
    }

    //The string is inverted
    while (Num_Bit > 0) {
        Str_Array[Str_Bit] = Num_Array[Num_Bit - 1];
        Str_Bit ++;
        Num_Bit --;
    }

    //show
    Graphics_DrawString(Xpoint, Ypoint, (const char*)pStr, Font, Color_Background, Color_Foreground);
}

/******************************************************************************
function:	Display time
parameter:
    Xstart           ：X coordinate
    Ystart           : Y coordinate
    pTime            : Time-related structures
    Font             ：A structure pointer that displays a character size
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
void Graphics_DrawTime(uint16_t Xstart, uint16_t Ystart, GRAPHICS_TIME *pTime, sFONT* Font,
                    uint16_t Color_Foreground, uint16_t Color_Background)
{
    uint8_t value[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    uint16_t Dx = Font->Width;

    //Write data into the cache
    Graphics_DrawChar(Xstart                           , Ystart, value[pTime->Hour / 10], Font, Color_Background, Color_Foreground);
    Graphics_DrawChar(Xstart + Dx                      , Ystart, value[pTime->Hour % 10], Font, Color_Background, Color_Foreground);
    Graphics_DrawChar(Xstart + Dx  + Dx / 4 + Dx / 2   , Ystart, ':'                    , Font, Color_Background, Color_Foreground);
    Graphics_DrawChar(Xstart + Dx * 2 + Dx / 2         , Ystart, value[pTime->Min / 10] , Font, Color_Background, Color_Foreground);
    Graphics_DrawChar(Xstart + Dx * 3 + Dx / 2         , Ystart, value[pTime->Min % 10] , Font, Color_Background, Color_Foreground);
    Graphics_DrawChar(Xstart + Dx * 4 + Dx / 2 - Dx / 4, Ystart, ':'                    , Font, Color_Background, Color_Foreground);
    Graphics_DrawChar(Xstart + Dx * 5                  , Ystart, value[pTime->Sec / 10] , Font, Color_Background, Color_Foreground);
    Graphics_DrawChar(Xstart + Dx * 6                  , Ystart, value[pTime->Sec % 10] , Font, Color_Background, Color_Foreground);
}

/******************************************************************************
function:	Display monochrome bitmap
parameter:
    image_buffer ：A picture data converted to a bitmap
info:
    Use a computer to convert the image into a corresponding array,
    and then embed the array directly into Imagedata.cpp as a .c file.
******************************************************************************/
void Graphics_DrawBitMap(const unsigned char* image_buffer)
{
    uint16_t x, y;
    uint32_t Addr = 0;

    for (y = 0; y < Graphics.HeightByte; y++) {
        for (x = 0; x < Graphics.WidthByte; x++) {//8 pixel =  1 byte
            Addr = x + y * Graphics.WidthByte;
            Graphics.Image[Addr] = (unsigned char)image_buffer[Addr];
        }
    }
}

/******************************************************************************
function:	Display image
parameter:
    image            ：Image start address
    xStart           : X starting coordinates
    yStart           : Y starting coordinates
    xEnd             ：Image width
    yEnd             : Image height
******************************************************************************/
void Graphics_DrawImage(const unsigned char *image_buffer, uint16_t xStart, uint16_t yStart, uint16_t W_Image, uint16_t H_Image) 
{
    uint16_t x, y;
	uint16_t w_byte=(W_Image%8)?(W_Image/8)+1:W_Image/8;
    uint32_t Addr = 0;
	uint32_t pAddr = 0;
    for (y = 0; y < H_Image; y++) {
        for (x = 0; x < w_byte; x++) {//8 pixel =  1 byte
            Addr = x + y * w_byte;
			pAddr=x+(xStart/8)+((y+yStart)*Graphics.WidthByte);
            Graphics.Image[pAddr] = (unsigned char)image_buffer[Addr];
        }
    }
}
