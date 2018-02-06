sudo yum update
sudo yum install java-1.8.0

ssh-keygen -f ~/.ssh/id_rsa -t rsa -P ""
cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys

cd ~
wget http://mirrors.koehn.com/apache/hadoop/common/hadoop-2.9.0/hadoop-2.9.0.tar.gz
tar -zxvf hadoop-2.9.0.tar.gz
chmod 777 hadoop-2.9.0
exit 0