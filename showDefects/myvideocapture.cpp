#include "myvideocapture.h"
#include <QDebug>
#include <iostream>

using namespace cv;
using namespace std;
RNG rng(12345);

MyVideoCapture::MyVideoCapture(QObject *parent)
    :QThread {parent}
    ,mVideoCap { ID_CAMERA }
{
}

void MyVideoCapture::run()
{
    if(mVideoCap.isOpened()){
        while(true){
            mVideoCap >> mFrame;
            if(!mFrame.empty()){

               cvtColor(mFrame,imgGray, COLOR_BGR2GRAY);
               kernel = getStructuringElement(MORPH_RECT, Size(3,3));
               erode(imgGray, imgDil, kernel);
               GaussianBlur(imgDil,imgBlur, Size(3,3),3,0);
               Canny(imgBlur,imgCanny,11,85);
               getContours(imgCanny,mFrame);

                mPixmap = cvMatToQPixmap(mFrame);
                emit newPixmapCaptured();
            }
        }

    }

}
QImage MyVideoCapture::cvMatToQImage( const cv::Mat &inMat )
   {
      switch ( inMat.type() )
      {
         // 8-bit, 4 channel
         case CV_8UC4:
         {
            QImage image( inMat.data,
                          inMat.cols, inMat.rows,
                          static_cast<int>(inMat.step),
                          QImage::Format_ARGB32 );

            return image;
         }

         // 8-bit, 3 channel
         case CV_8UC3:
         {
            QImage image( inMat.data,
                          inMat.cols, inMat.rows,
                          static_cast<int>(inMat.step),
                          QImage::Format_RGB888 );

            return image.rgbSwapped();
         }

         // 8-bit, 1 channel
         case CV_8UC1:
         {
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
            QImage image( inMat.data,
                          inMat.cols, inMat.rows,
                          static_cast<int>(inMat.step),
                          QImage::Format_Grayscale8 );
#else
            static QVector<QRgb>  sColorTable;

            // only create our color table the first time
            if ( sColorTable.isEmpty() )
            {
               sColorTable.resize( 256 );

               for ( int i = 0; i < 256; ++i )
               {
                  sColorTable[i] = qRgb( i, i, i );
               }
            }

            QImage image( inMat.data,
                          inMat.cols, inMat.rows,
                          static_cast<int>(inMat.step),
                          QImage::Format_Indexed8 );

            image.setColorTable( sColorTable );
#endif

            return image;
         }

         default:
            qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
            break;
      }

      return QImage();
   }

  QPixmap MyVideoCapture::cvMatToQPixmap( const cv::Mat &inMat )
   {
      return QPixmap::fromImage( cvMatToQImage( inMat ) );
   }
  void MyVideoCapture::getContours(Mat imgCanny, Mat mFrame){

      vector<vector<Point>> contours;// declara um vetor com todos os contornos de entrada
           vector<Vec4i> hierarchy;// declaara um vetor, contendo informações sobre a topologia da imagem. Possui tantos elementos quanto o número de contornos.

           /*findContours(imgDil,contours, hierarchy, RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
           //CHAIN_APPROX_SIMPLE, todos os pontos de fronteira são armazenados, Ele remove todos os pontos redundantes e comprime o contorno, economizando memória.

           drawContours(img,contours, -1, Scalar(127,255,255),2);
           //Para desenhar os contornos, a função cv.drawContours é usada.

           for (int i = 0; i < contours.size(); i++)
           {

               int area = contourArea(contours[i]);
               cout << area << endl;


               }*/

                vector<vector<Point> > contoursSOS;
                //vector<Vec4i> hierarchy;

                /// Detect edges using Threshold
                //threshold( DOS_gray, threshold_outputSO, 0, 255, THRESH_BINARY );
                /// Find contours
                findContours( imgCanny, contoursSOS, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

                /// Approximate contours to polygons + get bounding rects and circles
                vector<vector<Point> > contours_poly( contoursSOS.size() );
                vector<Rect> boundRect( contoursSOS.size() );
                vector<Point2f>center( contoursSOS.size() );
                vector<float>radius( contoursSOS.size() );

                for( size_t i = 0; i < contoursSOS.size(); i++ )
                   { approxPolyDP( Mat(contoursSOS[i]), contours_poly[i], 3, true );
                     boundRect[i] = boundingRect( Mat(contours_poly[i]) );
                     minEnclosingCircle( contours_poly[i], center[i], radius[i] );
                   }


                /// Get the moments
                vector<Moments> mu(contoursSOS.size() );
                for( size_t i = 0; i < contoursSOS.size(); i++ ){
                    mu[i] = moments( contoursSOS[i], false );
                  }

                ///  Get the mass centers:
                vector<Point2f> mc( contoursSOS.size() );
                for( size_t i = 0; i < contoursSOS.size(); i++ ){
                    mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
                }

                  float disteuclidiana;

                  /// Draw polygonal contour + bonding rects + circles
                  Mat drawingA = Mat::zeros( mFrame.size(), CV_8UC1 );

               for( size_t i = 0; i< contoursSOS.size(); i++ ){

                      Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );

                      disteuclidiana = sqrt(pow((boundRect[i].tl().y - boundRect[i].br().y),2) + pow((boundRect[i].tl().x - boundRect[i].br().x),2));
                      if((disteuclidiana>60)&&(disteuclidiana<1200)){
                             //drawContours( drawing, contours_poly, (int)i, color, 1, 8, vector<Vec4i>(), 0, Point() );
                             rectangle( mFrame, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );


                      }
              }

           //imshow("Contour DOS", drawingA);

  }
