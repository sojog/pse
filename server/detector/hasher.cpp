#include "server/detector/util.h"

using namespace detector;
using namespace std;

int main(int argc, const char** argv) {
    vector<string> files = GetAllFiles("../freebase/data");
    for (const string& file : files) {
        string image_path;
        if (GetContainerImagePath(file, image_path)) {
            // TODO(sghiaus): Compute hashes for this image.
        }
    }
    return 0;
}

