import pandas as pd
from lib.radix import * 
from pympler import asizeof
import time


scv=pd.read_csv("../cinema.csv", index_col=False)
sort_keys=scv["timestamp"]
# import base36
data=[[key,ind] for ind,key in enumerate(sort_keys)]

# A=get_spline(data[:10000],5,6,[True,True])
A=get_spline(data,ERR=6,bitcount=6)

start2=time.time_ns()
for j in data:
    get_key(data,A,j)
end2=time.time_ns()
print("calculated Radix asize = ",asizeof.asizeof(A))
print("search_time", (end2-start2)/10**6, "millisec" )