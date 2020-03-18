# mapvisual
Software for analysis and visualization of Memory Access Pattern (MAP)

MAPvisual is a tool that creates and combines memory tracing and filtering proccesses to create data for the visualisation of memory access 
patterns through a TCL shell. At the moment:

<<<<<<< HEAD
-You can visualise the complete execution of the executable, and see it on a 3D plot, containing information about Addresses, Array (or Variable), Occurrence Index and a Heat Map according to time of appearance.

-You can visualise a specific 2D Array, and see it as inserted on a 3D plot, where every position is determined through (i, j) (or z, x). Axis contain information about Row index, Col index, Occurrence Index (y axis) and a Heat Map according to time of appearance.

-You can visualise a 3D Array with i, j, k (z, x, y respectively) on axis and a heat map on time of appearance. On 3x3 array visualisation we do not take in consideration occurrence index.

-Memory Activity Graph of operations (Store, Load and Modify) on a 2D plot, Y-axis contains Variables and X-axis Time.
=======
-You can visualise the complete execution of the executable, and see it on a 3D plot, containing information about Addresses, 
  Array (or Variable), Occurence Index and a Heat Map according to time of appearance.
  
-You can visualise a specific 2x2 Array, and see it as inserted on a 3D plot, where every position is determined through (i, j) (or z, x). 
  Axis contain information about Row index, Col index, Occurence Index (y axis) and a Heat Map according to time of appearance.
  
-You can visualise a 3x3 Array with i, j, k (z, x, y respectivly) on axis and a heat map on time of appearance. 
  On 3x3 array visualisation we dont take in consideration occurrence index.
>>>>>>> GUI completed deleted. A new TCL shell is now responsible to

-Complete analysis with addresses and occurrences on saved format.

## Libraries
You will need plotly, matplotlib, valgrind with gleipnir tool installed.
### Plotly
$ pip install plotly==4.3.0
### Matplotlib 
Depending on your Linux distrubution.

Debian / Ubuntu: sudo apt-get install python3-matplotlib

Fedora: sudo dnf install python3-matplotlib

Red Hat: sudo yum install python3-matplotlib
### Gleipnir
Install a new valgrind to have access

$ svn co svn://svn.valgrind.org/valgrind/trunk valgrind

Clone gleipnir

$ cd valgrind; git clone https://bitbucket.org/janjust/gleipnir-pub.git
rename the folder from gleipnir-pub to gleipnir.

(On current version as of 5/12/2019 you need to access glconf.sh and remove everything in comments)

$ ./glconf.sh -d

$ ./glconf.sh -m replace

Back on valgrind folder. Edit Makefile.am and add gleipnir on TOOLS.

Then on valgrind

$ ./autogen.sh

$ ./configure --prefix=<current valgrind dir>

$ sudo make

$ sudo make install

Gleipnir should be installed in valgrind. 

## Run
In order to run memory tracing through gleipnir you need to include in your source code gleipnir.h header file 
before compiling (#include "/path/to/valgrind/gleipnir/gleipnir.h") and GL_START; and GL_STOP; as pins to the part of the code you want
to analyse. For more information of how to run gleipnir visit this link https://csrl.cse.unt.edu/node/30

Compile mapvis tool

$ make all

run mapvis

$ ./mapvis

Be sure to check the <help> tcl command, to see the commands, how they work and a usual flow. Also there is a script as an example to get into the tool logic.

There are some tests on tests/ folder. BE SURE to change the library include (on .c files) to your path to gleipnir. There is a path to 
contributors machine. Once you change it, recompile the code.

## Bottlenecks
Avoid including system calls inside Gleipnir's Pins (GL_START, GL_STOP).

Also, gleipnir is a slow tool and creates extremely large files. For big programs you will need a strong machine and a lot 
of time. (mandelbrot.c on /tests/stack folder is a good example of a slow execution)

## Acknowledgements
Design and implementation of a tool for memory access
pattern visualization, http://ir.lib.uth.gr/bitstream/handle/11615/48305/16578.pdf?sequence=1

## Random Examples
![alt text](https://i.ibb.co/Y7Kjqyg/Visualization-of-complete-execution-of-matrix-mul-with-blocks.png "Complete Run Execution")
![alt text](https://i.ibb.co/sJ9fCL7/Visualization-of-Y-array-in-matrix-mul-with-blocks.png "2D Array Vis")
![alt text](https://i.ibb.co/pd0QTtc/3-DARRAYVIS.png "3D Array Vis")

