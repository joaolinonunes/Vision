#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myvideocapture.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mOpenCV_videoCapture = new MyVideoCapture(this);

    connect(mOpenCV_videoCapture, &MyVideoCapture::newPixmapCaptured, this, [&]()
           {
            ui->opencvFrame->setPixmap(mOpenCV_videoCapture->pixmap().scaled(500,500));
            });
}

MainWindow::~MainWindow()
{
    delete ui;
    mOpenCV_videoCapture->terminate();
}


void MainWindow::on_StartBut_clicked()
{
    mOpenCV_videoCapture->start(QThread::HighestPriority);
}

void MainWindow::on_StopBut_clicked()
{

    mOpenCV_videoCapture->terminate();
    //ui->opencvFrame->clear();
}

void MainWindow::on_CloseBut_clicked()
{
    //on_StopBut_clicked();
    disconnect(mOpenCV_videoCapture);
    ui->opencvFrame->clear();

    //mOpenCV_videoCapture->terminate();
    //ui->opencvFrame->close();
}
