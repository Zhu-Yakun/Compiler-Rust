#include "SemanticAnalyzer.hpp"
#include "common/dlog.hpp"
#include "MipsInstructionGenerator/MipsInstructionGenerator.hpp"

// 根据 { 和 } 的出现，来更新变量的作用域深度，并在退出作用域时清除相关的局部变量
void SemanticAnalyzer::updateShiftSymbol(Symbol &symbol)
{
    switch (Token::stringToTokenType(symbol.literal))
    {
    case T_LEFT_BRACE:
        variable_deep++;
        break;
    case T_RIGHT_BRACE:
        if (variable_deep > 0)
        {
            variable_deep--;
            variable_table.pop_until_deep(variable_deep); // 将当前层及以上的局部变量从符号表中移除
        }
        break;
    default:
        break;
    }
}

// 根据产生式左部的非终结符（lhs.literal），调用对应的语义处理函数，对整个右部 rhs 做语义分析处理。
void SemanticAnalyzer::doSemanticAction(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    try {
        std::cerr << "[doSemanticAction] lhs=" << lhs.literal << ", rhs.size=" << rhs.size() << std::endl;
        // <Program> ::= <declaration_list>
        if (lhs.literal == "<Program>")
        {
            handleProgram(lhs, rhs);
        }
        // <declaration_list> ::= <declaration> <declaration_list> | <declaration>
        else if (lhs.literal == "<declaration_list>")
        {
            handleDeclarationList(lhs, rhs);
        }
        // <declaration> ::= <var_declaration> | <fun_declaration> | <stmt_list>
        else if (lhs.literal == "<declaration>")
        {
            handleDeclaration(lhs, rhs);
        }
        // <var> ::= T_IDENTIFIER
        else if (lhs.literal == "<var>")
        {
            handleVar(lhs, rhs);
        }
        // <var_declaration> ::= T_MUT <var> | <var>
        else if (lhs.literal == "<var_declaration>")
        {
            handleVarDeclaration(lhs, rhs);
        }
        // <type> ::= T_I32 | T_F32 | T_CHAR | T_BOOL
        else if (lhs.literal == "<type>")
        {
            handleType(lhs, rhs);
        }
        // <fun_declaration> ::= <fun_definition> <compound_stmt>
        else if (lhs.literal == "<fun_declaration>")
        {
            handleFunDeclaration(lhs, rhs);
        }
        // <fun_definition> ::= T_FN T_IDENTIFIER T_LEFT_PAREN <param_list> T_RIGHT_PAREN | T_FN T_IDENTIFIER T_LEFT_PAREN T_RIGHT_PAREN | T_FN T_IDENTIFIER T_LEFT_PAREN <param_list> T_RIGHT_PAREN T_ARROW <type> | T_FN T_IDENTIFIER T_LEFT_PAREN T_RIGHT_PAREN T_ARROW <type>
        else if (lhs.literal == "<fun_definition>")
        {
            handleFunDefinition(lhs, rhs);
        }
        // <param_list> ::= <param> | <param_list> T_COMMA <param>
        else if (lhs.literal == "<param_list>")
        {
            handleParamList(lhs, rhs);
        }
        // <param> ::= <var_declaration> T_COLON <type>
        else if (lhs.literal == "<param>")
        {
            handleParam(lhs, rhs);
        }
        // <compound_stmt> ::= T_LEFT_BRACE <stmt_list> T_RIGHT_BRACE | T_LEFT_BRACE T_RIGHT_BRACE
        else if (lhs.literal == "<compound_stmt>")
        {
            handleCompoundStmt(lhs, rhs);
        }
        // <stmt_list> ::= <stmt> <stmt_list> | <stmt>
        else if (lhs.literal == "<stmt_list>")
        {
            handleStmtList(lhs, rhs);
        }
        // <stmt> ::=  T_SEMICOLON | <return_stmt> T_SEMICOLON | <var_declaration_stmt> T_SEMICOLON | <assignment_stmt> T_SEMICOLON | <var_declaration_assignment_stmt> T_SEMICOLON | <expression> T_SEMICOLON | <selection_stmt> | <iteration_stmt> | <break_stmt> T_SEMICOLON | <continue_stmt> T_SEMICOLON
        else if (lhs.literal == "<stmt>")
        {
            handleStmt(lhs, rhs);
        }
        // <return_stmt> ::= T_RETURN | T_RETURN <expression>
        else if (lhs.literal == "<return_stmt>")
        {
            handleReturnStmt(lhs, rhs);
        }
        // <var_declaration_stmt> ::= T_LET <var_declaration> T_COLON <type> | T_LET <var_declaration>
        else if (lhs.literal == "<var_declaration_stmt>")
        {
            handleVarDeclarationStmt(lhs, rhs);
        }
        // <assignment_stmt> ::= <var> T_ASSIGN <expression>
        else if (lhs.literal == "<assignment_stmt>")
        {
            handleAssignmentStmt(lhs, rhs);
        }
        // <var_declaration_assignment_stmt> ::= T_LET <var_declaration> T_COLON <type> T_ASSIGN <expression> | T_LET <var_declaration> T_ASSIGN <expression>
        else if (lhs.literal == "<var_declaration_assignment_stmt>")
        {
            handleVarDeclarationAssignmentStmt(lhs, rhs);
        }
        // <expression> ::= <additive_expression> | <expression> <comparison_operator> <additive_expression>
        else if (lhs.literal == "<expression>")
        {
            handleExpression(lhs, rhs);
        }
        // <additive_expression> ::= <term> | <additive_expression> <additive_operator> <term>
        else if (lhs.literal == "<additive_expression>")
        {
            handleAdditiveExpression(lhs, rhs);
        }
        // <term> ::= <factor> | <term> <multiplicative_operator> <factor>
        else if (lhs.literal == "<term>")
        {
            handleTerm(lhs, rhs);
        }
        // <factor> ::= <element>
        else if (lhs.literal == "<factor>")
        {
            handleFactor(lhs, rhs);
        }
        // <element> ::= T_INTEGER_LITERAL | <var> | T_LEFT_PAREN <expression> T_RIGHT_PAREN | <fun_call>
        else if (lhs.literal == "<element>")
        {
            handleElement(lhs, rhs);
        }
        // <fun_call> ::= T_IDENTIFIER T_LEFT_PAREN <argument> T_RIGHT_PAREN | T_IDENTIFIER T_LEFT_PAREN T_RIGHT_PAREN
        else if (lhs.literal == "<fun_call>")
        {
            handleFunCall(lhs, rhs);
        }
        // <argument> ::= <expression> | <expression> T_COMMA <argument>
        else if (lhs.literal == "<argument>")
        {
            handleArgument(lhs, rhs);
        }
        // <additive_operator> ::= T_PLUS | T_MINUS
        else if (lhs.literal == "<additive_operator>")
        {
            handleAdditiveOperator(lhs, rhs);
        }
        // <multiplicative_operator> ::= T_MULTIPLY | T_DIVIDE
        else if (lhs.literal == "<multiplicative_operator>")
        {
            handleMultiplicativeOperator(lhs, rhs);
        }
        // <comparison_operator> ::= T_LESS | T_GREATER | T_EQUAL | T_GREATEREQUAL | T_LESSEQUAL | T_NOTEQUAL
        else if (lhs.literal == "<comparison_operator>")
        {
            handleComparisonOperator(lhs, rhs);
        }
        // <selection_stmt> ::= T_IF <expression> <compound_stmt> | T_IF <expression> <compound_stmt> <else_part>
        else if (lhs.literal == "<selection_stmt>")
        {
            handleSelectionStmt(lhs, rhs);
        }
        // <else_part> ::= T_ELSE <compound_stmt> | T_ELSE T_IF <expression> <compound_stmt> <else_part>
        else if (lhs.literal == "<else_part>")
        {
            handleElsePart(lhs, rhs);
        }
        // <iteration_stmt> ::= <while_statement> | <for_statement> | <loop_statement>
        else if (lhs.literal == "<iteration_stmt>")
        {
            handleIterationStmt(lhs, rhs);
        }
        // <while_statement> ::= T_WHILE <expression> <compound_stmt>
        else if (lhs.literal == "<while_statement>")
        {
            handleWhileStatement(lhs, rhs);
        }
        // <for_statement> ::= T_FOR <var_declaration> T_IN <iterable_structure> <compound_stmt>
        else if (lhs.literal == "<for_statement>")
        {
            handleForStatement(lhs, rhs);
        }
        // <iterable_structure> ::= <expression> T_DOTDOT <expression>
        else if (lhs.literal == "<iterable_structure>")
        {
            handleIterableStructure(lhs, rhs);
        }
        // <loop_statement> ::= T_LOOP <compound_stmt>
        else if (lhs.literal == "<loop_statement>")
        {
            handleLoopStatement(lhs, rhs);
        }
        // <break_stmt> ::= T_BREAK
        else if (lhs.literal == "<break_stmt>")
        {
            handleBreakStmt(lhs, rhs);
        }
        // <continue_stmt> ::= T_CONTINUE
        else if (lhs.literal == "<continue_stmt>")
        {
            handleContinueStmt(lhs, rhs);
        }
    } catch (const std::exception& e) {
        std::cerr << "[doSemanticAction][EXCEPTION] lhs=" << lhs.literal << ", what(): " << e.what() << std::endl;
        throw;
    } catch (...) {
        std::cerr << "[doSemanticAction][UNKNOWN EXCEPTION] lhs=" << lhs.literal << std::endl;
        throw;
    }
}

