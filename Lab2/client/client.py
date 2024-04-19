import socket
import sys
import os.path

SIZE = 1024
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
    client_socket.connect(server_addr)

    request = input(" -> ")
    # ;;; is a protocol Wilkin made
    while(request.lower().strip() != ';;;'):
        handle_request(client_socket, request)
        request = input(" -> ")

    # MAKE SURE TO CLOSE IT
    client_socket.close()

def handle_request(socket, request):
    if (request.startswith("iWant")):
        client_i_want(socket, request)
    elif (request.startswith("uTake")):
        client_u_take(socket, request)
    else:
        print("Client: Improperly formatted request, did not send to server.")

def client_i_want(socket, request):
    file_path = request[6:]
    if os.path.isfile(file_path):
        print("You already have this file dummy")
        print("Cancelling request")
    else:
        socket.send(request.encode())
        print("iWant request sent")
        recv_file(socket, file_path)
        print("Finished receiving file")

def client_u_take(socket, request):
    file_path = request[6:]
    if os.path.isfile(file_path):
        # send the request and file
        socket.send(request.encode())
        print("uTake request sent")
        send_file(socket, file_path)
        print("Finished sending file")
    else:
        # File does not exist at the specified file path
        print("Error: file not found, verify file exists at file path and request is properly formatted")

def send_file(socket, file_path):
    file_size = os.path.getsize(file_path)
    print("Sending file of size: " + str(file_size))
    socket.send(str(file_size).encode())
    # open file and read in binary mode
    file = open(file_path, 'rb')
    # send one line at a time
    line = file.read(SIZE)
    while (line):
        socket.send(line)
        line = file.read(SIZE)
    file.close()

def recv_file(socket, file_path):
    try:
        with open(file_path, "xb") as file:
            # start receiving file
            # assumes next message is size of file
            file_size_string = socket.recv(SIZE).decode()
            print("File size:")
            print(file_size_string)
            file_size = int(file_size_string)
            # recv one line at a time
            if file_size == 0:
                print("File is not coming, size given is < 1")
            else:
                bytes_received = 0
                while (bytes_received < file_size):
                    data = socket.recv(SIZE)
                    file.write(data)
                    bytes_received += sys.getsizeof(data)
                    print("Received " + str(bytes_received) + " of " + str(file_size) + " bytes ")
                file.close()
    except FileExistsError:
        print("Error: File already exists")

if __name__ == '__main__':
    client_program()