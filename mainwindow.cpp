#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <windows.h>
#include <time.h>
#include <string>
#include <iostream>
#include <assert.h>
#include <QTime>
#include <QDebug>
using namespace std;
bool tools[5];  //全局变量，用餐工具
CRITICAL_SECTION cs; //信号量, 在线程中使用，临界区

class Philosopher
{
private:
    int number;
    int status;   /*标记当前哲学家的状态,0表示正在等待(即处于饥饿状态),1表示得到两支筷子正在吃饭,2表示正在思考*/
public:
    Philosopher(int num=0): status(2), number(num)   { }
    const  int find()
    {
        return number;
    }
    const  int getinfo()
    { return status; }
    void Change() ;  //状态改变函数
    void dead_lock();
};
void Philosopher::dead_lock()
{
    EnterCriticalSection (&cs) ; //进入临界区
    string s;
    if(status==1)
    {
        tools[number%5]=true;
        // tools[(number-1)%5]=true;
        status=2;
    }
    else if(status==2)
    {
        status=0;
    //tools[(number-1)%5]=false;
     //tools[(number-1)%5]=true;
    }
    else if(status==0)
    {
        tools[number%5]=false;
        tools[(number-1)%5]=false;
        status=1;
    }
    LeaveCriticalSection (&cs) ;
}
void Philosopher::Change()
{
    EnterCriticalSection (&cs) ; //进入临界区
    if(status==1) //正在进餐
    {
        tools[number%5]=true;  //放下左手工具
        tools[(number-1)%5]=true;  //放下右手工具
        status=2;   //改变状态为思考
    }
    else if(status==2) //思考中
    {
        status=0;   //改变状态为等待
    }
    else if(status==0) //等待中
    {
        if(tools[number%5]&&tools[(number-1)%5])   //左右手两边工具均为空闲状态
        {
            tools[number%5]=false;  //拿起左手工具
            tools[(number-1)%5]=false;  //拿起右手工具
            status=1;
        }
    }
    LeaveCriticalSection (&cs) ;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}
QString print(Philosopher *pA)
{
 //pA->Change();
     int i=pA->getinfo();
    QString str;
    if(i==0)
    str="等待";
    else if(i==1)
    str="就餐";
    else str="思考";
    return str;
}
QString toolstatus(bool a)
{
    QString state;
    if(a==true)
    state="空闲";
    if(a==false)
    state="使用";
    return state;
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    for(int i=0;i<5;i++)
        tools[i]=true; //筷子都未使用，初始化
        Philosopher P1(1),P2(2),P3(3),P4(4),P5(5);
        InitializeCriticalSection (&cs) ; //初始化初始化临界区
        int m=1;
        ui->label_12->setNum(m);
        ui->label_2->setText(print(&P1));//initalize
        ui->label_3->setText(print(&P2));
        ui->label_4->setText(print(&P3));
        ui->label_5->setText(print(&P4));
        ui->label_6->setText(print(&P5));
        ui->label_7->setText(toolstatus(tools[1]));
        ui->label_8->setText(toolstatus(tools[2]));
        ui->label_9->setText(toolstatus(tools[3]));
        ui->label_10->setText(toolstatus(tools[4]));
        ui->label_11->setText(toolstatus(tools[0]));
        while(m)
        {
                P1.Change(); P2.Change();  P3.Change();  P4.Change();  P5.Change();
                ui->label_12->setNum(m);
                ui->label_2->setText(print(&P1));//initalize
                ui->label_3->setText(print(&P2));
                ui->label_4->setText(print(&P3));
                ui->label_5->setText(print(&P4));
                ui->label_6->setText(print(&P5));
                ui->label_7->setText(toolstatus(tools[1]));
                ui->label_8->setText(toolstatus(tools[2]));
                ui->label_9->setText(toolstatus(tools[3]));
                ui->label_10->setText(toolstatus(tools[4]));
                ui->label_11->setText(toolstatus(tools[0]));
                QTime t;
                t.start();
                m++;
                while(t.elapsed()<1000)
                    QCoreApplication::processEvents();

        }
        DeleteCriticalSection (&cs) ; //退出资源区
}