std::string SemanticAnalyzer::getNewTemp()
{
    // 默认类型全部是INT
    return TEMP_PREFIX + std::to_string(++temp_index);
}

void SemanticAnalyzer::handleProgram(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <Program> ::= <declaration_list>
    // 从declaration_list提取quat
    if (rhs[0].meta.contains("quat")) {
        QuaterList quater_list = rhs[0].meta.get<QuaterList>("quat");
        global_quater_list.append_quaters(quater_list); 
    }

    finalMips = MipsInstructionGenerator::generate_header();       // 生成头部代码 .data .text .globl main
    auto mips_list = std::move(rhs[0].meta.get<CodeList>("code")); // 从 <declaration_list> 的语义信息（meta）中，取出对应的 MIPS 代码 CodeList
    finalMips.append(mips_list);
}

void SemanticAnalyzer::handleDeclarationList(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <declaration_list> ::= <declaration> <declaration_list> | <declaration>
    // using DeclarationList = std::vector<std::vector<Symbol>>;

    QuaterList quat;

    if (rhs.size() == 1)
    {
        if (rhs[0].meta.contains("quat")) {
            quat.append_quaters(rhs[0].meta.get<QuaterList>("quat"));
        }
        lhs.meta.set("quat", quat);

        auto mips = rhs[0].meta.get<CodeList>("code");
        lhs.meta.set("code", mips);
    }
    else if (rhs.size() == 2)
    {
        // 递归合并右侧节点的quat
        if (rhs[1].meta.contains("quat")) {
            quat.append_quaters(rhs[1].meta.get<QuaterList>("quat"));
        }
        // 合并左侧节点的quat
        if (rhs[0].meta.contains("quat")) {
            quat.append_quaters(rhs[0].meta.get<QuaterList>("quat"));
        }
        lhs.meta.set("quat", quat);

        auto mips_list = rhs[1].meta.get<CodeList>("code");
        auto mips = rhs[0].meta.get<CodeList>("code");
        mips_list.code_list.insert(mips_list.code_list.end(), mips.code_list.begin(), mips.code_list.end());
        lhs.meta.set("code", mips_list);
    }

    if (rhs[0].meta.contains("quat")) {
        QuaterList quater_list = rhs[0].meta.get<QuaterList>("quat");
    }
}

void SemanticAnalyzer::handleDeclaration(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    std::cerr << "[handleDeclaration][ENTER] lhs=" << lhs.literal << ", rhs.size=" << rhs.size() << std::endl;
    QuaterList quat;

    if (rhs[0].literal == "<fun_declaration>")
    {
        if (rhs[0].meta.contains("quat")) {
            quat.append_quaters(rhs[0].meta.get<QuaterList>("quat"));
        }
        lhs.meta.set("quat", quat);

        const auto &code = rhs[0].meta.get<CodeList>("code");
        lhs.meta.set("code", code);
        std::cerr << "[handleDeclaration][EXIT] fun_declaration code set, code.size=" << code.code_list.size() << std::endl;
    }
    else if (rhs[0].literal == "<var_declaration>")
    {
        // 变量声明一般无需生成code，但要设置 name/is_mut 等属性，保证后续规约流程不出错
        if (rhs[0].meta.contains("name"))
            lhs.meta.set("name", rhs[0].meta.get<std::string>("name"));
        if (rhs[0].meta.contains("is_mut"))
            lhs.meta.set("is_mut", rhs[0].meta.get<bool>("is_mut"));
        lhs.meta.set("quat", QuaterList());
        lhs.meta.set("code", CodeList());
        std::cerr << "[handleDeclaration][EXIT] var_declaration name=" << (rhs[0].meta.contains("name") ? rhs[0].meta.get<std::string>("name") : "") << std::endl;
        return;
    }
    else if (rhs[0].literal == "<stmt_list>") // TODO 允许顶层语句用于调试
    {
        if (rhs[0].meta.contains("quat"))
        {
            const auto &quat = rhs[0].meta.get<QuaterList>("quat");
            std::cout << "stmt_list quat size: " << quat.size() << std::endl;

            std::string name = "global_stmt_block" + std::to_string(global_stmt_block_index++);
            if (rhs[0].meta.contains("name"))
            {
                name = rhs[0].meta.get<std::string>("name");
            }
            std::cout << "stmt_list name: " << name << std::endl;

            auto mips_generator = MipsInstructionGenerator(quat, name);
            mips_generator.generator();

            lhs.meta.set("quat", quat); // 如果后续还需要中间代码分析
            std::cout << "stmt_list quat:" << quat.to_string() << std::endl;

            lhs.meta.set("code", mips_generator.get_code_list());
            std::cerr << "[handleDeclaration][EXIT] stmt_list code set, code.size=" << mips_generator.get_code_list().code_list.size() << std::endl;
        }
        else
        {
            // 如果没有 quat，可能是空语句块
            lhs.meta.set("code", CodeList{});
            std::cerr << "[handleDeclaration][EXIT] stmt_list empty" << std::endl;
        }
    }
    else
    {
        std::cerr << "[handleDeclaration][EXIT][ERROR] 规约出错" << std::endl;
        throw std::exception();
    }
    std::cerr << "[handleDeclaration][EXIT] lhs.meta: ";
    if (lhs.meta.contains("name")) std::cerr << "name=" << lhs.meta.get<std::string>("name") << ", ";
    if (lhs.meta.contains("is_mut")) std::cerr << "is_mut=" << lhs.meta.get<bool>("is_mut") << ", ";
    if (lhs.meta.contains("code")) std::cerr << "code=SET, ";
    if (lhs.meta.contains("quat")) std::cerr << "quat=SET, ";
    std::cerr << std::endl;
}

void SemanticAnalyzer::handleVar(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <var> ::= T_IDENTIFIER 标识符
    if (rhs.size() != 1)
    {
        std::cerr << "[handleVar] 暂时不能解析数组变量" << std::endl;
    }

    lhs.meta.set("name", rhs[0].real_value);
}

void SemanticAnalyzer::handleVarDeclaration(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <var_declaration> ::= T_MUT <var> | <var>
    // var_declaration 没有type但是var_declaration_stmt和var_declaration_assignment_stmt有type
    if (rhs.size() == 1)
    {
        // <var_declaration> ::= <var> 不可变变量
        const auto &name = rhs[0].meta.get<std::string>("name");
        lhs.meta.set("name", name);
        lhs.meta.set("is_mut", false);
    }
    else if (rhs.size() == 2)
    {
        // <var_declaration> ::= T_MUT <var> 可变变量
        const auto &name = rhs[1].meta.get<std::string>("name");
        lhs.meta.set("name", name);
        lhs.meta.set("is_mut", true);
    }
    else
    {
        std::cerr << "[handleVarDeclaration] 规约出错" << std::endl;
        throw std::exception();
    }
}

void SemanticAnalyzer::handleType(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <type> ::= T_I32 | T_F32 | T_CHAR | T_BOOL
    switch (Token::stringToTokenType(rhs[0].literal))
    {
    case T_F32:
        lhs.meta.set("type", F32);
        break;
    case T_I32:
        lhs.meta.set("type", I32);
        break;
    case T_CHAR:
        lhs.meta.set("type", CHAR);
        break;
    case T_BOOL:
        lhs.meta.set("type", BOOL);
        break;
    default:
        std::cerr << "[handleType] 变量类型不存在：" << rhs[0].literal << std::endl;
        throw std::exception();
        break;
    }
}

