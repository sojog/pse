from django.test import TestCase
from django.test.client import Client

class SimpleTest(TestCase):
    def setUp(self):
        self.client = Client()
        
    def test_empty_painting_detection(self):
        # TODO(sghiaus): Make this path relative to the current file's location.
        starry_night = open("detection_app/testdata/StarryNightPhoto.png", "rb")
        response = self.client.post('/identify_painting/', {
            'image': starry_night,
            'x': '0',
            'y': '0',
        })
        self.assertEqual(200, response.status_code)        

        # TODO(sghiaus): Also verify the result content.

