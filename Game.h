#include "Tool.h"
#include <conio.h>
#include <vector>
#include <cstring>
#include <stdlib.h>

double money = 10000;
double startMoney = money;

HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
std::vector<std::string> vec;
char nameList[] = { 'A', 'B', 'C', 'D', 'E'};
const int count = 5;
Tool* tools;

void gameStep();
bool processOrders();
void checkResult();

void gameLoop()
{
	tools = (Tool*)malloc(count * sizeof(Tool));
	for (int i = 0; i < 5; i++)
	{
		std::string str = "data";
		str += std::to_string(i+1);
		str += ".txt";
		std::cout << str;
		new(tools + i) Tool(str, nameList[i], money);
	}

	for (int i = 0; i < 120; i++)
	{
		system("cls");
		std::cout << "До конца периода осталось: " << 120 - i << " дней" << std::endl;
		gameStep();
		if (processOrders())
		{
			break;
		}
		for (int i = 0; i < count; i++)
		{
			tools[i].update();
		}
	}
	checkResult();
	for (int i = 0; i < count; i++)
		tools[i].~Tool();
	free(tools);
}

void gameStep()
{
	std::cout << "Инструмент: " << '\t';
	for (int i = 0; i < count; i++)
	{
		std::cout << tools[i].getName() << "   ";
		if (tools[i].getGrowth())
			SetConsoleTextAttribute(handle, FOREGROUND_GREEN);
		else
			SetConsoleTextAttribute(handle, FOREGROUND_RED);
		std::cout << tools[i].getPercent() << '\t';
		SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
	std::cout << std::endl;
	std::cout << "Цена продажи: " << '\t';
	for (int i = 0; i < count; i++)
	{
		std::cout << tools[i].getPriceSell() << '\t';
	}
	std::cout << std::endl;
	std::cout << "Цена покупки: " << '\t';
	for (int i = 0; i < count; i++)
	{
		std::cout << tools[i].getPriceBuy() << '\t';
	}
	std::cout << std::endl;
	std::cout << "Продажа: " << '\t';
	for (int i = 0; i < count; i++)
	{
		std::cout << tools[i].getCountSell() << '\t';
	}
	std::cout << std::endl;
	std::cout << "Покупка: " << '\t';
	for (int i = 0; i < count; i++)
	{
		std::cout << tools[i].getCountBuy() << '\t';
	}
	std::cout << std::endl;
	std::cout << "Позиция: " << '\t';
	for (int i = 0; i < count; i++)
	{
		std::cout << tools[i].getPosition() << '\t';
	}
	std::cout << std::endl;
	std::cout << "Средства: " << money << std::endl;
}

bool processOrders()
{
	clock_t start = clock();
	while (!_kbhit())
	{
		if (((clock() - start) / CLOCKS_PER_SEC) >= 2)
		{
			break;
		}
	}
	if (((clock() - start) / CLOCKS_PER_SEC) < 2)
	{
		std::string str = "t";
		int lim[] = { 0,0,0,0,0 };
		while (str != "next")
		{
			while (std::cin >> str)
			{
				vec.push_back(str);
				if (str == "exit")
					return true;
				if (str == "next" || vec.size() == 3)
					break;
			}
			if (vec.size() == 0)
			{
				std::cout << "Ошибка операции" << std::endl;
				continue;
			}
			str = vec[0];
			if (str == "sell")
			{
				char name = vec[1][0];
				int cnt = atoi(vec[2].c_str());
				int pos = -1;
				for (int i = 0; i < count; i++)
				{
					if (name == nameList[i])
					{
						pos = i;
						break;
					}
				}
				if (pos < 0 || cnt + lim[pos] > tools[pos].getCountBuy() || tools[pos].getPosition() < cnt)
					std::cout << "Ошибка операции" << std::endl;
				else
				{
					tools[pos].updatePosition(-cnt);
					lim[pos] -= cnt;
					money += cnt*tools[pos].getPriceSell();
					std::cout << "Операция проведена успешно. Средств на счету: " << money << std::endl;
				}
			}
			if (str == "buy")
			{
				char name = vec[1][0];
				int cnt = atoi(vec[2].c_str());
				int pos = -1;
				for (int i = 0; i < count; i++)
				{
					if (name == nameList[i])
					{
						pos = i;
						break;
					}
				}
				if (pos < 0 || cnt * tools[pos].getPriceBuy() > money || cnt + lim[pos] > tools[pos].getCountSell())
					std::cout << "Ошибка операции" << std::endl;
				else
				{
					tools[pos].updatePosition(cnt);
					money -= cnt * tools[pos].getPriceBuy();
					lim[pos] += cnt;
					std::cout << "Операция проведена успешно. Средств на счету: " << money << std::endl;
				}
			}
			vec.clear();
		}
	}
	return false;
}

void checkResult()
{
	double sum = money;
	for (int i = 0; i < count; i++)
	{
		sum += tools[i].getPriceSell() * tools[i].getPosition();
	}
	if(sum >= startMoney)
		std::cout << "Вы молодец и заработали: "<< sum - startMoney<< "$" << std::endl;
	else
		std::cout << "На биржу еще рановато, вы спустили: " << startMoney - sum << "$" << std::endl;
}
