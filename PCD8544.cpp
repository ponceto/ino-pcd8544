/*
 * PCD8544.cpp - Copyright (c) 2014-2025 - Olivier Poncet
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
#include "PCD8544.h"

// ---------------------------------------------------------------------------
// some useful macros
// ---------------------------------------------------------------------------

#ifndef countof
#define countof(array) (sizeof(array) / sizeof(array[0]))
#endif

// ---------------------------------------------------------------------------
// font6x8
// ---------------------------------------------------------------------------

namespace {

const uint8_t font6x8[128][6] PROGMEM = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x00 NUL */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x01 SOH */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x02 STX */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x03 ETX */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x04 EOT */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x05 ENQ */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x06 ACK */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x07 BEL */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x08 BS  */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x09 HT  */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x0a LF  */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x0b VT  */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x0c FF  */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x0d CR  */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x0e SO  */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x0f SI  */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x10 DLE */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x11 DC1 */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x12 DC2 */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x13 DC3 */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x14 DC4 */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x15 NAK */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x16 SYN */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x17 ETB */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x18 CAN */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x19 EM  */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x1a SUB */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x1b ESC */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x1c FS  */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x1d GS  */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x1e RS  */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x1f US  */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /* 0x20 SPC */
    { 0x00, 0x00, 0x5f, 0x00, 0x00, 0x00 }, /* 0x21 !   */
    { 0x00, 0x07, 0x00, 0x07, 0x00, 0x00 }, /* 0x22 "   */
    { 0x14, 0x7f, 0x14, 0x7f, 0x14, 0x00 }, /* 0x23 #   */
    { 0x24, 0x2a, 0x7f, 0x2a, 0x12, 0x00 }, /* 0x24 $   */
    { 0x23, 0x13, 0x08, 0x64, 0x62, 0x00 }, /* 0x25 %   */
    { 0x36, 0x49, 0x55, 0x22, 0x50, 0x00 }, /* 0x26 &   */
    { 0x00, 0x05, 0x03, 0x00, 0x00, 0x00 }, /* 0x27 '   */
    { 0x00, 0x1c, 0x22, 0x41, 0x00, 0x00 }, /* 0x28 (   */
    { 0x00, 0x41, 0x22, 0x1c, 0x00, 0x00 }, /* 0x29 )   */
    { 0x14, 0x08, 0x3e, 0x08, 0x14, 0x00 }, /* 0x2a *   */
    { 0x08, 0x08, 0x3e, 0x08, 0x08, 0x00 }, /* 0x2b +   */
    { 0x00, 0x50, 0x30, 0x00, 0x00, 0x00 }, /* 0x2c ,   */
    { 0x08, 0x08, 0x08, 0x08, 0x08, 0x00 }, /* 0x2d -   */
    { 0x00, 0x60, 0x60, 0x00, 0x00, 0x00 }, /* 0x2e .   */
    { 0x20, 0x10, 0x08, 0x04, 0x02, 0x00 }, /* 0x2f /   */
    { 0x3e, 0x51, 0x49, 0x45, 0x3e, 0x00 }, /* 0x30 0   */
    { 0x00, 0x42, 0x7f, 0x40, 0x00, 0x00 }, /* 0x31 1   */
    { 0x42, 0x61, 0x51, 0x49, 0x46, 0x00 }, /* 0x32 2   */
    { 0x21, 0x41, 0x45, 0x4b, 0x31, 0x00 }, /* 0x33 3   */
    { 0x18, 0x14, 0x12, 0x7f, 0x10, 0x00 }, /* 0x34 4   */
    { 0x27, 0x45, 0x45, 0x45, 0x39, 0x00 }, /* 0x35 5   */
    { 0x3c, 0x4a, 0x49, 0x49, 0x30, 0x00 }, /* 0x36 6   */
    { 0x01, 0x71, 0x09, 0x05, 0x03, 0x00 }, /* 0x37 7   */
    { 0x36, 0x49, 0x49, 0x49, 0x36, 0x00 }, /* 0x38 8   */
    { 0x06, 0x49, 0x49, 0x29, 0x1e, 0x00 }, /* 0x39 9   */
    { 0x00, 0x36, 0x36, 0x00, 0x00, 0x00 }, /* 0x3a :   */
    { 0x00, 0x56, 0x36, 0x00, 0x00, 0x00 }, /* 0x3b ;   */
    { 0x08, 0x14, 0x22, 0x41, 0x00, 0x00 }, /* 0x3c <   */
    { 0x14, 0x14, 0x14, 0x14, 0x14, 0x00 }, /* 0x3d =   */
    { 0x00, 0x41, 0x22, 0x14, 0x08, 0x00 }, /* 0x3e >   */
    { 0x02, 0x01, 0x51, 0x09, 0x06, 0x00 }, /* 0x3f ?   */
    { 0x32, 0x49, 0x79, 0x41, 0x3e, 0x00 }, /* 0x40 @   */
    { 0x7e, 0x11, 0x11, 0x11, 0x7e, 0x00 }, /* 0x41 A   */
    { 0x7f, 0x49, 0x49, 0x49, 0x36, 0x00 }, /* 0x42 B   */
    { 0x3e, 0x41, 0x41, 0x41, 0x22, 0x00 }, /* 0x43 C   */
    { 0x7f, 0x41, 0x41, 0x22, 0x1c, 0x00 }, /* 0x44 D   */
    { 0x7f, 0x49, 0x49, 0x49, 0x41, 0x00 }, /* 0x45 E   */
    { 0x7f, 0x09, 0x09, 0x09, 0x01, 0x00 }, /* 0x46 F   */
    { 0x3e, 0x41, 0x49, 0x49, 0x7a, 0x00 }, /* 0x47 G   */
    { 0x7f, 0x08, 0x08, 0x08, 0x7f, 0x00 }, /* 0x48 H   */
    { 0x00, 0x41, 0x7f, 0x41, 0x00, 0x00 }, /* 0x49 I   */
    { 0x20, 0x40, 0x41, 0x3f, 0x01, 0x00 }, /* 0x4a J   */
    { 0x7f, 0x08, 0x14, 0x22, 0x41, 0x00 }, /* 0x4b K   */
    { 0x7f, 0x40, 0x40, 0x40, 0x40, 0x00 }, /* 0x4c L   */
    { 0x7f, 0x02, 0x0c, 0x02, 0x7f, 0x00 }, /* 0x4d M   */
    { 0x7f, 0x04, 0x08, 0x10, 0x7f, 0x00 }, /* 0x4e N   */
    { 0x3e, 0x41, 0x41, 0x41, 0x3e, 0x00 }, /* 0x4f O   */
    { 0x7f, 0x09, 0x09, 0x09, 0x06, 0x00 }, /* 0x50 P   */
    { 0x3e, 0x41, 0x51, 0x21, 0x5e, 0x00 }, /* 0x51 Q   */
    { 0x7f, 0x09, 0x19, 0x29, 0x46, 0x00 }, /* 0x52 R   */
    { 0x46, 0x49, 0x49, 0x49, 0x31, 0x00 }, /* 0x53 S   */
    { 0x01, 0x01, 0x7f, 0x01, 0x01, 0x00 }, /* 0x54 T   */
    { 0x3f, 0x40, 0x40, 0x40, 0x3f, 0x00 }, /* 0x55 U   */
    { 0x1f, 0x20, 0x40, 0x20, 0x1f, 0x00 }, /* 0x56 V   */
    { 0x3f, 0x40, 0x38, 0x40, 0x3f, 0x00 }, /* 0x57 W   */
    { 0x63, 0x14, 0x08, 0x14, 0x63, 0x00 }, /* 0x58 X   */
    { 0x07, 0x08, 0x70, 0x08, 0x07, 0x00 }, /* 0x59 Y   */
    { 0x61, 0x51, 0x49, 0x45, 0x43, 0x00 }, /* 0x5a Z   */
    { 0x00, 0x7f, 0x41, 0x41, 0x00, 0x00 }, /* 0x5b [   */
    { 0x02, 0x04, 0x08, 0x10, 0x20, 0x00 }, /* 0x5c \   */
    { 0x00, 0x41, 0x41, 0x7f, 0x00, 0x00 }, /* 0x5d ]   */
    { 0x04, 0x02, 0x01, 0x02, 0x04, 0x00 }, /* 0x5e ^   */
    { 0x40, 0x40, 0x40, 0x40, 0x40, 0x00 }, /* 0x5f _   */
    { 0x00, 0x01, 0x02, 0x04, 0x00, 0x00 }, /* 0x60 `   */
    { 0x20, 0x54, 0x54, 0x54, 0x78, 0x00 }, /* 0x61 a   */
    { 0x7f, 0x48, 0x44, 0x44, 0x38, 0x00 }, /* 0x62 b   */
    { 0x38, 0x44, 0x44, 0x44, 0x20, 0x00 }, /* 0x63 c   */
    { 0x38, 0x44, 0x44, 0x48, 0x7f, 0x00 }, /* 0x64 d   */
    { 0x38, 0x54, 0x54, 0x54, 0x18, 0x00 }, /* 0x65 e   */
    { 0x08, 0x7e, 0x09, 0x01, 0x02, 0x00 }, /* 0x66 f   */
    { 0x0c, 0x52, 0x52, 0x52, 0x3e, 0x00 }, /* 0x67 g   */
    { 0x7f, 0x08, 0x04, 0x04, 0x78, 0x00 }, /* 0x68 h   */
    { 0x00, 0x44, 0x7d, 0x40, 0x00, 0x00 }, /* 0x69 i   */
    { 0x20, 0x40, 0x44, 0x3d, 0x00, 0x00 }, /* 0x6a j   */
    { 0x7f, 0x10, 0x28, 0x44, 0x00, 0x00 }, /* 0x6b k   */
    { 0x00, 0x41, 0x7f, 0x40, 0x00, 0x00 }, /* 0x6c l   */
    { 0x7c, 0x04, 0x18, 0x04, 0x78, 0x00 }, /* 0x6d m   */
    { 0x7c, 0x08, 0x04, 0x04, 0x78, 0x00 }, /* 0x6e n   */
    { 0x38, 0x44, 0x44, 0x44, 0x38, 0x00 }, /* 0x6f o   */
    { 0x7c, 0x14, 0x14, 0x14, 0x08, 0x00 }, /* 0x70 p   */
    { 0x08, 0x14, 0x14, 0x18, 0x7c, 0x00 }, /* 0x71 q   */
    { 0x7c, 0x08, 0x04, 0x04, 0x08, 0x00 }, /* 0x72 r   */
    { 0x48, 0x54, 0x54, 0x54, 0x20, 0x00 }, /* 0x73 s   */
    { 0x04, 0x3f, 0x44, 0x40, 0x20, 0x00 }, /* 0x74 t   */
    { 0x3c, 0x40, 0x40, 0x20, 0x7c, 0x00 }, /* 0x75 u   */
    { 0x1c, 0x20, 0x40, 0x20, 0x1c, 0x00 }, /* 0x76 v   */
    { 0x3c, 0x40, 0x30, 0x40, 0x3c, 0x00 }, /* 0x77 w   */
    { 0x44, 0x28, 0x10, 0x28, 0x44, 0x00 }, /* 0x78 x   */
    { 0x0c, 0x50, 0x50, 0x50, 0x3c, 0x00 }, /* 0x79 y   */
    { 0x44, 0x64, 0x54, 0x4c, 0x44, 0x00 }, /* 0x7a z   */
    { 0x00, 0x08, 0x36, 0x41, 0x00, 0x00 }, /* 0x7b {   */
    { 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00 }, /* 0x7c |   */
    { 0x00, 0x41, 0x36, 0x08, 0x00, 0x00 }, /* 0x7d }   */
    { 0x10, 0x08, 0x08, 0x10, 0x08, 0x00 }, /* 0x7e ~   */
    { 0x7f, 0x55, 0x49, 0x55, 0x7f, 0x00 }  /* 0x7f     */
};

}

