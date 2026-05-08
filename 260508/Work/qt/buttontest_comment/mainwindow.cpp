#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QProcess>
#include <QDebug>

/*
 * MainWindow 생성자
 *
 * 프로그램 창이 만들어질 때 자동으로 실행됨
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    /*
     * Qt Designer에서 만든 UI를 실제 코드와 연결
     *
     * 이 코드를 실행해야 ui->pushButton,
     * ui->label_led 같은 UI 요소를 사용할 수 있음
     */
    ui->setupUi(this);

    /*
     * =========================
     * 1. 부저 스레드 생성 및 시작
     * =========================
     */

    /*
     * BuzzerThread 객체 생성
     *
     * buzzerPin은 mainwindow.h에서 24로 설정되어 있음
     * 즉, GPIO24에 연결된 부저를 제어함
     *
     * this를 parent로 넘기면 MainWindow가 부모가 됨
     */
    buzzerThread = new BuzzerThread(buzzerPin, this);

    /*
     * 부저 스레드 시작
     *
     * start()를 호출하면 BuzzerThread의 run() 함수가
     * 별도 스레드에서 실행됨
     */
    buzzerThread->start();

    /*
     * =========================
     * 2. 초음파 센서 스레드 생성 및 시작
     * =========================
     */

    /*
     * UltrasonicThread 객체 생성
     *
     * ultrasonicTrigPin = GPIO7
     * ultrasonicEchoPin = GPIO8
     */
    ultrasonicThread = new UltrasonicThread(ultrasonicTrigPin, ultrasonicEchoPin, this);

    /*
     * 초음파 스레드의 DistanceChanged 시그널을
     * MainWindow의 OnUltraSonicDistanceChanged 슬롯과 연결
     *
     * 의미:
     * - UltrasonicThread에서 거리 평균값을 계산함
     * - emit DistanceChanged(avgDistance)를 호출함
     * - 그러면 MainWindow의 OnUltraSonicDistanceChanged(avgDistance)가 자동 호출됨
     */
    connect(
        ultrasonicThread,
        &UltrasonicThread::DistanceChanged,
        this,
        &MainWindow::OnUltraSonicDistanceChanged
        );

    /*
     * 초음파 거리 측정 스레드 시작
     */
    ultrasonicThread->start();

    /*
     * =========================
     * 3. 부저 기본값 설정
     * =========================
     */

    /*
     * 기본 주파수를 261.63Hz로 설정
     *
     * 261.63Hz는 도 음계에 가까운 값
     */
    currentBuzzerFrequency = 261.63;

    /*
     * 기본 볼륨은 0
     *
     * 프로그램 시작하자마자 소리가 나지 않게 하기 위함
     */
    currentBuzzerVolume = 0.0;

    /*
     * VolumeDial의 최소값을 0으로 설정
     */
    ui->VolumeDial->setMinimum(0);

    /*
     * VolumeDial의 최대값을 100으로 설정
     *
     * 다이얼 값 0   -> 볼륨 0.0
     * 다이얼 값 100 -> 볼륨 0.9
     */
    ui->VolumeDial->setMaximum(100);

    /*
     * 처음 다이얼 값을 0으로 설정
     */
    ui->VolumeDial->setValue(0);

    /*
     * 부저 스레드에 현재 주파수 전달
     */
    SetBuzzerFrequency(currentBuzzerFrequency);

    /*
     * 부저 스레드에 현재 볼륨 전달
     * 현재 볼륨이 0이므로 소리는 나지 않음
     */
    SetBuzzerVolume(currentBuzzerVolume);

    /*
     * =========================
     * 4. LED 초기화
     * =========================
     *
     * 프로그램 시작 시 모든 LED를 꺼두기 위한 코드
     *
     * currentLED를 Red -> Green -> Blue 순서로 바꾸면서
     * LED_Off()를 호출함
     */
    for (int i = 0; i < static_cast<int>(leds.size()); i++)
    {
        /*
         * 현재 선택된 LED 끄기
         */
        LED_Off();

        /*
         * currentLED를 다음 색상으로 변경
         *
         * static_cast<int>(currentLED)
         * - enum 값을 int로 변환
         *
         * + 1
         * - 다음 색상으로 이동
         *
         * % leds.size()
         * - 마지막 색상 다음에는 다시 0으로 돌아오게 함
         *
         * static_cast<LED_Color>
         * - int 값을 다시 LED_Color enum으로 변환
         */
        currentLED = static_cast<LED_Color>((static_cast<int>(currentLED) + 1) % leds.size());
    }

    /*
     * 초기화가 끝난 뒤 다시 Red로 맞춰두고 싶다면 아래 코드 사용 가능
     *
     * currentLED = Red;
     */
}

