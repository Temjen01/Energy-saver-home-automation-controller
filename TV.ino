#include <Wire.h>
#include "SSD1306Wire.h"

SSD1306Wire display(0x3c, SDA, SCL);



void drawLines() {
  for (int16_t i = 0; i < display.getWidth(); i += 4) {
    display.drawLine(0, 0, i, display.getHeight() - 1);
    display.display(); delay(10);
  }
  for (int16_t i = 0; i < display.getHeight(); i += 4) {
    display.drawLine(0, 0, display.getWidth() - 1, i);
    check_status();
    display.display(); delay(10);
  }
  delay(1000);
  display.clear();
}

void drawRect() {
  for (int16_t i = 0; i < display.getHeight() / 2; i += 2) {
    display.drawRect(i, i, display.getWidth() - 2 * i, display.getHeight() - 2 * i);
    check_status();
    display.display(); delay(10);
  }
  delay(1000); 
  display.clear();
}

void fillRect() {
  uint8_t color = 1;
  for (int16_t i = 0; i < display.getHeight() / 2; i += 3) {
    display.setColor((color % 2 == 0) ? BLACK : WHITE);
    display.fillRect(i, i, display.getWidth() - i * 2, display.getHeight() - i * 2);
    check_status();
    display.display(); delay(10);
    color++;
  }
  display.setColor(WHITE);
  delay(1000); 
  display.clear();
}

void drawCircle() {
  for (int16_t i = 0; i < display.getHeight(); i += 2) {
    display.drawCircle(display.getWidth() / 2, display.getHeight() / 2, i);
    check_status();
    display.display(); delay(10);
  }
  delay(1000); 
  display.clear();
}

void displayText(const char* line1, const char* line2) {
  // Display "GHSS" on the top line, smaller size
  display.setFont(ArialMT_Plain_16); // Smaller font size
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(display.getWidth() / 2, 10, line1); // Adjust Y position as needed
  
  // Display "PURANA BAZAR" below "GHSS", bold and bigger size
  display.setFont(ArialMT_Plain_16); // Bigger font size
  display.drawString(display.getWidth() / 2, 30, line2); // Adjust Y position as needed
  check_status();
  display.display();
  delay(1000); // Display the text for 2 seconds before running other functions 
  display.clear();
}

void check_status(){
  if (digitalRead(12) == 0)
    display.displayOff();
  if (digitalRead(12) == 1)
    display.displayOn();
}

void setup() {
  Serial.begin(115200);
  pinMode(12, INPUT);
  display.init();
  display.setContrast(255);
  display.setBrightness(255);
}

void loop() {
  while (digitalRead(12) == 0) 
  {
    display.displayOff();
    Serial.print("=========== ITS OFF\n");
  }
  while (digitalRead(12) == 1) 
  {
    Serial.print("=========== ITS ON\n");
    display.displayOn();
    check_status();
    displayText("GHSS", "PURANA BAZAR");
    check_status();
    drawLines(); 
    check_status();
    displayText("GHSS", "PURANA BAZAR");
    check_status();
    drawRect();
    check_status();
    displayText("GHSS", "PURANA BAZAR");
    check_status();
    drawLines();
    check_status();
    displayText("GHSS", "PURANA BAZAR");
    check_status();
    fillRect();
    check_status();
    displayText("GHSS", "PURANA BAZAR");
    check_status();
    drawCircle();
    check_status();
  }
}