// ---------------------------------------------------------------------------
// Command
// ---------------------------------------------------------------------------

namespace {

struct Command
{
    struct Nop
    {
        static constexpr uint8_t instruction   = 0b00000000;
        static constexpr uint8_t operand_mask  = 0b00000000;
        static constexpr uint8_t default_value = 0b00000000;

        static uint8_t command(const uint8_t operand_data)
        {
            return (instruction | (operand_data & operand_mask));
        }

        static uint8_t operand(const uint8_t value)
        {
            return value;
        }
    };

    struct FunctionSet
    {
        static constexpr uint8_t instruction   = 0b00100000;
        static constexpr uint8_t operand_mask  = 0b00000111;
        static constexpr uint8_t default_value = 0b00000000;
        static constexpr uint8_t std_inst_set  = 0b00000000;
        static constexpr uint8_t ext_inst_set  = 0b00000001;
        static constexpr uint8_t h_addressing  = 0b00000000;
        static constexpr uint8_t v_addressing  = 0b00000010;
        static constexpr uint8_t power_down    = 0b00000100;

        static uint8_t command(const uint8_t operand_data)
        {
            return (instruction | (operand_data & operand_mask));
        }

        static uint8_t operand(const uint8_t value)
        {
            return value;
        }
    };

    struct DisplayControl
    {
        static constexpr uint8_t instruction   = 0b00001000;
        static constexpr uint8_t operand_mask  = 0b00000101;
        static constexpr uint8_t default_value = 0b00000100;
        static constexpr uint8_t blank_mode    = 0b00000000;
        static constexpr uint8_t black_mode    = 0b00000001;
        static constexpr uint8_t normal_mode   = 0b00000100;
        static constexpr uint8_t inverse_mode  = 0b00000101;

