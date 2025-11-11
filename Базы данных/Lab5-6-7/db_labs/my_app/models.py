from django.db import models

class House(models.Model):
    address = models.CharField(max_length=256, null=False, blank=False)

class Work(models.Model):
    house = models.ForeignKey(
        House,
        on_delete=models.CASCADE,
        related_name='works',
        null=False
    )
    name = models.CharField(max_length=256, null=False, blank=False)

class Contractor(models.Model):
    name = models.CharField(max_length=256, null=False, blank=False)
    specialization = models.CharField(max_length=256, null=False, blank=False)

class WorkContractor(models.Model):
    work = models.ForeignKey(
        Work,
        on_delete=models.CASCADE,
        related_name='work_contractors',
        null=False
    )
    contractor = models.ForeignKey(
        Contractor,
        on_delete=models.CASCADE,
        related_name='contractor_works',
        null=False
    )

class Resident(models.Model):
    name = models.CharField(max_length=256, null=False, blank=False)
    room = models.BigIntegerField(null=False, blank=False)
    house = models.ForeignKey(
        House,
        on_delete=models.CASCADE,
        related_name='residents',
        null=False
    )

class Bill(models.Model):
    amount = models.BigIntegerField(null=False, blank=False)
    bill_date = models.DateField(null=False, blank=False)
    resident = models.ForeignKey(
        Resident,
        on_delete=models.CASCADE,
        related_name='bills',
        null=False
    )

class Payment(models.Model):
    amount = models.BigIntegerField(null=False, blank=False)
    payment_date = models.DateField(null=False, blank=False)
    bill = models.ForeignKey(
        Bill,
        on_delete=models.CASCADE,
        related_name='payments',
        null=False
    )