from django.contrib import admin
from . import models

admin.site.register(models.Package)
admin.site.register(models.Transit)
admin.site.register(models.TransitUpdate)
