#pragma once

#include "LR1Parser/Symbol.hpp"
#include "Lexer/Token.hpp"
#include "Variable/VariableTable.hpp"
#include <cstdlib>
#include "Quater/QuaterList.hpp"
#include "Function/FunctionTable.hpp"

class SemanticAnalyzer
{
private:
    // 定义一个结构来存储 break/continue 语句的位置信息
    struct ControlFlowStmt {
        std::string type;      // "break" 或 "continue"
        std::string location;  // 文件位置信息（行号等）
        bool in_loop;          // 是否在循环内（初始设为 false）
    };
    
    // 存储所有 break/continue 语句的列表，用于延迟检查
    std::vector<ControlFlowStmt> control_flow_stmts;
    
    FunctionTable function_table;
    VariableTable variable_table;
    size_t variable_deep = 0;
    size_t loop_deep = 0;  // 循环结构深度，用于检查 break/continue 是否在循环内
    size_t in_loop_body_count = 0;  // 循环体嵌套深度，用于 break/continue 检查

    size_t temp_index = 0;
    std::string getNewTemp();

    bool show_output;

    int global_stmt_block_index = 0;

    // 添加全局四元式列表
    QuaterList global_quater_list;

    CodeList finalMips;

public:
    SemanticAnalyzer(bool show_output = false);

    /**
     * 移进的时候，要处理的语法制导
     * @param symbol
     */
    void updateShiftSymbol(Symbol &symbol);

    /**
     * 执行语义动作，
     * @param symbol
     * @param vector1
     */
    void doSemanticAction(Symbol &symbol, const std::vector<Symbol> &vector1);
    
    /**
     * 在语法分析完成后检查所有 break/continue 语句是否在循环内
     * @return 如果所有语句都合法返回 true，否则返回 false
     */
    bool checkControlFlowStatements();
    
    // 获取全局四元式的方法
    QuaterList get_global_quater_list() { return global_quater_list; }

    CodeList getFinalCode() { return finalMips; }

private:
    void handleProgram(Symbol &symbol, const std::vector<Symbol> &vector);

    void handleDeclarationList(Symbol &symbol, const std::vector<Symbol> &vector);

    void handleDeclaration(Symbol &symbol, const std::vector<Symbol> &vector);

    void handleVar(Symbol &symbol, const std::vector<Symbol> &vector);

    void handleVarDeclaration(Symbol &lhs, const std::vector<Symbol> &rhs);

    void handleType(Symbol &symbol, const std::vector<Symbol> &vector);

    void handleFunDeclaration(Symbol &lhs, const std::vector<Symbol> &rhs);

    void handleFunDefinition(Symbol &lhs, const std::vector<Symbol> &rhs);

    void handleParamList(Symbol &symbol, const std::vector<Symbol> &vector);

    void handleParam(Symbol &symbol, const std::vector<Symbol> &vector);

    void handleCompoundStmt(Symbol &symbol, const std::vector<Symbol> &vector, bool is_loop_body = false);

    void handleStmtList(Symbol &symbol, const std::vector<Symbol> &vector);

    void handleStmt(Symbol &symbol, const std::vector<Symbol> &vector);

    void handleReturnStmt(Symbol &symbol, const std::vector<Symbol> &vector);

    void handleVarDeclarationStmt(Symbol &symbol, const std::vector<Symbol> &vector);

    void handleAssignmentStmt(Symbol &symbol, const std::vector<Symbol> &vector);

    void handleVarDeclarationAssignmentStmt(Symbol &symbol, const std::vector<Symbol> &vector);

    void handleExpression(Symbol &lhs, const std::vector<Symbol> &rhs);

    void handleAdditiveExpression(Symbol &lhs, const std::vector<Symbol> &rhs);

    void handleTerm(Symbol &lhs, const std::vector<Symbol> &rhs);

    void handleFactor(Symbol &lhs, const std::vector<Symbol> &rhs);

    void handleElement(Symbol &lhs, const std::vector<Symbol> &rhs);

    void handleFunCall(Symbol &symbol, const std::vector<Symbol> &vector);

    void handleArgument(Symbol &symbol, const std::vector<Symbol> &vector);

    void handleAdditiveOperator(Symbol &lhs, const std::vector<Symbol> &rhs);

    void handleMultiplicativeOperator(Symbol &lhs, const std::vector<Symbol> &rhs);

    void handleComparisonOperator(Symbol &lhs, const std::vector<Symbol> &rhs);

    void handleSelectionStmt(Symbol &symbol, const std::vector<Symbol> &vector);

    void handleElsePart(Symbol &symbol, const std::vector<Symbol> &vector);

    void handleIterationStmt(Symbol &symbol, const std::vector<Symbol> &vector);

    void handleWhileStatement(Symbol &symbol, const std::vector<Symbol> &vector);

    void handleForStatement(Symbol &symbol, const std::vector<Symbol> &vector);

    void handleIterableStructure(Symbol &symbol, const std::vector<Symbol> &vector);

    void handleLoopStatement(Symbol &symbol, const std::vector<Symbol> &vector);

    void handleBreakStmt(Symbol &symbol, const std::vector<Symbol> &vector);

    void handleContinueStmt(Symbol &symbol, const std::vector<Symbol> &vector);
};
