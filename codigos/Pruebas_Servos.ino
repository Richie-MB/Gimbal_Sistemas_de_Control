#include <Servo.h>

Servo sPitch, sRoll, sYaw;

void setup() {
  Serial.begin(115200);
  
  sPitch.attach(9);
  sRoll.attach(10);
  sYaw.attach(11);

  // Posición inicial estándar
  sPitch.write(90);
  sRoll.write(90);
  sYaw.write(90);

  Serial.println("--- Buscador de Ángulos de Reposo ---");
  Serial.println("Escribe la letra del eje y el ángulo (Ejemplo: P105, R80, Y90)");
}

void loop() {
  if (Serial.available() > 0) {
    char eje = Serial.read();      // Lee la letra (P, R o Y)
    int angulo = Serial.parseInt(); // Lee el número que sigue

    if (eje == 'P' || eje == 'p') {
      sPitch.write(angulo);
      Serial.print("Pitch movido a: "); Serial.println(angulo);
    } 
    else if (eje == 'R' || eje == 'r') {
      sRoll.write(angulo);
      Serial.print("Roll movido a: "); Serial.println(angulo);
    } 
    else if (eje == 'Y' || eje == 'y') {
      sYaw.write(angulo);
      Serial.print("Yaw movido a: "); Serial.println(angulo);
    }
  }
}