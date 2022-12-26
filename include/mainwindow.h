#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QLabel>
#include <QTabWidget>
#include <QVBoxLayout>
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
        void remTask(int index=2);
        void subTime();
    
    private:
        QScrollArea *scrAr;
        QWidget *base;
        QTabWidget *tabWid;
        QLabel *timeLbl;
        QVBoxLayout *layout;

        QPushButton *addBtn;
        QHBoxLayout *newTaskLayout;
        QLineEdit *lineE;
        
        int time;
        CustomTimer *timer;
        QFont timerFont;
        QFont taskFont;
};
#endif // MAINWINDOW_H
