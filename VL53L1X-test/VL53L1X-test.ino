#include <Melopero_VL53L1X.h>

Melopero_VL53L1X sensor;

void setup()
{
  Serial.begin(9600);
  Wire1.begin();
  sensor.initI2C(0x29, Wire1);
  sensor.initSensor();
  sensor.setDistanceMode(VL53L1_DISTANCEMODE_MEDIUM);
  sensor.setMeasurementTimingBudgetMicroSeconds(66000);
  sensor.setInterMeasurementPeriodMilliSeconds(75);
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