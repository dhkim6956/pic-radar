#include <project.h>

// Interfacing PIC16F877A with SSD1306 OLED display (128x64 Pixel)
// http://simple-circuit.com/

#define SSD1306_RST   PIN_D4

#fuses HS,NOWDT,NOPROTECT,NOLVP
#use I2C(MASTER, I2C1, FAST = 400000, stream = SSD1306_STREAM)  // Initialize I2C

#byte porta = 0x05

// Include SSD1306 OLED driver source code
#include <SSD1306.c>
#bit trig = porta.0
#bit echo = porta.1


int16 loop0 = 0;
int16 loop1 = 0;
int mod1 = 1;

int8 angle = 30;

int test = 0;


#int_RTCC             //0.05ms timer interrupt
void rtcc()
{
  set_timer0(0x83);
  if(loop0 == 0)
  {
   output_e(0x07);
  } else if(loop0 == angle)
  {
   output_e(0);
  } 
  
  
  if(loop0 < 400)
  {
   loop0++;
  } else {
   loop0 = 0;
  }
  
  
  
  if(loop1 < 5000)
  {
   loop1++;
  } else {
   if (mod1 == 1 && angle < 42) {
      angle++;
   } else if (mod1 == 0 && angle > 8) {
      angle--;
   } else {
      if(mod1 == 0)
      {
         mod1 = 1;
      } else {
         mod1 = 0;
      }
   }
   loop1 = 0;
  }
}


void main() {
  
  enable_interrupts(int_RTCC);
  enable_interrupts(global);
  
  set_timer0(0x83);
  setup_timer_0 (RTCC_internal|RTCC_div_2);
  
  delay_ms(500);
  
  // Initialize the SSD1306 OLED with an I2C addr = 0x7A (default address)
  SSD1306_Init(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
  
  // clear the display
  SSD1306_ClearDisplay();

  SSD1306_GotoXY(1, 1);
  SSD1306_PutC("Minimal Radar Display");
  delay_ms(5000);
  
  
  SSD1306_ClearDisplay();

  SSD1306_GotoXY(6, 2);
  SSD1306_PutC("Hello world!");
  delay_ms(2000);

  while(TRUE) {
    SSD1306_GotoXY(10, 5);
    printf(SSD1306_PutC, "%03u", angle);

  }

}
// End of code
