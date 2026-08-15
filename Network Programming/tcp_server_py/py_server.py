import socket as stk
import threading as th

HOST = '127.0.0.1'
PORT = 8080
BUF_SIZE = 4096

def handle_client(conn, addr):
    print(f"Connected: {addr}")

    def receive():
        while True:
            try:
                data = conn.recv(BUF_SIZE)
                if not data:
                    break
                msg = data.decode('utf-8')
                print(f"\nClient: {msg}")
                if msg == "exit":
                    break
            except:
                break
        print(f"Disconnected: {addr}")

    def send():
        while True:
            try:
                reply = input("Server: ")
                conn.sendall(reply.encode('utf-8'))
                if reply == "exit":
                    break
            except:
                break

    th.Thread(target=receive, daemon=True).start()
    th.Thread(target=send, daemon=True).start()

def main():
    server = stk.socket(stk.AF_INET, stk.SOCK_STREAM)
    server.setsockopt(
        stk.SOL_SOCKET, 
        stk.SO_REUSEADDR, 
        1)
    server.bind((HOST, PORT))
    server.listen()
    print("Server running...")
    while True:
        conn, addr = server.accept()
        t = th.Thread(target=handle_client, args=(conn, addr), daemon=True)
        t.start()

main()