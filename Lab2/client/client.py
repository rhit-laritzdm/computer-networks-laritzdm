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
        print("That just aint right!")


def client_i_want(socket, request):
    file_path_server = request[6:]
    file_name = skim_file_name(file_path_server)
    print("What client directory would you like to save this file?")
    file_dir_client = input(" -> ")
    if file_dir_client == "":
        file_dir_client = "received_files/"
    file_path_client = file_dir_client + file_name
    print("Planning to store file on client at " + file_path_client)

    file_dir_server = skim_file_dir(file_path_server)
    if file_dir_server == "":
        file_dir_server = "store/"
        file_path_server = file_dir_server + file_name
        request = "iWant " + file_path_server


    if os.path.isfile(file_path_client):
        print("You already have this file dummy")
        print("Cancelling request")
    else:
        socket.send(request.encode())
        print("iWant request sent")
        recv_file(socket, file_path_client)


def client_u_take(socket, request):
    file_path_client = request[6:]
    file_name = skim_file_name(file_path_client)
    print("What server directory would you like to save this file?")
    file_dir_server = input(" -> ")
    if file_dir_server == "":
        file_dir_server = "store/"
    file_path_server = file_dir_server + file_name
    print("Planning to store file on server at " + file_path_server)
    request = "uTake " + file_path_server

    file_dir_client = skim_file_dir(file_path_client)
    if file_dir_client == "":
        file_dir_client = "received_files/"
        file_path_client = file_dir_client + file_name


    if os.path.isfile(file_path_client):
        # send the request and file
        socket.send(request.encode())
        response = socket.recv(SIZE).decode()
        if response != "1":
            print("Server already has specified file")
            return
        print("uTake request sent")
        send_file(socket, file_path_client)
    else:
        # File does not exist at the specified file path
        print("What you talkin bout Willis? I aint seen that file anywhere")


def send_file(socket, file_path_client):
    file_size = os.path.getsize(file_path_client)
    print("Sending file of size: " + str(file_size))
    socket.send(str(file_size).encode())
    # Wait for confirmation before sending file
    confirmation = socket.recv(SIZE).decode()
    if confirmation != "1":
        # don't send file
        print("Didn't receive proper confirmation, not sending file.")
        return

    # open file and read in binary mode
    file = open(file_path_client, 'rb')
    # send one line at a time
    line = file.read(SIZE)
    while line:
        socket.send(line)
        line = file.read(SIZE)
    file.close()
    print("Finished sending file")


def recv_file(socket, file_path_client):
    # assumes next message is size of file
    file_size_string = socket.recv(SIZE).decode()
    print("File size: " + file_size_string)
    file_size = int(file_size_string)

    if file_size == 0:
        print("What you talkin bout Willis? I aint seen that file anywhere!")
        return
    try:
        with open(file_path_client, "xb") as file:
            # send confirmation
            socket.send("1".encode())
            # Start receiving file
            bytes_received = 0
            print("Started receiving file")
            while (bytes_received < file_size):
                data = socket.recv(SIZE)
                file.write(data)
                bytes_received += SIZE
                #print("Received " + str(bytes_received) + " of " + str(file_size) + " bytes ")
            file.close()
            print("Finished receiving file of size " + str(os.path.getsize(file_path_client)))
    except FileExistsError:
        print("Error: File already exists")
        socket.send("0".encode())


def skim_file_name(file_path):
    index_of_slash = file_path.rfind("/")
    file_name = file_path[index_of_slash + 1 : ]
    return file_name


def skim_file_dir(file_path):
    index_of_slash = file_path.rfind("/")
    file_dir = file_path[: index_of_slash + 1]
    return file_dir


if __name__ == '__main__':
    client_program()
