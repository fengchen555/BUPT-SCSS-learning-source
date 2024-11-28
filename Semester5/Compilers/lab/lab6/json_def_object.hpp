// Enums
// 定义布尔值的枚举类型
enum Boolean { FALSE, TRUE };

// 定义空值的枚举类型
enum Null { VNULL };

// Structures
// 定义表示JSON对象的主结构体
struct Json {
    Value *root;  // 指向JSON值的指针 起始json值
};

// 定义表示JSON值的结构体
struct Value {
    union {
        Object *object;           // 对象类型
        Array *array;             // 数组类型
        char *string;             // 字符串类型
        double number;            // 数字类型
        enum Boolean boolean;     // 布尔类型
        enum Null vnull;          // 空值类型
    };
};

// 定义表示JSON对象的结构体
struct Object {
    Member **member;  // 指向成员数组的指针
};

// 定义表示JSON对象成员的结构体
struct Member {
    char *key;       // 指向成员键的字符串指针
    Value *value;    // 指向成员值的指针
};

// 定义表示JSON数组的结构体
struct Array {
    Value **base;    // 指向值数组的指针
};
