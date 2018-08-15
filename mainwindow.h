#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <performancetimer.h>
#include <log.h>
#include <linearregression.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *e);

private:
    Ui::MainWindow *ui;

    Log debug;
    PerformanceTimer timer;
    Dataframe<> df = Dataframe<>("hotels.csv", {}, {"Stadt\n"});

    LinearRegression lr;
    LinearRegression lr2;
};

#endif // MAINWINDOW_H
