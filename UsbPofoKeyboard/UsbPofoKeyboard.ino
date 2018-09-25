#include <Keyboard.h>
#include "keymap.h"
#include <avr/pgmspace.h>

#define DEBOUNCE 5
#define SIZE 8
#define matrix_row_t uint8_t
#define D0 PD2

int rows[SIZE] = { 6,  7, 15, 16, 14,  8,  9, 10};
<<<<<<< HEAD
//int rows[SIZE] = {10,  9,  8, 14· 16, 15·  7,  6};
int cols[SIZE] = { 2,  3,  4,  5, 21, 20, 19, 18};
//int cols[SIZE] = {18, 19, 20, 21,  5,  4,  3,  2};
=======
int cols[SIZE] = {18, 19, 20, 21,  5,  4,  3,  2};
>>>>>>> d0919d2277ac5ec578ea0f66a9bf8c163f52a8c3
uint8_t debouncing = DEBOUNCE;

matrix_row_t matrix[SIZE];
matrix_row_t matrix_debouncing[SIZE];

matrix_row_t read_rows(void) {
  matrix_row_t result = 0;
  for (int i = 0; i < SIZE; i++) {
    if (digitalRead(rows[i]) == LOW) {
      bitSet(result, i);
    }
  }
  return result;
}

void unselect_cols(void) {
  for (int pin = 0; pin < SIZE; pin++) {
    pinMode(cols[pin], OUTPUT);
    digitalWrite(cols[pin], HIGH);
  }
}

void select_col(uint8_t col) {
  digitalWrite(cols[col], LOW);
}

void matrix_scan(void)
{
    for (uint8_t i = 0; i < SIZE; i++) {
        select_col(i);
        delayMicroseconds(30);  // without this wait read unstable value.
        matrix_row_t rows = read_rows();
        if (matrix_debouncing[i] != rows) {
            matrix_debouncing[i] = rows;
            debouncing = DEBOUNCE;
        }
        unselect_cols();
    }

    if (debouncing) {
        if (--debouncing) {
            delay(1);
        } else {
            for (uint8_t i = 0; i < SIZE; i++) {
                matrix[i] = matrix_debouncing[i];
            }
        }
    }
}

void setup() {
  for (int pin = 0; pin < SIZE; pin++) {
    pinMode(rows[pin], INPUT_PULLUP);
    pinMode(cols[pin], OUTPUT);
    digitalWrite(cols[pin], HIGH);
  }

  // Powerup Pin
  pinMode(D0, OUTPUT);
  digitalWrite(D0, LOW);
  TXLED0;
  
  Serial.begin(115200);
//  Keyboard.begin();
}

void clearAndHome()
{
  Serial.write(27); // ESC
  Serial.print("[H"); // cursor to home
}

/*
 * Matrix: 8x8
 *    |       0 |       1 |       2 |       3 |       4 |       5 |       6 |       7
 * ---+---------+---------+---------+---------+---------+---------+---------+---------
 *  0 | LCTRL   | Tab     | Up      | F       | 0       | S       | <       | C      
 *  1 | 1       | Atari   | Q       | J       | ß       | 4       | Space   | B      
 *  2 | 3       | R       | O       | Right   | Ü       | RSFT    | .       | M      
 *  3 | 5       | Enter   | BS      | 8       | Ä       | ,       | Fn      | A      
 *  4 |         | W       | LALT    | H       | I       | P       | Y       | V      
 *  5 | 2       | E       | U       | Entf    | LSFT    | G       | Ö       | N      
 *  6 | D       | T       | 7       | Caps    | Left    | Down    | '       | -      
 *  7 | 6       | Z       | 9       | K       | +       | L       | X       | Esc    
 */

#define KEY_ATARI 0xFF
#define KEY_FN    0xFE

const uint8_t pofo_key_map[64] PROGMEM = {
	KEY_LEFT_CTRL,    KEY_TAB,  KEY_UP_ARROW,             K_f,            K_0,             K_s,  K_LEQ, K_c,    // 0
	          K_1,  KEY_ATARI,           K_q,             K_j,            K_s,             K_4, K_SPCE, K_b,    // 1
	          K_3,        K_r,           K_o, KEY_RIGHT_ARROW,           0x00, KEY_RIGHT_SHIFT,  K_DOT, K_m,    // 2
            K_5, KEY_RETURN, KEY_BACKSPACE,             K_8,           0x00,           K_CMA, KEY_FN, K_a,    // 3
           0x00,        K_w,  KEY_LEFT_ALT,             K_h,            K_i,             K_p,    K_y, K_v,    // 4
            K_2,        K_e,           K_u,      KEY_DELETE, KEY_LEFT_SHIFT,             K_g,   0x00, K_n,    // 5
            K_d,        K_t,           K_7,   KEY_CAPS_LOCK, KEY_LEFT_ARROW,  KEY_DOWN_ARROW,  K_QUT, K_MNS,  // 6
            K_6,        K_z,           K_9,             K_K,         K_PLUS,             K_L,    K_x, KEY_ESC // 7
};

uint8_t state = 0;
long mills = 0;

void loop() {
  // scan the matrix
  matrix_scan();
  
  // k = row_index
  int k = 0;
  
  // cycle through the matrix
  for (int i = 0; i < SIZE; i++) {
	  // print the matrix
	  Serial.print(i, DEC);
	  Serial.print(": ");
	  Serial.print(matrix[i], BIN);
	  Serial.println();
	  delay(1);
    
	  // transform matrix into a number, assuming only one button is pressed
	  int res = 0;
	  for (int x = 1; x < SIZE+1; x++) {
		  if (bitRead(matrix[i], x-1)) {
			  res += x;
		  }
	  }
	
	  // fetch the char from pgm_space
	  uint8_t chr = pgm_read_byte(pofo_key_map + res + k);
	  k += 8;
	  if (chr == KEY_ATARI) { // atari key pressed
        bitSet(state, 0);
      } else if (chr == KEY_FN) { // FN key pressed
        bitSet(state, 1);
      } else if (bitRead(state, 0) && chr == K_o) { // if atari key + o is pressed, turn on
        mills = millis();
        digitalWrite(D0, HIGH);
        TXLED1;
        state = 0;
      } else {
	    Serial.println(chr);
      }
    }

  if (millis() - mills > 30000) {
    mills = millis();
    digitalWrite(D0, LOW); // after 30secs turn off rpi
    TXLED0;
  }
  
  delay(50);
  clearAndHome();
}

