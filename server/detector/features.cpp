#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "server/detector/features.h"

using namespace cv;
using namespace std;

namespace detector {

// TODO(sghiaus): Rename this to ComputeSimilarity to reflect that the number of
// matches is also taken into account and not just the distance.
double ComputeFeatureDistance(const Mat& input_image,
                              const string& template_path) {
    Mat template_image = imread(template_path, CV_LOAD_IMAGE_GRAYSCALE);

    // Detect keypoints.
    SurfFeatureDetector detector(400);
    vector<KeyPoint> input_kps, template_kps;
    detector.detect(input_image, input_kps);
    detector.detect(template_image, template_kps);

    // Calculate descriptors.
    SurfDescriptorExtractor extractor;
    Mat input_des, template_des;
    extractor.compute(input_image, input_kps, input_des);
    extractor.compute(template_image, template_kps, template_des);

    // Perform matching.
    BFMatcher matcher(NORM_L2, true);
    vector<DMatch> matches;
    matcher.match(input_des, template_des, matches);

    vector<DMatch> good_matches;
    for (const DMatch& match : matches) {
        // TODO(sghiaus): Need a better way than a hard coded threhsold.
        if (match.distance < 0.22) {
            good_matches.push_back(match);
        }
    }

    // TODO(sghiaus): Need to find a homography for these points.

#ifdef DEBUG
    Mat matches_image;
    drawMatches(input_image, input_kps, template_image, template_kps,
                good_matches, matches_image);

    imwrite(template_path + ".matches.png", matches_image);
#endif

    double normalized_distance = -1;

    if (good_matches.size() > 0) {
        double total_distance = 0;
        for (int i = 0; i < good_matches.size(); ++i) {
            total_distance += good_matches[i].distance;
        }
        // TODO(sghiaus): The more features matched, the higher the score should be,
        // rather than just normalizing the distance.
        normalized_distance = total_distance / good_matches.size();
    }

    return normalized_distance;
}

}  // namespace detector
