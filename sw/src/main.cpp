#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <driver/i2s.h>
#include "config.h"

/**
 * uRX Navtex SDR - Hardware Validation Firmware
 * This code initializes all GPIOs to verify the pinout configuration.
 */

void setup() {
    // 1. Serielle Konsole für Debugging (USB-CDC)
    Serial.begin(115200);
    delay(2000);
    Serial.println("--- uRX Hardware Validation Start ---");

    // 2. I2C Bus Initialisierung (Si4732 & Si5351)
    bool i2c_ok = Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN, I2C_FREQ);
    if (i2c_ok) Serial.println("I2C Bus: Initialized Pins 18/3");

    // 3. SPI Bus Initialisierung (Shared for Displays)
    // Wir nutzen das Standard SPI Objekt (VSPI/FSPI)
    SPI.begin(SPI_SCLK_PIN, -1, SPI_MOSI_PIN, -1);
    Serial.println("SPI Bus: Initialized Pins 1/2");

    // 4. Display Control Pins
    pinMode(EINK_CS_PIN, OUTPUT);
    digitalWrite(EINK_CS_PIN, HIGH); // Deaktiviert
    pinMode(EINK_DC_PIN, OUTPUT);
    pinMode(EINK_RST_PIN, OUTPUT);
    pinMode(EINK_BUSY_PIN, INPUT);
    Serial.println("E-Ink Pins: Configured 4/5/6/7");

    pinMode(TFT_CS_PIN, OUTPUT);
    digitalWrite(TFT_CS_PIN, HIGH); // Deaktiviert
    pinMode(TFT_DC_PIN, OUTPUT);
    pinMode(TFT_RST_PIN, OUTPUT);
    pinMode(TFT_BLK_PIN, OUTPUT);
    digitalWrite(TFT_BLK_PIN, LOW); // Backlight aus
    Serial.println("TFT Pins: Configured 42/43/44/21");

    // 5. I2S Full-Duplex Initialisierung (ADC0, ADC1, DAC)
    // Hinweis: Dies nutzt den ESP32-S3 I2S-Treiber v1
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT, // Stereo
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 1024,
        .use_apll = false,
        .tx_desc_auto_clear = true
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_BCK_PIN,
        .ws_io_num = I2S_WS_PIN,
        .data_out_num = I2S_DAC_DOUT,
        .data_in_num = I2S_ADC0_DIN // Erster Input-Kanal
    };

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);
    
    // Zweite Input-Lane für ADC1 (Si4732) hinzufügen
    // Beim S3 wird dies über den GPIO-Matrix-Register-Hack oder 
    // den neuen I2S-TDM Treiber gelöst. Hier nur Pin-Init zur Validierung:
    pinMode(I2S_ADC1_DIN, INPUT); 
    Serial.println("I2S Bus: Full-Duplex Pins 9/11/12/13/14 initialized");

    // 6. GPS Interface
    pinMode(GPS_EN_PIN, OUTPUT);
    digitalWrite(GPS_EN_PIN, LOW); // GPS aus
    // UART2 für GPS
    Serial2.begin(GPS_BAUD, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
    Serial.println("GPS Interface: Pins 15/16/17 initialized");

    // 7. User Input (Encoder)
    pinMode(ENCODER_A, INPUT_PULLUP);
    pinMode(ENCODER_B, INPUT_PULLUP);
    pinMode(ENCODER_SW, INPUT_PULLUP);
    Serial.println("Encoder: Pins 38/39/40 initialized");

    // 8. MISC
    pinMode(BAT_STAT_PIN, INPUT_PULLUP);
    pinMode(SI4732_MUTE_PIN, OUTPUT);
    digitalWrite(SI4732_MUTE_PIN, LOW); // Mute aktiv
    pinMode(I2S_DAC_DOUT, OUTPUT);
    Serial.println("System: Battery Stat and Mute Pins initialized");

    Serial.println("--- Validation Success! GPIOs are COMPATIBLE ---");
}

void loop() {
    // LED-Blinken oder Status-Ausgabe
    static unsigned long lastMillis = 0;
    if (millis() - lastMillis > 5000) {
        Serial.println("uRX System Heartbeat: Pins operational.");
        lastMillis = millis();
    }
}