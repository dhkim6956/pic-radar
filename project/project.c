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


int8 loop0 = 0;
int16 loop1 = 0;
int mod1 = 1;

int8 angle = 15;

int test = 0;


int measure = 0;     //0 : before send signal, 1 : after send signal, 2 : wait for response
int16 responseloop = 0;
int responsedelay = 0;
int distance = 0;    //

#int_RTCC             //0.1ms timer interrupt
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
  
  
  if(loop0 < 200)
  {
   loop0++;
  } else {
   loop0 = 0;
  }
  
  
  
  if(loop1 < 5000)
  {
   loop1++;
  } else {
   if (mod1 == 1 && angle < 20) {
      angle++;
   } else if (mod1 == 0 && angle > 10) {
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


#int_timer2             //0.01ms timer interrupt
void timer2()
{
   if(measure == 0) {
      trig = 1;
      measure = 1;
   } else if (measure == 1) {
      trig = 0;
      measure = 2;
      responsedelay = 0;
   } else {
      if(echo != 0) {
         distance = responsedelay;
         measure = 0;
      } else {
         responsedelay++;
      }
   }
   
   
   
   if(responseloop < 10000)
   {
      responseloop++;
   } else {
      responseloop = 0;
      if(test == 0) {
         output_high(PIN_B0);
         test = 1;
      } else {
         output_low(PIN_B0);
         test = 0;
      }
   }
}

void main() {
  
  enable_interrupts(int_RTCC);
  enable_interrupts(int_timer2);
  enable_interrupts(global);
  
  set_tris_b(0x02);
  
  set_timer0(0x83);
  set_timer2(0xCE);
  setup_timer_0 (RTCC_internal|RTCC_div_4);
  
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
    printf(SSD1306_PutC, "%03u", responsedelay);

  }

}
// End of code
