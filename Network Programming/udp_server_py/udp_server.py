import socket as skt

HOST = '0.0.0.0'
PORT = 9090
BUFF_SIZE = 1024

with skt.socket(skt.AF_INET, skt.SOCK_DGRAM) as s:
    s.setsockopt(skt.SOL_SOCKET, skt.SO_REUSEADDR, 1)
    s.bind((HOST, PORT))

    print(f"UDP Server Listening on {HOST}:{PORT}")

    while True:
        # Receive message from client
        data, addr = s.recvfrom(BUFF_SIZE)

        if data:
            msg = data.decode("utf-8")

            print(f"\nClient {addr[0]}:{addr[1]} says: {msg}")

            if msg.lower() == "exit":
                print("Client ended chat.")
                break

            # Type reply
            reply = input("You: ")

            # Send reply back to same client
            s.sendto(reply.encode("utf-8"), addr)

            if reply.lower() == "exit":
                print("Server ended chat.")
                break