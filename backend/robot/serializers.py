from rest_framework import serializers
from .models import Package, Transit, TransitUpdate

class TransitUpdateSerializer(serializers.ModelSerializer):
    created_at = serializers.DateTimeField(format='%Y-%m-%d %H:%M:%S')
    
    class Meta:
        model = TransitUpdate
        fields = ['update', 'created_at']

class TransitSerializer(serializers.ModelSerializer):
    updates = TransitUpdateSerializer(many=True, read_only=True)
    transit_date = serializers.DateTimeField(format='%Y-%m-%d %H:%M:%S')
    transit_type_display = serializers.CharField(source='get_transit_type_display', read_only=True)
    transit_status_display = serializers.CharField(source='get_transit_status_display', read_only=True)
    pickup_display = serializers.CharField(source='get_pickup_display', read_only=True)
    dropoff_display = serializers.CharField(source='get_dropoff_display', read_only=True)
    
    class Meta:
        model = Transit
        fields = [
            'id',
            'transit_date',
            'transit_type',
            'transit_type_display',
            'transit_status',
            'transit_status_display',
            'pickup',
            'pickup_display',
            'dropoff',
            'dropoff_display',
            'notes',
            'updates'
        ]


class PackageSerializer(serializers.ModelSerializer):
    destination_floor_display = serializers.CharField(source='get_destination_floor_display', read_only=True)
    destination_tag_display = serializers.CharField(source='get_destination_tag_display', read_only=True)
    status_display = serializers.CharField(source='get_status_display', read_only=True)
    priority_display = serializers.CharField(source='get_priority_display', read_only=True)
    updated_at = serializers.DateTimeField(format='%Y-%m-%d %H:%M:%S')
    created_at = serializers.DateTimeField(format='%Y-%m-%d %H:%M:%S')
    transits = TransitSerializer(many=True, read_only=True)
    
    class Meta:
        model = Package
        fields = [
            'pkg_id',
            'weight',
            'length',
            'width',
            'height',
            'destination_floor',
            'destination_floor_display',
            'destination_tag',
            'destination_tag_display',
            'priority',
            'priority_display',
            'status',
            'status_display',
            'notes',
            'updated_at',
            'created_at',
            'transits'
        ]