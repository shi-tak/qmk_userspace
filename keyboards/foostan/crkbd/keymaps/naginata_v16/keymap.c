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
 #include "twpair_on_jis.h"
 #include "keymap_japanese.h"
 
 // 薙刀式
 #include "naginata.h"
 NGKEYS naginata_keys;
 // 薙刀式
 #define NAGINATA_TIMEOUT 30000
 
 enum layer_names {
   _WIN,
   _MAC,
   // 薙刀式
   _NAGINATA, // 薙刀式入力レイヤー
   // 薙刀式
   _LWR,
   _RS,
   _ADJ
 };
 
 // Defines the keycodes used by our macros in process_record_user
 enum custom_keycodes {
     KANAON = NG_SAFE_RANGE,
     EISUON,
 };
 
 static deferred_token my_token;

 const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
 
 [_WIN] = LAYOUT_split_3x6_3_ex2(
   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,     KC_T,   KC_CAPS,     KC_ESC,  KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_AT,
   KC_LSFT, KC_A,    KC_S,    KC_D,    KC_F,     KC_G,   KC_DEL,      KC_BSPC, KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_COLN,
   KC_LCTL, KC_Z,    KC_X,    KC_C,    KC_V,     KC_B,                         KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_BSLS, KC_SLSH,
                              KC_LGUI, MO(_LWR), KC_SPC,                       KC_ENT,  MO(_RS),  KC_RALT
 ),

 [_MAC] = LAYOUT_split_3x6_3_ex2(
    KC_TAB, KC_Q,     KC_W,   KC_E,    KC_R,     KC_T,   KC_CAPS,     KC_ESC,  KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_AT,
    KC_LSFT, KC_A,    KC_S,   KC_D,    KC_F,     KC_G,   KC_DEL,      KC_BSPC, KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_COLN,
    KC_LCTL, KC_Z,    KC_X,   KC_C,    KC_V,     KC_B,                         KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_BSLS, KC_SLSH,
                              KC_LGUI, MO(_LWR),   KC_SPC,                       KC_ENT,  MO(_RS),  KC_RALT
 ),
 
 [_LWR] = LAYOUT_split_3x6_3_ex2(
   _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,     _______, _______, _______, _______, _______, _______, _______,
   _______, XXXXXXX, KC_LABK, KC_LCBR, KC_LBRC, KC_LPRN, _______,     _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, _______,
   _______, XXXXXXX, KC_RABK, KC_RCBR, KC_RBRC, KC_RPRN,                       KC_HOME, KC_PGUP, KC_PGDN, KC_END,  _______, _______,
                              _______, _______, _______,                       _______, _______, _______
 ),

 [_RS] = LAYOUT_split_3x6_3_ex2(
   _______, KC_ASTR, KC_7,    KC_8,    KC_9,    KC_MINS, _______,     _______, KC_EXLM, KC_DQUO, KC_HASH, KC_DLR,  KC_PERC, _______,
   _______, KC_SLSH, KC_4,    KC_5,    KC_6,    KC_PLUS, _______,     _______, KC_AMPR, KC_QUOT, KC_EQL,  KC_TILD, KC_PIPE, _______,
   _______, KC_0,    KC_1,    KC_2,    KC_3,    KC_DOT,                        KC_GRV,  KC_CIRC, KC_QUES, KC_UNDS, _______, _______,
                              _______, _______, _______,                       _______,  _______, _______
 ),
 
 [_ADJ] =  LAYOUT_split_3x6_3_ex2(
   NGSW_LNX, XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX, QK_BOOT, _______,     _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   _______,
   NGSW_MAC, KC_INS,   KC_PSCR,  KC_SCRL, KC_NUM,  NG_TAYO, _______,     _______, KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  _______,
   NGSW_WIN, JP_KANA,  JP_ZKHK,  JP_MHEN, JP_HENK, NG_SHOS,                       KC_F11,  KC_F12,  XXXXXXX, XXXXXXX, XXXXXXX, _______,
                              _______, _______, _______,                          _______, _______, _______
 ),

 // 薙刀式
 [_NAGINATA] = LAYOUT_split_3x6_3_ex2(
   _______, NG_Q,     NG_W,     NG_E,    NG_R,    NG_T,     _______,     _______, NG_Y,     NG_U,    NG_I,    NG_O,   NG_P,    _______,
   _______, NG_A,     NG_S,     NG_D,    NG_F,    NG_G,     _______,     _______, NG_H,     NG_J,    NG_K,    NG_L,   NG_SCLN, _______,
   _______, NG_Z,     NG_X,     NG_C,    NG_V,    NG_B,                           NG_N,     NG_M,    NG_COMM, NG_DOT, NG_SLSH, _______,
                                _______, _______, NG_SHFT,                        NG_SHFT2, _______, _______
 ),
 // 薙刀式
 };
 
 uint32_t kanaoff(uint32_t trigger_time, void *cb_arg) {
     if (naginata_state())
      naginata_off();
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
 
   // typewriter pairing on jis keyboard
   if (!twpair_on_jis(keycode, record))
     return false;

   return true;
 }
 
 layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LWR, _RS, _ADJ);
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
          layer_move(_WIN);
          switchOS(NG_WIN);
          break;
        case OS_MACOS:
        case OS_IOS:
          layer_move(_MAC);
          switchOS(NG_MAC);
          break;
        case OS_LINUX:
          layer_move(_WIN);
          switchOS(NG_LINUX);
          break;
        default:
          layer_move(_WIN);
          switchOS(NG_WIN);
      }
   
     my_token = defer_exec(NAGINATA_TIMEOUT, kanaoff, NULL);
   }