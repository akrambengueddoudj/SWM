from django.db import models
from django.contrib.auth.models import AbstractUser

class Employee(AbstractUser):
    employee_id = models.CharField(max_length=25, unique=True)
    
    USERNAME_FIELD = 'employee_id'
    REQUIRED_FIELDS = []

    username = None

    def __str__(self):
        return self.employee_id
