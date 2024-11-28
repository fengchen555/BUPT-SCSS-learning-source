# Project 2 - Semantic Analysis





**注意：这里是以project1为基础，继续进行编写。其中lex.l与syntax.y的基础部分和node文件已存在。**





文件目录为：

![image-20231207230721205](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231207230721205.png)



## Abstract Data Types

由哈希表实现，维护一个ID与数据类型的哈希表

```c
extern unordered_map<string, Type *> symbolTable;
```



并在语法解析，生成语法解析树时，生成哈希表。

## Scope Checking

在先前的Abstract Data Types中的哈希表，变量名与数据类型的哈希表中可以查看是否定义过，声明过。

如类型错误2

![image-20231208001145193](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231208001145193.png)

类型错误3：

![image-20231208001211122](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231208001211122.png)

等等。。。





## TypeChecking

在type代码中定义多种type，并定义多种数据类型的类，结合前面的数据类型的哈希表，可以检查数据类型是否符合各种情况已经具体的值。如赋值语句、返回类型。

```c++
#ifndef _TYPE_H //防止头文件的内容被重复包含
#define _TYPE_H
#include <string>
#include <vector>
#include <unordered_map>
//表示类型的类的声明
using std::string;
using std::unordered_map;
using std::vector;

enum class PRIM;
enum class CATEGORY;
class Array;
class FieldList;
class Type;


// 枚举类型，表示基本数据类型
enum class PRIM
{
    INT,
    FLOAT,
    CHAR
};

// 枚举类型，表示类型的种类
enum class CATEGORY
{
    PRIMITIVE,
    ARRAY,
    STRUCTURE,
    FUNCTION,
};

class Type
{
public:
    string name;        // 数据类型的名称
    enum CATEGORY category;  // 数据类型的种类
    union
    {
        enum PRIM primitive;   // 基本数据类型
        Array *array;          // 数组类型
        FieldList *structure;  // 结构体类型
        FieldList *param;      // 函数参数类型
    } foo;
    Type *returnType = nullptr; // 函数返回类型

public:
    // 不同构造函数用于创建不同种类的 Type 对象
    explicit Type(string name, enum CATEGORY category, enum PRIM primitive);
    explicit Type(string name, enum CATEGORY category, Array *array);
    explicit Type(string name, enum CATEGORY category, FieldList *structure);
    explicit Type(string name, enum CATEGORY category, FieldList *param, Type *returnType);

    // 获取基本数据类型的 Type 对象
    static Type *getPrimitiveINT();
    static Type *getPrimitiveCHAR();
    static Type *getPrimitiveFLOAT();

    // 析构函数
    ~Type() = default;
};

// 表示数组类型的类
class Array
{
public:
    int size;      // 数组的大小
    Type *base = nullptr;  // 数组的基本类型

public:
    // 构造函数
    Array() = default;
    Array(int size, Type *base);

    // 析构函数
    ~Array() = default;
};

// 表示结构体中的字段的类
class FieldList
{
public:
    string name;      // 字段的名称
    Type *type = nullptr;  // 字段的数据类型
    FieldList *next = nullptr;  // 下一个字段

public:
    // 构造函数
    FieldList() = default;
    FieldList(string name, Type *type);

    // 析构函数
    ~FieldList() = default;
};
//表示函数的类的声明
class Function
{
public:
    string name;                                // 函数名
    vector<Type *> parameter;                   // 参数列表
    Type *return_type;                          // 返回值
    unordered_map<string, Type> varSymbolTable; // scope参数符号表

public:
    Function(string name, vector<Type *> parameter, Type *return_type);
    ~Function() = default;
};
#endif  /* _TYPE_H */
```







## Required rules

在semanticError.hpp中声明15种语法解析错误，并在semanticError.cpp中定义输出格式

