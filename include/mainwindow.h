#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QLabel>
#include <QTabWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QScrollArea>
#include <QFont>
#include <QPushButton>

#include "customtimer.h"

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
    
    public slots:
        void addTask();
        void remTask(int index=3);
        void remTask(QLabel *lbl);
        void subTime();
        void resetTime();
    
    private:
        QScrollArea *scrAr;
        QWidget *base;
        QTabWidget *tabWid;
        QLabel *timeLbl;
        QFormLayout *layout;

        QPushButton *addBtn;
        QLineEdit *lineE;
        QString iconSuffix;
        
        int time;
        CustomTimer *timer;
        QFont timerFont;
        QFont taskFont;
};
#endif // MAINWINDOW_H
