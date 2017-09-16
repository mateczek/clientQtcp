#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDateTime>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    clientSocket=new QTcpSocket(this);
    clientSocket->connectToHost("192.168.0.11",3333);
    connect(clientSocket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    connect(clientSocket,SIGNAL(connected()),this,SLOT(connectToServer()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readyRead()
{
     static qint16 blockSize=0;
     static bool unlock=true;

     QDataStream in(clientSocket);
     if ((clientSocket->bytesAvailable()>=2)&&unlock){
         in>>blockSize;
         unlock=false;
     }
     if (blockSize==-127){
         ui->int1->setText("brak danych do obróbki");
         ui->int2->setText("brak danych do obróbki");
         ui->dateTime1->setText("brak danych do obróbki");
         ui->dateTime2->setText("brak danych do obróbki");
         ui->string1->setText("brak danych do obróbki");
         ui->string2->setText("brak danych do obróbki");
         unlock=true;
         return;
     }

     if(clientSocket->bytesAvailable()>=blockSize){
        int int1,int2;
        QString string1,string2;
        QDateTime time1,time2;
        in>>int1>>string1>>time1>>time2>>string2>>int2;
        ui->int1->setText(QString::number(int1));
        ui->int2->setText(QString::number(int2));
        ui->dateTime1->setText(time1.toString());
        ui->dateTime2->setText(time2.toString());
        ui->string1->setText(string1);
        ui->string2->setText(string2);
        unlock=true;
        blockSize=0;
     }
}

void MainWindow::connectToServer()
{
    QByteArray frame {"gateA\r\n"};
    clientSocket->write(frame);
}
