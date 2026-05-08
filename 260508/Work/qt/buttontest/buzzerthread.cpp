#include "buzzerthread.h"

#include <QDebug>
#include <gpiod.hpp>
#include <exception>

BuzzerThread::BuzzerThread(int pinNumber, QObject *parent)
    : QThread(parent),
    buzzerPin(pinNumber),
    currentFrequency(0.0),
    currentVolume(0.0),
    running(true)
{
}

BuzzerThread::~BuzzerThread()
{
    StopThread();
}

void BuzzerThread::SetFrequency(double frequency)
{
    currentFrequency.store(frequency);
}

void BuzzerThread::SetVolume(double volume)
{
    if (volume < 0.0)
    {
        volume = 0.0;
    }

    if (volume > 1.0)
    {
        volume = 1.0;
    }

    currentVolume.store(volume);
}

void BuzzerThread::StopBuzzer()
{
    currentFrequency.store(0.0);
}

void BuzzerThread::StopThread()
{
    running.store(false);

    if (isRunning())
    {
        wait();
    }
}

void BuzzerThread::run()
{
    try
    {
        gpiod::chip chip("/dev/gpiochip0");

        gpiod::line_settings settings;

        settings.set_direction(gpiod::line::direction::OUTPUT);
        settings.set_output_value(gpiod::line::value::INACTIVE);

        auto request = chip.prepare_request()
                           .set_consumer("qt-buzzer")
                           .add_line_settings(buzzerPin, settings)
                           .do_request();

        while (running.load())
        {
            double frequency = currentFrequency.load();
            double volume = currentVolume.load();

            if (frequency <= 0.0 || volume <= 0.0)
            {
                request.set_value(buzzerPin, gpiod::line::value::INACTIVE);
                QThread::msleep(10);
                continue;
            }

            double periodMicroSec = 1000000.0 / frequency;

            unsigned long onTime = static_cast<unsigned long>(periodMicroSec * volume);
            unsigned long offTime = static_cast<unsigned long>(periodMicroSec * (1.0 - volume));

            if (onTime < 100)
            {
                onTime = 100;
            }

            if (offTime < 100)
            {
                offTime = 100;
            }

            request.set_value(buzzerPin, gpiod::line::value::ACTIVE);
            QThread::usleep(onTime);

            request.set_value(buzzerPin, gpiod::line::value::INACTIVE);
            QThread::usleep(offTime);
        }

        request.set_value(buzzerPin, gpiod::line::value::INACTIVE);
    }
    catch (const std::exception &e)
    {
        qDebug() << "BuzzerThread error:" << e.what();
    }
}
