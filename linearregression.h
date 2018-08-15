#ifndef LINEARREGRESSION_H
#define LINEARREGRESSION_H

#include <matrix.h>
#include <dataframe.h>
#include <QDebug>

template<class T>
class Matrix;

template<class T>
class Dataframe;


class LinearRegression{

public:
    Matrix<double> B = Matrix<double>();
    Matrix<double> X = Matrix<double>();
    Matrix<double> Y = Matrix<double>();


public:
    LinearRegression();

    template<class T>
    void fit(Dataframe<T> &dataframe, std::string Y_value){
        qDebug() << "Fit Dataframe: ";
        int y_col_i = dataframe.GetColIndex(Y_value);
        if(y_col_i < 0 ){
            qDebug() << "LR fit: Y_name not exist in dataframe";
            return;
        }
        X = dataframe;
        Y = X.PullCol(y_col_i);

        __fit();
    }

    template<class T>
    void fit(Matrix<T> &_X, Matrix<T> &_Y) {
        qDebug() << "Fit Matrix";
        X = _X;
        Y = _Y;

        __fit();
    }

    Dataframe<double> predict(Dataframe<> &y_df);

    Matrix<double> predict();
    double GetR2Score(Dataframe<> &df_y, Dataframe<> &df_y_pre);
    Matrix<double> GetB();
    Matrix<double> GetX();
    Matrix<double> GetY();

protected:


private:
    void __fit(){
        X.AddCol(0, 1);
        qDebug() << X[5][0];
        qDebug() << X[5][1];
        qDebug() << X[5][2];
        Matrix<double> XX = X.transpose() * X;
        Matrix<double> XY = X.transpose() * Y;
        B = Matrix<double>(XX.inverse() * XY);
    }

};
#endif // LINEARREGRESSION_H
