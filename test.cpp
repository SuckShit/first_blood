#include "test.h"
//#include "function.h"
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

//class myclass_constructor_test
//{
//private:
//	char* my_test_v;
//	int size;
//public:
//	myclass_constructor_test()
//	{
//		my_test_v = NULL;
//		size = 0;
//		cout << "default constructor" << endl;
//	}
//	myclass_constructor_test(const myclass_constructor_test& myclass)
//	{
//		cout << "copy constructor called" << endl;
//		this->size = myclass.size;
//		this->my_test_v = myclass.my_test_v;
//	}
//	myclass_constructor_test(myclass_constructor_test&& myclass)
//	{
//		cout << "move constructor called" << endl;
//		my_test_v = myclass.my_test_v;
//		size = myclass.size;
//	}
//	myclass_constructor_test& init()
//	{
//		size = 100;
//		my_test_v = new char[size];
//		cout << "init func called" << endl;
//		strcpy_s(my_test_v, 5, "fuck");
//		return *this;
//	}
//	virtual ~myclass_constructor_test()
//	{
//		cout << "myclass_constructor_test destructor called" << endl;
//		if (my_test_v)
//		{
//			delete my_test_v;
//			my_test_v = NULL;
//		}
//		size = 0;
//	}
//	myclass_constructor_test& operator= (const myclass_constructor_test& myclass)
//	{
//		cout << "copy assignment called" << endl;
//		if (this != &myclass)
//		{
//			this->size = myclass.size;
//			this->my_test_v = myclass.my_test_v;
//		}
//		return *this;
//	}
//	myclass_constructor_test& operator= (myclass_constructor_test&& myclass)
//	{
//		cout << "move assignment called" << endl;
//		this->size = myclass.size;
//		this->my_test_v = myclass.my_test_v;
//		return *this;
//	}
//	void test_func()
//	{
//		my_test_v[0] = 'a';
//	}
//};
//class mc_testa : public myclass_constructor_test
//{
//public:
//	~mc_testa()
//	{
//		cout << "mc_testa destructor called" << endl;
//	}
//};
//
//class mc_testb : public myclass_constructor_test
//{
//public:
//	~mc_testb()
//	{
//		cout << "mc_testb destructor called" << endl;
//	}
//};
//
//template<typename T>
//void funObj(T x, T y)
//{
//	cout << typeid(x).name() << endl;
//	cout << typeid(y).name() << endl;
//}
//template<typename T>
//void funRef(const T& x, const T& y)
//{
//	cout << typeid(x).name() << endl;
//	cout << typeid(y).name() << endl;
//}
//template<typename T>
//void funRef_noConst(T& x, T& y)
//{
//	cout << typeid(x).name() << endl;
//	cout << typeid(y).name() << endl;
//}
//template<typename T>
//void g(T && value)
//{
//	vector<T> v;
//}
//class A
//{
//public:
//	char x;
//	int y;
//	virtual void aoo() { cout << "a: foo" << endl; }
//};
//class B:public A
//{
//public:
//	virtual void boo() { cout << "b: boo" << endl; }
//};
//class C
//{
//public:
//	virtual void coo() { cout << "c: coo" << endl; }
//};
//class D :public A,public C
//{};
//class E :virtual public A
//{
//	int z;
//	virtual void eoo() { cout << "e: eoo" << endl; }
//};
//class F :virtual public A
//{
//	virtual void foo() { cout << "f: foo" << endl; }
//};
//class G:public E, public F
//{};
//typedef void(*pfun)(void);
//using gfun = void(*)(int);
//
//void thread_task(int sec, promise<int> mypromise)
//{
//	this_thread::sleep_for(chrono::seconds(sec));
//	mypromise.set_value(sec);
//}
//
//class test_copyable
//{
//public:
//	test_copyable(int i = 0)
//	{
//		val = i;
//	}
//
//	test_copyable(const test_copyable& other) = delete;
//	test_copyable(test_copyable &&other)
//	{
//		cout << "move constructor" << endl;
//	}
//private:
//	int val;
//};
//void test_fun(test_copyable &&T)//参数类型限定右值 注意与模板的引用坍缩区分
//{
//
//}
//template<class T>
//void test_fun2(T&& t)
//{
//	test_fun(move(t));
//}
//class TestCopyAndMove
//{
//public:
//	TestCopyAndMove()
//	{
//		cout << "default" << endl;
//	}
//	TestCopyAndMove(const TestCopyAndMove& ref)
//	{
//		cout << "copy" << endl;
//	}
//	//TestCopyAndMove(TestCopyAndMove&& rv)
//	//{
//	//	cout << "move" << endl;
//	//}
//};
//
//TestCopyAndMove TestCopyAndMoveFun()
//{
//	TestCopyAndMove tcam;
//	return tcam;
//}


