import RPi.GPIO as GPIO
import time
import pigpio
import os

CLOSED_CLAW_POSITION = 30
OPEN_CLAW_POSITION = 50
RIGHT_SWPR_STRT = 0
LEFT_SWPR_STRT = 195
BIN_GRABBER_UP_POS = 105
"""
20 is the lowest position to go on servos (at least for the claw)
0 to 200 is the range for the blue servos, the servo connected to channel 0 starts at 0
The servo connected to channel 1 starts at 200 or 195
Let 105 degrees be the upright position for bingrabber, 165 is out of the way, 15 is down position
"""
class Servo:
    def __init__(self):
        self.channel1=7
        self.channel2=8
        self.channel3=25
        self.PwmServo = pigpio.pi()
        self.PwmServo.set_mode(self.channel1,pigpio.OUTPUT) 
        self.PwmServo.set_mode(self.channel2,pigpio.OUTPUT) 
        self.PwmServo.set_mode(self.channel3,pigpio.OUTPUT) 
        self.PwmServo.set_PWM_frequency(self.channel1,50)
        self.PwmServo.set_PWM_frequency(self.channel2,50)
        self.PwmServo.set_PWM_frequency(self.channel3,50)
        self.PwmServo.set_PWM_range(self.channel1, 4000)
        self.PwmServo.set_PWM_range(self.channel2, 4000)
        self.PwmServo.set_PWM_range(self.channel3, 4000)
        self.setServoPwm('0', RIGHT_SWPR_STRT)
        self.setServoPwm('1', LEFT_SWPR_STRT)
        self.setServoPwm('2', BIN_GRABBER_UP_POS)
        time.sleep(.25)
    def angle_range(self,channel,init_angle):
        if channel=='0':
            if init_angle<0:
                init_angle=0
            elif init_angle>360 :
                init_angle=360
            else:
                init_angle=init_angle
        elif channel=='1':
            if init_angle<0 :
                init_angle=0
            elif init_angle>360 :
                init_angle=360
            else:
                init_angle=init_angle
        elif channel=='2':
            if init_angle<0 :
                init_angle=0
            elif init_angle>360:
                init_angle=360
            else:
                init_angle=init_angle
        return init_angle
        
    def setServoPwm(self,channel,angle):
        if channel=='0':
            angle=int(self.angle_range('0',angle))
            self.PwmServo.set_PWM_dutycycle(self.channel1,80+(400/180)*angle)
        elif channel=='1':
            angle=int(self.angle_range('1',angle))
            self.PwmServo.set_PWM_dutycycle(self.channel2,80+(400/180)*angle)
        elif channel=='2':
            angle=int(self.angle_range('2',angle))
            self.PwmServo.set_PWM_dutycycle(self.channel3,80+(400/180)*angle)
            
    def openSweepers(self):
        for i in range(LEFT_SWPR_STRT, LEFT_SWPR_STRT-120,-1):
            self.setServoPwm('1',i)
            time.sleep(.02)
        time.sleep(.25)
        for i in range(RIGHT_SWPR_STRT, RIGHT_SWPR_STRT+120,1):
            self.setServoPwm('0',i)
            time.sleep(.02)

            
    def closeSweepers(self):
        for i in range(RIGHT_SWPR_STRT+120, RIGHT_SWPR_STRT, -1):
            self.setServoPwm('0',i)
            time.sleep(.02)
        time.sleep(.25)
        for i in range(LEFT_SWPR_STRT-120, LEFT_SWPR_STRT, 1):
            self.setServoPwm('1',i)
            time.sleep(.02)
    
    def lowerBinGrabber(self):
        for i in range(BIN_GRABBER_UP_POS, 15, -1):
            self.setServoPwm('2', i)
            time.sleep(.01)
    
    def raiseBinGrabber(self):
        for i in range(15, BIN_GRABBER_UP_POS, 1):
            self.setServoPwm('2', i)
            time.sleep(.01)
        
            
    

# Main program logic follows:
if __name__ == '__main__':
    servo=Servo() 
    servo.lowerBinGrabber()
    time.sleep(3)
    servo.raiseBinGrabber()
"""
    def openClaw(self):
        self.setServoPwm('0', OPEN_CLAW_POSITION)
    
    def closeClaw(self):
        self.setServoPwm('2', CLOSED_CLAW_POSITION)
"""
