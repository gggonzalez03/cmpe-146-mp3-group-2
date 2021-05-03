#include <stddef.h>
#include <stdint.h>

#include "SSD1306_OLED.h"
#include "SSD1306_OLED_ascii.h"

static uint8_t ascii_A_size = 5;
static uint8_t ascii_A[5] = {0x7E, 0x13, 0x13, 0x7F, 0x7E};

static uint8_t ascii_B_size = 6;
static uint8_t ascii_B[6] = {0x7F, 0x7F, 0x49, 0x49, 0x7F, 0x36};

static uint8_t ascii_C_size = 6;
static uint8_t ascii_C[6] = {0x1C, 0x3E, 0x63, 0x41, 0x41, 0x41};

static uint8_t ascii_D_size = 7;
static uint8_t ascii_D[7] = {0x7F, 0x7F, 0x41, 0x41, 0x63, 0x3E, 0x1C};

static uint8_t ascii_E_size = 5;
static uint8_t ascii_E[5] = {0x7F, 0x7F, 0x45, 0x45, 0x41};

static uint8_t ascii_F_size = 5;
static uint8_t ascii_F[5] = {0x7F, 0x7F, 0x05, 0x05, 0x01};

static uint8_t ascii_G_size = 7;
static uint8_t ascii_G[7] = {0x1C, 0x3E, 0x63, 0x41, 0x49, 0x79, 0x70};

static uint8_t ascii_H_size = 7;
static uint8_t ascii_H[7] = {0x7F, 0x7F, 0x08, 0x08, 0x08, 0x7F, 0x7F};

static uint8_t ascii_I_size = 2;
static uint8_t ascii_I[2] = {0x7F, 0x7F};

static uint8_t ascii_J_size = 4;
static uint8_t ascii_J[4] = {0x40, 0x40, 0x7F, 0x3F};

static uint8_t ascii_K_size = 6;
static uint8_t ascii_K[6] = {0x7F, 0x7F, 0x1C, 0x36, 0x63, 0x41};

static uint8_t ascii_L_size = 5;
static uint8_t ascii_L[5] = {0x7F, 0x7F, 0x40, 0x40, 0x40};

static uint8_t ascii_M_size = 6;
static uint8_t ascii_M[6] = {0x7F, 0x06, 0x0C, 0x06, 0x7F, 0x7F};

static uint8_t ascii_N_size = 5;
static uint8_t ascii_N[5] = {0x7F, 0x0F, 0x1C, 0x38, 0x7F};

static uint8_t ascii_O_size = 8;
static uint8_t ascii_O[8] = {0x1C, 0x3E, 0x63, 0x41, 0x41, 0x63, 0x3E, 0x1C};

static uint8_t ascii_P_size = 6;
static uint8_t ascii_P[6] = {0x7F, 0x7F, 0x09, 0x09, 0x0F, 0x06};

static uint8_t ascii_Q_size = 8;
static uint8_t ascii_Q[8] = {0x1C, 0x3E, 0x63, 0x41, 0x41, 0xE3, 0xBE, 0x9C};

static uint8_t ascii_R_size = 7;
static uint8_t ascii_R[7] = {0x7F, 0x7F, 0x09, 0x19, 0x7F, 0x66, 0x40};

static uint8_t ascii_S_size = 5;
static uint8_t ascii_S[5] = {0x46, 0x4F, 0x5C, 0x79, 0x61};

static uint8_t ascii_T_size = 6;
static uint8_t ascii_T[6] = {0x01, 0x01, 0x7F, 0x7F, 0x01, 0x01};

static uint8_t ascii_U_size = 6;
static uint8_t ascii_U[6] = {0x3F, 0x7F, 0x60, 0x40, 0x60, 0x3F};

static uint8_t ascii_V_size = 7;
static uint8_t ascii_V[7] = {0x01, 0x1F, 0x7C, 0x60, 0x78, 0x0F, 0x01};

