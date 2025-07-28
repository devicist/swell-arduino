/*
   VL53L1X Distance Sensor Test
   ============================

   This Arduino sketch provides a simple test for the VL53L1X time-of-flight
   distance sensor. It initializes the sensor and continuously reads distance
   measurements, outputting them to the serial monitor.

   Features:
   - Basic VL53L1X sensor initialization and configuration
   - Continuous distance reading with blocking operation
   - Serial output of distance measurements in millimeters
   - Long-range mode configuration for maximum range

   Hardware Requirements:
   - ESP8266 or ESP32 board
   - VL53L1X time-of-flight distance sensor
   - I2C connections: SDA=D4, SCL=D5

   Wiring:
   - VCC: 3.3V
   - GND: Ground
   - SDA: D4
   - SCL: D5

   Usage:
   - Upload to Arduino board
   - Open Serial Monitor at 9600 baud
   - Distance readings will be displayed in mm

   Note: This is a basic test sketch. For production use, consider using
   non-blocking methods as implemented in serialControl.ino

   Author: Nicholas Stedman, Devicist, 2025
   License: GNU General Public License v3.0
*/

#include <Melopero_VL53L1X.h>

Melopero_VL53L1X sensor;

/**
 * Arduino Setup Function
 * =====================
 *
 * Initializes the VL53L1X sensor with optimal settings for distance measurement.
 * Configures I2C communication, sensor parameters, and starts continuous measurement.
 */
void setup()
{
  Serial.begin(9600);
  Wire.begin(D4, D5);
  sensor.initI2C(0x29, Wire);
  sensor.initSensor();
  sensor.setDistanceMode(VL53L1_DISTANCEMODE_LONG);
  // sensor.setMeasurementTimingBudgetMicroSeconds(66000);
  // sensor.setInterMeasurementPeriodMilliSeconds(75);
  sensor.setMeasurementTimingBudgetMicroSeconds(140000);
  sensor.setInterMeasurementPeriodMilliSeconds(150);
  sensor.clearInterruptAndStartMeasurement();
}

/**
 * Arduino Main Loop
 * =================
 *
 * Continuously reads distance measurements from the VL53L1X sensor
 * and outputs them to the serial monitor. Uses blocking operation
 * for simplicity in this test sketch.
 */
void loop()
{
  sensor.waitMeasurementDataReady();
  sensor.getRangingMeasurementData();
  sensor.clearInterruptAndStartMeasurement();

  Serial.print(sensor.measurementData.RangeMilliMeter);
  Serial.println(" mm");
}