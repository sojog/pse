import unittest

from detector import detect_painting_by_features

class TestPaintingBinaryChoice(unittest.TestCase):

    def setUp(self):
        self.mona_lisa_image = "testdata/MonaLisaPhoto.jpg"
        self.starry_night_image = "testdata/StarryNightPhoto.jpg"
        self.the_scream_image = "testdata/TheScreamPhoto.jpg"
        self.mona_lisa_template = "testdata/MonaLisaTemplate.png"
        self.starry_night_template = "testdata/StarryNightTemplate.jpg"
        self.templates = [self.mona_lisa_template,
                          self.starry_night_template]

    def test_selects_mona_lisa(self):
        template = detect_painting_by_features(self.mona_lisa_image, self.templates)
        self.assertEqual(self.mona_lisa_template, template)

    def test_selects_starry_night(self):
        template = detect_painting_by_features(self.starry_night_image, self.templates)
        self.assertEqual(self.starry_night_template, template)

    def test_fails_for_painting_without_template(self):
        template = detect_painting_by_features(self.the_scream_image, self.templates)
        self.assertEqual(None, template)

if __name__ == '__main__':
    unittest.main()