void SemanticAnalyzer::handleFunDeclaration(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <fun_declaration> ::= <fun_definition> <compound_stmt>
    QuaterList quat;
    if (rhs[0].meta.contains("quat"))
    {
        quat.append_quaters(rhs[0].meta.get<QuaterList>("quat"));
    }
    if (rhs[1].meta.contains("quat"))
    {
        quat.append_quaters(rhs[1].meta.get<QuaterList>("quat"));
    }

    // 调试输出 meta keys
    std::cerr << "[handleFunDeclaration] rhs[0].meta keys: ";
    for (const auto& k : rhs[0].meta.keys()) std::cerr << k << " ";
    std::cerr << std::endl;
    std::cerr << "[handleFunDeclaration] rhs[1].meta keys: ";
    for (const auto& k : rhs[1].meta.keys()) std::cerr << k << " ";
    std::cerr << std::endl;

    // 检查 return_type 是否存在
    if (!rhs[0].meta.contains("return_type")) {
        std::cerr << "[handleFunDeclaration][ERROR] rhs[0].meta 没有 return_type" << std::endl;
        throw std::exception();
    }
    const auto &def_type = rhs[0].meta.get<VariableType>("return_type");
    VariableType body_type = UNIT;
    if (rhs[1].meta.contains("return_type"))
        body_type = rhs[1].meta.get<VariableType>("return_type");
    if (def_type != body_type)
    {
        std::cerr << "[handleFunDeclaration] 函数返回值类型不一致：定义为 " << to_string(def_type)
                  << "，但函数体返回 " << to_string(body_type) << std::endl;
        throw std::exception();
    }
    VariableType lhs_type = def_type;

    auto name = rhs[0].meta.get<std::string>("name");
    if (show_output)
    {
        std::cout << name << std::endl;
        quat.display();
    }

    // 进入函数体，提升作用域
    variable_deep++;

    lhs.meta.set("quat", quat);

    if (rhs[0].meta.contains("quat")) {
        QuaterList quater_list = rhs[0].meta.get<QuaterList>("quat");
    }

    // 从四元式中生成 MIPS 代码
    auto mips_generator = MipsInstructionGenerator(quat, name);
    mips_generator.generator();
    try {
        std::cerr << "[handleFunDeclaration][DEBUG] before set code" << std::endl;
        lhs.meta.set("code", mips_generator.get_code_list());
        std::cerr << "[handleFunDeclaration][DEBUG] after set code" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[handleFunDeclaration][EXCEPTION] set code what(): " << e.what() << std::endl;
        throw;
    } catch (...) {
        std::cerr << "[handleFunDeclaration][EXCEPTION] set code unknown exception" << std::endl;
        throw;
    }

    // 函数体结束，退出作用域
    variable_table.pop_until_deep(variable_deep);
    variable_deep--;

    std::cerr << "[handleFunDeclaration][END] lhs.meta keys: ";
    for (const auto& k : lhs.meta.keys()) std::cerr << k << " ";
    std::cerr << std::endl;
}

void SemanticAnalyzer::handleFunDefinition(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <fun_definition> ::=
    // T_FN T_IDENTIFIER T_LEFT_PAREN <param_list> T_RIGHT_PAREN |
    // T_FN T_IDENTIFIER T_LEFT_PAREN T_RIGHT_PAREN |
    // T_FN T_IDENTIFIER T_LEFT_PAREN <param_list> T_RIGHT_PAREN T_ARROW <type> |
    // T_FN T_IDENTIFIER T_LEFT_PAREN T_RIGHT_PAREN T_ARROW <type>
    variable_deep++;
    if (rhs.size() == 5)
    {
        // T_FN T_IDENTIFIER T_LEFT_PAREN <param_list> T_RIGHT_PAREN
        std::cout << "handleFunDefinition name:" << rhs[1].real_value << std::endl;
        QuaterList quat;
        const auto &param_list = rhs[3].meta.get<std::vector<Variable>>("param_list");
        for (auto i = 0; i < param_list.size(); i++)
        {
            const auto &param = "param" + std::to_string(i);
            variable_table.add_item(param, param_list[i].type, param_list[i].is_mut, true, variable_deep, i); // 用当前deep
            quat.add_quater("=", param, "", param_list[i].name, variable_table);
        }

        Function function(UNIT, rhs[1].real_value, param_list);
        function_table.add_func(function); // 添加函数到符号表

        lhs.meta.set("quat", quat);
        lhs.meta.set("name", rhs[1].real_value);
        lhs.meta.set("return_type", UNIT);
    }
    else if (rhs.size() == 4)
    {
        // T_FN T_IDENTIFIER T_LEFT_PAREN T_RIGHT_PAREN
        std::cout << "handleFunDefinition name:" << rhs[1].real_value << std::endl;
        Function function(UNIT, rhs[1].real_value, {});
        function_table.add_func(function); // 添加函数到符号表
        lhs.meta.set("name", rhs[1].real_value);
        lhs.meta.set("return_type", UNIT);
    }
    else if (rhs.size() == 7)
    {
        // T_FN T_IDENTIFIER T_LEFT_PAREN <param_list> T_RIGHT_PAREN T_ARROW <type>
        QuaterList quat;

        const auto &param_list = rhs[3].meta.get<std::vector<Variable>>("param_list");
        for (auto i = 0; i < param_list.size(); i++)
        {
            const auto &param = "param" + std::to_string(i);
            variable_table.add_item(param, param_list[i].type, param_list[i].is_mut, true, variable_deep, i); // 用当前deep
            quat.add_quater("=", param, "", param_list[i].name, variable_table);
        }

        Function function(rhs[6].meta.get<VariableType>("type"), rhs[1].real_value, param_list);
        function_table.add_func(function); // 添加函数到符号表

        lhs.meta.set("quat", quat);
        lhs.meta.set("name", rhs[1].real_value);
        lhs.meta.set("return_type", rhs[6].meta.get<VariableType>("type"));
    }
    else if (rhs.size() == 6)
    {
        // T_FN T_IDENTIFIER T_LEFT_PAREN T_RIGHT_PAREN T_ARROW <type>
        Function function(rhs[5].meta.get<VariableType>("type"), rhs[1].real_value, {});
        function_table.add_func(function); // 添加函数到符号表

        lhs.meta.set("name", rhs[1].real_value);
        lhs.meta.set("return_type", rhs[5].meta.get<VariableType>("type"));
    }
    else
    {
        std::cerr << "[handleFunDefinition] 规约出错" << std::endl;
        throw std::exception();
    }
    // 函数体结束，退出作用域
    variable_table.pop_until_deep(variable_deep);
    variable_deep--;
}

void SemanticAnalyzer::handleParamList(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <param_list> ::= <param> | <param_list> T_COMMA <param>
    if (rhs.size() == 1)
    {
        // <param_list> ::= <param>
        auto name = rhs[0].meta.get<std::string>("name");
        auto type = rhs[0].meta.get<VariableType>("type");
        auto is_mut = rhs[0].meta.get<bool>("is_mut");
        std::vector<Variable> param_list;
        param_list.emplace_back(name, type, is_mut, true, 1);
        lhs.meta.set("param_list", param_list);
        variable_table.add_item(name, type, is_mut, true, 1, (int)param_list.size() - 1);
    }
    else
    {
        // <param_list> ::= <param_list> T_COMMA <param>
        auto name = rhs[2].meta.get<std::string>("name");
        auto type = rhs[2].meta.get<VariableType>("type");
        auto is_mut = rhs[2].meta.get<bool>("is_mut");
        auto param_list = rhs[0].meta.get<std::vector<Variable>>("param_list");
        param_list.emplace_back(name, type, is_mut, true, 1);
        lhs.meta.set("param_list", param_list);
        variable_table.add_item(name, type, is_mut, true, 1, (int)param_list.size() - 1);
    }
}

void SemanticAnalyzer::handleParam(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <param> ::= <var_declaration> T_COLON <type>
    if (rhs.size() == 3)
    {
        // <param> ::= <var_declaration> T_COLON <type>
        lhs.meta.set("name", rhs[0].meta.get<std::string>("name"));
        lhs.meta.set("type", rhs[2].meta.get<VariableType>("type"));
        lhs.meta.set("is_mut", rhs[0].meta.get<bool>("is_mut"));
    }
    else
    {
        std::cerr << "[handleParam] 规约有误" << std::endl;
        throw std::exception();
    }
}

void SemanticAnalyzer::handleCompoundStmt(Symbol &lhs, const std::vector<Symbol> &rhs, bool is_loop_body)
{
    // <compound_stmt> ::= T_LEFT_BRACE <stmt_list> T_RIGHT_BRACE | T_LEFT_BRACE T_RIGHT_BRACE
    
    // 如果是循环体，增加循环体嵌套深度
    if (is_loop_body) {
        in_loop_body_count++;
        std::cout << "[handleCompoundStmt] 进入循环体，in_loop_body_count = " << in_loop_body_count << std::endl;
    }
    if (rhs.size() == 2)
    {
        // <compound_stmt> ::= T_LEFT_BRACE T_RIGHT_BRACE
        lhs.meta.set("quat", QuaterList());
    }
    else if (rhs.size() == 3)
    {
        // <compound_stmt> ::= T_LEFT_BRACE <stmt_list> T_RIGHT_BRACE
        QuaterList quat;
        if (rhs[1].meta.contains("quat"))
            quat = rhs[1].meta.get<QuaterList>("quat");
        if (rhs[1].meta.contains("return_type"))
            lhs.meta.set("return_type", rhs[1].meta.get<VariableType>("return_type"));
        lhs.meta.set("quat", quat);
        // 传递 break/continue
        if (rhs[1].meta.contains("break_list"))
            lhs.meta.set("break_list", rhs[1].meta.get<std::vector<size_t>>("break_list"));
        if (rhs[1].meta.contains("continue_list"))
            lhs.meta.set("continue_list", rhs[1].meta.get<std::vector<size_t>>("continue_list"));
    }
    else
    {
        std::cerr << "[handleCompoundStmt]规约出错" << std::endl;
        throw std::exception();
    }
    if (!lhs.meta.contains("code")) {
        lhs.meta.set("code", CodeList{});
    }
    
    // 如果是循环体，减少循环体嵌套深度
    if (is_loop_body) {
        in_loop_body_count--;
        std::cout << "[handleCompoundStmt] 退出循环体，in_loop_body_count = " << in_loop_body_count << std::endl;
    }
    return;
}

