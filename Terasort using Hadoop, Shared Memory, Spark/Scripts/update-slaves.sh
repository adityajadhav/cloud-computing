# need to upload config to ~/.ssh
for number in 1 2 3 4 5 6 7
do
cat ~/.ssh/id_rsa.pub | ssh datanode$number 'cat >> ~/.ssh/authorized_keys'
done
exit 0