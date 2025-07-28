# Serial Control LED Display System

A complete system for controlling a WS2813 LED strip display (647 LEDs) via serial communication between a Python script and Arduino, with integrated VL53L1X distance sensor support. This system is used to control the custom developed screens in Nell Tenhaaf's Swell sculpture.

## Overview

This project consists of:

- **Arduino sketch** (`serialControl.ino`) - Controls LED strip and distance sensor
- **Python control script** (`bar-animation.py`) - Generates animations and sends commands
- **VL53L1X test sketch** (`VL53L1X-test/`) - Basic sensor testing
- **VL53L1X library** (`Melopero_VL53L1X/`) - Sensor driver library

## Features

### Hardware Support

- **WS2813 LED Strip** (647 LEDs) with custom coordinate mapping
- **VL53L1X Time-of-Flight Distance Sensor** with non-blocking operation
- **ESP8266/ESP32** microcontroller support
- **High-speed serial communication** (1Mbps)

### Software Features

- **Real-time video frame processing** (24x33 resolution)
- **Dynamic color and brightness control**
- **Non-blocking sensor reading** for maximum responsiveness
- **Background serial monitoring** with automatic data display
- **Animated bar patterns** with smooth movement
- **Keyboard input handling** (spacebar for settings, Ctrl+C to exit)

## Hardware Requirements

### Required Components

- ESP8266 or ESP32 development board
- WS2813 LED strip (647 LEDs)
- VL53L1X time-of-flight distance sensor
- USB cable for programming and communication

### Wiring Diagram

```
ESP32    WS2813 LED Strip    VL53L1X Sensor
-----    ----------------    --------------
5V          --> VCC
3.3V                           --> VCC
GND         --> GND            --> GND
D3          --> Data In
D4                             --> SDA
D5                             --> SCL
```

## Installation

### 1. Arduino Setup

1. Install Arduino IDE with ESP32 board support
2. Install required libraries:
   - FastLED (for LED control)
3. Upload `serialControl.ino` to your board

### 2. Python Setup (Optional)

1. Install Python 3.7+ and pip
2. Install required packages:
   ```bash
   pip install pyserial numpy
   ```
3. Update the `SERIAL_PORT` variable in `bar-animation.py` to match your system:
   - Windows: `COM3` (or similar)
   - macOS: `/dev/cu.usbmodem11201` (or similar)
   - Linux: `/dev/ttyUSB0` (or similar)

## Usage

### Basic Operation

1. Connect hardware according to wiring diagram
2. Upload `serialControl.ino` to Arduino
3. Run the Python script:
   ```bash
   python bar-animation.py
   ```
4. Watch the animated bar pattern on your LED strip
5. Press **SPACEBAR** to change colors and brightness
6. Press **Ctrl+C** to exit

### Advanced Features

#### Distance Sensor Monitoring

The system automatically monitors and displays distance readings from the VL53L1X sensor:

- Readings are sent automatically when new data is available
- Displayed in the Python console with `[Serial In]` prefix
- Non-blocking operation ensures smooth animation

#### Custom Animations

Modify `bar-animation.py` to create custom patterns:

- Change `make_bar_frame()` function for different visual effects
- Adjust animation speed with `time.sleep()` values
- Modify color generation in `random_color()` function

#### LED Mapping

The LED coordinate mapping is defined in `serialControl.ino`:

- `coordsX[]` and `coordsY[]` arrays map video pixels to physical LEDs
- Generated using the [LED Mapper tool](https://jasoncoon.github.io/led-mapper/)
- Supports custom LED layouts and arrangements

## Serial Protocol

### Video Frame Packet

```
Header: 0xFE 0xFE 0xFD
Data: 792 bytes (24x33 pixels)
Checksum: 1 byte (sum of data bytes modulo 256)
```

### Settings Packet

```
Header: 0xFE 0xFE 0xFC
Data: 7 bytes [R0, G0, B0, R1, G1, B1, Brightness]
Checksum: 1 byte (sum of data bytes modulo 256)
```

### Distance Data

- Sent automatically as plain text
- Format: `[distance] mm\n`
- Example: `1234 mm`

## File Structure

```
serialControl/
├── serialControl.ino          # Main Arduino sketch
├── bar-animation.py           # Python control script
├── VL53L1X-test/
│   └── VL53L1X-test.ino      # Basic sensor test
└── README.md                  # This file
```

## Configuration

### Arduino Settings

- **LED Data Pin**: D3 (configurable in `DATA_PIN`)
- **I2C Pins**: D4 (SDA), D5 (SCL)
- **Serial Speed**: 1,000,000 baud
- **LED Type**: WS2813 with BRG color order

### Python Settings

- **Serial Port**: Configurable in `SERIAL_PORT`
- **Baud Rate**: 1,000,000 baud
- **Animation Speed**: 0.05 seconds per frame
- **Video Resolution**: 24x33 pixels

## Troubleshooting

### Common Issues

#### Serial Connection Failed

- Check USB cable connection
- Verify correct serial port in Python script
- Ensure Arduino is not in use by Arduino IDE

#### LED Strip Not Working

- Verify power supply (5V, sufficient current)
- Check data pin connection (D3)
- Confirm LED count matches `NUM_LEDS` (647)

#### Distance Sensor Issues

- Check I2C wiring (D4, D5)
- Verify sensor power (3.3V)
- Test with `VL53L1X-test.ino` first

#### Animation Lag

- Reduce `time.sleep()` value in Python script
- Check serial buffer size
- Verify non-blocking sensor operation

### Debug Mode

```python
# Uncomment these lines for frame logging
print("\nFrame array:")
for row in frame_2d:
    print(' '.join(f'{val:02X}' for val in row))
print(f"\nPosition x={x}, checksum: {checksum:02X}, total bytes: {len(packet)}")
```

## Performance Optimization

### Arduino Optimizations

- Non-blocking sensor reading prevents animation lag
- Optimized timing budget (140ms) for reliable measurements
- Efficient LED update using FastLED library

### Python Optimizations

- Background serial monitoring thread
- Non-blocking keyboard input
- Optimized frame generation with NumPy

## License

This project is licensed under the GNU General Public License v3.0. See the license headers in individual files for details.

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add appropriate documentation
5. Submit a pull request

## Acknowledgments

- Based on FastLED Mapping Demo by Jason Coon
- Uses Melopero VL53L1X library for sensor control
- LED coordinate mapping generated with LED Mapper tool

## Version History

- **v1.0** - Initial release with basic animation and sensor support
- **v1.1** - Added non-blocking sensor operation
- **v1.2** - Improved documentation and error handling
