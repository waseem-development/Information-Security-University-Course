import socket as stk
import threading
import time

HOST = '127.0.0.1'
PORT = 8080
BUF_SIZE = 4096
def receive_message(sock):
    while True:
        try:
            data = sock.recv(BUF_SIZE)
            if not data:
                break
            print("\nServer:", data.decode('utf-8'))
        except:
            break

def send_message(sock):
    while True:
        message = input("You: ")
        sock.sendall(message.encode('utf-8'))

        if message == "exit":
            break

# Step 1: Create Socket
s = stk.socket(stk.AF_INET, stk.SOCK_STREAM)

# Step 2: Connect to server by performing 3 way handshake
s.connect((HOST, PORT))

print("Connected!")

# Step 3: Recieve Message
threading.Thread(target=receive_message, args=(s,), daemon=True).start()

# Step 4: Send Message
threading.Thread(target=send_message, args=(s,), daemon=True).start()
while True:
    time.sleep(1)