from django.shortcuts import render
from django.http import JsonResponse
from django.contrib.auth import authenticate, login
from django.contrib.auth.hashers import make_password
from .models import Employee

def employee_signup(request):
    if request.method == 'POST':
        employee_id = request.POST.get('employee_id')
        password = request.POST.get('password')
        confirm_password = request.POST.get('confirm_password')

        
        if not employee_id:
            return JsonResponse({'success': False, 'errors': 'Employee ID is required'}, status=400)
        if not password:
            return JsonResponse({'success': False, 'errors': 'Password is required'}, status=400)
        if password != confirm_password:
            return JsonResponse({'success': False, 'errors': 'Passwords do not match'}, status=400)
        if Employee.objects.filter(employee_id=employee_id).exists():
            return JsonResponse({'success': False, 'errors': 'Employee ID already exists'}, status=400)

        try:
            user = Employee.objects.create(
                employee_id=employee_id,
                password=make_password(password)
            )
            login(request, user)
            return JsonResponse({
                'success': True,
                'redirect': '/dashboard/'
            })
        except Exception as e:
            return JsonResponse({'success': False, 'errors': str(e)}, status=500)

    return JsonResponse({'success': False, 'errors': 'Invalid request method'}, status=405)

def employee_login(request):
    if request.method == 'POST':
        employee_id = request.POST.get('employee_id')
        password = request.POST.get('password')
        
        user = authenticate(request, employee_id=employee_id, password=password)
        
        if user is not None:
            login(request, user)
            return JsonResponse({
                'redirect': '/dashboard/'
            })
        else:
            return JsonResponse({
                'errors': 'Invalid credentials'
            }, status=400)