```c
//semanticError.hpp
Required rules 
定义语义解析错误函数，分类处理语义错误
semantic errors:
*/
// error type 1
void variableNoDefinition_1(int lineNum, const std::string &name);

// error type 2
void functionNoDefinition_2(int lineNum, const std::string &name);

// error type 3
void variableRedefined_3(int lineNum, const std::string &name);

// error type 4
void functionRedefined_4(int lineNum, const std::string &name);

// error type 5
void nonMatchTypeBothSide_5(int lineNum);

// error type 6
void rvalueLeftSetError_6(int lineNum);

// error type 7
void binaryOperatorNotValid_7(int lineNum);

// error type 7
void singleOperatorNotValid_7(int lineNum);

// error type 8
void returnTypeDisMatch_8(int lineNum);

// error type 9
void invalidArgumentNumber_9(int lineNum, const std::string &name, int expect, int actually);

//error type 10
void indexingOnNonArray_10(int lineNum);

// error type 11
void invokeNonFunctionVariable_11(int lineNum, const std::string &name);

//error type 12
void indexingByNonInteger_12(int lineNum);

// error type 13
void nonStructFVariable_13(int lineNum);

// error type 14
void noSuchMember_14(int lineNum, const std::string &name);

// error type 15
void structRedefined_15(int lineNum, const std::string &name);
```

并在syntaxTree代码中对语法树解析时，检查语义有无错误，并输出错误

![image-20231207234208253](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231207234208253.png)

```c
#ifndef _SYNTAXTREE_H
#define _SYNTAXTREE_H

#include <string>
#include <unordered_map>
#include <iterator>
#include <set>
//----------------------------------------------------------------
#include "node.hpp"
#include "type.hpp"
#include "semanticAnalysisUtil.hpp"
#include "semanticError.hpp"




using std::string;
using std::unordered_map;

// External symbol table that stores variable and function information
extern unordered_map<string, Type *> symbolTable;

// Function declarations for semantic analysis
void idToExp(Node *exp, Node *id);
string getName(Node *node, string nodeName);
Array *getArrayFromVarDec(Node *node, Type *type);
void getArrayType(Node *expOut, Node *expIn, Node *Integer);
void checkExists_ID(Node *node);
void checkExists_Array(Node *Exp);
void checkExist_FUN(Node *id);
void checkParam_FUN(Node *id, Node *args);
void ExtDefVisit_SES(Node *node);
void ExtDefVisit_SS(Node *node);
void ExtDefVisit_SFC(Node *ExtDef);
void getNamesOfDefList(Node *node, vector<Node *> &namesofFileds);
FieldList *getFiledListFromNodesVector(const vector<Node *> &vec);
void FunDecVisit(Node *node);
void getVarList(Node *node);
void getParamDec(Node *node);
void defVisit(Node *node);
void getDecList(Node *node);
void getVarDec(Node *node);
void getReturnTypeOfFunction(Node *expOut, Node *ID);
void checkRvalueInLeftSide(Node *Exp);
void checkAssignmentTypeMatching(Node *outExp, Node *leftExp, Node *rightExp);
void checkTypeOfDot(Node *expOut, Node *expIn, Node *ID);
void checkReturnType(Node *ExtDef);
#endif//_SYNTAXTREE_
```







## Code introduction

### lex.l

不需要改变

### node

在节点的类中增加一个指针

```c++
#ifndef _NODE_HPP_
#define _NODE_HPP_
#include <string>
#include <iostream>
#include <vector>//向量
#include <initializer_list>//初始化列表
#include "type.hpp"
using namespace std;

// 节点类型的枚举
enum class TYPE
{
    MEDIAN,  // 中间节点
    OTHER,   // 其他节点
    TYPE,    // 类型节点
    INT,     // 整数节点
    CHAR,    // 字符节点
    FLOAT,   // 浮点数节点
    ID,      // 标识符节点
    NOTHING, // 空节点
};
//最后生成的AST树的节点类型
// AST 节点类
class Node
{
public:

    enum TYPE type;            // 节点类型
    string name;               // AST 节点的名称
    string content;            // 节点内容，可能是字符、整数、浮点数等
    int line_num;              // 节点在源代码中的行号
    vector<Node *> child;      // 子节点的指针向量
    Type *var = nullptr;       // 增加一个指向类型的指针，表示该节点最后的类型

public:
    // 构造函数
    explicit Node(enum TYPE type, string name, const string &content, int line);

    // 析构函数
    ~Node() = default;

    // 添加子节点
    void addChild(initializer_list<Node *> childs);

    // 打印节点及其子节点信息
    static void print(Node *node, long depth);
};
#endif
```



