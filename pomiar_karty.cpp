#include <opencv2/opencv.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "pomiar_karty.h"
#include <iostream>


PomiarKarty::PomiarKarty()
{
}


PomiarKarty::~PomiarKarty()
{
}

int PomiarKarty::WykonajPomiar(Mat& image)
{
  cout << "Pomiar karty..." << endl;

  // blur will enhance edge detection
  Mat blurred(image);
  medianBlur(image, blurred, 9);

  Mat gray0(blurred.size(), CV_8U), gray;
  vector<vector<Point> > contours;

  // find squares in every color plane of the image
  for (int c = 0; c < 3; c++)
  {
    int ch[] = { c, 0 };
    mixChannels(&blurred, 1, &gray0, 1, ch, 1);

    // try several threshold levels
    const int threshold_level = 2;
    for (int l = 0; l < threshold_level; l++)
    {
      // Use Canny instead of zero threshold level!
      // Canny helps to catch squares with gradient shading
      if (l == 0)
      {
        Canny(gray0, gray, 10, 20, 3); // 

        // Dilate helps to remove potential holes between edge segments
        dilate(gray, gray, Mat(), Point(-1, -1));
      }
      else
      {
        gray = gray0 >= (l + 1) * 255 / threshold_level;
      }

      // Find contours and store them in a list
      findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

      double max_area = 0;
      int square_id = 0;
      vector<Point> max_approx;

      // Test contours
      vector<Point> approx;
      for (size_t i = 0; i < contours.size(); i++)
      {
        // approximate contour with accuracy proportional
        // to the contour perimeter
        approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

        // Note: absolute value of an area is used because
        // area may be positive or negative - in accordance with the
        // contour orientation
        if (approx.size() == 4 &&
          fabs(contourArea(Mat(approx))) > 1000 &&
          isContourConvex(Mat(approx)))
        {
          double area = fabs(contourArea(Mat(approx)));

          if (area > max_area) {
            square_id = (int)i;
            max_area = area;
          }


          /*
          double maxCosine = 0;

          for (int j = 2; j < 5; j++)
          {
            double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
            maxCosine = MAX(maxCosine, cosine);
          }

          if (maxCosine < 0.3)
            squares.push_back(approx);  
            */
        }
      }


      if (contours.size() > 0) {
        approxPolyDP(Mat(contours[square_id]), approx, arcLength(Mat(contours[square_id]), true)*0.02, true);
        // max_approx.push_back(approx[0]);
         //max_approx.push_back(approx[1]);
         //max_approx.push_back(approx[2]);
         //max_approx.push_back(approx[3]);

        if (approx.size() > 3) rectangle(image, approx[0], approx[2], 2, 8, 0);
        cout << max_area << endl;


      }


    }
  }
  return 0;
}
