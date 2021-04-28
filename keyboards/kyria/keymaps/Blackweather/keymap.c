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
    _GAME,
    _RSTHD,
    _NAV,
    _MOUSE,
    _MEDIA,
    _NUM,
    _NUMT,
    _SYM,
    _FUNC
};

enum custom_keycodes {
    GAME = SAFE_RANGE,
    RSTHD,
    CMAKDH,
    DSCRD,
    GMAIL,
    MSNGR,
    CALC,
    SLCK,
    OUTLK
};

// Tap Dance declarations
enum {
    TD_CHRM_FFX,
    TD_Z_EMOJI
};

void browser_dance_finished(qk_tap_dance_state_t* state, void* user_data) {
    if (state->count == 1) {
        SEND_STRING(SS_TAP(X_LGUI)SS_DELAY(700)"chrome"SS_DELAY(500)SS_TAP(X_ENT));
    } else if (state->count == 2) {
        SEND_STRING(SS_TAP(X_LGUI)SS_DELAY(700)"firefox"SS_DELAY(500)SS_TAP(X_ENT));
    }
}

void emoji_dance_finished(qk_tap_dance_state_t* state, void* user_data) {
    if (state->count == 1) {
        register_code(KC_Z);
    } else if (state->count == 2) {
        SEND_STRING(":sweat_smile:");
    }
}

void emoji_dance_reset(qk_tap_dance_state_t* state, void* user_data) {
    if (state->count == 1) {
        unregister_code(KC_Z);
    }
}

// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_CHRM_FFX] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, browser_dance_finished, NULL),
    [TD_Z_EMOJI] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, emoji_dance_finished, emoji_dance_reset),
};

// combos
enum combo_events {
    PB_LPRN,
    JL_RPRN
};

const uint16_t PROGMEM pb_combo[] = {KC_P, KC_B, COMBO_END};
const uint16_t PROGMEM jl_combo[] = {KC_J, KC_L, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {
    [PB_LPRN] = COMBO(pb_combo, KC_LPRN),
    [JL_RPRN] = COMBO(jl_combo, KC_RPRN)
};

bool is_game_active = false;
bool is_rsthd_active = false;

bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: COLEMAK_DHM GACS
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |Chrm/Ffx|   Q  |   W  |   F  |   P  |   B  |                              |   J  |   L  |   U  |   Y  | ' "  |  Calc  |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * | GMAIL | GUI/A| Alt/R|Ctrl/S|LSft/T|   G  |                              |   M  |LSft/N|Ctrl/E| Alt/I| GUI/O |  Slck  |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | MSNGR |   Z  |ALGR/X|   C  |   D  |   V  | Dscrd| GAME |   | RSTHD|XXXXXX|   K  |   H  | ,  < |ALGR.>| /  ? |  Outlk  |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        | GUI  | Esc  | Space| Tab  | XXX  |  | XXXXX| Enter| Bksp |Delete| Mute |
 *                        |      | Media| Nav  |      | XXXXX|  | XXXXX| Sym  | Num |  Fn  |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_COLEMAK_DHM] = LAYOUT(
      TD(TD_CHRM_FFX), KC_Q, KC_W, KC_F, KC_P, KC_B, KC_J, KC_L, KC_U, KC_Y, KC_QUOT, CALC,
      GMAIL, MT(MOD_LGUI,KC_A), MT(MOD_LALT, KC_R), MT(MOD_LCTL, KC_S), MT(MOD_LSFT, KC_T), KC_G, KC_M, MT(MOD_RSFT, KC_N), MT(MOD_RCTL, KC_E), MT(MOD_LALT, KC_I), MT(MOD_RGUI, KC_O), SLCK,
      MSNGR, TD(TD_Z_EMOJI), MT(MOD_RALT, KC_X), KC_C, KC_D, KC_V, DSCRD, GAME, RSTHD, KC_NO, KC_K, KC_H, KC_COMM, MT(MOD_RALT, KC_DOT), KC_SLSH, OUTLK,
      KC_LGUI, LT(_MEDIA, KC_ESC), LT(_NAV, KC_SPC), KC_TAB, KC_NO, KC_NO, LT(_SYM, KC_ENT), LT(_NUM, KC_BSPC), LT(_FUNC, KC_DEL), KC_MUTE
    ),

