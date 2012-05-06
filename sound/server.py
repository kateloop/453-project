import socket


UDP_IP="128.104.180.233"
UDP_PORT=1514

sock  = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

while True:
	data, addr = sock.recvfrom(1024) # buffer size os 1024 bytes
	print "Message Received: ", data

