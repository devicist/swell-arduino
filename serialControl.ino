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

// Based on FastLED "100-lines-of-code" demo reel, showing just a few
// of the kinds of animation patterns you can quickly and easily
// compose using FastLED.
//
// This example also shows one easy way to define multiple
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

// change these to match your data pin, LED type, and color order
#define DATA_PIN D3
#define LED_TYPE WS2813
#define COLOR_ORDER BRG

#define BRIGHTNESS 32

// start of data copied from LED Mapper:

#define NUM_LEDS 647

byte coordsX[NUM_LEDS] = {22, 33, 44, 55, 67, 78, 89, 100, 111, 122, 133, 144, 155, 166, 177, 188, 200, 211, 11, 22, 33, 44, 55, 67, 78, 89, 100, 111, 122, 133, 144, 155, 166, 177, 188, 200, 211, 222, 11, 22, 33, 44, 55, 67, 78, 89, 100, 111, 122, 133, 144, 155, 166, 177, 188, 200, 211, 222, 233, 11, 22, 33, 44, 55, 67, 78, 89, 100, 111, 122, 133, 144, 155, 166, 177, 188, 200, 211, 222, 233, 244, 11, 22, 33, 44, 55, 67, 78, 89, 100, 111, 122, 133, 144, 155, 166, 177, 188, 200, 211, 222, 233, 244, 11, 22, 33, 44, 55, 67, 78, 89, 100, 111, 122, 133, 144, 155, 166, 177, 188, 200, 211, 222, 233, 244, 11, 22, 33, 44, 55, 67, 78, 89, 100, 111, 122, 133, 144, 155, 166, 177, 188, 200, 211, 222, 233, 244, 255, 11, 22, 33, 44, 55, 67, 78, 89, 100, 111, 122, 133, 144, 155, 166, 177, 188, 200, 211, 222, 233, 244, 255, 22, 33, 44, 55, 67, 78, 89, 100, 111, 122, 133, 144, 155, 166, 177, 188, 200, 211, 222, 233, 244, 255, 33, 44, 55, 67, 78, 89, 100, 111, 122, 133, 144, 155, 166, 177, 188, 200, 211, 222, 233, 244, 255, 33, 44, 55, 67, 78, 89, 100, 111, 122, 133, 144, 155, 166, 177, 188, 200, 211, 222, 233, 244, 255, 33, 44, 55, 67, 78, 89, 100, 111, 122, 133, 144, 155, 166, 177, 188, 200, 211, 222, 233, 244, 255, 44, 55, 67, 78, 89, 100, 111, 122, 133, 144, 155, 166, 177, 188, 200, 211, 222, 233, 244, 255, 55, 67, 78, 89, 100, 111, 122, 133, 144, 155, 166, 177, 188, 200, 211, 222, 233, 244, 255, 67, 78, 89, 100, 111, 122, 133, 144, 155, 200, 211, 222, 233, 244, 89, 100, 111, 122, 133, 144, 244, 233, 222, 211, 200, 188, 177, 166, 155, 144, 133, 122, 111, 100, 89, 78, 67, 55, 44, 33, 22, 233, 222, 211, 200, 188, 177, 166, 155, 144, 133, 122, 111, 100, 89, 78, 67, 55, 44, 33, 22, 244, 233, 222, 211, 200, 188, 177, 166, 155, 144, 133, 122, 111, 100, 89, 78, 67, 55, 44, 33, 22, 11, 244, 233, 222, 211, 200, 188, 177, 166, 155, 144, 133, 122, 111, 100, 89, 78, 67, 55, 44, 33, 22, 11, 244, 233, 222, 211, 200, 188, 177, 166, 155, 144, 133, 122, 111, 100, 89, 78, 67, 55, 44, 33, 22, 11, 244, 233, 222, 211, 200, 188, 177, 166, 155, 144, 133, 122, 111, 100, 89, 78, 67, 55, 44, 33, 22, 11, 244, 233, 222, 211, 200, 188, 177, 166, 155, 144, 133, 122, 111, 100, 89, 78, 67, 55, 44, 33, 22, 11, 233, 222, 211, 200, 188, 177, 166, 155, 144, 133, 122, 111, 100, 89, 78, 67, 55, 44, 33, 22, 11, 233, 222, 211, 200, 188, 177, 166, 155, 144, 133, 122, 111, 100, 89, 78, 67, 55, 44, 33, 22, 11, 233, 222, 211, 200, 188, 177, 166, 155, 144, 133, 122, 111, 100, 89, 78, 67, 55, 44, 33, 22, 11, 0, 233, 222, 211, 200, 188, 177, 166, 155, 144, 133, 122, 111, 100, 89, 78, 67, 55, 44, 33, 22, 11, 0, 233, 222, 211, 200, 188, 177, 166, 155, 144, 133, 122, 111, 100, 89, 78, 67, 55, 44, 33, 22, 11, 0, 244, 233, 222, 211, 200, 188, 177, 166, 155, 144, 133, 122, 111, 100, 89, 78, 67, 55, 44, 33, 22, 11, 0, 244, 233, 222, 211, 200, 188, 177, 166, 155, 144, 133, 122, 111, 100, 89, 78, 67, 55, 44, 33, 22, 11, 0, 244, 233, 222, 211, 200, 188, 177, 166, 111, 100, 89, 78, 67, 55, 44, 33, 22, 233, 222, 211, 200, 188, 89, 78, 67, 55, 44};
byte coordsY[NUM_LEDS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 120, 120, 120, 120, 120, 120, 120, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 247, 239, 239, 239, 239, 239, 239, 239, 239, 239, 239, 239, 239, 239, 239, 239, 239, 239, 239, 239, 239, 239, 239, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 231, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 223, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 199, 199, 199, 199, 199, 199, 199, 199, 199, 199, 199, 199, 199, 199, 199, 199, 199, 199, 199, 199, 199, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 191, 183, 183, 183, 183, 183, 183, 183, 183, 183, 183, 183, 183, 183, 183, 183, 183, 183, 183, 183, 183, 183, 183, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 167, 167, 167, 167, 167, 167, 167, 167, 167, 167, 167, 167, 167, 167, 167, 167, 167, 167, 167, 167, 167, 167, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 159, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 151, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 135, 135, 135, 135, 135, 135, 135, 135, 135, 135};
byte angles[NUM_LEDS] = {42, 43, 45, 47, 50, 52, 54, 56, 59, 61, 64, 66, 69, 71, 73, 76, 78, 80, 39, 40, 42, 44, 47, 49, 51, 54, 56, 59, 61, 64, 66, 69, 72, 74, 76, 79, 81, 83, 37, 39, 41, 43, 45, 48, 50, 53, 55, 58, 61, 64, 67, 69, 72, 75, 77, 80, 82, 84, 86, 36, 38, 40, 42, 44, 47, 49, 52, 55, 58, 61, 64, 67, 70, 73, 75, 78, 81, 83, 85, 88, 90, 34, 36, 38, 41, 43, 46, 48, 51, 54, 57, 61, 64, 67, 70, 73, 76, 79, 82, 84, 87, 89, 91, 33, 35, 37, 39, 42, 44, 47, 50, 53, 57, 60, 64, 67, 71, 74, 77, 80, 83, 86, 88, 91, 93, 31, 33, 35, 37, 40, 43, 46, 49, 52, 56, 60, 64, 68, 71, 75, 79, 82, 85, 88, 90, 93, 95, 97, 29, 31, 33, 35, 38, 41, 44, 48, 51, 55, 59, 64, 68, 72, 76, 80, 83, 87, 90, 92, 95, 97, 99, 29, 31, 33, 36, 39, 42, 46, 50, 54, 59, 64, 69, 73, 78, 82, 85, 89, 92, 94, 97, 99, 101, 28, 31, 33, 36, 40, 44, 48, 53, 58, 64, 69, 74, 79, 84, 88, 91, 94, 97, 99, 101, 103, 25, 28, 30, 33, 37, 41, 46, 52, 58, 64, 70, 76, 81, 86, 90, 94, 97, 100, 102, 104, 106, 22, 24, 27, 30, 34, 38, 43, 50, 56, 64, 71, 78, 84, 89, 94, 97, 100, 103, 105, 107, 109, 21, 23, 26, 30, 34, 40, 47, 55, 64, 73, 81, 88, 93, 98, 101, 104, 107, 109, 110, 112, 19, 21, 25, 29, 35, 43, 52, 64, 75, 85, 93, 98, 103, 106, 109, 111, 112, 114, 115, 16, 19, 23, 28, 36, 48, 64, 79, 91, 111, 114, 115, 117, 118, 15, 19, 26, 40, 64, 88, 121, 170, 172, 174, 176, 179, 181, 183, 186, 189, 191, 194, 196, 199, 201, 204, 206, 208, 211, 213, 215, 169, 171, 173, 175, 178, 180, 183, 186, 188, 191, 194, 197, 200, 202, 205, 207, 210, 212, 214, 216, 165, 167, 170, 172, 174, 177, 180, 182, 185, 188, 191, 194, 197, 200, 203, 206, 208, 211, 213, 215, 217, 219, 164, 166, 168, 171, 173, 176, 179, 182, 185, 188, 191, 194, 198, 201, 204, 207, 209, 212, 214, 217, 219, 221, 162, 164, 167, 169, 172, 175, 178, 181, 184, 188, 191, 195, 198, 202, 205, 208, 211, 213, 216, 218, 220, 222, 160, 162, 165, 167, 170, 173, 176, 180, 184, 187, 191, 195, 199, 203, 206, 209, 212, 215, 218, 220, 222, 224, 158, 160, 163, 165, 168, 172, 175, 179, 183, 187, 191, 196, 200, 204, 207, 211, 214, 217, 220, 222, 224, 226, 158, 161, 163, 166, 170, 173, 177, 182, 186, 191, 196, 201, 205, 209, 213, 216, 219, 222, 224, 226, 228, 156, 158, 161, 164, 167, 171, 176, 181, 186, 191, 197, 202, 207, 211, 215, 219, 222, 224, 227, 229, 231, 153, 155, 158, 161, 165, 169, 174, 179, 185, 191, 197, 203, 209, 214, 218, 222, 225, 227, 230, 232, 233, 235, 150, 152, 155, 158, 161, 166, 171, 177, 184, 191, 199, 205, 212, 217, 221, 225, 228, 231, 233, 235, 236, 238, 146, 148, 151, 154, 157, 162, 167, 174, 182, 191, 200, 208, 215, 221, 225, 229, 232, 234, 236, 238, 239, 240, 141, 143, 144, 146, 149, 152, 157, 162, 170, 180, 191, 203, 212, 220, 226, 230, 234, 236, 238, 240, 241, 242, 243, 137, 138, 140, 141, 144, 146, 150, 156, 164, 176, 191, 207, 219, 227, 232, 236, 239, 241, 243, 244, 245, 246, 247, 134, 134, 135, 136, 137, 139, 142, 146, 229, 236, 240, 243, 245, 246, 247, 248, 249, 130, 130, 130, 131, 132, 250, 251, 252, 252, 252};
byte radii[NUM_LEDS] = {255, 248, 242, 237, 232, 228, 224, 222, 220, 218, 218, 218, 220, 222, 224, 228, 232, 237, 251, 244, 237, 231, 225, 220, 215, 212, 209, 207, 205, 205, 205, 207, 209, 212, 215, 220, 225, 231, 241, 233, 226, 219, 213, 207, 203, 199, 196, 193, 192, 192, 192, 193, 196, 199, 203, 207, 213, 219, 226, 230, 222, 214, 207, 201, 195, 190, 186, 183, 180, 179, 178, 179, 180, 183, 186, 190, 195, 201, 207, 214, 222, 220, 212, 204, 196, 189, 183, 178, 173, 170, 167, 166, 165, 166, 167, 170, 173, 178, 183, 189, 196, 204, 212, 210, 201, 193, 185, 178, 171, 166, 161, 157, 154, 153, 152, 153, 154, 157, 161, 166, 171, 178, 185, 193, 201, 201, 192, 183, 174, 167, 160, 154, 149, 144, 141, 139, 139, 139, 141, 144, 149, 154, 160, 167, 174, 183, 192, 201, 192, 182, 173, 164, 156, 149, 142, 136, 132, 128, 126, 126, 126, 128, 132, 136, 142, 149, 156, 164, 173, 182, 192, 173, 164, 154, 146, 138, 130, 124, 119, 115, 113, 112, 113, 115, 119, 124, 130, 138, 146, 154, 164, 173, 184, 155, 145, 136, 127, 119, 112, 107, 103, 100, 99, 100, 103, 107, 112, 119, 127, 136, 145, 155, 165, 176, 147, 136, 126, 117, 108, 101, 95, 90, 87, 86, 87, 90, 95, 101, 108, 117, 126, 136, 147, 158, 169, 139, 128, 118, 108, 98, 90, 83, 77, 74, 73, 74, 77, 83, 90, 98, 108, 118, 128, 139, 151, 163, 121, 110, 99, 89, 80, 71, 65, 61, 59, 61, 65, 71, 80, 89, 99, 110, 121, 133, 145, 157, 103, 92, 81, 70, 61, 53, 48, 46, 48, 53, 61, 70, 81, 92, 103, 115, 128, 140, 153, 86, 74, 62, 52, 42, 36, 33, 36, 42, 86, 98, 111, 123, 136, 56, 44, 33, 24, 20, 24, 134, 237, 231, 225, 220, 215, 212, 209, 207, 205, 205, 205, 207, 209, 212, 215, 220, 225, 231, 237, 244, 226, 219, 213, 207, 203, 199, 196, 193, 192, 192, 192, 193, 196, 199, 203, 207, 213, 219, 226, 233, 222, 214, 207, 201, 195, 190, 186, 183, 180, 179, 178, 179, 180, 183, 186, 190, 195, 201, 207, 214, 222, 230, 212, 204, 196, 189, 183, 178, 173, 170, 167, 166, 165, 166, 167, 170, 173, 178, 183, 189, 196, 204, 212, 220, 201, 193, 185, 178, 171, 166, 161, 157, 154, 153, 152, 153, 154, 157, 161, 166, 171, 178, 185, 193, 201, 210, 192, 183, 174, 167, 160, 154, 149, 144, 141, 139, 139, 139, 141, 144, 149, 154, 160, 167, 174, 183, 192, 201, 182, 173, 164, 156, 149, 142, 136, 132, 128, 126, 126, 126, 128, 132, 136, 142, 149, 156, 164, 173, 182, 192, 164, 154, 146, 138, 130, 124, 119, 115, 113, 112, 113, 115, 119, 124, 130, 138, 146, 154, 164, 173, 184, 155, 145, 136, 127, 119, 112, 107, 103, 100, 99, 100, 103, 107, 112, 119, 127, 136, 145, 155, 165, 176, 147, 136, 126, 117, 108, 101, 95, 90, 87, 86, 87, 90, 95, 101, 108, 117, 126, 136, 147, 158, 169, 180, 139, 128, 118, 108, 98, 90, 83, 77, 74, 73, 74, 77, 83, 90, 98, 108, 118, 128, 139, 151, 163, 174, 133, 121, 110, 99, 89, 80, 71, 65, 61, 59, 61, 65, 71, 80, 89, 99, 110, 121, 133, 145, 157, 169, 140, 128, 115, 103, 92, 81, 70, 61, 53, 48, 46, 48, 53, 61, 70, 81, 92, 103, 115, 128, 140, 153, 165, 136, 123, 111, 98, 86, 74, 62, 52, 42, 36, 33, 36, 42, 52, 62, 74, 86, 98, 111, 123, 136, 149, 162, 134, 121, 108, 95, 82, 69, 56, 44, 33, 44, 56, 69, 82, 95, 108, 121, 134, 119, 106, 93, 80, 66, 53, 66, 80, 93, 106};
// end of data copied from LED Mapper

