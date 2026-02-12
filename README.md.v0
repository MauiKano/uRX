Hybrid NAVTEX & Wideband Receiver (ESP32 + Tayloe Mixer + Si4732)
Overview
This project is a high-performance, dual-path radio receiver designed for the ESP32 platform. It combines a traditional digital "Radio-on-a-Chip" (Si4732) with a high-sensitivity, software-defined Tayloe Quadrature Product Detector (QPD). The design is specifically optimized for the reception and decoding of NAVTEX signals at 518 kHz, utilizing a professional-grade analog signal chain and high-resolution external ADCs.
Key Features
1. Dual Receiver Architecture

    Path A (SDR): A high-linearity Tayloe Mixer (using the CBT3253 bus switch) for I/Q sampling. This path is intended for precision DSP decoding of weak digital modes like NAVTEX.
    Path B (Digital): An Si4732-A10 all-in-one receiver for general-purpose listening (AM/FM/SW/LW) and RDS data acquisition.

2. High-End Analog Signal Chain

    Active Antenna Splitter: An OPA2690 high-speed current-feedback op-amp serves as an active buffer/splitter, providing high isolation between the two receiver paths without signal loss.
    Post-Mixer Amplification: A LM4562 ultra-low noise, high-fidelity audio operational amplifier provides 33.4 dB of gain (Gain=47) to the I/Q signals.
    Optimized Filtering: Integrated active low-pass filtering with a 2.8 kHz cutoff, specifically tuned to maximize the SNR for NAVTEX/SITOR-B signals.
    Ultra-Stable Reference: A precision LM4040 (2.5V) shunt reference provides a clean virtual ground for the analog stages, ensuring maximum dynamic range and rejection of power supply noise.

3. Precision Clocking & Digitization

    Clock Generation: An Si5351A triple clock generator provides the quadrature sampling clock for the Tayloe mixer and a dedicated low-jitter master clock for the ADC.
    External ADC: Instead of the noisy internal ESP32 ADC, this design uses the PCM1808 (24-bit, 99dB SNR). It interfaces via I2S, providing a transparent digital stream to the ESP32 for software demodulation.

4. EMI/EMC Optimized Layout

    Power Isolation: Independent Ferrite Bead filtering for every major IC to prevent digital switching noise from the ESP32 and Si5351 from polluting the analog front-end.
    Ground Management: Strict separation of analog and digital ground paths, joined at a single star-point under the ADC to minimize ground loops.

Technical Specifications

    Primary Frequency: 518 kHz (NAVTEX)
    Mixer Type: Quadrature Sampling Detector (Tayloe)
    ADC Resolution: 24-Bit I2S (PCM1808)
    Processor: ESP32 (Dual-Core 240 MHz)
    I/Q Bandwidth: ~3 kHz (Optimized for digital modes)

