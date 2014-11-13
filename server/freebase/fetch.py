import json
import os
import sys
import urllib

# TODO(sghiaus): All paths should be relative to this file's location, rather than
# invocation  folder.
# Read api key.
api_key_file = open('key', 'r')
api_key = api_key_file.read().rstrip()

# Parse args.
args = sys.argv
num_requests = int(args[1])

BATCH_SIZE = 100
SERVICE_URL = 'https://www.googleapis.com/freebase/v1/search'
IMAGE_BASE_URL = 'https://usercontent.googleapis.com/freebase/v1/image'
CONTAINER_FOLDER = 'data/'
params = {
        'filter': '(all type:/visual_art/artwork /visual_art/artwork/art_form:"Painting")',
        'key': api_key,
        'limit': '100',
        'output' : '(/common/topic/image /common/topic/description)',
        'lang' : 'en'
}

# Perform query.
remaining_requests = num_requests
print("Starting download.")
print("Remaining: " + str(remaining_requests))
while remaining_requests > 0:
    if remaining_requests > BATCH_SIZE:
        current_size = BATCH_SIZE
    else:
        current_size = remaining_requests

    params['limit'] = str(current_size)
    url = SERVICE_URL + '?' + urllib.urlencode(params)
    batch_response = json.loads(urllib.urlopen(url).read())

    for result in batch_response['result']:
        mid = result['mid']

        # Create container folder.
        mid_folder = CONTAINER_FOLDER + mid[3:] + '/'  # Removing the /m/ prefix.
        if not os.path.exists(mid_folder):
            os.makedirs(mid_folder)

        # Read metadata.
        metadata = dict()
        metadata['name'] = result['name']
        metadata['description'] = result['output']['/common/topic/description']['/common/topic/description'][0]

        # Write metadata.
        with open(mid_folder + 'metadata.json', 'w') as outfile:
            json.dump(metadata, outfile)

        # Download image.
        image_mid = result['output']['/common/topic/image']['/common/topic/image'][0]['mid']
        image_params = {
                'maxwidth' : '512',
                'maxheight' : '512'
        }
        image_url = IMAGE_BASE_URL + image_mid + '?' + urllib.urlencode(image_params)
        image_save_path = mid_folder + 'image.png'
        urllib.urlretrieve(image_url, image_save_path)

        remaining_requests -= 1
        if remaining_requests != 0:
            print("Remaining: " + str(remaining_requests))
