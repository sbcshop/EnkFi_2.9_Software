// Demo code to test onboard Buzzer and Button working 
const int buzzerPin = 15;    // buzzer at GPIO15
const int bootButton = 0;   // Boot button 
const int userButton1 = 4;  // programmable button 1 at GPIO4
const int userButton2 = 6;  // programmable button 2 at GPIO6
const int userButton3 = 5;  // programmable button 3 at GPIO5
const int userButton4 = 7;  // programmable button 4 at GPIO7

void beep(int note, int duration){  // need note frequency(Hz) and time duration (in millis)
  tone(buzzerPin, note, duration);
  delay(duration);
}


void setup() {
  pinMode(bootButton, INPUT_PULLUP);  // set boot Button pin to INPUT PULLUP
  pinMode(userButton1, INPUT_PULLUP); 
  pinMode(userButton2, INPUT_PULLUP); 
  pinMode(userButton3, INPUT_PULLUP);  
  pinMode(userButton4, INPUT_PULLUP); 
  pinMode(buzzerPin, OUTPUT); // set Buzzer pin as OUTPUT
 
  Serial.begin(115200);
  beep(1245,500); // call beep method to sound buzzer, pass (Frequency, duration)
  Serial.println("Press Buttons to Test!");
}

void loop() {
  
  if (digitalRead(bootButton)==0){      // Check if bootButton Pressed or NOT, LOW when Pressed else HIGH
    Serial.println("Boot Button Pressed!");
    beep(1245,500); // call beep method to sound buzzer, pass (Frequency, duration)
    delay(50);
  }

  if (digitalRead(userButton1)==0){     // Check if userButton1 Pressed or NOT, LOW when Pressed else HIGH
    Serial.println("Button 1 Pressed!");
    beep(1245,500);
    delay(50);
  }

  if (digitalRead(userButton2)==0){     // Check if userButton2 Pressed or NOT, LOW when Pressed else HIGH
    Serial.println("Button 2 Pressed!");
    beep(1245,500);
    delay(50);
  }

  if (digitalRead(userButton3)==0){     // Check if userButton3 Pressed or NOT, LOW when Pressed else HIGH
    Serial.println("Button 3 Pressed!");
    beep(1245,500);
    delay(50);
  }

  if (digitalRead(userButton4)==0){     // Check if userButton4 Pressed or NOT, LOW when Pressed else HIGH
    Serial.println("Button 4 Pressed!");
    beep(1245,500);
    delay(50);
  }
  
  delay(50);
}
