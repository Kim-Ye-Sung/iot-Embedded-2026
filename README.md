# iot-Embedded-2026
Iot 개발자과정 임베디드 기초프로그래밍


# 2026-04-27
## 라즈베리파이
- 라즈베리파이5를 사용하였으며, 64GB 크기의 SD카드가 꽂혀있음
- https://www.raspberrypi.com/software/   접속하여 다운로드하고 설치
- sd카드를 컴퓨터와 연결한 상태로 이미저를 설정
- 아이디 및 이름 : rpi, 비밀번호: raspi
- sd카드를 라즈베이파이에 꽂은채로 라즈베리파이에 전원을 넣어서 실행한다.
- 그리고 네트워크 관리자(192.168.0.1)에 들어가서 내부 네트워크 설정에 들어가면 "rpi"라는 이름으로 ip주소를 확인할수있다.
- 그러고 cmd 창에서 `ssh rpi@할당받은 주소`를 입력한다.
- [vnc viewer](https://www.realvnc.com/en/connect/download/viewer/)를 접속하여 다운로드하고 설치한다.
- cmd창에서 `sudo raspi-config`를 입력한다.
- `Interface Options`에서 `VNC`를 들어간다. 그렇게 접속이 완료되면 VNC Viewer를 들어간다.
- vnc viewer에서 "rpi"의 ip주소를 입력하고 :0을 붙인다. 예) 192.168.0.3:0
- 이러고 아이디와 비밀번호를 입력하면 원격조종이 라즈베리 파이의 원격조종이 가능해진다.
- sudo apt install fonts-nanum fonts-nanum-extra // 폰트설치
- sudo apt install fonts-unfonts-core  // 폰트 등록
- sudo apt install ibus // 입력시 설치
- sudo apt install ibus-hangul  // ibus 패키지설치
- ibus-setup // 여기서 한글 설정 가능
- ibus restart // 한글 전환키가 먹으려면 이걸로 해야함.

- sudo apt install libgpiod-dev gpiod -y   // 이걸로 c언어 패키지를 설치한다.
- sudo apt install python3-libgpiod     // 이걸로 파이썬언어 패키지를 설치한다.

- python -m venv --system-site-packages .venv   // 가상환경 만들기
- source ./.venv/bin/activate  // 만들어둔 가상환경 진입

라즈베리 파이에서 GND = 0V, 기준전압이다.
- `pinout`으로 라즈베리파이의 핀번호를 확인 가능하다.

- 하드웨어적인 관점에서 볼때, 딱히 켜주는 코드가 없더라도, 전위차에 의해 전류가 흐르기 때문에 LED가 전부 켜진 상태로 시작한다.
- 따라서 코드상에서는 항상 동작안하고픈 하드웨어는 먼저 꺼주는 코드를 반드시 넣고 시작해야한다.
- [코드](./260427/led01.py)


# 2026-04-28
- `pinctrl set 14 op`  // 14번핀을 output(출력핀)으로 설정하겠다.
- `pinctrl set 14 dl`  // 출력핀인 14번핀에 low값(0V)로 설정하라 = 작동하게끔 해라
- `pinctrl set 14 dh`  // 출력핀인 14번핀에 high값(5V)로 설정하다 = 동작을 중지하라

- 캐소드 타입, 애노드 타입이면 위의 코드가 서로 반대로 동작한다.
- `gpioinfo`로 핀의 정보를 확인할 수 있다.

## 스위치

- LED는 GPIO에서 전기를 내보내서 제어하는 출력 장치이고,
  스위치는 GPIO가 현재 전기 상태를 읽는 입력 장치이다.

- 스위치는 스스로 0이나 1을 만들어내는 부품이 아니다.
  스위치는 단순히 회로를 연결하거나 끊어주는 부품이다.

- 스위치를 누르면 끊어져 있던 회로가 연결된다.
  그래서 GPIO 입력핀에 3.3V 또는 GND가 연결되면서
  HIGH(1) 또는 LOW(0) 값을 읽을 수 있다.

