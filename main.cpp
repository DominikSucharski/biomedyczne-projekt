#include <opencv2/opencv.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "pomiar_karty.h"
#include "wykrywanie_oczu.h"
#include <iostream>

using namespace std;
using namespace cv;


string cascadeName;
string nestedCascadeName;


int main(int argc, const char** argv)
{
  vector<Point> approx;
  Mat frame;  // klatka obrazu
  CascadeClassifier cascade, nestedCascade;
  double scale =  1.0;  // skala
  double rozmiar_karty_mm = 85.60;  // d³ugoœæ d³u¿szego boku karty p³atniczej
  

  // przetwarzanie parametów
  cv::CommandLineParser parser(argc, argv,
    "{cascade|data/haarcascades/haarcascade_frontalface_alt.xml|}"
    "{nested-cascade|data/haarcascades/haarcascade_eye_tree_eyeglasses.xml|}"
  );
  cascadeName = parser.get<string>("cascade");
  nestedCascadeName = parser.get<string>("nested-cascade");
  if (!parser.check())
  {
    parser.printErrors();
    return 0;
  }
  if (!nestedCascade.load(samples::findFileOrKeep(nestedCascadeName)))
    cerr << "WARNING: Could not load classifier cascade for nested objects" << endl;

  if (!cascade.load(samples::findFile(cascadeName)))
  {
    cerr << "ERROR: Could not load classifier cascade" << endl;
    return -1;
  }


  int id_kamery = 0;  // id zewnêtrznej kamery
  VideoCapture capture(id_kamery);  // otwarcie kamery o id id_kamery
  if (!capture.isOpened()) {  // sprawdzenie czy kamera jest otwarta
    capture = VideoCapture(0);   // otwarcie domyœlnej kamery, je¿eli kamera o id id_kamery jest niedostêpna
  }
  if (!capture.isOpened())
    return -1;


  for (;;) {  // nieskoñczona pêtla
    capture >> frame;  // zapisanie obrazu z kamery
    if (frame.empty())
      break;
    Mat frame1 = frame.clone();

    wykrywanieOczu wykrywanie_oczu;
    int odleglosc_miedzy_zrenicami_px = wykrywanie_oczu.wykryj_i_rysuj(frame1, cascade, nestedCascade, scale);
    

    if (odleglosc_miedzy_zrenicami_px > 0) {
      frame1 = frame.clone();
      PomiarKarty pomiar_karty;
      int rozmiar_karty_px = pomiar_karty.WykonajPomiar(frame1, approx);

      if (rozmiar_karty_px > 0) {
        double odleglosc_zrenic = (odleglosc_miedzy_zrenicami_px * rozmiar_karty_mm) / rozmiar_karty_px;
        cout << "odleglosc zrenic = " << odleglosc_zrenic;
        cout << " mm" << endl;
      }

    }
   

    char c = (char)waitKey(10);
    if (c == 27 || c == 'q' || c == 'Q')
      break;
  }
  return 0;
}
