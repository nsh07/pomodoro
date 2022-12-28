#include "mainwindow.h"
#include "customtimer.h"
#include <QIcon>
#include <QPalette>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Initialize all variables/pointers
    base         = new QWidget;
    scrAr        = new QScrollArea;
    layout       = new QFormLayout(base); // Base layout
    tabWid       = new QTabWidget(this);
    timeLbl      = new QLabel;
    breakTimeLbl = new QLabel;
    timerFont    = QFont("sans-serif", 100);
    taskFont     = QFont("sans-serif", 18);
    lineE        = new QLineEdit;
    timer        = new CustomTimer;

    int textHSV = timeLbl->palette().color(QPalette::WindowText).value();
    int bgHSV = timeLbl->palette().color(QPalette::Window).value();
    iconSuffix = "";
    if (textHSV > bgHSV) iconSuffix = "-light";

    addBtn = new QPushButton(QIcon(":/icons/add_task" + iconSuffix + ".svg"), "");
    QPushButton *startBtn = new QPushButton(QIcon(":/icons/play_arrow" + iconSuffix + ".svg"), "Start");
    QPushButton *stopBtn  = new QPushButton(QIcon(":/icons/pause" + iconSuffix + ".svg"), "Stop");
    QPushButton *resetBtn = new QPushButton(QIcon(":/icons/restart_alt" + iconSuffix + ".svg"), "Reset");
    QHBoxLayout *stopResetLayout = new QHBoxLayout;

    // Setup and add widgets
    stopResetLayout->addWidget(stopBtn);
    stopResetLayout->addWidget(resetBtn);

    lineE  ->setPlaceholderText("What to do next?");
    time = 1500;
    breakTime = 300;
    timeLbl->setText("25:00");
    breakTimeLbl->setText("5:00");
    timeLbl->setFont(timerFont);
    breakTimeLbl->setFont(timerFont);
    timeLbl->setAlignment(Qt::AlignCenter);
    breakTimeLbl->setAlignment(Qt::AlignCenter);

    base  ->setLayout(layout);
    scrAr ->setWidget(base);
    layout->addRow(timeLbl);
    layout->addRow(startBtn);
    layout->addRow(stopResetLayout);
    layout->addRow(addBtn, lineE);
    base  ->setMinimumHeight(50);
    layout->setSizeConstraint(QLayout::SetMinimumSize);

    tabWid->addTab(scrAr, QIcon(":/icons/pending_actions" + iconSuffix + ".svg"), "Tasks");
    tabWid->addTab(breakTimeLbl, QIcon(":/icons/local_cafe" + iconSuffix + ".svg"), "Break");
    tabWid->setMinimumSize(640, 480);

    // Connections
    connect(addBtn,   &QPushButton::clicked,     this,  &MainWindow::addTask  );
    connect(lineE,    &QLineEdit::returnPressed, this,  &MainWindow::addTask  );
    connect(timer,    &QTimer::timeout,          this,  &MainWindow::subTime  );
    connect(startBtn, &QPushButton::clicked,     timer, &CustomTimer::startTmr);
    connect(stopBtn,  &QPushButton::clicked,     timer, &CustomTimer::stop    );
    connect(resetBtn, &QPushButton::clicked,     this,  &MainWindow::resetTime);

    scrAr->setAlignment(Qt::AlignCenter);
    this ->setCentralWidget(tabWid);
}

MainWindow::~MainWindow()
{
}

void MainWindow::subTime()
{
    /*
    Time step logic
    If the current time is not zero, reduce it by 1 second
    else, remove the topmost task from the task list if the number of
    widgets in layout is not 4 (i.e. there is at least 1 task), else
    stop the timer
    */
    if (time > 0) time--;
    else {
        if (layout->rowCount() == 4) {
            time = 1500;
            timer->stop();
        }
        else {
            remTask();
            time = 1500;
            if (layout->rowCount() == 4) timer->stop();
            else startBreak();
        }
    }
    timeLbl->setText(QString::number(time / 60) + ':' + QString::number(time % 60));
}

void MainWindow::subBreakTime()
{
    if (breakTime != 0) breakTime--;
    else endBreak();
    breakTimeLbl->setText(QString::number(breakTime / 60) + ':' + QString::number(breakTime % 60));
}

void MainWindow::resetTime()
{
    time = 1500;
    timeLbl->setText(QString::number(time / 60) + ':' + QString::number(time % 60));
}

void MainWindow::addTask()
{
    /*
    Adds a task to base layout
    Adds a button to remove the task and the task label to the base layout
    See the QLabel* overload of remTask()
    */
    if (!(lineE->text().isEmpty())) {
        QLabel *taskLbl = new QLabel;
        QPushButton *remBtn = new QPushButton(QIcon(":/icons/delete" + iconSuffix + ".svg"), "");
        taskLbl->setText(lineE->text());
        taskLbl->setFont(taskFont);
        connect(remBtn, &QPushButton::clicked, this, [this, taskLbl]{ remTask(taskLbl); });

        layout->insertRow(layout->rowCount() - 1, remBtn, taskLbl);
        lineE->clear();
    }
}

void MainWindow::remTask(int index)
{
    layout->removeRow(index);
}

void MainWindow::remTask(QLabel *lbl)
{
    /*
    Removes a task from the base layout
    Obtains index of the task to be removed by passing its task label to
    QFormLayout::getWidgetPosition() and then removes the row at that index
    */
    int i;
    layout->getWidgetPosition(lbl, &i, nullptr);
    layout->removeRow(i);
}

void MainWindow::startBreak()
{
    /*
    Connect timer to subBreakTime and switch to break tab
    */
    disconnect(timer, &QTimer::timeout, this, &MainWindow::subTime);
    tabWid->setCurrentIndex(1);
    connect(timer, &QTimer::timeout, this, &MainWindow::subBreakTime);
}

void MainWindow::endBreak()
{
    /*
    Reset break time, connect timer to subTime and switch to tasks tab
    */
    breakTime = 300;
    disconnect(timer, &QTimer::timeout, this, &MainWindow::subBreakTime);
    tabWid->setCurrentIndex(0);
    connect(timer, &QTimer::timeout, this, &MainWindow::subTime);
}
