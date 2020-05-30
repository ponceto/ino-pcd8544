/*
 * Shield.h - Copyright (c) 2014-2020 - Olivier Poncet
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
#ifndef __INO_PCD8544_Shield_h__
#define __INO_PCD8544_Shield_h__

#include <PCD8544.h>

// ---------------------------------------------------------------------------
// Keypad / KeyVal / KeySym / KeyStr
// ---------------------------------------------------------------------------

struct Keypad
{
    typedef int16_t Type;
};

struct KeyVal
{
    typedef int16_t Type;

    static constexpr Type KP_NONE      = 255;
    static constexpr Type KP_UP        = 185;
    static constexpr Type KP_DOWN      =  82;
    static constexpr Type KP_LEFT      =   0;
    static constexpr Type KP_RIGHT     = 126;
    static constexpr Type KP_BUTTON    =  35;
    static constexpr Type KP_TOLERANCE =  15;
};

struct KeySym
{
    typedef uint8_t Type;

    static constexpr Type KP_NONE   = 0;
    static constexpr Type KP_UP     = 1;
    static constexpr Type KP_DOWN   = 2;
    static constexpr Type KP_LEFT   = 3;
    static constexpr Type KP_RIGHT  = 4;
    static constexpr Type KP_BUTTON = 5;
};

struct KeyStr
{
    typedef __FlashStringHelper* Type;
};

typedef Keypad::Type KeyPadType;
typedef KeyVal::Type KeyValType;
typedef KeySym::Type KeySymType;
typedef KeyStr::Type KeyStrType;

// ---------------------------------------------------------------------------
// Shield_Traits
// ---------------------------------------------------------------------------

struct Shield_Traits
{
    struct Config
    {
        static constexpr unsigned long LOOP_DELAY = 100UL;
    };
};

// ---------------------------------------------------------------------------
// Shield_Wiring
// ---------------------------------------------------------------------------

struct Shield_Wiring
{
    static constexpr uint8_t SCLK_PIN  =  2;
    static constexpr uint8_t DATA_PIN  =  3;
    static constexpr uint8_t MODE_PIN  =  4;
    static constexpr uint8_t CSEL_PIN  =  5;
    static constexpr uint8_t RESET_PIN =  6;
    static constexpr uint8_t LIGHT_PIN =  7;
    static constexpr uint8_t INPUT_PIN = A0;
};

// ---------------------------------------------------------------------------
// Shield
// ---------------------------------------------------------------------------

class Shield
    : public Print
{
public: // public interface
    Shield();

    void setup();
    void loop();

    void       setBacklight ( const bool backlight = true );

    void       clearDisplay ( const bool inverse = false );

    void       setCursor    ( const uint8_t row
                            , const uint8_t col );

    KeyPadType readKeypad   ( ) const;

    KeyValType getKeyVal    ( const KeyPadType keypad ) const;

    KeySymType getKeySym    ( const KeyValType keyval ) const;

    KeyStrType getKeyStr    ( const KeySymType keysym ) const;

    virtual size_t write(uint8_t character) override;

protected: // protected interface
    using Traits = Shield_Traits;
    using Wiring = Shield_Wiring;
    using Config = Traits::Config;

    void initKeypad();
    void initScreen();

protected: // protected data
    PCD8544 _screen;
};

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------

#endif /* __INO_PCD8544_Shield_h__ */
