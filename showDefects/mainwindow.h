#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MyVideoCapture;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_StartBut_clicked();

    void on_StopBut_clicked();

    void on_CloseBut_clicked();

private:
    Ui::MainWindow *ui;
    MyVideoCapture *mOpenCV_videoCapture;
};
#endif // MAINWINDOW_H