        static uint8_t command(const uint8_t operand_data)
        {
            return (instruction | (operand_data & operand_mask));
        }

        static uint8_t operand(const uint8_t value)
        {
            return value;
        }
    };

    struct SetRowAddressOfRam
    {
        static constexpr uint8_t instruction   = 0b01000000;
        static constexpr uint8_t operand_mask  = 0b00000111;
        static constexpr uint8_t default_value = 0b00000000;
        static constexpr uint8_t min_row       = 0b00000000;
        static constexpr uint8_t max_row       = 0b00000101;

        static uint8_t command(const uint8_t operand_data)
        {
            return (instruction | (operand_data & operand_mask));
        }

        static uint8_t operand(const uint8_t value)
        {
            return value % (max_row + 1);
        }
    };

    struct SetColAddressOfRam
    {
        static constexpr uint8_t instruction   = 0b10000000;
        static constexpr uint8_t operand_mask  = 0b01111111;
        static constexpr uint8_t default_value = 0b00000000;
        static constexpr uint8_t min_col       = 0b00000000;
        static constexpr uint8_t max_col       = 0b01010011;

        static uint8_t command(const uint8_t operand_data)
        {
            return (instruction | (operand_data & operand_mask));
        }

        static uint8_t operand(const uint8_t value)
        {
            return value % (max_col + 1);
        }
    };

