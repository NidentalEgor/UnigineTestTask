#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <queue>
#include <memory>
#include <iostream>

/*!
* Переводит строку в нижний регистр.
*
\param[in] data Строка с возможным наличием символов в верхнем регистре.
*
\return строка data в нижнем регистре.
*/
std::string ToLowerCase(
		const std::string& data)
{
	std::string lower_case_data;
	lower_case_data.resize(data.size());
	std::transform(
			data.begin(),
			data.end(),
			lower_case_data.begin(),
			::tolower);
	return lower_case_data;
}

/*!
* Класс для проверки символов на предмет их содержания во множестве допустимых значений.
*/
class SymbolChecker
{
public:

	enum class CorrectSymbolsSetType
	{
		Domain,
		Path
	};

	/*!
	* Конструктор.
	*
	\param[in] correct_symbols_set_type Задает набор допустимых значений.
	*/
	SymbolChecker(
			const CorrectSymbolsSetType correct_symbols_set_type)
		: correct_symbols_set_type_(correct_symbols_set_type)
	{
	};

	virtual ~SymbolChecker()
	{
	}

	/*!
	* Проверяет символ на принадлежность к множеству допустимых значений.
	*
	\return true если символ принадлежит множеству допустимых значений, false - иначе.
	*/
	virtual bool CheckSymbol(
			const char symbol) const = 0;

protected:
	bool CheckLettersAndNumbers(
			const char symbol) const
	{
		return (symbol >= 'a' && symbol <= 'z') ||
				(symbol >= 'A' && symbol <= 'Z') ||
				(symbol >= '0' && symbol <= '9');
	}

protected:
	CorrectSymbolsSetType correct_symbols_set_type_;
};

class PathSymbolChecker : public SymbolChecker
{
public:
	explicit PathSymbolChecker(
			const CorrectSymbolsSetType correct_symbols_set_type)
		: SymbolChecker(correct_symbols_set_type)
	{
	}

	bool CheckSymbol(
		const char symbol) const override
	{
		return CheckLettersAndNumbers(symbol) ||
				symbol == '.' ||
				symbol == ',' ||
				symbol == '/' ||
				symbol == '+' ||
				symbol == '_';
	}
};

class DomainSymbolChecker : public SymbolChecker
{
public:
	explicit DomainSymbolChecker(
			const CorrectSymbolsSetType correct_symbols_set_type)
		: SymbolChecker(correct_symbols_set_type)
	{
	}

	bool CheckSymbol(
		const char symbol) const override
	{
		return CheckLettersAndNumbers(symbol) ||
				symbol == '.' ||
				symbol == '-';
	}
};

/*!
* Класс для поиска подстрок в строке. Реализация алгоритма Кнута — Морриса — Пратта.
*/
class SubstringSearcher
{
public:
	/*!
	* Конструктор.
	*
	\param[in] pattern Строка, которая будет искаться.
	*/
	SubstringSearcher(
			const std::string& pattern)
		: pattern_(pattern)
		, prefix_function_result_(pattern.size())
	{
		Preprocessing();
	}

	/*!
	* Ищет заранее определенную подстроку в строке line.
	*
	\param[in] line Строка, в которой будет производиться поиск.
	\param[in] begin_position Позиция в строке line, начиная с которой
	* будет производиться поиск.
	*
	\return Позиция в строке, следующая за последним символом искомой
	* подстроки, std::strin::npos иначе.
	*/
	std::string::size_type Search(
			const std::string& line,
			const std::string::size_type begin_position)
	{
		for (std::string::size_type k = 0, i = begin_position
			; i < line.size()
			; ++i)
		{
			while ((k > 0) && (pattern_[k] != line[i]))
			{
				k = prefix_function_result_[k - 1];
			}

			if (pattern_[k] == line[i])
			{
				++k;
			}

			if (k == pattern_.size())
			{
				return (i - pattern_.size() + 1);
			}
		}

		return std::string::npos;
	}
private:
	void Preprocessing()
	{
		prefix_function_result_.resize(
				pattern_.size());
		prefix_function_result_.front() = 0;

		for (std::string::size_type k = 0, i = 1
			; i < pattern_.size()
			; ++i)
		{
			while ((k > 0) && (pattern_[i] != pattern_[k]))
			{
				k = prefix_function_result_[k - 1];
			}

			if (pattern_[i] == pattern_[k])
			{
				k++;
			}

			prefix_function_result_[i] = k;
		}
	}

private:
	std::string pattern_;
	std::vector<int> prefix_function_result_;
};

