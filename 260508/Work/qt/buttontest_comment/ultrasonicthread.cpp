#include "ultrasonicthread.h"

#include <QElapsedTimer>
#include <QDebug>
#include <gpiod.hpp>
#include <exception>

/*
 * 생성자
 *
 * trigPin(trigPinNumber)
 * - 매개변수로 받은 TRIG GPIO 번호를 멤버변수에 저장
 *
 * echoPin(echoPinNumber)
 * - 매개변수로 받은 ECHO GPIO 번호를 멤버변수에 저장
 *
 * running(true)
 * - 스레드가 시작되면 while문이 돌 수 있도록 true로 설정
 */
UltrasonicThread::UltrasonicThread(int trigPinNumber, int echoPinNumber, QObject *parent)
    : QThread(parent),
    trigPin(trigPinNumber),
    echoPin(echoPinNumber),
    running(true)
{
}

/*
 * 소멸자
 *
 * UltrasonicThread 객체가 사라질 때
 * 거리 측정 스레드도 안전하게 종료함
 */
UltrasonicThread::~UltrasonicThread()
{
    StopThread();
}

/*
 * 스레드 종료 함수
 */
void UltrasonicThread::StopThread()
{
    /*
     * run() 함수 안의 while문을 멈추기 위해 running을 false로 변경
     */
    running.store(false);

    /*
     * 스레드가 아직 실행 중이면 완전히 끝날 때까지 기다림
     */
    if (isRunning())
    {
        wait();
    }
}

/*
 * 실제 거리 측정이 이루어지는 함수
 *
 * ultrasonicThread->start()를 호출하면
 * 이 함수가 별도 스레드에서 실행됨
 */
