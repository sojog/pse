#include <algorithm>
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
#include "server/detector/feature_matcher.h"
#include "server/detector/geometry.h"
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

Mat ReadImage(const string& path, int& hit_x, int& hit_y);
Mat ExtractPainting(const Mat& image, int x, int y);
Mat ReadHue(const Mat& image);

// TODO(sghiaus): Move this to some utils file.
vector<string> GetAllFiles(const string& path);

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
    Mat gray_input_image;
    cvtColor(input_image, gray_input_image, CV_BGR2GRAY);
    Mat hue_input_image = ReadHue(input_image);

#ifdef DEBUG
    // Process the input image to only keep the painting quadrilateral and transform
    // it to a rectangle.
    // TODO(sghiaus): Once the function is ready, use this image for feature extraction.
    Mat gray_painting_image = ExtractPainting(gray_input_image, hit_x, hit_y);
    imwrite(debug_container_path + "scaled.png", input_image);
    imwrite(debug_container_path + "gray_scaled.png", gray_input_image);
    imwrite(debug_container_path + "hue.png", hue_input_image);
    // imwrite(debug_container_path + "gray_painting.png", gray_painting_image);
#endif

    SizeType best_index = 0;
    int best_score = 0;

    vector<string> files = GetAllFiles(database_path);
    for (size_t i = 0; i < files.size(); ++i) {
        string file = files[i];
        DIR* folder = opendir(file.c_str());
        if (folder) {
            // Check if it's a png.
            string image_path;
            ifstream png(file + "/image.png");
            if (png) {
                image_path = file + "/image.png";
            } else {
                ifstream jpeg(file + "/image.jpeg");
                if (jpeg) {
                    image_path = file + "/image.jpeg";
                } else {
                    cerr << "Missing image for " << file;
                    return ERR_MISSING_TEMPLATE_IMAGE;
                }
            }
            int score = ComputeFeatureMatchScore(gray_input_image, image_path);
            if (score > best_score) {
                best_score = score;
                best_index = i;
            }
            closedir(folder);
        }
    }

    if (best_score > 0) {
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
    } else {
        cerr << "Error loading image: " << path << "\n";
        exit(ERR_FAILED_TO_OPEN_IMAGE);
    }
    return raw_image;
}

Mat ReadHue(const Mat& image) {
    Mat hue;
    vector<Mat> channels;
    Mat hsv;
    cvtColor(image, hsv, CV_RGB2HSV);
    split(hsv, channels);
    hue = channels[0];
    return hue;
}

Mat ExtractPainting(const Mat& image, int hit_x, int hit_y) {
    Mat painting;

#ifndef DEBUG
    EdgeDetector edge_detector;
#else
    EdgeDetector edge_detector(debug_container_path);
#endif

    Quad2f painting_quad;
    if (edge_detector.DetectPaintingQuad(image, hit_x, hit_y, painting_quad)) {
        // TODO(sghiaus): Apply perspective transform on the rect and compare the result
        // with the known templates.
    }

    return painting;
}

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

} // namespace

