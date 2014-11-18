#ifndef SERVER_DETECTOR_UTIL_H_
#define SERVER_DETECTOR_UTIL_H_

#include <string>
#include <vector>

#include "opencv2/opencv.hpp"

namespace detector {

std::vector<std::string> GetAllFiles(const std::string& path);

bool GetContainerImagePath(const std::string& container_path, std::string& image_path);

cv::Mat RescaleImage(const cv::Mat& raw_image, int max_edge_pixels);

}  // namespace detector

#endif  // SERVER_DETECTOR_UTIL_H_
