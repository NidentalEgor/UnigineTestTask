#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

class UrlStatisticsCollector
{
public:
	UrlStatisticsCollector()
	{	
	}

	void CollectStatistics(
			const std::string& input_file_path)
	{
		if (input_file_path.empty())
		{
			//Error!!!
		}

		std::ifstream input_file(input_file_path);

		if (!input_file.is_open())
		{
			//Error!!!
		}

		std::string input_file_line;
		while (std::getline(
				input_file,
				input_file_line))
		{
			ProcessLine(input_file_line);
		}
	}

private:

	std::string::size_type IsPrefixCorrect(
			const std::string& line,
			std::string::size_type position)
	{
		if (AreSubstringsCoincide(line, position, "://", 0, 3))
		{
			return position + 3;
		}
		if (AreSubstringsCoincide(line, position, "s://", 0, 4))
		{
			return position + 4;
		}

		return std::string::npos;
	}

	class SymbolChecker
	{
	public:

		enum class CorrectSymbolsSetType
		{
			Domen,
			Path
		};

		SymbolChecker() {};

		bool CheckSymbol(
				const CorrectSymbolsSetType correct_symbols_set_type,
				const char symbol)
		{
			if (correct_symbols_set_type == CorrectSymbolsSetType::Domen)
			{
				return CheckDomen(symbol);
			}
			else
			{
				return CheckPath(symbol);
			}
		}

	private:
		bool CheckLettersAndNumbers(
				const char symbol)
		{
			return (symbol >= 'a' && symbol <= 'z') ||
					(symbol >= 'A' && symbol <= 'Z') ||
					(symbol >= '0' && symbol <= '9');
		}

		bool CheckDomen(const char symbol)
		{
			return CheckLettersAndNumbers(symbol) ||
					symbol == '.' ||
					symbol == '-';
		}

		bool CheckPath(const char symbol)
		{
			return CheckLettersAndNumbers(symbol) ||
					symbol == '.' ||
					symbol == ',' ||
					symbol == '/' ||
					symbol == '+' ||
					symbol == '_';
		}
	};

	

	bool IsCorrectSymbol(
			const SymbolChecker::CorrectSymbolsSetType correct_symbols_set_type)
	{
		std::string correct_symbols;
		
	}

	std::string GetDomen(
			const std::string& line,
			const std::string::size_type position)
	{
		
	}

	std::string::size_type ParseUrl(
			const std::string& line,
			std::string::size_type position)
	{
		const std::string::size_type after_prefix_position =
				IsPrefixCorrect(line, position);
		if (after_prefix_position != std::string::npos)
		{
			
		}
		else
		{
			return position + 4;
		}
	}

	bool AreSubstringsCoincide(
			const std::string& first,
			const std::string::size_type first_position,
			const std::string& second,
			const std::string::size_type second_position,
			const size_t substring_size)
	{
		for (size_t i = 0; i < substring_size; ++i)
		{
			if (first[first_position + i] != second[second_position + i])
			{
				return false;
			}
		}

		return true;
	}

	void ProcessLine(
			const std::string& input_file_line)
	{
		std::string::size_type current_position = 0;
		std::string::size_type match_position = 0;

		while (current_position != std::string::npos &&
				current_position < input_file_line.size())
		{
			current_position =
					KMP(input_file_line, current_position, "http"); // »щем первое вхождение префикса URL-а
			if (current_position == std::string::npos)
			{
				break;
			}
			else
			{
				/*std::cout << "match_position = " << current_position << std::endl;
				std::for_each(
						input_file_line.begin() + current_position,
						input_file_line.begin() + current_position + 20,
						[](const char symbol)
						{
							std::cout << symbol;
						});

				std::cout << std::endl;*/

				current_position =
						ParseUrl(input_file_line, current_position);
			}
		}
	}

	std::string::size_type KMP(const std::string& S, int begin, const std::string& pattern)
	{
		std::vector<int> pf(pattern.length());

		pf[0] = 0;
		for (int k = 0, i = 1; i < pattern.length(); ++i)
		{
			while ((k > 0) && (pattern[i] != pattern[k]))
				k = pf[k - 1];

			if (pattern[i] == pattern[k])
				k++;

			pf[i] = k;
		}

		for (int k = 0, i = begin; i < S.length(); ++i)
		{
			while ((k > 0) && (pattern[k] != S[i]))
				k = pf[k - 1];

			if (pattern[k] == S[i])
				k++;

			if (k == pattern.length())
				return (i - pattern.length() + 1); //либо продолжаем поиск следующих вхождений
		}

		return std::string::npos;
	}

private:

};

int main ()
{
	UrlStatisticsCollector url_statistics_collector;
	url_statistics_collector.CollectStatistics("Input.txt");

	return 0;
}