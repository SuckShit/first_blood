#include "test.h"

//#define _CRT_SECURE_NO_WARNINGS 1
//
//#include <WINSOCK2.H>
//#include <stdio.h>
//#include <string.h>
//#include <iostream>
//#include <WS2tcpip.h>
//#define SERVER_ADDRESS "127.0.0.1"
//#define PORT           9876
//#define MSGSIZE        1024
//using namespace std;
//#pragma comment(lib, "ws2_32.lib")
//
//int main()
//{
//	WSADATA     wsaData;
//	SOCKET      sClient;
//	SOCKADDR_IN server;
//	char        sendMessage[MSGSIZE],recvMessage[MSGSIZE];
//	int         ret;
//	memset(sendMessage, 0, MSGSIZE);
//	memset(recvMessage, 0, MSGSIZE);
//
//	// Initialize Windows socket library
//	WSAStartup(0x0202, &wsaData);
//conn:
//	// Create client socket
//	sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//
//	// Connect to server
//	memset(&server, 0, sizeof(SOCKADDR_IN));
//	server.sin_family = AF_INET;
//	InetPton(AF_INET, SERVER_ADDRESS, &server.sin_addr.s_addr);
//	server.sin_port = htons(PORT);
//
//	ret = connect(sClient, (struct sockaddr *)&server, sizeof(SOCKADDR_IN));
//	if (ret < 0)
//	{
//		printf("connect error:%d\n", WSAGetLastError());
//	}
//	
//	while (TRUE)
//	{
//		printf("Send:");
//		cin >> sendMessage;
//		if (strlen(sendMessage) > MSGSIZE - 1)
//		{
//			return 0;
//		}
//		else
//		{
//			strcat(sendMessage, "\r\n");
//		}
//		// Send message
//		ret = send(sClient, sendMessage, strlen(sendMessage), 0);
//		if (ret == SOCKET_ERROR) {
//			printf("send failed: %d\n", WSAGetLastError());
//			closesocket(sClient);
//			goto conn;
//			//return 0;
//		}
//		// Receive message
//
//		ret = recv(sClient, recvMessage, MSGSIZE, 0);
//		if (ret > 0)
//		{
//			printf("Bytes received: %d\n", ret);
//			recvMessage[ret] = '\0';
//			printf("Received [%d bytes]: '%s'\n", ret, recvMessage);
//		}		
//		else if (ret == 0)
//		{
//			printf("Connection closed\n");
//			closesocket(sClient);
//			goto conn;
//		}
//		else
//		{
//			printf("recv failed: %d\n", WSAGetLastError());
//			closesocket(sClient);
//			goto conn;
//		}
//	}
//
//	// Clean up
//	closesocket(sClient);
//	WSACleanup();
//	return 0;
//}
//#ifdef __cplusplus
//extern "C" {
//#endif
//#include <event2/event.h>
//#include <event2/bufferevent.h>
//#include <event2/buffer.h>
//
//#include <WINSOCK2.h>
//#include <WS2tcpip.h>
//
//#include <string.h>
//#include <stdlib.h>
//#pragma comment(lib, "ws2_32.lib")
//	int64_t total_bytes_read = 0;
//	int64_t total_messages_read = 0;
//
//	static void set_tcp_no_delay(evutil_socket_t fd)
//	{
//		int one = 1;
//		setsockopt(fd, IPPROTO_TCP, TCP_NODELAY,
//			(char*)&one, sizeof one);
//	}
//
//	static void timeoutcb(evutil_socket_t fd, short what, void *arg)
//	{
//		struct event_base *base = (struct event_base*)arg;
//		printf("timeout\n");
//
//		event_base_loopexit(base, NULL);
//	}
//
//	static void readcb(struct bufferevent *bev, void *ctx)
//	{
//		/* This callback is invoked when there is data to read on bev. */
//		struct evbuffer *input = bufferevent_get_input(bev);
//		struct evbuffer *output = bufferevent_get_output(bev);
//
//		++total_messages_read;
//		total_bytes_read += evbuffer_get_length(input);
//
//		/* Copy all the data from the input buffer to the output buffer. */
//		evbuffer_add_buffer(output, input);
//	}
//
//	static void eventcb(struct bufferevent *bev, short events, void *ptr)
//	{
//		if (events & BEV_EVENT_CONNECTED) {
//			evutil_socket_t fd = bufferevent_getfd(bev);
//			set_tcp_no_delay(fd);
//		}
//		else if (events & BEV_EVENT_ERROR) {
//			printf("NOT Connected\n");
//		}
//	}
//
//	int main(int argc, char **argv)
//	{
//		struct event_base *base;
//		struct bufferevent **bevs;
//		struct sockaddr_in sin;
//		struct event *evtimeout;
//		struct timeval timeout;
//		int i;
//		WSADATA wsadata;
//		WSAStartup(0x0202, &wsadata);
//		//if (argc != 5) {
//		//	fprintf(stderr, "Usage: client <port> <blocksize> ");
//		//	fprintf(stderr, "<sessions> <time>\n");
//		//	return 1;
//		//}
//
//		int port = 9876;
//		int block_size = 10;
//		int session_count = 1;
//		int seconds = 1;
//		timeout.tv_sec = seconds;
//		timeout.tv_usec = 0;
//
//		base = event_base_new();
//		if (!base) {
//			puts("Couldn't open event base");
//			return 1;
//		}
//
//		char* message = (char*)malloc(block_size + 2);
//		memset(message, 0, block_size);
//		for (i = 0; i < block_size; ++i) {
//			message[i] = 'a' + i;
//		}
//		message[block_size] = '\r';
//		message[block_size + 1] = '\n';
//
//		//evtimeout = evtimer_new(base, timeoutcb, base);
//		//evtimer_add(evtimeout, &timeout);
//
//		memset(&sin, 0, sizeof(sin));
//		sin.sin_family = AF_INET;
//		sin.sin_addr.s_addr = htonl(0x7f000001); /* 127.0.0.1 */
//		sin.sin_port = htons(port);
//
//		bevs = (struct bufferevent **)malloc(session_count * sizeof(struct bufferevent *));
//		for (i = 0; i < session_count; ++i) {
//			struct bufferevent *bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
//
//			bufferevent_setcb(bev, readcb, NULL, eventcb, NULL);
//			bufferevent_enable(bev, EV_READ | EV_WRITE);
//			evbuffer_add(bufferevent_get_output(bev), message, block_size + 2);
//
//			if (bufferevent_socket_connect(bev,
//				(struct sockaddr *)&sin, sizeof(sin)) < 0) {
//				/* Error starting connection */
//				bufferevent_free(bev);
//				puts("error connect");
//				return -1;
//			}
//			bevs[i] = bev;
//		}
//
//		event_base_dispatch(base);
//
//		for (i = 0; i < session_count; ++i) {
//			bufferevent_free(bevs[i]);
//		}
//		free(bevs);
//		/*event_free(evtimeout);*/
//		event_base_free(base);
//		free(message);
//
//		printf("%zd total bytes read\n", total_bytes_read);
//		printf("%zd total messages read\n", total_messages_read);
//		printf("%.3f average messages size\n",
//			(double)total_bytes_read / total_messages_read);
//		printf("%.3f MiB/s throughtput\n",
//			(double)total_bytes_read / (timeout.tv_sec * 1024 * 1024));
//		return 0;
//	}
//#ifdef __cplusplus
//	}
//#endif

