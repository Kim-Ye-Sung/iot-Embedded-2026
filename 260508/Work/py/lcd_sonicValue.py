from RPLCD.i2c import CharLCD
from gpiozero import DistanceSensor
import statistics
from gpiozero import PWMOutputDevice
from time import sleep, monotonic

lcd = CharLCD(i2c_expander = 'PCF8574', address = 0x27, port = 1, cols = 16, rows = 2, charmap = 'A00')
sensor = DistanceSensor(echo = 23, trigger = 24)
buzzer = PWMOutputDevice(21)
buzzer.frequency = 392.00

buzzer_state = False          # 현재 부저가 켜져 있는지 꺼져 있는지
last_buzzer_time = monotonic() # 마지막으로 부저 상태를 바꾼 시간

def get_distance_cm(samples = 5):
   values=[]
   for _ in range(samples):
      values.append(sensor.distance * 100)
      sleep(0.05)
   return statistics.mean(values)

def update_buzzer(interval):
    global buzzer_state
    global last_buzzer_time

    now = monotonic()  
    
    if now - last_buzzer_time >= interval:  # interval 시간이 지났을 때만 부저 상태를 바꿈
        last_buzzer_time = now

        if buzzer_state == False:
            buzzer.value = 0.5 
            buzzer_state = True
        else:
            buzzer.off()
            buzzer_state = False

try:
    while True:
        dist = get_distance_cm()
        lcd.clear()                             
        lcd.write_string("UltraSonicValue")
        lcd.crlf()                             
        lcd.write_string(f"{dist:.3f} cm")
    
        if dist < 5.0:
            update_buzzer(0.1)   # 0.1초마다 켜짐/꺼짐 변경
        elif dist<15.0:
            update_buzzer(0.25)  # 0.25초마다 켜짐/꺼짐 변경
        elif dist < 30.0:
            update_buzzer(0.5)    # 0.5초마다 켜짐/꺼짐 변경
        else:
            buzzer.off()
            buzzer_state = False

        sleep(0.05)
    
finally:
    lcd.clear()
    lcd.close(clear = True)
    buzzer.off()
    buzzer.close()
    sensor.close()