- 라즈베리파이 GPIO 입력핀은 3.3V 기준으로 동작한다.
  따라서 GPIO 입력핀에 5V를 직접 넣으면 안 된다.
  5V가 들어가면 GPIO 핀이 손상될 수 있다.

- 스위치를 누르지 않았을 때는 GPIO 입력핀이 아무 곳에도 연결되지 않은 상태가 될 수 있다.
  이런 상태를 floating 상태라고 한다.

- floating 상태에서는 GPIO가 0인지 1인지 안정적으로 알 수 없다.
  그래서 스위치를 누르지 않았는데도 값이 1로 읽히거나,
  값이 계속 흔들릴 수 있다.

- 이 문제를 막기 위해 저항을 사용한다.
  저항을 이용해서 스위치를 누르지 않았을 때의 기본값을
  HIGH(1) 또는 LOW(0) 중 하나로 고정해준다.

- 스위치 입력에 사용하는 저항에는 풀다운 저항과 풀업 저항이 있다.

## 풀다운 저항

- 풀다운 저항은 GPIO 입력핀을 GND 쪽으로 약하게 연결하는 저항이다.

- 풀다운 저항을 사용하면 스위치를 누르지 않았을 때
  GPIO 입력핀은 GND에 연결된 것처럼 되어 LOW(0)로 읽힌다.

- 스위치를 누르면 GPIO 입력핀이 3.3V와 연결되어 HIGH(1)로 읽힌다.

- 즉, 풀다운 방식에서는 다음과 같이 동작한다.

  스위치를 누르지 않음 → LOW(0)
  스위치를 누름       → HIGH(1)

- 연결 구조는 대략 다음과 같다.

  3.3V ---- 스위치 ---- GPIO 입력핀
                         |
                       저항
                         |
                        GND

- 여기서 저항은 GPIO 입력핀이 평소에 GND 쪽으로 연결되게 만들어준다.
  그래서 스위치를 누르지 않았을 때 값이 흔들리지 않고 0으로 안정된다.

## 풀업 저항

- 풀업 저항은 GPIO 입력핀을 3.3V 쪽으로 약하게 연결하는 저항이다.

- 풀업 저항을 사용하면 스위치를 누르지 않았을 때
  GPIO 입력핀은 3.3V에 연결된 것처럼 되어 HIGH(1)로 읽힌다.

- 스위치를 누르면 GPIO 입력핀이 GND와 연결되어 LOW(0)로 읽힌다.

- 즉, 풀업 방식에서는 다음과 같이 동작한다.

  스위치를 누르지 않음 → HIGH(1)
  스위치를 누름       → LOW(0)

- 연결 구조는 대략 다음과 같다.

  3.3V
   |
  저항
   |
  GPIO 입력핀 ---- 스위치 ---- GND

- 여기서 저항은 GPIO 입력핀이 평소에 3.3V 쪽으로 연결되게 만들어준다.
  그래서 스위치를 누르지 않았을 때 값이 흔들리지 않고 1로 안정된다.

## 저항이 필요한 이유

- 스위치가 열려 있을 때 GPIO 입력핀이 아무 곳에도 연결되지 않으면
  GPIO는 0인지 1인지 정확히 판단할 수 없다.

- 이 상태를 floating 상태라고 한다.

- floating 상태에서는 주변 전기 신호나 노이즈 때문에
  입력값이 0과 1 사이에서 불안정하게 바뀔 수 있다.

- 그래서 저항을 사용해서 GPIO 입력핀의 기본 상태를 정해준다.

- 풀다운 저항을 사용하면 기본값이 0이 되고,
  풀업 저항을 사용하면 기본값이 1이 된다.

## 내부 풀업/풀다운 저항

- 라즈베리파이에는 GPIO 핀 안에 내부 풀업 저항과 내부 풀다운 저항 기능이 있다.

- 그래서 항상 외부에 실제 저항 부품을 연결하지 않아도,
  코드에서 내부 풀업 또는 내부 풀다운을 설정할 수 있다.

- 예를 들어 gpiozero에서 Button을 사용할 때 pull_up 값을 설정할 수 있다.

