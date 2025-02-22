from motor import Motor
from servo import Servo
import serial
import time

rpi_motors = Motor()
rpi_servos = Servo()



def clear_serial(elegoo) -> None:
    elegoo.setDTR(False)
    time.sleep(1)
    elegoo.flushInput()
    elegoo.setDTR(True)
    time.sleep(1)

if __name__=='__main__':
    print ('PROGRAM IS STARTING \n')
    elegoo = serial.Serial('/dev/ttyACM0', 115200, timeout=1)
    clear_serial(elegoo)
    time.sleep(.1)
    ack = ""
    while True:
        ack = elegoo.readline().decode('utf-8').rstrip() if elegoo.in_waiting > 0 else ""
        if ack == "1":
            elegoo.write(b'MOVE MOTORS SHORT')
            time.sleep(1)
            rpi_motors.setMotorsForward()
            time.sleep(2)
            rpi_motors.stopMotors()
            break
			
"""
elegoo.write(b'1')
print("sent message to arduino")
time.sleep(1)
ack = elegoo.read()
print('Elegoo sent back %s' % ack)
time.sleep(1)
"""
