import socket
import sys

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
    # Bind (something about this only works for python)
    server_socket.bind('', port)
    # Listen (5 is how many it will listen to)
    server_socket.listen(5)

    while True:
        # sit and wait for connection
        # in python it will return 2 different things (new socket and address of the client that tried to connect)
        # USE THIS SOCKET NOW (different port number, doesn't matter)
        conn_socket, address = server_socket.accept()

        print("Connection made from ", str(address))

        while True:
            # it wont just send all if u do big number, it will break it up
            # decode puts in utf-8 format (string)
            # this is where client sends first
            data = conn_socket.recv(1024).decode()

            # times out or client left: received null
            if not data:
                break
            else:
                print("Data from client: ", str(data))
                data = str(data).upper()
                conn_socket.send(data.encode())

        conn_socket.close()


if __name__ == '__main__':
    server_program()