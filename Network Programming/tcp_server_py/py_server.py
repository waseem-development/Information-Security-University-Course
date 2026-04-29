import socket
import threading

HOST = '127.0.0.1'
PORT = 8080
BUF_SIZE = 4096

def handle_client(conn, addr):
    print(f"Connected: {addr}")

    with conn:
        while True:
            data = conn.recv(BUF_SIZE)

            if not data:
                break

            msg = data.decode('utf-8')
            print(f"Client: {msg}")

            if msg == "exit":
                break

            reply = input("Server: ")
            conn.sendall(reply.encode('utf-8'))

    print(f"Disconnected: {addr}")

def main():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind((HOST, PORT))
    server.listen()

    print("Server running...")

    while True:
        conn, addr = server.accept()

        t = threading.Thread(
            target=handle_client,
            args=(conn, addr),
            daemon=True
        )
        t.start()

main()