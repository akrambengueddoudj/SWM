# Generated by Django 5.2 on 2025-04-11 14:51

import django.db.models.deletion
from django.db import migrations, models


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Package',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('pkg_id', models.CharField(max_length=50, unique=True)),
                ('weight', models.FloatField(blank=True, null=True)),
                ('length', models.FloatField(blank=True, null=True)),
                ('width', models.FloatField(blank=True, null=True)),
                ('height', models.FloatField(blank=True, null=True)),
                ('destination_tag', models.CharField(choices=[('1', 'Tag 1'), ('2', 'Tag 2'), ('3', 'Tag 3'), ('4', 'Tag 4')], default='1', max_length=2)),
                ('destination_floor', models.CharField(choices=[('1', 'Floor 1'), ('2', 'Floor 2')], default='1', max_length=2)),
                ('priority', models.CharField(choices=[('1', 'Low'), ('2', 'Medium'), ('2', 'High')], default='2', max_length=2)),
                ('status', models.CharField(choices=[('1', 'Waiting'), ('2', 'In Transit'), ('3', 'Delivered'), ('4', 'Canceled')], default='1', max_length=2)),
                ('notes', models.TextField(blank=True, null=True)),
                ('created_at', models.DateTimeField(auto_now_add=True)),
                ('updated_at', models.DateTimeField(auto_now=True)),
            ],
        ),
        migrations.CreateModel(
            name='Transit',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('transit_date', models.DateTimeField(auto_now_add=True)),
                ('transit_type', models.CharField(choices=[('1', 'Pickup'), ('2', 'Retrieve'), ('3', 'Custom')], default='1', max_length=2)),
                ('transit_status', models.CharField(choices=[('1', 'Waiting'), ('2', 'In Transit'), ('3', 'Delivered'), ('4', 'Canceled')], default='1', max_length=2)),
                ('pickup', models.CharField(choices=[('1', 'Tag 1'), ('2', 'Tag 2'), ('3', 'Tag 3'), ('4', 'Tag 4')], default='1', max_length=2)),
                ('dropoff', models.CharField(choices=[('1', 'Tag 1'), ('2', 'Tag 2'), ('3', 'Tag 3'), ('4', 'Tag 4')], default='1', max_length=2)),
                ('notes', models.TextField(blank=True, null=True)),
                ('package', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='robot.package')),
            ],
        ),
        migrations.CreateModel(
            name='TransitUpdate',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('update', models.TextField()),
                ('created_at', models.DateTimeField(auto_now_add=True)),
                ('transit', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='robot.transit')),
            ],
        ),
    ]
