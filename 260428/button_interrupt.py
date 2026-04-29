from gpiozero import LED, Button
from signal import pause

button = Button(15)
button2 = Button(23)

leds = [LED(18, active_high = False),LED(24, active_high = False), LED(25, active_high = False)]

current_led = 0

on_off = False

for led in leds:
   led.off()

def button_pressed():
   global on_off
   if on_off == False:
      leds[current_led].on()
      print("BUTTON PRESSED")
      on_off = True
   else:
      leds[current_led].off()
      print("BUTTON PRESSED")
      on_off = False
   
def button_released():
   print("BUTTON RELEASED")
  
def button2_pressed():
   global current_led

   if on_off:
      leds[current_led].off()

      if current_led == len(leds) - 1:
         current_led = 0
      else:
         current_led = current_led + 1
      
      leds[current_led].on()

   print("Color Change!")
      

   
button.when_pressed = button_pressed

button.when_released = button_released

button2.when_pressed = button2_pressed


try:
   pause()
finally:
   led.off()
   led.close()
   print("\n exit")