void UltrasonicThread::run()
{
    try
    {
        /*
         * Raspberry Pi GPIO chip 열기
         */
        gpiod::chip chip("/dev/gpiochip0");

        /*
         * TRIG 핀 설정
         *
         * TRIG 핀은 초음파 센서에게 신호를 보내는 핀이므로 OUTPUT
         */
        gpiod::line_settings trigSettings;
        trigSettings.set_direction(gpiod::line::direction::OUTPUT);

        /*
         * 처음에는 TRIG를 꺼진 상태로 시작
         */
        trigSettings.set_output_value(gpiod::line::value::INACTIVE);

        /*
         * ECHO 핀 설정
         *
         * ECHO 핀은 초음파가 돌아왔는지 읽는 핀이므로 INPUT
         */
        gpiod::line_settings echoSettings;
        echoSettings.set_direction(gpiod::line::direction::INPUT);

        /*
         * TRIG 핀과 ECHO 핀을 동시에 요청
         *
         * 하나의 request 객체로 두 핀을 관리함
         */
        auto request = chip.prepare_request()
                           .set_consumer("qt-ultrasonic")
                           .add_line_settings(trigPin, trigSettings)
                           .add_line_settings(echoPin, echoSettings)
                           .do_request();

        /*
         * running이 true인 동안 계속 거리 측정
         */
        while (running.load())
        {
            /*
             * 5번 측정한 거리 값을 더할 변수
             */
            double sumDistance = 0.0;

            /*
             * 정상적으로 측정된 횟수
             *
             * 초음파 측정은 가끔 실패할 수 있음
             * 실패한 값은 평균에 포함하지 않기 위해 validCount를 따로 사용함
             */
            int validCount = 0;

            /*
             * 0.5초 동안 총 5번 측정
             *
             * 아래에서 한 번 측정 후 100ms 쉬기 때문에
             * 5번 반복하면 대략 0.5초마다 평균값이 갱신됨
             */
            for (int i = 0; i < 5; i++)
            {
                /*
                 * 중간에 프로그램 종료 요청이 들어오면
                 * for문을 빠져나감
                 */
                if (!running.load())
                {
                    break;
                }

                /*
                 * 초음파 센서 거리 측정 순서
                 *
                 * 1. TRIG를 잠깐 LOW로 둠
                 * 2. TRIG를 10마이크로초 정도 HIGH로 만듦
                 * 3. 다시 TRIG를 LOW로 만듦
                 * 4. ECHO 핀이 HIGH가 되는 순간을 기다림
                 * 5. ECHO 핀이 다시 LOW가 되는 순간까지 시간을 잼
                 * 6. 그 시간으로 거리를 계산함
                 */

                /*
                 * TRIG를 잠깐 LOW 상태로 둠
                 * 센서가 안정적으로 시작할 수 있게 하기 위함
                 */
                request.set_value(trigPin, gpiod::line::value::INACTIVE);
                QThread::usleep(2);

                /*
                 * TRIG를 HIGH로 변경
                 * 초음파 센서에게 초음파를 쏘라는 신호
                 */
                request.set_value(trigPin, gpiod::line::value::ACTIVE);

                /*
                 * HC-SR04 같은 초음파 센서는 보통 10us 이상의 TRIG 신호가 필요함
                 */
                QThread::usleep(10);

                /*
                 * 다시 TRIG를 LOW로 변경
                 */
                request.set_value(trigPin, gpiod::line::value::INACTIVE);

                /*
                 * 시간 측정을 위한 타이머
                 *
                 * ECHO 핀이 HIGH로 바뀌고 다시 LOW로 바뀌는 시간을 측정할 때 사용
                 */
                QElapsedTimer timer;
                timer.start();

                /*
                 * timeout 변수
                 *
                 * 센서가 응답하지 않으면 while문이 무한히 기다릴 수 있음
                 * 그래서 일정 시간이 지나면 실패 처리하기 위해 사용함
                 */
                bool timeout = false;

                /*
                 * ECHO 핀이 HIGH가 될 때까지 기다림
                 *
                 * ECHO가 HIGH가 된다는 뜻:
                 * - 초음파 발사가 시작되었고
                 * - 반사파를 기다리는 상태가 되었다는 의미
                 */
                while (request.get_value(echoPin) == gpiod::line::value::INACTIVE)
                {
                    /*
                     * 30ms 이상 기다려도 ECHO가 HIGH가 안 되면 실패 처리
                     *
                     * 30ms는 꽤 긴 시간이고,
                     * 이 안에 응답이 없으면 측정 실패로 보는 것
                     */
                    if (timer.nsecsElapsed() > 30000000)
                    {
                        timeout = true;
                        break;
                    }
                }

                /*
                 * ECHO가 HIGH가 되지 않았다면 이번 측정은 실패
                 */
                if (timeout)
                {
                    /*
                     * 바로 다음 측정을 하지 않고 잠깐 쉬기
                     */
                    QThread::msleep(100);
                    continue;
                }

                /*
                 * ECHO가 HIGH가 된 시점 저장
                 *
                 * nsecsElapsed()는 타이머 시작 후 지난 시간을 나노초 단위로 반환
                 */
                qint64 echoStart = timer.nsecsElapsed();

                /*
                 * ECHO 핀이 다시 LOW가 될 때까지 기다림
                 *
                 * ECHO가 HIGH로 유지된 시간은
                 * 초음파가 물체까지 갔다가 돌아온 왕복 시간임
                 */
                while (request.get_value(echoPin) == gpiod::line::value::ACTIVE)
                {
                    /*
                     * ECHO가 너무 오래 HIGH 상태면 실패 처리
                     */
                    if (timer.nsecsElapsed() > 30000000)
                    {
                        timeout = true;
                        break;
                    }
                }

                /*
                 * ECHO가 다시 LOW로 내려오지 않았다면 이번 측정은 실패
                 */
                if (timeout)
                {
                    QThread::msleep(100);
                    continue;
                }

                /*
                 * ECHO가 LOW로 내려온 시점 저장
                 */
                qint64 echoEnd = timer.nsecsElapsed();

                /*
                 * echoEnd - echoStart:
                 * - ECHO 핀이 HIGH로 유지된 시간
                 * - 단위는 나노초
                 *
                 * / 1000.0:
                 * - 나노초를 마이크로초로 변환
                 */
                double durationMicroSec = (echoEnd - echoStart) / 1000.0;

                /*
                 * 거리 계산
                 *
                 * 초음파 센서에서 자주 쓰는 공식:
                 *
                 * 거리(cm) = ECHO 시간(us) / 58
                 *
                 * 이유:
                 * - 초음파가 물체까지 갔다가 다시 돌아오기 때문에 왕복 시간임
                 * - 소리 속도를 기준으로 계산하면 대략 /58 공식이 나옴
                 */
                double distanceCm = durationMicroSec / 58.0;

                /*
                 * 정상 측정된 거리 값을 합산
                 */
                sumDistance += distanceCm;

                /*
                 * 정상 측정 횟수 증가
                 */
                validCount++;

                /*
                 * 다음 측정까지 100ms 대기
                 *
                 * 5번 반복하면 대략 500ms,
                 * 즉 0.5초마다 평균 거리 갱신
                 */
                QThread::msleep(100);
            }

            /*
             * 정상 측정값이 하나라도 있으면 평균 계산
             */
            if (validCount > 0)
            {
                /*
                 * 평균 거리 계산
                 */
                double avgDistance = sumDistance / validCount;

                /*
                 * MainWindow로 평균 거리 전달
                 *
                 * emit은 Qt 시그널을 발생시키는 키워드
                 */
                emit DistanceChanged(avgDistance);
            }
            else
            {
                /*
                 * 5번 모두 실패한 경우
                 * MainWindow에 -1.0을 보내서 에러 표시하게 함
                 */
                emit DistanceChanged(-1.0);
            }
        }

        /*
         * 스레드 종료 전에 TRIG 핀을 확실히 LOW 상태로 만듦
         */
        request.set_value(trigPin, gpiod::line::value::INACTIVE);
    }
    catch (const std::exception &e)
    {
        /*
         * GPIO 관련 예외 발생 시 출력
         */
        qDebug() << "UltrasonicThread error:" << e.what();
    }
}
