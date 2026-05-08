from gpiozero import Button
from gpiozero import LED

button = Button(15)
button2 = Button(23)

led = LED(18, active_high = False)
led.off()
led2 = LED(24, active_high = False)
led2.off()
led3 = LED(25, active_high = False)
led3.off()

try:
   while True:
      if button.is_pressed:
         print("Button is pressed")
         led.on()
      elif button2.is_pressed:
         print("Button is not pressed")
         led.off()
except KeyboardInterrupt:
   print("\n exit")
