import sys
import matplotlib.pyplot as plt
import numpy as np
import matplotlib

#get list of all benchmarks tested
benchmark_names = []

with open('data_list.txt') as fn:
  for line in fn:
    benchmark_names.append(line[:-1])

#read each file and generate scatterplot
for benchmark in benchmark_names:

  #generate ins->history scatterplots, overlay different history lengths
  x = []
  y = []
  with open('text_files/' + benchmark + '_8hist_full.txt') as f:
      for line in f:
          data = line.split()
          y.append(int(data[0]))
          x.append(int(data[1]))

  plt.scatter(np.array(x), np.array(y), c="r", alpha=0.5, label = "8-call history")

  x = []
  y = []
  with open('text_files/' + benchmark + '_16hist_full.txt') as f:
      for line in f:
          data = line.split()
          y.append(int(data[0]))
          x.append(int(data[1]))

  plt.scatter(np.array(x), np.array(y), c="g", alpha=0.5, label = "16-call history")

  x = []
  y = []
  with open('text_files/' + benchmark + '_32hist_full.txt') as f:
      for line in f:
          data = line.split()
          y.append(int(data[0]))
          x.append(int(data[1]))

  plt.scatter(np.array(x), np.array(y), c="b", alpha=0.5, label = "32-call history")

  plt.xlabel("Number of Calls")
  plt.ylabel("Number of Unique Call Histories")
  plt.legend(loc='upper right')
  plt.suptitle(benchmark)
  plt.savefig('scatterplots/ins_to_history/' + benchmark + '_ih.png')

  #clear plot
  plt.clf()
  plt.cla()


  #generate history->ins scatterplots, overlay different history lengths
  x = []
  y = []
  with open('text_files/reverse_' + benchmark + '_8hist_full.txt') as f:
      for line in f:
          data = line.split()
          y.append(int(data[0]))
          x.append(int(data[1]))

  plt.scatter(np.array(x), np.array(y), c="r", alpha=0.5, label = "8-call history")

  x = []
  y = []
  with open('text_files/reverse_' + benchmark + '_16hist_full.txt') as f:
      for line in f:
          data = line.split()
          y.append(int(data[0]))
          x.append(int(data[1]))

  plt.scatter(np.array(x), np.array(y), c="g", alpha=0.5, label = "16-call history")

  x = []
  y = []
  with open('text_files/reverse_' + benchmark + '_32hist_full.txt') as f:
      for line in f:
          data = line.split()
          y.append(int(data[0]))
          x.append(int(data[1]))

  plt.scatter(np.array(x), np.array(y), c="b", alpha=0.5, label = "32-call history")

  plt.xlabel("Number of Calls")
  plt.ylabel("Number of Unique Load Instructions")
  plt.legend(loc='upper right')
  plt.suptitle(benchmark)
  plt.savefig('scatterplots/history_to_ins/' + benchmark + '_hi.png')

  #clear plot
  plt.clf()
  plt.cla()



