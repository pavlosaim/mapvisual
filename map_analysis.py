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
class mclass:

	def __init__(self,window):
		global screen_width,screen_height,retain_files_fl
		screen_width=window.winfo_screenwidth()
		screen_height=window.winfo_screenheight()
		figList=[]
		figNames=[]
		retain_files_fl=0

		self.window=window
		self.initUI()
		self.window.minsize(width=720,height=586)
		self.window.maxsize(width=screen_width,height=screen_height)
		self.window.resizable(TRUE,TRUE)

		self.bottomframe=Frame(window,relief=RAISED,borderwidth=2)
		self.bottomframe.pack(side=BOTTOM,fill=BOTH,expand=1)

		#self.button1=Button(self.bottomframe,height=3,text="Reuse Distance",state=DISABLED,command=self.reuseDistance)
		#self.button1.pack(anchor=S,fill=BOTH,expand=1)
		#self.button3=Button(self.bottomframe,height=3,text="Variable Metrics",state=DISABLED,command=self.createVarMetrics)
		#self.button3.pack(anchor=S,fill=BOTH,expand=1)
		self.button8=Button(self.bottomframe,height=3,text="Memory Access Pattern per Array (3D)",state=DISABLED,command=self.memAccessPatternperArray3D)
		self.button8.pack(anchor=S,fill=BOTH,expand=1)
		self.button7=Button(self.bottomframe,height=3,text="Memory Access Pattern per Array (2D)",state=DISABLED,command=self.memAccessPatternperArray2D)
		self.button7.pack(anchor=S,fill=BOTH,expand=1)
		self.button6=Button(self.bottomframe,height=3,text="Memory Access Pattern",state=DISABLED,command=self.memAccessPattern)
		self.button6.pack(anchor=S,fill=BOTH,expand=1)
		self.button5=Button(self.bottomframe,height=3,text="Memory Activity Graph",state=DISABLED,command=self.createMemoryGraph)
		self.button5.pack(anchor=S,fill=BOTH,expand=1)
		self.button4=Button(self.bottomframe,height=3,text="Additional Information",state=DISABLED,command=self.displayProgramInfo)
		self.button4.pack(anchor=S,fill=BOTH,expand=1)


		self.topframe=Frame(window,relief=GROOVE,borderwidth=1)
		self.topframe.pack(side=TOP,fill=BOTH,expand=1)

		self.log=ScrolledText(self.topframe,height=30,width=30)

		self.log.insert(END,"Remember:\n - Include path to gleipnir.h in your source code.\n - Insert the GL_START command at the start of your function to be analyzed.\n - Insert the GL_STOP command at the end of your function to be analyzed. \n")
		self.log.insert(END,"Memory Access Pattern per Array (2D) works for Heap Allocations.\n")
		self.log.insert(END,"Memory Access Pattern per Array (3D) works for Heap Allocations, but does not so occurence index.\n")
		self.log.insert(END,"Load your executable...\n")
		self.log.config(state=DISABLED)
		self.log.pack(side=TOP,fill=BOTH,expand=1)


	def initUI(self):
		self.window.title("Memory Access Pattern Analysis")

		menubar=Menu(self.window)
		self.window.config(menu=menubar)

		fileMenu=Menu(menubar)
		fileMenu.add_command(label="Open file",command=self.chooseFile)
		fileMenu.add_command(label="Clear Analysis",command=self.clearAnalysis)
		fileMenu.add_command(label="Use existing files",command=self.existingFiles)
		fileMenu.add_command(label="Retain analysis files",command=self.retainFiles)
		fileMenu.add_command(label="Clear log",command=self.clearLog)
		fileMenu.add_command(label="Save log",command=self.fileTextSave)
		# fileMenu.add_command(label="Save plots",command=self.savePlots)
		fileMenu.add_command(label="Exit",command=self.onExit)
		menubar.add_cascade(label="File",menu=fileMenu)
		helpMenu=Menu(menubar)
		helpMenu.add_command(label="About",command=self.displayAbout)
		helpMenu.add_command(label="License",command=self.displayLicense)
		menubar.add_cascade(label="Help",menu=helpMenu)

	

	def beginAnalysis(self,filename,stackorheap,arguments,sourcecodepath):
		if filename:		
                        print("Begining Analysis\n")
			self.log.insert(END,"\n")
			file3=open("additional_info.txt","a")
			#Setting stack size
			exitcode=subprocess.check_call(["ulimit -s unlimited"],shell=True)
			# Compiling the c source code.
			#tmp_command="gcc -w -g "+filename+" -o temp_exec -lm"
			starting_point=timer()
			#start=timer()
			#exitcode=subprocess.call([tmp_command],shell=True)
			#end=timer()
			#file3.write("Compiling source code time: 							")
			#file3.write("%.3f s\n" %(end-start))
			#if exitcode != 0:
			#	sys.exit("Compilation error!")
			#self.log.insert(END,"Compiling source code...SUCCESS.\n")
			# Executing the source coude through Valgrind with Gleipnir
			#exitcode=subprocess.check_call(["which valgrind>tmp_path.txt"],shell=True)
			#if exitcode != 0:
				#sys.exit("Valgrind was not found on your system!")
			val_path_file=open("paths/valgrind_path","r")
			val_path=[line[:-1] for line in val_path_file]
			val_path_file.close()
                        print(filename)
                        print("Starting gleipnir analysis...\n")
			tmp_command=val_path[0]+" --quiet --read-var-info=yes --read-debug=yes --out=gl_out --tool=gleipnir "+filename + " " + arguments
			start=timer()
			exitcode=subprocess.check_call([tmp_command],shell=True)
			end=timer()
			file3.write("Valgrind/Gleipnir execution time: 							")
			file3.write("%.3f s\n" %(end-start))
			if exitcode != 0:
				sys.exit("Valgrind/Gleipnir error! Check your installation.")
			self.log.insert(END,"Running source code executable with Valgrind and Gleipnir...SUCCESS.\n")
			# Profiling the executable with Cachegrind
                        print("Starting cachegrind analysis...\n")
			tmp_command=val_path[0]+" --quiet --tool=cachegrind --cachegrind-out-file=cache_out.txt "+filename + " " + arguments
			start=timer()
			exitcode=subprocess.check_call([tmp_command],shell=True)
			end=timer()
			file3.write("Valgrind/Cachegrind execution time: 							")
			file3.write("%.3f s\n" %(end-start))
			if exitcode != 0:
				sys.exit("Valgrind/Cachegrind error! Check your installation.")
			# Finding the output filename
			exitcode=subprocess.check_call(["find . -maxdepth 1 -name gl_out.\*>tmp_path.txt"],shell=True)
			if exitcode != 0:
				sys.exit("Gleipnir output file was not found on your system!")
			self.log.insert(END,"Locating output file...SUCCESS.\n")
			val_path_file=open("tmp_path.txt","r")
			val_path=[line[:-1] for line in val_path_file]
			val_path_file.close()
                        print(val_path)
			tmp_str=str(val_path[0])[2:]
                        print(tmp_str)

                        #Compile memtrace analysis and output processing
                        subprocess.call(["make all"], shell=True) 
			# Executing our code for the gleipnir output file manipulation (creates lut_file & access_list binary files)
			#start=timer()
			#exitcode=subprocess.check_call(["gcc -w -g memtrace_analysis.c -o memtrace_analysis"],shell=True)
			#end=timer()
			#file3.write("Compiling gleipnir output manipulation code: 							")
			#file3.write("%.3f s\n" %(end-start))
			#if exitcode != 0:
				#sys.exit("Compilation error!")
			#self.log.insert(END,"Compiling Gleipnir output analysis source code...SUCCESS.\n")
			subprocess.call(["chmod +x "+"memtrace_analysis"],shell=True)
			subprocess.call(["chmod o+x "+tmp_str],shell=True)
                        print("Running memtrace analysis...\n")
			start=timer()
			exitcode=subprocess.check_call(["./memtrace_analysis " + tmp_str + " " + sourcecodepath] ,shell=True)
			end=timer()
			file3.write("Gleipnir output manipulation code execution time: 					 ")
			file3.write("%.3f s\n" %(end-start))
			self.log.insert(END,"Executing Gleipnir output analysis...SUCCESS.\n")
			# Compiling and executing our data manipulation code
			#start=timer()
			#exitcode=subprocess.check_call(["gcc -w -g output_processing.c -o output_processing -lm"],shell=True)
			#end=timer()
			#file3.write("Compiling data manipulation time: 							")
			#file3.write("%.3f s\n" %(end-start))
			#if exitcode != 0:
				#sys.exit("Compilation error!")
			#self.log.insert(END,"Compiling data manipulation source code...SUCCESS.\n")
			subprocess.call(["chmod +x "+"output_processing"],shell=True)
                        print("Running output processing analysis...\n")
			start=timer()
			exitcode=subprocess.check_call(["ulimit -s unlimited;./output_processing "+str(stackorheap)],shell=True)
			end=timer()
			file3.write("Data manipulation code execution time: 							")
			file3.write("%.3f s\n" %(end-start))
			file3.write("\n")
			file3.write("Total execution time: 							")
			file3.write("%.3f s\n" %(end-starting_point))
			file3.close()
			self.log.insert(END,"Executing output data manipulation...SUCCESS.\n")
			self.log.insert(END,"\n")
			self.log.insert(END,"Analysis complete!\n")
			self.log.insert(END,"\n")
			self.log.config(state=DISABLED)
                        print("Analysis completed!\n")
			#self.button1.config(state=NORMAL)
			#self.button3.config(state=NORMAL)
                        if stackorheap == 'H':
                            self.button8.config(state=NORMAL)
			    self.button7.config(state=NORMAL)
			self.button4.config(state=NORMAL)
			self.button5.config(state=NORMAL)
			self.button6.config(state=NORMAL)
        
        def setStack(self):
            self.stackorheapval='S'

        def setHeap(self):
            self.stackorheapval='H'

	def chooseFile(self):
		global fname
		self.log.config(state=NORMAL)
		self.log.delete("1.0",END)

                #File selection
		fname = tkFileDialog.askopenfilename(filetypes=(("All files", "*"), ("C files", "*.c")))

                #heap or stack choice selection window
                choice = Tk()
                choice.title('Settings')
                Label(choice, text="Analysis for Stack or Heap Allocations?").grid(row=3, sticky=W)
                self.stackorheapval='N'
                self.option = StringVar()
                Radiobutton(choice, text="Stack", variable=self.option, value='S', command=self.setStack).grid(row=5, sticky=W)
                Radiobutton(choice, text="Heap", variable=self.option,  value='V', command=self.setHeap).grid(row=6,sticky=W)
                Label(choice, text="Input arguments for given executable (Leave blank if no arguments)").grid(sticky=W)
                args = Entry(choice)
                args.grid(row=8, sticky=W)
                args.focus_set()
                Label(choice, text="Path to source code directory").grid(sticky=W)
                sourcecodepath = Entry(choice)
                sourcecodepath.grid(row=10, sticky=W)
                sourcecodepath.focus_set()
                Button(choice, text='Apply', command=choice.quit).grid(row=11, sticky=W, pady=4)
                Button(choice, text='Quit', command=choice.destroy).grid(row=12, sticky=W, pady=4)
                choice.mainloop()
                stackorheap = self.stackorheapval

		self.log.insert(END,"Selected file: "+fname+"\n")
		self.beginAnalysis(fname, stackorheap, args.get(), sourcecodepath.get())
		return

	def existingFiles(self):
		self.log.config(state=NORMAL)
		self.log.delete("1.0",END)
		self.log.insert(END,"Files needed for analysis:\n")
		self.log.insert(END,"--------------------------\n")
		self.log.insert(END,"\n")
		self.log.insert(END,"- 	additional_info.txt \n")
		self.log.insert(END,"- 	cache_out.txt \n")
		self.log.insert(END,"- 	lut_stats_data.txt \n")
		self.log.insert(END,"- 	order_data.txt \n")
		self.log.insert(END,"- 	scatterplot_data.txt \n")
		self.log.insert(END,"- 	stats.txt \n")
		self.log.insert(END,"\n")

		#self.button1.config(state=NORMAL)
		#self.button3.config(state=NORMAL)
		self.button4.config(state=NORMAL)
		self.button5.config(state=NORMAL)
		self.button6.config(state=NORMAL)
		self.button7.config(state=NORMAL)
		self.button8.config(state=NORMAL)
		return

	def retainFiles(self):
		retain_files_fl=1
		val_path_file=open("tmp_path.txt","r")
		val_path=[line[:-1] for line in val_path_file]
		val_path_file.close()
		tmp_str=str(val_path[0])[2:]
		pos=tmp_str.find("gl_out.")
		t_str=tmp_str[pos:]
		subprocess.call(["mkdir "+"map_"+t_str],shell=True)
		subprocess.call(["cp additional_info.txt map_"+t_str],shell=True)
		subprocess.call(["cp cache_out.txt map_"+t_str],shell=True)
		subprocess.call(["cp lut_stats_data.txt map_"+t_str],shell=True)
		subprocess.call(["cp order_data.txt map_"+t_str],shell=True)
		subprocess.call(["cp scatterplot_data.txt map_"+t_str],shell=True)
		subprocess.call(["cp stats.txt map_"+t_str],shell=True)

		self.log.config(state=NORMAL)
		self.log.insert(END,"\n")		
		self.log.insert(END,"Analysis output files stored in folder: map_"+t_str)
		self.log.insert(END,"\n")
		self.log.config(state=DISABLED)
		return

	def clearLog(self):
		self.log.config(state=NORMAL)
		self.log.delete("1.0",END)
		self.log.config(state=DISABLED)

        def clearAnalysis(self):
		if retain_files_fl == 0:
			subprocess.call(["rm cache_out.txt -f;rm scatterplot_data.txt -f;rm lut_stats_data.txt -f;rm order_data.txt -f;rm additional_info.txt -f;rm stats.txt -f"],shell=True)
		subprocess.call(["rm gl_out.* -f;rm temp_exec -f;rm tmp_path.txt -f;rm lut_file -f;rm access_list -f;rm output_processing -f;rm memtrace_analysis -f;rm src/functions.o -f; rm alloc_info -f; rm MemoryAccessPatternPerArrayFigure3D.html MemoryAccessPatternPerArrayFigure2D.html MemoryAccessPatternFig.html"],shell=True)
		self.log.config(state=NORMAL)
		self.log.delete("1.0",END)
                self.log.insert(END,"Everything has been cleared.\n Load your new executable or exit.\n")
		self.log.config(state=DISABLED)

	def onExit(self):
		if retain_files_fl == 0:
			subprocess.call(["rm cache_out.txt -f;rm scatterplot_data.txt -f;rm lut_stats_data.txt -f;rm order_data.txt -f;rm additional_info.txt -f;rm stats.txt -f"],shell=True)
		subprocess.call(["rm gl_out.* -f;rm temp_exec -f;rm tmp_path.txt -f;rm lut_file -f;rm access_list -f;rm output_processing -f;rm memtrace_analysis -f;rm src/functions.o -f; rm alloc_info -f; rm MemoryAccessPatternPerArrayFigure3D.html MemoryAccessPatternPerArrayFigure2D.html MemoryAccessPatternFig.html"],shell=True)
		self.window.quit()

	def displayProgramInfo(self):
		self.log.config(state=NORMAL)
		self.log.insert(END,"\n")
		file3=open("additional_info.txt","r")
		self.log.insert(END,"Additional info:\n")
		self.log.insert(END,"---------------\n")
		for line in file3:
			self.log.insert(END,line)
		file3.close()

		self.log.insert(END," \nCache statistics:\n")
		self.log.insert(END,"--------------------\n")
		self.log.insert(END,"\n")

		file3=open("cache_out.txt","r")
		i=0
		for line in file3:
			if i < 3:
				self.log.insert(END,line[6:])
			i+=1
		self.log.insert(END,"\n")
		
		cache_metrics=[]
		for el in line.strip().split()[1:]:
			cache_metrics.append(el)

		irefs = int(cache_metrics[0])
		i1misses = int(cache_metrics[1])
		i1miss_rate ='%s' % (float('%.2g' % (i1misses/irefs))*100)
		llimisses = int(cache_metrics[2])
		llimiss_rate ='%s' % (float('%.2g' % (llimisses/irefs))*100)

		drefs_rd = int(cache_metrics[3])
		d1misses_rd = int(cache_metrics[4])
		lldmisses_rd = int(cache_metrics[5])
		drefs_wr = int(cache_metrics[6])
		d1misses_wr = int(cache_metrics[7])
		lldmisses_wr = int(cache_metrics[8])
		drefs_total = drefs_rd+drefs_wr
		d1misses_total = d1misses_rd+d1misses_wr
		lldmisses_total = lldmisses_rd+lldmisses_wr
		d1miss_rate_rd ='%s' % (float('%.2g' % (d1misses_rd/drefs_rd))*100)
		lldmiss_rate_rd ='%s' % (float('%.2g' % (lldmisses_rd/drefs_rd))*100)
		d1miss_rate_wr ='%s' % (float('%.2g' % (d1misses_wr/drefs_wr))*100)
		lldmiss_rate_wr ='%s' % (float('%.2g' % (lldmisses_wr/drefs_wr))*100)
		d1miss_rate_total = '%s' % (float('%.2g' % (d1misses_total/drefs_total))*100)
		lldmiss_rate_total = '%s' % (float('%.2g' % (lldmisses_total/drefs_total))*100)

		llrefs_rd = d1misses_rd+i1misses
		llrefs_wr = d1misses_wr
		llrefs_total = llrefs_rd+llrefs_wr
		llmisses_rd = llimisses+lldmisses_rd
		llmisses_wr = lldmisses_wr
		llmisses_total = llmisses_rd+llmisses_wr
		refs_rd = irefs+drefs_rd 
		refs_wr = irefs+drefs_wr
		refs_total = irefs+drefs_total
		llmiss_rate_rd = '%s' % (float('%.2g' % (llmisses_rd/refs_rd))*100)
		llmiss_rate_wr = '%s' % (float('%.2g' % (llmisses_wr/refs_wr))*100)
		llmiss_rate_total = '%s' % (float('%.2g' % (llmisses_total/refs_total))*100)


		self.log.insert(END,"I refs 		"+str(irefs)+"\n")
		self.log.insert(END,"I1 misses 		"+str(i1misses)+"\n")
		self.log.insert(END,"LLi misses 		"+str(llimisses)+"\n")
		self.log.insert(END,"I1 miss rate 		"+str(i1miss_rate)+"%\n")
		self.log.insert(END,"LLi miss rate 		"+str(llimiss_rate)+"%\n")
		self.log.insert(END,"\n")
		self.log.insert(END,"D refs 		"+str(drefs_total)+" ( "+str(drefs_rd)+" rd + "+str(drefs_wr)+" wr )\n")
		self.log.insert(END,"D1 misses 		"+str(d1misses_total)+" ( "+str(d1misses_rd)+" rd + "+str(d1misses_wr)+" wr )\n")
		self.log.insert(END,"LLd misses 		"+str(lldmisses_total)+" ( "+str(lldmisses_rd)+" rd + "+str(lldmisses_wr)+" wr)\n")
		self.log.insert(END,"D1 miss rate 		"+str(d1miss_rate_total)+"% ( "+str(d1miss_rate_rd)+"% + "+str(d1miss_rate_wr)+"% )\n")
		self.log.insert(END,"LLd miss rate 		"+str(lldmiss_rate_total)+"% ( "+str(lldmiss_rate_rd)+"% + "+str(lldmiss_rate_wr)+"% )\n")
		self.log.insert(END,"\n")
		self.log.insert(END,"LL refs 		"+str(llrefs_total)+" ( "+str(llrefs_rd)+" rd + "+str(llrefs_wr)+" wr )\n")
		self.log.insert(END,"LL misses 		"+str(llmisses_total)+" ( "+str(llmisses_rd)+" rd + "+str(llmisses_wr)+" wr )\n")
		self.log.insert(END,"LL miss rate 		"+str(llmiss_rate_total)+"% ( "+str(llmiss_rate_rd)+"% + "+str(llmiss_rate_wr)+"% )\n")
		self.log.insert(END,"\n")
		self.log.config(state=DISABLED)



	def displayAbout(self):
		file=open("About.txt","r")
		about = ""
		for line in file:
			about+=line
		file.close()
		tkMessageBox.showinfo("About",about)


	def displayLicense(self):
		file=open("LICENSE.txt","r")
		license = ""
		for line in file:
			license+=line
		file.close()
		tkMessageBox.showinfo("License",license)


	def fileTextSave(self):
	    f = tkFileDialog.asksaveasfile(mode='w+', defaultextension=".txt")
	    if f is None:
	        return
	    text2save = str(self.log.get(14.0, END))
	    f.write(text2save)
	    f.close()


	def filterResults(self,value):
		loads = []
		stores = []
		modifies = []
		if value is not "Select variable":
			if value == 'All':
				for idx in t_new:
					if idx[3] == "S":
						loads.append(-5)
						stores.append(int(idx[2]))
						modifies.append(-5)
					if idx[3] == "L":
						loads.append(int(idx[2]))
						stores.append(-5)
						modifies.append(-5)
					if idx[3] == "M":
						loads.append(-5)
						stores.append(-5)
						modifies.append(int(idx[2]))
			else:			
				for idx in t_new:
					if idx[4] == value:
						if idx[3] == "S":
							loads.append(-5)
							stores.append(int(idx[2]))
							modifies.append(-5)
						if idx[3] == "L":
							loads.append(int(idx[2]))
							stores.append(-5)
							modifies.append(-5)
						if idx[3] == "M":
							loads.append(-5)
							stores.append(-5)
							modifies.append(int(idx[2]))
					else:
						loads.append(-5)
						stores.append(-5)
						modifies.append(-5)			

		a5 = f5.add_subplot(111)
		a5.set_title("Memory Activity")
		a5.set_yticks(sorted(new_lut_indices))
		a5.tick_params(axis='y', which='both', pad=10,labelsize=10)
		a5.set_xticks([])
		a5.set_ylim(bottom=-2)
		a5.set_yticklabels(var_names_list)
		a5.set_ylabel("Variables",fontsize=15) 
		a5.set_xlabel("Time",fontsize=15)
		a5.legend(['Load','Store','Modify'],mode='expand',bbox_to_anchor=(0., -0.1, 1., .13), loc=3, ncol=3)#bbox_to_anchor=(0.4825, -0.04),fancybox=True,shadow=True,ncol=3)
		a5.scatter(all_ticks,loads,color='green',s=1)
		a5.scatter(all_ticks,stores,color='red',s=1)
		a5.scatter(all_ticks,modifies,color='blue',s=1)
		a5.legend(['Load','Store','Modify'],mode='expand',bbox_to_anchor=(0., -0.1, 1., .13), loc=3, ncol=3)#bbox_to_anchor=(0.4825, -0.04),fancybox=True,shadow=True,ncol=3)
		
		# Forcing window redreaw by registering resize events
		w=memory_window.winfo_width()
		h=memory_window.winfo_height()
		num =1
		wnew = w+num
		hnew = h+num
		new_size_a = str(wnew)+"x"+str(hnew)
		new_size_b = str(wnew-num)+"x"+str(hnew-num)

		memory_window.geometry(new_size_a)
		memory_window.update_idletasks()
		memory_window.geometry(new_size_b)
		memory_window.update_idletasks()

		canvas = FigureCanvasTkAgg(f5, master=memory_window)
		canvas.draw()


	def clearGraph(self):
		a5 = f5.clf()
		memory_window.update_idletasks()
		memory_window.after(10, self.filterResults("Select variable"))


	def showMoreDetails(self,input):
		if input == 0:
			self.log.config(state=NORMAL)
			self.log.insert(END,"\n")
			self.log.insert(END,"Reuse distance details:\n")
			self.log.insert(END,"-----------------------\n")
			self.log.insert(END,"\n")
			self.log.insert(END,"Reuse distance 		Number of times\n")
			self.log.insert(END,"--------------------------------------\n")
			file1=open("scatterplot_data.txt","r")
			sublist_a = []
			sublist_b = []
			for line in file1:
				val1,val2=line.strip().split()
				norm_val1=long(val1)
				norm_val2=long(val2)
				sublist_a.append(norm_val1)
				sublist_b.append(norm_val2)
			sorted_list=sorted(zip(sublist_a,sublist_b))
			for el in sorted_list:
				tmp_str=str(el[0])+" 		"+str(el[1])+"\n"
				self.log.insert(END,tmp_str)
			self.log.insert(END,"\n")
			self.log.config(state=DISABLED)
			file1.close()		
		if input == 2:
			self.log.config(state=NORMAL)
			self.log.insert(END,"Most accessed memory locations:\n")
			self.log.insert(END,"-------------------------------\n")
			self.log.insert(END,"\n")
			self.log.insert(END,"#	Memory Address		Variable Name 			Accesses 		Bytes Stored 		Bytes Loaded\n")
			self.log.insert(END,"-----------------------------------------------------------------------------------------------\n")
			file2=open("lut_stats_data.txt","r")
		 	lut_idx=[]
			idx_times=[]
			lut_idx_addr=[]
			lut_idx_var_name=[]
			mem_s=[]
			mem_l=[]
                        #Pavlos Aimoniotis: Probably this have to change in order to count all the addresses from the same start. 
                        #Should be something like for every variable name in file.
			for line in file2:
				val1,val2,str1,val3,val4,str2=line.strip().split()
				lut_idx.append(int(val1))
				idx_times.append(int(val2))
				lut_idx_addr.append(str1)
				lut_idx_var_name.append(str2)
				mem_s.append(val3)
				mem_l.append(val4)
			file2.close()
			new_zip_list=zip(idx_times,lut_idx,lut_idx_addr,lut_idx_var_name,mem_s,mem_l)
	 		treemap_list=sorted(new_zip_list,reverse=True)
                        single=[]
                        single_addr=[]
                        for el in zip(lut_idx_var_name, lut_idx_addr):
                            if ((el[0] not in single) and ("00" in el[1])) :
                               single.append(el[0])
                               single_addr.append(el[1])
                        address=[]
                        for el in zip(lut_idx_var_name):
                            for se in zip(single, single_addr):
                               if (el[0] == se[0]):
                                address.append(se[1])
	 		i=1
	 		for el in treemap_list:
	 			self.log.insert(END,str(i)+"	"+el[2]+"		"+el[3]+"			"+str(el[0])+"	 	"+el[4]+"	 	"+el[5]+"\n")
	 			i+=1
			self.log.insert(END,"\n")
			self.log.config(state=DISABLED)
		if input == 3:
			self.log.config(state=NORMAL)
			self.log.insert(END,"Memory accesses order:\n")
			self.log.insert(END,"----------------------\n")
			self.log.insert(END,"\n")
			file7=open("order_data.txt","r")
			l_idx = []
			addr = []
			ops = []
			v_names = []

			for line in file7:
				val1,str1,char1,str2=line.strip().split()
				l_idx.append(val1)
				addr.append(str1)
				ops.append(char1)
				v_names.append(str2)
			file7.close()
			zip_list = zip(l_idx,addr,ops,v_names)
			i = 1
			self.log.insert(END,"# 	Address 	    Operation 	Variable_name\n")
			self.log.insert(END,"--------------------------------------------\n")
			for el in zip_list:
				self.log.insert(END,str(i)+" 	"+str(el[1])+" 		"+str(el[2])+" 	"+str(el[3])+"\n")
				i+=1
			self.log.insert(END,"\n")
			self.log.config(state=DISABLED)
		self.log.config(state=NORMAL)
		self.log.insert(END,"\n")
		self.log.config(state=DISABLED)	
		

        ######################
        #Temporary out of use#
        ######################
	def reuseDistance(self):

	 	scatter_window=Toplevel(window)
	 	scatter_window.title("Reuse distance")
	 	scatter_window.minsize(width=1024,height=800)
	 	scatter_window.resizable(TRUE,TRUE)

	 	file1=open("scatterplot_data.txt","r")
	 	reuse_distance=[]
		rd_times=[]

		for line in file1:
			val1,val2=line.strip().split()
			reuse_distance.append(int(val1))
			rd_times.append(int(val2))
		file1.close()

		file2=open("stats.txt","r")
		avg_rd = []
		var_rd = []

		for line in file2:
			val1,val2 = line.strip().split()
			avg_rd.append(float(val1))
			var_rd.append(float(val2))

		#Colors
	 	cmap = matplotlib.cm.brg
	 	mini, maxi = min(rd_times), max(rd_times)
		norm = matplotlib.colors.Normalize(vmin=mini, vmax=maxi)
		colors = [cmap(norm(value)) for value in rd_times]

		full_x_seq = [i for i in range(0,len(avg_rd))]

		global f2
		f2 = Figure(figsize=(7,7))
		f2.subplots_adjust(hspace = 0.3,wspace=0.3)
		
		a2a = f2.add_subplot(211)
		a2a.scatter(reuse_distance, rd_times, s=50,alpha=0.5)
		a2a.set_xlim(left=-10)
		a2a.set_ylim(bottom=-50)
		a2a.set_title("Reuse Distance")
		a2a.set_ylabel("Number of times")
		temp_ticks_list=a2a.get_xticks()
		temp_ticks_list=[int(x) for x in temp_ticks_list]
		a2a.set_xticklabels(temp_ticks_list,fontsize=10,rotation=90)

		a2b = f2.add_subplot(223)
		a2b.plot(full_x_seq,avg_rd,color='black')
		a2b.set_title("Average Reuse Distance")
		a2b.set_ylabel('Avg Reuse distance')
		a2b.set_xlabel('Accesses')
		temp_ticks_list=a2b.get_xticks()
		temp_ticks_list=[int(x) for x in temp_ticks_list]
		a2b.set_xticklabels(temp_ticks_list,fontsize=10,rotation=90)

		a2c = f2.add_subplot(224)
		a2c.scatter(full_x_seq, var_rd, s=1,alpha=0.5,color='red')
		a2c.set_title("Reuse Distance Variance")
		a2c.set_ylabel('Standard Deviation')
		a2c.set_xlabel('Accesses')
		temp_ticks_list=a2c.get_xticks()
		temp_ticks_list=[int(x) for x in temp_ticks_list]
		a2c.set_xticklabels(temp_ticks_list,fontsize=10,rotation=90)

		canvas = FigureCanvasTkAgg(f2, master=scatter_window)
		tkagg.NavigationToolbar2TkAgg(canvas, scatter_window)
		canvas.get_tk_widget().pack(fill=BOTH,expand=TRUE)		
		canvas.draw()


		self.showMoreDetails(0)



        ######################
        #Temporary out of use#
        ######################
	def createVarMetrics(self):

	 	treemap_window=Toplevel(window)
	 	treemap_window.title("Variable Metrics")
	 	treemap_window.minsize(width=1024,height=800)
	 	treemap_window.resizable(TRUE,TRUE)

	 	file2=open("lut_stats_data.txt","r")
	 	lut_idx=[]
		idx_times=[]
		lut_idx_addr=[]
		lut_idx_var_name=[]
		mem_s=[]
		mem_l=[]
		for line in file2:
			val1,val2,str1,val3,val4,str2=line.strip().split()
			lut_idx.append(int(val1))
			idx_times.append(int(val2))
			lut_idx_addr.append(str1)
			lut_idx_var_name.append(str2)
			mem_s.append(val3)
			mem_l.append(val4)
		file2.close()

		y_ticks = [i for i in range(1,len(set(lut_idx_var_name))+1)]
		bar_list = []
		for el in list(set(lut_idx_var_name)):
			t_s = 0
			t_l = 0
			t_t = 0
			for rec in zip(lut_idx_var_name,mem_s,mem_l,idx_times):
				if el == rec[0]:
					t_s += int(rec[1])
					t_l += int(rec[2])
					t_t = int(rec[3])
			t = (el,t_s,t_l,t_t)
			bar_list.append(t)

		global f4
	 	f4=Figure(figsize=(7,7))
	 	a4a = f4.add_subplot(121,aspect="auto")
	 	a4b = f4.add_subplot(122,aspect="auto")

	 	b1 = []
	 	b2 = []
	 	b3 = []
	 	for el in bar_list:
	 		b1.append(el[1])
	 		b2.append(el[2])
	 		b3.append(el[3])

	 	b_width = 0.4
	 	y1 = []
	 	y2 = []
	 	y3 = []
	 	for i in y_ticks:
	 		y1.append(i-0.4)
	 		y2.append(i)
	 		y3.append(i-0.2)
	 	rects1 = a4a.barh(y1,b1,b_width, color='red')
	 	rects2 = a4a.barh(y2,b2,b_width, color='green')
	 	rects3 = a4b.barh(y3,b3,b_width,color='blue')
	 	a4a.legend((rects1[0], rects2[0]), ('Stored', 'Loaded'))

	 	a4a.set_yticks(y_ticks)
	 	a4a.set_ylim(top=(max(y_ticks)+2))
		a4a.set_yticklabels(list(set(lut_idx_var_name)))
		a4a.tick_params(axis='y', which='both', pad=10,labelsize=10,direction='out')
		a4a.set_title("Memory Usage Per Variable")
		a4a.set_ylabel("Variables",fontsize=15)
		a4a.yaxis.set_label_position("right")
		a4a.set_xlabel("Bytes",fontsize=15)
		temp_ticks_list=a4a.get_xticks()
		temp_ticks_list=[int(x) for x in temp_ticks_list]
		a4a.set_xticklabels(temp_ticks_list,fontsize=10,rotation=90)
		a4a.grid()

		a4b.set_yticks(y_ticks)
		a4b.set_ylim(top=(max(y_ticks)+2))
		a4b.set_yticklabels(list(set(lut_idx_var_name)))
		a4b.tick_params(axis='y', which='both', pad=10,labelsize=10,direction='out')
		a4b.set_title("Accesses Per Variable")
		a4b.set_ylabel("Variables",fontsize=15)
		a4b.yaxis.set_ticks_position("right")
		a4b.set_xlabel("Number of accesses",fontsize=15)
		temp_ticks_list=a4b.get_xticks()
		temp_ticks_list=[int(x) for x in temp_ticks_list]
		a4b.set_xticklabels(temp_ticks_list,fontsize=10,rotation=90)
		a4b.grid()

	 	canvas = FigureCanvasTkAgg(f4, master=treemap_window)
	 	tkagg.NavigationToolbar2TkAgg(canvas, treemap_window)
		canvas.get_tk_widget().pack(fill=BOTH,expand=TRUE)		
		canvas.draw()

	 	self.showMoreDetails(2)
        
        def memAccessPattern(self):
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
		self.log.config(state=NORMAL)
		self.log.insert(END,"Index      Address         Variable        Occurence Index\n")
		self.log.insert(END,"----------------------------------------------------------\n")
                for entry in sorted(zip(total_accesses,addresses, order_of_names, sorted_times_accessed_each_address)):
		    self.log.insert(END,str(entry[0])+"      "+str(entry[1])+"           	"+str(entry[2])+"              "+str(entry[3])+"\n")
		
                self.log.config(state=DISABLED)
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
                plotly.offline.plot(fig, filename = "MemoryAccessPatternFig.html")
                
                '''fig = plt.figure()
                ax = plt.axes(projection='3d')


                print(sorted_times_accessed_each_address)
                ax.scatter3D(sorted_order_of_names, sorted_addresses, sorted_times_accessed_each_address, color = gradient, s = 25, marker="^")


                ax.set_xlabel("Variable");
                ax.set_ylabel("Address");
                ax.set_zlabel("Access Occurence");
                ax.set_title("Heap Memory Access Pattern")

                plt.show()'''

        def memAccessPatternperArray2D(self):
                settings = Tk()
                settings.title("Settings")
                Label(settings, text="Which array do you want to visualise").grid(row=3, sticky=W)
                variable = Entry(settings)
                variable.grid(row=4, sticky=W)
                variable.focus_set()
                Label(settings, text="How many columns does it have?").grid(row=5,sticky=W)
                columns= Entry(settings)
                columns.grid(row=6, sticky=W)
                columns.focus_set()
                Label(settings, text="How many bytes per entry? (4 for Int, 8 for Double etc)?").grid(row=7,sticky=W)
                typebytes = Entry(settings)
                typebytes.grid(row=8, sticky=W)
                typebytes.focus_set()
                Button(settings, text='Apply', command=settings.quit).grid(row=9, sticky=W, pady=4)
                Button(settings, text='Quit', command=settings.destroy).grid(row=10, sticky=W, pady=4)
                settings.mainloop()

                array = variable.get()
                cols = columns.get()
                typeb = typebytes.get()

                order_data_file = open("order_data.txt","r");
                addresses = []
                operations = []
                order_of_names = []
                lut_indices = []
                total_accesses = []
                count = 0;
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

                if (found == 0):
                    self.log.config(state=NORMAL)
                    self.log.insert(END,"\nThere is not an array with name:" + array + "\n")
                    self.log.config(state=DISABLED)
                    return 

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
		self.log.config(state=NORMAL)
		self.log.insert(END,"Index      Address         Variable        Occurence Index\n")
		self.log.insert(END,"----------------------------------------------------------\n")
                for entry in sorted(zip(total_accesses,addresses, order_of_names, sorted_times_accessed_each_address)):
		    self.log.insert(END,str(entry[0])+"      "+str(entry[1])+"           	"+str(entry[2])+"              "+str(entry[3])+"\n")
		
                self.log.config(state=DISABLED)
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
                plotly.offline.plot(fig, filename = "MemoryAccessPatternPerArrayFigure2D.html")

        def memAccessPatternperArray3D(self):
                settings = Tk()
                settings.title("Settings")
                Label(settings, text="Which array do you want to visualise").grid(row=3, sticky=W)
                variable = Entry(settings)
                variable.grid(row=4, sticky=W)
                variable.focus_set()
                Label(settings, text="How many columns does it have (j)?").grid(row=5,sticky=W)
                columns= Entry(settings)
                columns.grid(row=6, sticky=W)
                columns.focus_set()
                Label(settings, text="What is the depth of the 3x3 array (k)?").grid(row=7,sticky=W)
                depthent = Entry(settings)
                depthent.grid(row=8, sticky=W)
                depthent.focus_set()
                Label(settings, text="How many bytes per entry? (4 for Int, 8 for Double etc)?").grid(row=9,sticky=W)
                typebytes = Entry(settings)
                typebytes.grid(row=10, sticky=W)
                typebytes.focus_set()
                Button(settings, text='Apply', command=settings.quit).grid(row=11, sticky=W, pady=4)
                Button(settings, text='Quit', command=settings.destroy).grid(row=12, sticky=W, pady=4)
                settings.mainloop()

                array = variable.get()
                cols = columns.get()
                depth = depthent.get()
                typeb = typebytes.get()

                order_data_file = open("order_data.txt","r");
                addresses = []
                operations = []
                order_of_names = []
                lut_indices = []
                total_accesses = []
                count = 0;
                # Parse order_data file
                for line in order_data_file:
                    val1,str1,char1,str2 = line.strip().split()
                    if str2 == array:
                        lut_indices.append(val1)
                        addresses.append(str1)
                        print(str1)
                        operations.append(char1)
                        order_of_names.append(str2)
                        count = count + 1
                        total_accesses.append(count);
                order_data_file.close()
                
                found = 0
                # Parse alloc_info file
                # in alloc file the start point is stored
                alloc_info = open("alloc_info","r")
                for line in alloc_info:
                    addr1,str1= line.strip().split()
                    if addr1 in addresses:
                        found = 1
                        start_addr = addr1 
                        total_bytes_alloced = str1
                order_data_file.close()

                if (found == 0):
                    self.log.config(state=NORMAL)
                    self.log.insert(END,"\nThere is not an array with name:" + array + "\n")
                    self.log.config(state=DISABLED)
                    return 

                print("Creating data for figure drawing...\n")
                #Sort them as a pack. Every index will refer to a specifix line of index access and address
                sorted_addresses = []
                sorted_total_accesses = []
                rows = []
                columns = []
                kdepth = []
                #Sort them by address
                for entry in sorted(zip(total_accesses,addresses)):
                    sorted_addresses.append(entry[1])
                    difference = int(entry[1],16) - int(start_addr,16)
                    element = difference / int(typeb)
                    intcols = int(cols)
                    intdepth = int(depth)
                    total = intcols * intdepth
                    i = element // total 
                    ans = element - (i * total)
                    j = ans // int(depth)
                    k = ans % int(depth)
                    rows.append(i)
                    columns.append(j)
                    kdepth.append(k)
                    sorted_total_accesses.append(entry[0])

                '''
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
                '''

                # Print on Window
		self.log.config(state=NORMAL)
		self.log.insert(END,"Index      Address         Variable\n")
		self.log.insert(END,"------------------------------------\n")
                for entry in sorted(zip(total_accesses,addresses, order_of_names)):
		    self.log.insert(END,str(entry[0])+"      "+str(entry[1])+"           	"+str(entry[2])+"\n")
		
                self.log.config(state=DISABLED)
                print("Data was created.\n")
                print("Starting painting...\n")
                fig = go.Figure(data=[go.Scatter3d(
                    y=kdepth, 
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
                plotly.offline.plot(fig, filename = "MemoryAccessPatternPerArrayFigure3D.html")

	def createMemoryGraph(self):
		global memory_window
		memory_window=Toplevel(window)
	 	memory_window.title("Memory Activity Graph")
	 	memory_window.minsize(width=1280,height=900)
	 	memory_window.resizable(1,1)

	 	global loads,stores,modifies,order_of_names
		addresses = []
		operations = []
		lut_indices = []
		order_of_names = []		
		loads = []
		stores = []
		modifies = []
		file5=open("order_data.txt","r")

		for line in file5:
			val1,str1,char1,str2=line.strip().split()
			lut_indices.append(val1)
			addresses.append(str1)
			operations.append(char1)
			order_of_names.append(str2)
		file5.close()

		global f5
		f5 = Figure(figsize=(7,7))
		a5 = f5.add_subplot(111)

		address_int = []
		time_progression = []

		for el in addresses:
			address_int.append(int(el,16))

		file6=open("additional_info.txt","r")
		i = 0
		tmp_str2 = []
		for line in file6:
			if i == 9:
				tmp_str2=line[40:]
			i+=1
		
		num_str = ""
		k = 0 
		for c in tmp_str2:
			if c.isdigit():
				num_str += c
				k+=1
			if c == '.':
				num_str += '.'
		
		num_fl = float(num_str)
		file6.close()

		ordered_list=sorted(zip(address_int,addresses))
		unique_idx_set = set(lut_indices)
		global new_lut_indices
		new_lut_indices = list(unique_idx_set)
		new_lut_indices = sorted(list(map(int, new_lut_indices)))
		
		global lut_idx
		
		file2=open("lut_stats_data.txt","r")
	 	lut_idx=[]
		idx_times=[]
		lut_idx_addr=[]
		lut_idx_var_name=[]
		mem_s=[]
		mem_l=[]

		for line in file2:
			val1,val2,str1,val3,val4,str2=line.strip().split()
			lut_idx.append(int(val1))
			idx_times.append(int(val2))
			lut_idx_addr.append(str1)
			lut_idx_var_name.append(str2)
			mem_s.append(val3)
			mem_l.append(val4)
		file2.close()

		global var_names_list
		var_names_list = []
		sorted_indices = []
		sorted_addresses = []		# SORT addresses FROM lut
		sorted_var_names = sorted(lut_idx_var_name)
		i = 0
		for el in sorted(zip(lut_idx_var_name,lut_idx,lut_idx_addr)):
			try:
				var_names_list.index(el[0])
				var_names_list.append("")
			except:
				var_names_list.append(el[0])
			sorted_indices.append(i)
			sorted_addresses.append(el[2])
			i+=1

		addresses_list = []			# SORT addresses BASED ON var_name
		for el in addresses:
			addresses_list.append(sorted_addresses.index(el))

		global t_list,t_new
		t_list = zip(address_int,addresses,lut_indices,operations)
		t_new = zip(address_int,addresses,addresses_list,operations,order_of_names)
	

		global all_ticks
		all_ticks = []
		for i in range(0,len(order_of_names)):
			all_ticks.append(i)
			# Initializing values outside graph
			loads.append(-5)
			stores.append(-5)
			modifies.append(-5)

		a5.set_title("Memory Activity")
		a5.set_yticks(new_lut_indices)
		
		
		a5.tick_params(axis='y', which='both', pad=10,labelsize=10)
		a5.set_xticks([])
		a5.set_ylim(bottom=-2)
		a5.set_yticklabels(var_names_list)
		a5.set_ylabel("Variables",fontsize=15) 
		a5.set_xlabel("Time",fontsize=15)
		a5.legend(['Load','Store','Modify'],mode='expand',bbox_to_anchor=(0., -0.1, 1., .13), loc=3, ncol=3)
		

		# Buttons frame
		fm = Frame(memory_window,height=2,bd=1,relief=SUNKEN)

		# Option menu
		choices = ['All']
	 	choices += [i for i in sorted(list(set(var_names_list))) if i is not ""]
		variable = StringVar(memory_window)
		variable.set('Select Variables')
		om = OptionMenu(fm, variable, *choices,command=self.filterResults)
		om.config(font=('calibri',(10)),width=30)
		om.pack(side=LEFT)

		# Clear graph button
		btn = Button(fm,text="Clear graph",command=self.clearGraph)
		btn.config(font=('calibri',(10)),width=30)
		btn.pack(side=LEFT)

		fm.pack()

		canvas = FigureCanvasTkAgg(f5, master=memory_window)
		tkagg.NavigationToolbar2TkAgg(canvas, memory_window)
		canvas.get_tk_widget().pack(fill=BOTH,expand=TRUE)
		canvas.draw()

		self.showMoreDetails(3)


if __name__ == '__main__':
	window=Tk()
	start=mclass (window)
	window.mainloop()
