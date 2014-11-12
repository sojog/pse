#include <iostream>
#include <stdio.h>

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "server/detector/feature_matcher.h"

using namespace cv;
using namespace std;

namespace detector {

// TODO(sghiaus): Also scale down the images so the computation is faster.
// TODO(sghiaus): Template features can be precomputed and input image features
// should only be computed once.
int ComputeFeatureMatchScore(const Mat& input_image,
                             const string& template_path) {
    Mat template_image = imread(template_path, CV_LOAD_IMAGE_GRAYSCALE);

    // Using SURF.
    // Detect keypoints.
    int minHessian = 400;
    SurfFeatureDetector detector(minHessian);
    vector<KeyPoint> input_kps, template_kps;
    detector.detect(input_image, input_kps);
    detector.detect(template_image, template_kps);

    // Calculate descriptors.
    SurfDescriptorExtractor extractor;
    Mat input_des, template_des;
    extractor.compute(input_image, input_kps, input_des);
    extractor.compute(template_image, template_kps, template_des);

    // Perform matching.
    FlannBasedMatcher matcher;
    vector<DMatch> matches;
    matcher.match(input_des, template_des, matches);

    double max_dist = 0; double min_dist = 100;

    for (int i = 0; i < input_des.rows; i++) {
        double dist = matches[i].distance;
        if (dist < min_dist) min_dist = dist;
        if (dist > max_dist) max_dist = dist;
    }


    // Keep only "good matches".
    vector<DMatch> good_matches;

    for (int i = 0; i < input_des.rows; i++) {
        if (matches[i].distance <= max(2 * min_dist, 0.02)) {
            good_matches.push_back(matches[i]);
        }
    }

    return good_matches.size();
}

}  // namespace detector
