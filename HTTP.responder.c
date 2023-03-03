/*******************************************
Copyright Eric Dee 2023 all rights reserved.
*******************************************/

#include <sys/socket.h>
#include <netinet/in.h>

//
#include <stdio.h>
#include <stdlib.h>

// Headers:

/*
    For book-keeping.
                */

typedef struct {
    int Domain;
    int Service;
    int Protocol;
    u_long Interface;
    u_short Port;
    int BacklogQueue;
    int BufferSize;
} DaemonSettings;

/* 
    This struct will not to operate on server data.
                                                */

struct HTTPServingSocket {
    int Service;
    int Protocol;
    int Domain;
    int Backlog; // The listen() queue count.
    int OutDescriptor;
    
    u_short Port;

    u_long Interface;

    struct sockaddr_in SocketFrame;

    void (*InActivity)(void*); // Delegate.
    void (*OutActivity)(void*); // Delegate.
    void (*Activity)(struct HTTPServingSocket*); // Forward declaration.

    int BufferSize;
    char* Buffer;
};

// Utilities:

/* 
    Allows use of the name socket. 
                                */

int BuildSocketDescriptor(int domain, int type, int protocol)
{
    return socket(domain, type, protocol);
}

// Delegation:

/* 
    Applies class-like functionality to the delegates.
                                                    */

void AssignSocketIO(struct HTTPServingSocket* socket)
{
    socket->InActivity(*&socket);
    socket->OutActivity(*&socket);
}

// Constructor:

struct HTTPServingSocket SocketListeningHTTP(
    int domain, int service, int protocol, u_long interface, int port, int backlog, 
    void (*inActivity)(void*), void (*outActivity)(void*), int bufferSize
)
{
    struct HTTPServingSocket socket;

    // Output descriptions:
    socket.Service = service;
    socket.Protocol = protocol;
    socket.Domain = domain;
    socket.Backlog = backlog;
    socket.Interface = interface;
    socket.Port = port;

    // Buffer info:
    socket.BufferSize = (bufferSize + 1); // Array + 1 for null terminator. !Important
    socket.Buffer = malloc(sizeof(char) * socket.BufferSize); // Calloc is not guaranteed to get a zero page.

    // Socket frame:
    socket.SocketFrame.sin_family = socket.Domain;
    socket.SocketFrame.sin_addr.s_addr = htonl(socket.Interface);
    socket.SocketFrame.sin_port = htons(socket.Port);
    memset(socket.SocketFrame.sin_zero, '\0', sizeof(socket.SocketFrame.sin_zero)); // Same as above.

    // Socket descriptors:

    socket.OutDescriptor = BuildSocketDescriptor(socket.Domain, socket.Service, socket.Protocol);

    if (socket.OutDescriptor == 0)
    {
        perror("Socket out could not be assigned.");
        exit(EXIT_FAILURE);
    }

    else if ((bind(socket.OutDescriptor, (struct sockaddr*) &socket.SocketFrame, sizeof(socket.SocketFrame))) < 0) 
    {
        perror("Socket out could not be bound to socket in.");
        exit(EXIT_FAILURE);
    }

    else if (listen(socket.OutDescriptor, socket.Backlog) < 0)
    {
        perror("Listen was less than zero.");
        exit(EXIT_FAILURE);
    }

    // Socket options:

    int result = setsockopt(socket.OutDescriptor, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
    if (result < 0)
    {
        printf("The socket could not be set up for reuse.\n");
        exit(EXIT_FAILURE);
    }

    // Final setup:

    else
    {
        socket.InActivity = inActivity;
        socket.OutActivity = outActivity;
        socket.Activity = AssignSocketIO;

        if (socket.InActivity != NULL && socket.OutActivity != NULL)
        {
            printf("The HTTP socket file descriptor was set up.\n");
        }

        return socket;
    }
}