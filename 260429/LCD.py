from RPLCD.i2c import CharLCD
import time

lcd = CharLCD(i2c_expander = 'PCF8574', address = 0x27, port = 1, cols = 16, rows = 2, charmap = 'A00')

try:
    lcd.clear()                             # 화면 지우기
    lcd.write_string("Rasberry Pi 5")
    lcd.crlf()                              # 줄바꿈
    lcd.write_string("1602 LCD OK")
    
    time.sleep(20)
    
finally:
    lcd.clear()
    lcd.close(clear = True)