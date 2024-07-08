import os
import pty
import serial
import time
import struct
import random
import crcmod.predefined
from threading import Thread

crc16 = crcmod.predefined.mkCrcFun('crc-ccitt-false')

def generate_message():
    sender_address = random.randint(1, 3)
    message_type = random.randint(1, 7)
    
    if message_type == 1:
        data = random.randint(0, 1000)
        data_bytes = struct.pack('<i', data)
    elif message_type == 2:
        text = "Hello, world!"
        data_bytes = text.encode('utf-8')
    else:
        data_bytes = b''
    
    data_size = len(data_bytes)
    print(f"data_size = {data_size}, type = {message_type}")
    data_size_bytes = struct.pack('>H', data_size)
    
    message = struct.pack('>BB', sender_address, message_type) + data_size_bytes + data_bytes
    
    crc = crc16(message)
    crc_bytes = struct.pack('>H', crc)
    
    full_message = message + crc_bytes
    
    return full_message

def emulate_serial_port(master_fd):
    try:
        while True:
            for _ in range(10):
                message = generate_message()
                os.write(master_fd, message)
                print(f"Sent message: {message}")
                time.sleep(0.1)  
    except KeyboardInterrupt:
        print("Эмуляция завершена.")

def create_virtual_serial_port():
    master_fd, slave_fd = pty.openpty()
    slave_name = os.ttyname(slave_fd)
    
    symlink_path = '/dev/ttyUSB0'
    try:
        os.symlink(slave_name, symlink_path)
    except FileExistsError:
        os.remove(symlink_path)
        os.symlink(slave_name, symlink_path)
    
    return master_fd, slave_name

def main():
    master_fd, slave_name = create_virtual_serial_port()
    print(f"Виртуальный последовательный порт создан: {slave_name} (ссылка: /dev/ttyUSB0)")
    
    emulator_thread = Thread(target=emulate_serial_port, args=(master_fd,))
    emulator_thread.start()
    
    try:
        emulator_thread.join()
    except KeyboardInterrupt:
        print("Эмуляция завершена.")
        os.remove('/dev/ttyUSB0')

if __name__ == "__main__":
    main()
