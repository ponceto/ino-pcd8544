/*
 * PCD8544.h - Copyright (c) 2014-2020 - Olivier Poncet
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
#ifndef __INO_PCD8544_PCD8544_h__
#define __INO_PCD8544_PCD8544_h__

// ---------------------------------------------------------------------------
// PCD8544_Traits
// ---------------------------------------------------------------------------

struct PCD8544_Traits
{
    struct Setup
    {
        static constexpr uint8_t SCLK_PINMODE  = OUTPUT;
        static constexpr uint8_t DATA_PINMODE  = OUTPUT;
        static constexpr uint8_t MODE_PINMODE  = OUTPUT;
        static constexpr uint8_t CSEL_PINMODE  = OUTPUT;
        static constexpr uint8_t RESET_PINMODE = OUTPUT;
        static constexpr uint8_t LIGHT_PINMODE = OUTPUT;
    };

    struct Screen
    {
        static constexpr uint8_t  MAX_ROWS   = (48 / 8);
        static constexpr uint8_t  MAX_COLS   = (84 / 1);
        static constexpr uint16_t BYTE_COUNT = (MAX_ROWS * MAX_COLS);
    };

    struct Initial
    {
        static constexpr uint8_t PREV_CHR = '\0';
        static constexpr uint8_t CURR_ROW = 0;
        static constexpr uint8_t CURR_COL = 0;
        static constexpr bool    WRAP_ROW = false;
        static constexpr bool    WRAP_COL = false;
    };
};

// ---------------------------------------------------------------------------
// PCD8544_Wiring
// ---------------------------------------------------------------------------

struct PCD8544_Wiring
{
    uint8_t sclkPin;
    uint8_t dataPin;
    uint8_t modePin;
    uint8_t cselPin;
    uint8_t resetPin;
    uint8_t lightPin;
};

// ---------------------------------------------------------------------------
// PCD8544_Cursor
// ---------------------------------------------------------------------------

struct PCD8544_Cursor
{
    uint8_t prev_chr;
    uint8_t curr_row;
    uint8_t curr_col;
    bool    wrap_row;
    bool    wrap_col;
};

// ---------------------------------------------------------------------------
// PCD8544
// ---------------------------------------------------------------------------

class PCD8544
    : public Print
{
public: // public interface
    PCD8544 ( const uint8_t sclkPin
            , const uint8_t dataPin
            , const uint8_t modePin
            , const uint8_t cselPin
            , const uint8_t resetPin
            , const uint8_t lightPin );

    virtual ~PCD8544() = default;

    void begin();
    void reset();
    void end();

    void setCursor            ( const uint8_t row
                              , const uint8_t col );

    void setContrast          ( const uint8_t level );

    void setBacklight         ( const uint8_t level );

    void clearDisplay         ( const uint8_t value );

    void putImage             ( const uint8_t* image, const bool pgm );

    void setStdInstructionSet ( );

    void setExtInstructionSet ( );

    void setPowerDownMode     ( );

    void setBlankMode         ( );

    void setBlackMode         ( );

    void setNormalMode        ( );

    void setInverseMode       ( );

    void setRowAddressOfRam   ( const uint8_t value );

    void setColAddressOfRam   ( const uint8_t value );

    void setTempCtrl          ( const uint8_t value );

    void setBiasSyst          ( const uint8_t value );

    void setVoltOper          ( const uint8_t value );

    void sendCommand          ( const uint8_t value );

    void sendData             ( const uint8_t value );

    virtual size_t write      ( uint8_t character ) override;

protected: // protected interface
    using Traits = PCD8544_Traits;
    using Wiring = PCD8544_Wiring;
    using Cursor = PCD8544_Cursor;

protected: // protected data
    Wiring const _wiring;
    Cursor       _cursor;
};

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------

#endif /* __INO_PCD8544_PCD8544_h__ */
