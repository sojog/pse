import cv2

MIN_MATCH_COUNT = 0

BF = 0
FLANN = 1

def match_features_with_best_template(image_path, template_paths, algorithm = BF):
    return _match(image_path, template_paths, algorithm)

def _flann_match(image_des, template_des):
    FLANN_INDEX_LSH = 6
    flann_params = dict(algorithm = FLANN_INDEX_LSH,
                        table_number = 6,
                        key_size = 12,
                        multi_probe_level = 1)
    flann = cv2.FlannBasedMatcher(flann_params, {})
    matches = flann.knnMatch(image_des, template_des, k = 2)

    good = []
    for match in matches:
        assert(len(match) == 2)
        if match[0].distance < match[1].distance * 0.75:
            good.append(match)

    return good
    

def _brute_force_match(image_des, template_des):
    bf = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=True)
    return bf.knnMatch(image_des, template_des, k = 1)

def _match(image_path, template_paths, algorithm):
    best_candidate = None
    best_candidate_score = 0

    orb = cv2.ORB(nfeatures = 1000)
    image = cv2.imread(image_path)
    image_kp, image_des = orb.detectAndCompute(image, None)

    for template_path in template_paths:
        template = cv2.imread(template_path)

        template_kp, template_des = orb.detectAndCompute(template, None)
        if algorithm == FLANN:
            matches = _flann_match(image_des, template_des)
        elif algorithm == BF:
            matches = _brute_force_match(image_des, template_des)
        
        score = len(matches)
        if score > best_candidate_score and score > MIN_MATCH_COUNT:
            best_candidate = template_path
            best_candidate_score = score

    return best_candidate
