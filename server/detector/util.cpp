#include "server/detector/util.h"

#include <algorithm>
#include <dirent.h>
#include <fstream>

using namespace cv;
using namespace std;

namespace detector {

vector<string> GetAllFiles(const string& path) {
    vector<string> files;
    DIR* dir = opendir(path.c_str());
    if (dir) {
        dirent* dirent = readdir(dir);
        while (dirent != 0) {
            string file = dirent->d_name;
            if (file != "." && file != "..") {
                files.push_back(path + "/" + dirent->d_name);
            }
            dirent = readdir(dir);
        }
        closedir(dir);
    }
    return files;
}

bool GetContainerImagePath(const std::string& container_path, std::string& image_path) {
    bool success = false;
    DIR* folder = opendir(container_path.c_str());
    if (folder) {
        // Check if it's a png.
        ifstream png(container_path + "/image.png");
        if (png) {
            image_path = container_path + "/image.png";
            success = true;
        } else {
            // Check if it's a jpeg.
            ifstream jpeg(container_path + "/image.jpeg");
            if (jpeg) {
                image_path = container_path + "/image.jpeg";
                success = true;
            }
        }
    }
    closedir(folder);
    return success;
}

Mat RescaleImage(const Mat& raw_image, int max_edge_pixels) {
    Size raw_image_size = raw_image.size();
    auto max_edge = max(raw_image_size.width, raw_image_size.height);
    float scale_ratio = max_edge_pixels / (float) max_edge;
    Size final_size(raw_image_size.width * scale_ratio,
                    raw_image_size.height * scale_ratio);
    Mat scaled_image;
    resize(raw_image, scaled_image, final_size);
    return scaled_image;
}

}  // namespace detector
