#include "UrlStatisticsCollector/UrlStatisticsCollector.cpp"

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