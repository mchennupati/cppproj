#include "cpp-httplib/httplib.h"
#include <fstream>
#include <sstream>
#include <cstdlib>

int main(void) {
    using namespace httplib;

    Server svr;

    // Serve the HTML file
    svr.Get("/", [](const Request& req, Response& res) {
        std::ifstream file("input.html");
        std::stringstream buffer;
        buffer << file.rdbuf();

        res.set_content(buffer.str(), "text/html");
    });

    // Handle POST request
    svr.Post("/sendMessage", [](const Request& req, Response& res) {
        std::string message = req.get_param_value("message");
        std::string command = "../llama.cpp/main -m ../llama.cpp/models/llama-2-7b-chat/ggml-model-q4_0.gguf -n 1024 --prompt \"" + message + "\"";

        // Execute the command and capture output
        std::cout << "Executing command: " << command << std::endl;
        std::string output;
        char buffer[128];
        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) {
            std::cerr << "Failed to run command" << std::endl;
            res.set_content("Error executing command", "text/plain");
            return;
        }
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            output += buffer;
        }
        pclose(pipe);

        // Send output as response
        res.set_content(output, "text/plain");
    });

    svr.listen("0.0.0.0", 8000);
}
