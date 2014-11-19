#include <cassert>
#include <cstdlib>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "server/detector/edges.h"
#include "server/detector/features.h"
#include "server/detector/geometry.h"
#include "server/detector/util.h"
#include "third_party/rapidjson/document.h"
#include "third_party/rapidjson/stringbuffer.h"
#include "third_party/rapidjson/writer.h"

using namespace cv;
using namespace detector;
using namespace rapidjson;
using namespace std;

namespace {

// The input image will be scaled down while keeping the aspect ratio, so that 
// max(width, height) = 512.
const int MAX_IMAGE_EDGE_PIXELS = 512;

const int ERR_INVALID_ARGC = 1;
const int ERR_FAILED_TO_OPEN_IMAGE = 2;
const int ERR_MISSING_TEMPLATE_IMAGE = 3;

#ifdef DEBUG
string debug_container_path;
#endif

Mat ReadImage(const string& path);
Mat ExtractPainting(const Mat& image, int x, int y);

} // namespace

// Usage: "./detector <container_path> <database_path> <hit_x> <hit_y>"
int main(int argc, const char** argv) {
    // Read arguments.
    if (argc != 5) {
        cerr << "Invalid number of arguments (" << argc - 1 << ").";
        return ERR_INVALID_ARGC;
    }
    string container_path = argv[1];
    string database_path = argv[2];
    // TODO(sghiaus): Scale hit x and y before using them.
    int hit_x = stoi(argv[3]);
    int hit_y = stoi(argv[4]);
    string input_image_path = container_path + "image.png";

#ifdef DEBUG
    debug_container_path = container_path;
#endif

    Mat input_image = ReadImage(input_image_path);

    Mat painting_image = ExtractPainting(input_image, hit_x, hit_y);

#ifdef DEBUG
    imwrite(debug_container_path + "painting.png", painting_image);
#endif

    vector<string> files = GetAbsoluteFilesIn(database_path);
    SizeType best_index = files.size();
    double closest_distance = 0;

    for (size_t i = 0; i < files.size(); ++i) {
        string image_path;
        if (GetContainerImagePath(files[i], image_path)) {
            double distance = ComputeFeatureDistance(painting_image, image_path);
            if (distance >= 0) {
                if (distance < closest_distance || best_index == files.size()) {
                    closest_distance = distance;
                    best_index = i;
                }
            }
        } else {
            cerr << "Missing image for " << files[i];
            return ERR_MISSING_TEMPLATE_IMAGE;
        }
    }

    // TODO(sghiaus): A minimum threshold should be set.
    if (best_index < files.size()) {
        fstream file_stream(files[best_index] + "/metadata.json", fstream::in);
        stringstream raw_metadata;
        raw_metadata << file_stream.rdbuf();
        file_stream.close();
        Document metadata;
        metadata.Parse(raw_metadata.str().c_str());

        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        writer.StartObject();
        writer.String("name");
        writer.String(metadata["name"].GetString());
        writer.String("description");
        writer.String(metadata["description"].GetString());
        // TODO(sghiaus): Add the image URL. Should it be the original freebase URL
        // or should it be hosted on our server?
        writer.EndObject();

        cout << buffer.GetString();
    }
    
    return 0;
}

namespace {

Mat ReadImage(const string& path) {
    Mat image = imread(path);
    if (!image.data) {
        cerr << "Error loading image: " << path << "\n";
        exit(ERR_FAILED_TO_OPEN_IMAGE);
    }
    return image;
}

Mat ExtractPainting(const Mat& image, int hit_x, int hit_y) {
#ifndef DEBUG
    EdgeDetector edge_detector;
#else
    EdgeDetector edge_detector(debug_container_path);
#endif

    Mat gray_image;
    cvtColor(image, gray_image, CV_BGR2GRAY);

#ifdef DEBUG
    imwrite(debug_container_path + "grayscale.png", gray_image);
#endif

    Quad2f painting_quad;
    if (edge_detector.DetectPaintingQuad(gray_image, hit_x, hit_y, painting_quad)) {
        // TODO(sghiaus): Apply perspective transform on the rect and compare the result
        // with the known templates.
    }

    // TODO(sghiaus): Return the painting warped image instead of the gray input image.
    Mat resized = RescaleImage(gray_image, 512);

    return resized;
}

} // namespace

