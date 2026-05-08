#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QProcess>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    buzzerThread = new BuzzerThread(buzzerPin, this);
    buzzerThread->start();

    ultrasonicThread = new UltrasonicThread(ultrasonicTrigPin, ultrasonicEchoPin, this);

    connect(
        ultrasonicThread,
        &UltrasonicThread::DistanceChanged,
        this,
        &MainWindow::OnUltraSonicDistanceChanged
        );

    ultrasonicThread->start();

    currentBuzzerFrequency = 261.63;
    currentBuzzerVolume = 0.0;

    ui->VolumeDial->setMinimum(0);
    ui->VolumeDial->setMaximum(100);
    ui->VolumeDial->setValue(0);

    SetBuzzerFrequency(currentBuzzerFrequency);
    SetBuzzerVolume(currentBuzzerVolume);

    for(int i = 0; i < leds.size(); i++)
    {
        LED_Off();

        currentLED = static_cast<LED_Color>((static_cast<int>(currentLED) + 1) % leds.size());
    }
}

MainWindow::~MainWindow()
{
    BuzzerOff();

    if (buzzerThread != nullptr)
    {
        buzzerThread->StopThread();
        buzzerThread = nullptr;
    }

    if (ultrasonicThread != nullptr)
    {
        ultrasonicThread->StopThread();
        ultrasonicThread = nullptr;
    }

    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    SetLED_StyleSheet();

    ui->label_led->setText("On");

    LED_On();
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->label_led->setStyleSheet("background-color:gray;border-radius:20px;");

    ui->label_led->setText("Off");

    LED_Off();
}

void MainWindow::on_ColorChangeBtn_clicked()
{
    if(IsLED_ON)
    {
        LED_Off();

        currentLED = static_cast<LED_Color>((static_cast<int>(currentLED) + 1) % leds.size());

        SetLED_StyleSheet();

        LED_On();
    }
}

void MainWindow::LED_On()
{
    IsLED_ON = true;

    QProcess::execute("pkill", QStringList() << "gpioset");

    QProcess::startDetached(
        "gpioset",
        QStringList()
            << "-c"
            << "/dev/gpiochip0"
            << QString::number(leds[static_cast<int>(currentLED)]) + "=0"
        );
}

void MainWindow::LED_Off()
{
    IsLED_ON = false;

    QProcess::execute("pkill", QStringList() << "gpioset");

    QProcess::startDetached(
        "gpioset",
        QStringList()
            << "-c"
            << "/dev/gpiochip0"
            << QString::number(leds[static_cast<int>(currentLED)]) + "=1"
        );
}

void MainWindow::SetLED_StyleSheet()
{
    switch(currentLED)
    {
    case Red:
        ui->label_led->setStyleSheet("background-color:red;border-radius:20px;");
        break;

    case Green:
        ui->label_led->setStyleSheet("background-color:green;border-radius:20px;");
        break;

    case Blue:
        ui->label_led->setStyleSheet("background-color:blue;border-radius:20px;");
        break;
    }
}

void MainWindow::on_NoteSlider_valueChanged(int value)
{
    double minFreq = 261.63;
    double maxFreq = 523.25;

    int sliderMin = ui->NoteSlider->minimum();
    int sliderMax = ui->NoteSlider->maximum();

    double ratio = double(value - sliderMin) / double(sliderMax - sliderMin);

    currentBuzzerFrequency = minFreq + ratio * (maxFreq - minFreq);

    qDebug() << "Slider value:" << value;
    qDebug() << "Frequency:" << currentBuzzerFrequency;

    SetBuzzerFrequency(currentBuzzerFrequency);
}

void MainWindow::SetBuzzerFrequency(double frequency)
{
    if (buzzerThread == nullptr)
    {
        return;
    }

    buzzerThread->SetFrequency(frequency);

    qDebug() << "Buzzer frequency:" << frequency;
}

void MainWindow::BuzzerOff()
{
    if (buzzerThread == nullptr)
    {
        return;
    }

    currentBuzzerVolume = 0.0;
    buzzerThread->StopBuzzer();
}

void MainWindow::SetBuzzerVolume(double volume)
{
    if (buzzerThread == nullptr)
    {
        return;
    }

    buzzerThread->SetVolume(volume);

    qDebug() << "Buzzer volume:" << volume;
}

void MainWindow::on_VolumeDial_valueChanged(int value)
{
    double minVolume = 0.0;
    double maxVolume = 0.9;

    currentBuzzerVolume = minVolume + (value / 100.0) * (maxVolume - minVolume);

    qDebug() << "Dial value:" << value;
    qDebug() << "Volume:" << currentBuzzerVolume;

    SetBuzzerVolume(currentBuzzerVolume);
}

void MainWindow::OnUltraSonicDistanceChanged(double distanceCm)
{
    if (distanceCm < 0.0)
    {
        ui->UltraSonicLabel->setText("Distance: Error");
        return;
    }

    ui->UltraSonicLabel->setText(
        QString("Distance:      %1 cm").arg(distanceCm, 0, 'f', 1)
        );

    qDebug() << "Ultrasonic distance:" << distanceCm << "cm";
}
