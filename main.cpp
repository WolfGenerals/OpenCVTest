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
        inRange(in, Scalar(lower, 20, 80), Scalar(upper, 235, 255), out);
    } else {
        Mat mark1;
        Mat mark2;
        inRange(in, Scalar(0, 20, 80), Scalar(upper, 235, 255), mark1);
        inRange(in, Scalar(180 + lower, 20, 80), Scalar(180, 235, 255), mark2);
        out = mark1 + mark2;
    }
    erode(out, out, getStructuringElement(0, Size(9, 9)), Point(-1, -1), 4);
//    erode( out, out, getStructuringElement(0, Size(13, 13)));
    dilate(out, out, getStructuringElement(0, Size(9, 9)), Point(-1, -1), 4);
//    dilate(out, out, getStructuringElement(0, Size(13, 13)));
}

void getContours(const Mat &in, vector<vector<Point>> &out) {
    vector<Vec4i> hierarchy;
    findContours(in, out, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
}

Point getCentre(const vector<Point> &in) {
    Point centre = Point(0, 0);
    for (const auto &point: in)
        centre += point;
    centre *= (1.0 / in.size());
    return centre;
}

void writeOnImage(const Mat &image, const vector<vector<Point>> &contours, vector<Point> &centres,
                  const Scalar_<double> &color = Scalar(0, 255, 255)) {

    for (int i = 0; i < contours.size(); ++i) {
        drawContours(image, contours, i, color, 3);
    }
    for (const auto &point: centres) {
        circle(image, point, 6, color, -1);
        putText(
                image,
                format("[%d,%d]", point.x, point.y),
                point+Point (-20,-10),
                FONT_HERSHEY_PLAIN,
                0.5,
                color);
//        cout << point.x << "|" << point.y << endl;
    }
}

void
getInfo(const Mat &image, Mat &cache, vector<vector<Point>> &contours, vector<Point> &centres, int lower, int upper) {
    cvtColor(image, cache, COLOR_BGR2HLS);

    getColor(cache, lower, upper, cache);

    getContours(cache, contours);
    for (const auto &points: contours) {
        centres.push_back(getCentre(points));
    }
}

void handle(Mat &image) {
    Mat cacheR;
    vector<vector<Point>> contoursR;
    vector<Point> centresR;

    Mat cacheB;
    vector<vector<Point>> contoursB;
    vector<Point> centresB;

    getInfo(image, cacheR, contoursR, centresR, -20, 3);
    getInfo(image, cacheB, contoursB, centresB, 80, 150);
    writeOnImage(image, contoursR, centresR,Scalar(255, 255, 0));
    writeOnImage(image, contoursB, centresB,Scalar(0, 255, 255));
}


int main() {
//    string url ="rtsp://10.237.49.125:8554/live";
//    cin >> url;
    Mat image = imread("/home/wolfgenerals/projects/OpenCVTest/data/Untitled 5.png");
    handle(image);
    namedWindow("1",WINDOW_AUTOSIZE);
    imshow("1", image);

    cout << "231;";
    VideoCapture camera = VideoCapture("rtsp://172.27.160.1");
//     (camera.getBackendName());
    cout << "233;";
//    camera.open();
    if (!camera.isOpened()) return 1;
    namedWindow("input",WINDOW_AUTOSIZE);
    namedWindow("output",WINDOW_AUTOSIZE);
    cout << "233;";

    while (true) {
        camera >> image;
        imshow("input", image);
        handle(image);
        imshow("output", image);
        waitKey(1);

    }
}