using StringSizeTPair = std::pair<std::string, size_t>;
using StringToCountMap = std::unordered_map<std::string, size_t>;

class UrlStatisticsCollector
{
public:
	/*!
	* Конструктор.
	*
	\param[in] input_file_path Путь к файлу с входными данными.
	*/
	UrlStatisticsCollector(
			const std::string& input_file_path)
		: input_file_path_(input_file_path)
		, is_file_processed_(false)
		, domain_symbol_checker_(
				std::make_shared<DomainSymbolChecker>(
					SymbolChecker::CorrectSymbolsSetType::Domain))
		, path_symbol_checker_(
				std::make_shared<PathSymbolChecker>(
					SymbolChecker::CorrectSymbolsSetType::Path))
		, substring_searcher_("http")
		, urls_count_(0)
		, is_statistics_collected_(false)
		, size_of_top_rate_(5)
	{
	}

	/*!
	* Устанавливает путь до файла с входными данными.
	*
	\param[in] input_file_path Путь к файлу с входными данными.
	*/
	void SetInputFilePath(
			const std::string& input_file_path)
	{
		if (input_file_path_ != input_file_path)
		{
			input_file_path_ = input_file_path;
			is_file_processed_ = false;
		}
	}

	/*!
	* При необходимости парсит файл с входными данными. Записывает результат
	в файл, находящийся по указанному пути.
	*
	\param[in] output_file_path Путь к файлу для записи результатов.
	\param[in] size_of_top Количество статистических записей, которое будет выведено.
	*/
	void WriteStatistics(
			const std::string& output_file_path,
			const size_t size_of_top)
	{
		EnsureStatisticsCollected();

		if (output_file_path.empty())
		{
			throw std::invalid_argument(
					"UrlStatisticsCollector::WriteStatistics : Output file path is empty!");
		}

		std::ofstream output_file(output_file_path);

		if (!output_file.is_open())
		{
			throw std::invalid_argument(
					"UrlStatisticsCollector::WriteStatistics : Can not open output file!");
		}

		output_file <<
				"total urls " << urls_count_ <<
				", domains " << domains_.size() <<
				", paths " << paths_.size() << std::endl << std::endl;

		output_file << "top domains" << std::endl;
		WriteTopNElements(
				domains_,
				size_of_top,
				false,
				output_file);

		output_file << "top paths" << std::endl;
		WriteTopNElements(
				paths_,
				size_of_top,
				true,
				output_file);
	}

private:
	void CollectStatistics(
			const std::string& input_file_path)
	{
		if (input_file_path.empty())
		{
			throw std::invalid_argument(
					"UrlStatisticsCollector::WriteStatistics : Input file path is empty!");
		}

		std::ifstream input_file(input_file_path);

		if (!input_file.is_open())
		{
			throw std::invalid_argument(
					"UrlStatisticsCollector::WriteStatistics : Can not open input file!");
		}

		std::string input_file_line;
		while (std::getline(
				input_file,
				input_file_line))
		{
			ProcessLine(input_file_line);
		}

		is_file_processed_ = true;
	}

	void EnsureStatisticsCollected()
	{
		if (!is_file_processed_)
		{
			CollectStatistics(input_file_path_);
		}
	}

