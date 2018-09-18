/*
Copyright 2018 Mike Vogt <vogt31337@gmx.de>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* 
 * Keymap for Pofo mod
 */
#include <stdint.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include "keycode.h"
#include "print.h"
#include "debug.h"
#include "util.h"
#include "keymap.h"


/*
 * Layout: 59key
 * ,-----------------------------------------------------------.
 * |Esc|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  ß|  '|   BS  | 14
 * |-----------------------------------------------------------|
 * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  Ü|  +|     | 13
 * |------------------------------------------------------     |
 * |Caps  |  A|  S|  D|  F|  G|  H|  J|  K|  L|  Ö|  Ä|Return  | 13
 * |-----------------------------------------------------------|
 * |Shft|  <|  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  -| Up|  Shft| 14
 * |-----------------------------------------------------------|
 * |Ctrl| Fn |Atari| Alt|      Spc           |Del|Lft|Dwn|Rght |  9
 * `-----------------------------------------------------------'
 *
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
 *
 */
#define KEYMAP( \
    K77, K01, K05, K02, K51, K03, K07, K26, K33, K27, K40, K41, K66, K23,\
    K10, K21, K14, K15, K12, K16, K64, K25, K44, K22, K54, K42, K47,\
    K36, K73, K50, K06, K30, K55, K34, K31, K37, K57, K65, K43, K13,\
    K45, K60, K17, K67, K70, K74, K71, K75, K72, K53, K62, K76, K20, K52,\
    K00, K63, K11, K24, K61, K35, K46, K56, K32 \
) { \
    { KC_##K00, KC_##K10, KC_##K20, KC_##K30, KC_##K40, KC_##K50, KC_##K60, KC_##K70 }, \
    { KC_##K01, KC_##K11, KC_##K21, KC_##K31, KC_##K41, KC_##K51, KC_##K61, KC_##K71 }, \
    { KC_##K02, KC_##K12, KC_##K22, KC_##K32, KC_##K42, KC_##K52, KC_##K62, KC_##K72 }, \
    { KC_##K03, KC_##K13, KC_##K23, KC_##K33, KC_##K43, KC_##K53, KC_##K63, KC_##K73 }, \
    {    KC_NO, KC_##K14, KC_##K24, KC_##K34, KC_##K44, KC_##K54, KC_##K64, KC_##K74 }, \
    { KC_##K05, KC_##K15, KC_##K25, KC_##K35, KC_##K45, KC_##K55, KC_##K65, KC_##K75 }, \
    { KC_##K06, KC_##K16, KC_##K26, KC_##K36, KC_##K46, KC_##K56, KC_##K66, KC_##K76 }, \
    { KC_##K07, KC_##K17, KC_##K27, KC_##K37, KC_##K47, KC_##K57, KC_##K67, KC_##K77 }, \
}

#define KEYCODE(layer, row, col) (pgm_read_byte(&keymaps[(layer)][(row)][(col)]))

// Assign Fn key(0-7) to a layer to which switch with the Fn key pressed.
static const uint8_t PROGMEM fn_layer[] = {
    0,              // Fn0
    1,              // Fn1
    2,              // Fn2
    3,              // Fn3
    4,              // Fn4
    0,              // Fn5
    0,              // Fn6
    0               // Fn7
};

// Assign Fn key(0-7) to a keycode sent when release Fn key without use of the layer.
// See layer.c for details.
static const uint8_t PROGMEM fn_keycode[] = {
    KC_NO,          // Fn0
    KC_NO,          // Fn1
    KC_SLSH,        // Fn2
    KC_SCLN,        // Fn3
    KC_SPC,         // Fn4
    KC_NO,          // Fn5
    KC_NO,          // Fn6
    KC_NO           // Fn7
};

static const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Layer 0: Default Layer
     * ,-----------------------------------------------------------.
     * |Esc|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  ß|  '|   BS  | 14
     * |-----------------------------------------------------------|
     * |Tab  |  Q|  W|  E|  R|  T|  Z|  U|  I|  O|  P|  Ü|  +|     | 13
     * |------------------------------------------------------     |
     * |Caps  |  A|  S|  D|  F|  G|  H|  J|  K|  L|  Ö|  Ä|Return  | 13
     * |-----------------------------------------------------------|
     * |Shft|  <|  Y|  X|  C|  V|  B|  N|  M|  ,|  .|  -| Up|  Shft| 14
     * |-----------------------------------------------------------|
     * |Ctrl| Fn |Atari| Alt|      Spc           |Del|Lft|Dwn|Rght |  9
     * `-----------------------------------------------------------'
     */
    KEYMAP(   ESC,   1,   2,   3,   4,   5,   6,   7,   8,   9,   0, SLSH,QUOT, BSPC, \
              TAB,   Q,   W,   E,   R,   T,   Z,   U,   I,   O,   P, LBRC,RBRC, \
             CAPS,   A,   S,   D,   F,   G,   H,   J,   K,   L,SCLN, QUOT, ENT, \
             LSFT,BSLS,   Y,   X,   C,   V,   B,   N,   M,COMM, DOT, MINS,  UP, RSFT, \
           LCTRL, FN1, FN2,LALT,          SPC,              DELETE, LEFT,DOWN, RIGHT),


    /* Layer 1: Fn mode
     * ,-----------------------------------------------------------.
     * |Esc| F1| F2| F3| F4| F5| F6| F7| F8| F9|F10|   |   |       | 14
     * |-----------------------------------------------------------|
     * |Tab  |   |   |   |   |   |   |   |   |Off|   |   |   |     | 13
     * |------------------------------------------------------     |
     * |Caps  |   |   |   |   |   |   |   |   |   |   |   |Return  | 13
     * |-----------------------------------------------------------|
     * |Shft|   |   |   |   |   |   |   |   |   |   |   |PgU|  Shft| 14
     * |-----------------------------------------------------------|
     * |Ctrl| Fn |Atari| Alt|      Space         |   |Hme|PgD|End  |  9
     * `-----------------------------------------------------------'
     */ 
    KEYMAP(   ESC, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9, F10,  NO,  NO, NO, \
              TAB, NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO, PWR,  NO,  NO,  NO, \
             CAPS, NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO, ENT, \
             LSFT, NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO,  NO,PGUP, RSFT, \
            LCTRL,FN1, FN2,LALT,          SPC,                  NO,HOME,PGDN, END),


    /* Layer 2: Atari mode
     * ,-----------------------------------------------------------.
     * |Esc|   |   |   |   |   |   |  7|  8|  9|  +|   |   |   BS  |
     * |-----------------------------------------------------------|
     * |Tab  |   |Wor|Edt|   |   |   |  4|  5|  6|  *|   |   |     |
     * |------------------------------------------------------     |
     * |Caps  |Adr|Set|Dia|   |   |   |  1|  2|  3|  -|   |       =|
     * |-----------------------------------------------------------|
     * |Shft|   |Men|   |Cal|   |   |   |  0|   |  .|  /|CnU|  Shft|
     * |-----------------------------------------------------------|
     * |Ctrl| Fn |Atari| Alt|      Space         |   |   |CnD|     |
     * `-----------------------------------------------------------'
     */
    KEYMAP(   ESC, NO,  NO,  NO,  NO,  NO,  NO,   7,   8,   9, PPLS,   NO,   NO,BSPC, \
              TAB, NO,  NO,  NO,  NO,  NO,  NO,   4,   5,   6, PAST,   NO,   NO, \
             CAPS, NO,  NO,  NO,  NO,  NO,  NO,   1,   2,   3, PMNS,   NO,  EQL, \
             LSFT, NO,  NO,  NO,  NO,  NO,  NO,  NO,   0,  NO,  DOT, SLSH, VOLU, RSFT, \
            LCTRL,FN1, FN2,LALT,          SPC,             NO,   NO, VOLD,   NO),
};


uint8_t keymap_get_keycode(uint8_t layer, uint8_t row, uint8_t col)
{
    return KEYCODE(layer, row, col);
}

uint8_t keymap_fn_layer(uint8_t index)
{
    return pgm_read_byte(&fn_layer[index]);
}

uint8_t keymap_fn_keycode(uint8_t index)
{
    return pgm_read_byte(&fn_keycode[index]);
}
