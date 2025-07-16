#include "Compiler/Compiler.hpp"
#include <iostream>
#include <vector>
#include <time.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "用法: " << argv[0] << "<输入文件> <输出位置> [-s 展示输出结果]" << std::endl;
        return 1;
    }

    std::string grammarFile = "grammar/grammar.txt";
    std::string inputFile = argv[1];
    bool show_output = argc == 3;

    clock_t start, end;


    Compiler compiler(grammarFile, inputFile, show_output);

    start = clock();
    compiler.compile();

    end = clock();  
	std::cout << "编译过程耗时 = " << double(end - start) << "ms" << std::endl;  

    return 0;
}
