#include <string>

#include "server/detector/util.h"

using namespace cv;
using namespace detector;
using namespace std;

namespace {

void StoreEqualizedHistogram(const Mat& image, const string& container_path);

}  // namespace

int main(int argc, const char** argv) {
    vector<string> files = GetAllFiles("../freebase/data");
    for (const string& container_path : files) {
        string image_path;
        if (GetContainerImagePath(container_path, image_path)) {
            Mat image = imread(image_path);
            StoreEqualizedHistogram(image, container_path);
            // TODO(sghiaus): Compute additional "hashes" for this image.
        }
    }
    return 0;
}

namespace {

void StoreEqualizedHistogram(const Mat& image, const string& container_path) {
    Mat equalized_image;
    cvtColor(image, equalized_image, CV_BGR2GRAY);
    EqualizeHistogram(equalized_image);
    imwrite(container_path + "/image_equalized_gray.png", equalized_image);
}

}  // namespace

