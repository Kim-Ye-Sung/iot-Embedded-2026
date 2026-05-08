from gpiozero import PWMOutputDevice
from time import sleep

buzzer = PWMOutputDevice(21)

notes = {
    "do": 261.63,
    "re": 293.66,
    "mi": 329.63,
    "fa": 349.23,
    "sol": 392.00,
    "ra": 440.00,
    "si": 493.88,
    "high_do": 523.25
}

song = ["sol", "mi", "mi", 0.5,"fa", "re", "re", 0.5,"do", "re", "mi", "fa","sol", "sol", "sol", 0.5,"sol", "mi", "mi", "mi","fa", "re", "re", 0.5,"do", "mi", "sol", "sol","mi", "mi", "mi"]

try:
    i = 0

    while i < len(song):
        note = song[i]

        # 숫자가 단독으로 나오면 음이 아니므로 건너뜀
        if isinstance(note, (int, float)):
            i += 1
            continue

        freq = notes[note]
        buzzer.frequency = freq
        buzzer.value = 0.5

        sleep(0.5)

        # 다음 값이 숫자라면, 현재 음을 더 길게 연주
        if i + 1 < len(song) and isinstance(song[i + 1], (int, float)):
            time = song[i + 1]
            sleep(time)

            # 숫자까지 처리했으므로 2칸 이동
            i += 2
        else:
            # 그냥 음 하나만 처리했으므로 1칸 이동
            i += 1

        buzzer.off()
        sleep(0.1)

finally:
    buzzer.close()
