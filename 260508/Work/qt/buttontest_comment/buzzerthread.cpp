#include "buzzerthread.h"

#include <QDebug>
#include <gpiod.hpp>
#include <exception>

/*
 * 생성자
 *
 * : QThread(parent)
 * - 부모 객체를 QThread에 전달
 *
 * buzzerPin(pinNumber)
 * - 매개변수로 받은 GPIO 핀 번호를 멤버변수 buzzerPin에 저장
 *
 * currentFrequency(0.0)
 * - 처음에는 주파수를 0으로 설정
 * - 즉, 처음에는 소리가 안 나도록 함
 *
 * currentVolume(0.0)
 * - 처음 볼륨도 0
 *
 * running(true)
 * - 스레드가 시작되면 while문이 돌 수 있도록 true로 설정
 */
BuzzerThread::BuzzerThread(int pinNumber, QObject *parent)
    : QThread(parent),
    buzzerPin(pinNumber),
    currentFrequency(0.0),
    currentVolume(0.0),
    running(true)
{
}

/*
 * 소멸자
 *
 * BuzzerThread 객체가 삭제될 때 자동으로 호출됨
 * 스레드가 아직 돌고 있을 수도 있으므로 StopThread()로 안전하게 종료함
 */
BuzzerThread::~BuzzerThread()
{
    StopThread();
}

/*
 * 부저 주파수 설정 함수
 *
 * MainWindow에서 슬라이더 값이 바뀌면 이 함수가 호출됨
 * currentFrequency에 새 주파수 값을 저장함
 */
void BuzzerThread::SetFrequency(double frequency)
{
    currentFrequency.store(frequency);
}

/*
 * 부저 볼륨 설정 함수
 *
 * volume 값이 0.0보다 작거나 1.0보다 크면 문제가 생길 수 있음
 * 그래서 범위를 강제로 0.0 ~ 1.0 사이로 제한함
 */
void BuzzerThread::SetVolume(double volume)
{
    /*
     * 볼륨이 0보다 작으면 0으로 고정
     */
    if (volume < 0.0)
    {
        volume = 0.0;
    }

    /*
     * 볼륨이 1보다 크면 1로 고정
     */
    if (volume > 1.0)
    {
        volume = 1.0;
    }

    /*
     * 안전하게 보정된 볼륨 값을 atomic 변수에 저장
     */
    currentVolume.store(volume);
}

/*
 * 부저 소리 끄기
 *
 * currentFrequency를 0으로 만들면 run() 함수 안에서
 * frequency <= 0.0 조건에 걸려 부저가 꺼짐
 */
void BuzzerThread::StopBuzzer()
{
    currentFrequency.store(0.0);
}


/*
 * 부저 스레드 종료
 *
 * running을 false로 바꾸면 run() 함수의 while문이 끝남
 */
void BuzzerThread::StopThread()
{
    /*
     * while문 종료 요청
     */
    running.store(false);

    /*
     * 현재 스레드가 실행 중이라면 wait()로 완전히 끝날 때까지 기다림
     *
     * wait()를 하지 않으면:
     * - 스레드가 아직 GPIO를 사용 중인데 객체가 사라질 수 있음
     * - 프로그램 종료 시 오류가 날 수 있음
     */
    if (isRunning())
    {
        wait();
    }
}

/*
 * 실제 부저 제어가 이루어지는 함수
 *
 * start()가 호출되면 이 run() 함수가 별도 스레드에서 실행됨
 */
