from RPLCD.i2c import CharLCD
import time

lcd = CharLCD(i2c_expander = 'PCF8574', address = 0x27, port = 1, cols = 16, rows = 2, charmap = 'A00')

try:
    for i in range(2,10):
        for j in range (1,10):
            lcd.clear()
            lcd.write_string(f"{i} x {j}")
            lcd.crlf()
            lcd.write_string(f"= {i*j}")
            
            time.sleep(1.5)
    
finally:
    lcd.clear()
    lcd.close(clear = True)
