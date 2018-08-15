#include "performancetimer.h"
#include <QDateTime>
#include <qdebug.h>

PerformanceTimer::PerformanceTimer(){

}

void PerformanceTimer::start(){
    Startpoint = QDateTime::currentMSecsSinceEpoch();
}

void PerformanceTimer::stop(){
    qint64 Endpoint = QDateTime::currentMSecsSinceEpoch();
    difference = Endpoint - Startpoint;
    Print();
}

bool PerformanceTimer::test(unsigned int runs){
    if(test_index == 0) start();
    else{
        double test_diff = QDateTime::currentMSecsSinceEpoch() - test_last_point;
        if(test_highest_diff < test_diff ){
            test_highest_diff = test_diff;
            test_highest_diff_index = test_index;
        }
    }
    test_last_point = QDateTime::currentMSecsSinceEpoch();


    if(test_index >= runs - 1){
        stop();
        qDebug() << "Runs:" << test_index + 1 << "Average:" << difference / (test_index + 1) << "ms" << "Longest Run" << test_highest_diff_index << ":" <<test_highest_diff << "ms";
        test_index = 0;
        return false;
    }

    test_index++;
    return true;
}

void PerformanceTimer::Print(){
    unsigned int sec = difference / 1000;
    unsigned int min = sec / 60;
    unsigned int hour = min / 60;
    unsigned int day = hour / 24;
    qDebug() << "Total Time: " << day << "d " << hour << "h " << min << "m " << sec << "s " << fmod(difference, 1000)  << "ms ";
}
