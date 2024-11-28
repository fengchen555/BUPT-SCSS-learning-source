#include "linked_list.h"
#include <limits.h>

node *linked_list_init(){
    node *head = (node*)malloc(sizeof(node));
    head->count = 0;
    head->next = NULL;
    return head;
}

void linked_list_free(node *head){
    node *cur = head;
    node *last;
    while(cur != NULL){
        last = cur;
        cur = cur->next;
        free(last);
    }
}

char linked_list_string[0x10000];

char *linked_list_tostring(node *head){
    node *cur = head->next;
    char *position;
    int length = 0;
    while(cur != NULL){
        position = linked_list_string + length;
        length += sprintf(position, "%d", cur->value);
        cur = cur->next;
        if(cur != NULL){
            position = linked_list_string + length;
            length += sprintf(position, "->");
        }
    }
    position = linked_list_string + length;
    length += sprintf(position, "%c", '\0');
    return linked_list_string;
}

int linked_list_size(node *head){
    return head->count;
}

void linked_list_append(node *head, int val){
    node *cur = head;
    node *new_node;
    while(cur->next != NULL){
        cur = cur->next;
    }
    new_node = (node*)malloc(sizeof(node));
    new_node->value = val;
    new_node->next = NULL;
    cur->next = new_node;
    head->count++;
}

/* your implementation goes here */
// 在指定位置插入节点
void linked_list_insert(node* head, int val, int index) {
    if (index > head->count || index < 0) {
        return; // 如果索引超出范围，不执行插入操作
    }
    node* cur = head;  // 从头节点开始遍历
    node* new_node = (node*)malloc(sizeof(node));  // 创建新节点
    new_node->value = val;  // 设置新节点的值
    new_node->next = NULL;  // 新节点的下一个节点为空

    for (int i = 0; i < index; i++) {
        cur = cur->next;  // 移动到指定索引位置的前一个节点
    }
    new_node->next = cur->next;  // 新节点的下一个节点指向当前节点的下一个节点
    cur->next = new_node;  // 当前节点的下一个节点指向新节点
    head->count++;  // 增加链表中的元素数量
}

// 删除指定位置的节点
void linked_list_delete(node* head, int index) {
    if (index >= head->count || index < 0) {
        return; // 如果索引超出范围，不执行删除操作
    }
    node* cur = head;  // 从头节点开始遍历
    for (int i = 0; i < index; i++) {
        cur = cur->next;  // 移动到指定索引位置的前一个节点
    }
    node* temp = cur->next;  // 临时保存待删除的节点
    cur->next = temp->next;  // 将当前节点的下一个节点指向待删除节点的下一个节点
    free(temp);  // 释放待删除节点的内存
    head->count--;  // 减少链表中的元素数量
}

// 移除具有指定值的第一个节点
void linked_list_remove(node* head, int val) {
    node* cur = head;  // 从头节点开始遍历
    while (cur->next != NULL) {
        if (cur->next->value == val) {  // 如果下一个节点的值等于目标值
            node* temp = cur->next;  // 临时保存待移除的节点
            cur->next = temp->next;  // 将当前节点的下一个节点指向待移除节点的下一个节点
            free(temp);  // 释放待移除节点的内存
            head->count--;  // 减少链表中的元素数量
            return;  // 返回，只移除第一个匹配的节点
        }
        cur = cur->next;  // 否则，继续遍历下一个节点
    }
}

// 移除具有指定值的所有节点
void linked_list_remove_all(node* head, int val) {
    node* cur = head;  // 从头节点开始遍历
    while (cur->next != NULL) {
        if (cur->next->value == val) {  // 如果下一个节点的值等于目标值
            node* temp = cur->next;  // 临时保存待移除的节点
            cur->next = temp->next;  // 将当前节点的下一个节点指向待移除节点的下一个节点
            free(temp);  // 释放待移除节点的内存
            head->count--;  // 减少链表中的元素数量
        } else {
            cur = cur->next;  // 否则，继续遍历下一个节点
        }
    }
}

// 获取指定位置的节点值
int linked_list_get(node* head, int index) {
    if (index >= head->count || index < 0) {
        return INT_MIN;  // 如果索引超出范围，返回INT_MIN（表示错误）
    }
    node* temp = head->next;  // 从第一个节点开始遍历
    for (int i = 0; i < index; i++) {
        temp = temp->next;  // 移动到指定索引位置的节点
    }
    return temp->value;  // 返回该节点的值
}

// 查找具有指定值的第一个节点的索引
int linked_list_search(node* head, int val) {
    if (head == NULL) {
        return -1;  // 如果链表为空，返回-1（表示未找到）
    }
    node* temp = head->next;  // 从第一个节点开始遍历
    int index = 0;  // 初始化索引为0
    while (temp != NULL) {
        if (temp->value == val) {  // 如果节点的值等于目标值
            return index;  // 返回该节点的索引
        }
        temp = temp->next;  // 否则，继续遍历下一个节点
        index++;  // 增加索引
    }
    return -1;  // 如果未找到，返回-1
}

// 查找具有指定值的所有节点，并返回包含它们索引的链表
node* linked_list_search_all(node* head, int val) {
    if (head == NULL) {
        return NULL;  // 如果链表为空，返回空链表
    }
    node* result = linked_list_init();  // 创建结果链表
    node* temp = head->next;  // 从第一个节点开始遍历
    int index = 0;  // 初始化索引为0
    while (temp != NULL) {
        if (temp->value == val) {  // 如果节点的值等于目标值
            linked_list_insert(result, index, result->count);  // 将匹配节点的索引插入结果链表
        }
        temp = temp->next;  // 否则，继续遍历下一个节点
        index++;  // 增加索引
    }
    return result;  // 返回包含匹配节点索引的链表
}