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