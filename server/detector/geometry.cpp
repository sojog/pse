#include <cmath>

#include "server/detector/geometry.h"

using namespace cv;

namespace detector {

bool IntersectLineLine(const Line2f& l1, const Line2f& l2, Point2f& result) {
    Point2f x = l2.a - l1.a;
    Point2f d1 = l1.b - l1.a;
    Point2f d2 = l2.b - l2.a;

    float cross = d1.x * d2.y - d1.y * d2.x;

    // TODO(sghiaus): Use float epsilon here.
    if (abs(cross) < 1e-8) {
        return false;
    }

    double t1 = (x.x * d2.y - x.y * d2.x) / cross;
    result = l1.a + d1 * t1;

    return true;
}

}  // namespace detector
