import imghdr
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
        'limit': str(BATCH_SIZE),
        'output' : '(/common/topic/image /common/topic/description)',
        'lang' : 'en',
}

# Perform query.
remaining_requests = num_requests
print("Starting download.")
print("Remaining: " + str(remaining_requests))
cursor = 0

while remaining_requests > 0:
    print('Starting new batch request.')
    if remaining_requests > BATCH_SIZE:
        current_size = BATCH_SIZE
    else:
        current_size = remaining_requests

    params['cursor'] = str(cursor)

    url = SERVICE_URL + '?' + urllib.urlencode(params)
    batch_response = json.loads(urllib.urlopen(url).read())

    for result in batch_response['result']:
        mid = result['mid']
        try:
            image_mid = result['output']['/common/topic/image']['/common/topic/image'][0]['mid']            
            name = result['name']
            description = result['output']['/common/topic/description']['/common/topic/description'][0]
        except:
            print('Mid', mid, 'has incomplete metadata. Skipping.')
            continue

        # Create container folder.
        mid_folder = CONTAINER_FOLDER + mid[3:] + '/'  # Removing the /m/ prefix.
        if not os.path.exists(mid_folder):
            os.makedirs(mid_folder)

        # Write metadata.
        metadata = dict()
        metadata['name'] = name
        metadata['description'] = description
        with open(mid_folder + 'metadata.json', 'w') as outfile:
            json.dump(metadata, outfile)

        # Download image.
        image_mid = result['output']['/common/topic/image']['/common/topic/image'][0]['mid']
        image_params = {
                'maxwidth' : '512',
                'maxheight' : '512'
        }
        image_url = IMAGE_BASE_URL + image_mid + '?' + urllib.urlencode(image_params)
        image_save_path = mid_folder + 'image'
        urllib.urlretrieve(image_url, image_save_path)

        # Add image extension.
        extension = imghdr.what(image_save_path)
        os.rename(image_save_path, image_save_path + '.' + extension)

        remaining_requests -= 1
        cursor += 1
        if remaining_requests != 0:
            print("Remaining: " + str(remaining_requests))
        else:
            break

