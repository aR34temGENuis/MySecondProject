#include <iostream>
#include <limits>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <iomanip>
#include <cctype>


struct AnimeEntry
	{
		std::string title; // 2. Название аниме
		std::string description; // 3. Краткое описание
		std::string watchStatus; // 4. Статус просмотра
		int rating = 0; // 5. Своя оценка (от 0 до 10) - Также потом проверить возможность выставление оценки по макс. 10-бальной шкале.
		bool hasSequel = false; // 6. Есть ли продолжение
	};
	
std::vector<AnimeEntry> g_anime;

std::size_t Utf8Len(const std::string& s) //Количество UTF-8 кодпоинтов
	{
		std::size_t count =0;
		for (unsigned char c : s)
		{
			if ((c & 0xC0) != 0x80)
			{
				++count;
			}
		}
		return count;
	}

std::string Utf8Truncate(const std:: string& s, std::size_t width) // Обрезка UTF-8 строки по "видимой" линии
	{
		if (width == 0) return "";
		if (Utf8Len(s) <= width) return s;
// Далее пишем, если есть пустое место
		std::size_t keep = (width <= 3) ? width : (width - 3);
		std::string out;
		out.reserve(s.size());
		std::size_t cp = 0;
		for (std::size_t i = 0; i < s.size(); )
			{
				unsigned char c = static_cast<unsigned char>(s[i]);
				std::size_t charBytes = 1;
				if ((c & 0x80) == 0x00) charBytes = 1;
				else if ((c & 0xE0) == 0xC0) charBytes = 2;
				else if ((c & 0xF0) == 0XE0) charBytes = 3;
				else if ((c & 0xF8) == 0xF0) charBytes = 4;
				if (cp >= keep) break;
				out.append(s, i, charBytes);
				i += charBytes;
				++cp;
			}
		if (width > 3) out += "...";
		return out;
	}

void PrintCell(const std::string& text, int width)
	{
		std::string t = Utf8Truncate(text, static_cast<std::size_t>(width));
		std::cout << t;
		int pad = width - static_cast<int>(Utf8Len(t));
		if (pad > 0) std::cout << std::string(pad, ' ');
	}
std::string Truncate(const std::string& s, std::size_t width)
	{
		if (width == 0) return "";
		if (s.size() <= width) return s;
		if (width <= 3) return s.substr(0, width);
		return s.substr(0, width - 3) + "...";	
	}
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

std::string ReadyNonEmptyLine(const std:: string& promt) //Считываем обязательную строку, поддерживаем пробелы внутри строки, триммим пробелы по краям и запрещаем пустой ввод.
	{
		while(true)
		{
		std::cout << promt << std::flush;
		std::string s;
		std::getline(std::cin, s);
		while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))) s.erase(s.begin()); //Убираем пробелы слева
		while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) s.pop_back(); //Убираем пробелы справа
		if (!s.empty()) return s;
		std::cout << "Поле обязательное. Повторите ввод.\n";
		}
	}

int ReadRating0to10(const std::string& promt)
	{
		while(true)
			{
				std::cout << promt << std::flush;
				std::string s;
				std::getline(std::cin, s);
// Попробуем распарсить int из строки
				try 
					{
						size_t pos = 0;
						int v = std::stoi(s, &pos);
						while (pos < s.size() && std::isspace(static_cast<unsigned char>(s[pos]))) ++pos;
						if (pos != s.size())
							{
								std::cout << "Введите только число.\n";
								continue;
							}
						if (v>= 0 && v <=10) return v;
						std::cout << "Оценка должна быть от 0 до 10.\n";
					}
				catch(...)
					{
						std::cout << "Ошибка ввода. Введите число от 0 до 10.\n";
					}
			}
	}

bool ReadYesOrNo(const std::string& promt)
	{
		while (true)
			{
				std::cout << promt << std::flush;
				std::string s;
				std::getline(std::cin, s);
				for (char& ch : s) ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
				while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))) s.erase(s.begin());
				while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) s.pop_back();
				if (s == "y" || s == "yes" || s == "1" || s == "да" || s == "д" || s == "Да" || s == "Yes") return true;
				if (s == "n" || s == "no" || s == "0" || s == "нет" || s == "н" || s == "Нет" || s == "No") return false;
				std::cout << "Введите Да/Нет (д/н) или Yes/No (y/n).\n";
			}
	}

