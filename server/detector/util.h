#ifndef SERVER_DETECTOR_UTIL_H_
#define SERVER_DETECTOR_UTIL_H_

#include <string>
#include <vector>

namespace detector {

std::vector<std::string> GetAllFiles(const std::string& path);

bool GetContainerImagePath(const std::string& container_path, std::string& image_path);

}  // namespace detector

#endif  // SERVER_DETECTOR_UTIL_H_
