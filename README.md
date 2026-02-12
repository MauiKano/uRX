# uRX Navtex SDR – System Documentation

## 1. Overview
The **uRX** is a high-performance Software Defined Radio (SDR) receiver, specifically optimized for **Navtex** transmissions (518 kHz / 490 kHz). It combines the processing power of the **ESP32-S3** with a precision **Tayloe Mixer** and the versatility of the **Si4732** radio chip.

This project is licensed under the **GNU General Public License v3.0 (GPL-3.0)**.

---

## 2. Technical Hardware Specification

### RF Frontend & Antenna Management
*   **Resonant Ferrite Rod Antenna:** Tuned as a high-Q circuit for the target frequencies.
*   **Signal Splitting:** The antenna signal is buffered and isolated via an **OPA2211** operational amplifier.
*   **Parallel Architecture:** The OPA2211 output simultaneously feeds the **Tayloe Mixer (CBT3253)** for SDR decoding and the **AM antenna input of the Si4732** for parallel monitoring.
*   **ATS-Mini Heritage:** The tuner section for the Si4732 implements the proven circuit design of the **ATS-Mini**, including an optimized FM input network (180nH / 18pF).

### Signal Processing (SDR & Audio)
*   **Dual-Channel Digitization:** A **PCM1808 24-bit ADC** handles the conversion.
    *   **Channel 1 (I/Q SDR):** Differential I/Q signals from the Tayloe Mixer are amplified via an **LM4562** instrumentation stage.
    *   **Channel 2 (Si4732 Audio):** The analog audio output of the Si4732 is routed to the second ADC channel for digital post-processing.
*   **I2S Integration:** Data is streamed via I2S to the ESP32-S3 for real-time decoding and noise reduction.
*   **Audio Output:** A **PAM8406** power amplifier drives the speaker. A dedicated PDM/PWM output provides audio tones for Navtex monitoring.

### Dual Display Architecture
The uRX features a unique dual-display system for maximum utility:
*   **4.2" E-Ink Display:** For low-power, high-contrast viewing of decoded Navtex text messages.
*   **1.9" TFT Display:** For real-time tuning, spectrum visualization, and system menus.

---

## 3. ESP32-S3 GPIO Assignment (WROOM-2 N16)
*Note: Optimized for the N16 (non-PSRAM) variant to ensure all GPIOs are available for peripherals.*

| Function | GPIO | Module Pin | Details |
| :--- | :--- | :--- | :--- |
| **I2C SDA** | 18 | 11 | Si4732, Si5351 (4.7k Pull-up) |
| **I2C SCL** | 03 | 15 | Si4732, Si5351 (4.7k Pull-up) |
| **SPI MOSI** | 01 | 39 | Shared Data (MOSI) |
| **SPI SCLK** | 02 | 38 | Shared Clock (SCLK) |
| **E-INK CS** | 04 | 08 | E-Ink Chip Select |
| **E-INK DC** | 05 | 09 | E-Ink Data/Command |
| **E-INK RST** | 06 | 10 | E-Ink Reset |
| **TFT CS** | 42 | 33 | TFT Chip Select |
| **TFT DC** | 44 | 37 | TFT Data/Command |
| **TFT RST** | 43 | 36 | TFT Reset |
| **I2S_DATA** | 13 | 21 | From PCM1808 ADC |
| **I2S_BCK** | 12 | 20 | Bit Clock (ADC) |
| **I2S_WS** | 11 | 19 | Word Select (ADC) |
| **GPS TX** | 16 | 09 | Data from GPS |
| **GPS RX** | 17 | 10 | Data to GPS |
| **GPS Power** | 15 | 28 | Controls BC847 (GND-Switch) |
| **Audio Out** | 10 | 18 | PDM/PWM Navtex Tones |
| **Encoder A/B**| 38/39| 31/32 | Rotary Encoder Phases |
| **Encoder SW** | 40 | 33 | Encoder Push-Switch |

---

## 4. Installation & Setup
1.  **Environment:** [PlatformIO](https://platformio.org) is recommended.
2.  **Configuration:** Hardware pins are defined in `include/config.h`. 
3.  **Libraries:** GxEPD2, TFT_eSPI, PU2CLR Si4735, Etherkit Si5351, TinyGPS++.
4.  **Flash:** Ensure `ARDUINO_USB_CDC_ON_BOOT=1` is set in `platformio.ini` for native USB programming.

---

## 5. Power & Safety
The system is powered by a **2S LiPo battery (8.4V)** charged via USB-C using the **IP2326**. Ferrite beads and L-C star-filtering are used to protect the sensitive RF stages from switching noise.

**Disclaimer:** This is an experimental open-source project. Handle LiPo batteries with care.

