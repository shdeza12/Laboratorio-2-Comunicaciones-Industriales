/*
 * Transmisión de datos con errores simulados y verificación mediante VRC y LRC.
 * Comunicación entre ESP8266 y Raspberry Pi Pico utilizando UART (Serial1).
 * 
 * Pines:
 *   - D1 (TX) -> Enviar datos al Pico
 *   - D2 (RX) -> Recibir datos del Pico
 * 
 * Monitor Serie para depuración y comunicación real a través de Serial1.
 */

byte datos[5] = {0x12, 0x34, 0x56, 0x78, 0x9A};

// Función para calcular la paridad (VRC) de un byte
byte calcularParidad(byte dato) {
  byte paridad = 0;
  for (int i = 0; i < 8; i++) {
    paridad ^= (dato >> i) & 0x01;
  }
  return paridad;
}

// Función para calcular el LRC (Longitud de Redundancia Cíclica) de un bloque de datos
byte calcularLRC(byte *bloque, int longitud) {
  byte lrc = 0x00;
  for (int i = 0; i < longitud; i++) {
    lrc ^= bloque[i];
  }
  return lrc;
}

void setup() {
  // Iniciar la comunicación con el monitor serie (para depuración)
  Serial.begin(115200);  // Monitor Serie

  // Iniciar la comunicación con el Raspberry Pi Pico a través de Serial1 (UART hardware)
  Serial1.begin(9600, SERIAL_8N1, D2, D1);  // RX=D2, TX=D1

  // Inicializar el generador de números aleatorios
  randomSeed(analogRead(A0));  // Utilizamos el pin A0 para obtener ruido y generar una semilla aleatoria
}

void loop() {
  // Mensaje de inicio de transmisión de datos
  Serial.println("+++ Iniciando transmisión de datos... +++");

  // Enviar 5 bloques de datos, junto con la paridad para cada uno
  for (int i = 0; i < 5; i++) {
    byte dato = datos[i];  // Tomamos el dato del bloque
    byte paridad = calcularParidad(dato);  // Calculamos la paridad del dato

    // Simular errores aleatorios (20% de probabilidad)
    if (random(100) < 20) {
      if (random(2) == 0) {
        // Corruptir aleatoriamente un bit del dato
        dato ^= (1 << random(8));
        Serial.println("¡Atención! Error introducido en el DATO.");
      } else {
        // Invertir la paridad para generar un error
        paridad ^= 1;
        Serial.println("¡Atención! Error introducido en la PARIDAD.");
      }
    }

    // Enviar el dato y la paridad al Raspberry Pi Pico
    Serial1.write(dato);
    Serial1.write(paridad);

    // Mostrar en el monitor serie los datos enviados
    Serial.print("Datos enviados -> Dato: ");
    Serial.print(dato, HEX);
    Serial.print(" | Paridad: ");
    Serial.println(paridad, HEX);
  }

  // Calcular y enviar el LRC para verificar la integridad del bloque
  byte lrc = calcularLRC(datos, 5);
  Serial1.write(lrc);

  // Mostrar el valor del LRC enviado
  Serial.print("LRC enviado: ");
  Serial.println(lrc, HEX);
  
  // Separador visual para cada ciclo de transmisión
  Serial.println("------------------------------");

  // Pausa antes de enviar el siguiente bloque
  delay(2000);  
}
