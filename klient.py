import socket
import time
from tkinter import *
from tkinter.scrolledtext import ScrolledText

current_username = ""


def logowanie(s, uzytkownik):
    s.send('1'.encode())
    print('tu33')
    s.send(uzytkownik.get().encode())
    print(uzytkownik.get().encode())
    resp2 = s.recv(100)
    print('tu55')
    serverMessage.configure(state=NORMAL)
    serverMessage.insert(INSERT, resp2)
    serverMessage.configure(state=DISABLED)
    if resp2 == "nie zalogowano\n":
        exit();
    return uzytkownik;  # [5


def rejestracja(s, login):
    s.send('2'.encode())
    resp = s.recv(1024)
    serverMessage.configure(state=NORMAL)
    serverMessage.insert(INSERT, resp)
    serverMessage.configure(state=DISABLED)
    s.send(login.get().encode())
    resp = s.recv(1024)
    serverMessage.configure(state=NORMAL)
    serverMessage.insert(INSERT, resp)
    serverMessage.configure(state=DISABLED)
    return login


def subskrypcja(s, u, sub):
    s.send('subskrybuj\n'.encode())
    time.sleep(1)

    s.send(u.encode())
    # wiad=s.recv(100)
    print("prosze dodac")
    s.send(sub.get().encode())
    if_add = s.recv(10)
    print(if_add)
    if if_add == "1":
        serverMessage.configure(state=NORMAL)
        serverMessage.insert(INSERT, """\
        dodano pomyslnie
        """)
        serverMessage.configure(state=DISABLED)
    else:
        serverMessage.configure(state=NORMAL)
        serverMessage.insert(INSERT, """\
        nie dodano
        """)
        serverMessage.configure(state=DISABLED)
    return 0


def dodaj_wpis(s, u, wpis):
    s.send('dodaj\n'.encode())
    time.sleep(1)

    s.send(u.encode())
    resp = s.recv(20)
    serverMessage.configure(state=NORMAL)
    serverMessage.insert(INSERT, resp)
    serverMessage.configure(state=DISABLED)
    time.sleep(1)
    wpis = "-" + wpis.get() + "\n"
    s.send(wpis.encode())


def pokaz_wpisy(s, u):
    s.send('wpisy\n'.encode())
    time.sleep(1)
    s.send(u)
    resp = s.recv(1024)
    newsFeed.configure(state=NORMAL)
    newsFeed.insert(INSERT, resp)
    newsFeed.configure(state=DISABLED)


def get_user(username):
    global current_username
    current_username = username


if __name__ == '__main__':
    proto = socket.getprotobyname('tcp')  # [1]
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, proto)
    s.connect(("127.0.0.1", 1234))
    resp = s.recv(1024)

    root = Tk()
    root.geometry('1400x700')
    root.title("Client")

    login = StringVar()

    loggingLabel = Label(root, text='LOGIN')

    loggingBox = Entry(root, textvariable=login)

    loggingLabel.place(x=0, y=0)
    loggingBox.place(x=50, y=0)

    logInButton = Button(root, text='CONFIRM', width=10, height=1, bg='#90ee90',
                         command=lambda: [logowanie(s, login), get_user(login)])
    logInButton.place(x=70, y=25)

    registerLogin = StringVar()

    registerLabel = Label(root, text='REGISTER')

    registerBox = Entry(root, textvariable=registerLogin)

    registerLabel.place(x=300, y=0)
    registerBox.place(x=370, y=0)

    registerButton = Button(root, text='CONFIRM', width=10, height=1, bg='#90ee90',
                            command=lambda: [rejestracja(s, registerLogin), get_user(registerLogin)])
    registerButton.place(x=380, y=25)

    subscriptionUsername = StringVar()

    subscriptionLabel = Label(root, text='SUBSCRIBE')

    subscriptionBox = Entry(root, textvariable=subscriptionUsername)

    subscriptionLabel.place(x=700, y=0)
    subscriptionBox.place(x=800, y=0)

    subscriptionButton = Button(root, text='CONFIRM', width=10, height=1, bg='#90ee90',
                                command=lambda: subskrypcja(s, current_username, subscriptionUsername))
    subscriptionButton.place(x=800, y=25)

    serverMessage = ScrolledText(root, width=80, height=13)

    serverMessageLabel = Label(root, text='SERVER MESSAGE').place(x=0, y=59)

    serverMessage.place(x=5, y=80)

    serverMessage.configure(state=DISABLED)

    message = StringVar()

    messageLabel = Label(root, text='MESSAGE')

    messageBox = ScrolledText(root, width=80, height=19)

    messageLabel.place(x=0, y=310)
    messageBox.place(x=5, y=330)

    messageBox.get(1.0, END)

    messageButton = Button(root, text='SEND', width=10, height=1, bg='#90ee90',
                           command=lambda: dodaj_wpis(s, current_username, message))
    messageButton.place(x=280, y=670)

    newsFeed = ScrolledText(root, width=70, height=34)

    newsFeedLabel = Label(root, text='NEWS FEED').place(x=700, y=59)

    newsFeed.place(x=700, y=80)

    newsFeed.configure(state=DISABLED)

    newsFeedButton = Button(root, text='UPDATE', width=10, height=1, bg='#90ee90',
                            command=lambda: pokaz_wpisy(s, current_username))
    newsFeedButton.place(x=950, y=670)

    serverMessage.configure(state=NORMAL)
    serverMessage.insert(INSERT, resp)
    serverMessage.configure(state=DISABLED)

    '''
    print(resp)
    wybor = input()
    wybor = wybor + "\n"

    while wybor != '1\n' and wybor != '2\n':
        print("blad")
        wybor = input()
        wybor + "\n"
    else:
        s.send(wybor)
    if wybor == "1\n":
        uzytkownik_zalogowany = logowanie(s)
    elif wybor == "2\n":
        uzytkownik_zalogowany = rejestracja(s)

    while (1):
        print("wybierz polecenie")
        print("1.dodaj subskrybcje")
        print("2.dodaj wpis")
        print("3.pokaz wpisy")
        print("4.koniec")
        wybor = input()
        if wybor == "1":
            subskrypcja(s, uzytkownik_zalogowany)
        elif wybor == "2":
            dodaj_wpis(s, uzytkownik_zalogowany)
        elif wybor == "3":
            pokaz_wpisy(s, uzytkownik_zalogowany)
        elif wybor == "4":
            s.send("koniec\n")
            break
            '''

    root.mainloop()
