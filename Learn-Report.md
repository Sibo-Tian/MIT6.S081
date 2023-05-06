### Lab1: Utilities

使用exit而非return退出（why）

1. prime.c: infinite recursion : changed in Makefile: `CFLAGS = -Wall -Werror -O -fno-omit-frame-pointer -ggdb -Wno-error=infinite-recursion`
2. find：递归延长路径名，从而在大范围内查找
3. xargs：//xxxx | xargs yyyyy : xxxx's result is stored in a pipe or whatever similar, then xargs read from it。

Grade script: make GRADEFLAGS=prime grade
