from motor import Motor
from servo import Servo
import serial
import time

rpi_motors = Motor()
rpi_servos = Servo()

def elegoo_send(elegoo, message) -> None:
    elegoo.write(f'{message}'.encode())
    time.sleep(1)
    
    
def clear_serial(elegoo) -> None:
    elegoo.setDTR(False)
    time.sleep(1)
    elegoo.flushInput()
    elegoo.setDTR(True)
    time.sleep(1)
    
def moveForward(elegoo) -> None:
    elegoo_send(elegoo, 'MOVE MOTORS SHORT')
    rpi_motors.setMotorsForward()
    time.sleep(2.6)
    rpi_motors.stopMotors()
    time.sleep(.2)
   
def moveBackward(elegoo) -> None:
	elegoo_send(elegoo, 'MOVE BACKWARD')
	rpi_motors.setMotorsBackward()
	time.sleep(2.6)
	rpi_motors.stopMotors()
	time.sleep(.2)
    
def strafeLeft(elegoo)-> None:
	elegoo_send(elegoo, 'MOVE BACKWARD')
	rpi_motors.setMotorsForward()
	time.sleep(2.6)
	rpi_motors.stopMotors()
	

def strafeRight(elegoo)-> None:
	elegoo_send(elegoo, 'MOVE MOTORS SHORT')
	rpi_motors.setMotorsBackward()
	time.sleep(2.6)
	rpi_motors.stopMotors()
   
def turnRight(elegoo) -> None:
	elegoo_send(elegoo, 'TURN RIGHT')
	rpi_motors.turnRight()
	time.sleep(.925)
	rpi_motors.stopMotors()

def turnLeft(elegoo) -> None:
	elegoo_send(elegoo, 'TURN LEFT')
	rpi_motors.turnLeft()
	time.sleep(.925)
	rpi_motors.stopMotors()
	
def read_elegoo(elegoo) -> str:
	return elegoo.readline().decode('utf-8').rstrip() if elegoo.in_waiting > 0 else ""


if __name__=='__main__':
    elegoo = serial.Serial('/dev/ttyACM0', 115200, timeout=1)
    clear_serial(elegoo)
    time.sleep(.1)
    ack = ""
    while True:
        ack = read_elegoo(elegoo)
        if ack == "1":
             moveForward(elegoo)
             moveForward(elegoo)
             moveForward(elegoo)
            

            

           


"""
elegoo.write(b'1')
print("sent message to arduino")
time.sleep(1)
ack = elegoo.read()
print('Elegoo sent back %s' % ack)
time.sleep(1)

put try catch block in elegoo

elegoo = serial.Serial('/dev/ttyACM0', 115200, timeout=1)
    clear_serial(elegoo)
    time.sleep(.1)
    ack = ""
    while True:
        ack = elegoo.readline().decode('utf-8').rstrip() if elegoo.in_waiting > 0 else ""
        if ack == "1":
            moveForward(elegoo)
            moveForward(elegoo)
            time.sleep(1)
            rpi_servos.lowerBinGrabber()
            time.sleep(1)
            turnLeft(elegoo)
            time.sleep(1)
            moveForward(elegoo)
            time.sleep(1)
            rpi_servos.raiseBinGrabber()
            time.sleep(1)
            moveBackward(elegoo)
            time.sleep(1)
            turnRight(elegoo)
            time.sleep(1)
            moveBackward(elegoo)
            time.sleep(1)
            turnRight(elegoo)
            time.sleep(1)
            moveForward(elegoo)
            time.sleep(1)
            rpi_servos.lowerBinGrabber()
            time.sleep(1)
            turnLeft(elegoo)
            time.sleep(1)
            turnLeft(elegoo)
            time.sleep(1)
            moveForward(elegoo)
            time.sleep(1)
            moveForward(elegoo)
            break		
"""