### type

存放数据类型的类

```c++
#ifndef _TYPE_H //防止头文件的内容被重复包含   type.hpp
#define _TYPE_H
#include <string>
#include <vector>
#include <unordered_map>
//表示类型的类的声明
using std::string;
using std::unordered_map;
using std::vector;

enum class PRIM;
enum class CATEGORY;
class Array;
class FieldList;
class Type;


// 枚举类型，表示基本数据类型
enum class PRIM
{
    INT,
    FLOAT,
    CHAR
};

// 枚举类型，表示类型的种类
enum class CATEGORY
{
    PRIMITIVE,
    ARRAY,
    STRUCTURE,
    FUNCTION,
};

class Type
{
public:
    string name;        // 数据类型的名称
    enum CATEGORY category;  // 数据类型的种类
    union
    {
        enum PRIM primitive;   // 基本数据类型
        Array *array;          // 数组类型
        FieldList *structure;  // 结构体类型
        FieldList *param;      // 函数参数类型
    } foo;
    Type *returnType = nullptr; // 函数返回类型

public:
    // 不同构造函数用于创建不同种类的 Type 对象
    explicit Type(string name, enum CATEGORY category, enum PRIM primitive);
    explicit Type(string name, enum CATEGORY category, Array *array);
    explicit Type(string name, enum CATEGORY category, FieldList *structure);
    explicit Type(string name, enum CATEGORY category, FieldList *param, Type *returnType);

    // 获取基本数据类型的 Type 对象
    static Type *getPrimitiveINT();
    static Type *getPrimitiveCHAR();
    static Type *getPrimitiveFLOAT();

    // 析构函数
    ~Type() = default;
};

// 表示数组类型的类
class Array
{
public:
    int size;      // 数组的大小
    Type *base = nullptr;  // 数组的基本类型

public:
    // 构造函数
    Array() = default;
    Array(int size, Type *base);

    // 析构函数
    ~Array() = default;
};

// 表示结构体中的字段的类
class FieldList
{
public:
    string name;      // 字段的名称
    Type *type = nullptr;  // 字段的数据类型
    FieldList *next = nullptr;  // 下一个字段

public:
    // 构造函数
    FieldList() = default;
    FieldList(string name, Type *type);

    // 析构函数
    ~FieldList() = default;
};
//表示函数的类的声明
class Function
{
public:
    string name;                                // 函数名
    vector<Type *> parameter;                   // 参数列表
    Type *return_type;                          // 返回值
    unordered_map<string, Type> varSymbolTable; // scope参数符号表

public:
    Function(string name, vector<Type *> parameter, Type *return_type);
    ~Function() = default;
};
#endif  /* _TYPE_H */
```



### syntaxTree

syntaxTree存放对生成语法树/语义树时，语义解析错误类型的判断并调用对应的错误处理

```c++
#ifndef _SYNTAXTREE_H//syntaxTree.hpp
#define _SYNTAXTREE_H

#include <string>
#include <unordered_map>
#include <iterator>
#include <set>
//----------------------------------------------------------------
#include "node.hpp"
#include "type.hpp"
#include "semanticAnalysisUtil.hpp"
#include "semanticError.hpp"




using std::string;
using std::unordered_map;

// External symbol table that stores variable and function information
extern unordered_map<string, Type *> symbolTable;

// Function declarations for semantic analysis
void idToExp(Node *exp, Node *id);
string getName(Node *node, string nodeName);
Array *getArrayFromVarDec(Node *node, Type *type);
void getArrayType(Node *expOut, Node *expIn, Node *Integer);
void checkExists_ID(Node *node);
void checkExists_Array(Node *Exp);
void checkExist_FUN(Node *id);
void checkParam_FUN(Node *id, Node *args);
void ExtDefVisit_SES(Node *node);
void ExtDefVisit_SS(Node *node);
void ExtDefVisit_SFC(Node *ExtDef);
void getNamesOfDefList(Node *node, vector<Node *> &namesofFileds);
FieldList *getFiledListFromNodesVector(const vector<Node *> &vec);
void FunDecVisit(Node *node);
void getVarList(Node *node);
void getParamDec(Node *node);
void defVisit(Node *node);
void getDecList(Node *node);
void getVarDec(Node *node);
void getReturnTypeOfFunction(Node *expOut, Node *ID);
void checkRvalueInLeftSide(Node *Exp);
void checkAssignmentTypeMatching(Node *outExp, Node *leftExp, Node *rightExp);
void checkTypeOfDot(Node *expOut, Node *expIn, Node *ID);
void checkReturnType(Node *ExtDef);
#endif//_SYNTAXTREE_H
```



