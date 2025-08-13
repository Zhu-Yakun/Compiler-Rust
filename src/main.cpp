#include "Compiler/Compiler.hpp"
#include <iostream>
#include <vector>
#include <time.h>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "用法: " << argv[0] << "<输入文件> <输出位置> [-s 展示输出结果]" << std::endl;
        return 1;
    }

    std::string grammarFile = "grammar/grammar.txt";
    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    bool show_output = argc == 4;

    clock_t start, end;

    try {
        std::cout << "[main] 开始编译: 输入文件=" << inputFile << ", 输出文件=" << outputFile << std::endl;
    Compiler compiler(grammarFile, inputFile, outputFile, show_output);
    start = clock();
    compiler.compile();
    end = clock();  
	std::cout << "编译过程耗时 = " << double(end - start) << "ms" << std::endl;  
    } catch (const std::exception& e) {
        std::cerr << "[main] 捕获到std::exception: " << e.what() << std::endl;
        // 这里可以输出更多上下文
        std::cerr << "[main] 编译失败，建议检查上方详细报错和符号表输出。" << std::endl;
        return 2;
    } catch (...) {
        std::cerr << "[main] 捕获到未知异常。" << std::endl;
        return 3;
    }

    return 0;
}
