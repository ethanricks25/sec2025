import RPi.GPIO as GPIO
import time
import pigpio
import serial

MOTOR_SPEED = 1024

"""
64 out 255 is the range on the elegoo, so 1024 is the duty cycle on here to match the speed
"""
class Motor:
    def __init__(self):
        self.pwm1 = 24
        self.pwm2 = 23
        self.pwm3 = 5
        self.pwm4 = 6         
        self.PwmServo = pigpio.pi()
        self.PwmServo.set_mode(self.pwm1,pigpio.OUTPUT) 
        self.PwmServo.set_mode(self.pwm2,pigpio.OUTPUT) 
        self.PwmServo.set_mode(self.pwm3,pigpio.OUTPUT) 
        self.PwmServo.set_mode(self.pwm4,pigpio.OUTPUT)         
        self.PwmServo.set_PWM_frequency(self.pwm1,50)
        self.PwmServo.set_PWM_frequency(self.pwm2,50)
        self.PwmServo.set_PWM_frequency(self.pwm3,50)
        self.PwmServo.set_PWM_frequency(self.pwm4,50)        
        self.PwmServo.set_PWM_range(self.pwm1, 4095)
        self.PwmServo.set_PWM_range(self.pwm2, 4095)
        self.PwmServo.set_PWM_range(self.pwm3, 4095)
        self.PwmServo.set_PWM_range(self.pwm4, 4095)
    
    def duty_range(self,duty1,duty2):
        if duty1>4095:
            duty1=4095
        elif duty1<-4095:
            duty1=-4095        
        if duty2>4095:
            duty2=4095
        elif duty2<-4095:
            duty2=-4095
        return duty1,duty2
    
    def left_Wheel(self,duty):
        if duty>0:
            self.PwmServo.set_PWM_dutycycle(self.pwm1,0)
            self.PwmServo.set_PWM_dutycycle(self.pwm2,duty)
        elif duty<0:
            self.PwmServo.set_PWM_dutycycle(self.pwm1,abs(duty))
            self.PwmServo.set_PWM_dutycycle(self.pwm2,0)
        else:
            self.PwmServo.set_PWM_dutycycle(self.pwm1,0)
            self.PwmServo.set_PWM_dutycycle(self.pwm2,0)

    def right_Wheel(self,duty):
        if duty>0:
            self.PwmServo.set_PWM_dutycycle(self.pwm3,0)
            self.PwmServo.set_PWM_dutycycle(self.pwm4,duty)
        elif duty<0:
            self.PwmServo.set_PWM_dutycycle(self.pwm3, abs(duty))
            self.PwmServo.set_PWM_dutycycle(self.pwm4,0)
        else:
            self.PwmServo.set_PWM_dutycycle(self.pwm3,0)
            self.PwmServo.set_PWM_dutycycle(self.pwm4,0)

    def setMotorModel(self,duty1,duty2):
        duty2 = -1 * duty2
        duty1,duty2=self.duty_range(duty1,duty2)
        self.left_Wheel(duty1)
        self.right_Wheel(duty2)
    
    def stopMotors(self):
        self.setMotorModel(0,0)
    
    def setMotorsBackward(self):
        self.setMotorModel(2048, 2048)
        
    def setMotorsForward(self):
        self.setMotorModel(-1024, -1024)
    
    def turnLeft(self):
        self.setMotorModel(-2048, 2048)
        
    def turnRight(self):
        self.setMotorModel(2048, -2048)
        
    def setMotorsStrafeRight(self):
        self.setMotorModel(3072, -3072)
    
    def setMotorsStrafeLeft(self):
        self.setMotorModel(-3072, 3072)
        
        
PWM=Motor()

if __name__=='__main__':
    print ('Program is starting ... \n')
    elegoo = serial.Serial('/dev/ttyACM0', 115200, timeout=1)
    time.sleep(2)
    try:
        elegoo.setDTR(False)
        time.sleep(1)
        elegoo.flushInput()
        elegoo.setDTR(True)
        time.sleep(1)
        elegoo.write(b'1')
        print("sent message to arduino")
        time.sleep(10)
        ack = elegoo.read()
        print('Elegoo sent back %s' % ack)
        time.sleep(1)
    except KeyboardInterrupt:  # When 'Ctrl+C' is pressed, the child program destroy() will be  executed.
        destroy()