static uint8_t ascii_W_size = 10;
static uint8_t ascii_W[10] = {0x01, 0x1F, 0x7C, 0x70, 0x1F, 0x1F, 0x70, 0x7C, 0x1F, 0x01};

static uint8_t ascii_X_size = 6;
static uint8_t ascii_X[6] = {0x63, 0x37, 0x1E, 0x3C, 0x76, 0x63};

static uint8_t ascii_Y_size = 6;
static uint8_t ascii_Y[6] = {0x07, 0x0F, 0x78, 0x78, 0x0F, 0x07};

static uint8_t ascii_Z_size = 6;
static uint8_t ascii_Z[6] = {0x71, 0x79, 0x5D, 0x4F, 0x47, 0x43};

static uint8_t ascii_0_size = 6;
static uint8_t ascii_0[6] = {0x3E, 0x7F, 0x41, 0x41, 0x7F, 0x3E};

static uint8_t ascii_1_size = 3;
static uint8_t ascii_1[3] = {0x02, 0x7F, 0x7F};

static uint8_t ascii_2_size = 6;
static uint8_t ascii_2[6] = {0x62, 0x73, 0x59, 0x49, 0x4F, 0x46};

static uint8_t ascii_3_size = 6;
static uint8_t ascii_3[6] = {0x22, 0x63, 0x49, 0x49, 0x7F, 0x36};

static uint8_t ascii_4_size = 6;
static uint8_t ascii_4[6] = {0x18, 0x14, 0x12, 0x7F, 0x7F, 0x10};

static uint8_t ascii_5_size = 5;
static uint8_t ascii_5[5] = {0x4F, 0x4F, 0x49, 0x79, 0x31};

static uint8_t ascii_6_size = 6;
static uint8_t ascii_6[6] = {0x3C, 0x7E, 0x4B, 0x49, 0x79, 0x30};

static uint8_t ascii_7_size = 6;
static uint8_t ascii_7[6] = {0x01, 0x01, 0x71, 0x7D, 0x0F, 0x03};

static uint8_t ascii_8_size = 6;
static uint8_t ascii_8[6] = {0x36, 0x7F, 0x49, 0x49, 0x7F, 0x36};

static uint8_t ascii_9_size = 6;
static uint8_t ascii_9[6] = {0x06, 0x4F, 0x49, 0x69, 0x3F, 0x1E};

static uint8_t ascii_space_size = 2;
static uint8_t ascii_space[2] = {0x00, 0x00};

void SSD1306_ascii_display_A(bool negative) { SSD1306_ascii_display_driver(ascii_A_size, ascii_A, negative); }

void SSD1306_ascii_display_B(bool negative) { SSD1306_ascii_display_driver(ascii_B_size, ascii_B, negative); }

void SSD1306_ascii_display_C(bool negative) { SSD1306_ascii_display_driver(ascii_C_size, ascii_C, negative); }

void SSD1306_ascii_display_D(bool negative) { SSD1306_ascii_display_driver(ascii_D_size, ascii_D, negative); }

void SSD1306_ascii_display_E(bool negative) { SSD1306_ascii_display_driver(ascii_E_size, ascii_E, negative); }

void SSD1306_ascii_display_F(bool negative) { SSD1306_ascii_display_driver(ascii_F_size, ascii_F, negative); }

void SSD1306_ascii_display_G(bool negative) { SSD1306_ascii_display_driver(ascii_G_size, ascii_G, negative); }

void SSD1306_ascii_display_H(bool negative) { SSD1306_ascii_display_driver(ascii_H_size, ascii_H, negative); }

void SSD1306_ascii_display_I(bool negative) { SSD1306_ascii_display_driver(ascii_I_size, ascii_I, negative); }

void SSD1306_ascii_display_J(bool negative) { SSD1306_ascii_display_driver(ascii_J_size, ascii_J, negative); }

void SSD1306_ascii_display_K(bool negative) { SSD1306_ascii_display_driver(ascii_K_size, ascii_K, negative); }

void SSD1306_ascii_display_L(bool negative) { SSD1306_ascii_display_driver(ascii_L_size, ascii_L, negative); }

