#ifndef SERVER_DETECTOR_GEOMETRY_H_
#define SERVER_DETECTOR_GEOMETRY_H_

#include "opencv2/core/core.hpp"

namespace detector {

struct Line2f {
    Line2f() {}
    Line2f(cv::Point2f a, cv::Point2f b) : a(a), b(b) {}

    cv::Point2f a;
    cv::Point2f b;
};

struct Quad2f {
    Quad2f() {}
    Quad2f(cv::Point2f a, cv::Point2f b, cv::Point2f c, cv::Point2f d) :
        a(a), b(b), c(c), d(d) {}

    cv::Point2f a;
    cv::Point2f b;
    cv::Point2f c;
    cv::Point2f d;
};

bool IntersectLineLine(const Line2f& m, const Line2f& n, cv::Point2f& result);

}  // namespace detector

#endif // SERVER_DETECTOR_GEOMETRY_H_
