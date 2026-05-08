#ifndef ULTRASONICTHREAD_H
#define ULTRASONICTHREAD_H

#include <QThread>
#include <atomic>

class UltrasonicThread : public QThread
{
    Q_OBJECT

public:
    UltrasonicThread(int trigPinNumber, int echoPinNumber, QObject *parent = nullptr);
    ~UltrasonicThread();

    void StopThread();

signals:
    void DistanceChanged(double distanceCm);

protected:
    void run() override;

private:
    int trigPin;
    int echoPin;

    std::atomic<bool> running;

    double MeasureDistanceCm();
};

#endif // ULTRASONICTHREAD_H
