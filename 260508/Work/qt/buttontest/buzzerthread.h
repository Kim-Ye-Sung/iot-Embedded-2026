#ifndef BUZZERTHREAD_H
#define BUZZERTHREAD_H

#include <QThread>
#include <atomic>

// 부저를 별도 스레드에서 계속 ON/OFF 반복시키는 클래스
class BuzzerThread : public QThread
{
public:
    BuzzerThread(int pinNumber, QObject *parent = nullptr);
    ~BuzzerThread();

    void SetFrequency(double frequency);
    void SetVolume(double volume);
    void StopBuzzer();
    void StopThread();

protected:
    void run() override;

private:
    int buzzerPin;

    std::atomic<double> currentFrequency;
    std::atomic<double> currentVolume;
    std::atomic<bool> running;
};

#endif // BUZZERTHREAD_H
