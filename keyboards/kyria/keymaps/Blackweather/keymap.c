/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

enum layers {
    _COLEMAK_DHM = 0,
    //_QWERTY,
    _NAV,
    _MOUSE,
    _MEDIA,
    _NUM,
    _SYM,
    _FUNC
};

enum custom_keycodes {
    QWERTY = SAFE_RANGE,
    CMAKDH,
    REDO,
    PASTE,
    COPY,
    CUT,
    UNDO
};

bool is_qwerty_active = false;

bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: COLEMAK_DHM GACS
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |XXXXXXXX|   Q  |   W  |   F  |   P  |   B  |                              |   J  |   L  |   U  |   Y  | ' "  |  XXX   |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |XXXXXXX | GUI/A| Alt/R|Ctrl/S|LSft/T|   G  |                              |   M  |LSft/N|Ctrl/E| Alt/I| GUI/O|  XXX   |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | XXXXXX |   Z  |ALGR/X|   C  |   D  |   V  |XXXXXX|XXXXXX|  |  XXX |XXXXXX|   K  |   H  | ,  < |ALGR.>| /  ? |  XXX   |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        | GUI  | Esc  | Space| Tab  | XXX  |  | XXXXX| Enter| Bksp |Delete| Mute |
 *                        |      | Media| Nav  | Mouse| XXXXX|  | XXXXX| Sym  | Num |  Fn  |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_COLEMAK_DHM] = LAYOUT(
      KC_NO, KC_Q, KC_W, KC_F, KC_P, KC_B, KC_J, KC_L, KC_U, KC_Y, KC_QUOT, KC_NO,
      KC_NO, MT(MOD_LGUI,KC_A), MT(MOD_LALT, KC_R), MT(MOD_LCTL, KC_S), MT(MOD_LSFT, KC_T), KC_G, KC_M, MT(MOD_RSFT, KC_N), MT(MOD_RCTL, KC_E), MT(MOD_LALT, KC_I), MT(MOD_RGUI, KC_O), KC_NO,
      KC_LSFT, KC_Z, MT(MOD_RALT, KC_X), KC_C, KC_D, KC_V, KC_LSFT, KC_LSFT, KC_NO, KC_LSFT, KC_K, KC_H, KC_COMM, MT(MOD_RALT, KC_DOT), KC_SLSH, KC_NO,
      KC_LGUI, LT(_MEDIA, KC_ESC), LT(_NAV, KC_SPC), LT(_MOUSE, KC_TAB), KC_NO, KC_NO, LT(_SYM, KC_ENT), LT(_NUM, KC_BSPC), LT(_FUNC, KC_DEL), KC_MUTE
    ),

/*
 * Alternative layout: QWERTY - disabled for now
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * | ESC    |   Q  |   W  |   E  |   R  |  T   |                              |   Y  |   U  |   I  |   O  |   P  |  |  \  |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |Ctrl/BS |   A  | S    |  D   |  F   |  G   |                              |   H  |   J  |  K   |   L  |   ;  |  ' "   |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |  B   |LShift|LShift|  |  Tab |LShift|   N  |   M  |  , < |  . > |  / ? |  - _   |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |  GUI | Del  | Space| Enter| Esc  |  | Enter| Space| AltGr| Bksp | Mute |
 *                        |      |      | Alt  | Lower| Raise|  | Lower| Raise|      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    // [_QWERTY] = LAYOUT(
    //   KC_ESC,    KC_Q, KC_W, KC_E, KC_R, KC_T,                                     KC_Y, KC_U, KC_I,    KC_O,   KC_P,    KC_PIPE,
    //   MT(MOD_LCTL, KC_BSPC), KC_A, KC_S, KC_D, KC_F, KC_G,                                     KC_H, KC_J, KC_K,    KC_L,   KC_SCLN, KC_QUOT,
    //   KC_LSFT,               KC_Z, KC_X, KC_C, KC_V, KC_B, KC_LSFT, KC_LSFT, KC_TAB, KC_LSFT,  KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_MINS,
    //                         KC_LGUI, KC_DEL, MT(MOD_LALT, KC_SPC), LT(_LOWER, KC_ENT), LT(_RAISE, KC_ESC), LT(_LOWER, KC_ENT), LT(_RAISE, KC_SPC), MOD_RALT, KC_BSPC, KC_MUTE
    // ),

/*
 * Navigation Layer
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              | Redo | Paste| Copy | Cut  | Undo |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |                              | Caps | Left | Down |  Up  | Right|        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |      |  |      |      |Insert| Home | PgDn | PgUp | End  |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      | Enter| Bksp |Delete|      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */

     [_NAV] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     REDO, PASTE, COPY, CUT, UNDO, _______,
      _______, _______, _______, _______, _______, _______,                                     KC_CAPS, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_INS, KC_HOME, KC_PGDN, KC_PGUP, KC_END, _______,
                                 _______, _______, _______, _______, _______, _______, KC_ENT, KC_BSPC, KC_DEL, _______
    ),

