/*
   Serial Control LED Display System
   =================================

   This Arduino sketch controls a WS2813 LED strip display (647 LEDs) using serial communication
   from a Python script. It receives video frame data and settings packets, and continuously
   reads distance data from a VL53L1X time-of-flight sensor.

   Features:
   - Receives 24x33 video frames via serial and maps them to LED coordinates
   - Processes color and brightness settings in real-time
   - Non-blocking VL53L1X distance sensor reading with automatic data transmission
   - Supports two-color mapping (color0 and color1) with brightness control
   - FastLED library integration for efficient LED control

   Hardware Requirements:
   - Xiao ESP32 or QTPy board
   - WS2813 LED strip (647 LEDs)
   - VL53L1X time-of-flight distance sensor
   - I2C connections: SDA=D4, SCL=D5
   - LED data pin: D3

   Serial Protocol:
   - Video frames: 0xFE 0xFE 0xFD + [792 bytes] + [checksum]
   - Settings: 0xFE 0xFE 0xFC + [7 bytes] + [checksum]
   - Distance data: Automatically sent as plain text when new readings available

   Author: Nicholas Stedman, Devicist

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <FastLED.h> // https://github.com/FastLED/FastLED
#include <Melopero_VL53L1X.h>

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

// LED Configuration
// =====================
#define DATA_PIN D3           // LED data pin
#define LED_TYPE WS2813       // LED strip type
#define COLOR_ORDER BRG       // Color order for WS2813
#define DEFAULT_BRIGHTNESS 32 // Default LED brightness (0-255)

// LED Mapping Configuration
// ========================
// The LED coordinates are mapped from a 24x33 video grid to 647 physical LEDs
// This mapping data was generated using the LED Mapper tool:
// https://jasoncoon.github.io/led-mapper/

#define NUM_LEDS 647

// X-coordinates for each LED (0-23 range)
const byte coordsX[] = {
    2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
    2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
    3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
    3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
    3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
    4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
    5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
    6, 7, 8, 9, 10, 11, 12, 13, 14, 18, 19, 20, 21, 22,
    8, 9, 10, 11, 12, 13, 22,
    21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2,
    21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2,
    22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,
    22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,
    22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,
    22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,
    22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,
    21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,
    21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,
    21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
    21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
    21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
    22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
    22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
    22, 21, 20, 19, 18, 17, 16, 15, 10, 9, 8, 7, 6, 5, 4, 3, 2,
    21, 20, 19, 18, 17, 8, 7, 6, 5, 4};

// Y-coordinates for each LED (0-32 range)
const byte coordsY[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    15, 15, 15, 15, 15, 15, 15,
    32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
    30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
    29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
    28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28,
    27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
    25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
    24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
    23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
    22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
    18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18,
    17, 17, 17, 17, 17, 17, 17, 17, 17, 17};

// LED Array and Color Configuration
// ===============================
CRGB leds[NUM_LEDS]; // FastLED array for all LEDs

// Default colors for video mapping (can be changed via serial settings)
CRGB color0 = CRGB::Blue;  // Primary color (used when high bit = 0)
CRGB color1 = CRGB::White; // Secondary color (used when high bit = 1)

#define VIDEO_WIDTH 24
#define VIDEO_HEIGHT 33
#define VIDEO_PIXELS (VIDEO_WIDTH * VIDEO_HEIGHT) // 792 bytes per frame

// Serial Communication Protocol
// ============================
// Packet headers for different message types
#define HEADER_1 0xFE
#define HEADER_2 0xFE
#define FRAME_TYPE 0xFD    // Video frame data
#define SETTINGS_TYPE 0xFC // Color/brightness settings

// Packet sizes
#define FRAME_SIZE (3 + VIDEO_PIXELS + 1) // header + payload + checksum
#define SETTINGS_PAYLOAD_SIZE 7
#define SETTINGS_PACKET_SIZE (3 + SETTINGS_PAYLOAD_SIZE + 1)

// Packet type enumeration for state machine
#define PACKET_TYPE_NONE 0
#define PACKET_TYPE_VIDEO 1
#define PACKET_TYPE_SETTINGS 2

// Global Variables
// ================
volatile byte receivedPacketType = PACKET_TYPE_NONE;
static byte videoFrame[VIDEO_PIXELS];              // Buffer for incoming video data
static byte settingsPacket[SETTINGS_PAYLOAD_SIZE]; // Buffer for settings data

// VL53L1X Distance Sensor Configuration
// ====================================
Melopero_VL53L1X sensor;
int vl53l1x_distance = 0;                         // Current distance reading in mm
bool vl53l1x_measurement_started = false;         // Track if measurement is active
unsigned long vl53l1x_measurement_start_time = 0; // Timeout tracking

/**
 * Serial Packet Receiver - State Machine
 * =====================================
 *
 * This function implements a state machine to receive and parse serial packets
 * from the Python control script. It handles three types of packets:
 * - Video frames (792 bytes + checksum)
 * - Settings packets (7 bytes + checksum)
 *
 * Packet Format:
 * - Header: 0xFE 0xFE
 * - Type: 0xFD (video), 0xFC (settings), 0xFB (distance request)
 * - Payload: Variable length data
 * - Checksum: Sum of all payload bytes modulo 256
 *
 * @return true if a complete packet was received and validated
 */
