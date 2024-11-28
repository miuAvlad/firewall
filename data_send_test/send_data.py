import socket
from dotenv import load_dotenv
import os
load_dotenv()

def send_data():
    server_address = os.getenv("SRVER_ADDRESS")
    port = 1234

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        client_socket.connect((server_address,port))
        print(f"Conectare la {server_address} : {port}")

        message = "Salut, message test!"
        client_socket.sendall(message.encode("utf-8"))
        print(f"Trimis : {message}")
    except Exception as e:
        print(f"Eroare {e}")
    finally:
        client_socket.close()
        print("Deconectare socket")

if __name__ == "__main__":
    send_data()
