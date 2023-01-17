import atexit
import socket
import time
from tkinter import *
from tkinter import messagebox
from tkinter.scrolledtext import ScrolledText

current_username = ""


def recv_data_only_one(client_socket):
    data = ""

    while True:
        one_letter = client_socket.recv(1).decode()
        if one_letter != "\n":
            data += one_letter
        else:
            return data


def init_window(r):
    r.geometry('1400x700')
    r.title("Client")


def login_ui(soc, r, server_message):
    login = StringVar()

    logging_label = Label(r, text='LOGIN')

    logging_box = Entry(r, textvariable=login)

    logging_label.place(x=0, y=0)
    logging_box.place(x=50, y=0)

    log_in_button = Button(r, text='CONFIRM', width=10, height=1, bg='#90ee90',
                           command=lambda: [logowanie(soc, login, server_message), get_user(login)])
    log_in_button.place(x=70, y=25)


def register_ui(soc, r, server_message):
    register_login = StringVar()

    register_label = Label(r, text='REGISTER')

    register_box = Entry(r, textvariable=register_login)

    register_label.place(x=300, y=0)
    register_box.place(x=370, y=0)

    register_button = Button(r, text='CONFIRM', width=10, height=1, bg='#90ee90',
                             command=lambda: [rejestracja(soc, register_login, server_message),
                                              get_user(register_login)])
    register_button.place(x=380, y=25)


def subscription_ui(soc, r, server_message):
    subscription_username = StringVar()

    subscription_label = Label(r, text='SUBSCRIBE')

    subscription_box = Entry(r, textvariable=subscription_username)

    subscription_label.place(x=700, y=0)
    subscription_box.place(x=800, y=0)

    subscription_button = Button(r, text='CONFIRM', width=10, height=1, bg='#90ee90',
                                 command=lambda: subskrybcja(soc, current_username, subscription_username,
                                                             server_message))
    subscription_button.place(x=800, y=25)


def show_subscriptions_ui(soc, r, server_message):
    subscription_button = Button(r, text='SHOW SUBSCRIPTIONS', width=20, height=1, bg='#90ee90',
                                 command=lambda: show_subscriptions(soc, current_username, server_message))
    subscription_button.place(x=1000, y=0)


def server_message_ui(r):
    server_message = ScrolledText(r, width=80, height=13)

    server_message_label = Label(r, text='SERVER MESSAGE')

    server_message.place(x=5, y=80)

    server_message_label.place(x=0, y=59)

    server_message.configure(state=DISABLED)

    return server_message


def message_box_ui(soc, r, server_message):
    message_label = Label(r, text='MESSAGE')

    message_box = ScrolledText(r, width=80, height=19)

    message_label.place(x=0, y=310)
    message_box.place(x=5, y=330)

    message_button = Button(r, text='SEND', width=10, height=1, bg='#90ee90',
                            command=lambda: dodaj_wpis(soc, current_username, message_box.get('1.0', 'end-1c'),
                                                       server_message))
    message_button.place(x=280, y=670)


def news_feed_ui(soc, r):
    news_feed = ScrolledText(r, width=70, height=34)

    news_feed_abel = Label(r, text='NEWS FEED')

    news_feed.place(x=700, y=80)

    news_feed_abel.place(x=700, y=59)

    news_feed.configure(state=DISABLED)

    news_feed_button = Button(r, text='UPDATE', width=10, height=1, bg='#90ee90',
                              command=lambda: pokaz_wpisy(soc, current_username, news_feed))
    news_feed_button.place(x=950, y=670)


def logowanie(soc, uzytkownik, server_message):
    soc.send('1\n'.encode())
    uzytkownik = uzytkownik.get() + '\n'
    soc.send(uzytkownik.encode())
    resp2 = recv_data_only_one(soc)
    messagebox.showinfo("SERVER MESSAGE", resp2)
    server_message.configure(state=NORMAL)
    server_message.delete('1.0', END)
    server_message.configure(state=DISABLED)
    if resp2 == b'nie zalogowano\n':
        quit()


def rejestracja(soc, login, server_message):
    soc.send('2\n'.encode())
    login = login.get() + '\n'
    soc.send(login.encode())
    response = recv_data_only_one(soc)
    messagebox.showinfo("SERVER MESSAGE", response)


def subskrybcja(soc, u, sub, server_message):
    soc.send('subskrybuj\n'.encode())
    time.sleep(1)

    u = u + '\n'
    soc.send(u.encode())

    sub = sub.get() + '\n'
    soc.send(sub.encode())
    if_add = recv_data_only_one(soc)
    if if_add == b"1":
        server_message.configure(state=NORMAL)
        server_message.insert(INSERT, "dodano pomyslnie\n")
        server_message.configure(state=DISABLED)
    else:
        server_message.configure(state=NORMAL)
        server_message.insert(INSERT, "nie dodano\n")
        server_message.configure(state=DISABLED)


def show_subscriptions(soc, u, server_message):
    soc.send('subs\n'.encode())
    time.sleep(1)

    u = u + '\n'
    soc.send(u.encode())
    response = recv_data_only_one(soc)
    server_message.configure(state=NORMAL)
    server_message.insert(INSERT, response)
    server_message.configure(state=DISABLED)


def dodaj_wpis(soc, u, wpis, server_message):
    soc.send('dodaj\n'.encode())
    time.sleep(1)

    u = u + '\n'
    soc.send(u.encode())
    time.sleep(1)

    wpis = "-" + wpis + "\n"

    soc.send(wpis.encode())


def pokaz_wpisy(soc, u, news_feed):
    soc.send('wpisy\n'.encode())
    time.sleep(1)

    u = u + '\n'
    soc.send(u.encode())
    response = recv_data_only_one(soc)
    news_feed.configure(state=NORMAL)
    news_feed.delete('1.0', END)
    news_feed.insert(INSERT, response)
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
    resp = recv_data_only_one(s)

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
