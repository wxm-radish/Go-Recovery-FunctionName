
## 使用方法

1、./get_sym_16 [filename]，提取函数名字和函数地址的关系，会在当前目录下生成data.py，把data.py和tools.py放到同一目录下

2、用IDA Pro打开需要逆向的二进制文件，运行tools.py，即可恢复函数名

## 使用范围
1、linux 64位可执行程序

2、go1.16和go1.16之后
