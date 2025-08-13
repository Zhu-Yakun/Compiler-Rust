cmake_minimum_required(VERSION 3.27)
project(Rust_Compiler)

set(CMAKE_CXX_STANDARD 17)        # 强制使用C++17标准
set(CMAKE_CXX_STANDARD_REQUIRED ON) # 要求编译器必须支持C++17
if(CMAKE_CROSSCOMPILING)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

include_directories(include task)

if(MINGW)
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=GBK)
    add_link_options(-fexec-charset=UTF-8)
endif()

# ========================
# 创建一个共享的静态库目标：用于主程序 + 测试都链接
# ========================
add_library(core STATIC
        task/Lexer/Lexer.cpp
        task/LR1Parser/LR1Parser.cpp
        task/Compiler/Compiler.cpp
        task/Semantic/SemanticAnalyzer.cpp
        task/common/MetaData.cpp
        task/Variable/Variable.cpp
        task/Variable/VariableType.cpp
        task/MipsInstructionGenerator/MipsInstructionGenerator.cpp
        task/MipsInstructionGenerator/RegisterManager.cpp
        task/MipsInstructionGenerator/StackManager.cpp
)

# target_include_directories(core PUBLIC include)

# ========================
# 主程序
# ========================
add_executable(Rust_Compiler
        src/main.cpp
)

target_link_libraries(Rust_Compiler core)
