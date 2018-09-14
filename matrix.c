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
 * D2 - D10, D14 - D16, A0 - A3
 * Pin usage:
 *   COL: PD1, PD0, PD4, PC6, PD7, PE6, PB4, PB5
 *   ROW: PB5, PB6, PB1-PB3, PF5-7
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
    DDRD = 0x00;
    PORTD = 0xFF;
    DDRC = 0x00;
    PORTC = 0xFF;
    DDRB = 0x00;
    PORTB = 0xFF;
    DDRE = 0x00;
    PORTE = 0xFF;

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
	if (PIND & (1 << PD1) {
		result &= 0b00000001;
	} else if (PIND & (1 << PD0) {
		result &= 0b00000010;
	} else if (PIND & (1 << PD4) {
		result &= 0b00000100;
	} else if (PINC & (1 << PC6) {
		result &= 0b00001000;
	} else if (PIND & (1 << PD7) {
		result &= 0b00010000;
	} else if (PINE & (1 << PE6) {
		result &= 0b00100000;
	} else if (PINB & (1 << PB4) {
		result &= 0b01000000;
	} else if (PINB & (1 << PB5) {
		result &= 0b10000000;
	}
    return result;
}

inline
static void unselect_rows(void)
{
    // Hi-Z(DDR:0, PORT:0) to unselect
    DDRB  &= ~0b10010001;
    PORTB &= ~0b10010001;
    DDRF  &= ~0b00011111;
    PORTF &= ~0b00011111;
}

inline
static void select_row(uint8_t row)
{
    // Output low(DDR:1, PORT:0) to select
    switch (row) {
        case 0:
	        DDRB |= (1<<5);
			PORTB &= ~(1<<5);
			break;
        case 1:
	        DDRB |= (1<<6);
			PORTB &= ~(1<<6);
			break;
        case 2:
            DDRB  |=  (1<<1);
            PORTB &= ~(1<<1);
            break;
        case 3:
            DDRB  |=  (1<<2);
            PORTB &= ~(1<<2);
            break;
        case 4:
            DDRB  |=  (1<<3);
            PORTB &= ~(1<<3);
            break;
        case 5:
        case 6:
        case 7:
            DDRF  |=  (1<<row);
            PORTF &= ~(1<<row);
            break;
    }
}