    struct TempCtrl
    {
        static constexpr uint8_t instruction   = 0b00000100;
        static constexpr uint8_t operand_mask  = 0b00000011;
        static constexpr uint8_t default_value = 0b00000000;
        static constexpr uint8_t tc0           = 0b00000000;
        static constexpr uint8_t tc1           = 0b00000001;
        static constexpr uint8_t tc2           = 0b00000010;
        static constexpr uint8_t tc3           = 0b00000011;

        static uint8_t command(const uint8_t operand_data)
        {
            return (instruction | (operand_data & operand_mask));
        }

        static uint8_t operand(const uint8_t value)
        {
            return value;
        }
    };

    struct BiasSyst
    {
        static constexpr uint8_t instruction   = 0b00010000;
        static constexpr uint8_t operand_mask  = 0b00000111;
        static constexpr uint8_t default_value = 0b00000011;
        static constexpr uint8_t n0            = 0b00000111;
        static constexpr uint8_t n1            = 0b00000110;
        static constexpr uint8_t n2            = 0b00000101;
        static constexpr uint8_t n3            = 0b00000100;
        static constexpr uint8_t n4            = 0b00000011;
        static constexpr uint8_t n5            = 0b00000010;
        static constexpr uint8_t n6            = 0b00000001;
        static constexpr uint8_t n7            = 0b00000000;

