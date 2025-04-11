from django.urls import path
from . import views
urlpatterns = [
    path('', views.index, name='home'),
    path('dashboard/', views.dashboard, name='dashboard'),
    path('management/', views.management, name='management'),
    path('tracking/', views.tracking, name='tracking'),
    path('reports/', views.reports, name='reports'),
    path('authenticate/', views.sign, name='authenticate'),
]