// /*
//  * Mouse Layer
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |                              |      | MLeft|MDown | MUp  |MRight|        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |ScLeft|ScDown|ScUp  |ScRght|        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      | LMB  | RMB  | MMB  |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
    [_MOUSE] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______,                                     _______, KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_WH_L, KC_WH_D, KC_WH_U, KC_WH_R, _______,
                                 _______, _______, _______, _______, _______, _______, KC_BTN1, KC_BTN3, KC_BTN2, _______
    ),

// /*
//  * Media Layer
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |      |      |      |      |      |                              |      |  SAI | SAD  |  HUI | HUD  | TOG    |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |                              |      | Prev | VolDn| VolUp| Next |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      | VAI  | VAD  |  MOD | RMOD |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      | Stop | Pause|      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
    [_MEDIA] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     _______, RGB_SAI, RGB_SAD, RGB_HUI, RGB_HUD, RGB_TOG,
      _______, _______, _______, _______, _______, _______,                                     _______, KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RGB_VAI, RGB_VAD, RGB_MOD, RGB_RMOD, _______,
                                 _______, _______, _______, _______, _______, _______, KC_MSTP, KC_MPLY, _______, _______
    ),

// /*
//  * Number layer
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |  [   |   7  |   8  |   9  |   ]  |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |  ;   |   4  |   5  |   6  |   =  |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |  `   |   1  |   2  |   3  |   \  |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |   .  |   0  |   -  |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
    [_NUM] = LAYOUT(
      _______, KC_LBRC, KC_7, KC_8, KC_9, KC_RBRC,                                     _______, _______, _______, _______, _______, _______,
      _______, KC_SCLN, KC_4, KC_5, KC_6, KC_EQL,                                     _______, _______, _______, _______, _______, _______,
      _______, KC_GRV, KC_1, KC_2, KC_3, KC_BSLS, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                _______, KC_DOT, KC_0, KC_MINS,  _______, _______, _______, _______, _______, _______
    ),

// /*
//  * Symbol Layer
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |   {  |   &  |   *  |  (   |   }  |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |   :  |   $  |   %  |   ^   |   +  |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |   ~  |   !  |   @  |   #  |   |  |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |   (  |   )  |   _  |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
    [_SYM] = LAYOUT(
      _______, KC_LCBR, KC_AMPR, KC_ASTR, KC_LPRN, KC_RCBR,                                     _______, _______, _______, _______, _______, _______,
      _______, KC_COLN, KC_DLR, KC_PERC, KC_CIRC, KC_PLUS,                                     _______, _______, _______, _______, _______, _______,
      _______, KC_TILD, KC_EXLM, KC_AT, KC_HASH, KC_PIPE, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 _______, KC_LPRN, KC_RPRN, KC_UNDS, _______, _______, _______, _______, _______, _______
    ),

// /*
//  * Function Layer
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |  F12 |  F7  |  F8  |  F9  | PrtSc|                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |  F11 |  F4  |  F5  |  F6  | ScrLk|                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |  F10 |  F1  |  F2  |  F3  |PauseB|      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      | Menu | Spc  |  Tab |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
    [_FUNC] = LAYOUT(
      _______, KC_F12, KC_F7, KC_F8, KC_F9, KC_PSCR,                                     _______, _______, _______, _______, _______, _______,
      _______, KC_F11, KC_F4, KC_F5, KC_F6, KC_SLCK,                                     _______, _______, _______, _______, _______, _______,
      _______, KC_F10, KC_F1, KC_F2, KC_F3, KC_PAUS, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 _______, KC_APP, KC_SPC, KC_TAB, _______, _______, _______, _______, _______, _______
    ),


// /*
//  * Layer template
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
//     [_LAYERINDEX] = LAYOUT(
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
//     ),
};

#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
	return OLED_ROTATION_180;
}

static void render_boris(void) {
    static const char PROGMEM boris[] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,192, 32, 16,  8,  4,  2,  6,  6, 28, 28, 56,112,224,128,  0,  0,  0,128,128,128,192, 96, 32, 16, 24,  4, 12, 12, 14, 30, 28, 24, 56, 56,240,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 96, 56,  4,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7, 15, 12,  6,  7,  3,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,255, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,192, 48, 30,  3,  0,  0,  0,  0,128,128,128,128,128,192,192,192,224,224,240,240,240,240,248,248,248,248,248,248,248,248,248,248,240,240,224,192,192,128,192,120, 31,  7,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,224,240,255,254,248,252,252,254,255,255,255,255,255,255, 63, 31, 15, 15, 15, 31, 31,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,252,224,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,224,254,255,255,255,255,255,255,255,255,255,255,255,255,255,255,254,252,248,248,252,252,254,255,255,255,255,255,255,241,224,224,192,192,225,255,255,255,255,255,255,255,255,255,255,254,248,224,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,254,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,252,240,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,255,255,255,255,255,255,255,255,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,223,159,159,191,191,191,191,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,252,240,224,224, 64,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  7,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,127,127, 63, 31, 15,  3,  1,  0,  0,  1,  1,  1,  1,  2,  4,  8, 16, 32, 64,192,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    };
    oled_write_raw_P(boris, sizeof(boris));
}

static void render_qmk_logo(void) {
  static const char PROGMEM qmk_logo[] = {
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
    0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
    0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0};

  oled_write_P(qmk_logo, false);
}

static void render_status(void) {
    // QMK Logo and version information
    render_qmk_logo();
    oled_write_P(PSTR("Karol\'s Kyria\n\n"), false);

    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);
    switch (get_highest_layer(layer_state)) {
        case _COLEMAK_DHM:
            if (is_qwerty_active) {
                oled_write_P(PSTR("QWERTY\n"), false);
            } else {
                oled_write_P(PSTR("Colemak-DHm\n"), false);
            }
            break;
        case _NAV:
            oled_write_P(PSTR("Navigation\n"), false);
            break;
        case _MOUSE:
            oled_write_P(PSTR("Mouse\n"), false);
            break;
        case _MEDIA:
            oled_write_P(PSTR("Media\n"), false);
            break;
        case _NUM:
            oled_write_P(PSTR("Number\n"), false);
            break;
        case _SYM:
            oled_write_P(PSTR("Symbols\n"), false);
            break;
        case _FUNC:
            oled_write_P(PSTR("Function\n"), false);
            break;
        default:
            oled_write_P(PSTR("Undefined\n"), false);
    }

    // Host Keyboard LED Status
    uint8_t led_usb_state = host_keyboard_leds();
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_NUM_LOCK) ? PSTR("NUMLCK ") : PSTR("       "), false);
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_CAPS_LOCK) ? PSTR("CAPLCK ") : PSTR("       "), false);
    oled_write_P(IS_LED_ON(led_usb_state, USB_LED_SCROLL_LOCK) ? PSTR("SCRLCK ") : PSTR("       "), false); 
    oled_write_P(PSTR("Have a nice day!    "), false);
}

// disabled for now since there is no QWERTY
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // case QWERTY:
        //     if (record->event.pressed) {
        //         set_single_persistent_default_layer(_QWERTY);
        //         is_qwerty_active = true;
        //     }
        //     return false;
        //     break;
        // case CMAKDH:
        //     if (record->event.pressed) {
        //         set_single_persistent_default_layer(_COLEMAK_DHM);
        //         is_qwerty_active = false;
        //     }
        //     return false;
        case REDO:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("y"));
            }
            return false;
            break;
        case PASTE:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("v"));
            }
            return false;
            break;
        case COPY:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("c"));
            }
            return false;
            break;
        case CUT:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("x"));
            }
            return false;
            break;
        case UNDO:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("z"));
            }
            return false;
            break;
        default:
            break;
    }
    return true;
};

void oled_task_user(void) {
    if (is_keyboard_master()) {
        render_status(); // Renders the current keyboard state (layer, lock, caps, scroll, etc)
    } 
    else {
        render_boris();
    }
}
#endif

#ifdef ENCODER_ENABLE
void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        // Volume control
        if (!clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }
    else if (index == 1) {
        /*
        Right encoder:
        - default - window scroll
        - LOWER - tab change
        - RAISE - window change
        */
        switch (get_highest_layer(layer_state)) {
            case _NAV:
                if (!clockwise) {
                    tap_code16(C(KC_TAB));
                } else {
                    tap_code16(S(C(KC_TAB)));
                }
                break;
            case _MOUSE:
                if (!clockwise) {
                    if (!is_alt_tab_active) {
                        is_alt_tab_active = true;
                        register_code(KC_LALT);
                    }
                    alt_tab_timer = timer_read();
                    tap_code16(KC_TAB);
                } 
                else {
                    alt_tab_timer = timer_read();
                    tap_code16(S(KC_TAB));
                }
                break;
            default:
                if (!clockwise) {
                    tap_code(KC_DOWN);
                    tap_code(KC_DOWN);
                } else {
                    tap_code(KC_UP);
                    tap_code(KC_UP);
                }
                break;
        }
    }
}

void matrix_scan_user(void) {
  if (is_alt_tab_active) {
    if (timer_elapsed(alt_tab_timer) > 900) {
      unregister_code(KC_LALT);
      is_alt_tab_active = false;
    }
  }
}
#endif
