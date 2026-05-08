# NER-1184 KEYPAD 4x4
from gpiozero import DigitalOutputDevice, Button,PWMOutputDevice
from RPLCD.i2c import CharLCD
from time import sleep

lcd = CharLCD(i2c_expander = 'PCF8574', address = 0x27, port = 1, cols = 16, rows = 2, charmap = 'A00')

buzzer = PWMOutputDevice(18)
buzzer.frequency = 523.25

# GPIO 설정
IN1 = DigitalOutputDevice(17)
IN2 = DigitalOutputDevice(27)
IN3 = DigitalOutputDevice(22)
IN4 = DigitalOutputDevice(23)

# Halt-step 시퀀스 (더 부드러움)
SEQ = [
   [1,0,0,0],
   [1,1,0,0],
   [0,1,0,0],
   [0,1,1,0],
   [0,0,1,0],
   [0,0,1,1],
   [0,0,0,1],
   [1,0,0,1],
]

pins = [IN1, IN2, IN3, IN4]


# 키패드 배열
KEYS = [
    ['1', '2', '3', 'A'],
    ['4', '5', '6', 'B'],
    ['7', '8', '9', 'C'],
    ['*', '0', '#', 'D']
]

# BCM GPIO 번호 기준
ROW_PINS = [5,6,13,19]  # OUT3, OUT2, OUT1, OUT0
COL_PINS = [21,20,16,12]    # IN0, IN1, IN2, IN3

UserPassword = "777777"
InputPassword =""

rows = [DigitalOutputDevice(pin, active_high = True, initial_value = False)
        for pin in ROW_PINS]
cols = [Button(pin, pull_up = False)
        for pin in COL_PINS]

def scan_keypad():
    for row_index, row in enumerate(rows):
        # 모든 행  LOW
        for r in rows:
            r.off()
        # 현재 행만  HIGH
        row.on()
        sleep(0.001)
        
        # 열 확인
        for col_index, col in enumerate(cols):
            if col.is_pressed:
                return KEYS[row_index][col_index]
    return None
    
def clear_second_line():
    global word_pos
    word_pos = 0
    lcd.cursor_pos = (1, 0)
    lcd.write_string(" " * 16)
    lcd.cursor_pos = (1, 0)
    
def step(delay = 0.002):
   for seq in SEQ:
      for pin, val in zip(pins, seq):
         if val:
            pin.on()
         else:
            pin.off()
      sleep(delay)
      
def step_reverse(delay = 0.002):
   for seq in reversed(SEQ):
      for pin, val in zip(pins, seq):
         if val:
            pin.on()
         else:
            pin.off()
      sleep(delay)
    
def CheckPassword(InPassword):
    global UserPassword
    if UserPassword == InPassword:
        buzzer.off()
        for _ in range(256):
            step()
        
        sleep(3)
        for _ in range(256):
            step_reverse()
    else:
        buzzer.value = 0.5
        
def wait_key_release():
    while scan_keypad() is not None:
        sleep(0.05)        

def SetNewPassword():
    global UserPassword
    NewPassword = ""
    
    lcd.clear()
    lcd.write_string("Input New Pw")
    lcd.crlf()
    
    last_key = None
    
    while True:
        NewKey = scan_keypad()
        
        if NewKey is not None and NewKey != last_key:
            last_key = NewKey
        
            if NewKey == "*":
                if len(NewPassword) != 6 or not NewPassword.isdigit():
                    clear_second_line()
                    NewPassword = ""
                    print("aaaa")
                    wait_key_release()
                    continue
                else:
                    UserPassword = NewPassword
                    wait_key_release()
                    break
                
            NewPassword = NewPassword + NewKey
        
            if len(NewPassword) > 16:
                NewPassword =""
                clear_second_line()
                continue
                
            lcd.write_string(NewKey)
            
        if NewKey is None:
            last_key = None
    
    lcd.clear()
    lcd.write_string("Password")
    lcd.crlf()
    
try:
    lcd.write_string(f"Password")
    lcd.crlf()
    
    last_key = None
    while True:
        key = scan_keypad()
        
        if key is not None and key != last_key:
            last_key = key
            
            print("aaaaaaaa", InputPassword)
            
            if len(InputPassword) == 0 and key =="#":
                last_key = None
                SetNewPassword()
                last_key = None
                continue
            
            if key == "*":
                clear_second_line()
                CheckPassword(InputPassword)
                InputPassword = ""
                continue
            InputPassword = InputPassword + key
            print("aaaa  ", InputPassword)
            
            if len(InputPassword) > 16:
                InputPassword =""
                clear_second_line()
                buzzer.value = 0.5
                continue
                
            lcd.write_string(key)
            
            
        if key is None:
            last_key = None
            
        sleep(0.05)
        
except KeyboardInterrupt:
    print("Exit")
    
finally:
    for r in rows:
        r.off()
