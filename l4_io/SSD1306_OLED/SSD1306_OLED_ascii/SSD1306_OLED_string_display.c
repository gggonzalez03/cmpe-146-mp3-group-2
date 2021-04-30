#include "SSD1306_OLED_string_display.h"
#include "SSD1306_OLED_ascii.h"
#include <stdio.h>

void SSD1306_ascii_display_string(SSD1306_OLED_string string) {
  for (int i = 0; i < string.string_size; i++) {
    SSD1306_ascii_display_char(string.string_content[i]);
    SSD1306_ascii_display_space();
  }
}

void SSD1306_ascii_display_char(char character) {
  switch (character) {
  case 'A':
  case 'a':
    SSD1306_ascii_display_A();
    break;

  case 'B':
  case 'b':
    SSD1306_ascii_display_B();
    break;

  case 'C':
  case 'c':
    SSD1306_ascii_display_C();
    break;

  case 'D':
  case 'd':
    SSD1306_ascii_display_D();
    break;

  case 'E':
  case 'e':
    SSD1306_ascii_display_E();
    break;

  case 'F':
  case 'f':
    SSD1306_ascii_display_F();
    break;

  case 'G':
  case 'g':
    SSD1306_ascii_display_G();
    break;

  case 'H':
  case 'h':
    SSD1306_ascii_display_H();
    break;

  case 'I':
  case 'i':
    SSD1306_ascii_display_I();
    break;

  case 'J':
  case 'j':
    SSD1306_ascii_display_J();
    break;

  case 'K':
  case 'k':
    SSD1306_ascii_display_K();
    break;

  case 'L':
  case 'l':
    SSD1306_ascii_display_L();
    break;

  case 'M':
  case 'm':
    SSD1306_ascii_display_M();
    break;

  case 'N':
  case 'n':
    SSD1306_ascii_display_N();
    break;

  case 'O':
  case 'o':
    SSD1306_ascii_display_O();
    break;

  case 'P':
  case 'p':
    SSD1306_ascii_display_P();
    break;

  case 'Q':
  case 'q':
    SSD1306_ascii_display_Q();
    break;

  case 'R':
  case 'r':
    SSD1306_ascii_display_R();
    break;

  case 'S':
  case 's':
    SSD1306_ascii_display_S();
    break;

  case 'T':
  case 't':
    SSD1306_ascii_display_T();
    break;

  case 'U':
  case 'u':
    SSD1306_ascii_display_U();
    break;

  case 'V':
  case 'v':
    SSD1306_ascii_display_V();
    break;

  case 'W':
  case 'w':
    SSD1306_ascii_display_W();
    break;

  case 'X':
  case 'x':
    SSD1306_ascii_display_X();
    break;

  case 'Y':
  case 'y':
    SSD1306_ascii_display_Y();
    break;

  case 'Z':
  case 'z':
    SSD1306_ascii_display_Z();
    break;

  case '0':
    SSD1306_ascii_display_0();
    break;

  case '1':
    SSD1306_ascii_display_1();
    break;

  case '2':
    SSD1306_ascii_display_2();
    break;

  case '3':
    SSD1306_ascii_display_3();
    break;

  case '4':
    SSD1306_ascii_display_4();
    break;

  case '5':
    SSD1306_ascii_display_5();
    break;

  case '6':
    SSD1306_ascii_display_6();
    break;

  case '7':
    SSD1306_ascii_display_7();
    break;

  case '8':
    SSD1306_ascii_display_8();
    break;

  case '9':
    SSD1306_ascii_display_9();
    break;

  case ' ':
    SSD1306_ascii_display_space();
    break;

  default:
    SSD1306_ascii_display_space();
    break;
  }
}