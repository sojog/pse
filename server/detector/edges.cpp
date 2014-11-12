#include "server/detector/edges.h"

#include <cmath>

using namespace cv;

namespace {

const int CANNY_MIN_THRESH = 100;
const int CANNY_MAX_THRESH = 100;
const int CANNY_APERTURE = 3;
const int GAUSS_KERNEL = 5;
const int GAUSS_SIGMA = 2;

}  // namespace

namespace detector {

EdgeDetector::EdgeDetector(const std::string& debug_container_path) {
    this->debug_container_path = debug_container_path;
}

bool EdgeDetector::DetectPaintingQuad(const Mat& image, int hit_x, int hit_y, 
                                      Quad2f& painging_quad) {
    Mat blurred;
    GaussianBlur(image, blurred, Size(GAUSS_KERNEL, GAUSS_KERNEL),
                 GAUSS_SIGMA, GAUSS_SIGMA);

    Mat canny_edges;
    Canny(blurred, canny_edges, CANNY_MIN_THRESH, CANNY_MAX_THRESH);

    vector<Vec4i> hough_lines;
    HoughLinesP(canny_edges, hough_lines, 1, CV_PI / 180, 90, 10, 40);

    vector<Line2f> horizontal_lines;
    vector<Line2f> vertical_lines;
    for (Vec4i hough_line : hough_lines) {
        Line2f line(Point(hough_line[0], hough_line[1]),
                    Point(hough_line[2], hough_line[3]));
        if (abs(line.a.x - line.b.x) > abs(line.a.y - line.b.y)) {
            horizontal_lines.push_back(line);
        } else {
            vertical_lines.push_back(line);
        }
    }

    Line2f left_line, right_line, top_line, bottom_line;
    if (horizontal_lines.size() >= 2 && vertical_lines.size() >= 2) {
    }

    // TODO(sghiaus): Cut everything that's outside of line intersections,
    // relative to hit point.

    if (!debug_container_path.empty()) {
        imwrite(debug_container_path + "blurred.png", blurred);
        imwrite(debug_container_path + "canny.png", canny_edges);
        Scalar color(0, 200, 0);

        Mat hough_image(image.size().height, image.size().width, CV_8UC3);        
        hough_image = Scalar(0, 0, 0);
        for (const Line2f& l : horizontal_lines) {
            line(hough_image, l.a, l.b, Scalar(0, 200, 0));
        }
        for (const Line2f& l : vertical_lines) {
            line(hough_image, l.a, l.b, Scalar(0, 0, 200));
        }
        imwrite(debug_container_path + "hough.png", hough_image);
    }

    return true;
}

}  // namespace detector
