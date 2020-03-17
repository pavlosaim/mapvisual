# Memory Access Pattern Visualization #
# Author: Pavlos Aimoniotis
#######################################
# This .py file uses data produced from
# memory tracing and filtering and 
# visualizes the complete execution 
# of the program.
# Produces a 3d plot.
# Information: Addresses, Number of times visited,
# variable name and a Heat Map, which shows
# the time of happening

from __future__ import division
import sys,subprocess,matplotlib
from Tkinter import *
import Tkinter as tk
import tkFileDialog,tkMessageBox
from ScrolledText import ScrolledText
matplotlib.use('TkAgg')
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.backends.backend_tkagg as tkagg
from matplotlib.figure import Figure
from matplotlib import pyplot as plt
from random import randint
import numpy as np
from matplotlib.ticker import ScalarFormatter
import matplotlib.patches as patches
from timeit import default_timer as timer
from copy import deepcopy
import plotly
import plotly.express as px
import plotly.graph_objects as go

order_data_file = open("order_data.txt","r");
addresses = []
operations = []
order_of_names = []
lut_indices = []
total_accesses = [];
count = 0;
# Parse order_data file
for line in order_data_file:
    val1,str1,char1,str2 = line.strip().split()
    lut_indices.append(val1)
    addresses.append(str1)
    operations.append(char1)
    order_of_names.append(str2)
    count = count + 1
    total_accesses.append(count);
order_data_file.close()

print("Creating data for figure drawing...\n")
#Sort them as a pack. Every index will refer to a specifix line of index access and address
sorted_addresses = []
sorted_total_accesses = []
sorted_order_of_names = []
#Sort them by address
for entry in sorted(zip(total_accesses,addresses, order_of_names)):
    sorted_addresses.append(entry[1])
    sorted_total_accesses.append(entry[0])
    sorted_order_of_names.append(entry[2])


# Find times of accesses in every address
# sorted_addresses is actually sorted according to total_accesses list
# Every access is tracked, and stored on a lookup table which has a counter of how many times the specific address has been accessed
# Every new access learns from that lookup table its time of occurence on the exact address and takes its index
temp_addresses= []
temp_count = []
sorted_times_accessed_each_address = []
for entry in sorted_addresses:
    if entry in temp_addresses:
        for i in range(0, len(temp_addresses)):
         if temp_addresses[i] == entry:
            temp_count[i] = temp_count[i] + 1
            sorted_times_accessed_each_address.append(str(temp_count[i]))
    else:
        temp_addresses.append(entry)
        temp_count.append(1)
        sorted_times_accessed_each_address.append("1")

# Print on Window
print("Index      Address         Variable        Occurence Index\n")
print("----------------------------------------------------------\n")
for entry in sorted(zip(total_accesses,addresses, order_of_names, sorted_times_accessed_each_address)):
    print(str(entry[0])+"      "+str(entry[1])+"           	"+str(entry[2])+"              "+str(entry[3])+"\n")

print("Data was created.\n")
print("Starting painting...\n")
fig = go.Figure(data=[go.Scatter3d(
    x=sorted_times_accessed_each_address, 
    y=sorted_order_of_names, 
    z=sorted_addresses, 
    mode='markers',
    marker = dict(
        size=10,
        color = sorted_total_accesses,
        colorbar = dict(
            title="Heat Map",
            ticktext=["Sooner", "Later"],
        ),
        colorscale = "Viridis"
    )
)])

print("Trying to show figure...\n")
plotly.offline.plot(fig, filename = "figures/complete_memory_access_pattern.html")

'''fig = plt.figure()
ax = plt.axes(projection='3d')


print(sorted_times_accessed_each_address)
ax.scatter3D(sorted_order_of_names, sorted_addresses, sorted_times_accessed_each_address, color = gradient, s = 25, marker="^")


ax.set_xlabel("Variable");
ax.set_ylabel("Address");
ax.set_zlabel("Access Occurence");
ax.set_title("Heap Memory Access Pattern")

plt.show()'''
