//#include <iostream>
//#include <fstream>
//#include <string>
//#include <cstring>
//#include <cstdlib>
//#include <unistd.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//
//#define PORT 4242
//#define BUFFER_SIZE 1024
//
//using namespace std;
//
//void sendFile(int socket, const char* filePath) {
//    ifstream fileStream(filePath, ios::in | ios::binary);
//
//    if (!fileStream) {
//        cerr << "Unable to open file" << endl;
//        return;
//    }
//
//    char buffer[BUFFER_SIZE];
//    while (fileStream) {
//        fileStream.read(buffer, BUFFER_SIZE);
//        int bytesRead = fileStream.gcount();
//        send(socket, buffer, bytesRead, 0);
//    }
//
//    fileStream.close();
//}
//
//int main(int argc, char const *argv[]) {
//    int serverSocket, newSocket, valRead;
//    struct sockaddr_in address;
//    int opt = 1;
//    int addrlen = sizeof(address);
//    char buffer[BUFFER_SIZE] = {0};
//    const char* videoFilePath = "/Users/ael-khni/CLionProjects/webserv/www/media/image/myimage.jpg";
//    char* response = "HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\nContent-Length: 1513071\r\n\r\n";
////	const char* videoFilePath = "/Users/ael-khni/CLionProjects/webserv/www/media/video/video.mp4";
////	char* response = "HTTP/1.1 200 OK\r\nContent-Type: video/mp4\r\nContent-Length: 1570024\r\n\r\n";
//
//    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
//        perror("socket failed");
//        exit(EXIT_FAILURE);
//    }
//
//    address.sin_family = AF_INET;
//    address.sin_addr.s_addr = INADDR_ANY;
//    address.sin_port = htons(PORT);
//
//    if (bind(serverSocket, (struct sockaddr *)&address, sizeof(address))<0) {
//        perror("bind failed");
//        exit(EXIT_FAILURE);
//    }
//
//    if (listen(serverSocket, 3) < 0) {
//        perror("listen");
//        exit(EXIT_FAILURE);
//    }
//
//    while (true) {
//        if ((newSocket = accept(serverSocket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
//            perror("accept");
//            exit(EXIT_FAILURE);
//        }
//
////        valRead = read(newSocket, buffer, BUFFER_SIZE);
//        recv(newSocket, buffer, BUFFER_SIZE, 0);
//        cout << buffer << endl;
//
//        send(newSocket, response, strlen(response), 0);
//        sendFile(newSocket, videoFilePath);
//
//        close(newSocket);
//    }
//
//    return 0;
//}

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>

//#define PORT 4242
#define PORT 1337
#define BUFFER_SIZE 1024

using namespace std;

void sendFile(int socket, const char* filePath) {
    ifstream fileStream(filePath, ios::in | ios::binary);

    if (!fileStream) {
        cerr << "Unable to open file" << endl;
        return;
    }

    // Calculate the size of the file
    fileStream.seekg(0, ios::end);
    int fileSize = fileStream.tellg();
    fileStream.seekg(0, ios::beg);

    // Create the HTTP response header with the Content-Length field
    ostringstream responseHeader;
//   stringstream responseHeader;
    responseHeader << "HTTP/1.1 200 OK\r\n";
    responseHeader << "Content-Type: video/mp4\r\n";
//	responseHeader << "Content-Type: image/jpeg\r\n";
    responseHeader << "Content-Length: " << to_string(fileSize) << "\r\n";
    responseHeader << "\r\n";

    // Send the HTTP response header
    const string& response = responseHeader.str();
    send(socket, response.c_str(), response.size(), 0);

    // Send the file data
    char buffer[BUFFER_SIZE];
    int bytesSent = 0;
    while (fileStream) {
        fileStream.read(buffer, BUFFER_SIZE);
        int bytesRead = fileStream.gcount();
        send(socket, buffer, bytesRead, 0);
        bytesSent += bytesRead;
    }

    fileStream.close();
}

int main(int argc, char const *argv[]) {
    int serverSocket, newSocket, valRead;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
//    const char* videoFilePath = "/Users/ael-khni/CLionProjects/webserv/www/media/image/myimage.jpg";
	const char* videoFilePath = "/Users/ael-khni/CLionProjects/webserv/www/media/video/video.mp4";


    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (true) {
        if ((newSocket = accept(serverSocket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        recv(newSocket, buffer, BUFFER_SIZE, 0);
        cout << buffer << endl;

        sendFile(newSocket, videoFilePath);

        close(newSocket);
    }

    return 0;
}