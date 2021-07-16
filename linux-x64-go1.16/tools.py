#coding:utf-8   
from data import *
index = 0
for k,v in data.items():
    addr = idc.LocByName(k)
    name = GetFunctionName(addr) #获取函数名称
    v = v.replace(".","_")
    v = v.replace("/","_")
    v = v.replace("*","_")
    v = v.replace(" ","")
    v = v.replace("·","_")
    v = v.replace("[","_")
    v = v.replace("]","_")
    v = v.replace("{","_")
    v = v.replace("}","_")
    v = v.replace("-","_")
    v = v.replace(";","_")
    ret = MakeName(addr,v)
    if ret == True:
        index += 1
print "\n共恢复"+str(index)+"个函数的名字\n"