void SemanticAnalyzer::handleStmtList(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <stmt_list> ::= <stmt> <stmt_list> | <stmt>
    if (rhs.size() == 1)
    {
        // <stmt_list> ::= <stmt>
        if (rhs[0].meta.contains("quat"))
            lhs.meta.set("quat", rhs[0].meta.get<QuaterList>("quat"));

        // 处理return类型传递
        if (rhs[0].meta.contains("return_type"))
            lhs.meta.set("return_type", rhs[0].meta.get<VariableType>("return_type"));
        // 传递 break/continue
        if (rhs[0].meta.contains("break_list"))
            lhs.meta.set("break_list", rhs[0].meta.get<std::vector<size_t>>("break_list"));
        if (rhs[0].meta.contains("continue_list"))
            lhs.meta.set("continue_list", rhs[0].meta.get<std::vector<size_t>>("continue_list"));
    }
    else if (rhs.size() == 2)
    {
        // <stmt_list> ::= <stmt> <stmt_list>
        QuaterList quat;
        if (rhs[0].meta.contains("quat"))
            quat.append_quaters(rhs[0].meta.get<QuaterList>("quat"));
        if (rhs[1].meta.contains("quat"))
            quat.append_quaters(rhs[1].meta.get<QuaterList>("quat"));
        lhs.meta.set("quat", quat);

        // 处理return类型传递
        if (rhs[1].meta.contains("return_type"))
            lhs.meta.set("return_type", rhs[1].meta.get<VariableType>("return_type"));
        // 合并 break/continue
        std::vector<size_t> break_list, continue_list;
        if (rhs[0].meta.contains("break_list"))
            break_list = rhs[0].meta.get<std::vector<size_t>>("break_list");
        if (rhs[1].meta.contains("break_list"))
        {
            auto b = rhs[1].meta.get<std::vector<size_t>>("break_list");
            break_list.insert(break_list.end(), b.begin(), b.end());
        }
        if (!break_list.empty())
            lhs.meta.set("break_list", break_list);
        if (rhs[0].meta.contains("continue_list"))
            continue_list = rhs[0].meta.get<std::vector<size_t>>("continue_list");
        if (rhs[1].meta.contains("continue_list"))
        {
            auto c = rhs[1].meta.get<std::vector<size_t>>("continue_list");
            continue_list.insert(continue_list.end(), c.begin(), c.end());
        }
        if (!continue_list.empty())
            lhs.meta.set("continue_list", continue_list);
    }
    if (!lhs.meta.contains("code")) {
        lhs.meta.set("code", CodeList{});
    }
}

void SemanticAnalyzer::handleStmt(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <stmt> ::=  T_SEMICOLON | <return_stmt> T_SEMICOLON | <var_declaration_stmt> T_SEMICOLON | <assignment_stmt> T_SEMICOLON | <var_declaration_assignment_stmt> T_SEMICOLON | <expression> T_SEMICOLON | <selection_stmt> | <iteration_stmt> | <break_stmt> T_SEMICOLON | <continue_stmt> T_SEMICOLON
    if (rhs[0].meta.contains("quat"))
        lhs.meta.set("quat", rhs[0].meta.get<QuaterList>("quat"));
    // if (rhs[0].meta.contains("type"))
    //     lhs.meta.set("type", rhs[0].meta.get<VariableType>("type"));
    if (rhs[0].meta.contains("return_type"))
        lhs.meta.set("return_type", rhs[0].meta.get<VariableType>("return_type"));
    // 传递 break/continue
    if (rhs[0].meta.contains("break_list"))
        lhs.meta.set("break_list", rhs[0].meta.get<std::vector<size_t>>("break_list"));
    if (rhs[0].meta.contains("continue_list"))
        lhs.meta.set("continue_list", rhs[0].meta.get<std::vector<size_t>>("continue_list"));
    if (!lhs.meta.contains("code")) {
        lhs.meta.set("code", CodeList{});
    }
}

void SemanticAnalyzer::handleReturnStmt(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <return_stmt> ::= T_RETURN | T_RETURN <expression>
    if (rhs.size() == 1)
    {
        QuaterList quat;
        quat.add_quater("return", "", "", "", variable_table);
        lhs.meta.set("quat", quat);
        lhs.meta.set("return_type", UNIT);
    }
    else if (rhs.size() == 2)
    {
        QuaterList quat;
        if (rhs[1].meta.contains("quat"))
        {
            quat.append_quaters(rhs[1].meta.get<QuaterList>("quat"));
        }
        auto var = rhs[1].meta.get<std::string>("value");
        quat.add_quater("return", var, "", "", variable_table);
        lhs.meta.set("quat", quat);
        lhs.meta.set("return_type", rhs[1].meta.get<VariableType>("type"));
        std::cout << "[handleReturnStmt] return_type: " << rhs[1].meta.get<VariableType>("type") << std::endl;
    }
    if (!lhs.meta.contains("code")) {
        lhs.meta.set("code", CodeList{});
    }
}

void SemanticAnalyzer::handleVarDeclarationStmt(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <var_declaration_stmt> ::= T_LET <var_declaration> T_COLON <type> | T_LET <var_declaration>
    if (rhs.size() == 4)
    {
        // <var_declaration_stmt> ::= T_LET <var_declaration> T_COLON <type>
        const auto &name = rhs[1].meta.get<std::string>("name");
        const auto &type = rhs[3].meta.get<VariableType>("type");
        variable_table.add_item(name, type, rhs[1].meta.get<bool>("is_mut"), false, variable_deep);
    }
    else if (rhs.size() == 2) // 允许遮蔽
    {
        // <var_declaration_stmt> ::= T_LET <var_declaration>
        const auto &name = rhs[1].meta.get<std::string>("name");
        if (!variable_table.add_item(name, UNIT, rhs[1].meta.get<bool>("is_mut"), false, variable_deep))
        {
            std::cerr << "[handleVarDeclarationStmt] " << name << "变量定义出错" << std::endl;
            throw std::exception();
        }
        lhs.meta.set("name", name);
    }
    else
    {
        std::cerr << "[handleVarDeclarationStmt] 规约出错" << std::endl;
        throw std::exception();
    }
    if (!lhs.meta.contains("code")) {
        lhs.meta.set("code", CodeList{});
    }
}

void SemanticAnalyzer::handleAssignmentStmt(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <assignment_stmt> ::= <var> T_ASSIGN <expression>
    const std::string &name = rhs[0].meta.get<std::string>("name");
    auto item = variable_table.lookup(name);

    if (!item.has_value())
    {
        std::cerr << "[handleAssignmentStmt] 未定义变量：" << name << ", 当前作用域深度: " << variable_deep << std::endl;
        std::cerr << "符号表内容: ";
        variable_table.debug_print();
        throw std::exception();
    }

    // 类型检查（Rust 是强类型语言）
    const VariableType type1 = item.value().type;
    const VariableType &type2 = rhs[2].meta.get<VariableType>("type");
    if (type2 != type1)
    {
        std::cerr << "[handleAssignmentStmt] 变量 " << name << " 的类型为 " << type1 << "，但被赋值为 " << type2 << std::endl;
        throw std::exception();
    }

    // 检查变量是否可变
    if (!item->is_mut)
    {
        std::cerr << "[handleAssignmentStmt] 变量 " << name << " 是不可变变量，不能赋值" << std::endl;
        throw std::exception();
    }
    item->is_initial = true;

    lhs.meta.set("value", name);

    QuaterList quat;
    if (rhs[2].meta.contains("quat"))
    {
        quat.append_quaters(rhs[2].meta.get<QuaterList>("quat"));
    }

    const std::string &value2 = rhs[2].meta.get<std::string>("value");
    quat.add_quater("=", value2, "", name, variable_table);

    lhs.meta.set("quat", quat);
    if (!lhs.meta.contains("code")) {
        lhs.meta.set("code", CodeList{});
    }
}

