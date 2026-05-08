from gpiozero import DigitalOutputDevice
from gpiozero import Button
import time

button = Button(15)    # 정방향 버튼
button2 = Button(18)   # 역방향 버튼

IN1 = DigitalOutputDevice(17)
IN2 = DigitalOutputDevice(27)
IN3 = DigitalOutputDevice(22)
IN4 = DigitalOutputDevice(23)

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

def set_pins(seq):
   for pin, val in zip(pins, seq):
      if val:
         pin.on()
      else:
         pin.off()

def motor_off():
   for pin in pins:
      pin.off()

def step_while_pressed(delay=0.002):
   for seq in SEQ:
      if not button.is_pressed:
         break

      set_pins(seq)
      time.sleep(delay)

def step_reverse_while_pressed(delay=0.002):
   for seq in reversed(SEQ):
      if not button2.is_pressed:
         break

      set_pins(seq)
      time.sleep(delay)

try:
   while True:
      if button.is_pressed:
         step_while_pressed()

      elif button2.is_pressed:
         step_reverse_while_pressed()

      else:
         motor_off()
         time.sleep(0.01)

except KeyboardInterrupt:
   print("종료")

finally:
   motor_off()
