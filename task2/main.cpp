#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <iostream>

using namespace std;
using namespace cv;

const static int MIN_THRESH = 50;
const static int BLUR_SIZE = 10;
Rect objectBoundingRectangle = Rect(0, 0, 0, 0);
Point object = Point(0, 0);

void searchForMovement(Mat, Mat &);

int main(int argc, char* argv[])
{
    VideoCapture capture("Walk.mpg");
    if(!capture.isOpened())
    {
        cout << "ERROR: FAILED TO OPEN THE INPUT FILE." << endl;
        return EXIT_FAILURE;
    }

    string windowName = "Moving Object Detection";
    namedWindow(windowName);

    Mat frame, nextFrame,
    frameGray, nextFrameGray,
    frameDifference,
    frameThreshold;

    while(capture.get(CV_CAP_PROP_POS_FRAMES) <
          capture.get(CV_CAP_PROP_FRAME_COUNT) - 1)
    {
        capture.read(frame);
        if (frame.empty())
        {
            break;
        }

        capture.read(nextFrame);
        if (nextFrame.empty())
        {
            break;
        }

        cvtColor(frame, frameGray, COLOR_BGR2GRAY);
        cvtColor(nextFrame,nextFrameGray,COLOR_BGR2GRAY);

        absdiff(frameGray,nextFrameGray,frameDifference);

        threshold(frameDifference, frameThreshold, MIN_THRESH, 255, THRESH_BINARY);

        /*
         use blur() to smooth the image, remove possible noise and
         increase the size of the object we are trying to track.
         */
        blur(frameThreshold, frameThreshold, cv::Size(BLUR_SIZE,BLUR_SIZE));

        threshold(frameThreshold,frameThreshold,MIN_THRESH,255,THRESH_BINARY);

        searchForMovement(frameThreshold, frame);

        imshow(windowName, frame);

        waitKey(60);
    }
    return EXIT_SUCCESS;
}
//////////////////////////////////////////////////////
void searchForMovement(Mat frameThreshold, Mat &frame)
{
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;

    findContours(frameThreshold, contours, hierarchy,
                 CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    if (!contours.empty())
    {
        vector<Point> largestContour;
        largestContour = contours.at(contours.size() - 1);
        /*
         make a bounding rectangle around the largest contour, then
         find its centroid this will be the object's final estimated position.
         */
        objectBoundingRectangle = boundingRect(largestContour);

        object.x = objectBoundingRectangle.x +
        objectBoundingRectangle.width / 2;

        object.y = objectBoundingRectangle.y +
        objectBoundingRectangle.height / 2;
    }
    rectangle(frame, Point(object.x - 25, object.y - 50),
              Point(object.x + 25, object.y + 50), Scalar(0, 255, 0));
}
