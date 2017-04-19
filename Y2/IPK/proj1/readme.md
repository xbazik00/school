#### NAME
  - ftrest - a client application for communication with server 
  - ftrestd - a server daemon for processing clients requests


#### SYNOPSIS
  - ftrest command remote-path [local-path]
  - ftrestd [-r root-dir] [-p port]

#### DESCRIPTION
Ftrest is client/sever file transfer application. It uses HTTP protocol and RESTful API. This applications is powered by two individual subprograms ftrest a ftrestd. The communication itself uses posix sockets to interact and send messages.
Ftrest is a client process basic commands [put,get,del,lst,mkd,rmd] and sends request to a server defined in remote-path. The remote-path corresponds with URI of the server. Request consists of header and body separated by two characters of a new line. Body is present only in case of command "put". RESTful operations used within this implementation are PUT, GET and DELETE.

Header structure:

    [RESTful operation] http://hostname/username/remote-path?type=[file/folder] HTTP1.1
    [header]: value
    .
    .
    .

Ftrestd is a server daemon for this network application. It runs in a loop and awaits a request from a client. Server works in concurrent regime so it is able to process more request at the same time. After the request is received and processed the server sends HTTP response to the client. In case of error, this error is sent in a JSON format. In case of "lst" command the answer is also structured in JSON format. 
Transmission of files is done by string possesing binary copy of the file.
Default port is 6677.

Commands:

- ftrest get remote-path [local-path]
  - file is stored under the name difined in remote path to the local-path directory
  - if no local-directory is present it is saved in the current working directory
- ftrest put remote-path local-path
  - the file stored in a local-path is send to the server and stored in a file defined in a remote-path
- ftrest del remote-path
  - removes a file in a remote-path
- ftrest lst remote-path
  - lists directories and files in a remote-path
- ftrest lst-more remote-path
  - lists directories and files in a remote-path similar to unix ls -la, not including . and ..
- ftrest mkd remote-path 
  - creates a directory in a remote-path
- ftrest rmd remote-path
  - removes a directory in a remote-path
- ftrest mv remote-path path
  - moves a file in from remote-path to path on the server, both paths represent files.
  - path is path on the remote server, not containing user, it is only possible to move files within one user
  - Example: ftrest cp http://localhost/user/test/test.txt test/test2.txt
- ftrest cp remote-path path
  - copies a file in from remote-path to path on the server, both paths represent files.
  - path is path on the remote server, not containing user, it is only possible to copy files within one user
  - Example: ftrest cp http://localhost/user/test/test.txt test/test2.txt



#### OPTIONS
ftrestd:
  - -r root directory of the server
  - -p port where this application will listen

#### EXTENSIONS
  - mv and cp commands
  - lst-more - extension of lst command

#### AUTHOR
Martin Bazik xbazik00@stud.fit.vutbr.cz
