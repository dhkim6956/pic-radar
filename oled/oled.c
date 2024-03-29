// Interfacing PIC16F877A with SSD1306 OLED display (128x64 Pixel)
// http://simple-circuit.com/

#define SSD1306_RST   PIN_D4

#include <16F877A.h>
#fuses HS,NOWDT,NOPROTECT,NOLVP                       
#use delay(clock = 20MHz)
#use I2C(MASTER, I2C1, FAST = 400000, stream = SSD1306_STREAM)  // Initialize I2C

// Include SSD1306 OLED driver source code
#include <SSD1306.c>    

int8 i = 0;

void main() {
  
  delay_ms(500);
  
  // Initialize the SSD1306 OLED with an I2C addr = 0x7A (default address)
  SSD1306_Init(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
  
  // clear the display
  SSD1306_ClearDisplay();

  SSD1306_GotoXY(1, 1);
  SSD1306_PutC("Interfacing PIC16F877A with SSD1306 OLED display");
  delay_ms(5000);

  SSD1306_ClearDisplay();

  SSD1306_GotoXY(6, 2);
  SSD1306_PutC("Hello world!");
  delay_ms(2000);

  SSD1306_StartScrollRight(1, 1);
  delay_ms(3000);
  SSD1306_StopScroll();
  
  SSD1306_StartScrollLeft(1, 1);
  delay_ms(3000);
  SSD1306_StopScroll();

  SSD1306_StartScrollDiagRight(1, 1);
  delay_ms(3000);
  SSD1306_StopScroll();

  SSD1306_StartScrollDiagLeft(1, 1);
  delay_ms(3000);
  SSD1306_StopScroll();

  delay_ms(3000);
  SSD1306_ClearDisplay();

  SSD1306_GotoXY(6, 2);
  SSD1306_PutC("Hello world!");
  delay_ms(2000);

  while(TRUE) {
    SSD1306_GotoXY(10, 5);
    printf(SSD1306_PutC, "%03u", i++);
    delay_ms(500);
  }

}
// End of code
