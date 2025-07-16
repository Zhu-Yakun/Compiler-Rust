# Define the translation mappings for tokens wrapped in <>
translation_map = {
    "S": "S",

    # 顶层结构
    "Program": "程序",
    "declaration_list": "声明串",
    "declaration": "声明",
    
    "T_INTEGER_LITERAL" : "整数",
    "T_STRING_LITERAL" : "字符串",
    "T_CHAR_LITERAL" : "字符",
    "T_BOOLEAN_LITERAL" : "布尔值",
    "T_FLOAT_LITERAL" : "浮点数",
    "T_EOF": "文件结束符",

    # 0
    "var_declaration" : "变量声明内部",
    "type": "类型",
    "var": "可赋值元素", # T_IDENTIFIER

    # 1
    "compound_stmt": "语句块",
    "stmt_list" : "语句串",
    "stmt" : "语句",
    "return_stmt" : "返回语句",
    "param_list" : "形参列表",
    "param" : "形参",
    # 函数定义
    "fun_declaration": "函数声明",
    "fun_definition" : "函数头声明",

    # 2
    "var_declaration_stmt" : "变量声明语句",
    "assignment_stmt" : "赋值语句",
    "var_declaration_assignment_stmt" : "变量声明赋值语句",
    "expression" : "表达式",

    # 3
    "additive_expression" : "加法表达式",
    "term" : "项",
    "factor" : "因子",
    "element" : "元素",
    "comparison_operator" : "比较运算符",
    "additive_operator" : "加减运算符",
    "multiplicative_operator" : "乘除运算符",
    "fun_call" : "函数调用",
    "argument" : "实参列表",

    # 4
    "selection_stmt" : "选择语句",
    "else_part" : "else部分",

    # 5
    "iteration_stmt" : "循环语句",
    "while_statement" : "while语句",
    "for_statement" : "for语句",
    "iterable_structure" : "可迭代结构",
    "loop_statement" : "循环语句",

    # 6
    "break_stmt" : "跳出语句",
    "continue_stmt" : "继续语句",

    ###########################################################
    # 终结符 token
    "T_IDENTIFIER": "标识符",
    "T_I32": "i32",
    "T_MUT": "mut",
    "T_LET" : "let",
    "T_FN": "fn",
    "T_IF" : "if",
    "T_ELSE" : "else",
    "T_WHILE" : "while",
    "T_FOR" : "for",
    "T_LOOP" : "loop",
    "T_IN" : "in",
    "T_RETURN" : "return",
    "T_BREAK" : "break",
    "T_CONTINUE" : "continue",

    "T_LEFT_PAREN": "(",
    "T_RIGHT_PAREN": ")",
    "T_LEFT_BRACE": "{",
    "T_RIGHT_BRACE": "}",
    "T_MULTIPLY" : "*",
    "T_COLON" : ":",
    "T_COMMA" : ",",
    "T_SEMICOLON" : ";",
    "T_ASSIGN" : "=",
    "T_PLUS" : "+",
    "T_MINUS" : "-",
    "T_DIVIDE" : "/",
    "T_LESSEQUAL" : "<=",
    "T_GREATEREQUAL" : ">=",
    "T_LESS" : "<",
    "T_GREATER" : ">",
    "T_EQUAL" : "==",
    "T_NOTEQUAL" : "!=",
    "T_ARROW" : "->",
    "T_DOTDOT" : "..",
    "T_REFERENCE" : "&",
}


# Modify the function to accept additional parameters for direction and custom file paths

def translate_content(content, translation_map, direction='en_to_zh'):
    """
    Translate content based on the direction.

    Parameters:
    - content: The content to be translated.
    - translation_map: A dictionary mapping source language tokens to target language tokens.
    - direction: The direction of translation ('en_to_zh' for English to Chinese, 'zh_to_en' for Chinese to English).
    """
    if direction == 'en_to_zh':
        for english, chinese in translation_map.items():
            content = content.replace(f"<{english}>", f"<{chinese}>")
        for english, chinese in translation_map.items():
            content = content.replace(f"{english}", f"{chinese}")
    elif direction == 'zh_to_en':
        for english, chinese in translation_map.items():
            content = content.replace(f"<{chinese}>", f"<{english}>")
            content = content.replace(f"{chinese}", f"{english}")
    return content

def translate_file(input_filepath, output_filepath, translation_map, direction='en_to_zh'):
    """
    Translate a file from English to Chinese or vice versa based on the direction.

    Parameters:
    - input_filepath: The path to the input file.
    - output_filepath: The path to the output file.
    - translation_map: A dictionary mapping source language tokens to target language tokens.
    - direction: The direction of translation ('en_to_zh' for English to Chinese, 'zh_to_en' for Chinese to English).
    """
    with open(input_filepath, 'r', encoding='utf-8') as file:
        content = file.read()

    translated_content = translate_content(content, translation_map, direction)

    with open(output_filepath, 'w', encoding='utf-8') as file:
        file.write(translated_content)

    return "Translation successful", output_filepath

# Example usage (the actual file paths and direction need to be provided by the user)
input_filepath = './grammar_zh.txt'  # This should be set by the user
output_filepath = './grammar.txt'  # This should be set by the user
direction = 'zh_to_en' if False else 'en_to_zh'  # or 'zh_to_en', should be set by the user

print("input_filepath:", input_filepath, "output_filepath:", output_filepath, "direction:", direction)
translate_file(output_filepath, input_filepath, translation_map, direction)