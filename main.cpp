#include <iostream>
#include <limits>
#include <string>

void PrintHeader() 
	{
		std::cout << "Автоматическая сборка информации для лучшего списка по аниме сериалам" << std::endl;
		std::cout << "---------------------------------------------------------------------" << std::endl;
	}
void PrintMain ()
	{
	std::cout << "Выберите действие: " << std::endl;
	std::cout << "1. Добавить новое аниме в список." << std::endl;
	std::cout << "2. Удалить аниме из списка." << std::endl;
	std::cout << "3. Вывести табличную часть со списком." << std::endl;
	std::cout << "0. Выход." << std::endl;
	std::cout << "Введите номер из списка." << std::endl;
	}
// Если пользователь долбоеб и пишет буквенное значение, а не числовое, то будет его поправлять
int ReadInt ()
	{
		int value;
			while (true)
				{
				if (std::cin >> value)
					{
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					return value;
					}
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Ошибка ввода. Введите число: " << std::endl;
				}
	}			 
int main ()
	{
		PrintHeader();
			bool running = true;
		while (running)
		{
			PrintMain ();
			int choice = ReadInt();
			std::cout << "\n";
			switch (choice)
				{
				case 1:
					std::cout << "В будущем будет добавлена возможность добавление в список нового аниме.\n\n" << std::endl;
					break;
				case 2:
					std::cout << "В будущем будет добавлена возможность убирать аниме из списка.\n\n" << std::endl;
					break;
				case 3:
					std::cout << "В будущем будет добавлена возможность выводить таблицу.\n\n" << std::endl;
					break;
				case 0:
					std::cout << "Выход из программы.\n" << std::endl;
					
					break;
				default:
					std::cout << "Нет такого пункта в меню. Повторите ввод.\n\n" << std::endl;
					break;
				}
		}
	return 0;
	}
