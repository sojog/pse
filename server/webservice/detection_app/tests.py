import json

from django.test import TestCase
from django.test.client import Client

class SimpleTest(TestCase):
    def setUp(self):
        self.client = Client()

    def test_mona_lisa_1(self):
        # TODO(sghiaus): Change test images to png.
        mona_lisa_1 = open("detection_app/testdata/MonaLisa1.png", "rb")
        mona_lisa_1_response = self.client.post('/identify_painting/', {
            'image': mona_lisa_1,
            'x': '2000',
            'y': '1350',
        })
        self.assertEqual(200, mona_lisa_1_response.status_code)
        mona_lisa_1_response_json = json.loads(mona_lisa_1_response.content)
        self.assertEqual(u'Mona Lisa', mona_lisa_1_response_json['name'])

    def test_mona_lisa_2(self):
        mona_lisa_2 = open("detection_app/testdata/MonaLisa2.png", "rb")
        mona_lisa_2_response = self.client.post('/identify_painting/', {
            'image': mona_lisa_2,
            'x': '1200',
            'y': '1400',
        })
        self.assertEqual(200, mona_lisa_2_response.status_code)
        mona_lisa_2_response_json = json.loads(mona_lisa_2_response.content)
        self.assertEqual(u'MonaLisa', mona_lisa_2_response_json['name'])
        
    def test_paintin_starry_night(self):
        starry_night = open("detection_app/testdata/StarryNightPhoto.png", "rb")
        starry_night_response = self.client.post('/identify_painting/', {
            'image': starry_night,
            'x': '750',
            'y': '450',
        })
        self.assertEqual(200, starry_night_response.status_code)
        starry_night_response_json = json.loads(starry_night_response.content)
        self.assertEqual(u'The Starry Night', starry_night_response_json['name'])

