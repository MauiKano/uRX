# uRX I2S System Architecture & Configuration

## 1. Overview
The uRX Navtex SDR utilizes a **Full-Duplex, Multi-Channel I2S Architecture**. To ensure perfect phase synchronization and zero-drift between the SDR data, radio audio, and the final output, all audio components are driven by a single I2S clock domain on the **ESP32-S3 (I2S0 Controller)**.

## 2. Bus Configuration
The system operates in **Master Mode** (ESP32 provides the clocks) with the following parameters:
* **Sample Rate:** 48,000 Hz
* **Bit Depth:** 24-bit (delivered in 32-bit slots)
* **Mode:** Full-Duplex (Simultaneous Input and Output)

## 3. Hardware Interconnect (GPIO Mapping)

| I2S Signal | Direction | ESP32 GPIO | Module Pin | Connected Devices |
| :--- | :--- | :--- | :--- | :--- |
| **BCLK** | Output | **GPIO 12** | Pin 20 | ADC0, ADC1, DAC (Bit Clock) |
| **WS / LRCK** | Output | **GPIO 11** | Pin 19 | ADC0, ADC1, DAC (Word Select) |
| **DIN0** | Input | **GPIO 13** | Pin 21 | **ADC0** (Tayloe Mixer I/Q) |
| **DIN1** | Input | **GPIO 14** | Pin 22 | **ADC1** (Si4732 Radio Audio) |
| **DOUT** | Output | **GPIO 09** | Pin 17 | **DAC** (PCM5102A Unified Audio) |

## 4. Signal Routing Logic
The ESP32-S3 I2S0 peripheral is configured to handle multiple data lines (Multi-lane I2S):

1.  **SDR Stream (DIN0):** Receives the raw 518 kHz I/Q samples from the Tayloe Mixer. These samples undergo FFT and digital demodulation within the ESP32.
2.  **Radio Stream (DIN1):** Receives the analog audio output from the Si4732. This allows the ESP32 to apply DSP (Digital Signal Processing) like noise reduction, EQ, or AGC (Automatic Gain Control) to the broadcast audio.
3.  **Unified Output (DOUT):** The ESP32 digitally mixes the demodulated Navtex tones and the processed Si4732 audio into a single stereo stream. This combined signal is sent to the PCM5102A DAC.

## 5. Implementation Notes
*   **Clock Master:** The ESP32-S3 generates the Master Clock. The **PCM5102A DAC** is configured in "Internal SCK" mode (SCK pin to GND) to derive its high-speed clock from the BCLK, reducing EMI.
*   **Phase Coherence:** Since all ADCs share the same **WS** and **BCLK** lines, the phase relationship between the I/Q samples (ADC0) and the radio audio (ADC1) is perfectly maintained, which is critical for advanced signal post-processing.
*   **WROOM-2 Compatibility:** By using **GPIO 09** (Pin 17) for DOUT, the design remains fully compatible with the **N16 (non-PSRAM)** variant, avoiding the Octal-SPI memory conflict zone (GPIO 33-37).

---
*Technical Note: I2S0 peripheral in the ESP32-S3 supports up to 4 input lanes, of which 2 are utilized in this design.*

