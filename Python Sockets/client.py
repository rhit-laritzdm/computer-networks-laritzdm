import socket
import sys

SIZE = 16
def client_program():
    if(len(sys.argv) != 3):
        print("Usage: python client.py <ServerIP> <ServerPort>")
        sys.exit()

    port = int(sys.argv[2])

    # might give you a linked list in C
    server_ip = socket.gethostbyname(sys.argv[1])

    print("Server IP:", server_ip)

    server_addr = (server_ip, port)

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # blocking line, waits till server comes
    # tuples are created with structs in C
    # in C unless you do checking yourself it will not help you
    client_socket.connect(server_addr)
    
    message = input(" -> ")
    # ;;; is a protocol Wilkin made
    while(message.lower().strip() != ';;;'):
        client_socket.send(message.encode())

        in_data = client_socket.recv(16).decode()

        print("Received from server: ", str(in_data))

        message = input(" -> ")

    # MAKE SURE TO CLOSE IT
    client_socket.close()

if __name__ == '__main__':
    client_program()