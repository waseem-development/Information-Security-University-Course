import socket as skt

SERVER_IP = '127.0.0.1'
PORT = 9090
BUFF_SIZE = 1024

SERVER = (SERVER_IP, PORT)

with skt.socket(skt.AF_INET, skt.SOCK_DGRAM) as s:

    print(f"Connected to {SERVER_IP}:{PORT}")

    while True:
        # Type message
        message = input("You: ")

        # Send to server
        s.sendto(message.encode("utf-8"), SERVER)

        if message.lower() == "exit":
            print("Client ended chat.")
            break

        # Wait for reply
        data, addr = s.recvfrom(BUFF_SIZE)

        reply = data.decode("utf-8")

        print(f"Server: {reply}")

        if reply.lower() == "exit":
            print("Server ended chat.")
            break