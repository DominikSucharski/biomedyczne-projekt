#include <opencv2/opencv.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "pomiar_karty.h"
#include <iostream>

#include "wykrywanie_oczu.h"


wykrywanieOczu::wykrywanieOczu() {}


wykrywanieOczu::~wykrywanieOczu() {}


int wykrywanieOczu::wykryj_i_rysuj(Mat & img, CascadeClassifier & cascade, CascadeClassifier & nestedCascade, double scale)
{
  vector<Rect> faces, faces2;
  const static Scalar colors[] =
  {
      Scalar(255,0,0),
      Scalar(255,128,0),
      Scalar(255,255,0),
      Scalar(0,255,0),
      Scalar(0,128,255),
      Scalar(0,255,255),
      Scalar(0,0,255),
      Scalar(255,0,255)
  };
  Mat gray, smallImg;
  cvtColor(img, gray, COLOR_BGR2GRAY);
  double fx = 1 / scale;
  resize(gray, smallImg, Size(), fx, fx, INTER_LINEAR_EXACT);
  equalizeHist(smallImg, smallImg);

  // wykrywanie twarzy
  cascade.detectMultiScale(smallImg, faces,
    1.1, 2, 0
    //|CASCADE_FIND_BIGGEST_OBJECT
    //|CASCADE_DO_ROUGH_SEARCH
    | CASCADE_SCALE_IMAGE,
    Size(30, 30));

  for (size_t i = 0; i < faces.size(); i++)
  {
    if (i > 0) break;  // wykorzystanie tylko pierwszej wykrytej twarzy
    Rect r = faces[i];
    Mat smallImgROI;
    vector<Rect> nestedObjects;
    Point center;
    Scalar color = colors[i % 8];
    int radius;
    double aspect_ratio = (double)r.width / r.height;

    // rysowanie okrêgów wokó³ twarzy
    if (0.75 < aspect_ratio && aspect_ratio < 1.3)
    {
      center.x = cvRound((r.x + r.width*0.5)*scale);
      center.y = cvRound((r.y + r.height*0.5)*scale);
      radius = cvRound((r.width + r.height)*0.25*scale);  // wielkoœæ okrêgów detekcji twarzy 0.25
      circle(img, center, radius, color, 3, 8, 0);
    }
    else
      rectangle(img, Point(cvRound(r.x*scale), cvRound(r.y*scale)),
        Point(cvRound((r.x + r.width - 1)*scale), cvRound((r.y + r.height - 1)*scale)),
        color, 3, 8, 0);
    if (nestedCascade.empty())
      continue;
    smallImgROI = smallImg(r);
    nestedCascade.detectMultiScale(smallImgROI, nestedObjects,
      1.1, 2, 0
      //|CASCADE_FIND_BIGGEST_OBJECT
      //|CASCADE_DO_ROUGH_SEARCH
      //|CASCADE_DO_CANNY_PRUNING
      | CASCADE_SCALE_IMAGE,
      Size(30, 30));

    Point oko1, oko2;  // œrodek oka 1 i 2

    // rysowanie okrêgów wokó³ oczu
    for (size_t j = 0; j < nestedObjects.size(); j++)
    {
      Rect nr = nestedObjects[j];
      center.x = cvRound((r.x + nr.x + nr.width*0.5)*scale);
      center.y = cvRound((r.y + nr.y + nr.height*0.5)*scale);
      if (j == 0) oko1 = Point(center.x, center.y);
      else if (j == 1) oko2 = Point(center.x, center.y);
      radius = cvRound((nr.width + nr.height)*0.25*scale);  // wielkoœæ okrêgów detekcji oczu 0.25
      circle(img, center, radius, color, 3, 8, 0);
    }

    if (nestedObjects.size() < 1) putText(img, "Nie wykryto oczu", Point(50, 100), cv::FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0), 2);
    else if (nestedObjects.size() < 2) putText(img, "Wykryto jedno oko", Point(50, 100), cv::FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0), 2);
    else {
      putText(img, "Zmierzono odleglosc", Point(50, 100), cv::FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0), 2);

      int diff_x = abs(oko1.x - oko2.x);
      return diff_x;
    }
  }
  return 0;
}
