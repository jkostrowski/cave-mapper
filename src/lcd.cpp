#include "lcd.h"

char l1[100] = "";
char l1r[50] = "";
char l2[100] = "";
char l3[100] = "";
char l4[100] = "";

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R2, RST, SCL1, SDA1); // screen on heltec
// U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE,  SCL, SDA); - mini external screen 

void lcdInitialize(void) {
    u8g2.begin();   
    u8g2.clearBuffer();					        
    u8g2.setFont(u8g2_font_fub14_tr);	
    u8g2.setFontPosTop();
    u8g2.drawStr(10,10, "Calibrate!");	
    u8g2.sendBuffer();					        
    u8g2.setFont(u8g2_font_fub11_tr);	
}

void lcd1(char * msg) {
  strcpy( l1, msg );
}

void lcd1r(char * msg) {
  strcpy( l1r, msg );
}

void lcd2(char * msg) {
  strcpy( l2, msg );
}

void lcd3(char * msg) {
  strcpy( l3, msg );
}

void lcd4(char * msg) {
  strcpy( l4, msg );
}

void lcdUpdate() {
  TRACE("lcdUpdate start");
  u8g2.clearBuffer();
  u8g2.drawStr( 0,  0, l1);	
  u8g2.drawStr(80,  0, l1r);	
  u8g2.drawStr( 0, 15, l2);	
  u8g2.drawStr( 0, 30, l3);	
  u8g2.drawStr( 0, 45, l4);	
  u8g2.sendBuffer();					        
  TRACE("lcdUpdate done");
}

