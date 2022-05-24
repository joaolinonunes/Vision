#ifndef MYVIDEOCAPTURE_H
#define MYVIDEOCAPTURE_H

#include <QPixmap>
#include <QImage>
#include <QThread>
#include <opencv2/opencv.hpp>
#include <opencv4/opencv2/opencv.hpp>

#define ID_CAMERA 0

class MyVideoCapture : public QThread
{
    Q_OBJECT
public:
    MyVideoCapture(QObject *parent = nullptr);
    QPixmap pixmap() const
    {
            return mPixmap;
     }
signals:
    void newPixmapCaptured();
protected:
    void run() override;
    void getContours(cv::Mat imgCanny,cv::Mat mFrame);
  private:
    QPixmap mPixmap;
    cv::Mat mFrame,imgGray, kernel,imgDil,imgBlur,imgCanny;
    cv::VideoCapture mVideoCap;


    QImage cvMatToQImage(const cv::Mat &inMat);
    QPixmap cvMatToQPixmap(const cv::Mat &inMat);
};

#endif // MYVIDEOCAPTURE_H
