//2021
#include <iostream>
#include <Windows.h> 
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <Windows.h> 
using namespace std;
vector<int> word;
vector<int> transformed_word(1);
int digit;
string input;
string output;
vector<vector<int>> matrix_coder;
vector<int> p_line;
vector<int> syndrome(1);
bool d = 0;

bool input_from_keyboard()
{
	bool b = 1;
	char x = '1';
	cout << "Введите 1 блок кода, признаком окончания кода является '.' " << endl;
	while (x != '.')
	{

		if (x - 48 != 0 && x - 48 != 1) return 0;
		word.push_back(x - 48);
		cin >> x;
	}
	return 1;
}
bool input_from_file()
{
	bool b = 1;
	ifstream input("input.txt");
	char x = '1';
	while (x != '.')
	{

		if (x - 48 != 0 && x - 48 != 1) return 0;
		word.push_back(x - 48);
		input >> x;
	}
	input.close();
	if (word.size() != 27) return 0;
	return 1;
}
void create_coder_matrix()
{
	int k = 3;
	matrix_coder.resize(word.size()); // E matrix
	for (int i = 1; i < word.size(); i++)
	{
		matrix_coder[i].resize(word.size() + digit);
		for (int j = 1; j < word.size(); j++)
		{
			if (i == j) matrix_coder[i][j] = 1;
			else matrix_coder[i][j] = 0;
		}
	}
	for (int i = 1; i < word.size(); i++) // E + digit matrix
	{
		for (; k < pow(2, digit);) // get p_line
		{
			bool b = 1;
			for (int c = 2; c <= digit; c++)
			{
				if (k == pow(2, c))
				{
					b = 0; break;
				}
			}
			if (!b)
			{
				k++; continue;
			}
			int h = k;
			while (h)
			{
				p_line.push_back(h % 2);
				h /= 2;
			}
			while (p_line.size() < digit)
			{
				p_line.push_back(0);
			}
			k++;
			break;
		}
		int r = 0;
		for (int j = word.size() + digit - 1; j > word.size() - 1; j--)
		{
			matrix_coder[i][j] = p_line[r];
			r++;
		}
		p_line.clear();
	}


}
void get_digit()
{
	for (int i = 1; i < word.size(); i++)
	{
		if (pow(2, i) >= i + word.size())
		{
			digit = i; break;
		}
	}
}
void get_digit_for_decoding()
{
	for (int i = 1; i < word.size(); i++)
	{
		if (pow(2, i) >= word.size())
		{
			digit = i; break;
		}
	}
}
void coder()
{
	for (int j = 1; j < word.size() + digit; j++)
	{
		int s = 0;
		for (int i = 1; i < word.size(); i++)
		{
			s = s + (word[i] * matrix_coder[i][j]);
		}
		s = s % 2;
		transformed_word.push_back(s);
	}
}
void output_file()
{
	ofstream output("output.txt");
	for (int i = 1; i < transformed_word.size(); i++)
	{
		output << transformed_word[i];
	}
	output.close();
}
void output_console()
{
	for (int i = 1; i < transformed_word.size(); i++)
	{
		cout << transformed_word[i];
	}
	cout << endl;
}
void create_decoder_matrix()
{
	int k = 3;
	matrix_coder.resize(word.size());
	for (int i = 1; i < word.size() - digit; i++) // E + digit matrix
	{
		for (; k < pow(2, digit);) // get p_line
		{
			bool b = 1;
			for (int c = 2; c <= digit; c++)
			{
				if (k == pow(2, c))
				{
					b = 0; break;
				}
			}
			if (!b)
			{
				k++; continue;
			}
			int h = k;
			while (h)
			{
				p_line.push_back(h % 2);
				h /= 2;
			}
			while (p_line.size() < digit)
			{
				p_line.push_back(0);
			}
			k++;
			break;
		}
		int r = 0;
		matrix_coder[i].resize(digit + 1);
		for (int j = digit; j > 0; j--)
		{
			matrix_coder[i][j] = p_line[r];
			r++;
		}
		p_line.clear();
	}
	for (int i = word.size() - digit; i < word.size(); i++)
	{
		matrix_coder[i].resize(digit + 1);
		for (int j = 1; j < digit + 1; j++)
		{
			if (i - word.size() + digit + 1 == j) matrix_coder[i][j] = 1;
			else matrix_coder[i][j] = 0;

		}

	}
}
void decoder()
{
	syndrome.resize(1);
	for (int j = 1; j < digit + 1; j++)
	{
		int s = 0;
		for (int i = 1; i < word.size(); i++)
		{
			s = s + (word[i] * matrix_coder[i][j]);
		}
		s = s % 2;
		syndrome.push_back(s);
	}
	bool b = 1;
	for (int i = 1; i < digit + 1; i++)
	{
		if (syndrome[i] == 1) { b = 0; break; }
	}
	if (b)
	{
		for (int i = 1; i < word.size() - digit; i++)
		{
			transformed_word.push_back(word[i]);
		}
	}
	else
	{
		d = 0;
		for (int i = 1; i < word.size() - digit; i++)
		{
			bool f = 1;
			for (int j = 1; j < digit + 1; j++)
			{
				if (matrix_coder[i][j] != syndrome[j]) f = 0;
			}
			if (f)
			{
				d = 1;
				for (int k = 1; k < word.size() - digit; k++)
				{
					if (k == i)
					{
						if (word[i] == 1) transformed_word.push_back(0);
						if (word[i] == 0) transformed_word.push_back(1);
					}
					else  transformed_word.push_back(word[k]);
				}
			}

		}
		if (d == 0)
		{
			cout << "ошибка декодирования";
		}
	}
}
void main()
{
	SetConsoleCP(1251);  // Подключаем ввод русских букв
	SetConsoleOutputCP(1251);
	int n;
	while (true) {
		cout << "Введите 1 для ввода с клавиатуры, введите 2 для ввода из файла:" << endl;
		cin >> n;
		if (n == 1)
		{
			int k;
			cout << "Введите адрес и имя файла вывода:" << endl;
			cin >> output;
			cout << "Введите 1 для кодирования кода, 2 для декодирования:" << endl;
			cin >> k;
			bool b;
			if (k == 1)
			{
				b = input_from_keyboard();
				if (b)
				{
					get_digit();
					create_coder_matrix();
					coder();
					output_console();
					output_file();
					transformed_word.resize(1);
					word.clear();
				}
			}
			else if (k == 2)
			{
				b = input_from_keyboard();
				if (b)
				{
					get_digit_for_decoding();
					create_decoder_matrix();
					decoder();
					output_console();
					output_file();
					transformed_word.resize(1);
					word.clear();
				}
			}

		}
		if (n == 2)
		{
			int k;
			cout << "Введите адрес и имя файла ввода:" << endl;
			cin >> input;
			cout << "Введите адрес и имя файла вывода:" << endl;
			cin >> output;
			cout << "Введите 1 для кодирования кода, 2 для декодирования:" << endl;
			cin >> k;
			bool b, f;
			if (k == 1)
			{
				b = input_from_file();
				if (b)
				{
					get_digit();
					create_coder_matrix();
					coder();
					output_console();
					output_file();
					transformed_word.resize(1);
					word.clear();
				}
			}
			else if (k == 2)
			{
				b = input_from_file();
				if (b)
				{
					get_digit_for_decoding();
					create_decoder_matrix();
					decoder();
					output_console();
					output_file();
					transformed_word.resize(1);
					word.clear();
				}
			}

		}
	}

}