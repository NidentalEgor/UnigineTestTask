#include <iterator>
#include <string>

#include "gtest/gtest.h"

#include "../UrlStatisticsCollector/UrlStatisticsCollector.cpp"

class SomeName
        : public testing::Test
{
protected:
    bool AreFilesEqual(
            const std::string& first_file_path, 
            const std::string& second_file_path) const
    {
        std::ifstream first_file(
                first_file_path, std::ios::in | std::ios::binary);

        if (!first_file)
        {
            return false;
        }

        std::ifstream second_file(
                second_file_path, std::ios::in | std::ios::binary);

        if (!second_file)
        {
            return false;
        }

        return std::string(
                       std::istreambuf_iterator<char>(first_file),
                       std::istreambuf_iterator<char>()) ==
                std::string(
                       std::istreambuf_iterator<char>(second_file),
                       std::istreambuf_iterator<char>());
    }
    
protected:
    const std::string test_data_path_common_prefix_ = R"(../../../../UnitTests/TestData/)";
};

TEST_F(SomeName, EmptyFileTest)
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

    ASSERT_TRUE(AreFilesEqual(output_file_path, expected_result_file_path));
}

TEST_F(SomeName, OneUrlTest)
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

    ASSERT_TRUE(AreFilesEqual(output_file_path, expected_result_file_path));    
}

TEST_F(SomeName, AllCases)
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

    ASSERT_TRUE(AreFilesEqual(output_file_path, expected_result_file_path));    
}

TEST_F(SomeName, BigTestFromUnigine)
{
    const std::string input_file_path =
            test_data_path_common_prefix_ + "BigTestFromUnigine/Input.txt";
    const std::string output_file_path =
            test_data_path_common_prefix_ + "BigTestFromUnigine/Output.txt";
    const std::string expected_result_file_path =
            test_data_path_common_prefix_ + "BigTestFromUnigine/ExpectedResult.txt";

    UrlStatisticsCollector url_statistics_collector(
            input_file_path);
    url_statistics_collector.WriteStatistics(
            output_file_path,
            100);

    ASSERT_TRUE(AreFilesEqual(output_file_path, expected_result_file_path));    
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