/*
 * MainWindow 소멸자
 *
 * 프로그램이 종료될 때 실행됨
 */
MainWindow::~MainWindow()
{
    /*
     * 부저 끄기
     */
    BuzzerOff();

    /*
     * 부저 스레드가 존재하면 종료
     */
    if (buzzerThread != nullptr)
    {
        /*
         * 스레드 안전 종료
         */
        buzzerThread->StopThread();

        /*
         * parent가 this라서 Qt가 나중에 delete할 수 있지만,
         * 여기서는 더 이상 사용하지 않도록 nullptr 처리
         */
        buzzerThread = nullptr;
    }

    /*
     * 초음파 스레드가 존재하면 종료
     */
    if (ultrasonicThread != nullptr)
    {
        /*
         * 거리 측정 스레드 안전 종료
         */
        ultrasonicThread->StopThread();

        /*
         * 더 이상 사용하지 않도록 nullptr 처리
         */
        ultrasonicThread = nullptr;
    }

    /*
     * UI 객체 삭제
     */
    delete ui;
}

/*
 * LED ON 버튼 클릭 시 실행
 */
void MainWindow::on_pushButton_clicked()
{
    /*
     * 현재 선택된 LED 색상에 맞게 label_led 배경색 변경
     */
    SetLED_StyleSheet();

    /*
     * 화면에 On 표시
     */
    ui->label_led->setText("On");

    /*
     * 실제 GPIO LED 켜기
     */
    LED_On();
}

/*
 * LED OFF 버튼 클릭 시 실행
 */
void MainWindow::on_pushButton_2_clicked()
{
    /*
     * LED가 꺼졌다는 것을 UI에서 회색으로 표시
     */
    ui->label_led->setStyleSheet("background-color:gray;border-radius:20px;");

    /*
     * 화면에 Off 표시
     */
    ui->label_led->setText("Off");

    /*
     * 실제 GPIO LED 끄기
     */
    LED_Off();
}

/*
 * LED 색상 변경 버튼 클릭 시 실행
 */
void MainWindow::on_ColorChangeBtn_clicked()
{
    /*
     * LED가 켜져 있을 때만 색상 변경
     *
     * 꺼져 있을 때 색상만 바뀌면 사용자가 헷갈릴 수 있으므로
     * 켜져 있는 상태에서만 색상 변경하도록 함
     */
    if (IsLED_ON)
    {
        /*
         * 현재 켜져 있는 LED를 먼저 끔
         */
        LED_Off();

        /*
         * currentLED를 다음 색상으로 변경
         *
         * Red -> Green -> Blue -> Red 순서로 반복
         */
        currentLED = static_cast<LED_Color>((static_cast<int>(currentLED) + 1) % leds.size());

        /*
         * 바뀐 색상에 맞게 UI label 색상 변경
         */
        SetLED_StyleSheet();

        /*
         * 새로 선택된 LED 켜기
         */
        LED_On();
    }
}

/*
 * 현재 선택된 LED를 켜는 함수
 */
