from django.db import models


TAG_CHOICES = [
    ('1', 'Tag 1'),
    ('2', 'Tag 2'),
    ('3', 'Tag 3'),
    ('4', 'Tag 4'),
]

FLOOR_CHOICES = [
    ('1', 'Floor 1'),
    ('2', 'Floor 2'),
]

PRIORITY_CHOICES = [
    ('1', 'Low'),
    ('2', 'Medium'),
    ('2', 'High'),
]

STATUS_CHOICES = [
    ('1', 'Waiting'),
    ('2', 'In Transit'),
    ('3', 'Delivered'),
    ('4', 'Canceled'),
]

TRANSIT_TYPE_CHOICES = [
    ('1', 'Pickup'),
    ('2', 'Retrieve'),
    ('3', 'Custom'),
]

class Package(models.Model):
    pkg_id = models.CharField(max_length=50, unique=True)
    weight = models.FloatField(blank=True, null=True)
    length = models.FloatField(blank=True, null=True)
    width = models.FloatField(blank=True, null=True)
    height = models.FloatField(blank=True, null=True)
    destination_tag = models.CharField(
        max_length=2,
        choices=TAG_CHOICES,
        default='1'
    )
    destination_floor = models.CharField(
        max_length=2,
        choices=FLOOR_CHOICES,
        default='1'
    )
    priority = models.CharField(
        max_length=2,
        choices=PRIORITY_CHOICES,
        default='2'
    )
    status = models.CharField(
        max_length=2,
        choices=STATUS_CHOICES,
        default='1'
    )
    notes = models.TextField(blank=True, null=True)
    created_at = models.DateTimeField(auto_now_add=True)
    updated_at = models.DateTimeField(auto_now=True)

class Transit(models.Model):
    package = models.ForeignKey(Package, on_delete=models.CASCADE, related_name="transits")
    transit_date = models.DateTimeField(auto_now_add=True)
    transit_type = models.CharField(
        max_length=2,
        choices=TRANSIT_TYPE_CHOICES,
        default='1'
    )
    transit_status = models.CharField(
        max_length=2,
        choices=STATUS_CHOICES,
        default='1'
    )
    pickup = models.CharField(
        max_length=2,
        choices=TAG_CHOICES,
        default='1'
    )
    dropoff = models.CharField(
        max_length=2,
        choices=TAG_CHOICES,
        default='1'
    )
    notes = models.TextField(blank=True, null=True)

class TransitUpdate(models.Model):
    transit = models.ForeignKey(Transit, on_delete=models.CASCADE, related_name='updates')
    update = models.TextField()
    created_at = models.DateTimeField(auto_now_add=True)
    