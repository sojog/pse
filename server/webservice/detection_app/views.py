import os
import subprocess
import time

from django.http import HttpResponse, HttpResponseServerError, HttpResponseForbidden
from django.views.decorators.csrf import csrf_exempt, csrf_protect
from webservice.settings import DETECTOR_PATH

# TODO(sghiaus): Use Django forms to manage the request.
@csrf_exempt
def identify_painting(request):
    if request.method != 'POST':
        return HttpResponseServerError('Request is not POST')

    if 'image' in request.FILES:
        image = request.FILES['image']
    else:
        return HttpResponseServerError('File \'image\' missing from request.')

    if 'x' in request.POST:
        x = round(float(request.POST['x']))
    else:
        return HttpResponseServerError('Parameter \'x\' missing from request.')

    if 'y' in request.POST:
        y = round(float(request.POST['y']))
    else:
        return HttpResponseServerError('Parameter \'y\' missing from request.')

    # TODO(sghiaus): Should use something better than a timestamp.
    request_temp_container = "detection_app/temp/" + str(int(time.time())) + "/"
    os.makedirs(request_temp_container)
    image_path = request_temp_container + "image.png"
    image_file = open(image_path, "wb")
    image_file.write(image.read())
    image_file.close()

    database_path = "../detector/data"
    abs_database_path = os.path.abspath(database_path)
    abs_container_path = os.path.abspath(request_temp_container) + "/"
    content = subprocess.check_output([DETECTOR_PATH, abs_container_path, abs_database_path, str(x), str(y)])

    response = HttpResponse(content)
    response['Content-Length'] = len(content)

    return response
