from django.urls import path
from . import views

urlpatterns = [
    path('add_package/', views.add_package, name='add_package'),
    path('update_package/', views.update_package, name='update_package'),
    path('get_package/', views.get_package, name='get_package'),
    path('get_packages/', views.get_packages, name='get_packages'),
    path('get_transits/', views.get_transits, name='get_transits'),
]