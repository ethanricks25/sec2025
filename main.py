import motors
import serial
import time

class Main():
    def __init__(self, elegoo):
        self.motors = motors(elegoo)


def clear_serial(elegoo) -> None:
    elegoo.setDTR(False)
    time.sleep(1)
    elegoo.flushInput()
    elegoo.setDTR(True)
    time.sleep(1)

if __name__=='__main__':
    print ('PROGRAM IS STARTING \n')
    elegoo = serial.Serial('/dev/ttyACM0', 115200, timeout=1)
    time.sleep(1)
    try:
        clear_serial(elegoo)
        elegoo.write(b'1')
        print("sent message to arduino")
        time.sleep(1)
        ack = elegoo.read()
        print('Elegoo sent back %s' % ack)
        time.sleep(1)
    except KeyboardInterrupt:  # When 'Ctrl+C' is pressed, the child program destroy() will be  executed.
        destroy()