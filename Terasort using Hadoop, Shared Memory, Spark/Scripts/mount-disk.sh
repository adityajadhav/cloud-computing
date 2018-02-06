
sudo mkfs.ext4 /dev/nvme0n1
sudo mke2fs -F  -t  ext4 /dev/nvme0n1

sudo mkfs.ext4 /dev/nvme1n1
sudo mke2fs -F  -t  ext4 /dev/nvme1n1

#Referance- http://docs.aws.amazon.com/AWSEC2/latest/UserGuide/raid-config.html

sudo mdadm --create --verbose /dev/md0 --level=0 --name=MY_RAID --raid-devices=2 /dev/nvme0n1 /dev/nvme1n1
sudo mkfs.ext4 -L MY_RAID /dev/md0
sudo mkdir -p /raid
sudo mount LABEL=MY_RAID /raid