        static uint8_t command(const uint8_t operand_data)
        {
            return (instruction | (operand_data & operand_mask));
        }

        static uint8_t operand(const uint8_t value)
        {
            return value;
        }
    };

    struct VoltOper
    {
        static constexpr uint8_t instruction   = 0b10000000;
        static constexpr uint8_t operand_mask  = 0b01111111;
        static constexpr uint8_t default_value = 0b01000010;

        static uint8_t command(const uint8_t operand_data)
        {
            return (instruction | (operand_data & operand_mask));
        }

        static uint8_t operand(const uint8_t value)
        {
            return value;
        }
    };
};

}

// ---------------------------------------------------------------------------
// PCD8544
// ---------------------------------------------------------------------------

PCD8544::PCD8544 ( const uint8_t sclkPin
                 , const uint8_t dataPin
                 , const uint8_t modePin
                 , const uint8_t cselPin
                 , const uint8_t resetPin
                 , const uint8_t lightPin )
    : _wiring { sclkPin
              , dataPin
              , modePin
              , cselPin
              , resetPin
              , lightPin }
    , _cursor { Traits::Initial::PREV_CHR
              , Traits::Initial::CURR_ROW
              , Traits::Initial::CURR_COL
              , Traits::Initial::WRAP_ROW
              , Traits::Initial::WRAP_COL }
{
}

void PCD8544::begin()
{
    /* setup */ {
        ::pinMode(_wiring.sclkPin , Traits::Setup::SCLK_PINMODE );
        ::pinMode(_wiring.dataPin , Traits::Setup::DATA_PINMODE );
        ::pinMode(_wiring.modePin , Traits::Setup::MODE_PINMODE );
        ::pinMode(_wiring.cselPin , Traits::Setup::CSEL_PINMODE );
        ::pinMode(_wiring.resetPin, Traits::Setup::RESET_PINMODE);
        ::pinMode(_wiring.lightPin, Traits::Setup::LIGHT_PINMODE);
    }
    /* reset */ {
        reset();
    }
}

void PCD8544::reset()
{
    /* reset cursor values */ {
        _cursor.prev_chr = Traits::Initial::PREV_CHR;
        _cursor.curr_row = Traits::Initial::CURR_ROW;
        _cursor.curr_col = Traits::Initial::CURR_COL;
        _cursor.wrap_row = Traits::Initial::WRAP_ROW;
        _cursor.wrap_col = Traits::Initial::WRAP_COL;
    }
    /* initial signals state */ {
        ::digitalWrite(_wiring.cselPin , 0x1);
        ::digitalWrite(_wiring.resetPin, 0x1);
    }
    /* reset pulse (~100ms) */ {
        ::digitalWrite(_wiring.resetPin, 0x0);
        ::delay(100);
        ::digitalWrite(_wiring.resetPin, 0x1);
    }
    /* configure controller */ {
        setExtInstructionSet();
        setTempCtrl(Command::TempCtrl::default_value);
        setBiasSyst(Command::BiasSyst::default_value);
        setVoltOper(Command::VoltOper::default_value);
        setStdInstructionSet();
        setNormalMode();
    }
    /* clear display */ {
        clearDisplay(0x00);
    }
}

