import pandas as pd
from lib.greedy import * 
from pympler import asizeof
scv=pd.read_csv("cinema.csv", index_col=False)
sort_keys=scv["timestamp"]

data=[[key,ind] for ind,key in enumerate(sort_keys)]
A=GreedySpline(6)
A.load_data(data)
print("calculated Greedy asize = ",asizeof.asizeof(A))
print("count of points = ", len(A.greedyspline))
