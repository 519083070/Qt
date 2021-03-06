#include "widget.h"
#include "ui_widget.h"
#include <QKeyEvent>
#include <QTimer>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    /*
     *      A timer event will occur every interval milliseconds until killTimer() is called.
     *  If interval is 0, then the timer event occurs once every time there are no more
     *  window system events to process.
     *
     * 定时器事件将会间隔internal参数毫秒出发，直到killTimer()被调用。如果
     * internal参数是0，那么当没有更多的窗口系统事件发生时，计时器事件
     * 就会触发一次。
    */

    /*
     * 点击按钮时，第一次让计时器停止，第二次点击时开始，以此类推。
     * 那么说在点击数为单数时就会停止定时器，偶数时就开始计时器
    */
    count = 0;
    timer_id = this->startTimer(0);
    click_times = 1;
    connect(ui->pushButton, &QPushButton::clicked,
            [ & ](){
        if(click_times % 2 == 0)
            timer_id = this->startTimer(0);
        else if(click_times % 2 == 1)
            this->killTimer(timer_id);

        click_times++;
    });

    /*
     * 2018年2月7日23:04:08
     * 目前想用QTimer实现上述QEvent操作存在一定的困难。
     *
     *
     * 2018年2月20日18:42:43
     * 解决：
     * 在帮助文档中有以下这句话：
     *      An alternative to using QTimer is to call QObject::startTimer() for your
     *  object and reimplement the QObject::timerEvent() event handler in your
     *  class (which must inherit QObject).
     *
     *      大致是说最终QTimer的start()函数还是会调用QObject::startTimer()
     * 然后重新实现QObject::timerEvent()事件输出函数。
     *
     *      所以说在程序实现了新的timerEvent时，需要定时器完成的事情，
     * 那么就不能只将timeout()信号进行链接处理。
     *
     * 2021年5月7日01:27:48
     * 和上面的操作方式相同，每个定时器处理每个定时器的内容即可。
     * 如果要类以成员变量的方式保存多个定时器ID，那么就在timeOut
     * 事件中进行timer的id区分即可。即event->timerID();
    */

    QTimer *timer = new QTimer;
    timer->start(0);
    count_2 = 0;
    click_times_2 =  1;

    connect(ui->pushButton_2, &QPushButton::clicked,
            [ = ](){
                if(click_times_2 % 2 == 0)
                    timer->start(0);
                else if(click_times_2 % 2 == 1)
                    timer->stop();

                click_times_2++;
            });

    connect(timer, &QTimer::timeout, this,
            [&](){
                count_2++;
                //ui->label_4->setText(tr("count_2 is %1").arg(count_2));
                ui->label_5->setText(QString("count_2  is  %1").arg(count_2));
    });
}

void Widget::enterEvent(QEvent *event){
    ui->label_2->setText("鼠标进入Widget");
}

void Widget::leaveEvent(QEvent *event){
    ui->label_2->setText("鼠标离开Widget");
}

void Widget::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
    case Qt::Key_Up:
        ui->label_3->setText("上被按下");
        break;
    case Qt::Key_Down:
        ui->label_3->setText("下被按下");
        break;
    case Qt::Key_Left:
        ui->label_3->setText("左被按下");
        break;
    case Qt::Key_Right:
        ui->label_3->setText("右被按下");
        break;
    }
}

void Widget::timerEvent(QTimerEvent *event){
        count++;
        //ui->label_4->setText(tr("count is %1").arg(count));
        ui->label_4->setText(QString("count  is  %1").arg(count));
}

Widget::~Widget()
{
    delete ui;
}

