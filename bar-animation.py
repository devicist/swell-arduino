#!/usr/bin/env python3
"""
Bar Animation Control Script
===========================

This Python script controls a WS2813 LED strip display (647 LEDs) via serial communication
with an Arduino. It generates animated bar patterns and sends them as video frames,
while also handling color/brightness settings and monitoring incoming sensor data.

Features:
- Generates animated vertical bars that move across the display
- Sends 24x33 video frames to Arduino via serial protocol
- Processes color and brightness settings with spacebar input
- Monitors incoming serial data (distance sensor readings)
- Non-blocking keyboard input handling
- Background serial monitoring thread

Hardware Requirements:
- Arduino/ESP8266/ESP32 with WS2813 LED strip
- VL53L1X distance sensor (optional)
- Serial connection to Arduino

Serial Protocol:
- Video frames: 0xFE 0xFE 0xFD + [792 bytes] + [checksum]
- Settings: 0xFE 0xFE 0xFC + [7 bytes] + [checksum]
- Distance data: Received as plain text from Arduino

Usage:
- Run script to start animation
- Press SPACEBAR to change colors and brightness
- Press CTRL+C to exit
- Distance readings will be printed automatically

Author: 2024
License: GNU General Public License v3.0
"""

import serial
import time
import numpy as np
import sys
import select
import tty
import termios
import threading

# Serial Communication Configuration
# ===============================
SERIAL_PORT = '/dev/cu.usbmodem11201'  # Change to your Arduino port (e.g., COM3 on Windows, /dev/tty.usbserial-xxxx on Mac)
BAUD_RATE = 1000000  # High-speed serial communication

# Video Frame Configuration
# ========================
VIDEO_WIDTH = 24   # Width of video frame
VIDEO_HEIGHT = 33  # Height of video frame
HEADER = bytes([0xFE, 0xFE, 0xFD])  # Video frame packet header

# Settings Packet Configuration
# ===========================
SETTINGS_HEADER = bytes([0xFE, 0xFE, 0xFC])  # Settings packet header

import random

def random_color():
    """
    Generate a random RGB color.
    
    Returns:
        list: RGB color values [R, G, B] in range 0-255
    """
    return [random.randint(0, 255) for _ in range(3)]

def random_brightness():
    """
    Generate a random brightness value.
    
    Returns:
        int: Brightness value in range 1-127
    """
    return random.randint(1, 127)


def make_bar_frame(x):
    """
    Create a 24x33 frame with a vertical white bar at column x and adjacent 
    columns at decreasing brightness, all others blue.
    
    The frame uses a special encoding:
    - High bit (bit 7): Color selection (0=blue, 1=white)
    - Lower 7 bits: Brightness value (0-127)
    
    Args:
        x (int): Column position for the center of the bar
        
    Returns:
        numpy.ndarray: 24x33 frame array with encoded pixel values
    """
    frame = np.zeros((VIDEO_HEIGHT, VIDEO_WIDTH), dtype=np.uint8)
    # Set all to blue, full brightness (high bit 0, lower 7 bits 127)
    frame[:, :] = 0x7F
    # Set the bar and adjacent columns to white (high bit 1) with decreasing brightness
    brightness_levels = [127, int(127 * 0.75), int(127 * 0.5), int(127 * 0.25), 0, 0]
    for offset, brightness in enumerate(brightness_levels):
        for sign in [-1, 1]:
            col = x + sign * offset
            if 0 <= col < VIDEO_WIDTH and brightness > 0:
                frame[:, col] = (0x80 | brightness)  # high bit 1 for white, lower 7 bits for brightness
    # Center column (x) always full white
    if 0 <= x < VIDEO_WIDTH:
        frame[:, x] = 0x80 | 0x7F  # high bit 1, lower 7 bits full brightness
    return frame


def kbhit():
    """
    Check if a keypress is waiting to be read from stdin (non-blocking).
    
    Returns:
        bool: True if a key is available, False otherwise
    """
    dr, dw, de = select.select([sys.stdin], [], [], 0)
    return dr != []

