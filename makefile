CXX ?= g++
INCLUDE := -I/usr/include/mysql/
LIBS := -lpthread -lmysqlclient

server: main.cpp ./http/http_conn.cpp ./threadpool/threadpool.cpp
	$(CXX) main.cpp ./threadpool/threadpool.cpp ./http/http_conn.cpp ./sqlconnectpool/conn_pool.cpp ./sqlconnectpool/sql_conn.cpp -o server $(LIBS) $(INCLUDE)
clean:
	rm -r server