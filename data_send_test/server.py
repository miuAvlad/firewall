import socket
import signal
import threading
from dotenv import load_dotenv
import os

stop_server=False
load_dotenv()
server_address = os.getenv("SRVER_ADDRESS")


def signal_handler(signum,frame):
    global stop_server
    print("Semnal primit, serverul se va opri...")
    stop_server = True

def start_server():
    global stop_server
    server_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    server_socket.bind((server_address,1234))
    server_socket.listen(1)
    server_socket.settimeout(1) 
    print("Serverul asculta pe portul 1234 ...")

    while not stop_server:
        try:
            client_socket,client_address = server_socket.accept()
            print(f"Conexiune primita de la clientul {client_address}!")
            data = client_socket.recv(1024).decode("utf-8")
            print(f"Mesaj primit: {data}")
            client_socket.close()
        except socket.timeout:
            continue
    
    server_socket.close()

if __name__ == "__main__":
    
    signal.signal(signal.SIGINT, signal_handler)
    server_thread = threading.Thread(target = start_server)
    server_thread.start()

    print("Apasa x pentru a  opri serverul")

    while not stop_server:
        key=input()
        if key.lower() == 'x':
            signal_handler(None,None)