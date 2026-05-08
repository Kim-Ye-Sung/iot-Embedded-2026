from gpiozero import LED, Button
from time import sleep

led = LED(17) # GPIO 17
button = Button(2) # GPIO2, 기본 pull_up = True

try:
   while True:
      if button.is_pressed:
         led.on()
         print("BUTTON ON")
      else:
         led.off()
         print("BUTTON OFF")
         
      sleep(0.1)
except KeyboardInterrupt:
   led.close()
   print("\nprogram exit")
