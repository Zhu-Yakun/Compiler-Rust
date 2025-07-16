<xxx> : 非终结符
xxx ：终结符

#### 0.1变量声明内部
- <var_declaration>(变量声明内部) ::= T_MUT(mut) T_IDENTIFIER(变量名) T_SEMICOLON(;)
- eg：mut a;

#### 0.2语句
- grammar
```
<type> ::= T_I32
```
- eg. let mut a: i32;

#### 0.3可赋值元素
- grammar
```
T_IDENTIFIER
```
- eg. a = 1;

#### 1.1基础程序
- grammar
```
<fun_declaration> ::= <fun_definition> <compound_stmt>
<fun_definition> ::= T_FN T_IDENTIFIER T_LEFT_PAREN <param_list> T_RIGHT_PAREN | T_FN T_IDENTIFIER T_LEFT_PAREN T_RIGHT_PAREN
<compound_stmt> ::= T_LEFT_BRACE <stmt_list> T_RIGHT_BRACE | T_LEFT_BRACE T_RIGHT_BRACE
```

- eg.
```rust
fn program_1_1() {}
```

#### 1.2语句.txt
- grammar
```
<expression_stmt>(表达式语句) ::= <expression>(表达式) T_SEMICOLON
```

#### 1.3返回语句.txt

<语句>-> <返回语句>
<返回语句> -> return ';'

- grammer
```
<expression> ::= <var> T_ASSIGN <expression> | <additive_expression>
<return_stmt> ::= T_RETURN T_SEMICOLON | T_RETURN <expression> T_SEMICOLON
```
- eg.
``` rust
fn program_1_3() {
    return ;
}
```

#### 1.4函数输入.txt
<形参列表>-> <形参> | <形参> ',' <形参列表>
<形参> -> <变量声明内部> ':' <类型>

- grammer
```
<fun_definition> ::= T_FN T_IDENTIFIER T_LEFT_PAREN <param_list>(形参列表) T_RIGHT_PAREN | T_FN T_IDENTIFIER T_LEFT_PAREN T_RIGHT_PAREN
<param_list> ::= <param> | <param_list> T_COMMA <param>
```
- eg.
```
fn program_1_4(mut a:i32) {
    
}
```

#### 1.5函数输出
<函数头声明> ->  fn <ID> '(' <形参列表> ')' '->' <类型>
<返回语句> -> return <表达式> ';'

- grammer
```
<fun_definition> ::= T_FN T_IDENTIFIER T_LEFT_PAREN <param_list> T_RIGHT_PAREN T_ARROW <type> | T_FN T_IDENTIFIER T_LEFT_PAREN T_RIGHT_PAREN T_ARROW <type>
```

- eg
```
fn program_1_5() -> i32 {
    return 1;
}
```

#### 2.1变量声明语句（前置规则0.1、0.2、1.2）
<语句> -> <变量声明语句>
<变量声明语句> ->  let <变量声明内部> ':'  <类型> ';'
<变量声明语句> ->  let <变量声明内部>  ';'

- grammer
```rust
<stmt> ::= <var_declaration_stmt>

<var_declaration> ::= T_MUT T_IDENTIFIER T_SEMICOLON | T_LET T_MUT T_IDENTIFIER T_COLON <type> T_SEMICOLON | T_LET T_MUT T_IDENTIFIER T_SEMICOLON | T_LET T_IDENTIFIER T_COLON <type> T_SEMICOLON | T_LET T_IDENTIFIER T_SEMICOLON
```

- eg
```
fn program_2_1() {
    let mut a:i32;
    let mut b;
}
```

#### 2.2 赋值语句（前置规则0.3、1.2、3.1）
<语句>-> <赋值语句>
<赋值语句> -> <可赋值元素> '='<表达式> ';'

- grammer
```rust
<stmt> ::= <assignment_stmt> T_SEMICOLON
<assignment_stmt> ::= T_IDENTIFIER T_ASSIGN <expression>
```

- eg
```
fn program_2_2(mut a:i32) {
    a=32;
}
```

#### 2.3 变量声明赋值语句（前置规则0.1、0.2、0.3、1.2、3.1）
<语句>-> <变量声明赋值语句>
<变量声明赋值语句> -> let <变量声明内部> ':'  <类型> '='<表达式> ';'
<变量声明赋值语句> -> let <变量声明内部> '='<表达式> ';'

- grammer
```rust
<stmt> ::= <var_declaration_assignment> T_SEMICOLON
<var_declaration_assignment> ::= T_LET <var_declaration> T_COLON <type> T_ASSIGN <expression> | T_LET <var_declaration> T_ASSIGN <expression>
```

```
fn program_2_3() {
    let mut a:i32=1;
    let mut b=1;
}
```

#### 3.1 基本表达式（前置规则0.3）
<语句> -> <表达式> ';'
<表达式> -> <加法表达式>
<加法表达式> ->  <项> 
<项> -> <因子>
<因子> -> <元素> 
<元素> -> <NUM>  | <可赋值元素> | '(' <表达式> ')'

- grammer
```rust
<stmt> ::=  <expression> T_SEMICOLON
<expression> ::= <additive_expression>
<additive_expression> ::= <term>
<term> ::= <factor>
<factor> ::= <element>
<element> ::= T_INTEGER_LITERAL | T_IDENTIFIER | T_LEFT_PAREN <expression> T_RIGHT_PAREN
```
- eg
```
fn program_3_1__1() {
    0;
    (1);
    ((2));
    (((3)));
}
fn program_3_1__2(mut a:i32) {
    a;
    (a);
    ((a));
    (((a)));
}
```

#### 3.2 表达式增加计算和比较（前置规则3.1）

