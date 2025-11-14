import os
import django
from tabulate import tabulate

os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'db_labs.settings')

django.setup()

from my_app.models import House, Work, Contractor, WorkContractor, Resident, Bill, Payment

def clear_screen():
    os.system('cls' if os.name == 'nt' else 'clear')

def show_menu():
    print("\n===== Меню =====")
    print("1. Дома")
    print("2. Работы")
    print("3. Подрядчики")
    print("4. Жители")
    print("5. Счета")
    print("6. Оплата")
    print("7. Выход")

def add_house():
    address = input("Введите адрес\n")
    House.objects.create(address=address)

def show_houses():
    headers = ["ID", "Адрес"]
    data = [[house.id, house.address] for house in House.objects.all()]
    print("\n" + tabulate(data, headers=headers, tablefmt="fancy_grid"))

def show_works():
    headers = ["ID", "ID дома", "Название работы"]
    data = [[works.id, works.address] for works in Work.objects.all()]
    print("\n" + tabulate(data, headers=headers, tablefmt="fancy_grid"))

def main():
    while True:
        clear_screen()
        show_menu()
        choice = input("Выберите пункт: ").strip()

        if choice == "1":
            show_houses()
            add = input("Нажмите 'a' для ввода данных, иначе нажмите Enter\n")
            if add == "a":
                add_house()
        elif choice == "2":
            pass
        elif choice == "3":
            pass
        elif choice == "4":
            pass
        elif choice == "5":
            pass
        elif choice == "6":
            pass
        elif choice == "7":
            break
        else:
            print("Неверный выбор. Попробуйте снова.")

        if choice != "8":
            input("\nНажмите Enter, чтобы вернуться в меню...")

if __name__ == "__main__":
    main()