	void WriteTopNElements(
			const StringToCountMap& container,
			const size_t size_of_top,
			const bool is_case_sensitive,
			std::ofstream& output_file) const
	{
		std::deque<StringSizeTPair> top_n_records;
		StringToCountMap::const_iterator current =
				container.begin();
		for (size_t i = 0
			; i < std::min(size_of_top, container.size())
			; ++i, ++current)
		{
			top_n_records.push_back(*current);
		}

		// Сначала сортируем по первичному признаку.
		std::sort(
				top_n_records.begin(),
				top_n_records.end(),
				[](const StringSizeTPair& left,
					const StringSizeTPair& right)
				{
					return left.second > right.second;
				});

		// Выбираем size_of_top элементов, которые встречались чаще других.
		for (; current != container.end(); ++current)
		{
			if (current->second > top_n_records.back().second)
			{
				top_n_records.pop_front();
				top_n_records.push_back(*current);
			}
		}

		// Сортируем по вторичному признаку(алфавитный порядок без учета регистра).
		std::sort(
				top_n_records.begin(),
				top_n_records.end(),
				[is_case_sensitive](const StringSizeTPair& left,
					const StringSizeTPair& right)
				{
					if (left.second != right.second)
					{
						return false;
					}

					if (is_case_sensitive)
					{
						return ToLowerCase(left.first) < ToLowerCase(right.first);
					}

					// Домены уже приведены к нижнему регистру.
					return left.first < right.first;
				});

		std::for_each(
				top_n_records.begin(),
				top_n_records.end(),
				[&output_file](const StringSizeTPair& domain)
				{
					output_file << domain.second << " " << domain.first << std::endl;
				});

		output_file << std::endl;
	}

	std::string::size_type IsPrefixCorrect(
			const std::string& line,
			std::string::size_type position) const
	{
		// Часть "http" уже проверена, проверяем оба возможных окончания префикса.
		if (line.compare(position + 4, 3, "://") == 0)
		{
			return position + 7;
		}

		if (line.compare(position + 4, 4, "s://") == 0)
		{
			return position + 8;
		}

		return std::string::npos;
	}

	std::string::size_type GetPositionAfterCertainUrlPart(
			const std::string& line,
			const std::string::size_type position,
			const std::shared_ptr<SymbolChecker>& symbol_checker) const
	{
		for (size_t i = position
			; i < line.size()
			; ++i)
		{
			if (!symbol_checker->CheckSymbol(line[i]))
			{
				return i;
			}
		}

		return line.size();
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

		const std::string::size_type after_domain_position =
				GetPositionAfterCertainUrlPart(
					line,
					after_prefix_position,
					domain_symbol_checker_);

		if (after_domain_position == after_prefix_position)
		{
			return position + 4;
		}

		const std::string domain(
				line.begin() + after_prefix_position,
				line.begin() + after_domain_position);
		// Домены не чувствительны к регистру, поэтому приводим к нижнему регистру сразу.
		++domains_[ToLowerCase(domain)];
		// Обязательные части(префикс и домен) существуют, поэтому теперь можем увеличить счетчик. 
		++urls_count_;

		const std::string::size_type after_path_position =
				GetPositionAfterCertainUrlPart(
					line,
					after_domain_position,
					path_symbol_checker_);

		std::string path(
				line.begin() + after_domain_position,
				line.begin() + after_path_position);

		if (path.empty())
		{
			path += '/';
		}
		++paths_[path];
		
		return after_path_position;
	}

	void ProcessLine(
			const std::string& input_file_line)
	{
		std::string::size_type current_position = 0;

		while (current_position != std::string::npos &&
				current_position < input_file_line.size())
		{
			// Ищем первое вхождение префикса URL-а.
			current_position =
					substring_searcher_.Search(
						input_file_line,
						current_position);

			if (current_position == std::string::npos)
			{
				break;
			}

			current_position =
					ParseUrl(input_file_line, current_position);
		}
	}

private:
	std::string input_file_path_;
	bool is_file_processed_;
	std::shared_ptr<SymbolChecker> domain_symbol_checker_;
	std::shared_ptr<SymbolChecker> path_symbol_checker_;
	SubstringSearcher substring_searcher_;
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
			++current_parameter_index;
		}
		command_line_options.input_file_path = argv[current_parameter_index++];
		command_line_options.output_file_path = argv[current_parameter_index++];
	}

	return command_line_options;
}

int main(int argc, char* argv[])
{
	try
	{
		const CommandLineOptions command_line_options =
				ParseCommandLine(
					argc,
					argv);

		UrlStatisticsCollector url_statistics_collector(
				command_line_options.input_file_path);
		url_statistics_collector.WriteStatistics(
				command_line_options.output_file_path,
				command_line_options.size_of_top);
	}
	catch (std::invalid_argument& ex)
	{
		std::cout << "Invalid argument: " << ex.what() << std::endl;
	}
	catch (std::exception& ex)
	{
		std::cout << "Unknown exception: " << ex.what() << std::endl;
	}
	

	return 0;
}