CXX ?= g++

server: main.cpp ./http/http_conn.cpp
	$(CXX) main.cpp ./http/http_conn.cpp -o server -lpthread
clean:
	rm -r server