//int main()
//{
//	//myclass_constructor_test mtg;
//	//{
//	//	myclass_constructor_test mt;
//	//	mt.init();
//	//	mtg = mt;
//	//}
//	//myclass_constructor_test* pmt = new myclass_constructor_test();
//	//mtg.test_func();
//	funObj(1, 2);
//	const int a = 0;
//	int b = 2;
//	funObj(a, b);
//	int* const c = &b;
//
//	int x;
//	g(42);//
//	g(move(x));//
//	{
//		myclass_constructor_test *mp = NULL, *mq = NULL;
//		mp = new mc_testa();
//		mq = new mc_testb();
//		delete mp;
//		delete mq;
//		mp = NULL;
//		mq = NULL;
//	}
//	cout << "============" << "sizeof A B D" << endl;
//	cout << sizeof(A) << " " << sizeof(B) << " " << sizeof(D) << endl;
//	cout << "============" << "sizeof E F" << endl;
//	cout << sizeof(E) << " " << sizeof(F) << endl;
//	A atest;
//	B btest;
//	cout << sizeof(int) << " " << sizeof(int*) << endl;
//	pfun fun;
//	fun = (pfun)*((int*)(*(int*)(&btest)));
//	fun();
//	fun = (pfun)*((int*)(*(int*)(&btest))+1);
//	fun();
//	cout << (int*)(&btest) << " " << (int*)(&btest) + 1 << endl;
//	D dtest;
//	int** pvftbl = (int**)(&dtest);
//	//for (int i = 0; pvftbl[i][0] != NULL; i++)
//	//{
//		for (int j = 0; pvftbl[0][j] != NULL; j++)
//		{
//			fun = pfun(pvftbl[0][j]);
//			fun();
//		}
//	//}
//	dtest.A::x = 'a';
//	E etest;
//	F ftest;
//	G gtest;
//	/*
//	decltype和auto都可以用来推断类型，
//	但是二者有几处明显的差异：
//	1.auto忽略顶层const，decltype保留顶层const；
//	2.对引用操作，auto推断出原有类型，decltype推断出引用；
//	3.对解引用操作，auto推断出原有类型，decltype推断出引用；
//	4.auto推断时会实际执行，decltype不会执行，只做分析。
//	*/
//	const int* const p = &b;
//	decltype(p) whatisthis = &b;
//	cout << typeid(whatisthis).name() << endl;
//	cout << typeid(p).name() << endl;
//	const int* q = &b;
//	cout << typeid(q).name() << endl;
//	int* const qq = &b;
//	cout << typeid(qq).name() << endl;
//	gfun gfunf = NULL;
//	gfunf = [](int x)->void {};
//
//	{
//		int a = 1, b = 1, c = 1;
//
//		auto m1 = [a, &b, &c]() mutable {
//			auto m2 = [a, b, &c]() mutable {
//				std::cout << a << b << c << '\n';
//				a = 4; b = 4; c = 4;
//			};
//			a = 3; b = 3; c = 3;
//			m2();
//		};
//
//		a = 2; b = 2; c = 2;
//
//		m1();                             // 调用 m2() 并打印 123
//		std::cout << a << b << c << '\n'; // 打印 234
//	}
//
//	test_copyable testa;
//	test_copyable testb(move(testa));
//	test_fun(move(testa));
//	test_fun2(testa);
//	test_fun2(move(testa));
//
//	int value = 0;
//	promise<int> my_promise;
//	future<int> my_future = my_promise.get_future();
//	thread mythread(thread_task, 5, move(my_promise));//如果去掉move my_promise当作左值传入 在模板特化时根据引用折叠变成左值引用 在tuple中构造时出现问题
//	mythread.detach();
//	TestCopyAndMove asd(TestCopyAndMoveFun());
//	//Testusingfun tf = Realtestfun;
//	Testusingfun2 tf2 = Realtestfun;
//	//ftest(Realtestfun);
//	return 0;
//}
//
//using Testusingfun = void(int);
//using Testusingfun2 = void(*)(int);
//
//void add1(int v)
//{
//	return;
//}
//
//template <void(*T)(int)>
//void doOperation()
//{
//	int temp = 0;
//	T(temp);
//}
//template <typename F>
//void doOperation2(F f)
//{
//	int temp = 0;
//	f(temp);
//}
//template <void(*T)(int)>
//class doOperationClass
//{
//public:
//	void operator()(int x)
//	{
//		T(x);
//		return;
//	}
//};
//template <typename T>
//T returntestfun(T t)
//{
//	return t;
//}
//
//template<typename T>
//struct function_traits;
//
////普通函数
//template<typename Ret, typename... Args>
//struct function_traits<Ret(Args...)>
//{
//public:
//	enum { arity = sizeof...(Args) };
//	typedef Ret function_type(Args...);
//	typedef Ret return_type;
//	using stl_function_type = std::function<function_type>;
//	typedef Ret(*pointer)(Args...);
//
//	template<size_t I>
//	struct args
//	{
//		static_assert(I < arity, "index is out of range, index must less than sizeof Args");
//		using type = typename std::tuple_element<I, std::tuple<Args...>>::type;
//	};
//};
//
////函数指针
//template<typename Ret, typename... Args>
//struct function_traits<Ret(*)(Args...)> : function_traits<Ret(Args...)> {};
//
////std::function
//template <typename Ret, typename... Args>
//struct function_traits<std::function<Ret(Args...)>> : function_traits<Ret(Args...)> {};
//
////member function
//#define FUNCTION_TRAITS(...) \
//    template <typename ReturnType, typename ClassType, typename... Args>\
//    struct function_traits<ReturnType(ClassType::*)(Args...) __VA_ARGS__> : function_traits<ReturnType(Args...)>{}; \
//
//FUNCTION_TRAITS()
//FUNCTION_TRAITS(const)
//FUNCTION_TRAITS(volatile)
//FUNCTION_TRAITS(const volatile)
//
////函数对象
//template<typename Callable>
//struct function_traits : function_traits<decltype(&Callable::operator())> {};
//
//template<typename T>
//void PrintType()
//{
//	cout << typeid(T).name() << endl;
//}
//
//template < typename T, typename... List >
//struct IndexOf;
//
//template < typename T, typename Head, typename... Rest >
//struct IndexOf<T, Head, Rest...>
//{
//	enum { value = IndexOf<T, Rest...>::value + 1 };
//};
//
//template < typename T, typename... Rest >
//struct IndexOf<T, T, Rest...>
//{
//	enum { value = 0 };
//};
//
//template < typename T >
//struct IndexOf<T>
//{
//	enum { value = -1 };
//};
//template<typename T>
//void commfuntest();
//template<typename T>
//void commfuntest<T>(){}
//
//template<typename T>
//class aTMP {
//public: typedef const T reType;
//};
//
//void f() { std::cout << "global f()\n"; }
//
//template<typename T>
//class Base {
//public:
//	template <int N = 99>
//	void f() { std::cout << "member f(): " << N << '\n'; }
//};
//
//template<typename T>
//class Derived : public Base<T> {
//public:
//	typename T::reType m; // https://zh.cppreference.com/w/cpp/language/dependent_name
//	Derived(typename T::reType a) : m(a) { }
//	void df1() { f(); }                       // 调用全局 f()，而非想象中的基类 f()
//	void df2() 
//	{
//#ifdef __GNUC__
//		this->template f(); 
//#elif defined(_MSC_VER)
//		this->f();
//#endif
//	}        // 基类 f<99>()
//	void df3() { Base<T>::template f<22>(); } // 强制基类 f<22>()
//	void df4() { ::f(); }                     // 强制全局 f()
//};
//
//// 神一般的代码 通过编译期的报错 来计算N以内的所有素数 需要关闭遇到第一个错误就停止编译的选项
////template<int i> struct D { D(void*); operator int(); }; // 构造函数参数为 void* 指针
////
////template<int p, int i> struct is_prime { // 判断 p 是否为素数，即 p 不能整除 2...p-1
////	enum { prim = (p%i) && is_prime<(i > 2 ? p : 0), i - 1>::prim };
////};
////template<> struct is_prime<0, 0> { enum { prim = 1 }; };
////template<> struct is_prime<0, 1> { enum { prim = 1 }; };
////
////template<int i> struct Prime_print {
////	Prime_print<i - 1> a;
////	enum { prim = is_prime<i, i - 1>::prim };
////	// prim 为真时， prim?1:0 为 1，int 到 D<i> 转换报错；假时， 0 为 NULL 指针不报错
////	void f() { D<i> d = prim ? 1 : 0; a.f(); } // 调用 a.f() 实例化 Prime_print<i-1>::f()
////};
////template<> struct Prime_print<2> { // 特例，递归终止
////	enum { prim = 1 };
////	void f() { D<2> d = prim ? 1 : 0; }
////};
//#define PASTE(x, y) x ## y
//template <typename T>
//void funtestconst(T& param) { cout << typeid(T).name() << " T" << endl; }
////template <typename T>
////void funtestconst(const T& param) { cout << typeid(T).name() << " T&" << endl; }
//
//void funtestconst2(int& param){}
//
//template<typename Container, typename Index>
//auto autotemplatededuce(Container &c, Index i)->decltype(c[i])
//{
//	return c[i];
//}
//template<typename Container>
//void ranl(Container &c)
//{
//	//cout << type_id_with_cvr<Container>().pretty_name() << endl;
//}
//vector<int> makeintvec()
//{
//	vector<int> veci = { 10 };
//	return veci;
//}
//class randlmemfuntest
//{
//public:
//	randlmemfuntest()
//	{
//		cout << "this is constructor" << endl;
//	}
//	void testrlfun() &
//	{
//		cout << "this is l fun" << endl;
//	}
//	void testrlfun() &&
//	{
//		cout << "this is r fun" << endl;
//	}
//};
//void shellfun(randlmemfuntest&& rl) { forward<randlmemfuntest>(rl).testrlfun(); return; }
//
//unsigned int bits(unsigned int *pn, int WIDTH)
//{
//	for (int pos = WIDTH - 1; pos >= 0; pos--) {
//		if (pn[pos]) {
//			for (int nbits = 31; nbits > 0; nbits--) {
//				if (pn[pos] & 1 << nbits)//妈的傻逼百度百科运算符优先级是错的 位移在与之上
//					return 32 * pos + nbits + 1;
//			}
//			return 32 * pos + 1;
//		}
//	}
//	return 0;
//}
//
//constexpr int constexprfuntest(int x) { return x; }
//#if 0
//#ifdef _MSC_VER
//void printbacktrace(int frames)
//{
//	frames > 10 ? frames = 10 : 1;
//	void* backstack[10];
//	auto process = GetCurrentProcess();
//	SymInitialize(process, nullptr, true);
//	auto stackfm = CaptureStackBackTrace(0, frames, backstack, nullptr);
//	auto syminfo = (SYMBOL_INFO*)malloc(sizeof(SYMBOL_INFO) + 1024);
//	syminfo->MaxNameLen = 1024;
//	syminfo->SizeOfStruct = sizeof(SYMBOL_INFO);
//	DWORD disp;
//	auto line = (IMAGEHLP_LINE64*)malloc(sizeof(IMAGEHLP_LINE64));
//	line->SizeOfStruct = sizeof(IMAGEHLP_LINE64);
//	for (int i = 0; i < stackfm; i++)
//	{
//		DWORD64 stkadd = (DWORD64)backstack[i];
//		if (SymFromAddr(process, stkadd, 0, syminfo))
//		{
//			if(SymGetLineFromAddr64(process, stkadd, &disp, line))
//			{
//				printf("\tat %s in %s: line: %lu: address: 0x%0X\n", 
//					syminfo->Name, line->FileName, line->LineNumber, syminfo->Address);
//			}
//			else
//			{
//				printf("\tSymGetLineFromAddr64 returned error code %lu.\n", GetLastError());
//				printf("\tat %s, address 0x%0X.\n", syminfo->Name, syminfo->Address);
//			}
//		}
//		else
//		{
//			DWORD err = GetLastError();
//			cout << "stack[" << i << "] err" << endl;
//			break;
//		}
//	}
//}
//#endif
//#endif
//vector<string> names;
//template <typename T>
//void universereftest(T&& name)
//{
//	names.emplace_back(forward<T>(name));
//}
//void universereftest(int idx)//如果调用时传入的入参是short类型，重载决议会决定调用万能引用的函数版本，结果int转string出问题，编译出错
//{
//	return;
//}
////解决方案 标签分派
//template <typename T>
//void unvreftestimp(T&& name, false_type)//编译阶段无bool型
//{
//	names.emplace_back(forward<T>(name));
//}
//template <typename T>
//void unvreftestimp(T&& name, true_type)
//{
//	return;
//}
//template <typename T>
//void unvreftest(T&& name)
//{
//	return unvreftestimp(forward<T>(name), is_integral<typename remove_reference<T>::type>::value);//如果传入的name是个左值，根据引用坍缩，T是左值引用，所以直接is_integral<T>, 返回的是false_type
//}
//
////一个防止编译器总是重载决议使用万能引用类型T的构造函数版本而引发问题的修改方案
//string namefromid(int idx)
//{
//	return string("test");
//}
//class Person
//{
//public:
//	template<typename T,
//		typename = typename enable_if<!is_base_of<Person, typename decay<T>::type>::value &&
//									  !is_integral<typename remove_reference<T>::type>::value>::type>
//		explicit Person(T&& n):name(forward<T>(n)){}
//	explicit Person(int idx):name(namefromid(idx)){}
//private:
//	string name;
//};
//
//class staticmemtest
//{
//public:
//	const static int value = 1;
//};
////int staticmemtest::value = 1;
//void staticmemfuntest(int i){}
//template <typename T>
//void fwd(T&& arg)
//{
//	staticmemfuntest(forward<T>(arg));
//}
//vector<function<int(int)>> filters;
//class lambdatest
//{
//public:
//	lambdatest() { div = 4; }
//	void addfilter()
//	{
//		filters.emplace_back([=](int val) {return val % div; });
//	}
//private:
//	int div;
//};
//
//void PrintAandB(string A, string B)
//{
//	cout << A << B << endl;
//}
//string PringStr(string A)
//{
//	cout << A << endl;
//	return A;
//}
//int main()
//{
//	doOperation<add1>();
//	doOperation2(add1);
//	Testusingfun2 t2 = add1;
//	//doOperation<t2>();//You cannot use the name or address of a local variable as a template argument
//	doOperationClass<add1> object1;
//	object1(1);
//	list<int> ll{0,1,1,2,3,4,5};
//	vector<int> vv{ 0,1,2,2,3,4,5 };
//	list<int>::iterator it1;
//	vector<int>::iterator it2;
//	for (it1 = ll.begin(); it1 != ll.end();)
//	{
//		if (*it1 == 1)
//		{
//			it1 = ll.erase(it1);
//		}
//		else
//			it1++;
//	}
//	for (it2 = vv.begin(); it2 != vv.end();)
//	{
//		if (*it2 == 2)
//		{
//			it2 = vv.erase(it2);
//		}
//		else
//			it2++;
//	}
//	function<Testusingfun> testfunctional = add1;
//	int m = returntestfun<int>(2);
//
//	//time_std_function<std::function<void(float)>>(argc, "std::function");
//	//time_std_function<func::function<void(float)>>(argc, "func::function");
//	//func::function<void(int)> testmyfunc;
//	//testmyfunc = add1;
//
//	std::function<int(int)> fff = [](int a) {return a; };
//	PrintType<function_traits<std::function<int(int)>>::function_type>(); //将输出int __cdecl(int)
//	PrintType<function_traits<std::function<int(int)>>::args<0>::type>();//将输出int
//	PrintType<function_traits<decltype(fff)>::function_type>();//将输出int __cdecl(int)
//	commfuntest<int>();//在定义模板特例之前必须已经有模板通例（primary template）的声明；
//	cout << IndexOf<int, char, short, double, float, int>::value << endl;
//
//	Derived<aTMP<int>> a(10);
//	a.df1(); a.df2(); a.df3(); a.df4();
//	cout << PASTE("asd", "zxc") << endl;
//	int x = 10;
//	const int ax = x;
//	const int& bx = x;
//	funtestconst(ax);
//	funtestconst(bx);
//	//funtestconst(move(x)); //err 右值无法绑定到左值引用
//	//funtestconst2(ax); //err
//	//funtestconst2(bx); //err
//	vector<int> veci = { 0, 1, 2, 3, 4, 5 };
//	autotemplatededuce(veci, 5) = 10;
//	auto s = autotemplatededuce(makeintvec(), 0);
//	auto ss = autotemplatededuce(move(veci), 5);//为啥这个右值可以绑定到左值引用
//	ranl(std::move(veci));//msc_ver忽略了move??
//
//	unordered_map<string, string> u = {
//	{ "RED","#FF0000" },
//	{ "GREEN","#00FF00" },
//	{ "BLUE","#0000FF" }
//	};
//
//	for (auto& n : u) 
//	{
//		cout << typeid(n.first).name() << endl;//由于我们不能改变一个元素的关键字，因此这些pair的关键字部分是const的;用boost查看
//		//cout << type_id_with_cvr<decltype(n.first)>().pretty_name() << endl;
//	}
//	
//	randlmemfuntest rlt;
//	rlt.testrlfun();
//	shellfun(move(rlt));
//
//	unsigned int pn[4] = { 4294967294, 268435454, 65535, 0 };
//	int _what_ = bits(pn, 4);
//	int _x = 5;
//	std::array<int, constexprfuntest(5)> arr;
//	x = constexprfuntest(x);
//	//int _y = 0;
//	//cin >> _y;
//	//_y = constexprfuntest(_y);
//
//	cpp11impltest impltest;
//	Person p1(1);
//	Person p2("asd");
//	auto p3(p1);
//	auto p = &(staticmemtest::value);
//	fwd(staticmemtest::value);//msc_ver没有报错
//
//	{
//		unique_ptr<BaseCs> ub = make_unique<DeprivedCs>();
//		unique_ptr<BaseCs> ub2(new DeprivedCs());
//	}
//	{
//		auto lbd = make_unique<lambdatest>();
//		lbd->addfilter();
//	}
//	auto res = filters[0](7);
//
//	vector<int> lbdv;
//	lbdv.push_back(11);
//	auto funclbdv = bind([](const vector<int>& data) {cout << data[0] << endl; }, move(lbdv));
//	funclbdv();
//
//	auto bindtest = bind(PrintAandB, bind(PringStr, _1), bind(PringStr, _2));
//	bindtest("fuck", " you");
//
//	Handle<MyObj> myobjptr;
//	{
//		myobjptr = new MyObj();
//	}
//
//	return 0;
//}
void threadfun(moveornottest& mt)
{
	return;
}
using namespace myrpggame;
int main()
{
	//drink* mydrink = new coffee();
	//drink* mysugar = new sugar(*mydrink);
	//drink* myice = new icecream(*mydrink);
	//bar<int>();
	//double cost = mydrink->cost();
	
	//locale::global(locale(""));
	//wcout << DEFAULT_IMG << endl;
	//char c;
	//cin.get(c);
	//myrpggame::GameMap<myrpggame::Terrain>* instance = myrpggame::GameMap<myrpggame::Terrain>::getInstance();
	//instance->init();
	//myrpggame::Character* charac = new myrpggame::Character(instance);
	//charac->StartMove();
	//if (charac)
	//{
	//	delete charac;
	//	charac = nullptr;
	//}
	//instance->destroy();
	hash_test htt{1};
	unordered_set<hash_test, my_hash> us = { htt, {2}, {3} };
	for (auto& s : us)
		std::cout << s.hash_key << '\n';

	//try
	//{
	//	throw std::exception("fuck");
	//}
	//catch (std::exception e)
	//{
	//	cout << e.what();
	//}
	moveornottest mt1;
	moveornottest& mt2 = mt1;
	//tuple<moveornottest> asd = make_tuple(mt1);
	//refornottestfun(mt2);
	auto t1 = copycontest(mt1);
	auto t2 = movecontest(mt1);
	auto fwd = forward<moveornottest>(ref(mt1));
	thread thread1(threadfun, move(mt1));
	thread1.join();
	
	using type1 = remove_cv<const int* const>::type;
	cout << is_same<const int*, type1>::value << endl;
	doPrint(cout, 35, "ddd", 1.2);
	cout << endl;
	tupletest(8, "ddd", mt1);
	cout << endl;
	TempTest<int> tt(1);				//use T
	TempTest<double> tt2(2);	//use U

	//map<int, int> intmap;
	//intmap.insert(make_pair<int, int>(1, 1));
	//cout << intmap.size() << endl;
	//intmap.clear();
	//cout << intmap.size() << endl;

	//vector<int> intvec;
	//intvec.push_back(1);
	//int x = intvec.capacity();
	//intvec.clear();
	//x = intvec.capacity();
	//vector<int>().swap(intvec);
	//x = intvec.capacity();
	vector<BaseCs*>* intpvec = new vector<BaseCs *>;
	BaseCs* bcs = new BaseCs;
	delete bcs;
	intpvec->push_back(new BaseCs);
	delete (*intpvec)[0];
	vector<BaseCs*>().swap(*intpvec);//清空vector 包含的对象泄露
	Basesingle* p = Basesingle::Getinstance();
	//Derivesingle* q = new Derivesingle; 可以继承但是无法实例化
	virtualtest* vp = new derivevirtual;
	((derivevirtual*)vp)->get();

	cout << "=====" << sizeof(long) << endl;

	auto ptr = make_shared<BaseCs>("fuck");
	shared_ptr<BaseCs> xx;
	xx = ptr;
	Basefun1(ptr);
	cout << ptr.use_count() << endl;
	Basefun2(ptr);
	cout << ptr.use_count() << endl;

	int* aa = new int(2);
	int* bb = new int(3);
	swapaandb(aa, bb);
	swapaandb2(aa, bb);
	int aaa = 2;
	int bbb = 3;
	swapaandb(&aaa, &bbb);
	cout << aaa << " " << bbb << endl;

	BaseCs bcs1;
	DeprivedCs dcs1;
	DeprivedCs dcs2 = dcs1;
	BaseCs bcs2 = dcs1;
	bcs1 = dcs2;

	BaseCs* pbcs1 = &dcs1;
	pbcs1->f();

	using pfun = void(*)();
	cout << "vbf of BaseCs:" << endl;
	cout << pbcs1 << ":" << *(int*)pbcs1 << endl;
	pfun pfunimp = (pfun)(*((int*)(*(int*)pbcs1)+1));
	pfunimp();
	pfunimp = (pfun)(*((int*)(*(int*)pbcs1) + 2));
	pfunimp();
	pfunimp = (pfun)(*((int*)(*(int*)pbcs1) + 3));
	pfunimp();
	D d1;
	d1._a = 1;
	d1._b = 2;
	d1._c = 3;
	d1._d = 4;
	/*   content of &d1 in condition of virtual inheritance*/
	/*   vbf of B -------------------4byte*/
	/*   offset of A ----------------4byte*/
	/*   _b -------------------------4byte*/
	/*   vbf of C -------------------4byte*/
	/*   offset of A ----------------4byte*/
	/*   _c -------------------------4byte*/
	/*   _d -------------------------4byte*/
	/*   vbf of A -------------------4byte*/
	/*   _a -------------------------4byte*/

	thread td1(putter);
	thread td2(getter);
	td1.join();
	td2.join();

	int** arr = new int*[20];
	for (int i = 0; i < 20; i++)
	{
		arr[i] = new int[10];
	}
	BaseCs bbcs = funbacs();//草泥马傻逼医科大

	cout << sizeof(char*) << endl;
	cout << sizeof(short) << endl;
	cout << sizeof(int) << endl;
	cout << sizeof(double) << endl;
	cout << sizeof(float) << endl;

	aligned_union_t<1, BaseCs> _storage; 
	cout << typeid(_storage._Val).name() << endl;
	int alignofsz = alignof(BaseCs);
	BaseCs* pbcsaln = new(static_cast<void*>(&_storage)) BaseCs;

	ComplexLink<int>* head = CreateComplexLink<int>(5);
	cout << "the origin one:" << endl;
	printComplexLink(head);

	ComplexLink<int>* newhead = copyfromComplexLink(head);
	ComplexLink<int>* newhead2 = copyfromComplexLink2(head);
	cout << "the copy one:" << endl;
	printComplexLink(newhead);
	cout << "the other one:" << endl;
	printComplexLink(newhead2);

	bool bres = isRegularExpMatching("aa", "a");
	bres = isRegularExpMatching("aaa", "a*a");
	bres = isRegularExpMatching("aa", "a*");
	bres = isRegularExpMatching("ab", ".*");
	bres = isRegularExpMatching("aab", "c*a*b");

	int maxlen = 0;
	vector<string> whatthefuck = LongestSubString("asdhjklnm", "asdsdjklndhjkasdh", maxlen);

	shared_ptr<Foo> f = make_shared<Foo>();
	shared_ptr<Foo> f1 = f->getptr();
	shared_ptr<Bar> specific_data(f, &f->bar);

	cout << "shared_ptr's ref count is: " << f.use_count() << "and" << specific_data.use_count() << endl;
	f.reset();
	cout << "after reset, the shared_ptr's ref count is: " << f.use_count() << "and" << specific_data.use_count() << endl;
	//delete f1.get();  实际不能delete 必须delete reference control block
	unique_ptr<Foo> up = make_unique<Foo>();
	auto up2 = move(up);

	is_arithmetic_fun<int>();
	is_arithmetic_fun<Foo>();

	//基类指针转为派生类指针,且该基类指针指向基类对象。
	ANIMAL * ani1 = new ANIMAL;
	DOG * dog1 = dynamic_cast<DOG*>(ani1);
	if (dog1)
	{
		dog1->OutPutname();
	}

	try
	{
		DOG& dog11 = dynamic_cast<DOG&>(*ani1);
	}catch (const bad_cast& e)
	{
		cout << e.what() << endl;
	}
	
	DOG * dog111 = static_cast<DOG*>(ani1);
	//dog111->OutPuttype();//错误，在ANIMAL类型指针不能调用方法OutPutType（）；在运行时出现错误。

	//基类指针转为派生类指针，且该基类指针指向派生类对象
	ANIMAL * ani3 = new DOG;
	DOG* dog3 = static_cast<DOG*>(ani3);
	dog3->OutPutname(); //正确
	dog3->OutPuttype();

	DOG* dog33 = dynamic_cast<DOG*>(ani3);
	dog33->OutPutname(); //正确
	dog33->OutPuttype();

	//派生类指针转为基类指针，且该派生类指针指向派生类对象
	DOG *dog2 = new DOG;
	ANIMAL *ani2 = static_cast<DOG*>(dog2);
	ani2->OutPutname(); //正确，结果输出为大黄
	//ani2->OutPuttype();

	//派生类指针转为基类指针，且该派生类指针指向基类对象 wrong!!!
	//DOG * dog22 = new ANIMAL;

	return 0;
}