/*
 * Gaming layout: GAME
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * | XXX    |  Esc  |  Q  |   W  |   E  |  R   |                              |   T  |   Y  |   U  |   I  |   O  |  XXXX  |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |  Nav   |LShift|   A  |   S  |  D   |  F   |                              |   G  |   H  |  J   |   K  |   L  |  XXXX  |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | XXXXXX |LCtrl |   Z  |   X  |   C  |  V   |XXXXXX|CMAKDH|  |  XXX |XXXXXX|   B  |   N  |  M   |  , < |   P  |  XXX   |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |  GUI | Alt  | Space| NUMT | Esc  |  | Enter| Bksp | Sym | /?    | Mute |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_GAME] = LAYOUT(
      _______,  KC_ESC,  KC_Q, KC_W,    KC_E,    KC_R,                                       KC_T, KC_Y, KC_U, KC_I, KC_O, _______,
      MO(_NAV), KC_LSFT, KC_A, KC_S,    KC_D,    KC_F,                                       KC_G, KC_H, KC_J, KC_K, KC_L, _______,
      _______,  KC_LCTL, KC_Z, KC_X,    KC_C,    KC_V,   _______, CMAKDH,  _______, _______, KC_B, KC_N, KC_M, KC_COMM, KC_P, _______,
                              KC_LGUI, KC_LALT, KC_SPC,  MO(_NUMT), KC_ESC, KC_ENT, KC_BSPC, MO(_SYM), KC_SLSH, KC_MUTE
    ),

/*
 * Alternative layer: RSTHD
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |Chrm/Ffx|   J  |   C  |   Y  |   F  |   K  |                              |   Z  |   L  |  , < |   U  |   Q  |  Calc  |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * | GMAIL | GUI/R| Alt/S|Ctrl/T|LSft/H|   D   |                              |   M  |LSft/N|Ctrl/A| Alt/I| GUI/O|  Slck  |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | MSNGR |  / ? |ALGR/V |   G  |   P  |   B  | Dscrd| XXXX |  | CMAKDH|XXXXXX|  X  |   W  | .  > |ALGR;:| -  _ |  Outlk |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        | GUI  | Esc  |   E  | Bksp | Tab  |  | XXXXX| Enter|Space|Delete| Mute |
 *                        |      | Media| Nav  |      |      |  | XXXXX| Sym  | Num |  Fn  |      |
 *                        `----------------------------------'  `----------------------------------'
 */

    [_RSTHD] = LAYOUT(
      TD(TD_CHRM_FFX), KC_J, KC_C, KC_Y, KC_F, KC_K,        KC_Z, KC_L, KC_COMM, KC_U, KC_Q, CALC,
      GMAIL, MT(MOD_LGUI,KC_R), MT(MOD_LALT, KC_S), MT(MOD_LCTL, KC_T), MT(MOD_LSFT, KC_H), KC_D,       KC_M, MT(MOD_RSFT, KC_N), MT(MOD_RCTL, KC_A), MT(MOD_LALT, KC_I), MT(MOD_RGUI, KC_O), SLCK,
      MSNGR, KC_SLSH, MT(MOD_RALT, KC_V), KC_G, KC_P, KC_B, DSCRD, KC_NO,      CMAKDH, KC_NO, KC_X, KC_W, KC_DOT, MT(MOD_RALT, KC_SCLN), KC_MINS, OUTLK,
                                 KC_LGUI, LT(_MEDIA, KC_ESC), LT(_NAV, KC_E), KC_BSPC, KC_TAB,       KC_NO, LT(_SYM, KC_ENT), LT(_NUM, KC_SPC), LT(_FUNC, KC_DEL), KC_MUTE
    ),

