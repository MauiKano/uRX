# uRX Tayloe Mixer & Frontend Specification

## 1. Overview
High-performance SDR frontend optimized for **518 kHz Navtex** reception. Features a resonant Ferrite Rod antenna, active buffer/splitter, and parallelized **CBT3253** Tayloe Mixer driving a differential **LM4562** instrumentation stage.

## 2. Signal Chain
`Ferrite Rod + C_tune` → `OPA2211 (Buffer/Splitter)` → `CBT3253 (Parallel Mixer)` → `LM4562 (I/Q Pre-amp)` → `PCM1808 (24-bit ADC)`

## 3. Component Details & Values

### A. Frontend Buffer (OPA2211)
* **Function:** High-impedance buffer to prevent loading the resonant antenna.
* **Gain:** 1x (Voltage Follower).
* **Bias:** Vref = 2.5 V (Virtual Ground).
* **Supply:** 5 V Filtered.

### B. Tayloe Mixer (CBT3253)
Internal switch banks are parallelized ($R_{on} \approx 2 \Omega$) to minimize thermal noise.
* **Inputs:** Pins 7 (1A) and 9 (2A) tied together.
* **Select Pins:** S0 (Pin 14) and S1 (Pin 2) driven by **Si5351A** (3.3 V TTL compatible).
* **Enable:** Pins 1 and 15 tied to GND.
* **Integration Caps:** 4x 470 nF (X7R) providing $\approx 850$ Hz RF bandwidth.
* **Input Resistors:** 4x 100 $\Omega$ for low noise and ADC gain matching.

### C. Quadrature Mapping (Mixer to OpAmp)

| FST3253 Pins (Parallel) | Phase | LM4562 Input | Function |
| :--- | :--- | :--- | :--- |
| **Pin 6 + Pin 10** | 0° | **Pin 5** (Non-Inv +) | **I-Channel** |
| **Pin 4 + Pin 12** | 180° | **Pin 6** (Inverting -) | **(I+ / I-)** |
| **Pin 5 + Pin 11** | 90° | **Pin 3** (Non-Inv +) | **Q-Channel** |
| **Pin 3 + Pin 13** | 270° | **Pin 2** (Inverting -) | **(Q+ / Q-)** |

### D. Post-Mixer Amplifier (LM4562)
* **Gain:** $\approx 100 \times$ (40 dB) via $R_{fb}=10\text{ k}\Omega$ and $R_{in}=100\text{ }\Omega$.
* **Filtering:** $C_{fb}=5.6\text{ nF}$ (Active Low-Pass, $f_c \approx 2.8$ kHz).
* **Reference:** Non-inverting pins tied to Vref via $10\text{ k}\Omega$ resistors (Bias current compensation).

## 4. Power & Grounding Strategy
* **Quiet Zone:** OPA2211, Mixer, and LM4562 run on **5 V Filtered** or **3.3 V Filtered** rails.
* **Mixed Zone:** PCM1808 and Si5351 run on a separate **3.3 V Filtered** branch.
* **Isolation:** Separate L/C star-filtering for Quiet and Mixed zones.

---
*Created for the uRX Open Source Project (GPL-3.0).*
