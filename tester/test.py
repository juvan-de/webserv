import sys
import requests # need the requests package for this one bois

def get_ports():
	ports = []
	f = open(sys.argv[1], "r")
	for line in f:
		if "listen" in line:
			for word in line.split():
				if not "listen" in word and not "localhost" in word:
					ports.append(int(word))
	return list(dict.fromkeys(ports))

def make_request():
	r = requests.get("http://localhost:8080/")

def	main():
	ports = []
	try:
		ports = get_ports()
	except:
		print("Invalid argument")
	print(ports)
	make_request()

main()