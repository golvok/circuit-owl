from django.shortcuts import redirect, render, render_to_response
from django.core.urlresolvers import reverse
from django.http import HttpResponse, JsonResponse

import json

from .forms import UploadFileForm
from .models import Circuit


def result(request, result_id):
    circuit = Circuit.objects.get(pk=result_id)
    return render(request, 'circuits/result.html', {'circuit':circuit})

def upload(request):
    if request.method == 'POST':
        form = UploadFileForm(request.POST, request.FILES)
        if form.is_valid():
            instance = Circuit(start_image=request.FILES['file'])
            instance.save()
            instance.process()
            instance.save()
            response_data = {'url': reverse('circuits:result', args=(instance.pk,))}
            return JsonResponse(response_data)
    else:
        form = UploadFileForm()
    return render(request, 'circuits/upload.html', {'form': form})