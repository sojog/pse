#include <string>

#include "server/detector/util.h"

using namespace cv;
using namespace detector;
using namespace std;

namespace {

void HashHistograms(const Mat& image, const string& container_path);
void SaveHistogram(const Mat& image, const string& save_path);

}  // namespace

int main(int argc, const char** argv) {
    vector<string> files = GetAbsoluteFilesIn("../freebase/data");
    for (const string& container_path : files) {
        string image_path;
        if (GetContainerImagePath(container_path, image_path)) {
            Mat image = imread(image_path);
            HashHistograms(image, container_path);
        }
    }
    return 0;
}

namespace {

void HashHistograms(const Mat& image, const string& container_path) {
    Mat hsv;
    cvtColor(image, hsv, COLOR_BGR2HSV);
    vector<Mat> hsv_channels;
    split(image, hsv_channels);

    vector<Mat> bgr_channels;
    split(image, bgr_channels);

    Mat gray_image;
    cvtColor(image, gray_image, COLOR_BGR2GRAY);

    SaveHistogram(hsv_channels[0], container_path + "/histogram_hue");
    SaveHistogram(hsv_channels[1], container_path + "/histogram_sat");
    SaveHistogram(hsv_channels[2], container_path + "/histogram_val");
    SaveHistogram(bgr_channels[0], container_path + "/histogram_blue");
    SaveHistogram(bgr_channels[1], container_path + "/histogram_green");
    SaveHistogram(bgr_channels[2], container_path + "/histogram_red");
    SaveHistogram(gray_image, container_path + "/histogram_gray");
}

void SaveHistogram(const Mat& image, const string& save_path) {
    Mat histogram;
    float range[] = {0, 256};
    const float* hist_ranges[] = {range};
    const int hist_size = 256;

    calcHist(&image, 1, 0, Mat(), histogram, 1, &hist_size, hist_ranges);

#ifdef DEBUG
    // Also plot the histogram.
    int height = 512;
    int width = 512;
    int bin_width = cvRound((double) width / hist_size);

    Mat plot(height, width, CV_8UC3, Scalar(0, 0, 0));

    Mat normalized_for_plot;
    normalize(histogram, normalized_for_plot, 0, plot.rows, NORM_MINMAX);

    for (int i = 1; i < hist_size; ++i) {
        line(plot,
             Point(bin_width * (i - 1), height - cvRound(normalized_for_plot.at<float>(i - 1))),
             Point(bin_width * i, height - cvRound(normalized_for_plot.at<float>(i))),
             Scalar(0, 255, 0), 2, 8, 0 );
    }
    imwrite(save_path + "_plot.png", plot);
#endif

    normalize(histogram, histogram, 0, 1, NORM_MINMAX);

    FileStorage fs(save_path, FileStorage::WRITE);
    fs << "histogram" << histogram;
    fs.release();
}

}  // namespace
