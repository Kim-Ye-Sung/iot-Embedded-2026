import smbus
import time
from gpiozero import LED

bus = smbus.SMBus(1)
ADDR = 0x48

led = LED(18, active_high = False)
led1 = LED(24, active_high = False)
led2 = LED(25, active_high = False)
led.off()
led1.off()
led2.off()

max_count = 5
count = 0
count_sum = 0

def read_cds():
    bus.write_byte(ADDR, 0x40)  # AIN0
    bus.read_byte(ADDR)         # dummy
    return bus.read_byte(ADDR)

try:
    while True:
        val = read_cds()  # 원래 센서값: 0 ~ 255

        percent = int((val / 255) * 100)  # 0 ~ 100으로 변환
        
        # print("CDS:", percent)
        
        count_sum = count_sum + percent
        count = count + 1
        
        if count == 5:
            average = count_sum/max_count
            if average <63:
                print(average,"  LED ON!")
                led.on()
            else:
                print(average,"  LED OFF!")
                led.off()
            
            count = 0
            count_sum = 0
        
        time.sleep(0.1)

except KeyboardInterrupt:
    print("종료")