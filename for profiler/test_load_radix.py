import pandas as pd
from lib.radix import * 
from pympler import asizeof
scv=pd.read_csv("cinema.csv", index_col=False)
sort_keys=scv["timestamp"]
# import base36
data=[[key,ind] for ind,key in enumerate(sort_keys)]

# A=get_spline(data[:10000],5,6,[True,True])
A=get_spline(data,ERR=5,bitcount=6)
print("calculated Radix asize = ",asizeof.asizeof(A))