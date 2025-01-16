/* EnkFi Demo Code to create Graphics like char, dot, line, circle, etc. */
#include "EnkFi_2in9.h"
#include "graphics.h"

const int buzzerPin = 15;    // Buzzer at GPIO15

// function to create note with buzzer
void beep(int note, int duration){  // need note frequency(Hz) and time duration (in millis)
  tone(buzzerPin, note, duration);
  delay(duration);
}

void setup()
{
  pinMode(buzzerPin, OUTPUT); // set Buzzer pin as OUTPUT
  Serial.begin(115200);       // Initialize serial communication 
  beep(1245,500); // call beep method to sound buzzer, pass (Frequency, duration)
  
  Serial.println("*** Display Image Demo ***");
  ENKFI_Config();
  Serial.println("Initialize and clear E-paper Display");
  ENKFI_Init();
  ENKFI_Clear();
  delay(500);

  //Create a new image cache and fill it with white
  uint8_t *BlackImage, *RedImage;
  uint16_t Imagesize = ((ENKFI_WIDTH % 8 == 0)? (ENKFI_WIDTH / 8 ): (ENKFI_WIDTH / 8 + 1)) * ENKFI_HEIGHT;
  if((BlackImage = (uint8_t *)malloc(Imagesize)) == NULL) {
      Serial.println("Failed to apply for black memory...");
      while(1);
  }
  if((RedImage = (uint8_t *)malloc(Imagesize)) == NULL) {
      Serial.println("Failed to apply for red memory...");
      while(1);
  }

  Graphics_NewImage(BlackImage, ENKFI_WIDTH, ENKFI_HEIGHT, 270, WHITE);
  Graphics_NewImage(RedImage, ENKFI_WIDTH, ENKFI_HEIGHT, 270, WHITE);

  beep(1245,500);
  
  Serial.println("/********** Draw BLACK ***********/");
  //Select Image
  Graphics_SelectImage(BlackImage);
  Graphics_Clear(WHITE);

  //Drawing on the image
  Serial.println("Drawing BlackImage..!");
  Graphics_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
  Graphics_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
  Graphics_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
  Graphics_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
  Graphics_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
  Graphics_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
  Graphics_DrawString(10, 0, "Hello world", &Font16, BLACK, WHITE);
  Graphics_DrawNum(10, 40, 123123123, &Font12, BLACK, WHITE);
  ENKFI_DisplayBLK(BlackImage);
  delay(2000);

  Serial.println("/********** Draw RED ***********/");
  //Select Image
  Graphics_SelectImage(RedImage);
  Graphics_Clear(WHITE);

  //Drawing on the image
  Serial.println("Drawing RedImage..!");
  Graphics_DrawCircle(200, 80, 40, RED, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
  Graphics_DrawCircle(200, 80, 20, RED, DOT_PIXEL_1X1, DRAW_FILL_FULL);
  Graphics_DrawString(10, 20, "SBComponents", &Font16, RED, WHITE);
  Graphics_DrawNum(10, 50, 123123123, &Font16, RED, WHITE);
  ENKFI_DisplayRED(RedImage);
  delay(2000);

  Serial.println("Goto Sleep...");
  ENKFI_Sleep();
  free(BlackImage);  // free memory
  BlackImage = NULL;
  free(RedImage);   
  RedImage = NULL;

}


void loop()
{ 
  delay(50);
}
