from django.conf.urls import patterns, include, url

# Uncomment the next two lines to enable the admin:
from django.contrib import admin
admin.autodiscover()

urlpatterns = patterns('',
    # Identifies the painting in the given image, at the target (x, y) location.
    # GET parameters:
    #   image: a file containing the image.
    #   x: pixel location on the x axis (pointing rightward)
    #   y: pixel location on the y axis (pointing downward)
    # Reponse:
    #   TODO(sghiaus): Decide response format. JSON? Protobuffers?
    # TODO(sghiaus): Decide which format the image should be in (.png?).
    url(r'^identify_painting/', 'detection_app.views.identify_painting'),

    # Django admin URLs.
    url(r'^admin/doc/', include('django.contrib.admindocs.urls')),
    url(r'^admin/', include(admin.site.urls)),
)
