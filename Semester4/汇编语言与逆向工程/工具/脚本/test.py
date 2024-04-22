import ast

# 定义一个函数访问者，用于访问AST树中的函数定义节点
class FuncVisitor(ast.NodeVisitor):
    # 定义一个空的函数列表，用于存储找到的所有函数
    def __init__(self):
        self.functions = []

    # 当访问到函数定义节点时，将函数名和函数定义添加到函数列表中
    def visit_FunctionDef(self, node):
        func_name = node.name
        func_args = [arg.arg for arg in node.args.args]
        func_defaults = [None] * (len(func_args) - len(node.args.defaults)) + node.args.defaults
        func_code = ast.unparse(node.body).strip()
        self.functions.append({
            'name': func_name,
            'args': func_args,
            'defaults': func_defaults,
            'code': func_code,
        })

# 读取Python代码文件，并解析为AST树
with open('rc4_2.py', 'r') as f:
    source_code = f.read()
    tree = ast.parse(source_code)

# 创建函数访问者，用于遍历AST树中的函数定义节点
visitor = FuncVisitor()
visitor.visit(tree)

# 输出找到的所有函数
for func in visitor.functions:
    print(f"Function name: {func['name']}")
    print(f"Function args: {func['args']}")
    print(f"Function defaults: {func['defaults']}")
    print(f"Function code:\n{func['code']}\n")