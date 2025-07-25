/*
   FastLED Mapping Demo: https://github.com/jasoncoon/led-mapper
   Copyright (C) 2022 Jason Coon, Evil Genius Labs LLC

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

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

// change these to match your data pin, LED type, and color order
#define DATA_PIN D3
#define LED_TYPE WS2813
#define COLOR_ORDER BRG
#define DEFAULT_BRIGHTNESS 32

// start of data copied from LED Mapper:
// https://jasoncoon.github.io/led-mapper/#

#define NUM_LEDS 647

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
// end of data copied from LED Mapper

CRGB leds[NUM_LEDS];

// Define base colors for video mapping
CRGB color0 = CRGB::Blue;
CRGB color1 = CRGB::White;

#define FRAMES_PER_SECOND 120
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

#define VIDEO_WIDTH 24
#define VIDEO_HEIGHT 33
#define VIDEO_PIXELS (VIDEO_WIDTH * VIDEO_HEIGHT)
#define HEADER_1 0xFE
#define HEADER_2 0xFE
#define FRAME_TYPE 0xFD
#define SETTINGS_TYPE 0xFC
#define FRAME_SIZE (3 + VIDEO_PIXELS + 1) // header + payload + checksum
#define SETTINGS_PAYLOAD_SIZE 7
#define SETTINGS_PACKET_SIZE (3 + SETTINGS_PAYLOAD_SIZE + 1) // header + payload + checksum

// Unified packet receiver
#define PACKET_TYPE_NONE 0
#define PACKET_TYPE_VIDEO 1
#define PACKET_TYPE_SETTINGS 2

volatile byte receivedPacketType = PACKET_TYPE_NONE;
static byte videoFrame[VIDEO_PIXELS];
static byte settingsPacket[SETTINGS_PAYLOAD_SIZE];

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
          Serial.println("ACK");
          state = WAIT_HEADER1;
          return true;
        }
        else
        {
          Serial.println("NACK");
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
          Serial.println("SETTINGS_ACK");
          state = WAIT_HEADER1;
          return true;
        }
        else
        {
          Serial.println("SETTINGS_NACK");
        }
      }
      state = WAIT_HEADER1;
      break;
    }
  }
  return false;
}

// Map a flat video array to the LED array using coordsX and coordsY
// video: pointer to VIDEO_WIDTH * VIDEO_HEIGHT bytes (row-major order)
// videoWidth, videoHeight: dimensions of the video frame
// leds: your FastLED LED array
// color0, color1: your two base colors
void mapVideoToLeds(const byte *video, int videoWidth, int videoHeight)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    int x = coordsX[i];
    int y = coordsY[i];
    // Bounds check in case some LEDs are outside the video area
    // if (x < videoWidth && y < videoHeight)
    if (1)
    {
      int videoIndex = y * videoWidth + x;
      byte val = video[videoIndex];
      // byte val = video[i];
      byte brightness = val & 0x7F; // lower 7 bits
      bool colorSel = val & 0x80;   // high bit
      CRGB base = colorSel ? color1 : color0;
      leds[i] = base;
      leds[i].nscale8_video(map(brightness, 0, 127, 0, 255));
    }
    else
    {
      leds[i] = CRGB::Black; // or some default
    }
  }
}

void processSettings(const byte *settings)
{
  // First 3 bytes: color0 (R, G, B)
  color0 = CRGB(settings[0], settings[1], settings[2]);
  // Next 3 bytes: color1 (R, G, B)
  color1 = CRGB(settings[3], settings[4], settings[5]);
  // Last byte: brightness
  FastLED.setBrightness(settings[6]);
}

void setup()
{
  //  delay(3000); // 3 second delay for recovery

  Serial.begin(1000000);
  Serial.setRxBufferSize(1024);

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setRgbw(RgbwDefault());
  FastLED.setCorrection(TypicalSMD5050);

  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000); // 1A

  FastLED.setBrightness(DEFAULT_BRIGHTNESS);
}

void loop()
{
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