void AddAnimeFlow()
	{
		while (true)
			{
				AnimeEntry a;
				std::cout << "\n--- Добавление нового аниме ---\n";
				a.title = ReadyNonEmptyLine("Название аниме: ");
				a.description = ReadyNonEmptyLine("Краткое описание: ");
				a.watchStatus = ReadyNonEmptyLine("Статус просмотра (например: Смотрю/Просмотрено/В планах/Брошено): ");
				a.rating = ReadRating0to10("Своя оценка (от 0 до 10): ");
				a.hasSequel = ReadYesOrNo("Существует ли продолжение этого аниме? (Да/Нет): ");
				g_anime.push_back(a);
				std::cout << "Запись добавлена. Сейчас в списке: " << g_anime.size() << " шт.\n";
				bool AddMoreAnime = ReadYesOrNo("Добавить ещё одно аниме? (Да/Нет)");
				if (!AddMoreAnime)
					{
						std::cout << "Возврат в меню.\n\n";
						return;
					}
				std::cout << "\n";
			}
	}
	
void PrintTable()
	{
		const int W_NO = 4;
		const int W_TITLE = 24;
		const int W_DESC = 40;
		const int W_STATUS = 16;
		const int W_RATING = 16;
		const int W_SEQUEL = 12;
		
		auto line = [&]()
			{
				std::cout
				<< '+' << std::string(W_NO + 2, '-')
				<< '+' << std::string(W_TITLE + 2, '-')
				<< '+' << std::string(W_DESC + 2, '-')
				<< '+' << std::string(W_STATUS + 2, '-')
				<< '+' << std::string(W_RATING + 2, '-')
				<< '+' << std::string(W_SEQUEL + 2, '-')
				<< "+\n";
			};
		line(); 
			std::cout << "| "; PrintCell("№", W_NO);
			std::cout << " | "; PrintCell("Название Аниме", W_TITLE);
			std::cout << " | "; PrintCell("Краткое описание", W_DESC);
			std::cout << " | "; PrintCell("Статус просмотра", W_STATUS);
			std::cout << " | "; PrintCell("Своя оценка", W_RATING);
			std::cout << " | "; PrintCell("Продолжение", W_SEQUEL);
			std::cout << " |\n";
		line();
		
		if (g_anime.empty())
		{
			std::cout << "| "; PrintCell("-", W_NO);
			std::cout << " | "; PrintCell("(Список пуст!)", W_TITLE);
			std::cout << " | "; PrintCell("-", W_DESC);
			std::cout << " | "; PrintCell("-", W_STATUS);
			std::cout << " | "; PrintCell("-", W_RATING);
			std::cout << " | "; PrintCell("-", W_SEQUEL);
			std::cout << " |\n";
			line();
			return;
		}
		for (std::size_t i=0; i < g_anime.size(); ++i)
			{
				const auto& a = g_anime[i];
				std::string sequelText = a.hasSequel ? "Да" : "Нет";
				std::cout << "| "; PrintCell(std::to_string(i + 1), W_NO);
				std::cout << " | "; PrintCell(a.title, W_TITLE);
				std::cout << " | "; PrintCell(a.description, W_DESC);
				std::cout << " | "; PrintCell(a.watchStatus, W_STATUS);
				std::cout << " | "; PrintCell(std::to_string(a.rating), W_RATING);
				std::cout << " | "; PrintCell(sequelText, W_SEQUEL);
				std::cout << " |\n";
				line (); // Разделитель для каждой отдельной ячейки
			}
	}			 
int main ()
	{
		PrintHeader();
		while (true)
		{
			PrintMain();
			int choice = ReadInt();
			std::cout << "\n";
			switch (choice)
				{
				case 1:
					AddAnimeFlow();
					std::this_thread::sleep_for(std::chrono::seconds(3));
					break;
				case 2:
					std::cout << "В будущем будет добавлена возможность удалять аниме из списка" << std::endl;
					std::this_thread::sleep_for(std::chrono::seconds(3));
					break;
				case 3:
					PrintTable();
					std::this_thread::sleep_for(std::chrono::seconds(10));
					break;
				case 0:
					std::cout << "Выход из программы.\n" << std::endl;
					std::this_thread::sleep_for(std::chrono::seconds(3));
					return 0;
				default:
					std::cout << "Нет такого пункта в меню. Повторите ввод.\n\n" << std::endl;
					std::this_thread::sleep_for(std::chrono::seconds(3));
					break;
				}
		}
	}
