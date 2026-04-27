from gpiozero import LED
from time import sleep

led = LED(14, active_high = False) 
led1 = LED(15, active_high = False)
led2 = LED(18, active_high = False)

while True:
	led.off()
	led1.off()
	led2.off()
	
	led.on()
	sleep(0.5)
	led.off()
	
	led1.on()
	sleep(0.5)
	led1.off()
	
	led2.on()
	sleep(0.5)
	led2.off()
