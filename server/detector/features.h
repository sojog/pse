#ifndef SERVER_DETECTOR_FEATURE_MATCHER_H_
#define SERVER_DETECTOR_FEATURE_MATCHER_H_
#include <string>

namespace cv {
class Mat;
}  // namespace cv

namespace detector {

// Reference image must be grayscale.
double ComputeFeatureDistance(const cv::Mat& reference_image,
                              const std::string& template_path);

}  // namespace detector
#endif  // SERVER_DETECTOR_FEATURE_MATCHER_H_
