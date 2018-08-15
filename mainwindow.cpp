#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPainter>
#include <grid.h>
#include <matrix.h>
#include <log.h>
#include <performancetimer.h>
#define BIT1 0x01;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){

    ui->setupUi(this);

   //while(timer.test(1000)){


    //debug << df;
    lr.fit(df, "Preis in Mio");
    //debug << df;
    debug << lr.predict({50000, 0});
    debug << "SCORE";
    debug << lr;
    Dataframe<> test = df.PullRandom(0.25);
    debug << test;

    Dataframe<> x_test = test[ {"Gewinn", "Quadratmeter"} ];
    Dataframe<> y_test = test[ {"Preis in Mio"} ];

    debug << x_test;
    debug << y_test;

    Dataframe<> y_test_pred = lr.predict(x_test);
    debug << lr.GetR2Score(y_test, y_test_pred);




    Matrix<double> mx({
                          {1, 9},
                          {3, 9},
                          {4, 6},
                          {7, 3},
                          {9, 1},
                          {9, 2}
                      });

    Matrix<double> my({
                          {3},
                          {5},
                          {6},
                          {8},
                          {7},
                          {10}
                      });



    //debug << mx;


    lr2.fit(mx, my);
    debug << lr2.predict({3, 7});
    //debug << lr2.GetR2Score();
    debug << lr2.GetB();

    debug << lr2;

    debug << df[1][1];
    debug << df;
    auto df2 = df[{"Preis in Mio"}];
    debug << df2;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *e){
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    painter.setBrush(Qt::red);
    /*
    auto highestX = df.GetHighestInCol(0);
    auto lowestX = df.GetLowestInCol(0);
    auto highestY = df.GetHighestInCol(1);
    auto lowestY = df.GetLowestInCol(1);

    for(auto it = df.begin(); it != df.end(); it++){
        int newX = (df[0][it.Col] - lowestX) / (highestX - lowestX) * (width() - 10);
        int newY = (df[1][it.Col] - lowestY) / (highestY - lowestY) * (height() - 10);
        painter.drawEllipse(newX, newY, 10, 10);

        if(it.Col == df.cols() -1){break;}
    }
    */
    /*
    painter.drawLine(
                0, (lr.GetB() - lowestY) / (highestY - lowestY) * height(),
                width(), (lr.predict({highestX}) - lowestY) / (highestY - lowestY) * height());
    */

}
