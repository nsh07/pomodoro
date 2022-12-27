#include "mainwindow.h"
#include "customtimer.h"
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Initialize all variables/pointers
    base = new QWidget;
    scrAr = new QScrollArea;
    layout = new QVBoxLayout(base); // Base layout
    tabWid = new QTabWidget(this);
    timeLbl = new QLabel;
    timerFont = QFont("sans-serif", 100);
    taskFont = QFont("sans-serif", 18);
    addBtn = new QPushButton("Add task");
    newTaskLayout = new QHBoxLayout; // Layout that holds the new task line edit and button
    lineE = new QLineEdit;
    timer = new CustomTimer;
    QPushButton *playBtn = new QPushButton(QIcon(":/icons/media-playback-start-symbolic.svg"), "Start");

    // Setup and add widgets
    newTaskLayout->addWidget(lineE);
    newTaskLayout->addWidget(addBtn);

    lineE->  setPlaceholderText("What to do next?");
    time = 1500;
    timeLbl->setText("25:00");
    timeLbl->setFont(timerFont);

    base->  setLayout(layout);
    scrAr-> setWidget(base);
    layout->addWidget(timeLbl);
    layout->addWidget(playBtn);
    base->  setMinimumHeight(50);
    layout->setSizeConstraint(QLayout::SetMinimumSize);

    tabWid->addTab(scrAr, QIcon(":/icons/preferences-system-time-symbolic.svg"), "Tasks");
    tabWid->setMinimumSize(500, 400);

    layout->addLayout(newTaskLayout);

    // Connections
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::addTask);
    connect(timer, &QTimer::timeout, this, &MainWindow::subTime);
    connect(playBtn, &QPushButton::clicked, timer, &CustomTimer::startTmr);

    layout->setAlignment(Qt::AlignCenter);
    scrAr-> setAlignment(Qt::AlignCenter);
    this->  setCentralWidget(tabWid);
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
    widgets in layout is not 3 (i.e. there is at least 1 task), else
    stop the timer
    */
    if (time != 0) time--;
    else {
        if (layout->count() == 3) {
            time = 1500;
            timer->stop();
        }
        else {
            remTask();
            time = 1500;
            if (layout->count() == 3) timer->stop();
        }
    }
    timeLbl->setText(QString::number(time / 60) + ':' + QString::number(time % 60));
}

void MainWindow::addTask()
{
    /*
    Adds a task to base layout
    */
    if (!(lineE->text().isEmpty())) {
        QLabel *taskLbl = new QLabel;
        taskLbl->setText(lineE->text());
        taskLbl->setFont(taskFont);

        layout->insertWidget(layout->count()-1, taskLbl);
        lineE->clear();
    }
}

void MainWindow::remTask(int index)
{
    /*
    Removes a task from the base layout
    Index is 2 (topmost task) by default
    */
    QLayoutItem *currItem = layout->takeAt(index);
    delete currItem->widget();
    delete currItem;
}
