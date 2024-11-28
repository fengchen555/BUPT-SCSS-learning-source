#ifndef _SEMANTICANALYSISUTIL_HPP_
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
