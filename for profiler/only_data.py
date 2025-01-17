import pandas as pd
scv=pd.read_csv("../cinema.csv", index_col=False)
sort_keys=scv["timestamp"]
data=[[key,ind] for ind,key in enumerate(sort_keys)]