from machine import UART, Pin
import utime

# Configuración del puerto UART (TX en D1, RX en D2)
uart = UART(1, baudrate=9600, tx=Pin(1), rx=Pin(2))  # UART1 -> D1(TX), D2(RX)

# Función para calcular el checksum
def Checksum(len_b, seq_b, data_bytes):
    s = len_b + seq_b
    for b in data_bytes:
        s += b
    return s & 0xFF

print("Inicio programa")

while True:
    if uart.any() >= 1:
        # Leer longitud de datos
        len_byte = uart.read(1)
        if not len_byte:
            continue

        data_len = len_byte[0]  # número de bytes de datos

        # Calcular cuántos bytes faltan: seq + data + chk
        bytes_to_read = 1 + data_len + 1

        # Esperar hasta que lleguen todos o hasta timeout (500 ms)
        start_time = utime.ticks_ms()
        while uart.any() < bytes_to_read and utime.ticks_diff(utime.ticks_ms(), start_time) < 500:
            utime.sleep_ms(5)

        # Verificar si llegó todo
        if uart.any() < bytes_to_read:
            print("ERROR: trama incompleta (timeout).")
            uart.write(b"NACK\n")
            uart.read()  # vaciar buffer
            continue

        # Leer paquete completo
        packet = uart.read(bytes_to_read)

        seq = packet[0]                # byte de secuencia
        data = packet[1:1+data_len]    # datos
        chksum_rx = packet[-1]         # checksum recibido
        chksum_calc = Checksum(data_len, seq, data)

        if chksum_calc == chksum_rx:
            try:
                sdata = bytes(data).decode('utf-8')
            except:
                sdata = str(list(data))
            print("OK - datos:", sdata, "seq:", seq, "chk:", chksum_rx)
            uart.write(b"ACK\n")  # Confirmación al Arduino (o dispositivo que envía)
        else:
            print("ERROR CHECKSUM - recibido:", chksum_rx, "calc:", chksum_calc)
            uart.write(b"NACK\n")  # Rechazo al Arduino (o dispositivo que envía)
    else:
        utime.sleep_ms(10)