<表达式> -> <表达式> <比较运算符> <加法表达式>
<加法表达式> -> <加法表达式> <加减运算符> <项>
<项> -> <项> <乘除运算符> <因子>
<比较运算符> -> '<' | '<=' | '>' | '>='  | '==' | '!='
<加减运算符> -> '+' | '-'
<乘除运算符> -> '*' | '/'

<元素> -> <ID> '(' <实参列表> ')'
<实参列表>-> 空 | <表达式> | <表达式> ',' <实参列表>

- grammer
```
<expression> ::= <additive_expression> | <expression> <comparison_operator> <additive_expression>
<additive_expression> ::= <term> | <additive_expression> <additive_operator> <term>
<term> ::= <factor> | <term> <multiplicative_operator> <factor>
<factor> ::= <element>
<element> ::= T_INTEGER_LITERAL | <var> | T_LEFT_PAREN <expression> T_RIGHT_PAREN

<additive_operator> ::= T_PLUS | T_MINUS
<multiplicative_operator> ::= T_MULTIPLY | T_DIVIDE
<comparison_operator> ::= T_LESS | T_GREATER | T_EQUAL | T_GREATEREQUAL | T_LESSEQUAL | T_NOTEQUAL
```

```
<element> ::= T_INTEGER_LITERAL | <var> | T_LEFT_PAREN <expression> T_RIGHT_PAREN | <fun_call>

<fun_call> ::= T_IDENTIFIER T_LEFT_PAREN <argument> T_RIGHT_PAREN | T_IDENTIFIER T_LEFT_PAREN T_RIGHT_PAREN
<argument> ::= <expression> | <expression> T_COMMA <argument>
```

- eg
```
fn program_3_2() {
    1*2/3;
    4+5/6;
    7<8;
    1*2+3*4<4/2-3/1;
}


fn program_3_3__1() {

}

fn program_3_3__2() {
    program_3_3__1();
}
```



#### 4.1 选择结构
<语句> -> <if语句>
<if语句> ->  if <表达式> <语句块> <else部分>
<else部分> -> 空 | else <语句块>

- grammer
```
<selection_stmt> ::= T_IF <expression> <compound_stmt> | T_IF <expression> <compound_stmt> <else_part>
<else_part> ::= T_ELSE <compound_stmt>
```
- eg
```
fn program_4_1(a:i32) -> i32 {
    if a>0 {
        return 1;
    } else {
        return 0;
    }
}
```



#### 4.2 增加else if（前置规则4.1）
<else部分> -> else if <表达式> <语句块> <else部分>

- grammer
```
<selection_stmt> ::= T_IF <expression> <compound_stmt> | T_IF <expression> <compound_stmt> <else_part>
<else_part> ::= T_ELSE <compound_stmt> | T_ELSE T_IF <expression> <compound_stmt> <else_part>
```
- eg
```
fn program_4_2(a:i32) -> i32 {
    if a>0 {
        return a+1;
    } else if a<0 {
        return a-1;
    } else {
        return 0;
    }
}
```

#### 5.1 while循环结构（前置规则1.2、3.1）
<语句> -> <循环语句>
<循环语句> -> <while语句>
<while语句>  -> while <表达式>  <语句块>


- grammer
```
<iteration_stmt> ::= <while_statement> | <for_statement> | <loop_statement>
<while_statement> ::= T_WHILE <expression> <compound_stmt>
```
- eg
```
fn program_5_1(mut n:i32) {
    while n>0 {
        n=n-1;
    }
}

```



#### 5.2 for循环结构（前置规则5.1）
<循环语句> -> <for语句> 
<for语句>->  for <变量声明内部> in  <可迭代结构> <语句块>
<可迭代结构> ->  <表达式> '..' <表达式>


- grammer
```
<for_statement> ::= T_FOR <var_declaration> T_IN <iterable_structure> <compound_stmt>
<iterable_structure> ::= <expression> T_DOTDOT <expression>

```
- eg
```


fn program_5_2(mut n:i32) {
    for mut i in 1..n+1 {
        n=n-1;
    }
}
```



#### 5.3 loop循环结构（前置规则5.1）
<循环语句> -> <loop语句>
<loop语句>->loop  <语句块>


- grammer
```
<loop_statement> ::= T_LOOP <compound_stmt>
```
- eg
```
fn program_5_3() {
    loop {

    }
}
```



#### 5.4 增加break和continue（前置规则5.1）
<语句> -> break ';' | continue ';'

- grammer
```
<stmt> ::=  <break_stmt> T_SEMICOLON | <continue_stmt> T_SEMICOLON
<break_stmt> ::= T_BREAK
<continue_stmt> ::= T_CONTINUE
```
- eg
```
fn program_5_4__1() {
    while 1==0 {
        continue;
    }
}

fn program_5_4__2() {
    while 1==1 {
        break;
    }
}
```

#### 6.1 声明不可变变量（前置规则0.2）
<变量声明内部> -> <ID>

- grammer
```
<var_declaration> ::= T_MUT <var> | <var>
```
- eg
```
fn program_6_1() {
    let a:i32;
    let b;
    let c:i32=1;
    let d=2;
}
```

#### 借用和引用（前置规则3.1、6.1）
<因子> -> '*' <因子>  | '&' mut <因子> | '&' <因子>
<类型>-> '&' mut <类型> | '&' <类型>

- grammer
```
factor
```
- eg
```
fn program_6_2__1() {
    let mut a:i32=1;
    let mut b:&mut i32=&mut a;
    let mut c:i32=*b;
    *b=2;
}
fn program_6_2__2() {
    let a:i32=1;
    let b:& i32=&a;
    let c:i32=*b;
}
```


#### 

- grammer
```

```
- eg
```

```


#### 

- grammer
```

```
- eg
```

```


#### 

- grammer
```

```
- eg
```

```