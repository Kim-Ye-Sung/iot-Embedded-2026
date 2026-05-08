# Half-step: 360도 2048, 90도 512, 45도 256 step
from gpiozero import DigitalOutputDevice
from gpiozero import Button
import time

button = Button(15)
button2 = Button(18)

btn1_push = False
btn2_push = False
btn1_pressed = False
btn2_pressed = False

# GPIO 설정
IN1 = DigitalOutputDevice(17)
IN2 = DigitalOutputDevice(27)
IN3 = DigitalOutputDevice(22)
IN4 = DigitalOutputDevice(23)

# Halt-step 시퀀스 (더 부드러움)
SEQ = [
   [1,0,0,0],
   [1,1,0,0],
   [0,1,0,0],
   [0,1,1,0],
   [0,0,1,0],
   [0,0,1,1],
   [0,0,0,1],
   [1,0,0,1],
]

pins = [IN1, IN2, IN3, IN4]

def step(delay = 0.002):
   for seq in SEQ:
      for pin, val in zip(pins, seq):
         if val:
            pin.on()
         else:
            pin.off()
      time.sleep(delay)
      
def step_reverse(delay = 0.002):
   for seq in reversed(SEQ):
      for pin, val in zip(pins, seq):
         if val:
            pin.on()
         else:
            pin.off()
      time.sleep(delay)
      
try:
   while True:
      if button.is_pressed and btn1_push == False:
         btn1_push = True
         if btn1_pressed == False:
            btn1_pressed = True
            btn2_pressed = False
         else:
            btn1_pressed = False
      elif button2.is_pressed and btn2_push == False:
         btn2_push = True
         if btn2_pressed == False:
            btn2_pressed = True
            btn1_pressed = False
         else:
            btn2_pressed = False
            
            
      if not button.is_pressed:
         btn1_push = False
         
      if not button2.is_pressed:
         btn2_push = False
         
      if btn1_pressed == True:
         for _ in range(1): # 약 1회전
            step()
      elif btn2_pressed == True:
         for _ in range(1): # 약 1회전
            step_reverse()
      
      
except KeyboardInterrupt:
   print("종료")
   
finally:
   for pin in pins:
      pin.off()
