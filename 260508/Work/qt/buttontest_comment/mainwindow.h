#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>

#include "buzzerthread.h"
#include "ultrasonicthread.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/*
 * LED_Color enum
 *
 * 현재 선택된 LED 색상을 표현하기 위한 자료형
 *
 * Red   = 0
 * Green = 1
 * Blue  = 2
 *
 * enum을 쓰면 숫자 0, 1, 2보다 코드 의미가 더 잘 보임
 */
enum LED_Color
{
    Red,
    Green,
    Blue
};

/*
 * MainWindow 클래스
 *
 * 역할:
 * - Qt UI 전체를 관리하는 메인 클래스
 * - 버튼 클릭
 * - LED 제어
 * - 부저 슬라이더/다이얼 제어
 * - 초음파 센서 거리값 화면 표시
 * 등을 담당함
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*
     * 생성자
     *
     * MainWindow가 만들어질 때 호출됨
     * 여기서 UI 초기화, 스레드 시작, 초기값 설정 등을 함
     */
    MainWindow(QWidget *parent = nullptr);

    /*
     * 소멸자
     *
     * 프로그램이 종료되거나 MainWindow가 사라질 때 호출됨
     * 여기서 스레드 종료, 메모리 정리 등을 함
     */
    ~MainWindow();

private slots:
    /*
     * pushButton을 눌렀을 때 자동으로 호출되는 슬롯
     *
     * Qt Designer에서 버튼 objectName이 pushButton이면
     * on_pushButton_clicked()라는 이름으로 자동 연결될 수 있음
     *
     * 현재 역할:
     * - LED ON
     */
    void on_pushButton_clicked();

    /*
     * pushButton_2를 눌렀을 때 호출되는 슬롯
     *
     * 현재 역할:
     * - LED OFF
     */
    void on_pushButton_2_clicked();

    /*
     * ColorChangeBtn을 눌렀을 때 호출되는 슬롯
     *
     * 현재 역할:
     * - LED가 켜져 있을 때 색상 변경
     */
    void on_ColorChangeBtn_clicked();

    /*
     * NoteSlider 값이 바뀔 때 호출되는 슬롯
     *
     * 현재 역할:
     * - 슬라이더 값에 따라 부저 주파수 변경
     */
    void on_NoteSlider_valueChanged(int value);

    /*
     * VolumeDial 값이 바뀔 때 호출되는 슬롯
     *
     * 현재 역할:
     * - 다이얼 값에 따라 부저 볼륨 변경
     */
    void on_VolumeDial_valueChanged(int value);

    /*
     * 초음파 센서에서 새로운 거리값이 들어왔을 때 호출되는 슬롯
     *
     * UltrasonicThread의 DistanceChanged 시그널과 연결됨
     */
    void OnUltraSonicDistanceChanged(double distanceCm);

private:
    /*
     * Qt Designer에서 만든 UI 객체
     *
     * ui->pushButton
     * ui->label_led
     * ui->VolumeDial
     * 이런 식으로 UI 요소에 접근할 수 있음
     */
    Ui::MainWindow *ui;

    /*
     * LED GPIO 핀 번호 목록
     *
     * 순서:
     * - Red   -> GPIO23
     * - Green -> GPIO18
     * - Blue  -> GPIO15
     *
     * currentLED enum 값을 int로 바꾸면
     * 이 vector의 index로 사용할 수 있음
     */
    std::vector<int> leds = {23, 18, 15};

    /*
     * 현재 선택된 LED 색상
     *
     * 처음에는 Red로 시작
     */
    LED_Color currentLED = Red;

    /*
     * 현재 LED가 켜져 있는지 여부
     *
     * true:
     * - LED 켜짐
     *
     * false:
     * - LED 꺼짐
     */
    bool IsLED_ON = false;

    /*
     * 현재 선택된 LED 켜기
     */
    void LED_On();

    /*
     * 현재 선택된 LED 끄기
     */
    void LED_Off();

    /*
     * LED 색상 변경 함수
     *
     * 지금 코드에서는 직접 사용하지 않지만,
     * 필요하면 색상 변경 로직을 따로 분리해서 사용할 수 있음
     */
    void LED_Color_Change();

    /*
     * 현재 LED 색상에 맞게 QLabel 스타일 변경
     *
     * 예:
     * - Red이면 label 배경 빨강
     * - Green이면 label 배경 초록
     * - Blue이면 label 배경 파랑
     */
    void SetLED_StyleSheet();

    /*
     * 부저 GPIO 핀 번호
     *
     * 부저가 GPIO24에 연결되어 있다고 가정
     */
    const int buzzerPin = 24;

    /*
     * 부저 제어 스레드 객체 포인터
     *
     * MainWindow에서 생성하고,
     * BuzzerThread가 실제 GPIO ON/OFF를 담당함
     */
    BuzzerThread *buzzerThread = nullptr;

    /*
     * 부저 주파수 설정 함수
     */
    void SetBuzzerFrequency(double frequency);

    /*
     * 부저 끄기 함수
     */
    void BuzzerOff();

    /*
     * 부저 볼륨 설정 함수
     */
    void SetBuzzerVolume(double volume);

    /*
     * 현재 부저 주파수
     *
     * 기본값:
     * - 261.63Hz
     * - 도 음계
     */
    double currentBuzzerFrequency = 261.63;

    /*
     * 현재 부저 볼륨
     *
     * 기본값:
     * - 0.0
     * - 처음에는 소리 안 남
     */
    double currentBuzzerVolume = 0.0;

    /*
     * 초음파 센서 TRIG 핀 번호
     */
    const int ultrasonicTrigPin = 7;

    /*
     * 초음파 센서 ECHO 핀 번호
     */
    const int ultrasonicEchoPin = 8;

    /*
     * 초음파 센서 거리 측정 스레드 객체 포인터
     */
    UltrasonicThread *ultrasonicThread = nullptr;
};

#endif // MAINWINDOW_H
