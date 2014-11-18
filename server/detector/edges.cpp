#include "server/detector/edges.h"

#include <cmath>

#include "server/detector/util.h"

using namespace cv;

namespace {

const int CANNY_APERTURE = 3;
const int GAUSS_KERNEL = 5;
const int GAUSS_SIGMA = 2;

const int MAX_EDGE_PIXELS = 512;

}  // namespace

namespace detector {

EdgeDetector::EdgeDetector(const std::string& debug_container_path) {
    this->debug_container_path = debug_container_path;
}

// Assuming the image is grayscale at this point.
bool EdgeDetector::DetectPaintingQuad(const Mat& image, int hit_x, int hit_y, 
                                      Quad2f& painging_quad) {
    Mat resized = RescaleImage(image, 512);

    Mat blurred;
    GaussianBlur(resized, blurred, Size(GAUSS_KERNEL, GAUSS_KERNEL),
                 GAUSS_SIGMA, GAUSS_SIGMA);

    Mat canny_edges;
    Canny(blurred, canny_edges, 20, 60);

    vector<Vec2f> hough_lines;
    HoughLines(canny_edges, hough_lines, 1, CV_PI/180, 90);

    vector<Vec4i> hough_p_lines;
    HoughLinesP(canny_edges, hough_p_lines, 1, CV_PI / 180, 90, 10, 40);

    vector<Line2f> horizontal_lines;
    vector<Line2f> vertical_lines;
    for (Vec4i hough_line : hough_p_lines) {
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

    // TODO(sghiaus): Cut everything that's outside of the painting rectangle.

    // TODO(sghiaus): Rescale the final output image.


    // Output debug info for intermediary steps.
    if (!debug_container_path.empty()) {
        imwrite(debug_container_path + "blurred.png", blurred);
        imwrite(debug_container_path + "canny.png", canny_edges);
        Scalar color(0, 200, 0);

        Mat hough_image(canny_edges.size().height, canny_edges.size().width, CV_8UC3);
        hough_image = Scalar(0, 0, 0);
        // Draw Hough lines.
        for (size_t i = 0; i < hough_lines.size(); ++i) {
          float rho = hough_lines[i][0], theta = hough_lines[i][1];
          Point pt1, pt2;
          double a = cos(theta), b = sin(theta);
          double x0 = a*rho, y0 = b*rho;
          pt1.x = cvRound(x0 + 1000*(-b));
          pt1.y = cvRound(y0 + 1000*(a));
          pt2.x = cvRound(x0 - 1000*(-b));
          pt2.y = cvRound(y0 - 1000*(a));
          line(hough_image, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
        }
        imwrite(debug_container_path + "hough.png", hough_image);

        // Draw probabilistic Hough lines.
        Mat hough_p_image(canny_edges.size().height, canny_edges.size().width, CV_8UC3);
        hough_p_image = Scalar(0, 0, 0);
        for (const Line2f& l : horizontal_lines) {
            line(hough_p_image, l.a, l.b, Scalar(0, 200, 0));
        }
        for (const Line2f& l : vertical_lines) {
            line(hough_p_image, l.a, l.b, Scalar(0, 0, 200));
        }
        imwrite(debug_container_path + "hough_p.png", hough_p_image);
    }

    return true;
}

}  // namespace detector