void SSD1306_ascii_display_M(bool negative) { SSD1306_ascii_display_driver(ascii_M_size, ascii_M, negative); }

void SSD1306_ascii_display_N(bool negative) { SSD1306_ascii_display_driver(ascii_N_size, ascii_N, negative); }

void SSD1306_ascii_display_O(bool negative) { SSD1306_ascii_display_driver(ascii_O_size, ascii_O, negative); }

void SSD1306_ascii_display_P(bool negative) { SSD1306_ascii_display_driver(ascii_P_size, ascii_P, negative); }

void SSD1306_ascii_display_Q(bool negative) { SSD1306_ascii_display_driver(ascii_Q_size, ascii_Q, negative); }

void SSD1306_ascii_display_R(bool negative) { SSD1306_ascii_display_driver(ascii_R_size, ascii_R, negative); }

void SSD1306_ascii_display_S(bool negative) { SSD1306_ascii_display_driver(ascii_S_size, ascii_S, negative); }

void SSD1306_ascii_display_T(bool negative) { SSD1306_ascii_display_driver(ascii_T_size, ascii_T, negative); }

void SSD1306_ascii_display_U(bool negative) { SSD1306_ascii_display_driver(ascii_U_size, ascii_U, negative); }

void SSD1306_ascii_display_V(bool negative) { SSD1306_ascii_display_driver(ascii_V_size, ascii_V, negative); }

void SSD1306_ascii_display_W(bool negative) { SSD1306_ascii_display_driver(ascii_W_size, ascii_W, negative); }

void SSD1306_ascii_display_X(bool negative) { SSD1306_ascii_display_driver(ascii_X_size, ascii_X, negative); }

void SSD1306_ascii_display_Y(bool negative) { SSD1306_ascii_display_driver(ascii_Y_size, ascii_Y, negative); }

void SSD1306_ascii_display_Z(bool negative) { SSD1306_ascii_display_driver(ascii_Z_size, ascii_Z, negative); }

void SSD1306_ascii_display_0(bool negative) { SSD1306_ascii_display_driver(ascii_0_size, ascii_0, negative); }

void SSD1306_ascii_display_1(bool negative) { SSD1306_ascii_display_driver(ascii_1_size, ascii_1, negative); }

void SSD1306_ascii_display_2(bool negative) { SSD1306_ascii_display_driver(ascii_2_size, ascii_2, negative); }

void SSD1306_ascii_display_3(bool negative) { SSD1306_ascii_display_driver(ascii_3_size, ascii_3, negative); }

void SSD1306_ascii_display_4(bool negative) { SSD1306_ascii_display_driver(ascii_4_size, ascii_4, negative); }

void SSD1306_ascii_display_5(bool negative) { SSD1306_ascii_display_driver(ascii_5_size, ascii_5, negative); }

void SSD1306_ascii_display_6(bool negative) { SSD1306_ascii_display_driver(ascii_6_size, ascii_6, negative); }

void SSD1306_ascii_display_7(bool negative) { SSD1306_ascii_display_driver(ascii_7_size, ascii_7, negative); }

void SSD1306_ascii_display_8(bool negative) { SSD1306_ascii_display_driver(ascii_8_size, ascii_8, negative); }

void SSD1306_ascii_display_9(bool negative) { SSD1306_ascii_display_driver(ascii_9_size, ascii_9, negative); }

void SSD1306_ascii_display_space(bool negative) {
  SSD1306_ascii_display_driver(ascii_space_size, ascii_space, negative);
}

void SSD1306_ascii_display_driver(uint8_t ascii_letter_size, uint8_t ascii_letter[], bool negative) {
  if (negative) {
    SSD1306__send_data_negative(ascii_letter, ascii_letter_size);
  } else {
    SSD1306__send_data(ascii_letter, ascii_letter_size);
  }
}

