#! /bin/python


to_print = ""

for i in range(1000):
    tmp = str(i).ljust(8,"a")
    to_print += tmp

print(to_print)
