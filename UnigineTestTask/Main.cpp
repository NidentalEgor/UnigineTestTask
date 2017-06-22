#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

class UrlStatisticsCollector
{
private:

	class SymbolChecker
	{
	public:
		enum class CorrectSymbolsSetType
		{
			Domen,
			Path
		};

		SymbolChecker(
			const CorrectSymbolsSetType correct_symbols_set_type)
			: correct_symbols_set_type_(correct_symbols_set_type)
		{
		};

		bool CheckSymbol(
			const char symbol) const
		{
			if (correct_symbols_set_type_ == CorrectSymbolsSetType::Domen)
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
			const char symbol) const
		{
			return (symbol >= 'a' && symbol <= 'z') ||
				(symbol >= 'A' && symbol <= 'Z') ||
				(symbol >= '0' && symbol <= '9');
		}

		bool CheckDomen(
			const char symbol) const
		{
			return CheckLettersAndNumbers(symbol) ||
				symbol == '.' ||
				symbol == '-';
		}

		bool CheckPath(
			const char symbol) const
		{
			return CheckLettersAndNumbers(symbol) ||
				symbol == '.' ||
				symbol == ',' ||
				symbol == '/' ||
				symbol == '+' ||
				symbol == '_';
		}

	private:
		CorrectSymbolsSetType correct_symbols_set_type_;
	};

public:
	UrlStatisticsCollector()
		: domen_symbol_checker_(SymbolChecker::CorrectSymbolsSetType::Domen)
		, path_symbol_checker_(SymbolChecker::CorrectSymbolsSetType::Path)
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



	std::string::size_type IsPrefixCorrect(
			const std::string& line,
			std::string::size_type position)
	{
		if (AreSubstringsCoincide(line, position + 4, "://", 0, 3)) // часть "http" уже проверена
		{
			return position + 7;
		}
		if (AreSubstringsCoincide(line, position + 4, "s://", 0, 4)) // часть "http" уже проверена
		{
			return position + 8;
		}

		return std::string::npos;
	}

	std::string::size_type GetPath(
			const std::string& line,
			const std::string::size_type position) const
	{
		for (size_t i = position; i < line.size(); ++i)
		{
			if (!path_symbol_checker_.CheckSymbol(line[i]))
			{
				return i;
			}
		}

		return std::string::npos;
	}

	std::string::size_type GetDomen(
			const std::string& line,
			const std::string::size_type position) const
	{
		size_t i = 0;
		for (i = position
			; i < line.size()
			; ++i)
		{
			if (line[i] == '/' || line[i] == ' ')
			{
				return i;
			}
			if (!domen_symbol_checker_.CheckSymbol(line[i]))
			{
				return std::string::npos;
			}
		}

		return i;
	}

	std::string::size_type ParseUrl(
			const std::string& line,
			std::string::size_type position)
	{
		const std::string::size_type after_prefix_position =
				IsPrefixCorrect(line, position);
		std::string::size_type after_path_position = 0;
		if (after_prefix_position != std::string::npos)
		{
			const std::string::size_type after_domen_position =
					GetDomen(line, after_prefix_position);
			if (after_domen_position != std::string::npos)
			{
				// Добавляем в хеш\мапу
				std::string domen(
						line.begin() + after_prefix_position,
						line.begin() + after_domen_position);
				std::cout << "domen = " << domen << std::endl;
			}
			else
			{
				return std::string::npos;
			}

			if (line[after_domen_position] != ' ')
			{
				after_path_position =
					GetPath(line, after_domen_position);
				if (after_path_position != std::string::npos)
				{
					// Добавляем в хеш\мапу
					std::string path(
						line.begin() + after_domen_position,
						line.begin() + after_path_position);
					std::cout << "path = " << path << std::endl;
				}
				else
				{
					return std::string::npos;
				}
			}
			else
			{
				std::cout << "path = '/'" << std::endl;
				// добавляем '/'  так как путь пустой
			}
			
		}
		else
		{
			return position + 4;
		}
		return after_path_position;
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
					KMP(input_file_line, current_position, "http"); // Ищем первое вхождение префикса URL-а
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
	SymbolChecker domen_symbol_checker_;
	SymbolChecker path_symbol_checker_;
};

int main ()
{
	UrlStatisticsCollector url_statistics_collector;
	url_statistics_collector.CollectStatistics("Input.txt");

	return 0;
}