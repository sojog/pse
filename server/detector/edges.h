#ifndef SERVER_DETECTOR_EDGES_H_
#define SERVER_DETECTOR_EDGES_H_

#include <string>

#include "opencv2/opencv.hpp"
#include "server/detector/geometry.h"

namespace detector {

class EdgeDetector {

public:
    EdgeDetector() {}
    EdgeDetector(const std::string& debug_container_path);

    // TODO(sghiaus): Implement (x, y) based detection, for the case of having
    // more than one top-level quad.
    bool DetectPaintingQuad(const cv::Mat& image, int x, int y, Quad2f& quad);

private:
    std::string debug_container_path;
};

}  // namespace detector

#endif // SERVER_DETECTOR_EDGES_H_