void SSD1306_ascii_display_string(const char *const string_to_print) {
  uint32_t index = 0UL;
  while (string_to_print[index] != '\0') {
    SSD1306_ascii_display_char(string_to_print[index], false);
    SSD1306_ascii_display_space(false);
    ++index;
  }
}

void SSD1306_ascii_display_string_with_max_length(const char *const string_to_print, const uint32_t *const max_length,
                                                  bool negative) {
  uint32_t index = 0UL;
  while (string_to_print[index] != '\0') {
    SSD1306_ascii_display_char(string_to_print[index], negative);
    SSD1306_ascii_display_space(negative);

    index++;

    if (max_length != NULL && index >= *max_length) {
      break;
    }
  }
}

void SSD1306_ascii_display_char(char character, bool negative) {
  switch (character) {
  case 'A':
  case 'a':
    SSD1306_ascii_display_A(negative);
    break;

  case 'B':
  case 'b':
    SSD1306_ascii_display_B(negative);
    break;

  case 'C':
  case 'c':
    SSD1306_ascii_display_C(negative);
    break;

  case 'D':
  case 'd':
    SSD1306_ascii_display_D(negative);
    break;

  case 'E':
  case 'e':
    SSD1306_ascii_display_E(negative);
    break;

  case 'F':
  case 'f':
    SSD1306_ascii_display_F(negative);
    break;

  case 'G':
  case 'g':
    SSD1306_ascii_display_G(negative);
    break;

  case 'H':
  case 'h':
    SSD1306_ascii_display_H(negative);
    break;

  case 'I':
  case 'i':
    SSD1306_ascii_display_I(negative);
    break;

  case 'J':
  case 'j':
    SSD1306_ascii_display_J(negative);
    break;

  case 'K':
  case 'k':
    SSD1306_ascii_display_K(negative);
    break;

  case 'L':
  case 'l':
    SSD1306_ascii_display_L(negative);
    break;

  case 'M':
  case 'm':
    SSD1306_ascii_display_M(negative);
    break;

  case 'N':
  case 'n':
    SSD1306_ascii_display_N(negative);
    break;

  case 'O':
  case 'o':
    SSD1306_ascii_display_O(negative);
    break;

  case 'P':
  case 'p':
    SSD1306_ascii_display_P(negative);
    break;

  case 'Q':
  case 'q':
    SSD1306_ascii_display_Q(negative);
    break;

  case 'R':
  case 'r':
    SSD1306_ascii_display_R(negative);
    break;

  case 'S':
  case 's':
    SSD1306_ascii_display_S(negative);
    break;

  case 'T':
  case 't':
    SSD1306_ascii_display_T(negative);
    break;

  case 'U':
  case 'u':
    SSD1306_ascii_display_U(negative);
    break;

  case 'V':
  case 'v':
    SSD1306_ascii_display_V(negative);
    break;

  case 'W':
  case 'w':
    SSD1306_ascii_display_W(negative);
    break;

  case 'X':
  case 'x':
    SSD1306_ascii_display_X(negative);
    break;

  case 'Y':
  case 'y':
    SSD1306_ascii_display_Y(negative);
    break;

  case 'Z':
  case 'z':
    SSD1306_ascii_display_Z(negative);
    break;

  case '0':
    SSD1306_ascii_display_0(negative);
    break;

  case '1':
    SSD1306_ascii_display_1(negative);
    break;

  case '2':
    SSD1306_ascii_display_2(negative);
    break;

  case '3':
    SSD1306_ascii_display_3(negative);
    break;

  case '4':
    SSD1306_ascii_display_4(negative);
    break;

  case '5':
    SSD1306_ascii_display_5(negative);
    break;

  case '6':
    SSD1306_ascii_display_6(negative);
    break;

  case '7':
    SSD1306_ascii_display_7(negative);
    break;

  case '8':
    SSD1306_ascii_display_8(negative);
    break;

  case '9':
    SSD1306_ascii_display_9(negative);
    break;

  case ' ':
    SSD1306_ascii_display_space(negative);
    break;

  default:
    SSD1306_ascii_display_space(negative);
    break;
  }
}