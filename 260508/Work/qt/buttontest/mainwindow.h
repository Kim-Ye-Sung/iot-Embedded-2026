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

enum LED_Color
{
    Red,
    Green,
    Blue
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_ColorChangeBtn_clicked();

    void on_NoteSlider_valueChanged(int value);

    void on_VolumeDial_valueChanged(int value);

    void OnUltraSonicDistanceChanged(double distanceCm);

private:
    Ui::MainWindow *ui;

    std::vector<int> leds = {23, 18, 15};  // led가 연결된 gpio핀 번호

    LED_Color currentLED = Red;

    bool IsLED_ON = false;

    void LED_On();

    void LED_Off();

    void LED_Color_Change();

    void SetLED_StyleSheet();

    const int buzzerPin = 24;       // buzzer가 연결된 gpio핀 번호

    BuzzerThread *buzzerThread = nullptr;

    void SetBuzzerFrequency(double frequency);
    void BuzzerOff();
    void SetBuzzerVolume(double volume);

    double currentBuzzerFrequency = 261.63;
    double currentBuzzerVolume = 0.0;

    const int ultrasonicTrigPin = 7;   // 초음파 센서 trig가 연결된 gpio 핀 번호
    const int ultrasonicEchoPin = 8;   // 초음파 센서 echo가 연결된 gpio 핀 번호

    UltrasonicThread *ultrasonicThread = nullptr;
};

#endif // MAINWINDOW_H