void SemanticAnalyzer::handleVarDeclarationAssignmentStmt(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <var_declaration_assignment_stmt> ::= T_LET <var_declaration> T_COLON <type> T_ASSIGN <expression> |
    // T_LET <var_declaration> T_ASSIGN <expression>
    if (rhs.size() == 4)
    {
        // <var_declaration_assignment_stmt> ::= T_LET <var_declaration> T_ASSIGN <expression>
        const auto &name = rhs[1].meta.get<std::string>("name");

        const VariableType &type = rhs[3].meta.get<VariableType>("type");
        if (type == UNIT)
        {
            std::cerr << "[handleVarDeclarationAssignmentStmt] 不可以将无返回值赋值给变量 " << name << std::endl;
            throw std::exception();
        }
        variable_table.add_item(name, type, rhs[1].meta.get<bool>("is_mut"), true, variable_deep);
        const auto &value = rhs[3].meta.get<std::string>("value");

        QuaterList quat;
        if (rhs[3].meta.contains("quat"))
            quat.append_quaters(rhs[3].meta.get<QuaterList>("quat"));

        quat.add_quater("=", value, "", name, variable_table);
        lhs.meta.set("quat", quat);
    }
    else if (rhs.size() == 6)
    {
        // <var_declaration_assignment_stmt> ::= T_LET <var_declaration> T_COLON <type> T_ASSIGN <expression>
        const auto &name = rhs[1].meta.get<std::string>("name");

        const VariableType &type1 = rhs[3].meta.get<VariableType>("type");
        const VariableType &type2 = rhs[5].meta.get<VariableType>("type");
        if (type2 != type1)
        {
            std::cerr << "[handleVarDeclarationAssignmentStmt] 变量 " << name << "类型不一致" << std::endl;
            throw std::exception();
        }
        if (type1 == UNIT)
        {
            std::cerr << "[handleVarDeclarationAssignmentStmt] 不可以将无返回值赋值给变量 " << name << std::endl;
            throw std::exception();
        }

        variable_table.add_item(name, type1, rhs[1].meta.get<bool>("is_mut"), true, variable_deep);
        const auto &value = rhs[5].meta.get<std::string>("value");

        QuaterList quat;
        if (rhs[3].meta.contains("quat"))
        {
            quat.append_quaters(rhs[2].meta.get<QuaterList>("quat"));
        }

        quat.add_quater("=", value, "", name, variable_table);
        lhs.meta.set("quat", quat);
    }
    else
    {
        std::cerr << "[handleVarDeclarationAssignmentStmt] 规约出错:" << rhs.size() << std::endl;
        throw std::exception();
    }
    if (!lhs.meta.contains("code")) {
        lhs.meta.set("code", CodeList{});
    }
}

void SemanticAnalyzer::handleExpression(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <expression> ::= <additive_expression> |
    // <expression> <comparison_operator> <additive_expression>
    if (rhs.size() == 1)
    {
        // <expression> ::= <additive_expression>
        lhs.meta.set("type", rhs[0].meta.get<VariableType>("type"));
        lhs.meta.set("value", rhs[0].meta.get<std::string>("value"));
        if (rhs[0].meta.contains("quat"))
            lhs.meta.set("quat", rhs[0].meta.get<QuaterList>("quat"));
        else
            lhs.meta.set("quat", QuaterList());
    }
    else if (rhs.size() == 3)
    {
        // <expression> ::= <expression> <comparison_operator> <additive_expression>
        const std::string &op = rhs[1].meta.get<std::string>("op");
        const VariableType &type1 = rhs[0].meta.get<VariableType>("type");
        const VariableType &type2 = rhs[2].meta.get<VariableType>("type");

        if (type1 != type2)
        {
            std::cerr << "[handleExpression] 比较操作符左右类型不一致：" << to_string(type1) << " vs. " << to_string(type2) << std::endl;
            throw std::exception();
        }

        lhs.meta.set("type", BOOL); // 比较表达式结果为 bool 类型

        const std::string &value1 = rhs[0].meta.get<std::string>("value");
        const std::string &value2 = rhs[2].meta.get<std::string>("value");

        QuaterList quat;
        if (rhs[0].meta.contains("quat"))
            quat.append_quaters(rhs[0].meta.get<QuaterList>("quat"));
        if (rhs[2].meta.contains("quat"))
            quat.append_quaters(rhs[2].meta.get<QuaterList>("quat"));

        std::vector<size_t> true_list, false_list;

        // 使用条件跳转来替代简单比较结果存储
        size_t true_id = quat.add_quater("j" + op, value1, value2, "", variable_table); // e.g., j== a, b, _
        true_list.push_back(true_id);

        size_t false_id = quat.add_quater("j", "", "", "", variable_table); // unconditional jump placeholder
        false_list.push_back(false_id);

        lhs.meta.set("quat", quat);
        lhs.meta.set("true_list", true_list);
        lhs.meta.set("false_list", false_list);
    }
    else
    {
        std::cerr << "[handleExpression] 规约出错" << std::endl;
        throw std::exception();
    }
    if (!lhs.meta.contains("code")) {
        lhs.meta.set("code", CodeList{});
    }
}

void SemanticAnalyzer::handleAdditiveExpression(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <additive_expression> ::= <term> | <additive_expression> <additive_operator> <term>
    if (rhs.size() == 1)
    {
        // <additive_expression> ::= <term>
        lhs.meta.set("type", rhs[0].meta.get<VariableType>("type"));
        lhs.meta.set("value", rhs[0].meta.get<std::string>("value"));
        if (rhs[0].meta.contains("quat"))
        {
            lhs.meta.set("quat", rhs[0].meta.get<QuaterList>("quat"));
        }
    }
    else if (rhs.size() == 3)
    {
        // <additive_expression> ::= <additive_expression> <additive_operator> <term>
        const std::string &op = rhs[1].meta.get<std::string>("op");
        const VariableType &type1 = rhs[0].meta.get<VariableType>("type");
        const VariableType &type2 = rhs[2].meta.get<VariableType>("type");
        VariableType type = std::max(type1, type2);
        lhs.meta.set("type", type);

        const std::string &value1 = rhs[0].meta.get<std::string>("value");
        const std::string &value2 = rhs[2].meta.get<std::string>("value");
        auto value = getNewTemp();
        lhs.meta.set("value", value);

        QuaterList quat;
        if (rhs[0].meta.contains("quat"))
            quat.append_quaters(rhs[0].meta.get<QuaterList>("quat"));
        if (rhs[2].meta.contains("quat"))
            quat.append_quaters(rhs[2].meta.get<QuaterList>("quat"));

        quat.add_quater(op, value1, value2, value, variable_table);
        lhs.meta.set("quat", quat);
    }
    else
    {
        std::cerr << "[handleAdditiveExpression] 规约出错" << std::endl;
        throw std::exception();
    }
    if (!lhs.meta.contains("code")) {
        lhs.meta.set("code", CodeList{});
    }
}

void SemanticAnalyzer::handleTerm(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <term> ::= <factor>
    // | <term> <multiplicative_operator> <factor>
    if (rhs.size() == 1)
    {
        // <term> ::= <factor>
        lhs.meta.set("type", rhs[0].meta.get<VariableType>("type"));
        lhs.meta.set("value", rhs[0].meta.get<std::string>("value"));
        if (rhs[0].meta.contains("quat"))
        {
            lhs.meta.set("quat", rhs[0].meta.get<QuaterList>("quat"));
        }
    }
    else if (rhs.size() == 3)
    {
        // <term> ::= <term> <multiplicative_operator> <factor>
        const std::string &op = rhs[1].meta.get<std::string>("op");
        const VariableType &type1 = rhs[0].meta.get<VariableType>("type");
        const VariableType &type2 = rhs[2].meta.get<VariableType>("type");
        VariableType type = std::max(type1, type2);
        lhs.meta.set("type", type);

        const std::string &value1 = rhs[0].meta.get<std::string>("value");
        const std::string &value2 = rhs[2].meta.get<std::string>("value");
        auto value = getNewTemp();
        lhs.meta.set("value", value);

        QuaterList quat;
        if (rhs[0].meta.contains("quat"))
            quat.append_quaters(rhs[0].meta.get<QuaterList>("quat"));
        if (rhs[2].meta.contains("quat"))
            quat.append_quaters(rhs[2].meta.get<QuaterList>("quat"));

        quat.add_quater(op, value1, value2, value, variable_table);

        lhs.meta.set("quat", quat);
    }
    else
    {
        std::cerr << "[handleTerm] 规约出错" << std::endl;
        throw std::exception();
    }
    if (!lhs.meta.contains("code")) {
        lhs.meta.set("code", CodeList{});
    }
}

void SemanticAnalyzer::handleFactor(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <factor> ::= <element>
    lhs.meta.set("type", rhs[0].meta.get<VariableType>("type"));
    lhs.meta.set("value", rhs[0].meta.get<std::string>("value"));
    if (rhs[0].meta.contains("quat"))
        lhs.meta.set("quat", rhs[0].meta.get<QuaterList>("quat"));
    if (!lhs.meta.contains("code")) {
        lhs.meta.set("code", CodeList{});
    }
}

