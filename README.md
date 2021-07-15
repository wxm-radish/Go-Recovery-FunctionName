# Go-Recovery-Symbol
一个逆向GO二进制程序的辅助工具

在`linux`上，GO编译的二进制可运行程序如果用`strip`去符号之后，可以用本工具来进行恢复函数名，方便之后的逆向。

## 1.16版本之前使用方法
1、`./get_sym_15 [filename]`，提取函数名字和函数地址的关系，会在当前目录下生成`data.py`，把`data.py`和`tools.py`放到同一目录下

2、用`IDA Pro`打开需要逆向的二进制文件，运行`tools.py`，即可恢复函数名

## 1.16版本之后使用方法
1、`./get_sym_16 [filename]`，提取函数名字和函数地址的关系，会在当前目录下生成`data.py`，把`data.py`和`tools.py`放到同一目录下

2、用`IDA Pro`打开需要逆向的二进制文件，运行`tools.py`，即可恢复函数名

## 使用范围
1、linux

2、64位可执行程序

