from django.db import models

# Create your models here.

class Circuit(models.Model):
    """docstring for Circuit"""
    
    title           =  models.CharField(max_length=50)
    result_image    =  models.FileField()