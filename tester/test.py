import sys # open
import threading # multi threading
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
	print("started new thread")
	link = "http://localhost:" + str(threading.current_thread().name) + "/"
	# print(link)
	try:
		r = requests.get(link, timeout=1)
		print("------------------------------\n" + r + "------------------------------\n")
	except:
		print("Bad return")
	return

def	main():
	ports = []
	try:
		ports = get_ports()
	except:
		print("Invalid argument")
	print(ports)
	for port in ports:
		try:
			threading.Thread(target=make_request, name=port).start()
		except:
			print("Couldnt make thread")

main()