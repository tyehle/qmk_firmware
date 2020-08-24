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
#include <stdio.h>
#include "layers.h"
#include "persistent_hold.h"
#include "multihold.h"

#ifdef RGBLIGHT_ENABLE
// Unused
const rgblight_segment_t PROGMEM no_lights[] = RGBLIGHT_LAYER_SEGMENTS({0, 0, HSV_RED});
// Lower
const rgblight_segment_t PROGMEM lower_lights[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 2, 191, 0xFF, 0xFF}, // purple
    {10, 2, 191, 0xFF, 0xFF}
);
// Raise
const rgblight_segment_t PROGMEM raise_lights[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 2, 93, 0xFF, 0xFF}, // mint (93 looks better than 106 [255*5/12] because of bad LED normalization)
    {10, 2, 93, 0xFF, 0xFF}
);
// Adjust
const rgblight_segment_t PROGMEM adjust_lights[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 2, 0, 0x01, 0xFF}, // white
    {10, 2, 0, 0x01, 0xFF}
);
// Nav
const rgblight_segment_t PROGMEM nav_lights[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 2, 149, 0xFF, 0xFF}, // azure
    {10, 2, 149, 0xFF, 0xFF}
);
// Mod
// TODO: Maybe just one led instead of two?
const rgblight_segment_t PROGMEM lmod_lights[] = RGBLIGHT_LAYER_SEGMENTS(
    {1, 1, 0, 0xFF, 0xFF} // red
);
const rgblight_segment_t PROGMEM rmod_lights[] = RGBLIGHT_LAYER_SEGMENTS(
    {11, 1, 0, 0xFF, 0xFF} // red
);

// Now define the array of layers. Later layers take precedence
const rgblight_segment_t* const PROGMEM layer_lights[] = RGBLIGHT_LAYERS_LIST(
    no_lights,
    lower_lights,
    raise_lights,
    adjust_lights,
    nav_lights,
    lmod_lights,
    rmod_lights
);
#endif

multihold_state_t nav_shift_state = { .timer = 0, .count = 0 };
multihold_state_t raise_shift_state = { .timer = 0, .count = 0 };
multihold_state_t lower_shift_state = { .timer = 0, .count = 0 };
multihold_state_t lctrl_lgui_state = { .timer = 0, .count = 0 };
multihold_state_t rctrl_rgui_state = { .timer = 0, .count = 0 };

uint16_t copy_paste_timer;
enum custom_keycodes {
    KC_LPAB = SAFE_RANGE,
    KC_RPAB,

    KC_NAV_SHIFT,
    KC_RAISE_SHIFT,
    KC_LOWER_SHIFT,

    KC_LCTRL_LGUI,
    KC_RCTRL_RGUI,
};


void keyboard_post_init_user(void) {
#ifdef RGBLIGHT_ENABLE
  rgblight_enable_noeeprom(); // Enables RGB, without saving settings
  rgblight_sethsv_noeeprom(255*1/12, 0xFF, 0x9F); // orange
  rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);

  rgblight_layers = layer_lights;
#endif
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: QWERTY
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |  Alt   |   Q  |   W  |   E  |   R  |   T  |                              |   Y  |   U  |   I  |   O  |   P  |  Alt   |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |Ctrl/GUI|   A  |   S  |  D   |   F  |   G  |                              |   H  |   J  |   K  |   L  | ;  : |Ctrl/GUI|
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  |LShift| Esc  |  | Esc  |RShift|   N  |   M  | ,  < | . >  | /  ? | RShift |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |Raise |Lower | Bksp | Tab  | Nav  |  | Nav  |Enter | Space|Lower |Raise |
 *                        |      |      |      | LMod |      |  |      | RMod |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_QWERTY] = LAYOUT(
      KC_LALT,         KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,                                       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_RALT,
      KC_LCTRL_LGUI,   KC_A,   KC_S,   KC_D,   KC_F,   KC_G,                                       KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_RCTRL_RGUI,
      KC_LSHIFT,       KC_Z,   KC_X,   KC_C,   KC_V,   KC_B, KC_LSHIFT, KC_ESC, KC_ESC, KC_RSHIFT, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSHIFT,
      KC_RAISE_SHIFT, KC_LOWER_SHIFT, KC_BSPC, LT(_LMOD, KC_TAB), KC_NAV_SHIFT, KC_NAV_SHIFT, LT(_RMOD, KC_ENT), KC_SPC, KC_LOWER_SHIFT, KC_RAISE_SHIFT
    ),


