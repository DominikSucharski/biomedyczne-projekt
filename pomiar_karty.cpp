#include <opencv2/opencv.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

#include "pomiar_karty.h"


PomiarKarty::PomiarKarty() {}
PomiarKarty::~PomiarKarty()
{
}

double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0) {
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1 * dy2) / sqrt((dx1*dx1 + dy1 * dy1)*(dx2*dx2 + dy2 * dy2) + 1e-10);
}

int PomiarKarty::WykonajPomiar(Mat& image, vector<Point>& approx)
{
	// poprawienie detekcji krawedzi
	Mat blurred(image);
	medianBlur(image, blurred, 9);

	Mat gray0(blurred.size(), CV_8U), gray;
	vector<vector<Point> > contours;

	//znalezienie kartki czesciowo przyslonietej
	for (int c = 0; c < 3; c++)
	{
		int ch[] = { c, 0 };
		mixChannels(&blurred, 1, &gray0, 1, ch, 1);

		// try several threshold levels
		const int threshold_level = 2;
		for (int l = 0; l < threshold_level; l++)
		{
			//wylapanie krawedzi przy roznych odcieniach kartki
			if (l == 0)
			{
				Canny(gray0, gray, 10, 20, 3); // 

				// usuniete mozliwych "dziur" pomiedzy segmentami obrazu
				dilate(gray, gray, Mat(), Point(-1, -1));
			}
			else
			{
				gray = gray0 >= (l + 1) * 255 / threshold_level;
			}

			//znalezienie konturow kartki i zapis do listy
			findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
			
			// kontury
			for (size_t i = 0; i < contours.size(); i++)
			{
				
				approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

				if (approx.size() == 4 && fabs(contourArea(Mat(approx))) > 1000 && isContourConvex(Mat(approx))) 
				{
					double maxCosine = 0;
					for (int j = 2; j < 5; j++)
					{
						double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
						maxCosine = MAX(maxCosine, cosine);
					}
					if (maxCosine < 0.3) {
						int v3 = abs(approx[0].x - approx[2].x) + abs(approx[0].y - approx[2].y);
						int v2 = abs(approx[0].x - approx[1].x) + abs(approx[0].y - approx[1].y);
						return min(v3, v2);
					}
				}
			}
			//rectangle(image, approx[0], approx[1], (0, 0, 0), 1, 8, 0);
		}
	}
	return 0;
}
