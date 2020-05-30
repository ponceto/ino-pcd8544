/*
 * Shield.cpp - Copyright (c) 2014-2020 - Olivier Poncet
 *
 * This file is part of the PCD8544 library
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
#include <Arduino.h>
#include "Shield.h"

// ---------------------------------------------------------------------------
// Shield
// ---------------------------------------------------------------------------

Shield::Shield()
    : _screen ( Wiring::SCLK_PIN
              , Wiring::DATA_PIN
              , Wiring::MODE_PIN
              , Wiring::CSEL_PIN
              , Wiring::RESET_PIN
              , Wiring::LIGHT_PIN )
{
}

void Shield::setup()
{
    initKeypad();
    initScreen();
    setBacklight();
    clearDisplay();
}

void Shield::loop()
{
    const KeyPadType    keypad = readKeypad();
    const KeyValType    keyval = getKeyVal(keypad);
    const KeySymType    keysym = getKeySym(keyval);
    const KeyStrType    keystr = getKeyStr(keysym);
    const unsigned long micros = ::micros();
    const unsigned long millis = ::millis();

    setCursor(0, 0);

    println(F("** LCD 4884 **"  ));
    println(F("~~~~~~~~~~~~~~"  ));
    print  (F("              \r"));
    print  (F("us: "            ));
    println( (micros            ));
    print  (F("              \r"));
    print  (F("ms: "            ));
    println( (millis            ));
    print  (F("              \r"));
    print  (F("keyval: "        ));
    println( (keyval            ));
    print  (F("              \r"));
    print  (F("keysym: "        ));
    println( (keystr            ));

    ::delay(Config::LOOP_DELAY);
}

void Shield::setBacklight(const bool backlight)
{
    _screen.setBacklight(backlight ? 0x00 : 0xff);
}

void Shield::clearDisplay(const bool inverse)
{
    _screen.clearDisplay(0x00);
    if(inverse != false) {
        _screen.setInverseMode();
    }
    else {
        _screen.setNormalMode();
    }
}

void Shield::setCursor(const uint8_t row, const uint8_t col)
{
    _screen.setCursor(row, col);
}

KeyPadType Shield::readKeypad() const
{
    return ::analogRead(Wiring::INPUT_PIN);
}

KeyValType Shield::getKeyVal(const KeyPadType keypad) const
{
    const KeyValType keyval = (keypad >> 2);

    return keyval;
}

KeySymType Shield::getKeySym(const KeyValType keyval) const
{
    /* up */ {
        constexpr KeySymType keysym       = KeySym::KP_UP;
        constexpr KeyValType kp_value     = KeyVal::KP_UP;
        constexpr KeyValType kp_min_value = kp_value - KeyVal::KP_TOLERANCE;
        constexpr KeyValType kp_max_value = kp_value + KeyVal::KP_TOLERANCE;

        if((keyval >= kp_min_value) && (keyval <= kp_max_value)) {
            return keysym;
        }
    }
    /* down */ {
        constexpr KeySymType keysym       = KeySym::KP_DOWN;
        constexpr KeyValType kp_value     = KeyVal::KP_DOWN;
        constexpr KeyValType kp_min_value = kp_value - KeyVal::KP_TOLERANCE;
        constexpr KeyValType kp_max_value = kp_value + KeyVal::KP_TOLERANCE;

        if((keyval >= kp_min_value) && (keyval <= kp_max_value)) {
            return keysym;
        }
    }
    /* left */ {
        constexpr KeySymType keysym       = KeySym::KP_LEFT;
        constexpr KeyValType kp_value     = KeyVal::KP_LEFT;
        constexpr KeyValType kp_min_value = kp_value - KeyVal::KP_TOLERANCE;
        constexpr KeyValType kp_max_value = kp_value + KeyVal::KP_TOLERANCE;

        if((keyval >= kp_min_value) && (keyval <= kp_max_value)) {
            return keysym;
        }
    }
    /* right */ {
        constexpr KeySymType keysym       = KeySym::KP_RIGHT;
        constexpr KeyValType kp_value     = KeyVal::KP_RIGHT;
        constexpr KeyValType kp_min_value = kp_value - KeyVal::KP_TOLERANCE;
        constexpr KeyValType kp_max_value = kp_value + KeyVal::KP_TOLERANCE;

        if((keyval >= kp_min_value) && (keyval <= kp_max_value)) {
            return keysym;
        }
    }
    /* button */ {
        constexpr KeySymType keysym       = KeySym::KP_BUTTON;
        constexpr KeyValType kp_value     = KeyVal::KP_BUTTON;
        constexpr KeyValType kp_min_value = kp_value - KeyVal::KP_TOLERANCE;
        constexpr KeyValType kp_max_value = kp_value + KeyVal::KP_TOLERANCE;

        if((keyval >= kp_min_value) && (keyval <= kp_max_value)) {
            return keysym;
        }
    }
    return KeySym::KP_NONE;
}

KeyStrType Shield::getKeyStr(const KeySymType keysym) const
{
    switch(keysym) {
        case KeySym::KP_UP:
            return const_cast<KeyStrType>(F("up"));
        case KeySym::KP_DOWN:
            return const_cast<KeyStrType>(F("down"));
        case KeySym::KP_LEFT:
            return const_cast<KeyStrType>(F("left"));
        case KeySym::KP_RIGHT:
            return const_cast<KeyStrType>(F("right"));
        case KeySym::KP_BUTTON:
            return const_cast<KeyStrType>(F("button"));
        default:
            break;
    }
    return const_cast<KeyStrType>(F("none"));
}

void Shield::initKeypad()
{
    ::pinMode(Wiring::INPUT_PIN, INPUT);
}

void Shield::initScreen()
{
    _screen.begin();
}

size_t Shield::write(uint8_t character)
{
    return _screen.write(character);
}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------
