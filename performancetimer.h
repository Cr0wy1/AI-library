#ifndef PERFORMANCETIMER_H
#define PERFORMANCETIMER_H
#include <QDateTime>


class PerformanceTimer{
private:
    qint64 Startpoint;

    double difference = 0;

    unsigned int test_index = 0;
    qint64 test_last_point = 0;
    double test_highest_diff = 0;
    unsigned int test_highest_diff_index = 0;
    double test_lowest_diff = 0;


public:
    PerformanceTimer();

    void start();
    void stop();

    bool test(unsigned int runs);
    void Print();
};

#endif // PERFORMANCETIMER_H
