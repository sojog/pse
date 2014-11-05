import os
import subprocess
import time

from django.http import HttpResponse, HttpResponseServerError, HttpResponseForbidden
from webservice.settings import DETECTOR_PATH

# TODO(sghiaus): Use Django forms to manage the upload.
def identify_painting(request):
    if request.method != 'POST':
        return log_bad_request_and_return(request, 'request is not POST')

    if 'image' in request.FILES:
        image = request.FILES['image']
    else:
        return HttpResponseServerError('File \'image\' missing from request.')

    if 'x' in request.POST:
        x = int(request.POST['x'])
    else:
        return HttpResponseServerError('Parameter \'x\' missing from request.')

    if 'y' in request.POST:
        y = int(request.POST['y'])
    else:
        return HttpResponseServerError('Parameter \'y\' missing from request.')

    # TODO(sghiaus): Make this path relative to the test file root?
    image_path = "detection_app/temp/image_" + str(int(time.time())) + ".png"
    abs_image_path = os.path.abspath(image_path)
    image_file = open(image_path, "wb")
    image_file.write(image.read())
    image_file.close()

    database_path = "../detector/data/data.json"
    abs_database_path = os.path.abspath(database_path)
    content = subprocess.check_output([DETECTOR_PATH, abs_image_path, abs_database_path])


    response = HttpResponse(content)
    response['Content-Length'] = len(content)

    return response