void MainWindow::LED_On()
{
    /*
     * LED 상태 변수 변경
     */
    IsLED_ON = true;

    /*
     * 기존에 실행 중인 gpioset 프로세스 종료
     *
     * gpioset은 GPIO 핀을 잡고 있을 수 있음
     * 그래서 새로 제어하기 전에 기존 gpioset을 종료함
     */
    QProcess::execute("pkill", QStringList() << "gpioset");

    /*
     * gpioset 명령어를 실행해서 LED 켜기
     *
     * 현재 회로는 active-low라고 가정
     *
     * active-low란?
     * - GPIO 값이 0이면 LED ON
     * - GPIO 값이 1이면 LED OFF
     *
     * 그래서 LED를 켤 때는 GPIO핀=0 으로 설정함
     */
    QProcess::startDetached(
        "gpioset",
        QStringList()
            << "-c"
            << "/dev/gpiochip0"
            << QString::number(leds[static_cast<int>(currentLED)]) + "=0"
        );
}

/*
 * 현재 선택된 LED를 끄는 함수
 */
void MainWindow::LED_Off()
{
    /*
     * LED 상태 변수 변경
     */
    IsLED_ON = false;

    /*
     * 기존 gpioset 종료
     */
    QProcess::execute("pkill", QStringList() << "gpioset");

    /*
     * active-low 회로이므로
     * GPIO 값을 1로 만들면 LED OFF
     */
    QProcess::startDetached(
        "gpioset",
        QStringList()
            << "-c"
            << "/dev/gpiochip0"
            << QString::number(leds[static_cast<int>(currentLED)]) + "=1"
        );
}

/*
 * 현재 LED 색상에 맞게 label_led 스타일 변경
 */
void MainWindow::SetLED_StyleSheet()
{
    /*
     * currentLED 값에 따라 다른 스타일 적용
     */
    switch (currentLED)
    {
    case Red:
        /*
         * 빨간색 LED 선택 상태
         */
        ui->label_led->setStyleSheet("background-color:red;border-radius:20px;");
        break;

    case Green:
        /*
         * 초록색 LED 선택 상태
         */
        ui->label_led->setStyleSheet("background-color:green;border-radius:20px;");
        break;

    case Blue:
        /*
         * 파란색 LED 선택 상태
         */
        ui->label_led->setStyleSheet("background-color:blue;border-radius:20px;");
        break;
    }
}

/*
 * NoteSlider 값이 바뀔 때 실행
 *
 * value:
 * - 현재 슬라이더 값
 */
void MainWindow::on_NoteSlider_valueChanged(int value)
{
    /*
     * 최소 주파수
     *
     * 슬라이더가 최소일 때 부저 주파수
     */
    double minFreq = 261.63;

    /*
     * 최대 주파수
     *
     * 슬라이더가 최대일 때 부저 주파수
     */
    double maxFreq = 523.25;

    /*
     * 슬라이더의 최소값 가져오기
     *
     * Qt Designer에서 설정한 최소값이 있으면 그 값을 사용
     */
    int sliderMin = ui->NoteSlider->minimum();

    /*
     * 슬라이더의 최대값 가져오기
     */
    int sliderMax = ui->NoteSlider->maximum();

    /*
     * 슬라이더 위치를 0.0 ~ 1.0 사이 비율로 변환
     *
     * 예:
     * - 슬라이더가 최소면 ratio = 0.0
     * - 슬라이더가 중간이면 ratio = 0.5
     * - 슬라이더가 최대면 ratio = 1.0
     */
    double ratio = double(value - sliderMin) / double(sliderMax - sliderMin);

    /*
     * 비율을 이용해서 주파수 계산
     *
     * ratio가 0.0이면 minFreq
     * ratio가 1.0이면 maxFreq
     * ratio가 0.5이면 두 값의 중간 주파수
     */
    currentBuzzerFrequency = minFreq + ratio * (maxFreq - minFreq);

    /*
     * 디버깅용 출력
     */
    qDebug() << "Slider value:" << value;
    qDebug() << "Frequency:" << currentBuzzerFrequency;

    /*
     * 계산된 주파수를 부저 스레드에 전달
     */
    SetBuzzerFrequency(currentBuzzerFrequency);
}

/*
 * 부저 주파수 설정 함수
 */
