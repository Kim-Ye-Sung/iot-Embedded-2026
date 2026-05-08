from gpiozero import DistanceSensor
from gpiozero import PWMOutputDevice
from time import sleep, monotonic
import statistics

sensor = DistanceSensor(echo=23, trigger=24)
buzzer = PWMOutputDevice(21)
buzzer.frequency = 392.00

buzzer_state = False          # 현재 부저가 켜져 있는지 꺼져 있는지
last_buzzer_time = monotonic() # 마지막으로 부저 상태를 바꾼 시간


def get_distance_cm(samples=5):
    values = []
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
        print(f"거리(평균): {dist:.2f} cm")

        if dist < 10.0:
            update_buzzer(0.25)   # 0.25초마다 켜짐/꺼짐 변경
        elif dist < 30.0:
            update_buzzer(0.5)    # 0.5초마다 켜짐/꺼짐 변경
        else:
            buzzer.off()
            buzzer_state = False

        sleep(0.05)

except KeyboardInterrupt:
    pass

finally:
    buzzer.off()
    buzzer.close()
    sensor.close()
