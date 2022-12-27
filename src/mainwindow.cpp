#include "mainwindow.h"
#include "customtimer.h"
#include <QIcon>
#include <QPalette>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Initialize all variables/pointers
    base      = new QWidget;
    scrAr     = new QScrollArea;
    layout    = new QFormLayout(base); // Base layout
    tabWid    = new QTabWidget(this);
    timeLbl   = new QLabel;
    timerFont = QFont("sans-serif", 100);
    taskFont  = QFont("sans-serif", 18);
    lineE     = new QLineEdit;
    timer     = new CustomTimer;

    int textHSV = timeLbl->palette().color(QPalette::WindowText).value();
    int bgHSV = timeLbl->palette().color(QPalette::Window).value();
    iconSuffix = "";
    if (textHSV > bgHSV) iconSuffix = "-light";

    addBtn    = new QPushButton(QIcon(":/icons/list-add-symbolic" + iconSuffix + ".svg"), "");
    QPushButton *startBtn = new QPushButton(QIcon(":/icons/media-playback-start-symbolic" + iconSuffix + ".svg"), "Start");
    QPushButton *stopBtn  = new QPushButton(QIcon(":/icons/media-playback-stop-symbolic" + iconSuffix + ".svg"), "Stop");
    QPushButton *resetBtn = new QPushButton(QIcon(":/icons/system-restart-symbolic" + iconSuffix + ".svg"), "Reset");
    QHBoxLayout *stopResetLayout = new QHBoxLayout;

    // Setup and add widgets
    stopResetLayout->addWidget(stopBtn);
    stopResetLayout->addWidget(resetBtn);

    lineE  ->setPlaceholderText("What to do next?");
    time = 1500;
    timeLbl->setText("25:00");
    timeLbl->setFont(timerFont);
    timeLbl->setAlignment(Qt::AlignCenter);

    base  ->setLayout(layout);
    scrAr ->setWidget(base);
    layout->addRow(timeLbl);
    layout->addRow(startBtn);
    layout->addRow(stopResetLayout);
    layout->addRow(addBtn, lineE);
    base  ->setMinimumHeight(50);
    layout->setSizeConstraint(QLayout::SetMinimumSize);

    tabWid->addTab(scrAr, QIcon(":/icons/preferences-system-time-symbolic" + iconSuffix + ".svg"), "Tasks");
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
    widgets in layout is not 3 (i.e. there is at least 1 task), else
    stop the timer
    */
    if (time != 0) time--;
    else {
        if (layout->rowCount() == 4) {
            time = 1500;
            timer->stop();
        }
        else {
            remTask();
            time = 1500;
            if (layout->rowCount() == 4) timer->stop();
        }
    }
    timeLbl->setText(QString::number(time / 60) + ':' + QString::number(time % 60));
}

void MainWindow::resetTime()
{
    time = 1500;
    timeLbl->setText("25:00");
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
        QPushButton *remBtn = new QPushButton(QIcon(":/icons/edit-delete-symbolic" + iconSuffix + ".svg"), "");
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
