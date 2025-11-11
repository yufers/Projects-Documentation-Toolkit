from django.contrib import admin
from django.urls import path, include
from rest_framework import routers
from my_app import views

router = routers.DefaultRouter()
router.register(r'house', views.HouseViewset)
router.register(r'work', views.WorkViewset)
router.register(r'contractor', views.ContractorViewset)
router.register(r'work_contractor', views.WorkContractorViewset)
router.register(r'resident', views.ResidentViewset)
router.register(r'bill', views.BillViewset)
router.register(r'payment', views.PaymentViewset)

urlpatterns = [
    path('admin/', admin.site.urls),
    path('', include(router.urls)),
    path('api-auth/', include('rest_framework.urls', namespace='rest_framework')),
]
