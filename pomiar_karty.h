#pragma once
using namespace std;
using namespace cv;

class PomiarKarty
{
public:
  PomiarKarty();
  ~PomiarKarty();


  /*
  Funkcja zwraca szeroko�� obiektu referencyjnego (karty) wyra�on� w pixelach.
  */
  int WykonajPomiar(Mat& image);
};

