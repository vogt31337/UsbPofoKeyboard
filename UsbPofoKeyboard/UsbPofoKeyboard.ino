#define DEBOUNCE 5
#define SIZE 8
#define matrix_row_t uint8_t

int rows[SIZE] = { 6,  7, 15, 16, 14,  8,  9, 10};
//int rows[SIZE] = {10,  9,  8, 14· 16, 15·  7,  6};
int cols[SIZE] = { 2,  3,  4,  5, 21, 20, 19, 18};
//int cols[SIZE] = {18, 19, 20, 21,  5,  4,  3,  2};
uint8_t debouncing = DEBOUNCE;

matrix_row_t matrix[SIZE];
matrix_row_t matrix_debouncing[SIZE];

matrix_row_t read_rows(void) {
  matrix_row_t result = 0;
  for (int i = 0; i < SIZE; i++) {
    if (digitalRead(rows[i]) == HIGH) {
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

void loop() {
  matrix_scan();
  for (int i = 0; i < SIZE; i++) {
    Serial.print(i, DEC);
    Serial.print(": ");
    Serial.print(matrix[i], BIN);
    Serial.println();
    delay(1);

  }
  delay(50);
  clearAndHome();
}

