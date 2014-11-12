#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "server/detector/edges.h"
#include "server/detector/feature_matcher.h"
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

#ifdef DEBUG
string debug_container_path;
#endif

Mat ReadImage(const string& path, int& hit_x, int& hit_y);
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
    int hit_x = stoi(argv[3]);
    int hit_y = stoi(argv[4]);
    string input_image_path = container_path + "image.png";

#ifdef DEBUG
    debug_container_path = container_path;
#endif

    // Read the input image.
    Mat input_image = ReadImage(input_image_path, hit_x, hit_y);
    if (!input_image.data) {
        cerr << "Error loading image: " << input_image_path << "\n";
        return ERR_FAILED_TO_OPEN_IMAGE;
    }
    Mat gray_input_image;
    cvtColor(input_image, gray_input_image, CV_BGR2GRAY);

    // Process the input image to only keep the painting quadrilateral and transform
    // it to a rectangle.
    // TODO(sghiaus): Use this image for feature extraction.
    Mat gray_painting_image = ExtractPainting(gray_input_image, hit_x, hit_y);

#ifdef DEBUG
    imwrite(debug_container_path + "scaled.png", input_image);
    imwrite(debug_container_path + "gray_scaled.png", gray_input_image);
    imwrite(debug_container_path + "gray_painting.png", gray_painting_image);
#endif
    
    // Read the json database.
    fstream file_stream;
    file_stream.open(database_path + "/data.json", fstream::in);
    stringstream database;
    database << file_stream.rdbuf();
    file_stream.close();

    Document document;
    document.Parse(database.str().c_str());

    SizeType best_index = 0;
    int best_score = 0;

    for (SizeType i = 0; i < document.Size(); ++i) {
        Value& entry = document[i];
        string template_path = database_path + "/" + entry["template"].GetString();
        int score = ComputeFeatureMatchScore(gray_input_image, template_path);
        if (score > best_score) {
            best_score = score;
            best_index = i;
        }
    }

    if (best_score > 0) {
        Value& match = document[best_index];

        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);

        writer.StartObject();
        writer.String("name");
        writer.String(match["name"].GetString());
        writer.String("description");
        writer.String(match["description"].GetString());
        writer.String("image");
        writer.String(match["image"].GetString());
        writer.EndObject();

        cout << buffer.GetString();
    }
    
    return 0;
}

namespace {

Mat ReadImage(const string& path, int& hit_x, int& hit_y) {
    Mat raw_image = imread(path);
    if (raw_image.data) {
        Size raw_image_size = raw_image.size();
        auto max_edge = max(raw_image_size.width, raw_image_size.height);
        if (max_edge > MAX_IMAGE_EDGE_PIXELS) {
            float scale_ratio = MAX_IMAGE_EDGE_PIXELS / (float) max_edge;
            Size final_size(raw_image_size.width * scale_ratio,
                            raw_image_size.height * scale_ratio);
            hit_x *= scale_ratio;
            hit_y *= scale_ratio;
            Mat scaled_image;
            resize(raw_image, scaled_image, final_size);
            return scaled_image;
        }
    }
    return raw_image;
}

Mat ExtractPainting(const Mat& image, int hit_x, int hit_y) {
    Mat painting;

#ifndef DEBUG
    EdgeDetector edge_detector;
#else
    EdgeDetector edge_detector(debug_container_path);
#endif

    Rect painting_edges = edge_detector.DetectPaintingRect(image, hit_x, hit_y);

    // TODO(sghiaus): Apply perspective transform on the rect and compare the result
    // with the known templates.

    return painting;
}

} // namespace

