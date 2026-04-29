from gpiozero import DistanceSensor
from signal import pause

sensor = DistanceSensor(echo = 23, trigger = 24, threshold_distance = 0.3)

sensor.when_in_range = lambda: print("30cm이내 접근!")
sensor.when_out_of_range = lambda: print("멀어짐")


try:
   pause()
except KeyboardInterrupt:
   sensor.close()