/*
 * Lower Layer: Symbols
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  !   |  @   | [ {  | ] }  |  |   |                              |  |   |  _   |  &   |  \   |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  #   |  $   | ( <  | ) >  |  `   |                              |  +   |  -   |  *   |  /   |  %   |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  %   |  ^   | ' "  | ' "  |  ~   |      |      |  |      |      |  ;   |  =   |  !   |  ?   |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      | Del  |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_LOWER] = LAYOUT(
      _______, KC_EXLM, KC_AT,   KC_LBRC, KC_RBRC, KC_PIPE,                                     KC_PIPE, KC_UNDS, KC_AMPR, KC_BSLS, _______, _______,
      _______, KC_HASH, KC_DLR,  KC_LPAB, KC_RPAB, KC_GRV,                                      KC_PLUS, KC_MINS, KC_ASTR, KC_SLSH, KC_PERC, _______,
      _______, KC_PERC, KC_CIRC, KC_QUOT, KC_QUOT, KC_TILD, _______, _______, _______, _______, KC_SCLN, KC_EQL,  KC_EXLM, KC_QUES, _______, _______,
                                 _______, _______, KC_DEL,  _______, _______, _______, _______, _______, _______, _______
    ),


/*
 * Raise Layer: Number keys, function keys
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  F1  |  F2  |  F3  |  F4  |  F5  |                              |  F6  |  F7  |  F8  |  F9  | F10  |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  1   |  2   |  3   |  4   |  5   |                              |  6   |  7   |  8   |  9   |  0   |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        | F11  | F12  | F13  | F14  | F15  |      |      |  |      |      | F16  | F17  | F18  | F19  | F20  |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      | Del  |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_RAISE] = LAYOUT(
      _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                       KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  _______,
      _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
      _______, KC_F11,  KC_F12,  KC_F13,  KC_F14,  KC_F15,  _______, _______, _______, _______, KC_F16,  KC_F17,  KC_F18,  KC_F19,  KC_F20,  _______,
                                 _______, _______, KC_DEL,  _______, _______, _______, _______, _______, _______, _______
    ),


/*
 * Adjust Layer: Mouse, scroll, RGB
 * TODO
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      | 🖱️ L | 📜 ^ | 🖱️ R | Loud |                              |Bright| 🖱️ L | 🖱️ ^ | 🖱️ R |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      | 📜 < | 📜 v | 📜 > |Quiet |                              | Dark | 🖱️ < | 🖱️ v | 🖱️ > |      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      | Mute |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_ADJUST] = LAYOUT(
      _______, _______, KC_BTN1, KC_WH_U, KC_BTN2, KC_VOLU,                                     KC_BRIU, KC_BTN1, KC_MS_U, KC_BTN2, _______, _______,
      _______, _______, KC_WH_L, KC_WH_D, KC_WH_R, KC_VOLD,                                     KC_BRID, KC_MS_L, KC_MS_D, KC_MS_R, _______, _______,
      _______, _______, _______, _______, _______, KC_MUTE, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),


/*
 * Nav Layer: Move the cursor, position windows, and change desktops
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      | CA ^ |      |      |                              |      |      |  ^   |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        | GUI  | CA < | CA v | CA > |      |                              | Home |  <   |  v   |  >   | End  |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |Alt ⇆ |Cmd ⇆ |Ctl ⇆ |      |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_NAV] = LAYOUT(
      _______, _______,      _______, LCA(KC_UP),   _______,       _______,                                     _______, _______, KC_UP,   _______, _______, _______,
      _______, KC_LGUI, LCA(KC_LEFT), LCA(KC_DOWN), LCA(KC_RIGHT), _______,                                     KC_HOME, KC_LEFT, KC_DOWN, KC_RIGHT,KC_END,  _______,
      _______, _______, A(KC_TAB),    G(KC_TAB),    C(KC_TAB),     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                      _______,      _______,       _______,  _______, _______, _______, _______, _______, _______, _______
    ),


/*
 * Left modifier layer
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      | LGUI | LCTL |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      |      | LALT | LSFT |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_LMOD] = LAYOUT(
      _______, _______, _______, KC_LGUI, KC_LCTRL,  _______,                                     _______, _______, _______, _______, _______, _______,
      _______, _______, _______, KC_LALT, KC_LSHIFT, _______,                                     _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 _______, _______,   _______, _______, _______, _______, _______, _______, _______, _______
    ),


/*
 * Right modifier layer
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              |      | RCTL | RGUI |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |                              |      | RSFT | RALT |      |      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_RMOD] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     _______, KC_RCTRL,  KC_RGUI, _______, _______, _______,
      _______, _______, _______, _______, _______, _______,                                     _______, KC_RSHIFT, KC_RALT, _______, _______, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,   _______, _______, _______, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______,   _______
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

layer_state_t layer_state_set_user(layer_state_t state) {
    state = update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
    // rgblight_set_layer_state(0, layer_state_cmp(state, 0));
    rgblight_set_layer_state(1, layer_state_cmp(state, 1));
    rgblight_set_layer_state(2, layer_state_cmp(state, 2));
    rgblight_set_layer_state(3, layer_state_cmp(state, 3));
    rgblight_set_layer_state(4, layer_state_cmp(state, 4));
    rgblight_set_layer_state(5, layer_state_cmp(state, 5));
    rgblight_set_layer_state(6, layer_state_cmp(state, 6));
    rgblight_set_layer_state(7, layer_state_cmp(state, 7));
    return state;
}

bool process_shifted(keyrecord_t *record, uint16_t normal, uint16_t shifted) {
    if(record->event.pressed) {
        if(keyboard_report->mods & MOD_BIT(KC_LSFT) || keyboard_report->mods & MOD_BIT(KC_RSFT)) {
            // register_code16(shifted);
            tap_code16(shifted);
        } else {
            // register_code16(normal);
            tap_code16(normal);
        }
    } else {
        // if(keyboard_report->mods & MOD_BIT(KC_LSFT) || keyboard_report->mods & MOD_BIT(KC_RSFT)) {
        //     unregister_code16(shifted);
        // } else {
        //     unregister_code16(normal);
        // }
    }

    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_NAV_SHIFT:
            layer_mod_event(record, _NAV, KC_LSHIFT, &nav_shift_state);
            return false;
        case KC_RAISE_SHIFT:
            layer_mod_event(record, _RAISE, KC_LSHIFT, &raise_shift_state);
            return false;
        case KC_LOWER_SHIFT:
            layer_mod_event(record, _LOWER, KC_LSHIFT, &lower_shift_state);
            return false;

        case KC_LCTRL_LGUI:
            mod_mod_event(record, KC_LCTRL, KC_LGUI, &lctrl_lgui_state);
            return false;
        case KC_RCTRL_RGUI:
            mod_mod_event(record, KC_RCTRL, KC_RGUI, &rctrl_rgui_state);
            return false;

        // Parens and angle brackets
        case KC_LPAB:
            return process_shifted(record, KC_LEFT_PAREN, KC_LEFT_ANGLE_BRACKET);

        case KC_RPAB:
            return process_shifted(record, KC_RIGHT_PAREN, KC_RIGHT_ANGLE_BRACKET);

    }
    return true;
}

#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
	return OLED_ROTATION_180;
}

static void render_kyria_logo(void) {
    static const char PROGMEM kyria_logo[] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,192,224,240,112,120, 56, 60, 28, 30, 14, 14, 14,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 14, 14, 14, 30, 28, 60, 56,120,112,240,224,192,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,192,224,240,124, 62, 31, 15,  7,  3,  1,128,192,224,240,120, 56, 60, 28, 30, 14, 14,  7,  7,135,231,127, 31,255,255, 31,127,231,135,  7,  7, 14, 14, 30, 28, 60, 56,120,240,224,192,128,  1,  3,  7, 15, 31, 62,124,240,224,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,240,252,255, 31,  7,  1,  0,  0,192,240,252,254,255,247,243,177,176, 48, 48, 48, 48, 48, 48, 48,120,254,135,  1,  0,  0,255,255,  0,  0,  1,135,254,120, 48, 48, 48, 48, 48, 48, 48,176,177,243,247,255,254,252,240,192,  0,  0,  1,  7, 31,255,252,240,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,255,255,255,  0,  0,  0,  0,  0,254,255,255,  1,  1,  7, 30,120,225,129,131,131,134,134,140,140,152,152,177,183,254,248,224,255,255,224,248,254,183,177,152,152,140,140,134,134,131,131,129,225,120, 30,  7,  1,  1,255,255,254,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0,255,255,  0,  0,192,192, 48, 48,  0,  0,240,240,  0,  0,  0,  0,  0,  0,240,240,  0,  0,240,240,192,192, 48, 48, 48, 48,192,192,  0,  0, 48, 48,243,243,  0,  0,  0,  0,  0,  0, 48, 48, 48, 48, 48, 48,192,192,  0,  0,  0,  0,  0,
        0,  0,  0,255,255,255,  0,  0,  0,  0,  0,127,255,255,128,128,224,120, 30,135,129,193,193, 97, 97, 49, 49, 25, 25,141,237,127, 31,  7,255,255,  7, 31,127,237,141, 25, 25, 49, 49, 97, 97,193,193,129,135, 30,120,224,128,128,255,255,127,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0, 63, 63,  3,  3, 12, 12, 48, 48,  0,  0,  0,  0, 51, 51, 51, 51, 51, 51, 15, 15,  0,  0, 63, 63,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 48, 48, 63, 63, 48, 48,  0,  0, 12, 12, 51, 51, 51, 51, 51, 51, 63, 63,  0,  0,  0,  0,  0,
        0,  0,  0,  0, 15, 63,255,248,224,128,  0,  0,  3, 15, 63,127,255,239,207,141, 13, 12, 12, 12, 12, 12, 12, 12, 30,127,225,128,  0,  0,255,255,  0,  0,128,225,127, 30, 12, 12, 12, 12, 12, 12, 12, 13,141,207,239,255,127, 63, 15,  3,  0,  0,128,224,248,255, 63, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  3,  7, 15, 62,124,248,240,224,192,128,  1,  3,  7, 15, 30, 28, 60, 56,120,112,112,224,224,225,231,254,248,255,255,248,254,231,225,224,224,112,112,120, 56, 60, 28, 30, 15,  7,  3,  1,128,192,224,240,248,124, 62, 15,  7,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  3,  7, 15, 14, 30, 28, 60, 56,120,112,112,112,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,112,112,112,120, 56, 60, 28, 30, 14, 15,  7,  3,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
    };
    oled_write_raw_P(kyria_logo, sizeof(kyria_logo));
}

static void render_qmk_logo(void) {
  static const char PROGMEM qmk_logo[] = {
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
    0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
    0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0};

  oled_write_P(qmk_logo, false);
}

void oled_render_mod_state(uint8_t modifiers) {
  oled_write_P(PSTR("\nMods: "), false);
  oled_write_P(PSTR("SHF"), (modifiers & MOD_MASK_SHIFT));
  oled_write_P(PSTR(" "), false);
  oled_write_P(PSTR("CTL"), (modifiers & MOD_MASK_CTRL));
  oled_write_P(PSTR(" "), false);
  oled_write_P(PSTR("ALT"), (modifiers & MOD_MASK_ALT));
  oled_write_P(PSTR(" "), false);
  oled_write_P(PSTR("GUI"), (modifiers & MOD_MASK_GUI));
}

static void render_status(void) {
    // QMK Logo and version information
    render_qmk_logo();
    oled_write_P(PSTR("         Kyria\n\n"), false);

    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);
    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
            oled_write_P(PSTR("Default\n"), false);
            break;
        case _LOWER:
            oled_write_P(PSTR("Lower\n"), false);
            break;
        case _RAISE:
            oled_write_P(PSTR("Raise\n"), false);
            break;
        case _ADJUST:
            oled_write_P(PSTR("Adjust\n"), false);
            break;
        case _NAV:
            oled_write_P(PSTR("Navigation\n"), false);
            break;
        case _LMOD:
            oled_write_P(PSTR("Left Mods\n"), false);
            break;
        case _RMOD:
            oled_write_P(PSTR("Right Mods\n"), false);
            break;
        default:
            oled_write_P(PSTR("Undefined\n"), false);
    }

    // debugging
    // char s[10];
    // snprintf(s, 10, "(%d, %d)", last_row, last_col);
    // oled_write_ln(s, false);

    oled_render_mod_state(get_mods()|get_oneshot_mods());

    // Host Keyboard LED Status
    // uint8_t led_usb_state = host_keyboard_leds();
    // oled_write_P(IS_LED_ON(led_usb_state, USB_LED_NUM_LOCK) ? PSTR("NUMLCK ") : PSTR("       "), false);
    // oled_write_P(IS_LED_ON(led_usb_state, USB_LED_CAPS_LOCK) ? PSTR("CAPLCK ") : PSTR("       "), false);
    // oled_write_P(IS_LED_ON(led_usb_state, USB_LED_SCROLL_LOCK) ? PSTR("SCRLCK ") : PSTR("       "), false);
}

void oled_task_user(void) {
    if (is_keyboard_master()) {
        render_status(); // Renders the current keyboard state (layer, lock, caps, scroll, etc)
    } else {
        render_kyria_logo();
    }
}
#endif

#ifdef ENCODER_ENABLE
void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        // Volume control
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }
    else if (index == 1) {
        // Page up/Page down
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    }
}
#endif
