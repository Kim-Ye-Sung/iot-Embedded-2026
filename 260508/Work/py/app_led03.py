from flask import Flask, render_template, redirect, url_for, request
from gpiozero import LED, PWMOutputDevice, DistanceSensor
from time import monotonic,sleep
import statistics
from threading import Thread

app = Flask(__name__)

leds = [LED(15, active_high = False), LED(18, active_high = False), LED(23, active_high = False)]

buzzer = PWMOutputDevice(24)

UltraSonicSensor = DistanceSensor(echo = 8, trigger = 7, threshold_distance = 0.3)

currentLedState = "LED OFF!!"
currentBuzzerState = "Buzzer OFF!!"

Avg_UltraSonicValue = 0

def get_distance_cm():
	global Avg_UltraSonicValue	
	while True:
		values=[]
		for _ in range(5):
			values.append(UltraSonicSensor.distance * 100)
			sleep(0.05)
      
		Avg_UltraSonicValue = statistics.mean(values)
		
def LED_Control(cstr, tf = 1000):
	global currentLedState
	currentLedState = cstr
	
	for led in leds:
		led.off()
		
	if tf == 1000:
		return	
		
	leds[tf].on()

@app.route('/')
def basic():
	#currentLEDStr = request.args.get("currentStr", "LED OFF!!")	# 요청이 올때, currentStr로 오면 그거대로 사용하고, 없으면 기본적으로 Basic을 사용하겠다는 뜻
	#currentBuzzerStr = request.args.get("currentbuzzer", "Buzzer Off!!")
	return render_template("MyControl.html", LED_state = currentLedState, Buzzer_state = currentBuzzerState, sonicValue=f"{Avg_UltraSonicValue:.3f} cm")
	
@app.route('/led/blue/on', methods=["POST"])
def BlueLed_on():
	LED_Control("BlueLedOn!!",0)
	return redirect(url_for("basic"))
	
@app.route('/led/off', methods=["POST"])
def Led_off():
	LED_Control("LED Off!!")
	return redirect(url_for("basic"))
	
@app.route('/led/red/on', methods=["POST"])
def RedLed_on():
	LED_Control("RedLedOn!!",2)
	return redirect(url_for("basic"))

@app.route('/led/green/on', methods=["POST"])
def GreenLed_on():
	LED_Control("GreenLedOn!!",1)
	return redirect(url_for("basic"))
	
@app.route('/buzzer/on', methods=["POST"])
def buzzer_on():
	global currentBuzzerState
	buzzer.frequency = 392.00
	buzzer.value = 0.5 # duty cycle (소리 크기)
	currentBuzzerState = "Buzzer On!!"
	return redirect(url_for("basic"))
	
@app.route('/buzzer/off', methods=["POST"])
def buzzer_off():
	global currentBuzzerState
	currentBuzzerState = "Buzzer OFF!!"
	buzzer.value = 0 # duty cycle (소리 크기)
	return redirect(url_for("basic"))
	


if __name__ == "__main__":
	try:
		t = Thread(target=get_distance_cm)
		t.daemon = True
		t.start()
		
		app.run(host='0.0.0.0', port = 5000, use_reloader=False)
		
	finally:
		for led in leds:
			led.off()
			led.close()
			
		buzzer.close()
		UltraSonicSensor.close()
