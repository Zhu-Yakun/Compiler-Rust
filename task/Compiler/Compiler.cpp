#include "Compiler.hpp"

#include <utility>

static int lastGotoState = -1;
static Symbol lastGotoLhs;

Compiler::Compiler(const std::string &grammar_file_path, const std::string &input_file_path, std::string output_file_path, bool show_output)
    : output_file_path(std::move(output_file_path)), show_output(show_output), semanticAnalyzer(show_output)
{
    /* 设置输入文件 */
    std::ifstream file(input_file_path);
    if (!file.is_open())
    {
        std::cerr << "无法打开输入文件: " << input_file_path << std::endl;
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    lexer.setInput(buffer.str());
    file.close();

    /* 设置文法文件 */
    fs::path grammar_path(grammar_file_path);
    // 构造缓存路径的过程，用于保存语法分析的“表”（LR1 的状态表）以及时间戳
    const std::string cache_file_base = grammar_path.replace_extension("").string();
    const std::string cache_file_path = cache_file_base + ".cache";
    const std::string cache_time_path = cache_file_base + ".cache.time";

    if (needsReParsing(grammar_file_path, cache_time_path))
    {
        std::cout << "重新解析文法文件..." << std::endl;
        parser = LR1Parser(grammar_file_path); // 从文法文件生成 LR1 分析器
        parser.save_tables(cache_file_path);   // 将分析表缓存到 .cache 文件
        saveLastModifiedTime(grammar_file_path, cache_time_path);
    }
    else
    {
        parser.load_tables(cache_file_path);
    }
}

bool Compiler::needsReParsing(const std::string &grammar_file_path, const std::string &cache_time_path)
{
    auto grammar_last_write_time = fs::last_write_time(grammar_file_path);

    // 检查缓存时间戳文件是否存在
    if (!fs::exists(cache_time_path))
    {
        return true; // 缓存时间戳文件不存在，需要重新解析
    }

    std::ifstream time_file(cache_time_path);
    std::time_t cached_time;
    time_file >> cached_time;
    if (time_file.fail() || fileTimeToTimeT(grammar_last_write_time) > cached_time)
    {
        return true; // 缓存无效或文法文件更新了，需要重新解析
    }

    return false; // 缓存有效，无需重新解析
}

void Compiler::saveLastModifiedTime(const std::string &grammar_file_path, const std::string &time_file_path)
{
    auto last_write_time = fs::last_write_time(grammar_file_path);
    std::ofstream time_file(time_file_path);
    time_file << fileTimeToTimeT(last_write_time);
}

std::time_t Compiler::fileTimeToTimeT(const std::filesystem::file_time_type &ftime)
{
    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
    return std::chrono::system_clock::to_time_t(sctp);
}

void Compiler::compile()
{
    try
    {
        // parser.print_tables();
        parser.stateStack.push(0); // 初始化状态栈，压入初始状态 0（文法分析入口）
        Token token;               // 定义 token 用于读取词法分析器返回的结果
        do
        {
            token = lexer.getNextToken();                                                    // 获取下一个 Token（词法分析）
            Symbol currentSymbol(SymbolType::Terminal, token.type_to_string(), token.value); // 构造一个当前处理的语法符号（Symbol），类型为终结符，内容为 token 的类型和值

            std::cout << "Symbol:" << currentSymbol.to_string() << std::endl;

            bool shouldContinue = true;
            while (shouldContinue) // 如果当前需要继续处理（如 REDUCE 后还要再处理），则循环
            {
                Action action = parser.get_next_action(currentSymbol); // 根据当前符号从语法分析器中获得下一步动作（Action）
                std::cout << action.to_string() << std::endl;
                switch (action.type)
                {
                case Action::Type::SHIFT: // 如果是 SHIFT（移入），说明当前符号合法
                {
                    shouldContinue = false;                            // 本轮循环结束
                    semanticAnalyzer.updateShiftSymbol(currentSymbol); // 语义分析器记录移入的符号
                    parser.stateStack.push(action.number);             // 状态栈压入新状态
                    parser.symbolStack.push(currentSymbol);            // 符号栈压入当前符号
                    break;
                }
                case Action::Type::REDUCE: // 如果是 REDUCE（规约），从栈中弹出产生式右侧符号，组成 RHS
                {
                    try {
                        // std::cerr << "[reduce] 规约前: stateStack.size=" << parser.stateStack.size()
                        //           << ", symbolStack.size=" << parser.symbolStack.size()
                        //           << ", lhs=" << action.production.lhs.to_string()
                        //           << ", rhs.size=" << action.production.rhs.size() << std::endl;
                        std::vector<Symbol> rhs;
                        Symbol lhs = action.production.lhs;

                        for (size_t i = 0; i < action.production.rhs.size(); ++i)
                        {
                            rhs.insert(rhs.begin(), parser.symbolStack.top()); // 将栈顶符号插入 RHS
                            parser.symbolStack.pop();                          // 弹出符号栈
                            parser.stateStack.pop();                           // 弹出状态栈
                        }
                        // std::cerr << "[reduce] 弹栈后: stateStack.size=" << parser.stateStack.size()
                        //           << ", symbolStack.size=" << parser.symbolStack.size() << std::endl;

                        semanticAnalyzer.doSemanticAction(lhs, rhs); // 执行对应语义动作（如生成中间代码）

                        parser.symbolStack.push(lhs); // 将规约后的非终结符压回符号栈
                        // std::cerr << "[reduce] lhs入栈后: symbolStack.size=" << parser.symbolStack.size() << std::endl;

                        // // 检查是否规约到文法入口且输入为EOF，直接ACCEPT
                        // if (lhs.literal == "<Program>" && currentSymbol.type == SymbolType::Terminal && currentSymbol.literal == "T_EOF") {
                        //     std::cout << "[ACCEPT] 规约到文法入口 <Program> 且输入为EOF，分析成功！" << std::endl;
                        //     // 输出全局四元式和中间代码
                        //     QuaterList global_quaters = semanticAnalyzer.get_global_quater_list();
                        //     std::cout << "四元式列表长度：" << global_quaters.size() << std::endl;
                        //     std::cout << "===== 全局四元式列表 =====" << std::endl;
                        //     global_quaters.display();
                        //     CodeList finalCode = semanticAnalyzer.getFinalCode();
                        //     finalCode.display(output_file_path, show_output);
                        //     std::cout << "成功" << std::endl;
                        //     return;
                        // }

                        int nextState = parser.gotoTable.at({parser.stateStack.top(), action.production.lhs}); // 查表获得下一个状态，并压入状态栈
                        parser.stateStack.push(nextState);
                        // std::cerr << "[reduce] goto后: stateStack.size=" << parser.stateStack.size() << ", 压入新状态: " << nextState << std::endl;
                        shouldContinue = true; // 规约后继续 while 循环，支持连续规约
                        break;
                    } catch (const std::exception& e) {
                        std::cerr << "[REDUCE][EXCEPTION] what(): " << e.what() << std::endl;
                        std::cerr << "[REDUCE][EXCEPTION] stateStack.size=" << parser.stateStack.size() << ", symbolStack.size=" << parser.symbolStack.size() << std::endl;
                        // return; // 直接终止编译，防止后续查表
                    }
                }
                case Action::Type::ACCEPT: // 接受状态：编译成功
                {
                    // 在接受状态时，检查所有控制流语句是否合法
                    try {
                        // 检查所有 break/continue 语句是否在循环内
                        semanticAnalyzer.checkControlFlowStatements();
                        
                        // 获取并输出全局四元式
                        QuaterList global_quaters = semanticAnalyzer.get_global_quater_list();
                        std::cout << "四元式列表长度：" << global_quaters.size() << std::endl;
                        std::cout << "===== 全局四元式列表 =====" << std::endl;
                        global_quaters.display();

                        CodeList finalCode = semanticAnalyzer.getFinalCode();
                        finalCode.display(output_file_path, show_output); // 输出最终中间代码到文件

                        std::cout << "成功" << std::endl;
                    } catch (const std::exception& e) {
                        std::cerr << "[ACCEPT][EXCEPTION] 控制流语句检查失败: " << e.what() << std::endl;
                        std::cerr << "编译失败" << std::endl;
                    }
                    return;
                }
                default: // 遇到非法动作，说明语法错误
                {
                    std::cerr << "失败" << std::endl;
                    std::cerr << lexer.getCurrentRowCol() << std::endl;
                    return;
                }
                }
            }
        } while (token.type != T_EOF);
    }
    catch (const std::exception& e)
    {
        std::cerr << "[catch] 最后查找失败的gotoTable key: state=" << lastGotoState << ", lhs=" << lastGotoLhs.to_string() << std::endl;
        std::cerr << "[Compiler::compile] 捕获到std::exception: " << e.what() << std::endl;
        std::cerr << "编译失败" << std::endl;
        std::cerr << lexer.getCurrentRowCol() << std::endl;
    }
    catch (...)
    {
        std::cerr << "[Compiler::compile] 捕获到未知异常。" << std::endl;
        std::cerr << "编译失败" << std::endl;
        std::cerr << lexer.getCurrentRowCol() << std::endl;
    }
}