CRGB leds[NUM_LEDS];

#define FRAMES_PER_SECOND 120
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

uint8_t offset = 0; // rotating "base color" used by many of the patterns
uint8_t speed = 30;

boolean autoplay = true;
uint8_t autoplaySeconds = 2;

void setup()
{
  //  delay(3000); // 3 second delay for recovery

  Serial.begin(1000000);

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setRgbw(RgbwDefault());
  FastLED.setCorrection(TypicalSMD5050);

  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000); // 1A

  FastLED.setBrightness(BRIGHTNESS);
}

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList patterns = {
    // 2D map examples:
    clockwisePalette,
    counterClockwisePalette,
    outwardPalette,
    inwardPalette,
    northPalette,
    northEastPalette,
    eastPalette,
    southEastPalette,
    // southPalette,
    // southWestPalette,
    // westPalette,
    // northWestPalette,

    // standard FastLED demo reel examples:
    //  rainbow,
    //  rainbowWithGlitter,
    //  confetti,
    //  sinelon,
    //  juggle,
    //  bpm
};

const uint8_t patternCount = ARRAY_SIZE(patterns);

uint8_t currentPatternIndex = 0; // Index number of which pattern is current

CRGBPalette16 IceColors_p = CRGBPalette16(CRGB::Black, CRGB::Blue, CRGB::Aqua, CRGB::White);