bool receivePacket()
{
  static enum { WAIT_HEADER1,
                WAIT_HEADER2,
                WAIT_TYPE,
                READ_PAYLOAD,
                READ_CHECKSUM } state = WAIT_HEADER1;
  static int payloadIndex = 0;
  static byte receivedChecksum = 0;
  static byte packetType = PACKET_TYPE_NONE;
  static int expectedPayloadSize = 0;
  while (Serial.available())
  {
    byte b = Serial.read();
    switch (state)
    {
    case WAIT_HEADER1:
      if (b == HEADER_1)
      {
        state = WAIT_HEADER2;
      }
      break;
    case WAIT_HEADER2:
      if (b == HEADER_2)
      {
        state = WAIT_TYPE;
      }
      else
      {
        state = WAIT_HEADER1;
      }
      break;
    case WAIT_TYPE:
      if (b == FRAME_TYPE)
      {
        packetType = PACKET_TYPE_VIDEO;
        expectedPayloadSize = VIDEO_PIXELS;
        payloadIndex = 0;
        state = READ_PAYLOAD;
      }
      else if (b == SETTINGS_TYPE)
      {
        packetType = PACKET_TYPE_SETTINGS;
        expectedPayloadSize = SETTINGS_PAYLOAD_SIZE;
        payloadIndex = 0;
        state = READ_PAYLOAD;
      }
      else
      {
        state = WAIT_HEADER1;
      }
      break;
    case READ_PAYLOAD:
      if (packetType == PACKET_TYPE_VIDEO)
      {
        videoFrame[payloadIndex++] = b;
      }
      else if (packetType == PACKET_TYPE_SETTINGS)
      {
        settingsPacket[payloadIndex++] = b;
      }
      if (payloadIndex >= expectedPayloadSize)
      {
        state = READ_CHECKSUM;
      }
      break;
    case READ_CHECKSUM:
      receivedChecksum = b;
      byte calcChecksum = 0;
      if (packetType == PACKET_TYPE_VIDEO)
      {
        for (int i = 0; i < VIDEO_PIXELS; i++)
          calcChecksum += videoFrame[i];
        calcChecksum %= 256;
        if (calcChecksum == receivedChecksum)
        {
          receivedPacketType = PACKET_TYPE_VIDEO;
          state = WAIT_HEADER1;
          return true;
        }
      }
      else if (packetType == PACKET_TYPE_SETTINGS)
      {
        for (int i = 0; i < SETTINGS_PAYLOAD_SIZE; i++)
          calcChecksum += settingsPacket[i];
        calcChecksum %= 256;
        if (calcChecksum == receivedChecksum)
        {
          receivedPacketType = PACKET_TYPE_SETTINGS;
          state = WAIT_HEADER1;
          return true;
        }
      }
      state = WAIT_HEADER1;
      break;
    }
  }
  return false;
}

/**
 * Video to LED Mapping Function
 * =============================
 *
 * Maps a 24x33 video frame array to the 647 physical LEDs using the coordinate
 * mapping arrays. Each video pixel contains:
 * - High bit (bit 7): Color selection (0=color0, 1=color1)
 * - Lower 7 bits: Brightness value (0-127)
 *
 * @param video Pointer to the video frame data (792 bytes)
 * @param videoWidth Width of video frame (24)
 * @param videoHeight Height of video frame (33)
 */
void mapVideoToLeds(const byte *video, int videoWidth, int videoHeight)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    int x = coordsX[i];
    int y = coordsY[i];
    int videoIndex = y * videoWidth + x;
    byte val = video[videoIndex];
    byte brightness = val & 0x7F; // lower 7 bits
    bool colorSel = val & 0x80;   // high bit
    CRGB base = colorSel ? color1 : color0;
    leds[i] = base;
    leds[i].nscale8_video(map(brightness, 0, 127, 0, 255));
  }
}

