from django.db import models
from django.conf import settings
from django.core.files import File

import os
from subprocess import call


# Create your models here.

class Circuit(models.Model):
    """docstring for Circuit"""
    title           =  models.CharField(max_length=50)    
    start_image     =  models.FileField()
    result_image    =  models.FileField()

    def process(self):        
        base = settings.BASE_DIR
        executable = os.path.join(os.path.dirname(base), "EXE", "main")

        dirname =  os.path.dirname(self.start_image.path)
        out_file = os.path.abspath(os.path.join(dirname, "result_tmp_{}.png".format(self.pk)))
        
        command_string = "{} {} {}".format(executable, os.path.abspath(self.start_image.path), out_file)
        call(command_string.split())

        f = open(out_file, 'rb')
        myfile = File(f)
        self.result_image.save("result_{}.png", myfile)

