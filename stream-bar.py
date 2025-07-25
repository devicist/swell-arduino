import serial
import time
import numpy as np

# Serial port settings
SERIAL_PORT = '/dev/cu.usbmodem1101'  # Change to your Arduino port (e.g., COM3 on Windows, /dev/tty.usbserial-xxxx on Mac)
BAUD_RATE = 1000000

VIDEO_WIDTH = 24
VIDEO_HEIGHT = 33
HEADER = bytes([0xFE, 0xFE, 0xFD])

SETTINGS_HEADER = bytes([0xFE, 0xFE, 0xFC])

import random

def random_color():
    return [random.randint(0, 255) for _ in range(3)]

def random_brightness():
    return random.randint(1, 127)


def make_bar_frame(x):
    """Create a 24x33 frame with a vertical white bar at column x and adjacent columns at decreasing brightness, all others blue (high bit 0, lower 7 bits = brightness)."""
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


def main():
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        print(f"Opened serial port {SERIAL_PORT} at {BAUD_RATE} baud.")
    except serial.SerialException as e:
        print(f"Failed to open serial port: {e}")
        return

    try:
        x = 0
        dx = 1
        loop_count = 0
        color0 = random_color()
        color1 = random_color()
        brightness = random_brightness()
        while True:
            frame = make_bar_frame(x)
            frame_bytes = frame.flatten()
            checksum = int(np.sum(frame_bytes) % 256)
            packet = HEADER + frame_bytes.tobytes() + bytes([checksum])
            frame_2d = frame.reshape((VIDEO_HEIGHT, VIDEO_WIDTH))
            print("\nFrame array:")
            for row in frame_2d:
                print(' '.join(f'{val:02X}' for val in row))
            print(f"\nPosition x={x}, checksum: {checksum:02X}, total bytes: {len(packet)}")
            ser.write(packet)
            x += dx
            if x >= VIDEO_WIDTH - 1 or x <= 0:
                dx *= -1
                loop_count += 1
                # After each full loop, send a new settings packet
                color0 = random_color()
                color1 = random_color()
                brightness = random_brightness()
                settings_payload = bytes(color0 + color1 + [brightness])
                settings_checksum = sum(settings_payload) % 256
                settings_packet = SETTINGS_HEADER + settings_payload + bytes([settings_checksum])
                print(f"Sending settings packet: color0={color0}, color1={color1}, brightness={brightness}, checksum={settings_checksum:02X}")
                ser.write(settings_packet)
            time.sleep(0.05)
    except KeyboardInterrupt:
        print("Exiting...")
    finally:
        ser.close()
        print("Serial port closed.")


if __name__ == "__main__":
    main() 