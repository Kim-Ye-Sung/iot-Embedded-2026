from gpiozero import Button
from gpiozero import LED
from time import sleep

button = Button(15)
button2 = Button(23)

leds = [LED(18, active_high = False),LED(24, active_high = False), LED(25, active_high = False)]

for led in leds:
   led.off()

on_off = False
IsPressed_ = False  
IsPressed2 = False
current_led = 0

try:
   while True:
      if button.is_pressed and IsPressed_ == False:
         IsPressed_ = True
         if on_off == False:
            print("LED ON")
            leds[current_led].on()
            on_off = True
         else:
            print("LED_OFF")
            leds[current_led].off()
            on_off = False
      elif button.is_pressed == False:
         IsPressed_= False
         
      if button2.is_pressed and on_off and IsPressed2 == False:
         IsPressed2 = True
         print("LED_CHANGE!!")
         leds[current_led].off()

         if current_led == len(leds)-1:
            current_led = 0
            leds[current_led].on()
         else:
            current_led = current_led +1
            leds[current_led].on()

      elif button2.is_pressed == False:
         IsPressed2 = False
      
      sleep(0.05)
         
except KeyboardInterrupt:
   print("\n exit")