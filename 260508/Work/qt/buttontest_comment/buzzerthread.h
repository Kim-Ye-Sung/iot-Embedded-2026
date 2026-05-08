#ifndef BUZZERTHREAD_H
#define BUZZERTHREAD_H

#include <QThread>
#include <atomic>

/*
 * BuzzerThread 클래스
 *
 * 역할:
 * - 부저를 메인 UI 스레드와 분리해서 별도 스레드에서 계속 제어하는 클래스
 *
 * 왜 스레드를 쓰는가?
 * - 부저 소리를 내려면 GPIO를 아주 빠르게 ON/OFF 해야 함
 * - 이 작업을 MainWindow에서 직접 하면 UI가 멈추거나 버벅일 수 있음
 * - 그래서 부저 제어만 담당하는 QThread를 따로 만들어서 사용함
 *
 * 동작 방식:
 * - currentFrequency 값에 따라 ON/OFF 주기를 계산함
 * - currentVolume 값에 따라 ON 시간과 OFF 시간을 나눔
 * - run() 함수 안에서 while문을 돌면서 계속 부저를 ON/OFF 반복함
 */
class BuzzerThread : public QThread
{
public:
    /*
     * 생성자
     *
     * pinNumber:
     * - 부저가 연결된 GPIO 핀 번호
     *
     * parent:
     * - Qt 객체 소유 관계를 위한 부모 객체
     * - 보통 MainWindow에서 this를 넘김
     */
    BuzzerThread(int pinNumber, QObject *parent = nullptr);

    /*
     * 소멸자
     *
     * 객체가 사라질 때 스레드가 아직 돌고 있으면 안전하게 멈춤
     */
    ~BuzzerThread();

    /*
     * 부저 주파수 설정 함수
     *
     * frequency:
     * - 부저에서 낼 소리의 주파수
     * - 예: 261.63Hz는 도(C4), 523.25Hz는 높은 도(C5)
     */
    void SetFrequency(double frequency);

    /*
     * 부저 볼륨 설정 함수
     *
     * volume:
     * - 0.0 ~ 1.0 사이 값
     * - 0.0이면 소리 없음
     * - 0.5이면 중간 정도
     * - 1.0이면 거의 계속 ON 상태
     */
    void SetVolume(double volume);

    /*
     * 부저 소리만 끄는 함수
     *
     * 스레드는 계속 살아있지만,
     * 주파수를 0으로 만들어서 부저가 울리지 않게 함
     */
    void StopBuzzer();

    /*
     * 스레드 자체를 멈추는 함수
     *
     * 프로그램 종료 시 반드시 호출해서
     * while문을 멈추고 스레드를 안전하게 종료함
     */
    void StopThread();

protected:
    /*
     * QThread에서 실제로 실행되는 함수
     *
     * start()를 호출하면 Qt가 내부적으로 run()을 실행함
     * 직접 run()을 호출하는 것이 아니라 start()를 호출해야 함
     */
    void run() override;

private:
    /*
     * 부저가 연결된 GPIO 핀 번호
     *
     * 예:
     * - GPIO24에 부저가 연결되어 있으면 buzzerPin = 24
     */
    int buzzerPin;

    /*
     * 현재 부저 주파수
     *
     * std::atomic을 쓰는 이유:
     * - MainWindow 스레드에서 값을 바꾸고
     * - BuzzerThread 스레드에서 값을 읽기 때문
     * - 여러 스레드가 동시에 접근해도 안전하게 하기 위해 atomic 사용
     */
    std::atomic<double> currentFrequency;

    /*
     * 현재 부저 볼륨
     *
     * 0.0 ~ 1.0 사이 값
     */
    std::atomic<double> currentVolume;

    /*
     * 스레드 실행 여부
     *
     * true이면 run()의 while문이 계속 돎
     * false가 되면 while문이 끝나고 스레드 종료
     */
    std::atomic<bool> running;
};

#endif // BUZZERTHREAD_H
