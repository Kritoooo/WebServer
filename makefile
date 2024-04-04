CXX ?= g++

server: main.cpp ./http/http_conn.cpp ./threadpool/threadpool.cpp
	$(CXX) main.cpp ./threadpool/threadpool.cpp ./http/http_conn.cpp -o server -lpthread
clean:
	rm -r server