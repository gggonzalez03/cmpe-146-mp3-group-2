#include "SSD1306_OLED_ascii.h"
#include "SSD1306_OLED.h"
#include <stdint.h>

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

static uint8_t ascii_space_size = 2;
static uint8_t ascii_space[2] = {0x00, 0x00};

void SSD1306_ascii_display_A() { SSD1306_ascii_display_driver(ascii_A_size, ascii_A); }

void SSD1306_ascii_display_B() { SSD1306_ascii_display_driver(ascii_B_size, ascii_B); }

void SSD1306_ascii_display_C() { SSD1306_ascii_display_driver(ascii_C_size, ascii_C); }

void SSD1306_ascii_display_D() { SSD1306_ascii_display_driver(ascii_D_size, ascii_D); }

void SSD1306_ascii_display_E() { SSD1306_ascii_display_driver(ascii_E_size, ascii_E); }

void SSD1306_ascii_display_F() { SSD1306_ascii_display_driver(ascii_F_size, ascii_F); }

void SSD1306_ascii_display_G() { SSD1306_ascii_display_driver(ascii_G_size, ascii_G); }

void SSD1306_ascii_display_H() { SSD1306_ascii_display_driver(ascii_H_size, ascii_H); }

void SSD1306_ascii_display_I() { SSD1306_ascii_display_driver(ascii_I_size, ascii_I); }

void SSD1306_ascii_display_J() { SSD1306_ascii_display_driver(ascii_J_size, ascii_J); }

void SSD1306_ascii_display_K() { SSD1306_ascii_display_driver(ascii_K_size, ascii_K); }

void SSD1306_ascii_display_L() { SSD1306_ascii_display_driver(ascii_L_size, ascii_L); }

void SSD1306_ascii_display_M() { SSD1306_ascii_display_driver(ascii_M_size, ascii_M); }

void SSD1306_ascii_display_N() { SSD1306_ascii_display_driver(ascii_N_size, ascii_N); }

void SSD1306_ascii_display_O() { SSD1306_ascii_display_driver(ascii_O_size, ascii_O); }

void SSD1306_ascii_display_P() { SSD1306_ascii_display_driver(ascii_P_size, ascii_P); }

void SSD1306_ascii_display_Q() { SSD1306_ascii_display_driver(ascii_Q_size, ascii_Q); }

void SSD1306_ascii_display_R() { SSD1306_ascii_display_driver(ascii_R_size, ascii_R); }

void SSD1306_ascii_display_S() { SSD1306_ascii_display_driver(ascii_S_size, ascii_S); }

void SSD1306_ascii_display_T() { SSD1306_ascii_display_driver(ascii_T_size, ascii_T); }

void SSD1306_ascii_display_U() { SSD1306_ascii_display_driver(ascii_U_size, ascii_U); }

void SSD1306_ascii_display_V() { SSD1306_ascii_display_driver(ascii_V_size, ascii_V); }

void SSD1306_ascii_display_W() { SSD1306_ascii_display_driver(ascii_W_size, ascii_W); }

void SSD1306_ascii_display_X() { SSD1306_ascii_display_driver(ascii_X_size, ascii_X); }

void SSD1306_ascii_display_Y() { SSD1306_ascii_display_driver(ascii_Y_size, ascii_Y); }

void SSD1306_ascii_display_Z() { SSD1306_ascii_display_driver(ascii_Z_size, ascii_Z); }

void SSD1306_ascii_display_space() { SSD1306_ascii_display_driver(ascii_space_size, ascii_space); }

void SSD1306_ascii_display_driver(uint8_t ascii_letter_size, uint8_t ascii_letter[]) {
  SSD1306__send_data(ascii_letter, ascii_letter_size);

  //  for (int i = 0; i < ascii_letter_size; i++) {
  //   SSD1306__send_data(ascii_letter[i], 1);
  //  / }
}