void SemanticAnalyzer::handleElement(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <element> ::= T_INTEGER_LITERAL | <var>
    // | T_LEFT_PAREN <expression> T_RIGHT_PAREN | <fun_call>
    if (rhs.size() == 3)
    {
        // <element> ::= T_LEFT_PAREN <expression> T_RIGHT_PAREN
        lhs.meta.set("type", rhs[1].meta.get<VariableType>("type"));
        lhs.meta.set("value", rhs[1].meta.get<std::string>("value"));
        if (rhs[1].meta.contains("quat"))
            lhs.meta.set("quat", rhs[1].meta.get<QuaterList>("quat"));
    }
    else if (rhs.size() == 1)
    {
        // <element> ::= T_INTEGER_LITERAL | <var> | <fun_call>
        const std::string &_literal = rhs[0].literal;
        if (_literal == "<var>")
        {
            // <element> ::= <var>
            auto name = rhs[0].meta.get<std::string>("name");
            auto item = variable_table.lookup(name);
            if (!item.has_value())
            {
                std::cerr << "[handleElement] 未定义变量：" << name << ", 当前作用域深度: " << variable_deep << std::endl;
                std::cerr << "符号表内容: ";
                variable_table.debug_print();
                throw std::exception();
            }
            if (!item->is_initial)
            {
                std::cerr << "[handleElement] 变量未初始化：" << name << std::endl;
                throw std::exception();
            }
            lhs.meta.set("type", item.value().type);
            lhs.meta.set("value", name);
        }
        else if (_literal == "<fun_call>")
        {
            // <element> ::= <fun_call>
            auto type = rhs[0].meta.get<VariableType>("type");
            lhs.meta.set("type", type);

            if (type == VariableType::UNIT)
                lhs.meta.set("value", to_string(type));
            else
                lhs.meta.set("value", rhs[0].meta.get<std::string>("value"));

            auto quat = rhs[0].meta.get<QuaterList>("quat");
            lhs.meta.set("quat", quat);
        }
        else if (_literal == "T_INTEGER_LITERAL")
        {
            // <element> ::= T_INTEGER_LITERAL
            lhs.meta.set("type", VariableType::I32);
            lhs.meta.set("value", rhs[0].real_value);
        }
        else
        {
            std::cerr << "[handleElement] 规约出错" << std::endl;
            throw std::exception();
        }
    }
    if (!lhs.meta.contains("code")) {
        lhs.meta.set("code", CodeList{});
    }
}

void SemanticAnalyzer::handleFunCall(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <fun_call> ::=
    // T_IDENTIFIER T_LEFT_PAREN <argument> T_RIGHT_PAREN |
    // T_IDENTIFIER T_LEFT_PAREN T_RIGHT_PAREN
    std::string fun_name = rhs[0].real_value;

    auto func = function_table.lookup(fun_name);
    if (!func.has_value())
    {
        std::cerr << "[handleFunCall] 未定义函数: " << fun_name << std::endl;
        throw std::exception();
    }

    QuaterList quat;

    size_t args_num = 0;
    std::vector<Variable> args;
    // 有变量，将变量传值
    if (rhs.size() == 4)
    {
        // <fun_call> ::= T_IDENTIFIER T_LEFT_PAREN <argument> T_RIGHT_PAREN
        args = rhs[2].meta.get<std::vector<Variable>>("args");
        args_num = args.size();
        if (rhs[2].meta.contains("quat"))
            quat.append_quaters(rhs[2].meta.get<QuaterList>("quat"));
        for (size_t i = 0; i < args.size(); i++)
            quat.add_quater("param", args[i].name, std::to_string(args.size()), std::to_string(i), variable_table);
    }

    // 检查函数参数数量
    if (args_num != func.value().formal_variables.size())
    {
        std::cerr << "[handleFunCall] 函数参数数量不匹配" << std::endl;
        throw std::exception();
    }

    // 检查函数参数类型
    for (size_t i = 0; i < args_num; i++)
    {
        const auto &arg = args[i];
        const auto &formal_var = func.value().formal_variables[i];
        if (arg.type != formal_var.type || arg.is_mut != formal_var.is_mut || !arg.is_initial)
        {
            std::cerr << "[handleFunCall] 函数参数类型不匹配，或者未赋初值" << std::endl;
            throw std::exception();
        }
    }

    // 有无返回值
    if (func.value().return_type == VariableType::UNIT)
    {
        auto _id = quat.add_quater("call", fun_name, std::to_string(args_num), "", variable_table);
        quat[_id].second.meta.set("args", args);
        lhs.meta.set("value", "");
        lhs.meta.set("type", func.value().return_type);
    }
    else
    {
        std::string value = getNewTemp();
        auto _id = quat.add_quater("call", fun_name, std::to_string(args_num), value, variable_table);
        quat[_id].second.meta.set("args", args);
        lhs.meta.set("value", value);
        lhs.meta.set("type", func.value().return_type);
    }

    lhs.meta.set("quat", quat);
    if (!lhs.meta.contains("code")) {
        lhs.meta.set("code", CodeList{});
    }
}

void SemanticAnalyzer::handleArgument(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <argument> ::= <expression> | <argument> T_COMMA <expression>
    if (rhs.size() == 1)
    {
        // <argument> ::= <expression>
        std::vector<Variable> args;
        auto value = rhs[0].meta.get<std::string>("value");
        auto type = rhs[0].meta.get<VariableType>("type");
        // auto is_mut = rhs[0].meta.get<bool>("is_mut");
        args.emplace_back(value, type, -1, true, true);

        lhs.meta.set("args", args);
        if (rhs[0].meta.contains("quat"))
            lhs.meta.set("quat", rhs[0].meta.get<QuaterList>("quat"));
    }
    else
    {
        // <argument> ::= <argument> T_COMMA <expression>
        auto args = rhs[0].meta.get<std::vector<Variable>>("args");
        auto value = rhs[2].meta.get<std::string>("value");
        auto type = rhs[2].meta.get<VariableType>("type");
        // auto is_mut = rhs[2].meta.get<bool>("is_mut"); // TODO
        auto var = variable_table.lookup(value);
        if (var.has_value())
            args.emplace_back(var.value());
        else
            args.emplace_back(value, type, -1, true, true);

        lhs.meta.set("args", args);

        QuaterList quat;

        if (rhs[0].meta.contains("quat"))
        {
            const auto &quat1 = rhs[0].meta.get<QuaterList>("quat");
            quat.append_quaters(quat1);
        }
        if (rhs[2].meta.contains("quat"))
        {
            const auto &quat2 = rhs[2].meta.get<QuaterList>("quat");
            quat.append_quaters(quat2);
        }

        lhs.meta.set("quat", quat);
    }
    if (!lhs.meta.contains("code")) {
        lhs.meta.set("code", CodeList{});
    }
}

void SemanticAnalyzer::handleAdditiveOperator(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <additive_operator> ::= T_PLUS | T_MINUS
    lhs.meta.set("op", rhs[0].real_value);
}

void SemanticAnalyzer::handleMultiplicativeOperator(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <multiplicative_operator> ::= T_MULTIPLY | T_DIVIDE
    lhs.meta.set("op", rhs[0].real_value);
}

void SemanticAnalyzer::handleComparisonOperator(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <comparison_operator> ::= T_LESS | T_GREATER | T_EQUAL | T_GREATEREQUAL | T_LESSEQUAL | T_NOTEQUAL
    lhs.meta.set("op", rhs[0].real_value);
}

void SemanticAnalyzer::handleSelectionStmt(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <selection_stmt> ::=
    // T_IF <expression> <compound_stmt> |
    // T_IF <expression> <compound_stmt> <else_part>
    auto quat = rhs[1].meta.get<QuaterList>("quat");
    const auto &true_list = rhs[1].meta.get<std::vector<size_t>>("true_list");
    const auto &false_list = rhs[1].meta.get<std::vector<size_t>>("false_list");

    const auto &then_quat = rhs[2].meta.get<QuaterList>("quat");
    const auto &true_exit = quat.append_quaters(then_quat); // compound_stmt

    // type
    if (rhs[2].meta.contains("return_type"))
        lhs.meta.set("return_type", rhs[2].meta.get<VariableType>("return_type"));

    if (rhs.size() == 3)
    {
        // 没有 else，false 直接跳转到语句后
        quat.set_bool_exit(true_list, true_exit);
        quat.set_bool_exit(false_list, quat.size());
        lhs.meta.set("quat", quat);
    }
    else if (rhs.size() == 4)
    {
        // 有 else_part
        const auto &else_quat = rhs[3].meta.get<QuaterList>("quat"); // else_part

        quat.add_quater("j", "", "", std::to_string(quat.size() + else_quat.size() + 1), variable_table);
        const auto &false_exit = quat.append_quaters(else_quat);

        quat.set_bool_exit(true_list, true_exit);
        quat.set_bool_exit(false_list, false_exit);

        lhs.meta.set("quat", quat);

        if (rhs[3].meta.contains("type"))
        {
            if (rhs[3].meta.get<VariableType>("type") != rhs[2].meta.get<VariableType>("type"))
            {
                std::cerr << "[handleSelectionStmt] if的返回类型和else返回类型不一致" << std::endl;
                throw std::exception();
            }
        }
    }
    else
    {
        std::cerr << "[handleSelectionStmt] 规约出错" << std::endl;
        throw std::exception();
    }
    if (!lhs.meta.contains("code")) {
        lhs.meta.set("code", CodeList{});
    }
}

