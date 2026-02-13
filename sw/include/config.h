#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

/**
 * uRX Navtex SDR - Global Configuration
 * Optimized for ESP32-S3-WROOM-2 (N16 / Non-PSRAM Variant)
 * License: GPL-3.0
 */

// --- I2C BUS (Radio Chips: Si5351 & Si4732) ---
#define I2C_SDA_PIN      18  // Pin 11 (WROOM-2)
#define I2C_SCL_PIN       3  // Pin 15 (WROOM-2)
#define I2C_FREQ     400000  // 400kHz Fast Mode

#define SI5351_ADDR    0x60
#define SI4732_ADDR    0x11  // SEN Pin to GND

// --- SHARED SPI BUS (MOSI/SCLK for Displays) ---
#define SPI_MOSI_PIN      1  // Pin 39 (WROOM-2) - SDA Label
#define SPI_SCLK_PIN      2  // Pin 38 (WROOM-2) - SCL Label

// --- 4.2" E-INK DISPLAY (WeAct/GxEPD2) ---
#define EINK_CS_PIN       4  // Pin 8  (WROOM-2)
#define EINK_DC_PIN       5  // Pin 9  (WROOM-2)
#define EINK_RST_PIN      6  // Pin 10 (WROOM-2)
#define EINK_BUSY_PIN     7  // Pin 11 (Busy Signal Input)

// --- 1.9" TFT DISPLAY (ST7789) ---
#define TFT_CS_PIN       42  // Pin 33 (WROOM-2)
#define TFT_DC_PIN       44  // Pin 37 (WROOM-2)
#define TFT_RST_PIN      43  // Pin 36 (WROOM-2)
#define TFT_BLK_PIN      21  // Pin 22 (WROOM-2) - Backlight via BC847

// --- I2S FULL-DUPLEX BUS (ADC0, ADC1, DAC) ---
// Shared Clocks
#define I2S_BCK_PIN      12  // Pin 20 (WROOM-2) - Shared Bit Clock
#define I2S_WS_PIN       11  // Pin 19 (WROOM-2) - Shared Word Select
// Data Lines
#define I2S_ADC0_DIN     13  // Pin 21 (WROOM-2) - SDR I/Q Data (ADC0)
#define I2S_ADC1_DIN     14  // Pin 22 (WROOM-2) - Radio Audio (ADC1)
#define I2S_DAC_DOUT     10  // Pin 18 (WROOM-2) - Unified Audio Out (DAC)
#define SAMPLE_RATE   48000

// --- GPS MODULE (JST-SH 1.0mm) ---
#define GPS_TX_PIN       16  // Pin 9  (WROOM-2) - ESP TX -> GPS RX
#define GPS_RX_PIN       17  // Pin 10 (WROOM-2) - ESP RX <- GPS TX
#define GPS_EN_PIN       15  // Pin 28 (WROOM-2) - GND-Switch via BC847
#define GPS_BAUD       9600

// --- USER INPUT (Rotary Encoder) ---
#define ENCODER_A        38  // Pin 31 (WROOM-2)
#define ENCODER_B        39  // Pin 32 (WROOM-2)
#define ENCODER_SW       40  // Pin 33 (WROOM-2)

// --- MISC / POWER ---
#define BAT_STAT_PIN     41  // Pin 34 (WROOM-2) - Charging Status IP2326
#define SI4732_MUTE_PIN  47  // Pin 35 (WROOM-2) - Hardware Mute Control

#endif // CONFIG_H
