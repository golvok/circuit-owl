from django.shortcuts import redirect, render
from django.core.urlresolvers import reverse
from django.http import HttpResponse

from .forms import UploadFileForm
from .models import Circuit


def result(request, result_id):
    circuit = Circuit.objects.get(pk=result_id)
    return render(request, 'circuits/result.html', {'circuit':circuit})

def upload(request):
    if request.method == 'POST':
        form = UploadFileForm(request.POST, request.FILES)
        if form.is_valid():
            print(request.FILES.keys())
            instance = Circuit(result_image=request.FILES['file_field'])
            instance.save()
            return redirect('result', instance.pk)
    else:
        form = UploadFileForm()
    return render(request, 'circuits/upload.html', {'form': form})