#pragma once
using namespace std;
using namespace cv;

class PomiarKarty
{
public:
  PomiarKarty();
  ~PomiarKarty();


  /*
  Funkcja zwraca szerokoœæ obiektu referencyjnego (karty) wyra¿on¹ w pixelach.
  */
  int WykonajPomiar(Mat& image);
};

