from django.urls import path
from . import views

urlpatterns = [
    path('login/', views.employee_login, name='login'),
    path('signup/', views.employee_signup, name='signup'),
]