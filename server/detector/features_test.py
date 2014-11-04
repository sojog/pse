import unittest

from features import _brute_force_match
from features import _flann_match
from features import match_features_with_best_template

class TestPaintingBinaryChoice(unittest.TestCase):

    def setUp(self):
        self.mona_lisa_image = "testdata/MonaLisaPhoto.jpg"
        self.starry_night_image = "testdata/StarryNightPhoto.jpg"
        self.the_scream_image = "testdata/TheScreamPhoto.jpg"
        self.mona_lisa_template = "testdata/MonaLisaTemplate.png"
        self.starry_night_template = "testdata/StarryNightTemplate.jpg"
        self.templates = [self.mona_lisa_template,
                          self.starry_night_template]

    def test_flann_selects_mona_lisa(self):
        template = _flann_match(self.mona_lisa_image, self.templates)
        self.assertEqual(self.mona_lisa_template, template)

    def test_flann_selects_starry_night(self):
        template = _flann_match(self.starry_night_image, self.templates)
        self.assertEqual(self.starry_night_template, template)

    def test_bf_selects_mona_lisa(self):
        template = _brute_force_match(self.mona_lisa_image, self.templates)
        self.assertEqual(self.mona_lisa_template, template)

    def test_bf_selects_starry_night(self):
        template = _brute_force_match(self.starry_night_image, self.templates)
        self.assertEqual(self.starry_night_template, template)

"""
    def test_flann_fails_for_painting_without_template(self):
        template = _flann_match(self.the_scream_image, self.templates)
        self.assertEqual(None, template)
"""

if __name__ == '__main__':
    unittest.main()
