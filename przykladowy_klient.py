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
        

def rejestracja(s):
	resp=s.recv(1024)
	print(resp)
	login=raw_input()
	s.send(login)
	resp=s.recv(1024)
	print(resp)
	return login

def subskrypcja(s,u):
	s.send("subskrybuj\n")
	time.sleep(1)
	
	s.send(u)
	#wiad=s.recv(100)
	print("prosze dodac")
	sub=raw_input()
	s.send(sub)
	if_add=s.recv(10)
	print(if_add)
	if if_add=="1":
		print("dodano pomyslnie")
	else:
		print("nie dodano")
	return 0

def dodaj_wpis(s,u):
	s.send("dodaj\n")
	time.sleep(1)

	s.send(u)
	wpis=s.recv(20)
	print(wpis)
	print("podaj wpis")
	wpis=raw_input()
	time.sleep(1)
	wpis="-"+wpis+"\n"
	s.send(wpis)

def pokaz_wpisy(s,u):
	s.send("wpisy\n")
	time.sleep(1)
	s.send(u)
	resp=s.recv(1024)
	print(resp)

if __name__ == '__main__':
	proto = socket.getprotobyname('tcp')                             # [1]
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, proto)
	s.connect(("127.0.0.1", 1234))
	resp = s.recv(1024)       
	print(resp)
	wybor=raw_input()
	wybor=wybor+"\n"
	
	while wybor!='1\n' and wybor!='2\n':
		print("blad")
		wybor=raw_input()
		wybor+"\n"
	else:
		s.send(wybor)
	if wybor=="1\n":
		uzytkownik_zalogowany=logowanie(s)
	elif wybor=="2\n":
		uzytkownik_zalogowany=rejestracja(s)

	while(1):
		print("wybierz polecenie")
		print("1.dodaj subskrybcje")
		print("2.dodaj wpis")
		print("3.pokaz wpisy")
		print("4.koniec")
		wybor=raw_input()
		if wybor=="1":
			subskrypcja(s,uzytkownik_zalogowany)
		elif wybor=="2":
			dodaj_wpis(s,uzytkownik_zalogowany)
		elif wybor=="3":
			pokaz_wpisy(s,uzytkownik_zalogowany)
		elif wybor=="4":
			s.send("koniec\n")
			break
