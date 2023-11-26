This is a Noob repo to display llama.cpp in the browser. It retains a diy feel, so it runs a cpp webserver using cpp httplib to get the results from running llamacpp. 

1. If you have got llama.cpp running on your machine, i.e you can get this command to work:
**_"../llama.cpp/main -m ../llama.cpp/models/llama-2-7b-chat/ggml-model-q4_0.gguf -n 1024 --prompt \""_**

2. Then you can change the path in line 23 in server.cpp 

3. hit make and run ./webserver

4. You can then access llama on localhost:8000

![llama](llama.png)




