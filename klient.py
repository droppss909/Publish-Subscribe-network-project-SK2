import atexit
import socket
import time
from tkinter import *
from tkinter.scrolledtext import ScrolledText

current_username = ""


def init_window(r):
    r.geometry('1400x700')
    r.title("Client")


def login_ui(s, r, sm):
    login = StringVar()

    logging_label = Label(r, text='LOGIN')

    logging_box = Entry(r, textvariable=login)

    logging_label.place(x=0, y=0)
    logging_box.place(x=50, y=0)

    log_in_button = Button(r, text='CONFIRM', width=10, height=1, bg='#90ee90',
                           command=lambda: [logowanie(s, login, sm), get_user(login)])
    log_in_button.place(x=70, y=25)


def register_ui(s, r, sm):
    register_login = StringVar()

    register_label = Label(r, text='REGISTER')

    register_box = Entry(r, textvariable=register_login)

    register_label.place(x=300, y=0)
    register_box.place(x=370, y=0)

    register_button = Button(r, text='CONFIRM', width=10, height=1, bg='#90ee90',
                             command=lambda: [rejestracja(s, register_login, sm), get_user(register_login)])
    register_button.place(x=380, y=25)


def subscription_ui(s, r, sm):
    subscription_username = StringVar()

    subscription_label = Label(r, text='SUBSCRIBE')

    subscription_box = Entry(r, textvariable=subscription_username)

    subscription_label.place(x=700, y=0)
    subscription_box.place(x=800, y=0)

    subscription_button = Button(r, text='CONFIRM', width=10, height=1, bg='#90ee90',
                                 command=lambda: subskrybcja(s, current_username, subscription_username, sm))
    subscription_button.place(x=800, y=25)


def show_subscriptions_ui(s, r, sm):
    subscription_button = Button(r, text='SHOW SUBSCRIPTIONS', width=20, height=1, bg='#90ee90',
                                 command=lambda: show_subscriptions(s, current_username, sm))
    subscription_button.place(x=1000, y=0)


def server_message_ui(r):
    server_message = ScrolledText(r, width=80, height=13)

    server_message_label = Label(r, text='SERVER MESSAGE').place(x=0, y=59)

    server_message.place(x=5, y=80)

    server_message.configure(state=DISABLED)

    return server_message


def message_box_ui(s, r, sm):
    message_label = Label(r, text='MESSAGE')

    message_box = ScrolledText(r, width=80, height=19)

    message_label.place(x=0, y=310)
    message_box.place(x=5, y=330)

    message_button = Button(r, text='SEND', width=10, height=1, bg='#90ee90',
                            command=lambda: dodaj_wpis(s, current_username, message_box.get('1.0', 'end-1c'), sm))
    message_button.place(x=280, y=670)


def news_feed_ui(s, r):
    news_feed = ScrolledText(r, width=70, height=34)

    news_feed_abel = Label(r, text='NEWS FEED').place(x=700, y=59)

    news_feed.place(x=700, y=80)

    news_feed.configure(state=DISABLED)

    news_feed_button = Button(r, text='UPDATE', width=10, height=1, bg='#90ee90',
                              command=lambda: pokaz_wpisy(s, current_username, news_feed))
    news_feed_button.place(x=950, y=670)


def logowanie(s, uzytkownik, server_message):
    s.send('1\n'.encode())
    s.send(uzytkownik.get().encode())
    resp2 = s.recv(100)
    server_message.configure(state=NORMAL)
    server_message.insert(INSERT, resp2)
    server_message.configure(state=DISABLED)
    if resp2 == b'nie zalogowano\n':
        quit()
    return uzytkownik  # [5


def rejestracja(s, login, server_message):
    s.send('2\n'.encode())
    resp = s.recv(1024)
    server_message.configure(state=NORMAL)
    server_message.insert(INSERT, resp)
    server_message.configure(state=DISABLED)
    s.send(login.get().encode())
    resp = s.recv(1024)
    server_message.configure(state=NORMAL)
    server_message.insert(INSERT, resp)
    server_message.configure(state=DISABLED)
    return login


def subskrybcja(s, u, sub, server_message):
    s.send('subskrybuj\n'.encode())
    time.sleep(1)

    s.send(u.encode())
    # wiad=s.recv(100)
    s.send(sub.get().encode())
    if_add = s.recv(10)
    server_message.configure(state=NORMAL)
    server_message.insert(INSERT, if_add)
    server_message.configure(state=DISABLED)
    if if_add == b"1":
        server_message.configure(state=NORMAL)
        server_message.insert(INSERT, """\
        dodano pomyslnie
        """)
        server_message.configure(state=DISABLED)
    else:
        server_message.configure(state=NORMAL)
        server_message.insert(INSERT, """\
        nie dodano
        """)
        server_message.configure(state=DISABLED)


def show_subscriptions(s, u, server_message):
    s.send('subs\n'.encode())
    time.sleep(1)

    s.send(u.encode())
    resp = s.recv(1024)
    server_message.configure(state=NORMAL)
    server_message.insert(INSERT, resp)
    server_message.configure(state=DISABLED)


def dodaj_wpis(s, u, wpis, server_message):
    s.send('dodaj\n'.encode())
    time.sleep(1)

    s.send(u.encode())
    resp = s.recv(20)
    server_message.configure(state=NORMAL)
    server_message.insert(INSERT, resp)
    server_message.configure(state=DISABLED)
    time.sleep(1)

    wpis = "-" + wpis + "\n"

    s.send(wpis.encode())


def pokaz_wpisy(s, u, news_feed):
    s.send('wpisy\n'.encode())
    time.sleep(1)
    s.send(u.encode())
    resp = s.recv(1024)
    news_feed.configure(state=NORMAL)
    news_feed.delete('1.0', END)
    news_feed.insert(INSERT, resp)
    news_feed.configure(state=DISABLED)


def get_user(username):
    global current_username
    current_username = username.get()


def exit_handler(soc):
    soc.send("koniec\n".encode())


if __name__ == '__main__':
    proto = socket.getprotobyname('tcp')  # [1]
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, proto)
    s.connect(("127.0.0.1", 1234))
    resp = s.recv(1024)

    root = Tk()

    init_window(root)

    sm = server_message_ui(root)

    login_ui(s, root, sm)

    register_ui(s, root, sm)

    subscription_ui(s, root, sm)

    show_subscriptions_ui(s, root, sm)

    message_box_ui(s, root, sm)

    news_feed_ui(s, root)

    sm.configure(state=NORMAL)
    sm.insert(INSERT, resp)
    sm.configure(state=DISABLED)

    atexit.register(exit_handler, s)

    root.mainloop()
