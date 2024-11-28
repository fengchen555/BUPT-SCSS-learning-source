CC=gcc
CPP=g++
FLEX=flex
BISON=bison

bplc:
	@rm -rf bin/# 删除已经存在的 bin 目录，加上 || true 避免在 bin 目录不存在时报错
	@mkdir -p bin         # 使用 -p 选项，确保 bin 目录存在，避免目录已存在时报错
	@touch bin/bplc
	@chmod +x bin/bplc
	$(BISON) -t -d syntax.y
	$(FLEX) lex.l
	$(CPP) Node.hpp Node.cpp syntax.tab.c -lfl -ly -o bin/bplc  # bplc输出路径为: bin/bplc

clean:
	@rm -rf bin/

.PHONY: bplc clean
