#include "ultrasonicthread.h"

#include <QElapsedTimer>
#include <QDebug>
#include <gpiod.hpp>
#include <exception>

UltrasonicThread::UltrasonicThread(int trigPinNumber, int echoPinNumber, QObject *parent)
    : QThread(parent),
    trigPin(trigPinNumber),
    echoPin(echoPinNumber),
    running(true)
{
}

UltrasonicThread::~UltrasonicThread()
{
    StopThread();
}

void UltrasonicThread::StopThread()
{
    running.store(false);

    if (isRunning())
    {
        wait();
    }
}

void UltrasonicThread::run()
{
    try
    {
        gpiod::chip chip("/dev/gpiochip0");

        gpiod::line_settings trigSettings;
        trigSettings.set_direction(gpiod::line::direction::OUTPUT);
        trigSettings.set_output_value(gpiod::line::value::INACTIVE);

        gpiod::line_settings echoSettings;
        echoSettings.set_direction(gpiod::line::direction::INPUT);

        auto request = chip.prepare_request()
                           .set_consumer("qt-ultrasonic")
                           .add_line_settings(trigPin, trigSettings)
                           .add_line_settings(echoPin, echoSettings)
                           .do_request();

        while (running.load())
        {
            double sumDistance = 0.0;
            int validCount = 0;


            for (int i = 0; i < 5; i++)  // 0.5초동안 5번이 거리를 측정
            {
                if (!running.load())
                {
                    break;
                }

                request.set_value(trigPin, gpiod::line::value::INACTIVE);
                QThread::usleep(2);

                request.set_value(trigPin, gpiod::line::value::ACTIVE);
                QThread::usleep(10);

                request.set_value(trigPin, gpiod::line::value::INACTIVE);

                QElapsedTimer timer;
                timer.start();

                bool timeout = false;

                while (request.get_value(echoPin) == gpiod::line::value::INACTIVE)
                {
                    if (timer.nsecsElapsed() > 30000000) // 30ms
                    {
                        timeout = true;
                        break;
                    }
                }

                if (timeout)
                {
                    QThread::msleep(100);
                    continue;
                }

                qint64 echoStart = timer.nsecsElapsed();

                while (request.get_value(echoPin) == gpiod::line::value::ACTIVE)
                {
                    if (timer.nsecsElapsed() > 30000000) // 30ms
                    {
                        timeout = true;
                        break;
                    }
                }

                if (timeout)
                {
                    QThread::msleep(100);
                    continue;
                }

                qint64 echoEnd = timer.nsecsElapsed();

                double durationMicroSec = (echoEnd - echoStart) / 1000.0;

                double distanceCm = durationMicroSec / 58.0;

                sumDistance += distanceCm;
                validCount++;


                QThread::msleep(100);
            }

            if (validCount > 0)  // 유효한 측정값이 하나라도 있으면 평균 출력 실패하면 -1 출력
            {
                double avgDistance = sumDistance / validCount;
                emit DistanceChanged(avgDistance);
            }
            else
            {
                emit DistanceChanged(-1.0);
            }
        }

        request.set_value(trigPin, gpiod::line::value::INACTIVE);
    }
    catch (const std::exception &e)
    {
        qDebug() << "UltrasonicThread error:" << e.what();
    }
}
