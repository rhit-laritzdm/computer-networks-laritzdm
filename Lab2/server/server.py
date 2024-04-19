import socket
import sys
import threading
import os.path

SIZE = 1024


def server_program():
    host = socket.gethostname()
    host_ip = socket.gethostbyname(host)

    print("Host name: ", str(host))
    print("Host IP: ", str(host_ip))

    if(len(sys.argv) != 2):
        print("Usage: python server.py <port_number>")
        sys.exit()

    # unsigned short
    port = int(sys.argv[1])

    # Create
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # forces socket to be able to be used even if already in use
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    # Bind (something about this only works for python)
    server_socket.bind(('', port))
    # Listen (5 is how many it will listen to)
    server_socket.listen(5)

    client_num = 1

    while True:
        # sit and wait for connection
        # in python it will return 2 different things (new socket and address of the client that tried to connect)
        # USE THIS SOCKET NOW (different port number, doesn't matter)
        conn_socket, address = server_socket.accept()

        print("Connection ", str(client_num), " made from ", str(address))

        # need comma after last
        t = threading.Thread(target=server_thread, args=(conn_socket, client_num,))
        t.start()

        client_num += 1

    server_socket.close()

def server_thread(client_socket, client_num):
    while True:
        # it wont just send all if u do big number, it will break it up
        # decode puts in utf-8 format (string)
        # this is where client sends first
        print("Listening for client request")
        request = client_socket.recv(SIZE).decode()

        # times out or client left: received null
        if not request:
            break
        else:
            print("Received request from client: ", str(client_num), str(request))
            handle_request(client_socket, request)

    client_socket.close()

def handle_request(socket, request):
    if request.startswith("iWant"):
        server_i_want(socket, request)
    elif request.startswith("uTake"):
        server_u_take(socket, request)
    else:
        print("Error: Improperly formatted request received from client.")

def server_i_want(socket, request):
    # check if file exists
    file_path = request[6:]
    if os.path.isfile(file_path):
        # file exists, so send it to client
        send_file(socket, file_path)
    else:
        # File does not exist at the specified file path
        print("Error: file not found, verify file exists at file path and request is properly formatted")
        # sends -1 for the file_size so client knows a file isn't coming
        socket.send("0".encode())


def server_u_take(socket, request):
    file_path = request[6:]
    if os.path.isfile(file_path):
        print("You already have this file dummy")
        print("Cancelling request")
    else:
        # send the request and file
        print("Replying to uTake request")
        recv_file(socket, file_path)

def send_file(socket, file_path):
    file_size = os.path.getsize(file_path)
    print("Sending file of size: " + str(file_size))
    socket.send(str(file_size).encode())
    # open file and read in binary mode
    file = open(file_path, 'rb')
    # send one line at a time
    line = file.read(SIZE)
    while line:
        socket.send(line)
        line = file.read(SIZE)
    file.close()

def recv_file(socket, file_path):
    try:
        with open(file_path, "xb") as file:
            # assumes next message is size of file
            rawrecv = socket.recv(SIZE)
            print(str(rawrecv))
            file_size_string = rawrecv.decode()
            print("File size:")
            print(file_size_string)
            file_size = int(file_size_string)
            print("error before checking file_size")
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
    server_program()