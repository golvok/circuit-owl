from django.conf.urls import url
from . import views

app_name = 'circuits'
urlpatterns = [
    url(r'^upload$', views.upload, name='upload'),
    url(r'^result/(?P<result_id>[0-9]+)/$', views.result, name='result'),
]