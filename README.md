# projectIP

Create a client/server program, using C language under Linux CentOS which implements the following tasks:

  a) Stream socket-based communication in IPv4 with the program ClientIPv4_win_2021.exe. Linux
     server will wait the clients requests at port 22GSE, where G=group’s number, S=semigroup’s
     number, E=team’s number. The IPv4 client should connect to all assigned addresses of the IPv4
     server.
     
  b) Processing of the command “xy#” (xy=01, 02, …20) obtained from the program
     ClientIPv4_win_2021.exe. The IPv6 client should connect to URL (Uniform Resource Locator)
     of the HTTP IPv6 server
     
  c) In case the received command is not recognized by the Linux server, a message “Command not
     implemented” will be sent back to the Windows-based IPv4 client. After the IPv6 stream socket
     connection has been established, the Linux-based IPv6 client will send the command
     GET / HTTP/1.0\r\n\r\n to program HTTP IPv6 Server.
     
 OBSERVATIONS: HTTP IPv6 Server waits commands from IPv6 client related to a web page. The
 results obtained are sent back without any additional processing and after that it closes the connection.
     
  d) Processing the answer from HTTP IPv6 Server (generic name, could be in reality an apache server
     for instance), forwarding it towards the program ClientIPv4_win_2021.exe but also storing it as .html
     file into the home directory. 
