#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "kp_keypad.h"

#define DBG_ALL
#ifdef DBG_ALL
#define DBG_PORT Serial
#define DBG_BAUD 9600
#endif

#define LCD_LEN_LINE 20
#define LCD_POS_LINEA 0

LiquidCrystal_I2C lcd_lcd(0x3F,20,4);
// LiquidCrystal_I2C lcd_lcd(0x27,20,4);

void setup() {
  // put your setup code here, to run once:
  #ifdef DBG_ALL
  DBG_PORT.begin(DBG_BAUD);
  DBG_PORT.println(F("Lcd Line Modes Test!"));
  #endif
  // LCD init start
  lcd_lcd.init();
  lcd_lcd.backlight();
  lcd_lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  // Global vars dec start =====
  static bool gIsInitDone{true};
  static uint32_t gCtr{};

  enum class LcdLineMode:uint8_t{
    deflt,
    yield,
  };
  enum class LcdCmd:uint8_t{
    none,
    flash
  };
  typedef struct {
    LcdCmd cmd;
    char inputBfr[LCD_LEN_LINE+1];
    uint32_t dura;
  } LcdCmdPrms;
  static LcdLineMode gLcdLineAMode{};
  LcdCmdPrms gLcdLineACmdPrms{};
  // Global vars dec end   =====

  // Keypad raw start ==================================================
  char kp_key{};
  kp_key = kp_kp.getKey();
  if(kp_key && gIsInitDone){
    // bzr_beep_cnt(1, 100);
    #ifdef DBG_KP_RAW
    DBG_PORT.print(F("Keypad key: ")); DBG_PORT.println(kp_key);
    #endif
  }
  // Keypad raw end   ==================================================
  // UI start =====
  static uint8_t ui_sm_st{};
  switch (ui_sm_st)
  {
  case 0:
    if(gIsInitDone){
      ui_sm_st = 1;
    }
    break;
  case 1:
  {
    if(kp_key >= '0' && kp_key <= '9'){
      static uint8_t ctr{};
      static char inputBfr[15+1]{};
      char lineBfr[LCD_LEN_LINE+1]{};
      if(ctr < 15){
        inputBfr[ctr] = kp_key;
        sprintf(lineBfr, "key: %-15s", inputBfr); // Left-aligned, occupying 15 characters.
        lcd_lcd.setCursor(0, LCD_POS_LINEA); lcd_lcd.print(lineBfr);
        gLcdLineAMode = LcdLineMode::yield;
        ctr++;
      }
      else{
        memset(inputBfr, 0, sizeof(inputBfr));
        ctr = 0;
        ////////////////////////////////////////////////////01234567890123456789
        lcd_lcd.setCursor(0, LCD_POS_LINEA); lcd_lcd.print("key:                ");
      }
    }
  }
    break;
  default:
    break;
  }
  // UI end   =====

  // TBI: Main process generates a flash message here.
  if(DBG_PORT.available()){
    uint8_t r = DBG_PORT.read();
    if(r == 'a'){
      gLcdLineAMode = LcdLineMode::deflt;
      gLcdLineACmdPrms.cmd = LcdCmd::flash;
      ////////////////////////////////////01234567890123456789
      sprintf(gLcdLineACmdPrms.inputBfr, "Flashing a message! ");
      gLcdLineACmdPrms.dura = 3000;
    }
    else if(r == 'b'){
      gLcdLineAMode = LcdLineMode::deflt;
    }
    DBG_PORT.print(F("DBG rcvd char: ")); DBG_PORT.println((char)r);
  }

  static uint32_t gMaint_ctr_tRef{};
  if(millis() - gMaint_ctr_tRef > 0){
    gCtr++;
    gMaint_ctr_tRef = millis();
  }

  static uint32_t lcdA_sm_tRef{};
  static uint32_t lcdA_sm_tSpanDura{};
  static uint8_t lcdA_sm_st{};
  switch (lcdA_sm_st)
  {
  case 0:
    if(gIsInitDone){
      lcdA_sm_st = 1;
    }
    break;
  case 1:
    if(gLcdLineAMode == LcdLineMode::deflt){
      if(gLcdLineACmdPrms.cmd == LcdCmd::none){
        // Process own lines here.
        static uint32_t ctr{};
        if(ctr != gCtr){
          ctr = gCtr;
          char lineBfr[LCD_LEN_LINE+1]{};
          // char inputBfr[10+1]{};
          sprintf(lineBfr,"ctr: %-10lu     ", ctr);
          lcd_lcd.setCursor(0, LCD_POS_LINEA); lcd_lcd.print(lineBfr);
        }
      }
      else if(gLcdLineACmdPrms.cmd == LcdCmd::flash){
        lcd_lcd.setCursor(0, LCD_POS_LINEA); lcd_lcd.print(gLcdLineACmdPrms.inputBfr);
        lcdA_sm_tSpanDura = gLcdLineACmdPrms.dura;
        lcdA_sm_tRef = millis();
        lcdA_sm_st = 2; 
      }
    }
    break;
  case 2:
    if(millis() - lcdA_sm_tRef > lcdA_sm_tSpanDura){
      ////////////////////////////////////////////////////01234567890123456789
      lcd_lcd.setCursor(0, LCD_POS_LINEA); lcd_lcd.print("Press key to resume ");
      lcdA_sm_st = 1;
    }
    break;
  default:
    break;
  }

}
