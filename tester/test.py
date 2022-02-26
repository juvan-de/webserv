import sys

def	main():
	ports = ""
	try:
		f = open(sys.argv[1], "r")
		# print(f.read())
		for i in f:
			if "listen" in i:
				ports += i
	except:
		print("Invalid argument")
	print(ports.replace("listen", ""))

main()