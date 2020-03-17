# Memory Access Pattern Visualization #
# Author: Pavlos Aimoniotis
#######################################
# This .py file uses data produced from
# memory tracing and filtering and 
# visualizes a 2d array 
# of the program.
# Produces a 3d plot.
# Information: Rows (i), Cols (j),
# Occurences indexes and a Heat Map, which shows
# the time of happening

from __future__ import division
import sys,subprocess,matplotlib,getopt
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

array = sys.argv[1] 
cols = sys.argv[2] 
typeb = sys.argv[3]

order_data_file = open("order_data.txt","r")
addresses = []
operations = []
order_of_names = []
lut_indices = []
total_accesses = []
count = 0
# Parse order_data file
for line in order_data_file:
    val1,str1,char1,str2 = line.strip().split()
    if str2 == array:
        lut_indices.append(val1)
        addresses.append(str1)
        operations.append(char1)
        order_of_names.append(str2)
        count = count + 1
        total_accesses.append(count);
order_data_file.close()

found = 0
# Parse alloc_info file
alloc_info = open("alloc_info","r")
for line in alloc_info:
    addr1,str1= line.strip().split()
    if addr1 in addresses:
        found = 1
        start_addr = addr1
        total_bytes_alloced = str1
order_data_file.close()

if(found == 0):
    print("\nThere is not an array with name:" + array + "\n")
    sys.exit()
print("Creating data for figure drawing...\n")
#Sort them as a pack. Every index will refer to a specifix line of index access and address
sorted_addresses = []
sorted_total_accesses = []
rows = []
columns = []
#Sort them by address
for entry in sorted(zip(total_accesses,addresses)):
    sorted_addresses.append(entry[1])
    difference = int(entry[1],16) - int(start_addr,16)
    element = difference / int(typeb)
    i = element // int(cols)
    j = element % int(cols)
    rows.append(i)
    columns.append(j)
    sorted_total_accesses.append(entry[0])

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
    y=sorted_times_accessed_each_address, 
    x=columns, 
    z=rows,
    mode='markers',
    marker = dict(
        size=12,
        color = sorted_total_accesses,
        colorbar = dict(
            title="Heat Map",
            ticktext=["Sooner", "Later"],
        ),
        colorscale = "Magma"
    )
)])

print("Trying to show figure...\n")
plotly.offline.plot(fig, filename = "figures/2d_memory_access_pattern.html")
