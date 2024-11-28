#include "node.hpp" // 包含节点类的头文件
#include <iostream>


// Node 类构造函数的实现
Node::Node(TYPE type, string name, const string &content, int line):
    type(type), name(name), content(content), line_num(line) {}

// 向节点添加子节点的函数
void Node::addChild(initializer_list<Node *> childs)
{
    // 遍历传入的初始化列表，并将其中的节点指针添加到当前节点的子节点向量中
    for (auto temp : childs)
    {
        this->child.push_back(temp);
    }
}

// 辅助函数，根据深度在控制台缩进显示节点名称
void print_name(long depth, string name)
{
    for (long i = depth; i > 0; i--)
    {
        printf("  ");
    }
    printf("%s", name.c_str());
}


// 递归打印节点及其子节点信息的函数
void Node::print(Node *node, long depth)
{
    // 根据节点类型选择不同的打印方式
    switch (node->type)
    {
    case TYPE::MEDIAN:
        // 打印中间节点的名称和行号
        print_name(depth, node->name);
        printf(" (%d)\n", node->line_num);
        //debeg用来打印Exp节点的var指针
       if (node->name == "Exp")
        {
            if (node->var == nullptr)
            {
                printf("nullptr\n");
            }
            else
            {
                printf("%p\n", static_cast<void*>(node->var));
            }
        }
        break;
    case TYPE::OTHER:
        // 打印其他类型节点的名称
        print_name(depth, node->name);
        printf("\n");
        break;
    case TYPE::INT:
    case TYPE::FLOAT:
    case TYPE::CHAR:
    case TYPE::ID:
    case TYPE::TYPE:
        // 打印类型节点的名称、内容和行号
        print_name(depth, node->name);
        printf(": %s\n", node->content.c_str());
        break;
    default:
        break;
    }

    // 递归打印子节点
    for (Node *temp : node->child)
    {
        print(temp, depth + 1);
    }
}
