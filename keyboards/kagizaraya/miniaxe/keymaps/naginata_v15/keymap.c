/* Copyright 2018-2019 eswai <@eswai>
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
#include "os_detection.h"
#include "keymap_japanese.h"

// 薙刀式
#include "naginata.h"
NGKEYS naginata_keys;
#define NAGINATA_TIMEOUT 30000
// 薙刀式

enum layer_names {
  _QWERTY,  
  // 薙刀式
  _NAGINATA, // 薙刀式入力レイヤー
  // 薙刀式
  _CTRL,
  _SYM1,
  _SYM2,
  _NUM,
  _FNC,
  _OTHR
};

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    KANAON = NG_SAFE_RANGE,
    EISUON,
};

static deferred_token my_token;

#define CTLSPC CTL_T(KC_SPC)
#define SFTENT SFT_T(KC_ENT)
#define COPY LCTL(KC_C)
#define CUT LCTL(KC_X)
#define PASTE LCTL(KC_V)
#define FIND LCTL(KC_F)
#define SELALL LCTL(KC_A)
#define BACK LCTL(KC_Z)
#define SAVE LCTL(KC_S)
#define REDO LCTL(KC_Y)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_QWERTY] = LAYOUT_split_3x5_3(
  KC_Q,    KC_W,    KC_E,    KC_R,      KC_T,      KC_Y,    KC_U,     KC_I,    KC_O,    KC_P,
  KC_A,    KC_S,    KC_D,    KC_F,      KC_G,      KC_H,    KC_J,     KC_K,    KC_L,    KC_BSPC,
  KC_Z,    KC_X,    KC_C,    KC_V,      KC_B,      KC_N,    KC_M,     KC_COMM, KC_DOT,  KC_ESC,
                    KC_LGUI, MO(_CTRL), CTLSPC,    SFTENT,  MO(_NUM), KC_RALT
),

[_CTRL] = LAYOUT_split_3x5_3(
  SELALL,  CUT,     COPY,      PASTE,     XXXXXXX,  KC_PGUP, KC_PGDN, KC_HOME, KC_END,  JP_AT,
  KC_LGUI, KC_LALT, KC_LCTL,   KC_LSFT,   XXXXXXX,  KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, JP_SCLN,
  BACK,    REDO,    MO(_SYM2), MO(_SYM1), XXXXXXX,  KC_CAPS, KC_TAB,  KC_DEL,  JP_UNDS, JP_COLN,
                    _______,   _______,   _______,  _______, _______, _______
),

[_SYM1] = LAYOUT_split_3x5_3(
  _______, _______, _______, _______, _______,     JP_EXLM, JP_DQUO, JP_HASH, JP_DLR,  JP_PERC,
  _______, _______, _______, _______, _______,     JP_AMPR, JP_QUOT, JP_EQL,  JP_TILD, JP_PIPE,
  _______, _______, _______, _______, _______,     JP_GRV,  JP_CIRC, JP_QUES, JP_SLSH, JP_BSLS,
                    _______, _______, _______,     _______, _______, _______
),

[_SYM2] = LAYOUT_split_3x5_3(
  _______, _______, _______, _______, _______,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  _______, _______, _______, _______, _______,     XXXXXXX, JP_LPRN, JP_LBRC, JP_LCBR, JP_LABK,
  _______, _______, _______, _______, _______,     XXXXXXX, JP_RPRN, JP_RBRC, JP_RCBR, JP_RABK,
                    _______, _______, _______,     _______, _______, _______
),

[_NUM] =  LAYOUT_split_3x5_3(
  JP_ASTR, KC_7,    KC_8,    KC_9,    JP_MINS,    XXXXXXX,  XXXXXXX,   XXXXXXX,   XXXXXXX, XXXXXXX,
  JP_SLSH, KC_4,    KC_5,    KC_6,    JP_PLUS,    XXXXXXX,  KC_RSFT,   KC_RCTL,   KC_RALT, KC_RGUI,
  KC_0,    KC_1,    KC_2,    KC_3,    JP_DOT,     XXXXXXX,  MO(_FNC),  MO(_OTHR), XXXXXXX, XXXXXXX,
                    _______, _______, _______,    _______,   _______, _______
),


[_FNC] =  LAYOUT_split_3x5_3(
  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,      _______, _______, _______, _______, _______,
  KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,     _______, _______, _______, _______, _______,
  KC_F11,  KC_F12,  XXXXXXX, XXXXXXX, XXXXXXX,    _______, _______, _______, _______, _______,
                    _______, _______, _______,    _______, _______, _______
),


[_OTHR] =  LAYOUT_split_3x5_3(
  NGSW_MAC, NGSW_WIN, NG_SHOS, NG_TAYO, XXXXXXX,   _______, _______, _______, _______, _______,
  KC_INS,   KC_PSCR,  KC_SCRL, KC_NUM,  XXXXXXX,   _______, _______, _______, _______, _______,
  JP_KANA,  JP_ZKHK,  JP_MHEN, JP_HENK, XXXXXXX,   _______, _______, _______, _______, _______,
                      _______, _______, _______,   _______, _______, _______
),



// 薙刀式
[_NAGINATA] = LAYOUT_split_3x5_3(
  NG_Q,   NG_W,   NG_E,    NG_R,    NG_T,          NG_Y,     NG_U,    NG_I,    NG_O,   NG_P, 
  NG_A,   NG_S,   NG_D,    NG_F,    NG_G,          NG_H,     NG_J,    NG_K,    NG_L,   NG_SCLN,
  NG_Z,   NG_X,   NG_C,    NG_V,    NG_B,          NG_N,     NG_M,    NG_COMM, NG_DOT, NG_SLSH,
                  _______, _______, NG_SHFT,       NG_SHFT2, _______, _______
),
// 薙刀式
};

uint32_t kanaoff(uint32_t trigger_time, void *cb_arg) {
    if (naginata_state()) naginata_off();
    return NAGINATA_TIMEOUT;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  extend_deferred_exec(my_token, NAGINATA_TIMEOUT);

  switch (keycode) {
    case EISUON:
      if (record->event.pressed) {
        // 薙刀式
        naginata_off();
        // 薙刀式
      }
      return false;
      break;
    case KANAON:
      if (record->event.pressed) {
        // 薙刀式
        naginata_on();
        // 薙刀式
      }
      return false;
      break;
  }

  // 薙刀式
  if (!process_naginata(keycode, record))
    return false;
  // 薙刀式

  return true;
}

void keyboard_post_init_user(void) {
    // 薙刀式
    uint16_t ngonkeys[]  = {KC_H, KC_J};
    uint16_t ngoffkeys[] = {KC_F, KC_G};
    set_naginata(_NAGINATA, ngonkeys, ngoffkeys);
    // 薙刀式

    wait_ms(400);
    switch (detected_host_os()) {
        case OS_WINDOWS:
            switchOS(NG_WIN);
            break;
        case OS_MACOS:
        case OS_IOS:
            switchOS(NG_MAC);
            break;
        case OS_LINUX:
            switchOS(NG_LINUX);
            break;
        default:
            switchOS(NG_WIN);
    }
    my_token = defer_exec(NAGINATA_TIMEOUT, kanaoff, NULL);
}
