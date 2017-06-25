#include "stdafx.h"
#include "CppUnitTest.h"
#include "../UnigineTestTask/Main.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
	TEST_CLASS(UnitTest1)
	{
	private:
		const std::string test_data_path_common_prefix_ = R"(D:/Projects/Repositories/UnigineTestTask/UnitTests/TestData/)";

		bool AreFilesEqual(
				const std::string& first_file_path, 
				const std::string& second_file_path) const
		{
			std::ifstream first_file(first_file_path);
			std::ifstream second_file(second_file_path);

			const std::istream_iterator<char> first_file_begin(first_file), first_file_end;
			const std::istream_iterator<char> second_file_begin(second_file);

			return std::equal(first_file_begin, first_file_end, second_file_begin);
		}

	public:
		
		TEST_METHOD(EmptyFileTest)
		{
			const std::string input_file_path =
					test_data_path_common_prefix_ + "EmptyFileTest/Input.txt";
			const std::string output_file_path =
					test_data_path_common_prefix_ + "EmptyFileTest/Output.txt";
			const std::string expected_result_file_path =
					test_data_path_common_prefix_ + "EmptyFileTest/ExpectedResult.txt";

			UrlStatisticsCollector url_statistics_collector(
					input_file_path);
			url_statistics_collector.WriteStatistics(
					output_file_path,
					5);

			Assert::IsTrue(
					AreFilesEqual(
						output_file_path,
						expected_result_file_path),
					L"Error!",
					LINE_INFO());
		}

		TEST_METHOD(OneUrlTest)
		{
			const std::string input_file_path =
				test_data_path_common_prefix_ + "OneUrlTest/Input.txt";
			const std::string output_file_path =
				test_data_path_common_prefix_ + "OneUrlTest/Output.txt";
			const std::string expected_result_file_path =
				test_data_path_common_prefix_ + "OneUrlTest/ExpectedResult.txt";

			UrlStatisticsCollector url_statistics_collector(
				input_file_path);
			url_statistics_collector.WriteStatistics(
				output_file_path,
				5);

			Assert::IsTrue(
				AreFilesEqual(
					output_file_path,
					expected_result_file_path),
				L"Error!",
				LINE_INFO());
		}

		TEST_METHOD(AllCases)
		{
			const std::string input_file_path =
				test_data_path_common_prefix_ + "AllCases/Input.txt";
			const std::string output_file_path =
				test_data_path_common_prefix_ + "AllCases/Output.txt";
			const std::string expected_result_file_path =
				test_data_path_common_prefix_ + "AllCases/ExpectedResult.txt";

			UrlStatisticsCollector url_statistics_collector(
				input_file_path);
			url_statistics_collector.WriteStatistics(
				output_file_path,
				5);

			Assert::IsTrue(
				AreFilesEqual(
					output_file_path,
					expected_result_file_path),
				L"Error!",
				LINE_INFO());
		}

	};
}