#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, const char **argv)
{
  const int serverPort = 22013;                      // IPv4 server port; the port the server listens on
  const int server6Port = 80;                        // IPv6 server port; the port the server listens on
  struct sockaddr_in server;                         // server structure of type sockaddr_in
  int sock_client = 0;                               // the socket for the client, initially set to 0
  int bl_flag = 0;                                   // bind/listen flag value
  char command[256];                                 // the buffer for the received command from the IPv4 client
  int sock_server = socket(AF_INET, SOCK_STREAM, 0); // creating the socket for the server. AF_INET - Address family for IPv4, type SOCK_STREAM - full-duplex sequenced byte streams, 0 - protocol TCP
  int sock_server6;                                  // creating the socket for the IPv6 server
  struct sockaddr_in6 server6;                       // server structure of type sockaddr_in6, for IPv6

  const char get_request[] = "GET / HTTP/1.0\r\n\r\n"; // GET request for IPv6
  char buf[10];                                        // the buffer in which we store each character from the ipv6  web server one by one
  int recv_state = 0;
  char received_data[10000]; // the buffer in which we store all html code from the ipv6 web server
  FILE *f;
  if (sock_server == -1)
  {
    perror("Socket creation for IPv4 not possible. Retry later.\n");
    printf("\nError number is: %d", errno);
    // printf("\nError description is: %s", strerror(errno));
    exit(1);
  }
  puts("IPv4 socket created!"); // If the socket creation is successful we print a message

  // IPv4 server initialization

  memset(&server, 0, sizeof(server)); // We allocate an area in memory for the server structure, filled initially with 0's
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(serverPort);

  // IPv6 server initialization

  memset(&server6, 0, sizeof(server6));
  inet_pton(AF_INET6, "2001:67c:2b0:19:6d:6169:6c6d:616e", &(server6.sin6_addr));
  server6.sin6_family = AF_INET6;
  server6.sin6_port = htons(server6Port);

  char nimp[24] = "Command not implemented"; // message for the commands that are not implemented
  // bind attaches an IP address and a port to the socket
  bl_flag = bind(sock_server, (struct sockaddr *)&server, sizeof(server));
  if (bl_flag == -1)
  {
    perror("Bind failed!");
    printf("\nError number is: %d", errno);
    // printf("\nError description is: %s", strerror(errno));
    exit(1);
  }
  else
    puts("Bind successfully!"); // if the bind is succesful we print a message

  // listen() waits for incoming connections
  bl_flag = listen(sock_server, 10); //10 is the maximum number of connections on the incoming queue
  if (bl_flag < 0)
  {
    perror("Listen failed!");
    printf("\nError number is: %d", errno);
    // printf("\nError description is: %s", strerror(errno));
    // exit(1);
  }
  // infinite loop
  while (1)
  {
    // we create the client socket
    sock_client = accept(sock_server, NULL, NULL); // first null because we accept any client, second null because the length of the second field (null) is 0.
    if (sock_client == -1)
    {
      perror("Accept failed!");
      printf("\nError number is: %d", errno);
      // printf("\nError description is: %s", strerror(errno));
      exit(1);
    }
    else
      puts("Client accepted!");

    int recv_value = 1; // initialize recv_value to 1 in order to enter inside the loop at the beginning

    while (recv_value != 0) // we stay in the loop as long as we receive data
    {
      recv_value = recv(sock_client, command, sizeof(command), 0); // puts in the buffer the command received from the client
      if (recv_value == -1)
      {
        perror("Error while receiving data");
        printf("Error number is: %d", errno);
        // printf("\nError description is: %s", strerror(errno));
      }
      else if (command[1] == '8')
      {
        // IPv6 client connection to the server
        sock_server6 = socket(AF_INET6, SOCK_STREAM, 0); // we create the IPv6 server socket only when we want to receive the html code
        if (sock_server6 == -1)
        {
          perror("Socket creation for IPv6 not possible. Retry later.\n");
          printf("\nError number is: %d", errno);
          // printf("\nError description is: %s", strerror(errno));
        }
        else
        {
          puts("IPv6 server socket created");

          // after the socket is created, we connect to to the IPv6 server
          if (connect(sock_server6, (struct sockaddr *)&server6, sizeof(server6)) < 0)
          {
            perror("IPv6 client connection failed");
            printf("\nError number is: %d", errno);
            // printf("\nError description is: %s", strerror(errno));
          }
          else
          {
            puts("IPv6 client connected");
            send(sock_server6, get_request, sizeof(get_request), 0); //after the connection is made, we send the GET HTTP request to the IPv6 server
            int recv_length = 1;
            while (recv_length > 0)
            {
              recv_length = recv(sock_server6, &buf, 1, 0); // we receive the data character by character from the IPv6 server in buf[]
              strcat(received_data, buf);                   // append each character in the received_data[] buffer
            }
            close(sock_server6); // if recv() returns 0 we close the connection with the IPv6 server
            puts("\nClosed connection with the IPv6 server");
            f = fopen("index.html", "w"); // open file for writing
            if (!f)
            {
              perror("\nindex.html file cannot be open for writing!");
              printf("\nError number is: %d", errno);
              // printf("\nError description is: %s", strerror(errno));
            }
            else
            {
              fputs(received_data, f); // store the html code in the file locally
              fclose(f);
            }
            // we send to the client the html code
            if (send(sock_client, received_data, sizeof(received_data), 0) == -1)
            {
              perror("Not sent!");
              printf("\nError number is: %d", errno);
              // printf("\nError description is: %s", strerror(errno));
            }
          }
        }
      }
      else
      {
        // we send to the client the 'not implemented' message
        if (send(sock_client, nimp, sizeof(nimp), 0) == -1) // sends to the client command not implemented message
        {
          perror("Not sent!");
          printf("\nError number is: %d", errno);
          // printf("\nError description is: %s", strerror(errno));
        }
      }
      bzero(received_data, sizeof(received_data)); // frees the array in order to be empty for the new request
    }
    puts("Client disconnected!");
    close(sock_client); // close IPv4 client socket
  }
  puts("Connection closed!");
  close(sock_server); // close the IPv4 server socket
}
