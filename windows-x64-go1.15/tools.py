#coding:utf-8   
from data import *
import idc, idaapi
def fmt_name(funcname):
    funcname = funcname.replace(".","_")
    funcname = funcname.replace("/","_")
    funcname = funcname.replace("*","_")
    funcname = funcname.replace(" ","")
    funcname = funcname.replace("·","_")
    funcname = funcname.replace("[","_")
    funcname = funcname.replace("]","_")
    funcname = funcname.replace("{","_")
    funcname = funcname.replace("}","_")
    funcname = funcname.replace("-","_")
    funcname = funcname.replace(";","_")
    return funcname


for k,v in data.items():
    func_address = eval("0x"+k)
    func_name = idc.get_func_name(func_address)
    print func_name
    if func_name:
        name = fmt_name(v)
        ret = MakeName(func_address,name)
        if not ret:
            print v+"is error"
    else:
        idc.del_items(func_address, idc.DELIT_EXPAND)
        idaapi.auto_wait()
        idc.create_insn(func_address)
        idaapi.auto_wait()
        if idc.add_func(func_address):
            idaapi.auto_wait()
            func_name = idc.get_func_name(func_address)
            print func_name
            if func_name:
                name = fmt_name(v)
                ret = MakeName(func_address,name)
                if not ret:
                    print v+"is error"

print "完成对函数名的恢复"