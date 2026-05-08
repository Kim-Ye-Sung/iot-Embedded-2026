from gpiozero import Button
from gpiozero import PWMLED
from time import sleep

button = Button(15)
button2 = Button(23)

leds = [PWMLED(18, active_high = False),PWMLED(24, active_high = False), PWMLED(25, active_high = False)] # 파,초, 빨

def SetColor(a,b,c):
   leds[2].value = a
   leds[1].value = b
   leds[0].value = c

def LED_ON():
   for led in leds:
      led.on()

def LED_OFF():
   for led in leds:
      led.off()

LED_OFF()

on_off = False
IsPressed_ = False  
IsPressed2 = False
current_color = 0

colors = [[1.0,0.0,0.0], [1.0,0.3,0.0], [1.0,1.0,0.0], [0.0,1.0,0.0], [0.0,0.0,1.0],[0.1,0.0,0.5],[0.5,0.0,1.0]]

try:
   while True:
      if button.is_pressed and IsPressed_ == False:
         IsPressed_ = True
         if on_off == False:
            print("LED ON")
            LED_ON()
            SetColor(*colors[current_color])
            on_off = True
         else:
            print("LED_OFF")
            LED_OFF()
            on_off = False
      elif button.is_pressed == False:
         IsPressed_= False
         
      if button2.is_pressed and on_off and IsPressed2 == False:
         IsPressed2 = True
         print("LED_CHANGE!!")

         if current_color == len(colors)-1:
            current_color = 0
            SetColor(*colors[current_color])
         else:
            current_color = current_color +1
            SetColor(*colors[current_color])

      elif button2.is_pressed == False:
         IsPressed2 = False
      
      sleep(0.05)
         
except KeyboardInterrupt:
   print("\n exit")
