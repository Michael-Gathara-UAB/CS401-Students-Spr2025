import copy
lst: list = [1, 2, 3, 4]

lst_shallow = copy.copy(lst)

print(f"The mem address of 1st elem of lst: {hex(id(lst[0]))}")
print(f"The mem address of 1st elem of lst_shallow: {hex(id(lst_shallow[0]))}")
print(f"The mem address of lst: {hex(id(lst))}")
print(f"The mem address of lst_shallow: {hex(id(lst_shallow))}")