from django.shortcuts import render
from robot.models import Transit
from django.shortcuts import redirect

def custom_login_required(view_func):
    def wrapper(request, *args, **kwargs):
        if not request.user.is_authenticated:
            return redirect("/authenticate/")
        return view_func(request, *args, **kwargs)
    return wrapper

def index(request):
    return render(request, 'index.html')

@custom_login_required
def dashboard(request):
    return render(request, 'dashboard.html')

@custom_login_required
def management(request):
    return render(request, 'management.html')

@custom_login_required
def tracking(request):
    transits = Transit.objects.filter(transit_status=2)
    return render(request, 'tracking.html', {'transits': transits})

@custom_login_required
def reports(request):
    return render(request, 'reports.html')

def sign(request):
    return render(request, 'sign.html')