def getch():
    """
    Read a single character from stdin.
    
    Returns:
        str: Single character from keyboard input
    """
    return sys.stdin.read(1)


def serial_monitor(ser):
    """
    Continuously monitor serial port for incoming data and print it.
    
    This function runs in a background thread and handles all incoming
    serial data from the Arduino, including distance sensor readings.
    
    Args:
        ser (serial.Serial): Serial port object to monitor
    """
    while True:
        try:
            if ser.in_waiting > 0:
                # Read all available data
                data = ser.read(ser.in_waiting)
                if data:
                    # Try to decode as UTF-8 string first
                    try:
                        decoded = data.decode('utf-8', errors='replace')
                        print(f"[Serial In] {decoded}", end='', flush=True)
                    except UnicodeDecodeError:
                        # If it's not valid UTF-8, print as hex
                        hex_data = ' '.join(f'{b:02X}' for b in data)
                        print(f"[Serial In] HEX: {hex_data}")
            time.sleep(0.01)  # Small delay to prevent excessive CPU usage
        except Exception as e:
            print(f"Error in serial monitor: {e}")
            break


def main():
    """
    Main program function.
    
    Sets up serial communication, starts background monitoring thread,
    and runs the main animation loop with keyboard input handling.
    """
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        print(f"Opened serial port {SERIAL_PORT} at {BAUD_RATE} baud.")
    except serial.SerialException as e:
        print(f"Failed to open serial port: {e}")
        return

    # Set terminal to raw mode for non-blocking keypress
    fd = sys.stdin.fileno()
    old_settings = termios.tcgetattr(fd)
    tty.setcbreak(fd)

    # Start serial monitor thread
    monitor_thread = threading.Thread(target=serial_monitor, args=(ser,), daemon=True)
    monitor_thread.start()
    print("Serial monitor started - all incoming data will be printed.")

    try:
        # Animation state variables
        x = 0          # Current bar position
        dx = 1         # Direction of movement
        color0 = random_color()  # Primary color (blue)
        color1 = random_color()  # Secondary color (white)
        brightness = random_brightness()  # LED brightness
        
        # Main animation loop
        while True:
            # Generate frame with animated bar
            frame = make_bar_frame(x)
            frame_bytes = frame.flatten()
            checksum = int(np.sum(frame_bytes) % 256)
            packet = HEADER + frame_bytes.tobytes() + bytes([checksum])
            frame_2d = frame.reshape((VIDEO_HEIGHT, VIDEO_WIDTH))
            # print("\nFrame array:")
            # for row in frame_2d:
            #     print(' '.join(f'{val:02X}' for val in row))
            # print(f"\nPosition x={x}, checksum: {checksum:02X}, total bytes: {len(packet)}")
            ser.write(packet)
            
            # Update bar position
            x += dx
            if x >= VIDEO_WIDTH - 1 or x <= 0:
                dx *= -1  # Reverse direction at edges
                
            # Check for keyboard input
            if kbhit():
                ch = getch()
                if ch == ' ':
                    # Spacebar: Change colors and brightness
                    color0 = random_color()
                    color1 = random_color()
                    brightness = random_brightness()
                    settings_payload = bytes(color0 + color1 + [brightness])
                    settings_checksum = sum(settings_payload) % 256
                    settings_packet = SETTINGS_HEADER + settings_payload + bytes([settings_checksum])
                    print(f"Sending settings packet: color0={color0}, color1={color1}, brightness={brightness}, checksum={settings_checksum:02X}")
                    ser.write(settings_packet)
                elif ch == '\x03':  # Ctrl-C
                    break
            time.sleep(0.05)  # Animation frame rate
    except KeyboardInterrupt:
        print("Exiting...")
    finally:
        # Restore terminal settings and close serial port
        termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        ser.close()
        print("Serial port closed.")


if __name__ == "__main__":
    main() 