## 풀다운 방식 예시

from gpiozero import Button
from signal import pause

button = Button(17, pull_up=False)

button.when_pressed = lambda: print("스위치 눌림")
button.when_released = lambda: print("스위치 뗌")

pause()

- pull_up=False는 내부 풀다운 저항을 사용한다는 뜻이다.

- 이 경우 스위치를 누르지 않았을 때는 0,
  스위치를 눌렀을 때는 1로 인식한다.

- 연결은 다음과 같이 생각하면 된다.

  3.3V ---- 스위치 ---- GPIO17

  GPIO17은 내부 풀다운 저항 때문에 평소에는 0이다.
  스위치를 누르면 3.3V가 연결되어 1이 된다.

## 풀업 방식 예시

from gpiozero import Button
from signal import pause

button = Button(17, pull_up=True)

button.when_pressed = lambda: print("스위치 눌림")
button.when_released = lambda: print("스위치 뗌")

pause()

- pull_up=True는 내부 풀업 저항을 사용한다는 뜻이다.

- 이 경우 스위치를 누르지 않았을 때는 1,
  스위치를 눌렀을 때는 0으로 인식한다.

- 연결은 다음과 같이 생각하면 된다.

  GPIO17 ---- 스위치 ---- GND

  GPIO17은 내부 풀업 저항 때문에 평소에는 1이다.
  스위치를 누르면 GND와 연결되어 0이 된다.

- gpiozero의 Button은 pull_up=True일 때
  스위치를 눌러 값이 0이 되어도 이것을 '눌림'으로 처리해준다.
  그래서 사용자는 편하게 button.when_pressed를 사용할 수 있다.


  ### 버튼을 이용하여 led켜고 끄기
  - [코드1](./260428/button_base.py)
  - [코드2](./260428/button_interrupt.py)

  ### 컨버터 사용하기
  - Work/py폴더에서 가상환경을 만든다.
  - `ls /dev/i2c*` 을 실행하면 /dev/i2c-1이 있어야한다.
  - `sudo apt install i2c-tools` 를 한다.
  - `i2cdetect -y 1`을 입력하면 48이 나온다.

```py 
# PCF8591.py

 import smbus
 import time
 
 bus = smbus.SMBus(1)
 ADDR = 0x48
 
 def read_cds():
    bus.write_byte(ADDR, 0x41)  # AIN1
    bus.read_byte(ADDR)         # dummy
    return bus.read_byte(ADDR)

try:
    while True:
       val = read_cds()
       print("CDS:", val)
       time.sleep(0.5)
except KeyboardInterrupt:
    print("종료")
```


# 2026-04-29
- 조도센서를 사용하여 어두우면 LED켜기 [코드](./260429/PCF.py)
- 부저 소리 [코드](./260429/Buzzer.py)
- 초음파 센서 [코드1](./260429/UltraSonic.py), [코드2](./260429/UltraSonic2.py), 
- 초음파 센서 거리에 따라 부저소리 주기 다르게 나게 하기 [코드](./260429/UltraSonic3.py)
- LCD 코드 [코드](./260429/LCD.py)
- 초음파 센서 거리를 LCD에 띄우고 거리에 따라 부저소리 주기 다르게 나게 하기 [코드](./260429/LCD2.py)


# 2026-04-30
- 모터는 반드시 드라이버와 함께 사용해야한다.(자기장?) 모터 혼자 직접 제어하는건 불가능하다. 라즈베리파이가 죽어버린다.
- 1상여자, 2상여자, 1-2상 여자가 있다. 1상여자방식보다 2상 여자 방식이 힘이 크다. 


- 온습도 센서

- 키패드


# 2026-05-06
`지각해서 12시에 도착했음. 앞에 내용 전혀 모름`

- 아마 조이스틱한거 같음. 오자마자 조이스틱이 있음

1. 일단 내가 오자마자 수업 듣는건 flask서버 만드는거인듯? flask04.py코드부터 시작했음