/**
 * Process Settings Packet
 * ======================
 *
 * Updates the LED colors and brightness based on received settings packet.
 * Packet format: [R0, G0, B0, R1, G1, B1, Brightness]
 *
 * @param settings Pointer to 7-byte settings packet
 */
void processSettings(const byte *settings)
{
  // First 3 bytes: color0 (R, G, B)
  color0 = CRGB(settings[0], settings[1], settings[2]);
  // Next 3 bytes: color1 (R, G, B)
  color1 = CRGB(settings[3], settings[4], settings[5]);
  // Last byte: brightness
  FastLED.setBrightness(settings[6]);
}

/**
 * Initialize VL53L1X Distance Sensor
 * ==================================
 *
 * Sets up the VL53L1X time-of-flight sensor with optimal settings for
 * non-blocking operation. Configures I2C communication and sensor parameters.
 */
void setupVL53L1X()
{
  Wire.begin(D4, D5);
  sensor.initI2C(0x29, Wire);
  sensor.initSensor();
  sensor.setDistanceMode(VL53L1_DISTANCEMODE_LONG);
  // sensor.setMeasurementTimingBudgetMicroSeconds(66000);
  // sensor.setInterMeasurementPeriodMilliSeconds(75);
  sensor.setMeasurementTimingBudgetMicroSeconds(140000);
  sensor.setInterMeasurementPeriodMilliSeconds(150);
  sensor.clearInterruptAndStartMeasurement();
  vl53l1x_measurement_started = true;
  vl53l1x_measurement_start_time = millis();
}

/**
 * Non-blocking VL53L1X Distance Reading
 * =====================================
 *
 * Checks if new distance data is available and reads it without blocking
 * the main loop. Uses timeout protection to restart measurements if needed.
 * Automatically sends distance data to serial when new readings are available.
 */
void readVL53L1X()
{
  // Check if data is ready without blocking
  if (sensor.getMeasurementDataReady() == VL53L1_ERROR_NONE && sensor.dataReady)
  {
    // Data is ready, get the measurement
    if (sensor.getRangingMeasurementData() == VL53L1_ERROR_NONE)
    {
      vl53l1x_distance = sensor.measurementData.RangeMilliMeter;
      // Send the data immediately when we have a new reading
      sendVL53L1X();
    }

    // Start next measurement immediately
    sensor.clearInterruptAndStartMeasurement();
    vl53l1x_measurement_started = true;
    vl53l1x_measurement_start_time = millis();
  }
  else
  {
    // If no data ready, check if we should restart measurement (timeout protection)
    if (vl53l1x_measurement_started && (millis() - vl53l1x_measurement_start_time > 200))
    {
      // Timeout - restart measurement
      sensor.clearInterruptAndStartMeasurement();
      vl53l1x_measurement_start_time = millis();
    }
  }
}

/**
 * Send Distance Data to Serial
 * ============================
 *
 * Transmits the current distance reading to the Python control script
 * as plain text with newline termination.
 */
void sendVL53L1X()
{
  Serial.println(vl53l1x_distance);
}

/**
 * Arduino Setup Function
 * =====================
 *
 * Initializes all hardware components and communication:
 * - Serial communication at 1Mbps
 * - VL53L1X distance sensor
 * - FastLED library and LED strip
 * - Default brightness and color correction
 */
void setup()
{
  //  delay(3000); // 3 second delay for recovery

  Serial.begin(1000000);
  Serial.setRxBufferSize(1024);

  setupVL53L1X();

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setRgbw(RgbwDefault());
  FastLED.setCorrection(TypicalSMD5050);

  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000); // 1A

  FastLED.setBrightness(DEFAULT_BRIGHTNESS);
}

/**
 * Arduino Main Loop
 * =================
 *
 * Main program loop that runs continuously:
 * - Reads VL53L1X sensor data non-blocking
 * - Processes incoming serial packets
 * - Updates LED display with video frames
 * - Applies color/brightness settings
 *
 * The loop is optimized for maximum responsiveness with minimal blocking operations.
 */
void loop()
{
  // Read sensor as frequently as possible without blocking
  readVL53L1X();

  if (receivePacket())
  {
    if (receivedPacketType == PACKET_TYPE_VIDEO)
    {
      mapVideoToLeds(videoFrame, VIDEO_WIDTH, VIDEO_HEIGHT);
      FastLED.show();
      receivedPacketType = PACKET_TYPE_NONE;
    }
    else if (receivedPacketType == PACKET_TYPE_SETTINGS)
    {
      processSettings(settingsPacket);
      receivedPacketType = PACKET_TYPE_NONE;
    }
  }
}