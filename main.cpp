#include <iostream>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

void show(const Mat &imageRed, const char *title) {
    namedWindow(title, WINDOW_AUTOSIZE);
    imshow(title, imageRed);
}

void getColor(const Mat &in, int lower, int upper, Mat &out) {
    if (lower >= 0) {
        inRange(in, Scalar(lower, 0, 50), Scalar(upper, 255, 255), out);
    } else {
        Mat mark1;
        Mat mark2;
        inRange(in, Scalar(0, 0, 50), Scalar(upper, 255, 255), mark1);
        inRange(in, Scalar(180+lower, 0, 50), Scalar(180, 255, 255), mark2);
        out = mark1 + mark2;
    }
    erode( out, out, getStructuringElement(0, Size(5, 5)),Point(-1,-1),4);
//    erode( out, out, getStructuringElement(0, Size(13, 13)));
    dilate(out, out, getStructuringElement(0, Size(5, 5)),Point(-1,-1),5);
//    dilate(out, out, getStructuringElement(0, Size(13, 13)));
}

void getContours(const Mat &in, vector<vector<Point>> &out){
    vector<Vec4i> hierarchy;
    findContours(in, out, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
}

Point getCentre(const vector<Point> &in){
    Point centre = Point(0, 0);
    for (const auto &point: in)
        centre += point;
    centre *= (1.0 / in.size());
    return  centre;
}

int main() {
    const Scalar_<double> &color = Scalar(255, 255, 0);

    Mat image = imread("/home/wolfgenerals/projects/OpenCVTest/data/Untitled 5.png");
    Mat imageResult = image;
    Mat imageRed;

    cvtColor(image, imageRed, COLOR_BGR2HLS);

    getColor(imageRed, -5, 5, imageRed);

    vector<vector<Point>> contours;
    getContours(imageRed, contours);

    for (int i = 0; i < contours.size(); ++i) {
        drawContours(imageResult, contours, i, color, 3);
    }


    vector<Point> centres;
    for (const auto &points: contours) {
        centres.push_back(getCentre(points));
    }
    for (const auto &point: centres) {
        circle(imageResult, point, 6, color, -1);
        cout << point.x << "|" << point.y << endl;
    }

    show(image, "original");
    show(imageRed, "zone");
    show(imageResult, "contours");

    waitKey(10000);
    return 0;
}



