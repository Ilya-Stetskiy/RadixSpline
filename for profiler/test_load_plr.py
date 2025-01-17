import pandas as pd
from lib.plr import * 
from pympler import asizeof
scv=pd.read_csv("../cinema.csv", index_col=False)
sort_keys=scv["timestamp"]
# import base36
data=[[key,ind] for ind,key in enumerate(sort_keys)]

A=PLR(data,6)
print("calculated PLR asize = ",asizeof.asizeof(A))
print("count of points = ", len(A))