void BuzzerThread::run()
{
    try
    {
        /*
         * Raspberry Pi의 GPIO chip 열기
         *
         * 보통 Raspberry Pi에서는 /dev/gpiochip0을 사용함
         */
        gpiod::chip chip("/dev/gpiochip0");

        /*
         * GPIO 핀 설정 객체 생성
         *
         * line_settings는 해당 GPIO 핀을 입력으로 쓸지,
         * 출력으로 쓸지 등을 설정하는 객체
         */
        gpiod::line_settings settings;

        /*
         * 부저는 GPIO에서 신호를 내보내야 하므로 OUTPUT으로 설정
         */
        settings.set_direction(gpiod::line::direction::OUTPUT);

        /*
         * 처음 상태는 INACTIVE
         *
         * 즉, 처음에는 부저가 꺼진 상태로 시작
         */
        settings.set_output_value(gpiod::line::value::INACTIVE);

        /*
         * 실제 GPIO 핀 사용 요청
         *
         * set_consumer("qt-buzzer")
         * - 이 GPIO를 누가 사용 중인지 이름을 붙이는 것
         *
         * add_line_settings(buzzerPin, settings)
         * - buzzerPin 번호의 GPIO를 위에서 만든 설정으로 사용하겠다는 뜻
         *
         * do_request()
         * - 실제로 GPIO 사용 권한을 요청함
         */
        auto request = chip.prepare_request()
                           .set_consumer("qt-buzzer")
                           .add_line_settings(buzzerPin, settings)
                           .do_request();

        /*
         * running이 true인 동안 계속 반복
         *
         * 이 while문 안에서 부저를 계속 ON/OFF 시켜서 소리를 냄
         */
        while (running.load())
        {
            /*
             * 현재 주파수와 볼륨 값을 가져옴
             *
             * MainWindow에서 슬라이더나 다이얼을 움직이면
             * 이 값들이 바뀔 수 있음
             */
            double frequency = currentFrequency.load();
            double volume = currentVolume.load();

            /*
             * 주파수가 0 이하이거나 볼륨이 0 이하이면
             * 부저를 꺼야 함
             */
            if (frequency <= 0.0 || volume <= 0.0)
            {
                /*
                 * GPIO를 INACTIVE로 만들어 부저 OFF
                 */
                request.set_value(buzzerPin, gpiod::line::value::INACTIVE);

                /*
                 * 너무 빠르게 while문이 돌면 CPU 사용량이 커짐
                 * 그래서 10ms 정도 잠깐 쉬게 함
                 */
                QThread::msleep(10);

                /*
                 * 아래의 ON/OFF 계산은 하지 않고
                 * while문의 처음으로 돌아감
                 */
                continue;
            }

            /*
             * 주기 계산
             *
             * 주파수 Hz는 1초에 몇 번 진동하는지를 의미함
             *
             * 예:
             * - 100Hz는 1초에 100번
             * - 한 번의 주기는 1 / 100초 = 0.01초
             *
             * 여기서는 마이크로초 단위로 계산함
             * 1초 = 1,000,000 마이크로초
             */
            double periodMicroSec = 1000000.0 / frequency;

            /*
             * ON 시간 계산
             *
             * volume이 0.5이면 전체 주기의 절반 동안 ON
             */
            unsigned long onTime = static_cast<unsigned long>(periodMicroSec * volume);

            /*
             * OFF 시간 계산
             *
             * volume이 0.5이면 전체 주기의 나머지 절반 동안 OFF
             */
            unsigned long offTime = static_cast<unsigned long>(periodMicroSec * (1.0 - volume));

            /*
             * ON 시간이 너무 짧으면 GPIO 제어가 불안정할 수 있음
             * 최소 100마이크로초는 유지하도록 보정
             */
            if (onTime < 100)
            {
                onTime = 100;
            }

            /*
             * OFF 시간도 너무 짧으면 불안정할 수 있음
             * 최소 100마이크로초는 유지하도록 보정
             */
            if (offTime < 100)
            {
                offTime = 100;
            }

            /*
             * 부저 ON
             */
            request.set_value(buzzerPin, gpiod::line::value::ACTIVE);

            /*
             * onTime 마이크로초 동안 ON 상태 유지
             */
            QThread::usleep(onTime);

            /*
             * 부저 OFF
             */
            request.set_value(buzzerPin, gpiod::line::value::INACTIVE);

            /*
             * offTime 마이크로초 동안 OFF 상태 유지
             */
            QThread::usleep(offTime);
        }

        /*
         * while문이 끝났다는 것은 스레드 종료 요청이 들어왔다는 뜻
         * 종료 전에 부저를 반드시 꺼줌
         */
        request.set_value(buzzerPin, gpiod::line::value::INACTIVE);
    }
    catch (const std::exception &e)
    {
        /*
         * GPIO 사용 중 예외가 발생하면 qDebug로 출력
         *
         * 예:
         * - GPIO 권한 문제
         * - 핀 사용 중
         * - /dev/gpiochip0 접근 실패
         */
        qDebug() << "BuzzerThread error:" << e.what();
    }
}
