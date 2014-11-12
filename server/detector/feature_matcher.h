#include <string>

namespace cv {
class Mat;
}  // namespace cv

// Reference image must be grayscale.
int ComputeFeatureMatchScore(const cv::Mat& reference_image,
                             const std::string& template_path);
