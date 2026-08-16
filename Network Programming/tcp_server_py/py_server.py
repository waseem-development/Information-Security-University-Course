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
    server = stk.socket(stk.AF_INET, stk.SOCK_STREAM) # AF_INET: IPv4, SOCK_STREAM: TCP
    server.setsockopt( # Configure this socket so that its address can be reused when appropriate.
        stk.SOL_SOCKET, # I wanna configure an option at the socket level
        stk.SO_REUSEADDR, # allow the socets local address to the resued when the operating syste would otherwise prevent the bind because of a recently used socket
        1)
    
    server.bind((HOST, PORT)) # This socket should use this IP address and port.

    server.listen() # Now start waiting for incoming TCP connection requests.

    print("Server running...")

    while True:
        conn, addr = server.accept() # where conn is a new socket used to communicate with that client
        # address is the client's address
        t = th.Thread(
            target=handle_client, # This new thread's job is to execute handle_client.
            args=(conn, addr),  # When the thread runs handle_client, pass it these two arguments.
            daemon=True # means this client-handling thread is a daemon/background thread.
            )
        t.start()

main()