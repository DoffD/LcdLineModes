#ifndef KP_KEYPAD_H
#define KP_KEYPAD_H
#include <Keypad.h>

static const char kp_KEYS[4][4]
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
};

// uint8_t kp_ROW[4] = {A15, A14, A13, A12};
// uint8_t kp_COL[4] = {A11, A10, A9, A8};

uint8_t kp_ROW[4] = {A7, A6, A5, A4};
uint8_t kp_COL[4] = {A3, A2, A1, A0};

Keypad kp_kp = Keypad( makeKeymap(kp_KEYS), kp_ROW, kp_COL, 4, 4);

#endif