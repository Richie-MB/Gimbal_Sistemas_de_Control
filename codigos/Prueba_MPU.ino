#include <Wire.h>
#include <MPU6050_tockn.h>

MPU6050 mpu6050(Wire);
unsigned long timer = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  // Calibración inicial (Crucial para que los valores empiecen en 0)
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

void loop() {
  mpu6050.update();

  // Enviamos datos cada 50ms para no saturar el puerto
  if (millis() - timer > 50) {
    
    // Formato compatible con Serial Monitor y Serial Plotter
    Serial.print("Roll:");  Serial.print(mpu6050.getAngleX());
    Serial.print(" Pitch:"); Serial.print(mpu6050.getAngleY());
    Serial.print(" Yaw:");   Serial.println(mpu6050.getAngleZ());

    timer = millis();
  }
}