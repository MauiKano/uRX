# uRX Antenna Subsystem: Navtex Dual-Band Ferrite Antenna

This document specifies the design and construction of the high-performance magnetic loopstick antenna for the **uRX** handheld receiver, optimized for **490 kHz** and **518 kHz** Navtex reception.

## Technical Specifications

| Parameter               | Specification              | Notes                                      |
|-------------------------|----------------------------|--------------------------------------------|
| **Primary Frequency**   | 518 kHz                    | International Navtex (English)             |
| **Secondary Frequency** | 490 kHz                    | Local Navtex (National languages)          |
| **Antenna Type**        | High-Q Magnetic Loopstick  | Optimized for MF (Medium Frequency)        |
| **Target Inductance**   | 330 µH                     | Matches [Si4732 AMI](https://www.silabs.com) requirements |
| **Rod Material**        | NiZn Ferrite (Mix 61)      | Low-loss at 500 kHz ([Fair-Rite Type 61](https://fair-rite.com)) |
| **Rod Dimensions**      | 10 mm x 80 mm              | Balanced for handheld portability          |
| **Conductor Type**      | 40/46 Litz Wire            | Minimizes skin effect, ensures high Q      |

## Winding & Construction

To achieve the target inductance of **330 µH** on the specified 10 mm rod, follow these construction steps:

*   **Turn Count:** Approximately **66 turns** (single-layer solenoid).
*   **Winding Method:** Wind the Litz wire tightly on a movable paper or plastic sleeve.
*   **Inductance Tuning:** Slide the coil toward the center of the rod to increase inductance, or toward the ends to decrease it.
*   **Resonance Tuning:**
    *   **Band Center:** 504 kHz.
    *   **Fixed Capacitor:** 270 pF (**NP0/C0G** dielectric for thermal stability).
    *   **Trimmer:** 10–60 pF parallel trimmer for fine-tuning the peak resonance.

## Interface & System Integration

*   **Buffer Stage:** The antenna feeds the [Texas Instruments OPA2211](https://www.ti.com) (U11).
*   **Input Impedance ($Z_{in}$):** Maintained at **100 kΩ** via the bias resistor (**R2**) to prevent damping of the tuned circuit.
*   **Isolation:** The dual-opamp configuration provides independent signal paths for the **Tayloe Mixer** and the **Si4732** tuner, preventing local oscillator (LO) leakage.
*   **Expected Performance:** A Q-factor of **>150**, providing excellent narrowband selectivity and rejection of out-of-band noise (e.g., from the ESP32-C3 digital lines).

---
*Documentation generated for the uRX Project - 2026*

