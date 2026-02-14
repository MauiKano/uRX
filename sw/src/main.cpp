#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <driver/i2s.h>
#include "config.h"

// Specific header for GPIO Matrix access
#include "rom/gpio.h" 

void init_i2s_system() {
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT, 
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT, 
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
        .data_in_num = I2S_ADC0_DIN // Mapped to I2S0I_DATA_IN0_IDX automatically
    };

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);

    /**
     * GPIO MATRIX HACK for ADC1 (GPIO 14)
     * Signal Index for ESP32-S3 I2S0 Data In 1 is 80.
     * We use gpio_matrix_in to route physical GPIO 14 to internal signal 80.
     */
    const int I2S0I_DATA_IN1_IDX = 80; 
    gpio_matrix_in((gpio_num_t)I2S_ADC1_DIN, I2S0I_DATA_IN1_IDX, false);

    Serial.println("I2S: System initialized with Multi-Lane Hack");
}

void setup() {
    Serial.begin(115200);
    delay(2000); 
    Serial.println("uRX Navtex SDR Booting...");

    // I2C Initialisation
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN, I2C_FREQ);

    // SPI Initialisation
    SPI.begin(SPI_SCLK_PIN, -1, SPI_MOSI_PIN, -1);

    // GPIO Pins for Displays
    pinMode(EINK_CS_PIN, OUTPUT);
    digitalWrite(EINK_CS_PIN, HIGH);
    pinMode(EINK_DC_PIN, OUTPUT);
    pinMode(EINK_RST_PIN, OUTPUT);
    pinMode(EINK_BUSY_PIN, INPUT);

    pinMode(TFT_CS_PIN, OUTPUT);
    digitalWrite(TFT_CS_PIN, HIGH);
    pinMode(TFT_DC_PIN, OUTPUT);
    pinMode(TFT_RST_PIN, OUTPUT);
    pinMode(TFT_BLK_PIN, OUTPUT);
    digitalWrite(TFT_BLK_PIN, LOW);

    // GPS and Encoder
    pinMode(GPS_EN_PIN, OUTPUT);
    digitalWrite(GPS_EN_PIN, LOW);
    Serial2.begin(GPS_BAUD, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);

    pinMode(ENCODER_A, INPUT_PULLUP);
    pinMode(ENCODER_B, INPUT_PULLUP);
    pinMode(ENCODER_SW, INPUT_PULLUP);

    // Battery and Mute
    pinMode(BAT_STAT_PIN, INPUT_PULLUP);
    pinMode(SI4732_MUTE_PIN, OUTPUT);
    digitalWrite(SI4732_MUTE_PIN, LOW);

    init_i2s_system();
}

static unsigned long lastCheck = 0;

void loop() {
    // Buffering logic using legacy i2s_read
    static int32_t samples[512];
    size_t bytes_read = 0;
    
    // Read from DMA
    i2s_read(I2S_NUM_0, &samples, sizeof(samples), &bytes_read, portMAX_DELAY);
    if (i2s_read(I2S_NUM_0, &samples, sizeof(samples), &bytes_read, 10) == ESP_OK) {
       if (bytes_read > 0) {
           size_t bytes_written = 0;
           // Output to DAC
           i2s_write(I2S_NUM_0, &samples, bytes_read, &bytes_written, portMAX_DELAY);
       }
       else {
           Serial.println("I2S Read Timeout");
       }    
    }
    if (millis() - lastCheck > 2000) {
        Serial.println("Loop is alive - Waiting for ADC hardware...");
        lastCheck = millis();
    }
}