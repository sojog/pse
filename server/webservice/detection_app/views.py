import imp

from django.http import HttpResponse, HttpResponseServerError, HttpResponseForbidden
from webservice.settings import DETECTOR_PATH

detector = imp.load_source('detector', DETECTOR_PATH)

def identify_painting(request):
    if request.method != 'GET':
        return log_bad_request_and_return(request, 'request is not GET')

    if 'image' in request.GET:
        image = request.GET['image']
    else:
        return HttpResponseServerError('Parameter \'image\' missing from request.')

    if 'x' in request.GET:
        x = int(request.GET['x'])
    else:
        return HttpResponseServerError('Parameter \'x\' missing from request.')

    if 'y' in request.GET:
        y = int(request.GET['y'])
    else:
        return HttpResponseServerError('Parameter \'y\' missing from request.')

    contents = detector.detect_painting(image, x, y)
    # TODO(sghiaus): Should we return something different when no painting is detected?
    response = HttpResponse(contents)
    response['Content-Length'] = len(contents)

    return response