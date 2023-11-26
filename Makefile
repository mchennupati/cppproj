all: word_stream

word_stream: word_stream.cpp
	@g++ word_stream.cpp -o word_stream

clean:
	@rm -f word_stream