////
//using namespace std;
//class maintest
//{
//public:
//	int main(int argc, const char* const argv[])
//	{
//		cout << "in main class" << endl;
//		return 0;
//	}
//};
//
//class son : public maintest
//{
//
//};
//
//int main(int argc, char* argv[])
//{
//	son sonofbeach;
//	return sonofbeach.main(argc, argv);
//}

class myclass_constructor_test
{
private:
	char* my_test_v;
	int size;
public:
	myclass_constructor_test()
	{
		my_test_v = NULL;
		size = 0;
		cout << "default constructor" << endl;
	}
	myclass_constructor_test(const myclass_constructor_test& myclass)
	{
		cout << "copy constructor called" << endl;
		this->size = myclass.size;
		this->my_test_v = myclass.my_test_v;
	}
	myclass_constructor_test(myclass_constructor_test&& myclass)
	{
		cout << "move constructor called" << endl;
		my_test_v = myclass.my_test_v;
		size = myclass.size;
	}
	myclass_constructor_test& init()
	{
		size = 100;
		my_test_v = new char[size];
		cout << "init func called" << endl;
		strcpy_s(my_test_v, 5, "fuck");
		return *this;
	}
	virtual ~myclass_constructor_test()
	{
		cout << "myclass_constructor_test destructor called" << endl;
		if (my_test_v)
		{
			delete my_test_v;
			my_test_v = NULL;
		}
		size = 0;
	}
	myclass_constructor_test& operator= (const myclass_constructor_test& myclass)
	{
		cout << "copy assignment called" << endl;
		if (this != &myclass)
		{
			this->size = myclass.size;
			this->my_test_v = myclass.my_test_v;
		}
		return *this;
	}
	myclass_constructor_test& operator= (myclass_constructor_test&& myclass)
	{
		cout << "move assignment called" << endl;
		this->size = myclass.size;
		this->my_test_v = myclass.my_test_v;
		return *this;
	}
	void test_func()
	{
		my_test_v[0] = 'a';
	}
};
class mc_testa : public myclass_constructor_test
{
public:
	~mc_testa()
	{
		cout << "mc_testa destructor called" << endl;
	}
};

