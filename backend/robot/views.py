from . import models
from.serializers import PackageSerializer
from django.http import JsonResponse

from django.http import JsonResponse
from django.utils import timezone
from datetime import timedelta

def add_package(request):
    pkg_id = None
    weight = None
    length = None
    width = None
    height = None
    destination_tag = None
    destination_floor = None
    priority = None

    if request.method == 'POST':
        if 'pkg_id' in request.POST and 'weight' in request.POST and 'length' in request.POST and 'width' in request.POST and 'height' in request.POST and 'destination_floor' in request.POST and 'destination_tag' in request.POST and 'priority' in request.POST:
            pkg_id = request.POST['pkg_id']
            weight = request.POST['weight']
            length = request.POST['length']
            width = request.POST['width']
            height = request.POST['height']
            destination_floor = request.POST['destination_floor']
            destination_tag = request.POST['destination_tag']
            priority = request.POST['priority']
        if 'notes' in request.POST:
            notes = request.POST['notes']
        if pkg_id and weight and length and width and height and destination_floor and destination_tag and priority:
            models.Package.objects.create(
                pkg_id=pkg_id,
                weight=weight,
                length=length,
                width=width,
                height=height,
                destination_floor=destination_floor,
                destination_tag=destination_tag,
                priority=priority,
                notes=notes
            )
            return JsonResponse({'success': True, 'message': 'Package added successfully'})
        else:
            return JsonResponse({'message': 'Invalid request'}, status=400)


def update_package(request):
    if request.method == 'POST':
        if 'pkg_id' not in request.POST:
            return JsonResponse({'message': 'Package ID is required'})

        try:
            package = models.Package.objects.get(pkg_id=request.POST['pkg_id'])
            
            if 'weight' in request.POST:
                package.weight = request.POST['weight']
            if 'length' in request.POST:
                package.length = request.POST['length']
            if 'width' in request.POST:
                package.width = request.POST['width']
            if 'height' in request.POST:
                package.height = request.POST['height']
            if 'destination_floor' in request.POST:
                package.destination_floor = request.POST['destination_floor']
            if 'destination_tag' in request.POST:
                package.destination_tag = request.POST['destination_tag']
            if 'priority' in request.POST:
                package.priority = request.POST['priority']
            if 'notes' in request.POST:
                package.notes = request.POST['notes']
            
            package.save()
            
            return JsonResponse({
                'success': True, 
                'message': 'Package updated successfully',
                'package': {
                    'pkg_id': package.pkg_id,
                    'weight': package.weight,
                    'dimensions': f"{package.length}x{package.width}x{package.height}",
                    'destination': f"{package.destination_floor}/{package.destination_tag}",
                    'priority': package.priority,
                    'notes': package.notes
                }
            })
            
        except models.Package.DoesNotExist:
            return JsonResponse({'success': False, 'message': 'Package not found'})
        except Exception as e:
            return JsonResponse({'success': False, 'message': f'Error updating package: {str(e)}'})
    
    return JsonResponse({'success': False, 'message': 'Invalid request method'})

def get_package(request):
    if request.method == 'GET':
        pkg_id = request.GET.get('pkg_id')
        
        if not pkg_id:
            return JsonResponse({'message': 'Package ID is required'}, status=400)
        
        try:
            package = models.Package.objects.get(pkg_id=pkg_id)
            
            return JsonResponse({
                'success': True,
                'package': {
                    'pkg_id': package.pkg_id,
                    'weight': package.weight,
                    'length': package.length,
                    'width': package.width,
                    'height': package.height,
                    'dimensions': f"{package.length}x{package.width}x{package.height}",
                    'destination_floor': package.destination_floor,
                    'destination_floor_display': package.get_destination_floor_display(),
                    'destination_tag': package.destination_tag,
                    'destination_tag_display': package.get_destination_tag_display(),
                    'priority': package.priority,
                    'notes': package.notes,
                    'status': package.status,
                    'created_at': package.created_at.strftime('%Y-%m-%d %H:%M:%S'),
                    'updated_at': package.updated_at.strftime('%Y-%m-%d %H:%M:%S')
                }
            })
            
        except models.Package.DoesNotExist:
            return JsonResponse({'message': 'Package not found'}, status=404)
        except Exception as e:
            return JsonResponse({'message': f'Error retrieving package: {str(e)}'}, status=500)
    
    return JsonResponse({'message': 'Invalid request method'}, status=405)

def get_packages(request):
    if request.method == 'GET':
        packages = models.Package.objects.all()
        print(packages)
        search_term = request.GET.get('search_term')
        if search_term:
            packages = packages.filter(pkg_id__icontains=search_term)
        
        status_filter = request.GET.get('status_filter')
        if status_filter:
            packages = packages.filter(status=status_filter)
        
        priority_filter = request.GET.get('priority_filter')
        if priority_filter:
            packages = packages.filter(priority=priority_filter)
        
        date_filter = request.GET.get('date_filter')
        today = timezone.now().date()
        
        if date_filter == '1':  
            packages = packages.filter(updated_at__date=today)
        elif date_filter == '2':
            start_of_week = today - timedelta(days=today.weekday())
            packages = packages.filter(updated_at__date__gte=start_of_week)
        elif date_filter == '3':
            packages = packages.filter(updated_at__month=today.month, updated_at__year=today.year)

        packages = packages.prefetch_related(
            'transits__updates'
        )
        return JsonResponse(
            {'packages': PackageSerializer(packages, many=True).data}, safe=False
        )
    return JsonResponse({'message': 'Invalid request method'})


def get_transits(request):
    pkg_id = None
    status = None
    if request.method == 'GET':
        try:
            transits = models.Transit.objects.all()
            pkg_id = request.GET.get('pkg_id')
            status = request.GET.get('status')
            if pkg_id is not None:
                if status is not None:
                    transits = transits.filter(pkg_id=pkg_id, transit_status=status)
                else:
                    transits = transits.filter(pkg_id=pkg_id)
            return JsonResponse({'success': True, 'transits': transits.values()})
        except Exception as e:
            return JsonResponse({'message': f'Error retrieving transits: {str(e)}'}, status=400)
            

