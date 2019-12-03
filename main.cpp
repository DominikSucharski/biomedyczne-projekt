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


void detectAndDraw(Mat& img, CascadeClassifier& cascade, CascadeClassifier& nestedCascade, double scale);
void find_squares(Mat& image, vector<vector<Point> >& squares);



string cascadeName;
string nestedCascadeName;


int main(int argc, const char** argv)
{


  Mat frame, image;
  CascadeClassifier cascade, nestedCascade;
  double scale =  1.0;
  int id_kamery = 0;

  VideoCapture capture(id_kamery);  // otwarcie kamery o id id_kamery


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


  
  if (!capture.isOpened())  // sprawdzenie czy kamera jest otwarta
  {
    // otwarcie domyœlnej kamery, je¿eli kamera o id id_kamery jest niedostêpna
    capture = VideoCapture(0);
  }
  if (!capture.isOpened())
    return -1;


  if (capture.isOpened())
  {
    cout << "Video capturing has been started ..." << endl;
    for (;;)
    {
      capture >> frame;
      if (frame.empty())
        break;
      Mat frame1 = frame.clone();
      

      wykrywanieOczu wykrywanie_oczu;
      int odleglosc_miedzy_zrenicami_px = wykrywanie_oczu.wykryj_i_rysuj(frame1, cascade, nestedCascade, scale);

      if (odleglosc_miedzy_zrenicami_px > 0) {
        cout << "Odleglosc miedzy oczami: ";
        cout << odleglosc_miedzy_zrenicami_px;
        cout << "px" << endl;
        Mat frame2 = frame.clone();
        PomiarKarty pomiar_karty;
        int rozmiar_karty_px = pomiar_karty.WykonajPomiar(frame2);
      }


      char c = (char)waitKey(10);
      if (c == 27 || c == 'q' || c == 'Q')
        break;
    }
  }
  return 0;
}
