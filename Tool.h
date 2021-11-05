#include<string>
#include<fstream>

int N = 65;

class Tool
{
	double price;
	int countSell;
	int countBuy;
	int position;
	int percent;
	bool growth;
	char name;

	double realPrices[65];
	double sma[46];
	double ema[46];
	double st[46];
	int n;
	double alfa;

	int tic;
	int step;

	void calcSma()
	{
		double s = 0;
		for (int i = 0; i < n; i++)
			s += realPrices[i];
		sma[0] = s / 20;
		for (int i = 1; i < N - n + 1; i++)
			sma[i] = sma[i - 1] - realPrices[i - 1] / 20 + realPrices[i + n - 1] / 20;
	}
	void calcEma()
	{
		double t = realPrices[0];
		for (int i = 1; i < n; i++)
			t = alfa * realPrices[i] + (1 - alfa) * t;
		ema[0] = t;
		for (int i = 1; i < N - n + 1; i++)
			ema[i] = alfa * realPrices[i + n - 1] + (1 - alfa) * ema[i-1];
	}

	void calcStd()
	{
		for (int i = 0; i < N - n + 1; i++)
		{
			st[i] = 0;
			for (int j = 0; j < n; j++)
				st[i] += (realPrices[j + i] - sma[j]) * (realPrices[j + i] - sma[j]);
			st[i] = sqrtf(st[i] / 20);
		}
	}

	void updatePrice()
	{
		double t = price;
		int sign = std::rand() % 2 > 0 ? 1 : -1;
		if (tic % step == 0)
			price = realPrices[tic / step + n - 1] + sign * realPrices[tic / step + n - 1] * 0.02 * (double)std::rand() / RAND_MAX;
		else
			price = ema[tic / step] + sign * (double)std::rand() / RAND_MAX * st[tic / step];
		tic += 1;
		growth = price > t;
		percent = growth ? (price / t - 1) * 100 : (t / price - 1) * 100;
	}
	void updateDOM()
	{
		if (growth)
		{
			countBuy -= percent * countBuy / 80 + std::rand() % (countBuy/10);
			countSell += percent * countSell / 80 + std::rand() % (countSell / 10);
		}
		else
		{
			countBuy += percent * countBuy / 80 + std::rand() % (countBuy / 10);
			countSell -= percent * countSell / 80 + std::rand() % (countSell / 10);
		}
	}

public:
	Tool(std::string dataName, char objName, double money)
	{
		n = 20;
		alfa = 0.9;
		tic = 0;
		step = 4;
		name = objName;
		percent = 0;
		position = 0;
		std::ifstream data;
		data.open(dataName);
		if (data.is_open() && data.peek() != EOF)
		{
			for (int i = 0; i < N; i++)
				data >> realPrices[i];
		}

		calcSma();
		calcEma();
		calcStd();
		price = realPrices[n-1] + (double)std::rand() / RAND_MAX * realPrices[n - 1] * 0.02;
		countSell = floor(money / price);
		countBuy = countSell;
	}
	bool updatePosition(int count)
	{
		if (count > 0 && count > countBuy || count < 0 && -count > countSell)
			return false;
		position += count;
		return true;
	}
	void update()
	{
		updatePrice();
		updateDOM();
	}
	double getPriceSell()
	{
		return price - 0.1;
	}
	double getPriceBuy()
	{
		return price + 0.1;
	}
	double getPercent()
	{
		return percent;
	}
	int getCountSell()
	{
		return countSell;
	}
	int getCountBuy()
	{
		return countBuy;
	}
	bool getGrowth()
	{
		return growth;
	}
	char getName()
	{
		return name;
	}
	int getPosition()
	{
		return position;
	}
};