const CRGBPalette16 palettes[] = {
    // RainbowColors_p,
    // RainbowStripeColors_p,
    CloudColors_p,
    // LavaColors_p,
    OceanColors_p,
    // ForestColors_p,
    // PartyColors_p,
    // HeatColors_p,
    IceColors_p,
};

const uint8_t paletteCount = ARRAY_SIZE(palettes);

uint8_t currentPaletteIndex = 0;
CRGBPalette16 currentPalette = palettes[currentPaletteIndex];

boolean autoplayPalettes = true;
uint8_t autoplayPaletteSeconds = autoplaySeconds * patternCount;

void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  patterns[currentPatternIndex]();

  offset = beat8(speed);

  // do some periodic updates
  EVERY_N_SECONDS(autoplaySeconds)
  {
    if (autoplay)
    {
      nextPattern(); // change patterns periodically
    }
  }

  EVERY_N_SECONDS(autoplayPaletteSeconds)
  {
    // change palettes periodically
    if (autoplayPalettes)
    {
      nextPalette();
    }
  }

  // send the 'leds' array out to the actual LED strip
  // FastLED.show(); called automatically, internally by FastLED.delay below:

  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  currentPatternIndex = (currentPatternIndex + 1) % patternCount;
}

void nextPalette()
{
  // add one to the current palette number, and wrap around at the end
  currentPaletteIndex = (currentPaletteIndex + 1) % paletteCount;
  currentPalette = palettes[currentPaletteIndex];
}

