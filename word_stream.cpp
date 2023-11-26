#include <iostream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0) {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

  while (1) {
    std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n";
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("In accept");
        exit(EXIT_FAILURE);
    }

    char buffer[30000] = {0};
    valread = read(new_socket, buffer, 30000);

    std::string requestStr = buffer;

    // Check if it's a POST request
    // if (requestStr.find("POST") != std::string::npos) {
    //     size_t messageStart = requestStr.find("message=");
    //     if (messageStart != std::string::npos) {
    //         std::string message = requestStr.substr(messageStart + 8);
    //         // Here, you should URL-decode the message
    //         // For simplicity, we're just printing it
    //         std::cout << "Received message: " << message << std::endl;
    //     }
    // }
    if (requestStr.find("POST") != std::string::npos) {
        size_t messageStart = requestStr.find("message=");
        if (messageStart != std::string::npos) {
            std::string message = requestStr.substr(messageStart + 8);
            std::string command = "../llama.cpp/main -m ../llama.cpp/models/llama-2-7b-chat/ggml-model-q4_0.gguf -n 1024 --prompt \"" + message + "\"";

            // Execute the command and capture output
            std::cout << "Executing command: " << command << std::endl;
            std::string output;
            char buffer[128];
            FILE* pipe = popen(command.c_str(), "r");
            if (!pipe) {
                std::cerr << "Failed to run command" << std::endl;
            } else {
                while (fgets(buffer, sizeof buffer, pipe) != NULL) {
                    output += buffer;
                }
                pclose(pipe);
            }

            // Send output as response
            std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\n" + output;
            write(new_socket, response.c_str(), response.length());
        }
    } else {
        // Handle GET request or other non-POST requests
        // Send the HTML form as response
        std::string form = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n"
                           "<html><body>"
                           "<form action=\"/\" method=\"post\">"
                           "Enter message: <input type=\"text\" name=\"message\">"
                           "<input type=\"submit\" value=\"Submit\">"
                           "</form>"
                           "</body></html>";
        write(new_socket, form.c_str(), form.length());
    }

    close(new_socket);
}

    return 0;
}
