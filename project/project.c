#include <project.h>
#include <math.h>

// Interfacing PIC16F877A with SSD1306 OLED display (128x64 Pixel)
// http://simple-circuit.com/

#define SSD1306_RST   PIN_D4

#fuses HS,NOWDT,NOPROTECT,NOLVP
#use I2C(MASTER, I2C1, FAST = 400000, stream = SSD1306_STREAM)  // Initialize I2C

#byte porta = 0x05

// Include SSD1306 OLED driver source code
#include <oled12864.c>
#bit trig = porta.0
#bit echo = porta.1




int16 loop0 = 0;
int16 loop1 = 0;
int scantime = FALSE;
int displaytime = 0;

int8 angle = 12;

int clear = 0;

int16 duration = 0;
int8 distance = 0;

int measure = 0;

signed int rad = 0;
float radf = 0;
unsigned int8 posx = 0, posy = 0;
unsigned int8 oledx = 0, oledy = 0;

int leftside = 0;



#INT_TIMER0                   //0.05ms timer interrupt
void timer0_ovf()
{
  set_timer0(0x06);
  if(loop0 == 0)
  {
   output_high(PIN_E0);
  } else if(loop0 == angle)
  {
   output_low(PIN_E0);
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
   if (scantime == 1 && angle < 42) {
      angle++;
      if (angle == 42) {
         scantime = 0;
      }
   } else if (scantime == 0) {
      angle = 12;
      displaytime++;
      if (displaytime == 30) {
         scantime = 1;
         displaytime = 0;
         clear = 1;
      }
   }
   loop1 = 0;
  }
  
  if(measure == 1)
  {
   duration++;
  }
}


void main() {
  set_tris_a(0x02);
  
  
  enable_interrupts(INT_TIMER0);
  enable_interrupts(global);
  
  
  delay_ms(500);
  
  // Initialize the SSD1306 OLED with an I2C addr = 0x7A (default address)
  SSD1306_Init(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
  
  // clear the display
  SSD1306_ClearDisplay();

  SSD1306_GotoXY(1, 1);
  SSD1306_PutC("Minimal Radar Display");
  delay_ms(5000);
  
  
  set_timer0(0x06);
  setup_timer_0(T0_INTERNAL|T0_DIV_1);
  
  SSD1306_ClearDisplay();

  while(TRUE) {
    
    if(clear == 1)
    {
      SSD1306_ClearDisplay();
      clear = 0;
    }
    
    if(scantime == TRUE) {
       output_high(PIN_A0);
       delay_us(10);
       output_low(PIN_A0);
       
       while(!input(PIN_A1));
       duration = 0;
       distance = 0;
       measure = 1;
       while(input(PIN_A1));
       measure = 0;
       
       distance = (duration * 50)/58;     //distance (cm)
       
       if(distance <= 30)                 //0~30cm
       {
         
         
         rad = (angle - 27)*6;
         if(rad < 0)
         {
            rad *= -1;
            leftside = 0;
         } else {
            leftside = 1;
         }
         
         radf = pi / 180 * rad;
         
         posx = sin(radf) * distance / 2; //0~15cm
         posy = cos(radf) * distance / 2;
         
         if(leftside == 1) {
            oledx = 17 - posx;            //2~32
         } else {
            oledx = 17 + posx;
         }
         oledy = 16 - posy;               //1~16
         
         
         
         SSD1306_GotoXY(1, 1);
         printf(SSD1306_PutC, "%02u", distance);
       
         SSD1306_GotoXY(1, 2);
         SSD1306_PutC("(cm)");
         
         SSD1306_DRAWDOT(oledx, oledy);
         
       } else {
         distance = 0;
       }
    } else {
      output_low(PIN_A0);
    }
    
    SSD1306_UPDATEDISPLAY();
  }
}
// End of code