void PCD8544::end()
{
    /* power-down */ {
        setPowerDownMode();
    }
}

void PCD8544::setCursor(const uint8_t row, const uint8_t col)
{
    setRowAddressOfRam(row);
    setColAddressOfRam(col);
}

void PCD8544::setContrast(const uint8_t level)
{
    setExtInstructionSet();
    setVoltOper(level);
    setStdInstructionSet();
}

void PCD8544::setBacklight(const uint8_t level)
{
    ::analogWrite(_wiring.lightPin, (0xff - level));
}

void PCD8544::clearDisplay(const uint8_t value)
{
    /* move to origin */ {
        setRowAddressOfRam(0);
        setColAddressOfRam(0);
    }
    /* send data */ {
        uint16_t bytes = Traits::Screen::BYTE_COUNT;
        do {
            sendData(value);
        } while(--bytes != 0);
    }
}

void PCD8544::putImage(const uint8_t* image, const bool pgm)
{
    /* move to origin */ {
        setRowAddressOfRam(0);
        setColAddressOfRam(0);
    }
    /* send data */ {
        if(pgm != false) {
            const uint8_t* bytes = image;
            uint16_t       count = Traits::Screen::BYTE_COUNT;
            do {
                sendData(pgm_read_byte(bytes++));
            } while(--count != 0);
        }
        else {
            const uint8_t* bytes = image;
            uint16_t       count = Traits::Screen::BYTE_COUNT;
            do {
                sendData(*bytes++);
            } while(--count != 0);
        }
    }
}

void PCD8544::setStdInstructionSet()
{
    typedef Command::FunctionSet command_traits;
    const uint8_t operand = command_traits::std_inst_set;
    const uint8_t command = command_traits::command(operand);

    sendCommand(command);
}

void PCD8544::setExtInstructionSet()
{
    typedef Command::FunctionSet command_traits;
    const uint8_t operand = command_traits::ext_inst_set;
    const uint8_t command = command_traits::command(operand);

    sendCommand(command);
}

void PCD8544::setPowerDownMode()
{
    typedef Command::FunctionSet command_traits;
    const uint8_t operand = command_traits::power_down;
    const uint8_t command = command_traits::command(operand);

    sendCommand(command);
}

void PCD8544::setBlankMode()
{
    typedef Command::DisplayControl command_traits;
    const uint8_t operand = command_traits::blank_mode;
    const uint8_t command = command_traits::command(operand);

    sendCommand(command);
}

void PCD8544::setBlackMode()
{
    typedef Command::DisplayControl command_traits;
    const uint8_t operand = command_traits::black_mode;
    const uint8_t command = command_traits::command(operand);

    sendCommand(command);
}

void PCD8544::setNormalMode()
{
    typedef Command::DisplayControl command_traits;
    const uint8_t operand = command_traits::normal_mode;
    const uint8_t command = command_traits::command(operand);

    sendCommand(command);
}

void PCD8544::setInverseMode()
{
    typedef Command::DisplayControl command_traits;
    const uint8_t operand = command_traits::inverse_mode;
    const uint8_t command = command_traits::command(operand);

    sendCommand(command);
}

void PCD8544::setRowAddressOfRam(const uint8_t value)
{
    /* adjust cursor row */ {
        _cursor.curr_row = (value % Traits::Screen::MAX_ROWS);
        _cursor.wrap_row = false;
    }
    typedef Command::SetRowAddressOfRam command_traits;
    const uint8_t operand = command_traits::operand(_cursor.curr_row);
    const uint8_t command = command_traits::command(operand);

    sendCommand(command);
}

