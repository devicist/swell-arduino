#include <Melopero_VL53L1X.h>

Melopero_VL53L1X sensor;

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

void loop()
{
  sensor.waitMeasurementDataReady();
  sensor.getRangingMeasurementData();
  sensor.clearInterruptAndStartMeasurement();

  Serial.print(sensor.measurementData.RangeMilliMeter);
  Serial.println(" mm");
}