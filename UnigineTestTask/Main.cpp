#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <queue>
#include <locale>
#include <iterator>

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

class UrlStatisticsCollector
{
private:
	using StringToCountMap = std::unordered_map<std::string, size_t>;
public:
	UrlStatisticsCollector()
		: domen_symbol_checker_(SymbolChecker::CorrectSymbolsSetType::Domen)
		, path_symbol_checker_(SymbolChecker::CorrectSymbolsSetType::Path)
		, urls_count_(0)
		, is_statistics_collected_(false)
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

	void EnsureStatisticsCollected()
	{
		//if ()
	}

	void WriteTopNElements(
			const StringToCountMap& container,
			const size_t n,
			std::ofstream& output_file) const
	{
		std::deque<std::pair<std::string, size_t>> top_n_damains;
		StringToCountMap::const_iterator current =
				container.begin();
		for (size_t i = 0
			; i < std::min(n, container.size())
			; ++i, ++current)
		{
			top_n_damains.push_back(*current);
		}

		std::sort(
				top_n_damains.begin(),
				top_n_damains.end(),
				[](const std::pair<std::string, size_t>& left,
					const std::pair<std::string, size_t>& right)
				{
					return left.second > right.second;
				});

		for (; current != container.end(); ++current)
		{
			if (current->second > top_n_damains.back().second)
			{
				top_n_damains.pop_front();
				top_n_damains.push_back(*current);
			}
		}

		std::sort(
				top_n_damains.begin(),
				top_n_damains.end(),
				[](const std::pair<std::string, size_t>& left,
					const std::pair<std::string, size_t>& right)
				{
					if (left.second != right.second)
					{
						return false;
					}

					const std::string& left_key = left.first;
					const std::string& right_key = right.first;
					std::string left_key_lower_case, right_key_lower_case;
					
					std::transform(
							left_key.begin(),
							left_key.end(),
							std::back_inserter(left_key_lower_case),
							::tolower);

					std::transform(
							right_key.begin(),
							right_key.end(),
							std::back_inserter(right_key_lower_case),
							::tolower);


					return left_key_lower_case < right_key_lower_case;
				});

		std::for_each(
				top_n_damains.begin(),
				top_n_damains.end(),
				[&output_file](const std::pair<std::string, size_t>& domain)
				{
					output_file << domain.second << " " << domain.first << std::endl;
				});
		output_file << std::endl;
	}

	void WriteStatistics(
			const std::string& output_file_path,
			const size_t size_of_top)
	{
		if (output_file_path.empty())
		{
			// Error !!!
			return;
		}

		std::ofstream output_file(output_file_path);

		if (!output_file.is_open())
		{
			// Error !!!
			return;
		}

		output_file <<
				"total urls " << urls_count_ <<
				", domains " << domains_.size() <<
				", paths " << paths_.size() << std::endl << std::endl;

		output_file << "top domains" << std::endl;
		WriteTopNElements(
				domains_,
				size_of_top,
				output_file);

		output_file << "top paths" << std::endl;
		WriteTopNElements(
				paths_,
				size_of_top,
				output_file);
	}

	std::string::size_type IsPrefixCorrect(
			const std::string& line,
			std::string::size_type position)
	{
		//if (AreSubstringsCoincide(line, position + 4, "://", 0, 3)) // часть "http" уже проверена
		if (line.compare(position + 4, 3, "://") == 0) // часть "http" уже проверена
		{
			return position + 7;
		}
		//if (AreSubstringsCoincide(line, position + 4, "s://", 0, 4)) // часть "http" уже проверена
		if (line.compare(position + 4, 4, "s://") == 0)
		{
			return position + 8;
		}

		return std::string::npos;
	}

	std::string::size_type GetPath(
			const std::string& line,
			const std::string::size_type position) const
	{
		size_t i = 0;
		for (i = position; i < line.size(); ++i)
		{
			if (!path_symbol_checker_.CheckSymbol(line[i]))
			{
				return i;
			}
		}

		return i;
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
			if (!domen_symbol_checker_.CheckSymbol(line[i]))
			{
				// Возвращаем первый символ, которого не может быть в домене.
				return i;
			}
		}

