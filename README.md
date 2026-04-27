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
