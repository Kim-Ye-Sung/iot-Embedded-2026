# pip install adafruit-circuitypython-dht
import time
import board
import adafruit_dht
from RPLCD.i2c import CharLCD

dht = adafruit_dht.DHT11(board.D4)  # GPIO 4번핀

lcd = CharLCD(i2c_expander = 'PCF8574', address = 0x27, port = 1, cols = 16, rows = 2, charmap = 'A00')

try:
   print("DHT11 온습도 측정 시작...")
   
   while True:
      try:
         temp = dht.temperature
         humi = dht.humidity
         
         if temp is not None and humi is not None:
            print(f"Temp: {temp:.1f}°C / Humi : {humi:.1f}%")
            lcd.clear()
            lcd.write_string(f"Temp: {temp:.1f}°C")
            lcd.crlf()
            lcd.write_string(f"Humi : {humi:.1f}%")
         else:
            print("Failed to measure")
            lcd.clear()
            lcd.write_string("Failed to measure")
            
      except RuntimeError as e:
         # DHT11은 가끔 읽기 실패가 정상적으로 발생함
         print("Failed, restart:", e)
         
      time.sleep(2)
      
except KeyboardInterrupt:
   print("exit")

finally:
   dht.exit()