// 2D map examples:

void clockwisePalette()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(currentPalette, offset + angles[i]);
  }
}

void counterClockwisePalette()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(currentPalette, offset - angles[i]);
  }
}

void outwardPalette()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(currentPalette, offset - radii[i]);
  }
}

void inwardPalette()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(currentPalette, offset + radii[i]);
  }
}

void northPalette()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(currentPalette, offset - coordsY[i]);
  }
}

void northEastPalette()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(currentPalette, offset - (coordsX[i] + coordsY[i]));
  }
}

void eastPalette()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(currentPalette, offset - coordsX[i]);
  }
}

void southEastPalette()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(currentPalette, offset - coordsX[i] + coordsY[i]);
  }
}

void southPalette()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(currentPalette, offset + coordsY[i]);
  }
}

void southWestPalette()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(currentPalette, offset + coordsX[i] + coordsY[i]);
  }
}

void westPalette()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(currentPalette, offset + coordsX[i]);
  }
}

void northWestPalette()
{
  for (uint16_t i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(currentPalette, offset + coordsX[i] - coordsY[i]);
  }
}

// standard FastLED demo reel examples:

void rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow(leds, NUM_LEDS, offset, 7);
}

void rainbowWithGlitter()
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter(fract8 chanceOfGlitter)
{
  if (random8() < chanceOfGlitter)
  {
    leds[random16(NUM_LEDS)] += CRGB::White;
  }
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV(offset + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS - 1);
  leds[pos] += CHSV(offset, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  for (int i = 0; i < NUM_LEDS; i++)
  { // 9948
    leds[i] = ColorFromPalette(palette, offset + (i * 2), beat - offset + (i * 10));
  }
}

const byte dotCount = 3;
const byte hues = 240 / dotCount;

void juggle()
{
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy(leds, NUM_LEDS, 20);
  for (int i = 0; i < dotCount; i++)
  {
    leds[beatsin16(i + 7, 0, NUM_LEDS - 1)] |= CHSV(i * hues, 200, 255);
  }
}