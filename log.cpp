#include "log.h"
#include <QDebug>
#include <matrix.h>
#include <dataframe.h>

Log::Log()
{

}

void Log::operator<<(int i){
    qDebug() << i;
}

template<class T>
void Log::operator <<(const Matrix<T> &matrix){
    //qDebug();
}

void Log::operator <<(const Dataframe &dataframe){
    qDebug();
}


