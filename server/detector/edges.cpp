#include "server/detector/edges.h"

using namespace cv;

namespace {

const int CANNY_MIN_THRESH = 100;
const int CANNY_MAX_THRESH = 100;
const int CANNY_APERTURE = 3;
const int GAUSS_KERNEL = 7;
const int GAUSS_SIGMA = 2;

}  // namespace

namespace detector {

EdgeDetector::EdgeDetector(const std::string& debug_container_path) {
    this->debug_container_path = debug_container_path;
}

Rect EdgeDetector::DetectPaintingRect(const Mat& image, int x, int y) {
    Mat blurred;
    GaussianBlur(image, blurred, Size(GAUSS_KERNEL, GAUSS_KERNEL),
                 GAUSS_SIGMA, GAUSS_SIGMA);

    Mat canny_edges;
    Canny(blurred, canny_edges, CANNY_MIN_THRESH, CANNY_MAX_THRESH);

    // TODO(sghiaus): Hough lines or probablistic Hough Lines?
    vector<Vec4i> hough_lines_p;
    HoughLinesP(canny_edges, hough_lines_p, 1, CV_PI / 180, 70, 30, 10);

    vector<Vec2f> hough_lines;
    HoughLines(canny_edges, hough_lines, 1, CV_PI / 180, 100, 0, 0);

    if (!debug_container_path.empty()) {
        imwrite(debug_container_path + "blurred.png", blurred);
        imwrite(debug_container_path + "canny.png", canny_edges);
        Scalar color(0, 200, 0);

        Mat hough_p_image(image.size().height, image.size().width, CV_8UC3);
        hough_p_image = Scalar(0, 0, 0);
        for (Vec4i hough_line : hough_lines_p) {
            line(hough_p_image, Point(hough_line[0], hough_line[1]),
                 Point(hough_line[2], hough_line[3]), color);
        }
        imwrite(debug_container_path + "hough_probabilistic.png", hough_p_image);

        Mat hough_image(image.size().height, image.size().width, CV_8UC3);        
        hough_image = Scalar(0, 0, 0);
        for (Vec2f hough_line : hough_lines) {
            float rho = hough_line[0];
            float theta = hough_line[1];
            Point p1, p2;
            double a = cos(theta);
            double b = sin(theta);
            double x0 = a * rho;
            double y0 = b * rho;
            p1.x = cvRound(x0 + 1000 * (-b));
            p1.y = cvRound(y0 + 1000 * (a));
            p2.x = cvRound(x0 - 1000 * (-b));
            p2.y = cvRound(y0 - 1000 * (a));
            line(hough_image, p1, p2, color);
        }
        imwrite(debug_container_path + "hough.png", hough_image);
    }

    // TODO(sghiaus): Return an actual result.
    Rect result;
    return result;
}

}  // namespace detector
