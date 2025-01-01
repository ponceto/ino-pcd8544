/*
 * PCD8544Test.ino - Copyright (c) 2014-2025 - Olivier Poncet
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
#include <PCD8544.h>

// ---------------------------------------------------------------------------
// Setup
// ---------------------------------------------------------------------------

struct Setup
{
    static constexpr unsigned long SERIAL_SPEED   = 9600UL;
    static constexpr unsigned long LOOP_DELAY     = 1000UL;
    static constexpr uint8_t       LCD_SCLK_PIN   =  2;
    static constexpr uint8_t       LCD_DATA_PIN   =  3;
    static constexpr uint8_t       LCD_MODE_PIN   =  4;
    static constexpr uint8_t       LCD_CSEL_PIN   =  5;
    static constexpr uint8_t       LCD_RESET_PIN  =  6;
    static constexpr uint8_t       LCD_LIGHT_PIN  =  7;
    static constexpr uint8_t       PAD_INPUT_PIN  = A0;
};

// ---------------------------------------------------------------------------
// globals
// ---------------------------------------------------------------------------

PCD8544 screen ( Setup::LCD_SCLK_PIN
               , Setup::LCD_DATA_PIN
               , Setup::LCD_MODE_PIN
               , Setup::LCD_CSEL_PIN
               , Setup::LCD_RESET_PIN
               , Setup::LCD_LIGHT_PIN );

// ---------------------------------------------------------------------------
// setup
// ---------------------------------------------------------------------------

void setup()
{
    Serial.begin(Setup::SERIAL_SPEED);
    screen.begin();
    screen.println(" Hello  World ");
}

// ---------------------------------------------------------------------------
// loop
// ---------------------------------------------------------------------------

void loop()
{
    ::delay(Setup::LOOP_DELAY);
}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------
