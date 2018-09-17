#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "config.h"
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"


/*
 * Atari Portfolio Keyboard
 *
 * Cols:  6(D6),  7(D7), 15(D15), 16(D14), 14(D16),  8(D8),  9(D9), 10(D10)
 * Rows:  2(D2),  3(D3),   4(D4),   5(D5), 21(A3) , 20(A2) , 19(A1), 18(A0)
 *
 * Pin usage:
 *   COL: PD7, PE6, PB1, PB2, PB3, PB4, PB5, PB6
 *   ROW: PD1, PD0, PD4, PC6, PF4, PF5, PF6, PF7
 */
static uint8_t debouncing = DEBOUNCE;

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static matrix_row_t read_cols(void);
static void unselect_rows(void);
static void select_row(uint8_t row);

void matrix_init(void)
{
    // initialize rows
    unselect_rows();

    // initialize columns to input with pull-up(DDR:0, PORT:1)
	// COL: PD7, PE6, PB1, PB2, PB3, PB4, PB5, PB6
	DDRD  &= ~0b10000000;
    PORTD &=  0b10000000;
    DDRE  &= ~0b01000000;
	PORTE &=  0b01000000;
	DDRB  &= ~0b01111110;
    PORTB &=  0b01111110;

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }
}

uint8_t matrix_scan(void)
{
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        select_row(i);
        _delay_us(30);  // without this wait read unstable value.
        matrix_row_t cols = read_cols();
        if (matrix_debouncing[i] != cols) {
            matrix_debouncing[i] = cols;
            if (debouncing) {
                debug("bounce!: "); debug_hex(debouncing); debug("\n");
            }
            debouncing = DEBOUNCE;
        }
        unselect_rows();
    }

    if (debouncing) {
        if (--debouncing) {
            _delay_ms(1);
        } else {
            for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
                matrix[i] = matrix_debouncing[i];
            }
        }
    }

    return 1;
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

inline
static matrix_row_t read_cols(void)
{
    matrix_row_t result = 0;
	if (PIND & (1 << PD7) {
		result &= 0b00000001;
	} else if (PINE & (1 << PE6) {
		result &= 0b00000010;
	} else if (PINB & (1 << PB1) {
		result &= 0b00000100;
	} else if (PINB & (1 << PB2) {
		result &= 0b00001000;
	} else if (PINB & (1 << PD3) {
		result &= 0b00010000;
	} else if (PINB & (1 << PE4) {
		result &= 0b00100000;
	} else if (PINB & (1 << PB5) {
		result &= 0b01000000;
	} else if (PINB & (1 << PB6) {
		result &= 0b10000000;
	}
    return result;
}

inline
static void unselect_rows(void)
{
    // Hi-Z(DDR:0, PORT:0) to unselect
    DDRD  &= ~0b00001011;
    PORTB &= ~0b00001011;
    DDRC  &= ~0b01000000;
    PORTC &= ~0b01000000;
    DDRF  &= ~0b11110000;
    PORTF &= ~0b11110000;
}

inline
static void select_row(uint8_t row)
{
    // Output low(DDR:1, PORT:0) to select
    switch (row) {
        case 0:
            DDRD  |=  (1<<PD1);
            PORTD &= ~(1<<PD1);
            break;
        case 1:
            DDRD  |=  (1<<PD0);
            PORTD &= ~(1<<PD0);
            break;
        case 2:
            DDRD  |=  (1<<PD4);
            PORTD &= ~(1<<PD4);
            break;
        case 3:
            DDRC  |=  (1<<PC6);
            PORTC &= ~(1<<PC6);
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            DDRF  |=  (1<<row);
            PORTF &= ~(1<<row);
            break;
    }
}
