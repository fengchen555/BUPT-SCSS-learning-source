import subprocess
import os

def build_compiler():
    try:
        # 调用 make 命令来编译你的代码
        subprocess.run(["make", "bplc"])

    except subprocess.CalledProcessError as e:
        print(f"Error while building compiler: {e}")

def run_compiler(input_file, output_file):
    try:
        # 创建输出文件所在的目录
        os.makedirs(os.path.dirname(output_file), exist_ok=True)

        # 调用编译器并传递输入文件作为参数，并将标准输出重定向到文件
        with open(output_file, 'w') as f:
            subprocess.run(["bin/bplc", input_file], stdout=f)

    except subprocess.CalledProcessError as e:
        print(f"Error while running compiler: {e}")

if __name__ == "__main__":
    # 编译你的代码
    build_compiler()

    # 运行编译器并传递输入文件（替换为你的实际输入文件路径）
    for i in range(1, 13):
        print("----------------------------------------------------------------")
        temp = i
        input_file = "test/test_1_r" + str(temp).zfill(2) + ".bpl"
        #output_file = "out/test_1_r" + str(temp).zfill(2) + ".out"
        output_file = "test/test_1_r" + str(temp).zfill(2) + ".out"
        print("Test " + str(i) + ":")
        run_compiler(input_file, output_file)
    

    print("The output has been saved in the test folder")
