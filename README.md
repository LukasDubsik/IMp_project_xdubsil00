# IMP Project: Filesystems on ESP32 (LittleFS + FATFS/SD) + Python UART Client

#### Author: Lukas Dubsik  
#### Login: xdubsil00  
#### Date: 18.12.2025  
#### Project adviser: Ing. Josef Strnadel, Ph. D.

---

## Table of Contents

- [Introduction](#introduction)
- [Repository structure](#repository-structure)
- [Overview of file systems for embedded platforms](#overview-of-file-systems-for-embedded-platforms)
  - [Comparison table](#comparison-table)
  - [Why I chose LittleFS + FATFS](#why-i-chose-littlefs--fatfs)
- [Chosen platform and environment](#chosen-platform-and-environment)
  - [Hardware](#hardware)
  - [Development environment](#development-environment)
- [Solution architecture](#solution-architecture)
  - [External components and wiring](#external-components-and-wiring)
  - [Modes 1–4](#modes-14)
- [Implementation details](#implementation-details)
  - [VFS layer and portability](#vfs-layer-and-portability)
  - [Internal flash: LittleFS](#internal-flash-littlefs)
  - [External SD card: FATFS](#external-sd-card-fatfs)
  - [Interactive filesystem shell](#interactive-filesystem-shell)
  - [Python UART client](#python-uart-client)
  - [Statistics and benchmarking](#statistics-and-benchmarking)
- [Compilation and execution](#compilation-and-execution)
  - [Firmware (ESP-IDF)](#firmware-esp-idf)
  - [Python client (Raspberry Pi)](#python-client-raspberry-pi)
- [Testing](#testing)
- [Known limitations](#known-limitations)
- [Sources and reused parts](#sources-and-reused-parts)
- [References](#references)

---

## Introduction

This project is about **using file systems in embedded environments**, where storage is often **raw flash** (wear, power-loss, erase blocks) and/or **external media** (SD card over SPI/SDMMC). The goal is not only to mount a filesystem, but also to **demonstrate configuration differences, mapping storage into a VFS namespace, typical operations, limits, and performance impact** on a real MCU.

The implementation targets an **ESP32 (D1 R32 style board)** and provides:

- **LittleFS on internal SPI flash** (persistent storage inside the module),
- **FATFS on an external microSD card** (FAT32, via SPI),
- a **4-mode firmware** (each mode demonstrates something different),
- a small **Python UART client** for interactive control from a Raspberry Pi.

## Repository structure

```text
.
├── D1R32/                 # ESP-IDF firmware project (ESP32)
│   ├── main/
│   │   ├── benchmark/     # performance tests
│   │   ├── filesystem/    # interactive shell + commands
│   │   ├── periferies/    # UART, SD(SPI), LittleFS, LED, keypad matrix
│   │   ├── params/        # stats, tree printing, helpers
│   │   └── ...            # glue code / tasks
│   ├── partitions.csv     # flash partition layout
│   └── sdkconfig          # project configuration (LittleFS/FATFS, etc.)
└── Pi4Screen/
    └── file_system.py     # Python serial client (interactive terminal)
```

---

## Overview of file systems for embedded platforms

Embedded storage has constraints that desktop storage usually hides:
- flash must be erased in **blocks** before writing,
- there is **wear** (finite erase/write cycles),
- power can be lost mid-operation (atomicity matters),
- RAM is limited → caches and metadata structures must be small.

### Comparison table

| FS / approach | Typical use | Strengths | Weaknesses / notes |
|---|---|---|---|
| **LittleFS** | General-purpose flash FS | Wear-leveling + good power-loss behavior; designed for MCUs/flash | Not a “desktop FS”; limited by partition size and config; needs integration layer |
| **SPIFFS** | Legacy small flash FS | Simple; widely used historically | No real directories; can get slow with many files / near-full; often treated as legacy on newer stacks |
| **FATFS (FatFs)** | Removable media (SD), compatibility | Standard FAT; easy to read on PC; mature library | Metadata-heavy; lots of small updates can be expensive; needs block device layer |
| **JFFS2 / YAFFS2 / UBIFS** | Linux-ish flash setups | Proven in some embedded Linux contexts | Usually heavier; not the first choice for small MCUs (RAM/CPU footprint) |

### Why I chose LittleFS + FATFS

- **LittleFS** covers the “internal flash persistent data” case well (resilience + wear leveling).
- **FATFS on SD** covers the “external removable storage” case and keeps PC interoperability (mount the card on a laptop, inspect files).
- ESP-IDF supports both via a **VFS layer** (mountpoints like `/littlefs`, `/sdcard`).

---

## Chosen platform and environment

### Hardware

- **MCU board:** ESP32 (Wemos D1 R32 style)
- **External storage:** microSD module in **SPI mode** (FAT32)
- **Control:** matrix keypad (used rows/cols subset) + onboard LED signaling
- **Host interaction:** Raspberry Pi (UART) + Python CLI

### Development environment

- **SDK/workflow:** ESP-IDF (`idf.py`)
- **Project config:** `sdkconfig` (LittleFS/FATFS options), `partitions.csv` (flash layout)

---

### External components and wiring

**UART (ESP32 ↔ Pi):**
- Firmware uses **UART2**, pins:
  - TX: GPIO17
  - RX: GPIO16

On Raspberry Pi, the client uses `/dev/serial0` (the standard serial alias). UART must be enabled and serial console disabled.

**SD card (SPI wiring used in code):**
- MOSI: GPIO23
- MISO: GPIO19
- SCK:  GPIO18
- CS:   GPIO5

**Keypad + LED:**
- Keypad is scanned periodically (FreeRTOS task).
- LED (GPIO2) is used for status/error codes (blink sequences).

### Modes 1–4

| Mode | Storage | What it demonstrates |
|---:|---|---|
| **1** | LittleFS (`/littlefs`, partition `l_fs`) | Internal flash FS usage: create/remove files and dirs, write/append, stat, navigation |
| **2** | FATFS on SD (`/sdcard`) | External removable media + FAT semantics through the same shell |
| **3** | Both | Limits and “what is mounted”: print SD card info + LittleFS partition stats + directory trees |
| **4** | Both | Benchmark: same operations on LittleFS (dedicated `b_fs` partition) vs SD/FATFS |

---

## Implementation details

### VFS layer and portability

The firmware relies on ESP-IDF’s **VFS abstraction**, meaning:
- mounting is per-filesystem (LittleFS / FATFS),
- after mount, the rest of code is mostly filesystem-agnostic (`open`, `stat`, directory ops).

This is also why modes 1 and 2 can share the same code implementation.

### Internal flash: LittleFS

LittleFS is mounted using the ESP-IDF LittleFS integration (managed component).

**Mountpoints and partitions:**
- `l_fs` → `/littlefs` (main storage)
- `b_fs` → `/benchmark` (smaller partition reserved for benchmark runs)

**Partition layout (`partitions.csv`):**
```text
# Name,   Type, SubType, Offset,  Size
nvs,      data, nvs,     0x9000,  0x6000
phy_init, data, phy,     0xf000,  0x1000
factory,  app,  factory, 0x10000, 1M
l_fs,     data, spiffs,          500K
b_fs,     data, spiffs,           50K
```

### External SD card: FATFS

The SD card is mounted as FATFS via ESP-IDF’s FATFS/VFS support (SD over SPI or SDMMC).

Notable implementation detail: the SD SPI host is configured with a conservative clock (stability-first). This is intentional for reliability on generic SPI SD modules.

### Interactive filesystem shell

The firmware exposes a small command set (same for LittleFS and SD):

```text
help
df
pwd
ls [path]
cd <path>
mkdir <dir>
rmdir <dir>
touch <file>
rm <file>
write <file> <text>
append <file> <text>
mv <src> <dst>
stat <path>
```

The shell is executed on the ESP32; Raspberry Pi is only a terminal + transport.

### Python UART client

`Pi4Screen/file_system.py` is a minimal interactive client:

- opens `/dev/serial0` at `115200`,
- waits for the firmware prompt (`IMP@file_system:`),
- sends commands as frames terminated by `!`,
- handles special control words:
  - `start_cmd` – “client is ready”
  - `rst_cmd` – firmware requests shutdown/restart
  - `overflow_cmd` – firmware indicates RX buffer overflow

This “framed UART” is intentionally simple so that the MCU side can be robust without heavy parsing.

### Statistics and benchmarking

**Mode 3 (Stats):**
- prints SD card info and filesystem usage,
- prints a directory tree (demonstrates traversal + metadata reading cost and limits).

**Mode 4 (Benchmark):**
- compares the same operations (touch/rm/mkdir/rmdir/write/append/…) across:
  - LittleFS (dedicated benchmark partition), and
  - FATFS on SD.

Example output format (from a LittleFS run):

```text
BENCHMARK FOR LITTLEFS:
TOUCH:  suc=100 fails=0 time=1.022s avg_time=0.010s
RM:     suc=100 fails=0 time=0.932s avg_time=0.009s
MKDIR:  suc=100 fails=0 time=4.234s avg_time=0.042s
...
```

This format is kept “report-friendly” so the results can be copy-pasted directly.

---

## Compilation and execution

### Firmware (ESP-IDF)

From `D1R32/`:

```bash
# Build
idf.py build

# Flash
idf.py flash

# Monitor (optional)
idf.py monitor

# If configuration changes
idf.py reconfigure
```

### Python client (Raspberry Pi)

1) Enable UART on the Pi and make sure `/dev/serial0` is available (and console is disabled on that port).

2) Install dependency:
```bash
python3 -m pip install pyserial
```

3) Run:
```bash
cd Pi4Screen
python3 file_system.py
```

4) On the ESP32 keypad select mode **1** or **2** (interactive modes). The Python script will wait for the firmware prompt and then you can type commands.

---

## Testing

Testing is primarily **manual + functional**, because performance and behavior depend on real storage media:

- **Mode 1 (LittleFS):**
  - `mkdir test`
  - `cd test`
  - `touch a.txt`
  - `write a.txt hello`
  - `stat a.txt`
  - `rm a.txt`, `cd ..`, `rmdir test`

- **Mode 2 (SD/FATFS):**
  - same command sequence; verify that the same CLI works under `/sdcard`.

- **Mode 3:**
  - verify that stats are printed for both storages and the directory tree is produced.

- **Mode 4:**
  - verify that benchmark runs for both storages and prints timing summaries.

---

## Known limitations

- Keypad scanning uses a subset of the matrix (enough for modes 1–4 and restart), not a full-feature menu system.
- SD card SPI clock is configured conservatively for stability; this can reduce SD performance compared to “maximum speed” configurations.
- Shell commands are intentionally minimal (e.g., no recursive `rm -r`).

---

## Sources and reused parts

- LittleFS support is provided via an ESP-IDF managed component (integration layer + VFS glue).
- FATFS is used via ESP-IDF’s FATFS/VFS integration for SD cards.
- Raspberry Pi UART setup follows official Raspberry Pi documentation.

---

## References

- ESP-IDF Virtual Filesystem (VFS):  
  https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/storage/vfs.html

- ESP-IDF File System Considerations (SPIFFS vs FATFS etc.):  
  https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/file-system-considerations.html

- ESP-IDF FATFS support (incl. esp_vfs_fat_sdmmc_mount):  
  https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/storage/fatfs.html

- ESP-IDF SD/MMC driver docs:  
  https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/storage/sdmmc.html

- ESP-IDF example: SD card + FATFS mount (sdmmc):  
  https://github.com/espressif/esp-idf/blob/master/examples/storage/sd_card/sdmmc/README.md

- LittleFS ESP-IDF component (Component Registry):  
  https://components.espressif.com/components/joltwallet/littlefs/versions/1.20.3/readme

- Raspberry Pi docs: configuration / enable_uart:  
  https://www.raspberrypi.com/documentation/computers/configuration.html