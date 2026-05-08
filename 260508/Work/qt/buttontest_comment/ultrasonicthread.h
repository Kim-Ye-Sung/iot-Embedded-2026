#ifndef ULTRASONICTHREAD_H
#define ULTRASONICTHREAD_H

#include <QThread>
#include <atomic>

/*
 * UltrasonicThread 클래스
 *
 * 역할:
 * - 초음파 센서 거리 측정을 MainWindow와 분리해서 별도 스레드에서 실행함
 *
 * 왜 스레드를 쓰는가?
 * - 초음파 센서는 거리 측정 시 while문으로 echo 핀 상태를 기다려야 함
 * - 이걸 MainWindow에서 직접 하면 UI가 멈출 수 있음
 * - 그래서 거리 측정을 별도 스레드에서 수행함
 *
 * 현재 동작 방식:
 * - 0.5초 동안 총 5번 거리 측정
 * - 5번 측정한 값 중 성공한 값들의 평균을 계산
 * - 평균 거리를 DistanceChanged 시그널로 MainWindow에 전달
 */
class UltrasonicThread : public QThread
{
    Q_OBJECT

public:
    /*
     * 생성자
     *
     * trigPinNumber:
     * - 초음파 센서 TRIG 핀이 연결된 GPIO 번호
     *
     * echoPinNumber:
     * - 초음파 센서 ECHO 핀이 연결된 GPIO 번호
     *
     * parent:
     * - Qt 부모 객체
     */
    UltrasonicThread(int trigPinNumber, int echoPinNumber, QObject *parent = nullptr);

    /*
     * 소멸자
     *
     * 객체가 삭제될 때 스레드를 안전하게 종료함
     */
    ~UltrasonicThread();

    /*
     * 스레드 종료 함수
     *
     * running을 false로 바꾸고,
     * 실행 중인 스레드가 끝날 때까지 기다림
     */
    void StopThread();

signals:
    /*
     * 거리 값이 새로 계산되었을 때 MainWindow로 보내는 시그널
     *
     * distanceCm:
     * - cm 단위 거리
     * - 정상 측정 시 양수 값
     * - 측정 실패 시 -1.0 전달
     */
    void DistanceChanged(double distanceCm);

protected:
    /*
     * QThread에서 실제로 실행되는 함수
     *
     * ultrasonicThread->start()를 호출하면
     * Qt가 자동으로 이 run() 함수를 별도 스레드에서 실행함
     */
    void run() override;

private:
    /*
     * 초음파 센서 TRIG 핀 번호
     *
     * TRIG:
     * - 초음파 센서에게 "초음파를 쏴라"라고 신호를 보내는 핀
     */
    int trigPin;

    /*
     * 초음파 센서 ECHO 핀 번호
     *
     * ECHO:
     * - 초음파가 물체에 반사되어 돌아오는 시간을 알려주는 핀
     */
    int echoPin;

    /*
     * 스레드 실행 여부
     *
     * true이면 계속 거리 측정
     * false이면 while문 종료
     */
    std::atomic<bool> running;
};

#endif // ULTRASONICTHREAD_H
