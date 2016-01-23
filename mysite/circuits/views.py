from django.shortcuts import render
from django.http import HttpResponse


def upload(request):
    return render(request, 'circuits/upload.html', {})

def result(request):
    return render(request, 'circuits/result.html', {})