void MainWindow::SetBuzzerFrequency(double frequency)
{
    /*
     * 부저 스레드가 아직 생성되지 않았으면 아무것도 하지 않음
     *
     * nullptr 체크를 하지 않으면 프로그램이 터질 수 있음
     */
    if (buzzerThread == nullptr)
    {
        return;
    }

    /*
     * BuzzerThread에 주파수 전달
     */
    buzzerThread->SetFrequency(frequency);

    /*
     * 디버깅용 출력
     */
    qDebug() << "Buzzer frequency:" << frequency;
}

/*
 * 부저 끄기 함수
 */
void MainWindow::BuzzerOff()
{
    /*
     * 부저 스레드가 없으면 아무것도 하지 않음
     */
    if (buzzerThread == nullptr)
    {
        return;
    }

    /*
     * 현재 볼륨 값을 0으로 저장
     */
    currentBuzzerVolume = 0.0;

    /*
     * BuzzerThread 내부 주파수를 0으로 만들어 부저 끄기
     */
    buzzerThread->StopBuzzer();
}

/*
 * 부저 볼륨 설정 함수
 */
void MainWindow::SetBuzzerVolume(double volume)
{
    /*
     * 부저 스레드가 없으면 아무것도 하지 않음
     */
    if (buzzerThread == nullptr)
    {
        return;
    }

    /*
     * BuzzerThread에 볼륨 전달
     */
    buzzerThread->SetVolume(volume);

    /*
     * 디버깅용 출력
     */
    qDebug() << "Buzzer volume:" << volume;
}

/*
 * VolumeDial 값이 바뀔 때 실행
 *
 * value:
 * - 현재 다이얼 값
 * - 위에서 최소 0, 최대 100으로 설정했음
 */
void MainWindow::on_VolumeDial_valueChanged(int value)
{
    /*
     * 최소 볼륨
     */
    double minVolume = 0.0;

    /*
     * 최대 볼륨
     *
     * 1.0까지 쓰지 않고 0.9까지만 쓰는 이유:
     * - 1.0이면 OFF 시간이 거의 없어져서
     *   부저 제어가 불안정할 수 있음
     */
    double maxVolume = 0.9;

    /*
     * 다이얼 값 0~100을 볼륨 0.0~0.9로 변환
     *
     * value / 100.0:
     * - value가 0이면 0.0
     * - value가 50이면 0.5
     * - value가 100이면 1.0
     */
    currentBuzzerVolume = minVolume + (value / 100.0) * (maxVolume - minVolume);

    /*
     * 디버깅용 출력
     */
    qDebug() << "Dial value:" << value;
    qDebug() << "Volume:" << currentBuzzerVolume;

    /*
     * 계산된 볼륨을 부저 스레드에 전달
     */
    SetBuzzerVolume(currentBuzzerVolume);
}

/*
 * 초음파 센서 거리값이 바뀌었을 때 실행
 *
 * UltrasonicThread에서 emit DistanceChanged(distanceCm)를 호출하면
 * 이 함수가 자동으로 호출됨
 */
void MainWindow::OnUltraSonicDistanceChanged(double distanceCm)
{
    /*
     * distanceCm가 0보다 작으면 측정 실패로 판단
     *
     * UltrasonicThread에서 측정 실패 시 -1.0을 보내도록 작성했음
     */
    if (distanceCm < 0.0)
    {
        /*
         * UI에 에러 표시
         */
        ui->UltraSonicLabel->setText("Distance: Error");
        return;
    }

    /*
     * 정상 거리값이면 UI에 표시
     *
     * QString("Distance:      %1 cm")
     * - %1 위치에 거리값을 넣겠다는 뜻
     *
     * arg(distanceCm, 0, 'f', 1)
     * - distanceCm 값을 소수점 1자리까지 표시
     *
     * 예:
     * - 12.345 -> 12.3
     * - 8.0    -> 8.0
     */
    ui->UltraSonicLabel->setText(
        QString("Distance:      %1 cm").arg(distanceCm, 0, 'f', 1)
        );

    /*
     * 디버깅용 출력
     */
    qDebug() << "Ultrasonic distance:" << distanceCm << "cm";
}