void PCD8544::setColAddressOfRam(const uint8_t value)
{
    /* adjust cursor col */ {
        _cursor.curr_col = (value % Traits::Screen::MAX_COLS);
        _cursor.wrap_col = false;
    }
    typedef Command::SetColAddressOfRam command_traits;
    const uint8_t operand = command_traits::operand(_cursor.curr_col);
    const uint8_t command = command_traits::command(operand);

    sendCommand(command);
}

void PCD8544::setTempCtrl(const uint8_t value)
{
    typedef Command::TempCtrl command_traits;
    const uint8_t operand = command_traits::operand(value);
    const uint8_t command = command_traits::command(operand);

    sendCommand(command);
}

void PCD8544::setBiasSyst(const uint8_t value)
{
    typedef Command::BiasSyst command_traits;
    const uint8_t operand = command_traits::operand(value);
    const uint8_t command = command_traits::command(operand);

    sendCommand(command);
}

void PCD8544::setVoltOper(const uint8_t value)
{
    typedef Command::VoltOper command_traits;
    const uint8_t operand = command_traits::operand(value);
    const uint8_t command = command_traits::command(operand);

    sendCommand(command);
}

void PCD8544::sendCommand(const uint8_t value)
{
    /* command mode */ {
        ::digitalWrite(_wiring.modePin, 0x0);
    }
    /* chip enable */ {
        ::digitalWrite(_wiring.cselPin, 0x0);
    }
    /* send command */ {
        ::shiftOut(_wiring.dataPin, _wiring.sclkPin, MSBFIRST, value);
    }
    /* chip disable */ {
        ::digitalWrite(_wiring.cselPin, 0x1);
    }
}

void PCD8544::sendData(const uint8_t value)
{
    /* data mode */ {
        ::digitalWrite(_wiring.modePin, 0x1);
    }
    /* chip enable */ {
        ::digitalWrite(_wiring.cselPin, 0x0);
    }
    /* send data */ {
        ::shiftOut(_wiring.dataPin, _wiring.sclkPin, MSBFIRST, value);
    }
    /* chip disable */ {
        ::digitalWrite(_wiring.cselPin, 0x1);
    }
}

size_t PCD8544::write(uint8_t character)
{
    const uint8_t curr_chr = (character & 0x80 ? 0x7f : character);
    const uint8_t prev_chr = _cursor.prev_chr;

    /* adjust cursor */ {
        _cursor.prev_chr = curr_chr;
    }
    /* process CR/LF */ {
        if(curr_chr == '\r') {
            return 1;
        }
        if(curr_chr == '\n') {
            if(_cursor.wrap_col == false) {
                setRowAddressOfRam(_cursor.curr_row + (Traits::Screen::MAX_ROWS + 1));
            }
            setColAddressOfRam(0);
            return 1;
        }
        if(prev_chr == '\r') {
            if(_cursor.wrap_col != false) {
                setRowAddressOfRam(_cursor.curr_row + (Traits::Screen::MAX_ROWS - 1));
            }
            setColAddressOfRam(0);
        }
    }
    /* adjust cursor */ {
        _cursor.wrap_row = false;
        _cursor.wrap_col = false;
    }
    /* send data to screen */ {
        const uint8_t* bytes = &font6x8[curr_chr][0];
        uint16_t       count = countof(font6x8[curr_chr]);
        do {
            sendData(pgm_read_byte(bytes++));
            if(++_cursor.curr_col >= Traits::Screen::MAX_COLS) {
                _cursor.curr_col = 0;
                _cursor.wrap_col = true;
                if(++_cursor.curr_row >= Traits::Screen::MAX_ROWS) {
                    _cursor.curr_row = 0;
                    _cursor.wrap_row = true;
                }
            }
        } while(--count != 0);
    }
    return 1;
}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------
