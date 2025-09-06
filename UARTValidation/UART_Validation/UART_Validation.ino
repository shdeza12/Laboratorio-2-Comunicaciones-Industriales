// Definición de pines para comunicación UART
const int RX_PIN = D2;   // pin RX en el ESP8266
const int TX_PIN = D1;   // pin TX en el ESP8266
const unsigned long BAUD = 9600;
const uint8_t SEQ = 0;  // Número de secuencia (puedes incrementarlo si es necesario)

void setup() {
  Serial.begin(BAUD);       // Monitor serie con la PC
  Serial1.begin(BAUD, SERIAL_8N1, RX_PIN, TX_PIN);  // Canal UART1 hacia la Pico (RX= D2, TX= D1)
  randomSeed(analogRead(A0)); // Semilla aleatoria usando el ADC
  Serial.println("Punto 1");
}

uint8_t calcular_checksum(uint8_t len, uint8_t seq, uint8_t *data) {
  uint16_t s = 0;
  s += len;
  s += seq;
  for (uint8_t i = 0; i < len; i++) s += data[i];
  return (uint8_t)(s & 0xFF);
}

void enviar_paquete(uint8_t *data, uint8_t len) {
  uint8_t chk = calcular_checksum(len, SEQ, data);

  // Con probabilidad del 20% alteramos el checksum
  if (random(100) < 20) {
    chk = chk + 1; // Forzar error en el checksum
    Serial.println("Paquete con ERROR forzado.");
  }
  

  // Enviar en orden: LEN, SEQ, DATA..., CHK
  Serial1.write(len);
  Serial1.write(SEQ);
  for (uint8_t i = 0; i < len; i++) Serial1.write(data[i]);
  Serial1.write(chk);

  // Imprimir en monitor para evidencias
  Serial.print("Enviado paquete len=");
  Serial.print(len);
  Serial.print(" chk=");
  Serial.println(chk);
}

void loop() {
  // Ejemplo: enviar la cadena "ABC"
  uint8_t datos[] = { 'A', 'B', 'C' };
  enviar_paquete(datos, 3);
  delay(1000);  // Espera 1 segundo antes de enviar el siguiente paquete
}
