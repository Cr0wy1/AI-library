#include "linearregression.h"
#include <QDebug>
#include <matrix.h>

LinearRegression::LinearRegression()
{

}

Matrix<double> LinearRegression::predict(){
    Matrix<double> result = X*B;
    return result;
}

Matrix<double> LinearRegression::predict(Dataframe &df_x){
    Matrix<double> result(df_x.content[0].rows(), df_x.content[0].cols());
    result.AddCol(0,1);

    return result*B;
}

double LinearRegression::predict(std::initializer_list<double> values){
    if(values.size() != X.cols()-1){
        qDebug() << "LR: Prediction expecting " << X.cols()-1 << "values";
        return 0;
    }
    Matrix<double> pre_X({values});

    pre_X.AddCol(0,1);

    return (pre_X*B)[0][0];
}


double LinearRegression::GetR2Score(Dataframe &df_y, Dataframe &df_y_pre){
    Matrix<double> df_Y = Matrix<double>::ToDouble(df_y.content[0]);
    Matrix<double> df_Y_pre = Matrix<double>::ToDouble(df_y_pre.content[0]);
    double pre_y = (df_Y.transpose() * df_Y_pre)[0][0];
    double mean_y = (df_Y.transpose() * df_Y)[0][0];

    return 1 - ( pre_y / mean_y);
}

Matrix<double> LinearRegression::GetB(){ return B; }
Matrix<double> LinearRegression::GetX(){return X;}
Matrix<double> LinearRegression::GetY(){return Y;}

