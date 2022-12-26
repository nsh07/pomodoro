#ifndef CUSTOMTIMER_H
#define CUSTOMTIMER_H

#include <QTimer>

class CustomTimer: public QTimer
{
    public slots:
        void startTmr();
};

#endif