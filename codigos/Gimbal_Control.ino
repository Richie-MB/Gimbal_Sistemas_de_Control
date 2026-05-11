#include <Wire.h>
#include <MPU6050_tockn.h>
#include <Servo.h>

// Instancia del sensor y actuadores
MPU6050 mpu6050(Wire);
Servo servoPitch, servoRoll, servoYaw;

// --- PARÁMETROS DE CONTROL ---
// Ganancias Proporcionales (Kp): Definen la agresividad de la respuesta ante el error
float kpRoll = 2.2, kpPitch = 2.5, kpYaw = 0.8; 

// Vectores de dirección: Corrigen la polaridad del movimiento según el montaje mecánico
int dirRoll = -1, dirPitch = -1, dirYaw = -1;

// --- PROCESAMIENTO DE SEÑAL (FILTRADO) ---
float smoothYaw = 90; 
// Alpha del Filtro EMA (Exponential Moving Average): 0.1 indica que el 10% es la lectura
// nueva y el 90% es el historial, reduciendo el ruido de alta frecuencia.
float alpha = 0.1; 

unsigned long timerPrint = 0; 

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  // Overclock del bus I2C a 400kHz para reducir la latencia de lectura
  Wire.setClock(400000); 
  
  // Asignación de pines PWM para los servomotores MG995
  servoPitch.attach(9);
  servoRoll.attach(10);
  servoYaw.attach(11);

  // Inicialización y autocalibración de offsets del giroscopio (Bias compensation)
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

void loop() {
  // Actualización de la unidad de medición inercial (IMU)
  mpu6050.update();

  // Captura de ángulos de Euler procesados por la librería (Filtro Complementario interno)
  float angX = mpu6050.getAngleX();
  float angY = mpu6050.getAngleY();
  float angZ = mpu6050.getAngleZ();

  // 1. LEY DE CONTROL PROPORCIONAL
  // Se calcula el setpoint del servo sumando el error multiplicado por la ganancia Kp
  float targetRoll  = 95 + (angX * dirRoll * kpRoll);
  float targetPitch = 90 + (angY * dirPitch * kpPitch);
  float rawYaw      = 90 + (angZ * dirYaw * kpYaw);

  // 2. FILTRADO DIGITAL (EMA)
  // Implementación de filtro pasa-bajas para mitigar vibraciones por torque en el eje Yaw
  smoothYaw = (rawYaw * alpha) + (smoothYaw * (1.0 - alpha));

  // 3. SATURACIÓN Y SALIDA
  // Uso de constrain para evitar que los servos excedan los límites mecánicos del frame
  servoRoll.write(constrain(targetRoll, 10, 150));
  servoPitch.write(constrain(targetPitch, 10, 170));
  
  // ZONA MUERTA (Deadband): Evita el jitter innecesario por ruido blanco del sensor
  if (abs(smoothYaw - servoYaw.read()) > 0.8) {
    servoYaw.write(constrain(smoothYaw, 10, 170));
  }
}