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