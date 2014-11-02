from django.test import TestCase
from django.test.client import Client

class SimpleTest(TestCase):
    def setUp(self):
        self.client = Client()
        
    def test_empty_painting_detection(self):
        response = self.client.get('/identify_painting/', {
            'image': '',
            'x': '0',
            'y': '0',
        })
        self.assertEqual(200, response.status_code)        
        self.assertEqual("", response.content)