2. html 작성했다. flask서버를 실행하는 파이썬 파일이 있는 위치에 `templates`라는 폴더가 반드시 존재해야한다. 그리고 그 폴더 안에 html파일이 있어야한다.

3. 플라스크 서버로 led제어하기

4. 플라스크 서버로 led, 부저, 초음파 센서를 작동하고 그에따른 상태를 사이트 화면에 표시함


# 2026-05-07
- qt를 사용하요 ui 제작
## qt설치 과정
1. 라즈베리파이의 터미널에서 `sudo apt install build-essential cmake gdb`를 입력하여 설치한다.
2. sudo apt install -y qt6-base-dev qt6-base-dev-tools
3. sudo apt install -y qt6-tools-dev qt6-tools-dev-tools qml6-module-qtquick qml6-module-qtquick-controls
4. sudo apt install -y qtcreator
5. qmake6 --version
6. qt앱을 확인하려면 vnc viewer에서 왼쪽 상단의 산딸기 마크에서 programming 부분을 보면 qt가 있다.
7. qt 사용하는건 프로젝트 해봤으니 알거다.



# 2026-05-11
- ros란 운영체제인것처럼 생각이 들지만 사실 운영체제인척하는 일종의 라이브러리이다?

- 일단 sd카드를 뽑아서 초기화하고 ros를 설치했다. 
- 우분투24.04.4 LTS를 설치한다.

- 사용자 : rpi
- 컴퓨터이름: rospi
- 비밀번호: ubuntu

- 라즈베리파이(ros2)와 vnc 연결을 위해 해줘야할것
1. cd Downloads
2. wget https://downloads.realvnc.com/download/file/vnc.files/VNC-Server-7.16.0-Linux-ARM64.deb
3. chmod u+x VNC-Server-7.16.0-Linux-ARM64.deb
4. sudo apt install ./VNC-Server-7.16.0-Linux-ARM64.deb
5. sudo nanorc
6. sudo nano /etc/gdm3/custom.conf      // 여기서 "WaylandEnable = false"라는 곳을 주석 해제한다.
7. sudo apt install xserver-xorg-video-dummy xinit
8. sudo nano /etc/X11/xorg.conf  // 완전히 비어있는 파일이 맞다. 여기에 아래와 같이 입력한다.
  -Section "Device"
      Identifier "Configured Video Device"
      Driver "dummy"
EndSection
 
Section "Monitor"
      Identifier "Configured Monitor"
      HorizSync 28-80
      VertRefresh 48-75
     Modeline "1920x1080" 172.80 1920 2040 2248 2576 1080 1081 1084 1118
EndSection

Section "Screen"
      Identifier "Default Screen"
      Device "Configured Video Device"
      Monitor "Configured Monitor"
      DefaultDepth 24
      SubSection "Display"
             Depth 24
             Modes "1920x1080"
      EndSubSection
EndSection

9. sudo systemctl enable vncserver-x11-serviced.service
10. sudo systemctl start vncserver-x11-serviced.service
11. sudo systemctl status vncserver-x11-serviced.service
12. sudo nano /boot/firmware/config.txt  // 여기서 맨 아래에 다음과 같은 코드들을 추가한다.
  - hdmi_force_hotplug=1
  - hdmi_group=2
  - hdmi_mode=82