### semanticAnalysisUtil

semanticAnalysisUtil存放对生成哈希表、语义解析，语义处理、错误判断的辅助函数

```c++
#ifndef _SEMANTICANALYSISUTIL_HPP_//semanticAnalysisUtil.hpp
#define _SEMANTICANALYSISUTIL_HPP_

#include "node.hpp"
#include "semanticError.hpp"
#include "type.hpp"

// 辅助工具函数声明
int countParamNum(FieldList *param);

// 字符串到基本数据类型的映射表
static unordered_map<string, PRIM> string_to_prim = {
    {string("int"), PRIM::INT},
    {string("float"), PRIM::FLOAT},
    {string("char"), PRIM::CHAR},
};

// 基本数据类型到字符串的映射表
static unordered_map<PRIM, string> prim_to_string = {
    {PRIM::INT, string("INT")},
    {PRIM::FLOAT, string("FLOAT")},
    {PRIM::CHAR, string("CHAR")},
};

// 将 FieldList 的 vector 转换为链表
FieldList *vector_to_fieldlist(vector<FieldList *> param);

// 获取参数列表的期望参数个数
int get_expect_param_num(FieldList *param);

// 获取实际参数个数
int get_real_param_num(Node *args);

// 设置布尔运算符表达式的类型
void setBoolOperatorType(Node *expOut, Node *expLeft, Node *expRight);

// 设置比较运算符表达式的类型
void setCompareOperatorType(Node *expOut, Node *expLeft, Node *expRight);

// 设置算术运算符表达式的类型
void setAlrthOperatorType(Node *expOut, Node *expLeft, Node *expRight);

// 设置一元算术运算符表达式的类型
void setAlrthOperatorType(Node *expOut, Node *innerExp);

// 检查表达式的类型是否为整数
bool checkIntegerType(Node *exp);

// 获取复合语句中所有可能含有返回值的表达式节点
vector<Node *> getReturnExpFromCompSt(Node *CompSt);

// 获取语句中所有可能含有返回值的表达式节点
vector<Node *> getReturnExpFromStmt(Node *Stmt);

// 获取数组的维度和类型
std::tuple<vector<int>, Type *> getArrayDemensionAndType(Type *_type);

// 检查节点是否为表达式节点
bool isExpressionNode(Node* node);

// 获取最后一个表达式节点
Node* getLastExpressionNode(Node* currentNode);

#endif // _SEMANTICANALYSISUTIL_HPP_

```

### semanticError

semanticError对语义解析的错误处理以及类型

```c++
#include <string>//semanticError.hpp
#include <unordered_map>
/*

Required rules 
定义语义解析错误函数，分类处理语义错误
semantic errors:
*/
// error type 1
void variableNoDefinition_1(int lineNum, const std::string &name);

// error type 2
void functionNoDefinition_2(int lineNum, const std::string &name);

// error type 3
void variableRedefined_3(int lineNum, const std::string &name);

// error type 4
void functionRedefined_4(int lineNum, const std::string &name);

// error type 5
void nonMatchTypeBothSide_5(int lineNum);

// error type 6
void rvalueLeftSetError_6(int lineNum);

// error type 7
void binaryOperatorNotValid_7(int lineNum);

// error type 7
void singleOperatorNotValid_7(int lineNum);

// error type 8
void returnTypeDisMatch_8(int lineNum);

// error type 9
void invalidArgumentNumber_9(int lineNum, const std::string &name, int expect, int actually);

//error type 10
void indexingOnNonArray_10(int lineNum);

// error type 11
void invokeNonFunctionVariable_11(int lineNum, const std::string &name);

//error type 12
void indexingByNonInteger_12(int lineNum);

// error type 13
void nonStructFVariable_13(int lineNum);

// error type 14
void noSuchMember_14(int lineNum, const std::string &name);

// error type 15
void structRedefined_15(int lineNum, const std::string &name);
```



