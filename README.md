# mapvisual
Software for analysis and visualization of Memory Access Pattern (MAP)

MAPvisual is a tool that creates and combines memory tracing and filtering proccesses to create data for the visualisation of memory access 
patterns through a user friendly GUI. At the moment:

-You can visualise the complete execution of the executable, and see it on a 3D plot, containing information about Addresses, Array (or Variable), Occurrence Index and a Heat Map according to time of appearance.

-You can visualise a specific 2x2 Array, and see it as inserted on a 3D plot, where every position is determined through (i, j) (or z, x). Axis contain information about Row index, Col index, Occurrence Index (y axis) and a Heat Map according to time of appearance.

-You can visualise a 3x3 Array with i, j, k (z, x, y respectively) on axis and a heat map on time of appearance. On 3x3 array visualisation we do not take in consideration occurrence index.

-Memory Activity Graph of operations (Store, Load and Modify) on a 2D plot, Y-axis contains Variables and X-axis Time.

-Complete analysis with addresses and occurrences on GUI.

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

(On current version as of 5/12/2019 you need to access glconf.sh and remove everything in comments)

$ ./glconf -d

$ ./glconf -m replace

Back on valgrind folder. Edit Makefile.am and add gleipnir on TOOLS.

Then on valgrind

$ ./autogen.sh

$ ./configure --prefix=<current valgrind dir>

$ sudo make

$ sudo make install

Gleipnir should be installed in valgrind. Once you installed it, you should modify the file "valgrind_path" on folder "paths" of mapvisual src code, and set it to the installed valgrind path which contains gleipnir tool. 

## Run
In order to run memory tracing through gleipnir you need to include in your source code gleipnir.h header file 
before compiling (#include "/path/to/valgrind/gleipnir/gleipnir.h") and GL_START; and GL_STOP; as pins to the part of the code you want
to analyse. For more information of how to run gleipnir visit this link https://csrl.cse.unt.edu/node/30

Compile your source code. (map_analysis.py requires executable)

Once everything is done.

$ python map_analysis.py

Open your executable, choose Stack or Heap analysis (IMPORTANT!!! If you dont select anything behaviour is unexpectable, even if it seems 
to be checked, overwrite the check!). You will be asked for arguments and source path (try to type it as close as possible). Once 
analysis is done you can choose visualization. For some visualizations you will be asked to know some information, like the name of the 
array you want to visualize or each size. Its important for the developer to know this stuff.

There are some tests on tests/ folder. BE SURE to change the library include (on .c files) to your path to gleipnir. There is a path to 
contributors machine. Once you change it, recompile the code.

## Bottlenecks
Possible errors occuring on gleipnir cannot be avoided. One common error is "The impossible happened". Those errors are not from the
current tool but from gleipnir tool and there is nothing we can do to change them. Some hints are, do not include system calls (avoid fclose)
, printfs.

Also, gleipnir is a extremly slow and creates extremely big output files. For big programs you will need a strong machine and a lot 
of time. (You can check how slow it is on mandelbrot.c on /tests/stack folder)

## Acknowledgements
Design and implementation of a tool for memory access
pattern visualization, http://ir.lib.uth.gr/bitstream/handle/11615/48305/16578.pdf?sequence=1

## Random Examples
![alt text](https://i.ibb.co/Y7Kjqyg/Visualization-of-complete-execution-of-matrix-mul-with-blocks.png "Complete Run Execution")
![alt text](https://i.ibb.co/sJ9fCL7/Visualization-of-Y-array-in-matrix-mul-with-blocks.png "2D Array Vis")
![alt text](https://i.ibb.co/pd0QTtc/3-DARRAYVIS.png "3D Array Vis")