		return line.size();
	}

	bool CheckEmptyPath(
			const std::string& line,
			const std::string::size_type position)
	{
		return line[position] == '\0';

		return line[position] == ' ' ||
				line[position] == '\t' || 
				line[position] == '\n';
	}

	std::string::size_type ParseUrl(
			const std::string& line,
			std::string::size_type position)
	{
		const std::string::size_type after_prefix_position =
				IsPrefixCorrect(line, position);

		if (after_prefix_position == std::string::npos)
		{
			return position + 4;
		}

		std::string::size_type after_path_position = 0;
		const std::string::size_type after_domen_position =
				GetDomen(line, after_prefix_position);
		if (after_domen_position == /*std::string::npos*/after_prefix_position)
		{
			return position + 4; // подумать
		}

		// Добавляем в хеш\мапу
		std::string domen(
				line.begin() + after_prefix_position,
				line.begin() + after_domen_position);
		std::cout << "domen = " << domen << std::endl;
		++domains_[domen];
		++urls_count_;

		if (CheckEmptyPath(line, after_domen_position))
		{
			std::cout << "path = '/'" << std::endl;
			++paths_["/"];
			// добавляем '/'  так как путь пустой
			return after_domen_position; // подумать
		}
		
		// Подумать
		after_path_position =
				GetPath(line, after_domen_position);
		//if (after_path_position == std::string::npos)
		//{
		//	return position + 4; // подумать
		//}

		// Добавляем в хеш\мапу
		std::string path(
				line.begin() + after_domen_position,
				line.begin() + after_path_position);

		if (path.back() != '/')
		{
			path += '/';
		}

		std::cout << "path = " << path << std::endl;
		++paths_[path];
		
		return after_path_position;
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

			current_position =
					ParseUrl(input_file_line, current_position);
		}
	}

	std::string::size_type KMP(
			const std::string& line,
			const std::string::size_type begin_position,
			const std::string& pattern) // нужно вынести в отдельный класс и pf вычислять всего раз
	{
		std::vector<int> pf(pattern.length());

		pf[0] = 0;
		for (std::string::size_type k = 0, i = 1
			; i < pattern.length()
			; ++i)
		{
			while ((k > 0) && (pattern[i] != pattern[k]))
			{
				k = pf[k - 1];
			}

			if (pattern[i] == pattern[k])
			{
				k++;
			}

			pf[i] = k;
		}

		for (std::string::size_type k = 0, i = begin_position
			; i < line.length()
			; ++i)
		{
			while ((k > 0) && (pattern[k] != line[i]))
			{
				k = pf[k - 1];
			}

			if (pattern[k] == line[i])
			{
				k++;
			}

			if (k == pattern.length())
			{
				return (i - pattern.length() + 1); //либо продолжаем поиск следующих вхождений
			}
		}

		return std::string::npos;
	}

private:
	SymbolChecker domen_symbol_checker_;
	SymbolChecker path_symbol_checker_;
	size_t urls_count_;
	bool is_statistics_collected_;
	size_t size_of_top_rate_;
	StringToCountMap domains_;
	StringToCountMap paths_;
};

struct CommandLineOptions
{
	size_t size_of_top = 5;
	std::string input_file_path = "Input.txt";
	std::string output_file_path ="Output.txt";
};

CommandLineOptions ParseCommandLine(
		int argc,
		char* argv[])
{
	CommandLineOptions command_line_options;

	if (argc > 1)
	{
		size_t current_parameter_index = 1;
		std::string parameter(argv[current_parameter_index]);
		if (parameter == "-n")
		{
			command_line_options.size_of_top =
					std::stoi(argv[++current_parameter_index]);
		}
		command_line_options.input_file_path = argv[current_parameter_index++];
		command_line_options.output_file_path = argv[current_parameter_index++];
	}

	return command_line_options;
}

int main(int argc, char* argv[])
{
	const CommandLineOptions command_line_options =
			ParseCommandLine(
				argc,
				argv);

	UrlStatisticsCollector url_statistics_collector;
	url_statistics_collector.CollectStatistics(
			command_line_options.input_file_path);
	url_statistics_collector.WriteStatistics(
			command_line_options.output_file_path,
			command_line_options.size_of_top);

	return 0;
}