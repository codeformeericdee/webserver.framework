/*******************************************
Copyright Eric Dee 2023 all rights reserved.
*******************************************/

#include "HTTP.responder.c"

void HTTPRespond(struct HTTPServingSocket* socket)
{
    printf("Responding to a request\n");
}

void HTTPAccept(struct HTTPServingSocket* socket)
{
    printf("Getting an HTTP connection\n");

    // Get the size of the input buffer.
    int addressLength = sizeof(socket->SocketFrame);

    // Generate an address with responder metadata attached to it.
    int socketBinding = accept(
        socket->OutDescriptor, 
        (struct sockaddr*) &socket->SocketFrame, 
        (socklen_t*) &addressLength
        );

    // Copy the data into that array.
    read(socketBinding, socket->Buffer, socket->BufferSize);

    // Log the request.
    printf("%s\n", socket->Buffer);
    close(socketBinding);
}

int main() 
{
    DaemonSettings daemon1 = {
        AF_INET,
        SOCK_STREAM,
        0,
        INADDR_ANY,
        256,
        10,
        30000
    };

    struct HTTPServingSocket socketThread1 = SocketListeningHTTP(
        daemon1.Domain, 
        daemon1.Service, 
        daemon1.Protocol, 
        daemon1.Interface, 
        daemon1.Port, 
        daemon1.BacklogQueue,
        HTTPAccept,
        HTTPRespond,
        daemon1.BufferSize
    );

    socketThread1.Activity(&socketThread1);

    return 0;
}