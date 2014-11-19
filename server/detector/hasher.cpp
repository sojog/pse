#include <string>

#include "server/detector/util.h"

using namespace cv;
using namespace detector;
using namespace std;

int main(int argc, const char** argv) {
    vector<string> files = GetAllFiles("../freebase/data");
    for (const string& container_path : files) {
        string image_path;
        if (GetContainerImagePath(container_path, image_path)) {
        }
    }
    return 0;
}

}  // namespace
