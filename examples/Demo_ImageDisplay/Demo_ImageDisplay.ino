/* EnkFi Demo Code to Display Image */

#include "EnkFi_2in9.h"
#include "graphics.h"
#include "imgData.h"  

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
  // show image   
  Graphics_SelectImage(BlackImage);
  Graphics_DrawBitMap(enkfi_black);
  Graphics_SelectImage(RedImage);
  Graphics_DrawBitMap(enkfi_red);
  ENKFI_Display(BlackImage, RedImage);

  delay(3000);
  ENKFI_Clear();
  Graphics_SelectImage(RedImage);
  Graphics_DrawBitMap(BIGOffer_IMG);
  ENKFI_Display_Red(RedImage);

  delay(3000);
  ENKFI_Clear();
  Graphics_SelectImage(BlackImage);
  Graphics_DrawBitMap(HOFFER_IMG);
  ENKFI_Display_Black(BlackImage);
  
  delay(500);
  
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
