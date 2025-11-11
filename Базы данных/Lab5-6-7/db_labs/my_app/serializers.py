from .models import *
from rest_framework import serializers

class HouseSerializer(serializers.ModelSerializer):
    class Meta:
        model = House
        fields = ['id', 'address']

class WorkSerializer(serializers.ModelSerializer):
    house = HouseSerializer(read_only=True)
    house_id = serializers.PrimaryKeyRelatedField(
        queryset=House.objects.all(),
        source='house'
    )

    class Meta:
        model = Work
        fields = ['id', 'name', 'house_id', 'house']

class ContractorSerializer(serializers.ModelSerializer):
    class Meta:
        model = Contractor
        fields = ['id', 'name', 'specialization']

class WorkContractorSerializer(serializers.ModelSerializer):
    work = WorkSerializer(read_only=True)
    contractor = ContractorSerializer(read_only=True)
    work_id = serializers.PrimaryKeyRelatedField(
        queryset=Work.objects.all(),
        source='work'
    )
    contractor_id = serializers.PrimaryKeyRelatedField(
        queryset=Contractor.objects.all(),
        source='contractor'
    )

    class Meta:
        model = WorkContractor
        fields = ['id', 'work_id', 'work', 'contractor_id', 'contractor']

class ResidentSerializer(serializers.ModelSerializer):
    house = HouseSerializer(read_only=True)
    house_id = serializers.PrimaryKeyRelatedField(
        queryset=House.objects.all(),
        source='house'
    )

    class Meta:
        model = Resident
        fields = ['id', 'name', 'room', 'house_id', 'house']

class BillSerializer(serializers.ModelSerializer):
    resident = ResidentSerializer(read_only=True)
    resident_id = serializers.PrimaryKeyRelatedField(
        queryset=Resident.objects.all(),
        source='resident'
    )

    class Meta:
        model = Bill
        fields = ['id', 'amount', 'bill_date', 'resident_id', 'resident']

class PaymentSerializer(serializers.ModelSerializer):
    bill = BillSerializer(read_only=True)
    bill_id = serializers.PrimaryKeyRelatedField(
        queryset=Bill.objects.all(),
        source='bill'
    )

    class Meta:
        model = Payment
        fields = ['id', 'amount', 'payment_date', 'bill_id', 'bill']

