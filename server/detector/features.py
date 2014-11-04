import cv2

MIN_MATCH_COUNT = 10

def match_features_with_best_template(image_path, template_paths):
    return _brute_force_match(image_path, template_paths)

def _flann_match(image_path, template_paths):
    FLANN_INDEX_LSH = 6
    flann_params = dict(algorithm = FLANN_INDEX_LSH,
                        table_number = 6,
                        key_size = 12,
                        multi_probe_level = 1)
    flann = cv2.FlannBasedMatcher(flann_params, {})
    return _match(image_path, template_paths, flann)

def _brute_force_match(image_path, template_paths):
    bf = cv2.BFMatcher()
    return _match(image_path, template_paths, bf)

def _match(image_path, template_paths, matcher):
    best_candidate = None
    best_candidate_score = 0

    orb = cv2.ORB(nfeatures = 1000)
    image = cv2.imread(image_path)
    image_kp, image_des = orb.detectAndCompute(image, None)

    for template_path in template_paths:
        template = cv2.imread(template_path)

        template_kp, template_des = orb.detectAndCompute(template, None)
        matches = matcher.knnMatch(image_des, template_des, k = 2)

        good = [m[0] for m in matches if len(m) == 2 and m[0].distance < m[1].distance * 0.75]

        score = len(good)
        if score > best_candidate_score and score > MIN_MATCH_COUNT:
            best_candidate = template_path
            best_candidate_score = score

    return best_candidate
