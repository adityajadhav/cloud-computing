
cd ~/hadoop-2.9.0/bin
./hdfs namenode -format

cd ~/hadoop-2.9.0/sbin/
./start-dfs.sh 
./start-yarn.sh

cd /raid
wget http://www.ordinal.com/try.cgi/gensort-linux-1.5.tar.gz
tar -xzvf gensort-linux-1.5.tar.gz
cd 64
./gensort -a 100000000 dataset

cd ~/hadoop-2.9.0
bin/hdfs dfs -mkdir -p /ajadhav4/input
bin/hdfs dfs -put /raid/64/dataset /ajadhav4/input
rm /raid/64/dataset

bin/hadoop jar terasort-hadoop-0.0.1-SNAPSHOT-jar-with-dependencies.jar /ajadhav4/input /ajadhav4/output

bin/hdfs dfs -get /ajadhav4/output /raid/output

cd /raid/64
./valsort /raid/output