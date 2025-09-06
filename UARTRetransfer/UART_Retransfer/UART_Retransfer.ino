// Definición de pines para comunicación serial
const int RX_PIN = D2;  // RX del ESP8266 (conectado a TX de la Pico)
const int TX_PIN = D1;  // TX del ESP8266 (conectado a RX de la Pico)
const unsigned long BAUD = 9600;
uint8_t seq = 0; // Número de secuencia, se incrementa en cada envío

void setup() 
{
  Serial.begin(BAUD);         // Monitor serie con la PC
  Serial1.begin(BAUD, SERIAL_8N1, RX_PIN, TX_PIN);  // Canal hardware hacia la Pico usando Serial1
  randomSeed(analogRead(A0));  // Semilla aleatoria
  Serial.println("Punto 2");
}

uint8_t calcular_checksum(uint8_t len, uint8_t seq, uint8_t *data) 
{
  uint16_t s = 0;
  s += len;
  s += seq;
  for (uint8_t i = 0; i < len; i++) 
  {
    s += data[i];
  }
  return (uint8_t)(s & 0xFF);
}

void enviar_paquete(uint8_t *data, uint8_t len) 
{
  uint8_t chk = calcular_checksum(len, seq, data);

  // Con probabilidad del 20% se fuerza error en el checksum
  if (random(100) < 20) 
  {
    chk = chk + 1;  // Forzar error en checksum
    Serial.println("Paquete con ERROR forzado.");
  }

  // Enviar trama: LEN, SEQ, DATA..., CHK
  Serial1.write(len);
  Serial1.write(seq);
  for (uint8_t i = 0; i < len; i++)
  {
    Serial1.write(data[i]);
  } 
  Serial1.write(chk);

  // Mostrar en consola de ESP8266
  Serial.print("Enviado paquete seq=");
  Serial.print(seq);
  Serial.print(" len=");
  Serial.print(len);
  Serial.print(" chk=");
  Serial.println(chk);
}

bool esperar_respuesta(unsigned long timeout_ms) 
{
  unsigned long t0 = millis();
  String respuesta = "";

  while (millis() - t0 < timeout_ms) 
  {
    if (Serial1.available()) 
    {
      char c = Serial1.read();
      if (c == '\n') break; // fin de mensaje
      respuesta += c;
    }
  }

  if (respuesta == "ACK") 
  {
    Serial.println("Recibido ACK");
    return true; // correcto
  } 
  else if (respuesta == "NACK") 
  {
    Serial.println("Recibido NACK");
    return false; // error, retransmitir
  } 
  else 
  {
    Serial.println("Sin respuesta / desconocida: " + respuesta);
    return false; // asumir error
  }
}

void loop() 
{
  uint8_t datos[] = { 'A', 'B', 'C' };
  bool entregado = false;
  int reintentos = 0;

  while (!entregado) 
  {
    enviar_paquete(datos, 3);
    entregado = esperar_respuesta(500); // espera 500 ms máx
    if (!entregado) 
    {
      reintentos++;
      Serial.println("Reintentando... intento #" + String(reintentos));
    }
  }

  Serial.print("Paquete confirmado con ");
  Serial.print(reintentos);
  Serial.println(" reintentos.");

  seq++; // Aumentar número de secuencia
  delay(2000); // Espera 2 seg antes de siguiente paquete
}
