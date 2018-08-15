#ifndef LOG_H
#define LOG_H

#include <math.h>
#include "linearregression.h"
#include <matrix.h>
#include <dataframe.h>
#include <QString>
#include <grid.h>
#include <vector>


template<class T>
class Matrix;

template<class T>
class Grid;

template<class T>
class Dataframe;

class LinearRegression;

class Log{
private:
    QString cLine = "";
    std::vector<QString> cLines;

public:
    Log(){

    }

    template<class T>
    void operator <<(Matrix<T> &matrix){
        qDebug() << "Matrix: ";
        Grid<T> *grid = &matrix;
        Print(*grid);
    }

    template<class T>
    void operator <<(const Matrix<T> &matrix){
        qDebug() << "Matrix: ";
        Grid<T> *grid = const_cast<Matrix<T>*>(&matrix);

        Print(*grid);
    }

    template<class T>
    void operator <<(const Dataframe<T> &dataframe){
        qDebug() << "Dataframe: ";
        QString s;

        for(auto it = dataframe.Col_Titles.begin(); it != dataframe.Col_Titles.end();it++ ){
            s += QString(it->c_str()) + " | ";
        }

        qDebug() << "    " << s;
        Grid<T> *grid = const_cast<Dataframe<T>*>(&dataframe);
        Print(*grid);
    }

    template<class T>
    void operator <<(Grid<T> &grid){
        Print(grid);
    }

    template<class T>
    void operator <<(const Grid<T> &grid){
        Print(*const_cast<Grid<T>*>(&grid));
    }

    void operator <<(LinearRegression &lr){

        Matrix<double> y_pre = lr.predict();

        AddMatrix(lr.X, "X");
        AddMatrix(lr.Y, "Y");
        AddMatrix(y_pre, "P");
        AddMatrix(lr.B, "B");

        Print();

    }

    template<class T>
    void Print(Grid<T> &grid, int DecimalRounds = -1){
        auto highestDigitsInCols = grid.GetLongestValueInCols();
        for(size_t row = 0; row < grid.rows(); row++){
            QString c;
            for(size_t col = 0; col < grid.cols(); col++){
                QString sExtention;
                QString newQStr;
                newQStr = QString::number(grid.GetValue(row, col));
                //newQStr = grid.GetRounded(row, col, T());
                for(size_t i = newQStr.length(); i < highestDigitsInCols[col]; i++){
                    sExtention += " ";
                }

                c += sExtention + newQStr  + " | ";

            }
            qDebug() << "    " << c;
        }
    }

    void operator <<(const std::string &s){
        qDebug() << s.c_str();
    }

    template<class T>
    void operator <<(const T &t){
        qDebug() << typeid(t).name() << t;
    }

    template<class T>
    T Round(T number){
        return round(number * 1000) / 1000;
    }

    template<class T>
    size_t GetDigits(T number){
        if(number == 0) return 1;

        size_t count = 0;

        if(number < 0){
            number = fabs(number);
            count++;
        }


        double integral = floor(number);
        double floating = round( (number - integral) * 1000);

        if(integral != 0){
            count += floor( log10(integral) ) +1;
        }else{
            count++;
        }

        if(floating == 0) return count;
        count++;

        count += log10(1000);
        while(fmod(floating, 10) == 0 ){

            floating /= 10;
            count--;
        }

        return count;
    }

    template<class T>
    Matrix<size_t> GetDigitMatrix(Matrix<T> &m){
        Matrix<size_t> Digit_Matrix(m.rows(), m.cols());

        for(size_t c = 0; c < m.cols(); c++){
            for(size_t r = 0; r < m.rows(); r++){
                Digit_Matrix[r][c] = GetDigits(m[r][c]);
            }
        }

        return Digit_Matrix;
    }

    void AddSpaces(size_t count, char ch = ' ', int row_i = -1){
        QString *cStr = row_i == -1 ? &cLine : &cLines[row_i];
        for(size_t i = 0; i < count; i++){
            *cStr += ch;
        }
    }

    void Print(){
        qDebug() << cLine;
        for(auto str : cLines){
            qDebug() << str;
        }
        cLine = "";
        cLines.clear();
    }

    template<class T>
    void AddMatrix(Matrix<T> &m, QString title = "_"){
        Matrix<size_t> digit_matrix = GetDigitMatrix(m);
        Grid<size_t> highests = digit_matrix.GetHighestInCols();

        size_t char_count = highests[0][0] + 2;
        for(size_t c = 1; c < highests.cols(); c++){
            char_count += highests[0][c] + 3;
        }

        cLines.emplace_back(QString());

        cLines[0] += " |";
        AddSpaces(floor((float)char_count / 2), '_', 0);
        cLines[0] += title;
        AddSpaces( ceil((float)char_count / 2) -1, '_', 0);
        cLines[0] += "| ";

        for(size_t r = 0; r < m.rows(); r++){
            if(cLines.size() < r+2) cLines.emplace_back(QString());
            cLines[r+1] += " |";
            for(size_t c = 0; c < m.cols();c++){
                AddSpaces(highests[0][c] - digit_matrix[r][c], ' ', r+1);

                cLines[r+1] += " " + QString::number(Round(m[r][c])) + " |";
            }
            cLines[r+1] += " ";
        }
    }
};

#endif // LOG_H