class mc_testb : public myclass_constructor_test
{
public:
	~mc_testb()
	{
		cout << "mc_testb destructor called" << endl;
	}
};

template<typename T>
void funObj(T x, T y)
{
	cout << typeid(x).name() << endl;
	cout << typeid(y).name() << endl;
}
template<typename T>
void funRef(const T& x, const T& y)
{
	cout << typeid(x).name() << endl;
	cout << typeid(y).name() << endl;
}
template<typename T>
void funRef_noConst(T& x, T& y)
{
	cout << typeid(x).name() << endl;
	cout << typeid(y).name() << endl;
}
template<typename T>
void g(T && value)
{
	vector<T> v;
}
class A
{
public:
	char x;
	int y;
	virtual void aoo() { cout << "a: foo" << endl; }
};
class B:public A
{
public:
	virtual void boo() { cout << "b: boo" << endl; }
};
class C
{
public:
	virtual void coo() { cout << "c: coo" << endl; }
};
class D :public A,public C
{};
class E :virtual public A
{
	int z;
	virtual void eoo() { cout << "e: eoo" << endl; }
};
class F :virtual public A
{
	virtual void foo() { cout << "f: foo" << endl; }
};
class G:public E, public F
{};
typedef void(*pfun)(void);
using gfun = void(*)(void);
int main()
{
	//myclass_constructor_test mtg;
	//{
	//	myclass_constructor_test mt;
	//	mt.init();
	//	mtg = mt;
	//}
	//myclass_constructor_test* pmt = new myclass_constructor_test();
	//mtg.test_func();
	funObj(1, 2);
	const int a = 0;
	int b = 2;
	funObj(a, b);
	int* const c = &b;

	int x;
	g(42);//
	g(move(x));//
	{
		myclass_constructor_test *mp = NULL, *mq = NULL;
		mp = new mc_testa();
		mq = new mc_testb();
		delete mp;
		delete mq;
		mp = NULL;
		mq = NULL;
	}
	cout << "============" << "sizeof A B D" << endl;
	cout << sizeof(A) << " " << sizeof(B) << " " << sizeof(D) << endl;
	cout << "============" << "sizeof E F" << endl;
	cout << sizeof(E) << " " << sizeof(F) << endl;
	A atest;
	B btest;
	cout << sizeof(int) << " " << sizeof(int*) << endl;
	pfun fun;
	fun = (pfun)*((int*)(*(int*)(&btest)));
	fun();
	fun = (pfun)*((int*)(*(int*)(&btest))+1);
	fun();
	cout << (int*)(&btest) << " " << (int*)(&btest) + 1 << endl;
	D dtest;
	int** pvftbl = (int**)(&dtest);
	//for (int i = 0; pvftbl[i][0] != NULL; i++)
	//{
		for (int j = 0; pvftbl[0][j] != NULL; j++)
		{
			fun = pfun(pvftbl[0][j]);
			fun();
		}
	//}
	dtest.A::x = 'a';
	E etest;
	F ftest;
	G gtest;
	return 0;
}