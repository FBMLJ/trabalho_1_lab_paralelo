from os import times_result
import pandas as pd

df =pd.read_csv("time_result.csv")
print(df.tempo.mean() ," + ",2* df.tempo.std())