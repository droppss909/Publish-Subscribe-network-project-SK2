import socket
import string
import time
def logowanie(s):                                
	uzytkownik=raw_input()
	s.send(uzytkownik)  
	resp2=s.recv(100)
	print(resp2)     
	if resp2=="nie zalogowano\n":
		exit();    
	return uzytkownik;                           # [5
        

def subskrypcja(s,u):
	s.send("subskrybuj")
	time.sleep(1)
	s.send(u)
	sub=raw_input()
	s.send(sub)
	if_add=s.recv(10)
	print(if_add)
	if if_add=="1":
		print("dodano pomyslnie")
	else:
		print("nie dodano")
	return 0

if __name__ == '__main__':
	proto = socket.getprotobyname('tcp')                             # [1]
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, proto)
	s.connect(("127.0.0.1", 1234))
	resp = s.recv(1024)       
	print(resp)
	uzytkownik_zalogowany=logowanie(s)
	subskrypcja(s,uzytkownik_zalogowany)
