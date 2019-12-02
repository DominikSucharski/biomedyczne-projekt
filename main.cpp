#include<opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;

int main(int, char**)
{
  int id_kamery = 2;



  VideoCapture cap(id_kamery);  // otwarcie kamery o id id_kamery
  if (!cap.isOpened())  // sprawdzenie czy kamera jest otwarta
  {
    cap = VideoCapture(0); // otwarcie domyœlnej kamery
  }
  if (!cap.isOpened())
    return -1;


  namedWindow("human height", 1);


  for (;;) {  // g³ówna nieskoñczona pêtla
    Mat frame, grey;
    cap >> frame;

    cvtColor(frame, grey, cv::COLOR_BGR2GRAY);

    cv::imshow("human height", frame);

    if (waitKey(30) >= 0) break;
  }

  // the camera will be deinitialized automatically in VideoCapture destructor
  return 0;
}