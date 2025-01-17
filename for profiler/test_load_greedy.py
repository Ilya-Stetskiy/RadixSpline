import pandas as pd
from lib.radix import * 
from pympler import asizeof
scv=pd.read_csv("cinema.csv", index_col=False)
sort_keys=scv["timestamp"]
# import base36
data=[[key,ind] for ind,key in enumerate(sort_keys)]

# A=get_spline(data[:10000],5,6,[True,True])
A=GreedySpline(6)
A.load_data(data)
print("calculated Greedy asize = ",asizeof.asizeof(A))
print("count of points = ", len(A.greedyspline))