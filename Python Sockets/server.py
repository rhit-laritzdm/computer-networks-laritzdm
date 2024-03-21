import socket
import sys
import threading

def server_thread(my_client_socket, client_num):
    while True:
        # it wont just send all if u do big number, it will break it up
        # decode puts in utf-8 format (string)
        # this is where client sends first
        data = my_client_socket.recv(1024).decode()

        # times out or client left: received null
        if not data:
            break
        else:
            print("Data from client: ", str(client_num), str(data))
            data = str(data).upper()
            my_client_socket.send(data.encode())

    my_client_socket.close()

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

if __name__ == '__main__':
    server_program()