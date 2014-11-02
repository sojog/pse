import cv2

# Returns a JSON file that contains information about the painting,
# or None if the painting can't be identified.
def detect_painting(image, x, y):
    # TODO(sghiaus): Implement the function.
    return ''

MIN_MATCH_COUNT = 10

def detect_painting_by_features(image_path, template_paths):
    best_candidate = None
    best_candidate_score = 0

    orb = cv2.ORB(nfeatures = 1000)
    image = cv2.imread(image_path)
    image_kp, image_des = orb.detectAndCompute(image, None)

    for template_path in template_paths:
        template = cv2.imread(template_path)
        template_kp, template_des = orb.detectAndCompute(template, None)

        FLANN_INDEX_LSH = 6
        flann_params = dict(algorithm = FLANN_INDEX_LSH,
                            table_number = 6,
                            key_size = 12,
                            multi_probe_level = 1)

        # search_params = dict(checks = 80)
        flann = cv2.FlannBasedMatcher(flann_params, {})

        matches = flann.knnMatch(image_des, template_des, k = 2)
        good = [m[0] for m in matches if len(m) == 2 and m[0].distance < m[1].distance * 0.75]

        score = len(good)

        if score > best_candidate_score and score > MIN_MATCH_COUNT:
            best_candidate = template_path
            best_candidate_score = score

    return best_candidate