### syntax.y

在syntax.y中添加语义解析时可能发生错误的检查函数，就是syntaxTree中的一些函数。

如：

![image-20231208002039856](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231208002039856.png)

具体可以查看代码。

### makefile

```makefile
CC=gcc
CPP=g++
FLEX=flex
BISON=bison
node = node.hpp node.cpp
type = type.hpp type.cpp
semanticError = semanticError.hpp semanticError.cpp
semanticAnalysisUtil = semanticAnalysisUtil.hpp semanticAnalysisUtil.cpp
syntaxTree = syntaxTree.hpp syntaxTree.cpp
bplc:
	@rm -rf bin/# 删除已经存在的 bin 目录，加上 || true 避免在 bin 目录不存在时报错
	@mkdir -p bin         # 使用 -p 选项，确保 bin 目录存在，避免目录已存在时报错
	@touch bin/bplc
	@chmod +x bin/bplc
	$(BISON) -t -d syntax.y
	$(FLEX) lex.l
	$(CPP) $(type) $(node)  $(semanticError) $(syntaxTree) $(semanticAnalysisUtil)  syntax.tab.c -lfl -ly -o bin/bplc  # bplc输出路径为: bin/bplc
	
clean:
	@rm -rf bin/

.PHONY: bplc clean

```

### test.py

读取样例，并输出结果

```python
import subprocess
import os

def build_compiler():
    try:
        # 调用 make 命令来编译你的代码
        print("Building compiler...")
        subprocess.run(["make", "bplc"])

    except subprocess.CalledProcessError as e:
        print(f"Error while building compiler: {e}")

def run_compiler(input_file, output_file):
    try:
        # 创建输出文件所在的目录
        os.makedirs(os.path.dirname(output_file), exist_ok=True)

        # 调用编译器并传递输入文件作为参数，并将标准输出重定向到文件
        with open(output_file, 'w') as f:
            subprocess.run(["bin/bplc", input_file], stdout=f)

    except subprocess.CalledProcessError as e:
        print(f"Error while running compiler: {e}")

if __name__ == "__main__":
    # 编译你的代码
    build_compiler()

    # 运行编译器并传递输入文件（替换为你的实际输入文件路径）
    for i in range(1, 16):
        print("----------------------------------------------------------------")
        temp = i
        input_file = "test/test_2_r" + str(temp).zfill(2) + ".bpl"
        print(input_file)
        #output_file = "out/test_1_r" + str(temp).zfill(2) + ".out"
        output_file = "test/test_2_r" + str(temp).zfill(2) + ".out"
        print("Test " + str(i) + ":")
        run_compiler(input_file, output_file)
    print("The output has been saved in the test folder")

```

运行结果：

![image-20231208003204696](C:/Users/24328/AppData/Roaming/Typora/typora-user-images/image-20231208003204696.png)

![image-20231208003229471](C:/Users/24328/AppData/Roaming/Typora/typora-user-images/image-20231208003229471.png)

并在test文件中可以查看结果

![image-20231208003423701](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231208003423701.png)

![image-20231208003433868](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231208003433868.png)

如testr11等等，就不一一举例。

## Project 2 results



make代码：

![image-20231208002257876](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231208002257876.png)

测试样例：

![image-20231208002139659](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231208002139659.png)

文件输出：

![image-20231208002358515](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231208002358515.png)

1-4的测试样例都符合所给的结果，后续的测试样例应该也符合吧，具体结果可以查看文件或者给的截图。

到此实验完成。