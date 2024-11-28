#include "semanticAnalysisUtil.hpp"

int countParamNum(FieldList *param)
{
    int counter = 0;
    FieldList *temp = param;

    if (temp == nullptr)
    {
        return 0;
    }
    while (temp->next != nullptr)
    {
        counter++;
        temp = temp->next;
    }
    return counter;
}

FieldList *vector_to_fieldlist(vector<FieldList *> param)
{
    FieldList *ans, *temp;
    for (size_t i = 0; i < param.size(); i++)
    {
        if (i == 0)
        {
            ans = param[i];
            temp = param[i];
        }
        else
        {
            temp->next = param[i];
            temp = temp->next;
        }
    }
    return ans;
}

int get_expect_param_num(FieldList *param)
{
    int counter = 0;
    FieldList *temp = param;
    while (temp != nullptr)
    {
        temp = temp->next;
        counter++;
    }
    return counter;
}

// Args -> Exp COMMA Args | Exp
int get_real_param_num(Node *args)
{
    if (args == nullptr)
    {
        return 0;
    }
    else
    {
        int counter = 0;
        Node *temp = args;
        while (temp->child.size() != 1) //还没有到最后一个参数
        {
            counter++;
            temp = temp->child[2];
        }
        counter++;
        return counter;
    }
}

std::tuple<vector<int>, Type *> getArrayDemensionAndType(Type *_type)
{
    vector<int> demensions;
    Type *temp = _type;
    while (temp != nullptr && temp->foo.array != nullptr)
    {
        auto tempArray = temp->foo.array;
        temp = tempArray->base;
        demensions.push_back(tempArray->size);
    }
    return std::tuple<vector<int>, Type *>(demensions, temp);
};

bool checkIntegerType(Node *exp)
{

    if (exp->var == nullptr)
    {
        return false;
    }
    else
    {
        if (exp->var == Type::getPrimitiveINT())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool checkFloatType(Node *exp)
{

    if (exp->var == nullptr)
    {
        return false;
    }
    else
    {
        if (exp->var == Type::getPrimitiveFLOAT())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

// 将外层的EXP的var设置为布尔类型 否则出错
void setBoolOperatorType(Node *expOut, Node *expLeft, Node *expRight)
{
    bool a = checkIntegerType(expLeft);
    bool b = checkIntegerType(expRight);

    if (a && b)
    {
        expOut->var = Type::getPrimitiveINT();
    }
    else
    {
        if (expLeft->var != nullptr && expRight->var != nullptr)
        {
            binaryOperatorNotValid_7(expOut->line_num);
        }
    }
}

void setCompareOperatorType(Node *expOut, Node *expLeft, Node *expRight)
{
    bool a = checkIntegerType(expLeft);
    bool b = checkIntegerType(expRight);
    bool c = checkFloatType(expLeft);
    bool d = checkFloatType(expRight);

    if ((a && b) || (c && d))
    {
        expOut->var = Type::getPrimitiveINT();
    }
    else
    {
        if (expLeft->var != nullptr && expRight->var != nullptr)
        {
            binaryOperatorNotValid_7(expOut->line_num);
        }
    }
}

void setAlrthOperatorType(Node *expOut, Node *expLeft, Node *expRight)
{
    bool a = checkIntegerType(expLeft);
    bool b = checkIntegerType(expRight);
    bool c = checkFloatType(expLeft);
    bool d = checkFloatType(expRight);

    if (a && b)
    {
        expOut->var = Type::getPrimitiveINT();
    }
    else if (c && d)
    {
        expOut->var = Type::getPrimitiveFLOAT();
    }
    else
    {
        if (expLeft->var != nullptr && expRight->var != nullptr)
        {
            binaryOperatorNotValid_7(expOut->line_num);
        }
    }
}

void setAlrthOperatorType(Node *expOut, Node *innerExp)
{
    if (expOut->name == "MINUS")
    {
        bool a = checkIntegerType(innerExp);
        bool b = checkFloatType(innerExp);

        if (a)
        {
            expOut->var = Type::getPrimitiveINT();
        }
        else if (b)
        {
            expOut->var = Type::getPrimitiveFLOAT();
        }
        else
        {
            if (innerExp == nullptr)
            {
                singleOperatorNotValid_7(expOut->line_num);
            }
        }
    }
    else if (expOut->name == "NOT")
    {
        bool a = checkIntegerType(innerExp);

        if (a)
        {
            expOut->var = Type::getPrimitiveINT();
        }
        else
        {
            if (innerExp == nullptr)
            {
                singleOperatorNotValid_7(expOut->line_num);
            }
        }
    }
}

vector<Node *> getReturnExpFromCompSt(Node *CompSt)
{
    Node *StmtList = CompSt->child[2];
    Node *temp = StmtList;
    vector<Node *> Exps;
    while (!temp->child.empty()) //还存在Stmt
    {

        Node *Stmt = temp->child[0];
        vector<Node *> innerExps = getReturnExpFromStmt(Stmt);

        for (auto &&i : innerExps)
        {
            Exps.push_back(i);
        }

        temp = temp->child[1];
    }
    return Exps;
}

vector<Node *> getReturnExpFromStmt(Node *Stmt)
{
    vector<Node *> Exps;
    if (Stmt->child.size() == 1)
    {
        Node *subCompSt = Stmt->child[0];
        vector<Node *> tempVector = getReturnExpFromCompSt(subCompSt);
        for (auto &&i : tempVector)
        {
            Exps.push_back(i);
        }
    }
    else if (Stmt->child.size() == 5 && (Stmt->child[0]->name == "IF"|| Stmt->child[0]->name == "WHILE"))
    {
        vector<Node *> tempVector1 = getReturnExpFromStmt(Stmt->child[4]);
        for (auto &&i : tempVector1)
        {
            Exps.push_back(i);
        }
    }
    else if (Stmt->child.size() == 7 && Stmt->child[0]->name == "IF")
    {
        vector<Node *> tempVector1 = getReturnExpFromStmt(Stmt->child[4]);
        vector<Node *> tempVector2 = getReturnExpFromStmt(Stmt->child[6]);
        for (auto &&i : tempVector1)
        {
            Exps.push_back(i);
        }
        for (auto &&i : tempVector2)
        {
            Exps.push_back(i);
        }
    }
    else if (Stmt->child.size() == 3 && Stmt->child[0]->name == "RETURN")
    {
        // printf("找到Return关键字");
        Exps.push_back(Stmt->child[1]);
    }
    return Exps;
}
bool isExpressionNode(Node* node) {
    return node && node->name == "Exp";
}
Node* getLastExpressionNode(Node* currentNode) {
    if (!currentNode) {
        return nullptr;
    }

    // 递归遍历子节点
    for (int i = currentNode->child.size() - 1; i >= 0; --i) {
        Node* lastChild = getLastExpressionNode(currentNode->child[i]);
        if (lastChild && isExpressionNode(lastChild)) {
            return lastChild;
        }
    }

    // 如果当前节点是表达式节点，返回当前节点
    if (isExpressionNode(currentNode)) {
        return currentNode;
    }

    return nullptr;  // 如果没有找到表达式节点，返回空指针
}