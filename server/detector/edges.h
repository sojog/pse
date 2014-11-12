#include <string>

#include "opencv2/opencv.hpp"

namespace detector {

class EdgeDetector {

public:
    EdgeDetector() {}
    EdgeDetector(const std::string& debug_container_path);

    // TODO(sghiaus): Implement (x, y) based detection, for the case of having
    // more than one top-level quad.
    cv::Rect DetectPaintingRect(const cv::Mat& image, int x, int y);

private:
    std::string debug_container_path;
};

}  // namespace detector