void SemanticAnalyzer::handleElsePart(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <else_part> ::=
    // T_ELSE <compound_stmt> |
    // T_ELSE T_IF <expression> <compound_stmt> <else_part>
    if (rhs.size() == 2)
    {
        // T_ELSE <compound_stmt>
        const auto &quat = rhs[1].meta.get<QuaterList>("quat");
        lhs.meta.set("quat", quat);
        if (rhs[1].meta.contains("return_type"))
            lhs.meta.set("return_type", rhs[1].meta.get<VariableType>("return_type"));
    }
    else if (rhs.size() == 5)
    {
        // T_ELSE T_IF <expression> <compound_stmt> <else_part>
        // 递归处理 else if 分支
        auto quat = rhs[2].meta.get<QuaterList>("quat");
        const auto &true_list = rhs[2].meta.get<std::vector<size_t>>("true_list");
        const auto &false_list = rhs[2].meta.get<std::vector<size_t>>("false_list");

        const auto &then_quat = rhs[3].meta.get<QuaterList>("quat");
        const auto &else_quat = rhs[4].meta.get<QuaterList>("quat");

        size_t true_exit = quat.append_quaters(then_quat);
        quat.add_quater("j", "", "", std::to_string(quat.size() + else_quat.size() + 1), variable_table);
        size_t false_exit = quat.append_quaters(else_quat);

        quat.set_bool_exit(true_list, true_exit);
        quat.set_bool_exit(false_list, false_exit);

        lhs.meta.set("quat", quat);
        if (rhs[3].meta.contains("return_type"))
            lhs.meta.set("return_type", rhs[3].meta.get<VariableType>("return_type"));
    }
    else
    {
        std::cerr << "[handleElsePart] 规约出错" << std::endl;
        throw std::exception();
    }
    if (!lhs.meta.contains("code")) {
        lhs.meta.set("code", CodeList{});
    }
}

void SemanticAnalyzer::handleIterationStmt(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <iteration_stmt> ::= <while_statement> | <for_statement> | <loop_statement>
    if (rhs[0].meta.contains("return_type"))
        lhs.meta.set("return_type", rhs[0].meta.get<VariableType>("return_type"));

    // 传递四元式
    if (rhs[0].meta.contains("quat")) {
        lhs.meta.set("quat", rhs[0].meta.get<QuaterList>("quat"));
    }
    if (!lhs.meta.contains("code")) {
        lhs.meta.set("code", CodeList{});
    }
}

void SemanticAnalyzer::handleWhileStatement(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <while_statement> ::= T_WHILE <expression> <compound_stmt>
    variable_deep++;
    loop_deep++; // 进入循环结构，增加循环深度
    std::cout << "[handleWhileStatement] 进入循环结构，loop_deep = " << loop_deep << std::endl;
    auto quat = rhs[1].meta.get<QuaterList>("quat");
    
    // 直接设置循环体标记
    in_loop_body_count++; // 增加循环体嵌套深度
    std::cout << "[handleWhileStatement] 设置循环体标记，in_loop_body_count = " << in_loop_body_count << std::endl;
    
    // 更新已收集的控制流语句的状态
    // 在这里，我们可以将所有已收集但尚未标记为在循环内的控制流语句更新为在循环内
    for (auto &stmt : control_flow_stmts) {
        if (!stmt.in_loop) {
            stmt.in_loop = true;
            std::cout << "[handleWhileStatement] 更新控制流语句状态：" << stmt.type << " 在循环内" << std::endl;
        }
    }
    
    const auto &quat2 = rhs[2].meta.get<QuaterList>("quat");
    const auto &true_list = rhs[1].meta.get<std::vector<size_t>>("true_list");
    const auto &false_list = rhs[1].meta.get<std::vector<size_t>>("false_list");

    const auto &true_exit = quat.append_quaters(quat2);
    size_t loop_entry = 0; // while条件入口
    quat.add_quater("j", "", "", std::to_string(loop_entry), variable_table);

    quat.set_bool_exit(true_list, true_exit);
    quat.set_bool_exit(false_list, quat.size());

    // 回填break/continue
    if (rhs[2].meta.contains("break_list"))
        quat.set_bool_exit(rhs[2].meta.get<std::vector<size_t>>("break_list"), quat.size());
    if (rhs[2].meta.contains("continue_list"))
        quat.set_bool_exit(rhs[2].meta.get<std::vector<size_t>>("continue_list"), loop_entry);

    // while体结束，退出作用域
    variable_table.pop_until_deep(variable_deep);
    variable_deep--;
    loop_deep--; // 退出循环结构，减少循环深度
    in_loop_body_count--; // 减少循环体嵌套深度
    std::cout << "[handleWhileStatement] 退出循环结构，loop_deep = " << loop_deep << ", in_loop_body_count = " << in_loop_body_count << std::endl;

    if (rhs[2].meta.contains("return_type"))
        lhs.meta.set("return_type", rhs[2].meta.get<VariableType>("return_type"));
    lhs.meta.set("quat", quat);
    if (!lhs.meta.contains("code")) {
        lhs.meta.set("code", CodeList{});
    }
}

void SemanticAnalyzer::handleForStatement(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    std::cerr << "[handleForStatement] 进入 for 语句, rhs.size=" << rhs.size() << std::endl;
    try {
        // 进入 for 循环，提升作用域
        variable_deep++;
        loop_deep++; // 进入循环结构，增加循环深度
        in_loop_body_count++; // 增加循环体嵌套深度
        std::cout << "[handleForStatement] 进入循环结构，loop_deep = " << loop_deep << ", in_loop_body_count = " << in_loop_body_count << std::endl;
        
        // 更新已收集的控制流语句的状态
        for (auto &stmt : control_flow_stmts) {
            if (!stmt.in_loop) {
                stmt.in_loop = true;
                std::cout << "[handleForStatement] 更新控制流语句状态：" << stmt.type << " 在循环内" << std::endl;
            }
        }
        const std::string &iter_var = rhs[1].meta.get<std::string>("name");
        const auto &range_start = rhs[3].meta.get<std::string>("range_start");
        const auto &range_end = rhs[3].meta.get<std::string>("range_end");
        std::cerr << "[handleForStatement] iter_var=" << iter_var << ", range_start=" << range_start << ", range_end=" << range_end << ", variable_deep=" << variable_deep << std::endl;
        // 输出变量表内容
        std::cerr << "[handleForStatement] 变量表内容（进入for前）：" << std::endl;
        variable_table.debug_print();

        auto quat = rhs[3].meta.get<QuaterList>("quat"); // 可迭代结构
        const auto &body_quat = rhs[4].meta.get<QuaterList>("quat");

        // 修正：循环变量作用域应为 variable_deep
        variable_table.add_item(iter_var, I32, true, true, variable_deep);
        std::cerr << "[handleForStatement] add_item done" << std::endl;
        std::cerr << "[handleForStatement] 变量表内容（添加循环变量后）：" << std::endl;
        variable_table.debug_print();

        // i = start
        quat.add_quater("=", range_start, "", iter_var, variable_table);
        std::cerr << "[handleForStatement] add_quater i=start done" << std::endl;

        size_t loop_entry = quat.size();

        // 条件跳转：if i >= end then exit
        std::string temp = getNewTemp();
        std::cerr << "[handleForStatement] 临时变量 temp: " << temp << std::endl;
        quat.add_quater(">=", iter_var, range_end, temp, variable_table);
        std::cerr << "[handleForStatement] add_quater >= done" << std::endl;

        std::vector<size_t> false_list, true_list;
        size_t jump_id = quat.add_quater("jif", temp, "", "", variable_table);
        false_list.push_back(jump_id);
        std::cerr << "[handleForStatement] add_quater jif done" << std::endl;

        // 循环体
        quat.append_quaters(body_quat);
        std::cerr << "[handleForStatement] append_quaters body done" << std::endl;

        // 记录 continue 跳转点
        size_t continue_entry = quat.size();

        // 自增 i = i + 1
        std::string i_plus = getNewTemp();
        std::cerr << "[handleForStatement] 临时变量 i_plus: " << i_plus << std::endl;
        quat.add_quater("+", iter_var, "1", i_plus, variable_table);
        quat.add_quater("=", i_plus, "", iter_var, variable_table);
        std::cerr << "[handleForStatement] add_quater i++ done" << std::endl;

        // 回跳
        quat.add_quater("j", "", "", std::to_string(loop_entry), variable_table);
        std::cerr << "[handleForStatement] add_quater j(loop_entry) done" << std::endl;

        // 回填 false_list 到结尾
        quat.set_bool_exit(false_list, quat.size(), loop_entry);
        std::cerr << "[handleForStatement] set_bool_exit false_list done" << std::endl;

        // 回填 break/continue
        if (rhs[4].meta.contains("break_list"))
            quat.set_bool_exit(rhs[4].meta.get<std::vector<size_t>>("break_list"), quat.size());
        if (rhs[4].meta.contains("continue_list"))
            quat.set_bool_exit(rhs[4].meta.get<std::vector<size_t>>("continue_list"), continue_entry);
        std::cerr << "[handleForStatement] set_bool_exit break/continue done" << std::endl;

        // for 体结束，退出作用域
        variable_table.pop_until_deep(variable_deep);
        variable_deep--;
        loop_deep--; // 退出循环结构，减少循环深度
        in_loop_body_count--; // 减少循环体嵌套深度
        std::cout << "[handleForStatement] 退出循环结构，loop_deep = " << loop_deep << ", in_loop_body_count = " << in_loop_body_count << std::endl;
        std::cerr << "[handleForStatement] pop_until_deep done, variable_deep=" << variable_deep << std::endl;
        std::cerr << "[handleForStatement] 变量表内容（for结束后）：" << std::endl;
        variable_table.debug_print();

        lhs.meta.set("quat", quat);
        if (rhs[4].meta.contains("return_type"))
            lhs.meta.set("return_type", rhs[4].meta.get<VariableType>("return_type"));
        std::cerr << "[handleForStatement] 结束 for 语句" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[handleForStatement][EXCEPTION] what(): " << e.what() << std::endl;
        throw;
    } catch (...) {
        std::cerr << "[handleForStatement][UNKNOWN EXCEPTION]" << std::endl;
        throw;
    }
    if (!lhs.meta.contains("code")) {
        lhs.meta.set("code", CodeList{});
    }
}

