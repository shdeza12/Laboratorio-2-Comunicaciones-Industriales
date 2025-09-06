# Laboratorio 2 - Comunicaciones Industriales

Este repositorio contiene el desarrollo del **Laboratorio 2** de la asignatura de Comunicaciones Industriales en la **Universidad Santo Tomás**.  
El trabajo se centró en la implementación de técnicas básicas de **detección de errores en comunicación serial** usando **ESP8266MOD** como emisor y **Raspberry Pi Pico** como receptor.

## Objetivos
- Configurar la comunicación UART entre ESP8266MOD y Raspberry Pi Pico.  
- Implementar detección de errores mediante **Checksum**.  
- Implementar un protocolo simple de **ARQ (ACK/NACK)** para retransmisión confiable.  
- Analizar el impacto de diferentes probabilidades de error en la eficiencia de transmisión.  

## Desarrollo
- **Punto 1:** Transmisión con **Checksum** → detección de errores simples en la comunicación.  
- **Punto 2:** Protocolo **ARQ con ACK/NACK** → retransmisión de paquetes y evaluación bajo probabilidades de error del 10%, 30%, 60% y 90%.  
- **Punto 3:** LRC y VRC → no implementados por limitaciones de tiempo y hardware.  

## Conclusiones
- El **Checksum** resulta sencillo y confiable para detectar errores básicos.  
- El **ARQ con ACK/NACK** asegura la confiabilidad, aunque disminuye la eficiencia en canales con alta tasa de error.  
- El uso de **ESP8266MOD** permitió experimentar más allá del Arduino propuesto en la guía.  
- Documentar el laboratorio en GitHub facilita la trazabilidad y la organización académica.  

## Autores
- Santiago Hernández Ávila  
- Diego Alejandro Rodríguez Guzmán  
- Camila Pérez Mercado  

Facultad de Ingeniería Electrónica – Universidad Santo Tomás
