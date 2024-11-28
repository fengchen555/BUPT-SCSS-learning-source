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
