# uRX I2S System Architecture: Multi-Lane Full-Duplex Synchronization

## 1. System Roles (Master/Slave Hierarchy)
The uRX Navtex SDR implements a **centralized clocking architecture** to ensure zero-drift signal processing:

*   **I2S Master:** The **ESP32-S3** acts as the sole bus master. It generates the Bit Clock (BCLK) and Word Select (LRCK) signals, governing the timing for all conversion stages.
*   **I2S Slaves:** Both **PCM1808 ADCs** (SDR and Radio Audio) and the **PCM5102A DAC** operate as slaves, synchronizing their data transmission and reception to the ESP32’s clocks.
*   **System Clock Generator:** The **Si5351A** acts as the high-frequency reference provider, delivering a dedicated 12.288 MHz (256 * Fs) Master Clock (SCKI) to the ADCs to drive their internal Delta-Sigma modulators.

## 2. Bus Configuration & Signal Path
The system utilizes a **Full-Duplex, Multi-Lane I2S configuration** on the I2S0 peripheral of the ESP32-S3. This allows the processor to simultaneously ingest two independent 24-bit data streams while outputting a unified analog audio signal.

### Physical Wiring (GPIO Mapping)
| Signal Name | ESP32 GPIO | Direction | Function | Connected To |
| :--- | :--- | :--- | :--- | :--- |
| **I2S_WS** | **GPIO 11** | Output | Word Select / LRCK | ADC0, ADC1, DAC |
| **I2S_BCK** | **GPIO 12** | Output | Bit Clock | ADC0, ADC1, DAC |
| **I2S_DIN0** | **GPIO 13** | Input | SDR I/Q Data | ADC0 (Tayloe Mixer) |
| **I2S_DIN1** | **GPIO 14** | Input | Radio Audio | ADC1 (Si4732) |
| **I2S_DOUT** | **GPIO 10** | Output | Unified Audio | DAC (PCM5102A) |

## 3. Clock Management Details
*   **Synchronous Operation:** Since the ADCs and the DAC share the exact same LRCK and BCLK lines from the ESP32, the SDR data and the audio output are phase-locked. This is critical for low-latency digital signal processing and avoiding buffer underruns.
*   **DAC PLL Mode:** The **PCM5102A DAC** is configured in **PLL Mode** (SCK/Pin 12 tied to GND). It derives its internal high-speed system clock directly from the incoming BCLK (GPIO 12), eliminating the need for a third Master Clock line from the Si5351 and reducing EMI.
*   **Signal Integrity:** 22Ω series termination resistors are placed at the ESP32 source pins (GPIO 10, 11, 12) to dampen fast edges and prevent RF interference within the sensitive 518 kHz receiver stages.

## 4. Processing Flow
1.  **Ingress:** The ESP32-S3 reads the **I/Q SDR stream** (DIN0) and the **Si4732 stream** (DIN1) into a single DMA buffer.
2.  **DSP:** The ESP32 performs FFT/demodulation on the SDR data and applies noise reduction/mixing to the radio audio.
3.  **Egress:** The unified audio (Navtex signal tones + processed radio audio) is pushed out via **DIN** (GPIO 10) to the PCM5102A, which feeds the **PAM8406** power amplifier.

---
*Target Hardware: ESP32-S3-WROOM-2 (N16 Variant).*
