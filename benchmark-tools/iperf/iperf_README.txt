Server:
	iperf3 -s

Client TCP:
	iperf3 -c <IP_ADDRESS> -w 64k -f Mb
	-w -> window size
	-f -> format to report: Kbits, Mbits, KBytes, MBytes
	-c -> server ip

Client UDP:
	iperf3 -u -c <IP_ADDRESS> -w 64k -f Mb -b
	-u -> UDP
	-w -> window size
	-f -> format to report: Kbits, Mbits, KBytes, MBytes
	-c -> server ip
	-b -> 0, will  disable  bandwidth  limits  (particularly  useful  for UDP tests)