13. 여기까지 했으면 vnc연결이 가능해진다. ip a로 ip를 확인하고 연결해보자.
14. 이 다음부터는 ros2를 설치하면된다. 아래의 사이트에서 "Set locale" ~ "Setup environment"까지 쭉 코드 따라쳐라. 혹은 복붙
[참고 사이트](https://docs.ros.org/en/jazzy/Installation/Ubuntu-Install-Debs.html)


## vmware 가상머신에서 기존 리눅스환경에 우분투 jazzy 설치해보기("Set locale" ~ "Setup environment"까지 쭉 코드 따라쳐라. 혹은 복붙)
[참고 사이트](https://docs.ros.org/en/jazzy/Installation/Ubuntu-Install-Debs.html)

## 가상머신에서 주피터 노트북?
- jupyter lab으로 서버를 실행한다.

- 자세한 순서 설명?
1. 가상환경으로 실행해야하니 가상환경을 실행시켜줄 폴더를 하나 생성한다. 그리고 그 폴더로 이동한다.
  - mkdir -p venvs        cd venvs
2. 파이선 3.12를 설치해준다?
  - sudo apt install python3.12-venv
3. .venv라는 가상환경을 만들어준다.
  - python3 -m venv --system-site-packages .venv
4. 만들어둔 가상환경을 실행한다.
  - source .venv/bin/activate
5. 파이선 pip를 업그레이드 해준다?
  - python -m pip install --upgrade pip
6. 커널 생성해준다?
  - python -m pip install ipykernel
7. 주피터 랩 설치
  - pip3 install jupyter lab
8. 커널 이름 등록
python -m ipykernel install --user --name .venv --display-name "python(ros:jazzy)"
9. 주피터랩 실행?
  - jupyter lab   // 일단 이거 실행하기 전에 아래의 포트 할당받는법 한번 볼 것

## 주피터 노트북실행할때 ip=0.0.0.0  port=7890으로 무조건 할당받는 방법
1. 일단 가상환경 먼저 실행
  - source ~/venvs/.venv/bin/activate
2. 설정파일 만들기
  - jupyter server --generate-config
3. 설정파일 열기
  - nano ~/.jupyter/jupyter_server_config.py
4. 파일 맨아래로 가서 다음과 같은 내용 추가
  - c.ServerApp.ip = '0.0.0.0'
  - c.ServerApp.port = 7890
  - c.ServerApp.port_retries = 0
  - c.ServerApp.open_browser = False
5. 위의 과정을 완료했다면 jupyer lab만 입력해도 7890포트로 할당받아서 실행가능.


## 위의 순서랑 똑같은건데 gpt가 정리해준거?
# 1. 가상환경을 모아둘 폴더 생성
mkdir -p ~/venvs

# 2. 폴더 이동
cd ~/venvs

# 3. venv 관련 패키지 설치
sudo apt update
sudo apt install python3-venv python3-pip

# 4. 가상환경 생성
python3 -m venv .venv

# 임베디드/라즈베리파이 시스템 패키지도 같이 쓰고 싶으면 대신 이 명령 사용
# python3 -m venv --system-site-packages .venv

# 5. 가상환경 실행
source .venv/bin/activate

# 6. pip 업그레이드
python -m pip install --upgrade pip

# 7. Jupyter 커널 설치
python -m pip install ipykernel

# 8. JupyterLab 설치
python -m pip install jupyterlab

# 9. 커널 이름 등록
python -m ipykernel install --user --name .venv --display-name "python(ros:jazzy)"

# 10. JupyterLab 실행
jupyter lab --ip=0.0.0.0 --port=8888 --no-browser


# 2026-05-12
- 아래 사이트대로 해서 라즈베리파이에 ros2설치 그대로 설치. "Set locale" ~ "Setup environment"까지 쭉 코드 따라쳐라. 혹은 복붙
[참고 사이트](https://docs.ros.org/en/jazzy/Installation/Ubuntu-Install-Debs.html)

- 설치가 다 되면 사용이 가능한게 아니다. ros2를 사용하기 위해서는 다음과 같은 코드로 가상환경처럼 환경을 불러와야한다.
  - source /opt/ros/jazzy/setup.bash
- 그런데 매번 컴퓨터를 켤때마다 새롭게 위의 코드를 입력해줘야하는것은 귀찮다. 따라서 다음과 같은 코드로 설정해준다.
  - echo "source /opt/ros/jazzy/setup.bash" >> ~/.bashrc
  - 또는 nano ~/.bashrc 로 들어가서 맨아랫줄에 source /opt/ros/jazzy/setup.bash를 직접 입력해도 된다. 그런데 위의 코드를 하는게 더 편하다.
  - 실제로 코드를 실행하고나서 nano ~/.bashrc로 들어가면 확인가능하다.

- 거북이 한번 사용해보기
  - ros2 run turtlesim turtlesim_node  // 거북이 켜기
  - ros2 run turtlesim turtle_teleop_key  // 거북이를 방향키로 움직일수있는 코드.현재 거북이가 켜져있는 터미널 외에 다른 터미널에서 실행해야함

  - ros2 node list // 현재 켜져있는 노드들을 확인하는 코드

  - rqt   // 현재 상태 확인

  - ros2 run turtlesim turtlesim_node --ros-args --remap __node:=my_turtle  // 거북이켜는것과 똑같은데, 노드 이름을 my_turtle로 하기

  - ros2 node info /my_turtle  // my_turtle 노드에 대한 정보 보기

  - ros2 topic list  // 실행되고 있는 토픽들을 확인하는 코드(토픽들의 이름을 확인할 수 있는 코드)
  - ros2 topic list -t  // 좀 더 자세하게 정보들을 확인할수있는 코드(각 토픽의 메시지 타입을 추가로 볼수있는 코드)
  - ros2 topic echo /turtle1/cmd_vel // 방향키로 거북이를 움직이면 위치 데이터를 볼 수 있다.
  - ros2 topic info /turtle1/cmd_vel  
  - ros2 topic type /turtle1/pose   // pose라는 토픽이 어떤 메시지 타입을 사용하는지 확인하는 코드
  - ros2 interface show /turtlesim/msg/Pose // Pose라는 메시지 타입의 내부 데이터 구조를 확인하는 코드
  - ros2 topic echo /turtle1/pose
  - ros2 topic pub --once /turtle1/cmd_vel geometry_msgs/msg/Twist "{linear: {x: 2.0}, angular: {z: 0.0}}" // 거북이가 x축으로 2.0만큼 가도록 한번만 실행한다.
  - ros2 topic pub --once /turtle1/cmd_vel geometry_msgs/msg/Twist "{linear: {x: 2.0}, angular: {z: 2.0}}" // 각도를 주며 이동 
  - ros2 topic pub --rate 1 /turtle1/cmd_vel geometry_msgs/msg/Twist "{linear: {x: 2.0}, angular: {z: 1.8}}" // 1초에 한바퀴 회전
  - ros2 topic pub --rate 1 /turtle1/cmd_vel geometry_msgs/msg/Twist "{linear: {x: 2.0}, angular: {z: 3.7}}" // 다른 터미널에서 실행하면 두개의 코드가 한꺼번에 실행되서 그에 맞는 움직임을 보인다.
  - ros2 service list  // 서비스 목록 확인
  - ros2 service type /turtle1/teleport_absolute // 해당 서비스 타입 확인
  - ros2 service call /turtle1/teleport_absolute turtlesim/srv/TeleportAbsolute "{x: 3, y: 7, theta: 0}" // x3, y7로 거북이가 순간이동
  - ros2 service call /reset std_srvs/srv/Empty {}  // reset 서비스의 데이터 타입을 확인하면 아무것도 없다. 따라서 사용하려면 코드와 같이 빈 공간으로 줘야한다. 
  -


  # 뭘 하고 있는건지 모르겠음. 서버를 켜기 위한 작업같은데?
  1. mkdir -p ros2_ws/src      // 새롭게 작업할 폴더를 만듦
  2. cd ros2_ws   // 작업 폴더로 이동
  3. colcon build   // 뭔지 모르겠음
  4. source install/setup.bash    // ros2의 환경을 켜주는것과 마찬가지로 이 환경을 켜준다
  5. cd src // src 폴더로 이동
  6. ros2 pkg create --build-type ament_python --node-name my_node my_pkg --dependencies rclpy std_msgs   // 패키지 생성
  7. sudo apt install tree    // tree를 설치
  8. cd ..   // 다시 ros2_ws로 이동
  9. tree     // tree 실행. 만든 폴더와 파일들을 계층적으로 확인가능하다. 
  10. colcon build   // 다시 빌드 실행?
  11. source install/setup.bash  // 다시 환경 적용
  12. ros2 run my_pkg my_node   // 노드를 실행?
  13. 