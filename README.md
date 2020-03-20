# mapvisual
Software for analysis and visualization of Memory Access Pattern (MAP)

MAPvisual is a tool that creates and combines memory tracing and filtering proccesses to create data for the visualisation of memory access  patterns through a TCL shell. At the moment:

-You can visualise the complete execution of the executable, and see it on a 3D plot, containing information about Addresses, Array - Variables, Occurrence Indexes and a Heat Map according to time of appearance.

-You can visualise a specific 2D Array, and see it as inserted on a 3D plot, where every position is determined through (i, j). Axis contain information about Row index, Col index, Occurrence Index (y axis) and a Heat Map according to time of appearance.

-You can visualise a 3D Array with rows, cols and depth on axis and a heat map on time of appearance. 

-Complete analysis with addresses and occurrences on saved format.

-Run valgrind plugin tools and save information.

-User-friendly TCL Shell, which makes the tool easy to run.

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

$ cd valgrind; 

for mapvisual a new version of optimized gleipnir (specific for this application) will be needed.

$ git clone https://github.com/pavlosaim/memory-tracing.git

rename the folder to gleipnir. Use that repository to install gleipnir and valgrind. If not continue with the following.

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

There are some tests on tests/ folder. BE SURE to change the library included (on .c files) to your path to gleipnir.  

## Bottlenecks
Avoid including system calls inside Gleipnir's Pins (GL_START, GL_STOP).

Also, gleipnir is a slow tool and creates extremely large files. For big programs you will need a strong machine and a lot 
of time. (mandelbrot.c on /tests/stack folder is a good example of a slow execution)

## Acknowledgements
Design and implementation of a tool for memory access
pattern visualization, http://ir.lib.uth.gr/bitstream/handle/11615/48305/16578.pdf?sequence=1

## Random Examples Pictures 
![alt text](https://i.ibb.co/WWcJZ8C/3dblockedsample.png "3d blocked mm sample")
![alt text](https://i.ibb.co/rF51w7r/mandelbrotsample.png "mandelbrot part sample")
![alt text](https://i.ibb.co/n7sLjqx/matrixcolyarraysample.png "random column y")
![alt text](https://i.ibb.co/Y2bQ25C/perarraymapsample.png "per array sample")
![alt text](https://i.ibb.co/NWk0KYP/3dtangledsample.png "3d tangled sample")
![alt text](https://i.ibb.co/rMBbD44/3darrsample.png "3d array sample")
