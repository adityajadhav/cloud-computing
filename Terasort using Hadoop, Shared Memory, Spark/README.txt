==================================================================
TERASORT IMPLEMENTATION FOR SHARED MEMORY, HADOOP AND SPARK
==================================================================

This file illustrates the folder contents and the commands need to be executed for each configuration.

The assignment folder contains the following folder:

1. Configuration files: It contains all the configuration files for Hadoop.
2. Peformance_Comparison excel which contains all the graphs and tables for comaparison.
3. prog2_report: It is the project report which contains all the prerequisites mentioned in the assignment.
4. Screenshots: It contains separate screenshots for all the three configurations.
5. Scripts_Virtual_Cluster_Setup: It contains all the scripts required to set up virtual cluster on amazon aws.
6. The Source_Code folder contains the following:

==================================================================
A. Shared-Memory TeraSort
==================================================================

===============================
Explanation of code:
===============================

The code is implemented in such a way that the files are being split initially in to smaller files that can fit in to memory and sort is performed on the smaller splits using multiple threads. We used ThreadPool Executor to implement multithreading in java. Secondly, the smaller files are being merged concurrently by multiple
threads. 

===============================
Folder Contents:
===============================

1. Executable/terasort-shared-memory-0.0.1-SNAPSHOT.jar - Executable jar to execute the program.
2. target/config.properties - configuration file which contains the source of the dataset and destination folder. 
3. pom.xml - It contains the dependencies of the project
4. Src/main/java/com/ajadhav4/cloud/terasortsharedmemory - This contains the source code of the program and the following files:
	
	SharedMemorySortMain.java: 
	-------------------------
	This is the main class of the program and accepts the command line arguments from user.
	
	SpliterTask.java:
	---------------
	This is the part of the program which splits the large files in to smaller splits and 			sort them concurrently using multithreading and implements the Runnable interface.

	MergeTask.java:
	---------------
	This files merge the sorted splits using the merge sort methodology and this is also being done 	concurrently using multiple threads and implements the Runnable interface.
	
	Reader.java:
	-----------
	This is used to read the configuration properties in to the above files.

5. The Execution Time is printed on the console.

===============================
Compile Shared-Memory TeraSort:
===============================

1. mvn package.

===============================
Run Shared-Memory TeraSort:
===============================

1. Copy the jar file from the target folder and create appropriate folders and configurations in the same folder as the jar.

2. Connect to the Amazon EC2 instance and copy the above contents to that location.
   Ssh -I private_key ec2_user@Instanceaddress

3. Generate the data using gensort of required memory (128GB, 1TB)
   ./gensort -a <size> <src_file_name>

5. Configuration settings in target/config.properties file can be tuned as follows:
   - Number of threads in the Thread pool executor.
   - Number of lines per split.
   - Input file location.
   - Destination file location. 

6. Remove contents of destination if already existed.
   cd destination && rm -fr *

7. Run the jar using following command
   java -jar terasort-shared-memory-0.0.1-SNAPSHOT.ja

9. The Execution time will be printed to the console in milliseconds.

10. Validate the sorted file to check if its sorted.
    ./valsort <output_file_name>

==================================================================
B. Hadoop MapReduce TeraSort
==================================================================

===============================
Explanation of code:
===============================

This is implemented using Hadoop MapReduce in two parts. First is the mapper which takes the each line and split that in to key and value. These intermediate key value pairs get sorted by key by the MapReduce framework automatically and sends the pair further to the reducer. This phase of transferring the mapper output to the reducer is called as shuffling and it can take place before the completion of map phase and results in a better execution time.
Second is the reducer which takes the key value from the mapper and output a key value pair in return. 

===============================
Folder Contents:
===============================
It contains two folders executable and source code

Executable contains the compiled jar for easy execution.
Source code contains the following:

Src/main/java/com/ajadhav4/cloud/tersort_hadoop has :
1. TeraSortApplication.java- This is the main file of the program.
2. TeraSortMapper.java- This is the mapper file of the which takes the each line and split that in to key and value.
3. TeraSortReducer.java- This is the reducer file that takes the input from mapper and output a key value in return.
4. pom.xml- It handles the dependency management.

===============================
Run Hadoop TeraSort:
===============================
This is also mentioned in hadooprun.sh script.
1. Create HDFS

cd ~/hadoop-2.9.0/bin
./hdfs namenode -format

2. Start dis and yarn

cd ~/hadoop-2.9.0/sbin/
./start-dfs.sh 
./start-yarn.sh

3. Generate the data

cd /raid
wget http://www.ordinal.com/try.cgi/gensort-linux-1.5.tar.gz
tar -xzvf gensort-linux-1.5.tar.gz
cd 64
./gensort -a 100000000 dataset

4. Put the dataset to RAID

cd ~/hadoop-2.9.0
bin/hdfs dfs -mkdir -p /ajadhav4/input
bin/hdfs dfs -put /raid/64/dataset /ajadhav4/input
rm /raid/64/dataset

5. Run the jar

bin/hadoop jar terasort-hadoop-0.0.1-SNAPSHOT-jar-with-dependencies.jar /ajadhav4/input /ajadhav4/output

6. Take the output

bin/hdfs dfs -get /ajadhav4/output /raid/output

7. Validate the output to check if it is sorted
cd /raid/64
./valsort /raid/output

==================================================================
C. Spark TeraSort
==================================================================

===============================
Explanation of code:
===============================

This implementation is performed using Scala. As Scala does not have its own file system and relies on Hadoop HDFS. So, it takes the input from the hdfs.  

===============================
Folder Contents:
===============================
This contains a file called TeraSort.scala which contains the main code of the program.

===============================
Run Spark TeraSort:
===============================

1. Follow the exact steps given in sparkrun.sh to run a Spark Program on amazon EC2 insatnces.


