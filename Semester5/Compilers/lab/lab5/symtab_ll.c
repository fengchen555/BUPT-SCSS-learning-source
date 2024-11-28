#include "symtab.h"

/*
 * symbol table type, linked list impl
 */
struct symtab {
    entry entry;
    struct symtab *next;
};

// ************************************************************
//    Your implementation goes here
// ************************************************************

symtab *symtab_init(){
    symtab *self = malloc(sizeof(symtab));
    memset(self, '\0', sizeof(symtab));
    self->next = NULL;
    return self;
}

int symtab_insert(symtab *self, char *key, VAL_T value){
    symtab *ptr = self;
    while(ptr->next != NULL){
        if(strcmp(ptr->entry.key, key) == 0)
            return 0;
        ptr = ptr->next;
    }
    symtab *node = malloc(sizeof(symtab));
    memset(node, '\0', sizeof(symtab));
    entry_init(&node->entry, key, value);
    node->next = NULL;
    ptr->next = node;
    return 1;
}

VAL_T symtab_lookup(symtab *self, char *key){
    symtab *ptr = self;
    while(ptr != NULL){
        if(strcmp(ptr->entry.key, key) == 0)
            return ptr->entry.value;
        ptr = ptr->next;
    }
    return -1;
}

int symtab_remove(symtab *self, char *key){
    symtab *ptr = self, *tmp;
    while(ptr->next != NULL) {
        if(strcmp(ptr->next->entry.key, key) == 0){
            tmp = ptr->next;
            ptr->next = ptr->next->next;
            free(tmp);
            return 1;
        }
        ptr = ptr->next;
    }
    return 0;
}
/*
这段代码定义了一个简单的符号表实现，用于存储键值对。主要包括以下几个部分：

数据结构定义：

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
*/
