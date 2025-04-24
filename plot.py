import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('result.csv')
max_threads = max(df['tn'])
min_value = np.empty(max_threads)
avg_value = np.empty(max_threads)
med_value = np.empty(max_threads)
max_value = np.empty(max_threads)

for tn in range(max_threads):
    current_df = df.loc[df['tn'] == tn + 1]
    min_value[tn] = current_df['time'].min()
    avg_value[tn] = current_df['time'].mean()
    med_value[tn] = current_df['time'].median()
    max_value[tn] = current_df['time'].max()

upper_error = max_value - med_value
lower_error = med_value - min_value

xticks = np.unique(df['tn'].to_numpy())

plt.plot(xticks, med_value)                                         # plot result
# plt.errorbar(xticks, med_value, yerr=(lower_error, upper_error))    # plot result with errorbar
# plt.xticks(xticks)                                                  # show x-axis ticks

plt.xlabel('# of threads')
plt.ylabel('processing time [s]')

mat_size = 1000                                                     # to show the matrix size on the plot title
plt.title(f'Processing time of {mat_size} x {mat_size} matrix by thread numbers')
# plt.show()
plt.savefig("result.png")