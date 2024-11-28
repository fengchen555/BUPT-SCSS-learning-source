# BUPT Compiler Lab5 2023Fall





there are three .c files

\- Read the code in symtab_ll.c

\- Make the target for symtab_ll.c



解释一下symtab_ll.c源代码：

```tex
//symtab_ll.c
//这段代码定义了一个简单的符号表实现，用于存储键值对。主要包括以下几个部分：

//数据结构定义：

entry 结构体用于表示符号表的条目，包含一个键 key（最大长度为 KEY_LEN，即32），以及对应的值 value（类型为 VAL_T，在此代码中定义为 int）。
symtab 结构体表示符号表，每个节点包含一个 entry 结构体，以及左右两个指向子树的指针。
函数定义：

entry_init: 初始化 entry 结构体，设置键值对的初始值。
symtab_init: 初始化符号表，返回指向新符号表的指针。
symtab_insert: 向符号表中插入键值对，如果插入成功返回1，否则返回0。
symtab_lookup: 查找符号表中特定键的值，如果找到返回对应的值，否则返回-1。
symtab_remove: 从符号表中移除特定键的键值对，如果移除成功返回1，否则返回0。
宏定义：

KEY_LEN 定义了键的最大长度。
VAL_T 定义了值的类型，在此处为 int。
此符号表是一个二叉树实现，其中 symtab 结构体的每个节点表示一个键值对。键值对通过 entry 结构体表示。符号表的操作包括插入、查找和移除键值对。这种实现通过二叉树的方式来组织数据，可以在对数时间内执行这些操作。
```

make一下

![image-20231130105948661](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231130105948661.png)

Optional: write your code in symtab_bt.c or symtab_ht.c and make the

corresponding target **(no bonus point)**

修改symtab_bt.c为

```c
#include <iostream>
#include <array>
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

#include "symtab.h"

// 哈希表大小
inline static constexpr int32_t TABLE_SIZE = 10007;

/*
 * 符号表类型，使用二叉树实现
 */
struct symtab {
    entry entry{};
    symtab *left = nullptr, *right = nullptr;

    // 默认构造函数，初始化 entry
    symtab() {
        memset(&this->entry, '\0', sizeof(this->entry));
    }

    // 带参构造函数，根据给定的 key 和 value 初始化 entry
    symtab(char *const key, VAL_T value) {
        entry_init(&(this->entry), key, value);
    }
};

// 哈希表，存储根节点
static std::array<symtab *, TABLE_SIZE> BFS_TABLE{nullptr,};

// ************************************************************
//    Your implementation goes here
// ************************************************************

// 初始化符号表
symtab *symtab_init() {
    return new symtab();
}

// 向符号表中插入键值对
int symtab_insert(symtab *const self, char *const key, VAL_T value) {
    // TODO: implement this function
    memset(&BFS_TABLE, '\0', sizeof(BFS_TABLE));
    BFS_TABLE[0] = self;
    for (int begin = 0, last = 0; begin < TABLE_SIZE && begin <= last; begin++) {
        if (strcmp(BFS_TABLE[begin]->entry.key, key) == 0) {
            return 0; // 已存在相同的 key，插入失败
        }
        if (BFS_TABLE[begin]->left != nullptr) {
            ++last;
            BFS_TABLE[last] = BFS_TABLE[begin]->left;
        } else {
            // 左子节点为空，创建新节点并插入左侧
            BFS_TABLE[begin]->left = new symtab(key, value);
            return 1; // 插入成功
        }
        if (BFS_TABLE[begin]->right != nullptr) {
            ++last;
            BFS_TABLE[last] = BFS_TABLE[begin]->right;
        } else {
            // 右子节点为空，创建新节点并插入右侧
            BFS_TABLE[begin]->right = new symtab(key, value);
            return 1; // 插入成功
        }
    }
    return -1; // 哈希表已满，插入失败
}

// 查找符号表中键对应的值
VAL_T symtab_lookup(symtab *const self, char * const key) {
    // 查找给定 key 对应的值
    memset(&BFS_TABLE, '\0', sizeof(BFS_TABLE));
    BFS_TABLE[0] = self;
    for (int begin = 0, last = 0; begin < TABLE_SIZE && begin <= last; ++begin) {
        if (strcmp(BFS_TABLE[begin]->entry.key, key) == 0) {
            return BFS_TABLE[begin]->entry.value; // 找到 key，返回对应的值
        }
        if (BFS_TABLE[begin]->left != nullptr) {
            ++last;
            BFS_TABLE[last] = BFS_TABLE[begin]->left;
        }
        if (BFS_TABLE[begin]->right != nullptr) {
            ++last;
            BFS_TABLE[last] = BFS_TABLE[begin]->right;
        }
    }
    return -1; // 未找到对应的 key，返回 -1
}

// 从符号表中移除键值对
int symtab_remove(symtab *const self, char * const key) {
    // 从哈希表中移除给定 key 对应的节点
    memset(&BFS_TABLE, '\0', sizeof(BFS_TABLE));
    BFS_TABLE[0] = self;
    int last = 0;
    int sign = -1;
    for (int begin = 0; begin < TABLE_SIZE && begin <= last; begin++) {
        if (BFS_TABLE[begin]->left != nullptr) {
            ++last;
            if (strcmp(BFS_TABLE[begin]->left->entry.key, key) == 0) {
                sign = last; // 记录找到节点的位置
            }
            BFS_TABLE[last] = BFS_TABLE[begin]->left;
        }
        if (BFS_TABLE[begin]->right != nullptr) {
            ++last;
            if (strcmp(BFS_TABLE[begin]->right->entry.key, key) == 0) {
                sign = last; // 记录找到节点的位置
            }
            BFS_TABLE[last] = BFS_TABLE[begin]->right;
        }
    }
    if (sign != -1) {
        auto *const temp = BFS_TABLE[sign];
        BFS_TABLE[sign] = BFS_TABLE[last];
        delete temp; // 删除节点
        return 1; // 移除成功
    }
    return 0; // 未找到对应 key 的节点，移除失败
}

#ifdef __cplusplus
}
#endif

```

并且修改makefile为g++编译。

![image-20231130111508960](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231130111508960.png)