/*
 * Navigation Layer
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |                              | Caps | Left | Down |  Up  | Right|        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |      |  |      |      |Insert| Home | PgDn | PgUp | End  |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      | Enter| Bksp |Delete|      |
 *                        `----------------------------------'  `----------------------------------'
 */

     [_NAV] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______,                                     KC_CAPS, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_INS, KC_HOME, KC_PGDN, KC_PGUP, KC_END, _______,
                                 _______, _______, _______, _______, _______, _______, KC_ENT, KC_BSPC, KC_DEL, _______
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
//  *                        `----------------------------------'  `----------------------------------'
//  */
    [_NUM] = LAYOUT(
      _______, KC_LBRC, KC_7, KC_8, KC_9, KC_RBRC,                                     _______, _______, _______, _______, _______, _______,
      _______, KC_SCLN, KC_4, KC_5, KC_6, KC_EQL,                                     _______, _______, _______, _______, _______, _______,
      _______, KC_GRV, KC_1, KC_2, KC_3, KC_BSLS, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                _______, KC_DOT, KC_0, KC_MINS,  _______, _______, _______, _______, _______, _______
    ),

// /*
//  * Top Number layer
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |   1  |   2  |   3  |   4  |  5   |                              |   6  |  7   |   8  |   9  |   0  |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
    [_NUMT] = LAYOUT(
      _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
      _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
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
            if (is_game_active) {
                oled_write_P(PSTR("Game\n"), false);
            } 
            else if (is_rsthd_active) {
                oled_write_P(PSTR("RSTHD\n"), false);
            }
            else {
                oled_write_P(PSTR("Colemak-DHm\n"), false);
            }
            break;
        case _NAV:
            oled_write_P(PSTR("Navigation\n"), false);
            break;
        case _MEDIA:
            oled_write_P(PSTR("Media\n"), false);
            break;
        case _NUM:
            oled_write_P(PSTR("Number\n"), false);
            break;
        case _NUMT:
            oled_write_P(PSTR("NumberT\n"), false);
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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case GAME:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_GAME);
                is_game_active = true;
            }
            return false;
        case RSTHD:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_RSTHD);
                is_rsthd_active = true;
            }
            return false;
        case CMAKDH:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_COLEMAK_DHM);
                is_game_active = false;
                is_rsthd_active = false;
            }
            return false;
        case DSCRD:
            if (record->event.pressed) {
                SEND_STRING(SS_TAP(X_LGUI)SS_DELAY(700)"discord"SS_DELAY(500)SS_TAP(X_ENT));
            }
            return false;
        case GMAIL:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("t")"gmail.com"SS_TAP(X_ENT));
            }
            return false;
        case MSNGR:
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("t")"messenger.com"SS_TAP(X_ENT));
            }
            return false;
        case CALC:
            if (record->event.pressed) {
                SEND_STRING(SS_TAP(X_LGUI)SS_DELAY(700)"calculator"SS_DELAY(500)SS_TAP(X_ENT));
            }
            return false;
        case SLCK:
            if (record->event.pressed) {
                SEND_STRING(SS_TAP(X_LGUI)SS_DELAY(700)"slack"SS_DELAY(500)SS_TAP(X_ENT));
            }
            return false;
        case OUTLK:
            if (record->event.pressed) {
                SEND_STRING(SS_TAP(X_LGUI)SS_DELAY(700)"outlook"SS_DELAY(500)SS_TAP(X_ENT));
            }
            return false;
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
        if (is_game_active) {
            // weapon switch
            if (!clockwise) {
                    tap_code(KC_WH_D);
                } 
                else {
                    tap_code(KC_WH_U);
                }
        } 
        else {
            // Volume control
            if (!clockwise) {
                    tap_code(KC_VOLU);
                } 
                else {
                    tap_code(KC_VOLD);
                }
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
            case _MEDIA:
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
                    tap_code(KC_WH_D);
                } else {
                    tap_code(KC_WH_U);
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
