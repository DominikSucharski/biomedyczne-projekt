#pragma once
using namespace std;
using namespace cv;


class wykrywanieOczu
{
public:
  wykrywanieOczu();
  ~wykrywanieOczu();

  /*
  Funkcja zwraca odleg�o�� mi�dzy �renicami wyra�on� w pixelach.
  */
  int wykryj_i_rysuj(Mat& img, CascadeClassifier& cascade, CascadeClassifier& nestedCascade, double scale);
};

