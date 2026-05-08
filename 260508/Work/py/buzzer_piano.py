from gpiozero import PWMOutputDevice
from time import sleep

import sys
import tty
import termios

# GPIO 핀 설정 (예: 18번)
buzzer = PWMOutputDevice(21)
# 음계 정의
notes = {
    "C4":261.63, "D4":293.66,
    "E4":329.63, "F4":349.23,
    "G4":392.00, "A4":440.00,
    "B4":493.88, "C5":523.25
}

# 도 레 미 파 솔 라 시 도
scale = ["C4", "D4", "E4", "F4", "G4", "A4", "B4", "C5"]

def play(pia):
    buzzer.off()
    sleep(0.1)
    freq = notes[pia]
    buzzer.frequency = freq
    buzzer.value = 0.5 # duty cycle (소리 크기)
    sleep(0.3)
    buzzer.off()


def get_key(): # 키보드에서 엔터키를 누르지 않고 한글자만 눌러도 바로 입력되게 하는 함수 
    fd = sys.stdin.fileno()
    old_settings = termios.tcgetattr(fd)
    
    try:
        tty.setraw(fd)
        key = sys.stdin.read(1)
    finally:
        termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)

    return key
    

try:
    while True:
        # piano = int(input())

        piano = get_key()
        
        if piano == "1":
            play("C4")
        elif piano == "2":
            play("D4")
        elif piano == "3":
            play("E4")
        elif piano == "4":
            play("F4")
        elif piano == "5":
            play("G4")
        elif piano == "6":
            play("A4")
        elif piano == "7":
            play("B4")
        elif piano == "8":
            play("C5")
        else:
            buzzer.off()
        
finally:
    buzzer.close()
