from machine import UART, Pin
import time

# UART0 en Pico (TX=GP0, RX=GP1)
uart = UART(0, baudrate=9600, tx=Pin(0), rx=Pin(1))

def check_parity(byte, parity):
    p = 0
    for i in range(8):
        p ^= (byte >> i) & 1
    return p == parity

def calc_lrc(block):
    lrc = 0
    for b in block:
        lrc ^= b
    return lrc

print("Pico receptor listo, esperando datos...")

while True:
    if uart.any() >= 11:  # 5 datos + 5 paridades + 1 LRC
        datos = []
        paridades = []

        # Leer 5 datos y sus paridades
        for i in range(5):
            d = uart.read(1)[0]  # Leer un byte para el dato
            p = uart.read(1)[0]  # Leer un byte para la paridad
            datos.append(d)
            paridades.append(p)
            
            # Verificar la paridad
            if check_parity(d, p):
                print(f"Dato: {hex(d)} | Paridad: {hex(p)} -> OK")
            else:
                print(f"Dato: {hex(d)} | Paridad: {hex(p)} -> Error")

        # Leer el LRC recibido
        lrc_rx = uart.read(1)[0]
        
        # Calcular el LRC
        lrc_calc = calc_lrc(datos)
        
        # Comparar LRC recibido y calculado
        if lrc_rx == lrc_calc:
            print(f"LRC recibido: {hex(lrc_rx)} | LRC calculado: {hex(lrc_calc)} -> OK")
        else:
            print(f"LRC recibido: {hex(lrc_rx)} | LRC calculado: {hex(lrc_calc)} -> ERROR")

        print("---------------------------------------------")
    else:
        # Si no hay suficientes bytes en el buffer, mostrar la cantidad de bytes disponibles
        print(f"Bytes en espera: {uart.any()}")

    # Pausar un poco antes de volver a revisar
    time.sleep(0.2)
