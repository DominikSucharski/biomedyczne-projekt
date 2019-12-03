#pragma once
using namespace std;
using namespace cv;


class wykrywanieOczu
{
public:
  wykrywanieOczu();
  ~wykrywanieOczu();

  /*
  Funkcja zwraca odleg³oœæ miêdzy Ÿrenicami wyra¿on¹ w pixelach.
  */
  int wykryj_i_rysuj(Mat& img, CascadeClassifier& cascade, CascadeClassifier& nestedCascade, double scale);
};

