#include <thread>
#include <iostream>
#include <mutex>
#include <fstream>
#include <vector>
#include <string>

void calculate_result(const std::string& directory, const int index, const int multiplier)
{
	std::mutex _mutex;

	std::ifstream fin(directory + "in_" + std::to_string(index) + ".dat");

	short command;
	fin >> command;

	std::vector<double> numbers;
	while (!fin.eof())
	{
		double temp;
		fin >> temp;
		numbers.push_back(temp);
	}

	double result = 0;
	int size = numbers.size();
	switch (command)
	{
	case 1:
		for (int i = 0; i < size; i++)
		{
			result += numbers[i];
		}
		break;
	case 2:
		for (int i = 0; i < size; i++)
		{
			result += multiplier * numbers[i];
		}
		break;
	case 3:
		for (int i = 0; i < size; i++)
		{
			result += numbers[i] * numbers[i];
		}
		break;
	default:
		break;
	}

	std::ofstream fout(directory + "out.dat", std::ios::app);

	_mutex.lock();

	fout << "Результат потока " << index << ": " << result << "\n";
	std::cout << "Поток " << index << " завершил работу.\n";
	fout.close();

	_mutex.unlock();
}

int main()
{
	setlocale(LC_ALL, "Ru");

	std::string directory;
	std::cout << "Введите путь в директорию с файлами \"in_<N>.dat\" :";
	std::cin >> directory;
	directory += "\\";

	int multiplier;
	std::cout << "Введите множитель для операций умножения (если таковые будут) :";
	std::cin >> multiplier;

	int amount_of_threads;
	std::cout << "Введите количество потоков: ";
	std::cin >> amount_of_threads;

	std::vector<std::thread> threads;

	for (int i = 0; i < amount_of_threads; i++)
	{
		threads.emplace_back(calculate_result, std::ref(directory), i + 1, multiplier);
	}

	for (int i = 0; i < amount_of_threads; i++)
	{
		threads[i].join();
	}

	return 0;
}