void SemanticAnalyzer::handleIterableStructure(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <iterable_structure> ::= <expression> T_DOTDOT <expression>
    std::cerr << "[handleIterableStructure] 进入可迭代结构, rhs.size=" << rhs.size() << std::endl;
    if (rhs[0].meta.contains("value"))
        std::cerr << "[handleIterableStructure] range_start value: " << rhs[0].meta.get<std::string>("value") << std::endl;
    if (rhs[2].meta.contains("value"))
        std::cerr << "[handleIterableStructure] range_end value: " << rhs[2].meta.get<std::string>("value") << std::endl;
    lhs.meta.set("range_start", rhs[0].meta.get<std::string>("value"));
    lhs.meta.set("range_end", rhs[2].meta.get<std::string>("value"));

    QuaterList quat;
    if (rhs[0].meta.contains("quat"))
        quat.append_quaters(rhs[0].meta.get<QuaterList>("quat"));
    if (rhs[2].meta.contains("quat"))
        quat.append_quaters(rhs[2].meta.get<QuaterList>("quat"));
    lhs.meta.set("quat", quat);

    if (rhs[0].meta.contains("type"))
        lhs.meta.set("type", rhs[0].meta.get<VariableType>("type"));
    std::cerr << "[handleIterableStructure] 结束可迭代结构" << std::endl;
    if (!lhs.meta.contains("code")) {
        lhs.meta.set("code", CodeList{});
    }
}

void SemanticAnalyzer::handleLoopStatement(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // <loop_statement> ::= T_LOOP <compound_stmt>
    variable_deep++;
    loop_deep++; // 进入循环结构，增加循环深度
    in_loop_body_count++; // 增加循环体嵌套深度
    std::cout << "[handleLoopStatement] 进入循环结构，loop_deep = " << loop_deep << ", in_loop_body_count = " << in_loop_body_count << std::endl;
    
    // 更新已收集的控制流语句的状态
    for (auto &stmt : control_flow_stmts) {
        if (!stmt.in_loop) {
            stmt.in_loop = true;
            std::cout << "[handleLoopStatement] 更新控制流语句状态：" << stmt.type << " 在循环内" << std::endl;
        }
    }
    
    // 创建四元式列表
    QuaterList quat;
    
    // 记录循环入口点
    size_t loop_entry = quat.size();
    
    // 添加循环体的四元式
    if (rhs[1].meta.contains("quat")) {
        quat.append_quaters(rhs[1].meta.get<QuaterList>("quat"));
    }
    
    // 添加无条件跳转回循环开始
    quat.add_quater("j", "", "", std::to_string(loop_entry), variable_table);
    
    // 回填break/continue
    if (rhs[1].meta.contains("break_list"))
        quat.set_bool_exit(rhs[1].meta.get<std::vector<size_t>>("break_list"), quat.size());
    if (rhs[1].meta.contains("continue_list"))
        quat.set_bool_exit(rhs[1].meta.get<std::vector<size_t>>("continue_list"), loop_entry);
    
    // loop体结束，退出作用域
    variable_table.pop_until_deep(variable_deep);
    variable_deep--;
    loop_deep--; // 退出循环结构，减少循环深度
    in_loop_body_count--; // 减少循环体嵌套深度
    std::cout << "[handleLoopStatement] 退出循环结构，loop_deep = " << loop_deep << ", in_loop_body_count = " << in_loop_body_count << std::endl;
    
    lhs.meta.set("quat", quat);
    if (rhs[1].meta.contains("return_type"))
        lhs.meta.set("return_type", rhs[1].meta.get<VariableType>("return_type"));
    if (!lhs.meta.contains("code")) {
        lhs.meta.set("code", CodeList{});
    }
}

void SemanticAnalyzer::handleBreakStmt(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // 记录当前的 break 语句，用于延迟检查
    std::cout << "[handleBreakStmt] 当前 loop_deep = " << loop_deep << ", in_loop_body_count = " << in_loop_body_count << std::endl;
    
    // 创建一个控制流语句记录
    ControlFlowStmt stmt;
    stmt.type = "break";
    stmt.location = "未知位置"; // 理想情况下应该包含文件名和行号
    stmt.in_loop = (loop_deep > 0); // 记录当前是否在循环内，但不立即检查
    
    // 将语句添加到列表中，稍后检查
    control_flow_stmts.push_back(stmt);
    
    // 生成四元式，不进行错误检查
    QuaterList quat;
    size_t id = quat.add_quater("j", "", "", "", variable_table);
    std::vector<size_t> break_list = {id};
    lhs.meta.set("break_list", break_list);
    lhs.meta.set("quat", quat);
    if (!lhs.meta.contains("code")) {
        lhs.meta.set("code", CodeList{});
    }
}

void SemanticAnalyzer::handleContinueStmt(Symbol &lhs, const std::vector<Symbol> &rhs)
{
    // 记录当前的 continue 语句，用于延迟检查
    std::cout << "[handleContinueStmt] 当前 loop_deep = " << loop_deep << ", in_loop_body_count = " << in_loop_body_count << std::endl;
    
    // 创建一个控制流语句记录
    ControlFlowStmt stmt;
    stmt.type = "continue";
    stmt.location = "未知位置"; // 理想情况下应该包含文件名和行号
    stmt.in_loop = (loop_deep > 0); // 记录当前是否在循环内，但不立即检查
    
    // 将语句添加到列表中，稍后检查
    control_flow_stmts.push_back(stmt);
    
    // 生成四元式，不进行错误检查
    QuaterList quat;
    size_t id = quat.add_quater("j", "", "", "", variable_table);
    std::vector<size_t> continue_list = {id};
    lhs.meta.set("continue_list", continue_list);
    lhs.meta.set("quat", quat);
    if (!lhs.meta.contains("code")) {
        lhs.meta.set("code", CodeList{});
    }
}

SemanticAnalyzer::SemanticAnalyzer(bool show_output) : show_output(show_output), in_loop_body_count(0)
{
    Function read(VariableType::I32, "read", {});
    function_table.add_func(read);
    Function write(VariableType::UNIT, "write", {Variable("a", VariableType::I32, 0)});
    function_table.add_func(write);
}

bool SemanticAnalyzer::checkControlFlowStatements() {
    bool all_valid = true;
    
    // 检查所有收集到的控制流语句
    for (const auto &stmt : control_flow_stmts) {
        if (!stmt.in_loop) {
            std::cerr << "[checkControlFlowStatements] 错误：" << stmt.type << " 语句必须在循环结构内使用，位置：" << stmt.location << std::endl;
            all_valid = false;
        }
    }
    
    // 如果有无效的控制流语句，抛出异常
    if (!all_valid) {
        throw std::exception();
    }
    
    return all_valid;
}
