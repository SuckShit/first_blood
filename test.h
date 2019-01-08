#include <utility>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <thread>
#include <future>
#include <memory>
#include <windows.h>
#include <functional>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <dbghelp.h>
#include <type_traits>
#include <assert.h>
#include <random>
#include <locale>
#include <tchar.h>
#include <stdexcept>
#include <map>
#include <condition_variable>
#include <bitset>
#include <iomanip>
#include "Impl.h"
//#include <boost/filesystem.hpp>
using namespace std;
using namespace placeholders;
//using boost::typeindex::type_id_with_cvr;
#pragma comment(lib,"Dbghelp.lib")

class BaseCs
{
public:
	operator int()
	{
		return strlen(m_data);
	}
	BaseCs()
	{ 
		m_data = new char[1];
		m_data[0] = '0';
		cout << "Base construction" << endl;
	}
	BaseCs(const char *str)
	{
		if (str == NULL) 
		{
			m_data = new char[1];
			m_data[0] = '0';
		}
		else 
		{
			m_data = new char[strlen(str) + 1];
			strcpy(m_data, str);
		}
	}
	virtual ~BaseCs() 
	{ 
		if (m_data)
		{
			delete[] m_data;
			m_data = nullptr;
		}
		cout << "Base destruction" << endl; 
	}
	BaseCs(const BaseCs& bc) 
	{ 
		if (!bc.m_data)
		{
			m_data = new char[1];
			m_data[0] = '0';
		}
		else
		{
			m_data = new char[int(const_cast<BaseCs*>(&bc)) + 1];
			strcpy(m_data, bc.m_data);
		}
		cout << "Base copy construction" << endl; 
	}
	BaseCs& operator=(const BaseCs& bc)
	{ 
		if (m_data)
		{
			delete[] m_data;
			m_data = nullptr;
		}
		if (this != &bc)
		{
			m_data = new char[int(const_cast<BaseCs*>(&bc)) + 1];
			strcpy(m_data, bc.m_data);
		}
		cout << "Base operator= overload" << endl; 
		return *this; 
	}
	virtual void f() { cout << "this is base f" << endl; }
	virtual void g() { cout << "this is base g" << endl; }

private:
	char* m_data;
};
class DeprivedCs:public BaseCs
{
public:
	DeprivedCs() { cout << "DeprivedCs construction" << endl; }
	~DeprivedCs() { cout << "DeprivedCs destruction" << endl; }
	DeprivedCs(const DeprivedCs& bc)
	{ 
		cout << "DeprivedCs copy construction" << endl; 
	}
	DeprivedCs& operator=(const DeprivedCs& bc) 
	{ 
		cout << "DeprivedCs operator= overload" << endl;
		return *this;
	}
	virtual void f1() { cout << "this is derived f1" << endl; }
	virtual void g() { cout << "this is derived g" << endl; }
};

void Basefun1(shared_ptr<BaseCs> spbc)
{
	cout << "Basefun1 " << spbc.use_count() << endl;
	return;
}
void Basefun2(shared_ptr<BaseCs>& spbc)
{
	cout << "Basefun2 " << spbc.use_count() << endl;
	return;
}

class BaseObj
{
public:

	BaseObj() : _ref(0)
	{
	}
	BaseObj(const BaseObj&) : _ref(0)
	{
	}

	virtual ~BaseObj()
	{
	}

	BaseObj& operator=(const BaseObj&)
	{
		return *this;
	}

	virtual void __incRef()
	{
		assert(_ref >= 0);
		++_ref;
	}
	virtual void __decRef() 
	{
		assert(_ref > 0);
		if (--_ref == 0)
		{
			delete this;
		}
	}
	virtual int __getRef() const
	{
		return _ref;
	}

protected:

	atomic<int> _ref;
};

template<typename T>
class Handle
{
public:
	typedef T element_type;
	Handle(T* p = 0)
	{
		this->_ptr = p;

		if (this->_ptr)
		{
			this->_ptr->__incRef();
		}
	}

	template<typename Y>
	Handle(const Handle<Y>& r)
	{
		this->_ptr = r._ptr;

		if (this->_ptr)
		{
			this->_ptr->__incRef();
		}
	}

	Handle(const Handle& r)
	{
		this->_ptr = r._ptr;

		if (this->_ptr)
		{
			this->_ptr->__incRef();
		}
	}

	~Handle()
	{
		if (this->_ptr)
		{
			this->_ptr->__decRef();
		}
	}

	Handle& operator=(T* p)
	{
		if (this->_ptr != p)
		{
			if (p)
			{
				p->__incRef();
			}

			T* ptr = this->_ptr;
			this->_ptr = p;

			if (ptr)
			{
				ptr->__decRef();
			}
		}
		return *this;
	}

	template<typename Y>
	Handle& operator=(const Handle<Y>& r)
	{
		if (this->_ptr != r._ptr)
		{
			if (r._ptr)
			{
				r._ptr->__incRef();
			}

			T* ptr = this->_ptr;
			this->_ptr = r._ptr;

			if (ptr)
			{
				ptr->__decRef();
			}
		}
		return *this;
	}

	Handle& operator=(const Handle& r)
	{
		if (this->_ptr != r._ptr)
		{
			if (r._ptr)
			{
				r._ptr->__incRef();
			}

			T* ptr = this->_ptr;
			this->_ptr = r._ptr;

			if (ptr)
			{
				ptr->__decRef();
			}
		}
		return *this;
	}

	template<class Y>
	static Handle dynamicCast(Y* p)
	{
		return Handle(dynamic_cast<T*>(p));
	}

	T* get() const
	{
		return _ptr;
	}

	T* operator->() const
	{
		if (!_ptr)
		{
			throw std::exception("operator-> err");
		}

		return _ptr;
	}

	T& operator*() const
	{
		if (!_ptr)
		{
			throw std::exception("operator* err");
		}

		return *_ptr;
	}

	operator bool() const
	{
		return _ptr ? true : false;
	}

	void swap(Handle& other)
	{
		std::swap(_ptr, other._ptr);
	}

	T* _ptr;
};

class MyObj : public BaseObj
{
public:
	MyObj():whatever(0)
	{}
	~MyObj()
	{
		cout << "MyObj destructor." << endl;
	}
	void setnum(int x)
	{
		whatever = x;
	}
private:
	int whatever;

};

namespace myLinkUp
{
#define defaultcols 10
#define defaultrows 10
#define defaulttypes 6

	class LinkUpStage
	{
	private:
		class icon
		{
		public:
			int xpos;
			int ypos;
			int type;
			bool empty; //false 无图
			bool operator==(icon tmp)
			{
				return this->xpos == tmp.xpos && this->ypos == tmp.ypos;
			}
		};
		int columns;
		int rows;
		int types;
		icon** icons;

	public:
		LinkUpStage(int col = defaultcols, int row = defaultrows, int tp = defaulttypes)
			:columns(col),rows(row),types(tp)
		{}
		bool setLinkUppara(int col, int row, int tp = defaulttypes)
		{
			//validation
			if (col % 2 && row % 2)
			{
				return false;
			}
			columns = col;
			rows = row;
			types = tp;
			return true;
		}
		void generateLinkUp()
		{
			icons = new icon*[rows];
			for (int i = 0; i < rows; i++)
			{
				icons[i] = new icon [columns];
			}

			vector<int> shuffle_vec;
			//srand(time(nullptr));
			int tp = 0;
			for (int i = 0; i < rows * columns; i = i + 2)
			{
				tp = rand() % types;
				shuffle_vec.push_back(tp);
				shuffle_vec.push_back(tp);
			}
			random_device rd;
			mt19937 gen(rd());
			shuffle(shuffle_vec.begin(), shuffle_vec.end(), gen);

			for (int i = 0; i < rows; i++)
			{
				for (int j = 0; j < columns; j++)
				{
					icons[i][j] = {i, j, shuffle_vec[i * rows + j], true};
				}
			}
		}
		~LinkUpStage()
		{
			for (int i = 0; i < rows; i++)
			{
				delete[] icons[i];
			}
			delete icons;
		}

		bool matchCon(icon a, icon b)
		{
			if(a == b || !(a.xpos == b.xpos || a.ypos == b.ypos))
			{
				return false;
			}
			int min, max;
			if (a.xpos == b.xpos)
			{
				if (a.ypos < b.ypos)
				{
					min = a.ypos;
					max = b.ypos;
				}
				else
				{
					min = b.ypos;
					max = a.ypos;
				}
				for (min++; min < max; min++)
				{
					if (icons[a.xpos][min].empty)
						return false;
				}
			}
			else
			{
				if (a.xpos < b.xpos)
				{
					min = a.xpos;
					max = b.xpos;
				}
				else
				{
					min = b.xpos;
					max = a.xpos;
				}
				for (min++; min < max; min++)
				{
					if (icons[min][a.ypos].empty)
						return false;
				}
			}
			return true;
		}
		bool matchOneCon(icon a, icon b)
		{
			if (a.xpos == b.xpos || a.ypos == b.ypos)
			{
				return false;
			}
			if (!icons[a.xpos][b.ypos].empty && matchCon(a, icons[a.xpos][b.ypos]) && matchCon(icons[a.xpos][b.ypos], b))
			{
				return true;
			}
			if (!icons[b.xpos][a.ypos].empty && matchCon(a, icons[b.xpos][a.ypos]) && matchCon(icons[b.xpos][a.ypos], b))
			{
				return true;
			}
			return false;
		}
		bool matchTwoCon(icon a, icon b)
		{
			for (int i = a.xpos - 1, j = a.ypos; i >= 0; i--)
			{
				if (icons[i][j].empty)
				{
					break;
				}
				if (matchOneCon(icons[i][j], b))
				{
					return true;
				}
			}
			for (int i = a.xpos + 1, j = a.ypos; i < columns; i++)
			{
				if (icons[i][j].empty)
				{
					break;
				}
				if (matchOneCon(icons[i][j], b))
				{
					return true;
				}
			}
			for (int i = a.xpos, j = a.ypos - 1; j >= 0; j--)
			{
				if (icons[i][j].empty)
				{
					break;
				}
				if (matchOneCon(icons[i][j], b))
				{
					return true;
				}
			}
			for (int i = a.xpos, j = a.ypos + 1; j < rows; j++)
			{
				if (icons[i][j].empty)
				{
					break;
				}
				if (matchOneCon(icons[i][j], b))
				{
					return true;
				}
			}
			return false;
		}
		bool matchLinkUp(icon a, icon b)
		{
			if (a.type != b.type)
			{
				return false;
			}

			if (matchCon(a, b))
			{
				return true;
			}
			else if (matchOneCon(a, b))
			{
				return true;
			}
			else if (matchTwoCon(a, b))
			{
				return true;
			}

			return false;
		}
	};
}

class drink
{
public:
	virtual double cost() { return total; };
	drink(double price = 0) { cout << "base drink" << endl; this->price = price; this->total = price; }
	void setprice(double price) { this->price = price; }
	void settotal(double price) { this->total = price; }

private:
	double total;
	double price;
};

class coffee : public drink
{
public:
	coffee(double price = 0.99) { cout << "coffee" << endl; setprice(price); settotal(price); }
};

//decorator
class decorator : public drink
{
public:
	decorator(drink& basedrink, double price = 0.0)
	{
		cout << "decorator base" << endl;
		basedrink.settotal(basedrink.cost() + price);
	}
};
class sugar : public decorator
{
public:
	sugar(drink& basedrink,double price = 0.14):decorator(basedrink, price)
	{
		cout << "decorator sugar" << endl; 
	}
};

class icecream : public decorator
{
public:
	icecream(drink& basedrink, double price = 0.25):decorator(basedrink, price)
	{
		cout << "decorator icecream" << endl;
	}
};

template<typename T>
struct S {
	template<typename U> void foo() {}
};

template<typename T>
void bar()
{
	S<T> s;
	s.foo<T>(); // vs can compile
	s.template foo<T>(); // OK
}
namespace myrpggame
{
#define DEFAUTL_HEIGHT 20
#define DEFAUTL_LENGTH 20
#define DEFAULT_IMG	   "_"
#define DEFAULT_CHA    "|"
	template <typename T>
	class GameMap
	{
	private:
		int length;
		int height;
		T** object;
		static mutex staticmutex;
		static GameMap* instance;
		GameMap() = delete;
		GameMap(int x , int y) :
			length(x), height(y)
		{
			object = new T*[height];
			for (int i = 0; i < height; i++)
			{
				object[i] = new T[length];
			}
		}
	public:
		static GameMap* getInstance(int x = DEFAUTL_LENGTH, int y = DEFAUTL_HEIGHT)
		{
			if (instance == nullptr)
			{
				lock_guard<mutex> lock(staticmutex);
				if (instance == nullptr)
				{
					instance = new GameMap(x, y);
				}
			}
			return instance;
		}
		void destroy()
		{
			for (int i = 0; i < height; i++)
			{
				delete[] object[i];
				object[i] = nullptr;
			}
			delete[] object;
			object = nullptr;

			delete instance;
			instance = nullptr;
		}
		int drawmap()
		{
			system("cls");
			for (int i = 0; i < length; i++)
			{
				for (int j = 0; j < height; j++)
				{
					object[i][j].loadimg();
				}
				cout << endl;
			}
			cout << endl;
			return 0;
		}
		int init()
		{
			int ret = drawmap();
			//and so on for the future
			return ret;
		}
		int getheight() { return height; }
		int getlength() { return length; }
		void drawpos(string pos, int x, int y)
		{
			system("cls");
			for (int i = 0; i < length; i++)
			{
				for (int j = 0; j < height; j++)
				{
					if (i == x && j == y)
					{
						cout << pos;
					}
					else
					{
						object[i][j].loadimg();
					}					
				}
				cout << endl;
			}
			cout << endl;
		}
	};

	class Terrain
	{
	private:
		string image;
	public:
		Terrain(string img = DEFAULT_IMG):image(img)
		{}
		void setimg(string img)
		{
			image = img;
		}
		void loadimg()
		{
			cout << image;
		}
	};

	class Character
	{
	private:
		int xpos;
		int ypos;
		GameMap<Terrain>* pInstance;
		string characimg;
	public:
		Character(GameMap<Terrain>* pinst, int x = 0, int y = 0, string img = DEFAULT_CHA):pInstance(pinst), characimg(img)
		{
			if (!ValidatePos(x, y))
			{
				xpos = 0;
				ypos = 0;
			}
			xpos = x;
			ypos = y;
			pInstance->drawpos(characimg, xpos, ypos);
		}
		int ValidatePos(int x, int y)
		{
			return pInstance->getheight() > y && pInstance->getlength() > x
				&& x >= 0 && y >= 0;
		}
		void StartMove()
		{
			HANDLE hd = GetStdHandle(STD_INPUT_HANDLE);
			DWORD mode;

			char c;
			while (1)
			{
				GetConsoleMode(hd, &mode);
				SetConsoleMode(hd, mode&~ENABLE_ECHO_INPUT&ENABLE_QUICK_EDIT_MODE);

				cin.get(c);
				switch (c)
				{
				case 'q':
				case 'Q':
					cout << "Game over" << endl;
					return;
				case 'a':
				case 'A':
					if (ValidatePos(xpos, ypos - 1))
					{
						ypos--;
						pInstance->drawpos(characimg, xpos, ypos);
					}
					break;
				case 'd':
				case 'D':
					if (ValidatePos(xpos, ypos + 1))
					{
						ypos++;
						pInstance->drawpos(characimg, xpos, ypos);
					}
					break;
				case 'w':
				case 'W':
					if (ValidatePos(xpos - 1, ypos))
					{
						xpos--;
						pInstance->drawpos(characimg, xpos, ypos);
					}
					break;
				case 's':
				case 'S':
					if (ValidatePos(xpos + 1, ypos))
					{
						xpos++;
						pInstance->drawpos(characimg, xpos, ypos);
					}
					break;
				}
				cin.clear();
			}
		}
	};

	mutex GameMap<Terrain>::staticmutex;
	GameMap<Terrain>* GameMap<Terrain>::instance;
}

class hash_test
{
public:
	int hash_key;
	bool operator==(const hash_test& hs)
	{
		return hash_key == hs.hash_key;
	}
};
bool operator==(const hash_test& lhs, const hash_test& rhs) {
	return lhs.hash_key == rhs.hash_key;
}
struct my_hash
{
	size_t operator()(hash_test const& hs) const
	{
		return hash<int>()(hs.hash_key);
	}
};

class moveornottest
{
public:
	moveornottest() = default;
	moveornottest(const moveornottest& mt)// = delete;
#if 1
	{
		cout << "moveornottest copy con" << endl;
	}
#endif
	moveornottest& operator=(const moveornottest& mt) = default;
	~moveornottest() = default;
	moveornottest(moveornottest&& mt)// = delete;
#if 1
	{
		cout << "moveornottest move con" << endl;
	}
#endif
	void set(int x) { what = x; }
private:
	int what;
};
template<typename T>
T refornottestfun(T&& x)
{
	auto at = forward<T>(x);
	return at;
}

template<typename T>
T copycontest(T t)
{
	t.set(10);
	return t;
}

template<typename T>
T movecontest(T t)
{
	t.set(11);
	return t;
}

template <typename Arg, typename... Args>
void doPrint(std::ostream& out, Arg&& arg, Args&&... args)//a holy fucking shit code
{
	out << forward<Arg>(arg);
	using expander = int[];
	(void)expander {
		0, (void(out << ',' << std::forward<Args>(args)), 0)...
	//  |     |                                          |
	//  |     |                                逗号表达式，返回后面一个0，因为是int数组
    //  |  防止内部返回类型重载了逗号操作符
	//防止参数包为空导致int[0]
	};
}

template < typename... Rest >
void doSomething(Rest&&... rest)
{
	using expander = int[];
	(void)expander {
		0, (void(cout << typeid(rest).name()), 0)...
	};
}
template <typename T, typename... Rest>
void tupletest(T&& t, Rest&&... rest)
{
	doSomething(tuple<decay_t<T>, decay_t<Rest>...>(forward<T>(t), forward<Rest>(rest)...));
}

template <typename T>
class TempTest
{
public:
	TempTest(T t):tt(t) 
	{
		cout << "use T" << endl;
	}
	template<typename U>
	TempTest(U u):tt(u)
	{
		cout << "use U" << endl;
	}
private:
	T tt;
};

class Basesingle
{
public:
	static Basesingle* Getinstance()
	{
		if (instance == nullptr)
		{
			lock_guard<mutex> lock(staticmutex);
			if (instance == nullptr)
			{
				instance = new Basesingle;
			}
		}
		return instance;
	}

private:
	Basesingle() = default;
	static Basesingle * instance;
	static mutex staticmutex;
};
mutex Basesingle::staticmutex;
Basesingle* Basesingle::instance = nullptr;

class Derivesingle : public Basesingle
{
public:
	Derivesingle() = default;
};

class virtualtest
{
public:
	virtualtest(int i):val(i){}
	virtual void whatever() { cout << "base" << endl; }
private:
	int val;
};
class  derivevirtual : public virtualtest
{
public:
	derivevirtual() :virtualtest(1) {}
	virtual void whatever() { cout << "derive" << endl; }
	void get() { return; }
private:

};
#pragma pack(1)
struct teststruct {
	uint8_t len; 
	uint8_t alloc; 
	unsigned char flags;
	char buf[];
};

void swapaandb(int* a, int* b)
{
	int* tmp = a;
	a = b;
	b = tmp;
}

void swapaandb2(int* a, int* b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

class A
{
public:
	virtual void f1()
	{
		cout << "A::f1()" << endl;
		cout << endl;
	}
	virtual void f2()
	{
		cout << "A::f2()" << endl;
		cout << endl;
	}
	int _a;
};

class B :virtual public A
{
public:
	virtual void f1()
	{
		cout << "B::f1()" << endl;
		cout << endl;
	}
	virtual void f3()
	{
		cout << "B::f3()" << endl;
		cout << endl;
	}

	int _b;
};
class C :virtual public A
{
public:
	virtual void f1()
	{
		cout << "C::f1()" << endl;
		cout << endl;
	}
	virtual void f4()
	{
		cout << "C::f4()" << endl;
		cout << endl;
	}
	int _c;
};

class D :public B, public C
{
public:
	virtual void f1()
	{
		cout << "D::f1()" << endl;
		cout << endl;
	}

	virtual void f5()
	{
		cout << "D::f5()" << endl;
		cout << endl;
	}

	int _d;
};


#define SIZE 4
struct product
{
	mutex mtx;
	condition_variable isfull;
	condition_variable isempty;
	int pos;
	int buf[SIZE];
};
struct product pdt;

void put(struct product * t, int data)
{
	unique_lock<mutex> lk(t->mtx);
	while (t->pos + 1 > SIZE - 1)//the  array is full
	{
		t->isfull.wait(lk);
	}
	t->pos = t->pos + 1;
	t->buf[t->pos] = data;
	printf("thread %d put a data %d  to  pos %d\n", GetCurrentThreadId(), data, t->pos);
	t->isempty.notify_all();
	lk.unlock();
}


void get(struct product * t)
{
	unique_lock<mutex> lk(t->mtx);
	while (t->pos < 0)//the  array is empty
	{
		t->isempty.wait(lk);
	}
	printf("thread %d get a data %d  to  pos %d\n", GetCurrentThreadId(), t->buf[t->pos], t->pos);
	t->pos = t->pos - 1;
	t->isfull.notify_all();
	lk.unlock();
}


void putter()
{
	int n;
	for (n = 0; n < 8; n++)
	{
		put(&pdt, n);
	}
}

void getter()
{
	int n;
	for (n = 0; n < 8; n++)
	{
		get(&pdt);
	}
}

BaseCs funbacs()
{
	BaseCs bcs;
	return bcs;
}
template <typename T>
void quicksort(T a[], int start, int end)
{
	T final = a[start];
	int i = start;
	int j = end;
	while (i < j)
	{
		while (i < j && a[j] >= final)
		{
			j--;
		}
		a[i] = a[j];
		while (i < j && a[i] <= final)
		{
			i++;
		}
		a[j] = a[i];
	}
	a[i] = final;
	quicksort(a, start, i - 1);
	quicksort(a, i + 1, end);
}

template <typename T>
void heap_adjust(T a[], int start, int end)
{
	int father = start;
	int son = father * 2 + 1;
	while (son <= end)
	{
		if (son + 1 <= end && a[son] > a[son + 1])
		{
			son++;
		}
		if (a[father] < a[son])
		{
			return;
		}
		else
		{
			swap(a[father], a[son]);
			father = son;
			son = son * 2 + 1;
		}
	}
}
template <typename T>
void heap_sort(T a[], int len)
{
	for (int i = len / 2 - 1; i >= 0; i--)
	{
		heap_adjust(a, i, len - 1);
	}
	for (int i = len - 1; i > 0; i--)
	{
		swap(a[0], a[i]);
		heap_adjust(a, 0, i - 1);
	}
}

//复杂链表的复制
template <typename T>
struct ComplexLink {
	T data;
	ComplexLink* next;
	ComplexLink* other;
};

template <typename T>
void printComplexLink(ComplexLink<T>* head)
{
	ComplexLink<T>* curr = head;
	while (curr)
	{
		cout << "curPos:" << curr << endl;
		cout << "-----" << curr->data << endl;
		if (curr->other)
		{
			cout << "-----otherPos:" << curr->other << endl;
			cout << "-------------" << curr->other->data << endl;
		}
		curr = curr->next;
	}
}

template <typename T>
ComplexLink<T>* copyfromComplexLink(ComplexLink<T>* head)
{
	if (head == nullptr)
	{
		return nullptr;
	}
	ComplexLink<T>* curr = head;
	ComplexLink<T>* newhead, *newcurr,*newprior;
	while (curr)
	{
		ComplexLink<T>* newNode = new ComplexLink<T>();
		newNode->data = curr->data;
		newNode->next = curr->next;
		curr->next = newNode;
		curr = newNode->next;
	}
	curr = head;
	newhead = curr->next;
	while (curr)//must assign the other pointer in one loop before disconnect the list in the other loop because the other pointer may point to the address before it
	{
		newcurr = curr->next;
		if (curr->other)
		{
			newcurr->other = curr->other->next;
		}
		curr = newcurr->next;
	}
	curr = head;
	newprior = nullptr;
	while (curr)
	{
		newcurr = curr->next;
		if (newprior)
		{
			newprior->next = newcurr;
			newprior = newprior->next;
		}
		else
		{
			newprior = newcurr;
		}
		curr->next = newcurr->next;
		newcurr->next = nullptr;
		curr = curr->next;
	}
	newprior->next = nullptr;
	return newhead;
}

template <typename T>
ComplexLink<T>* copyfromComplexLink2(ComplexLink<T>* head)
{
	using ComplexlinkMap = map<ComplexLink<T>*, ComplexLink<T>*>;
	ComplexlinkMap mymap;

	if (head == nullptr)
	{
		return nullptr;
	}
	ComplexLink<T>* curr = head;
	ComplexLink<T>* newhead = nullptr;
	ComplexLink<T>* prior = nullptr;

	while (curr)
	{
		ComplexLink<T>* newNode = new ComplexLink<T>();
		if (!prior)
		{
			prior = newNode;
		}
		else
		{
			prior->next = newNode;
			prior = prior->next;
		}
		if (!newhead)
		{
			newhead = newNode;
		}
		newNode->data = curr->data;
		mymap.insert(make_pair(curr, newNode));
		curr = curr->next;
	}
	curr = head;
	ComplexLink<T>* newcurr = newhead;
	while (curr && newcurr)
	{
		if (curr->other)
		{
			auto searchresult = mymap.find(curr->other);
			newcurr->other = searchresult->second;
		}
		curr = curr->next;
		newcurr = newcurr->next;
	}
	mymap.clear();
	return newhead;
}

template <typename T>
ComplexLink<T>* CreateComplexLink(int n)
{
	vector<ComplexLink<T>*> myvec;

	ComplexLink<T>* head = nullptr;
	ComplexLink<T>* prior = nullptr;
	for (int i = 0; i < n; i++)
	{
		ComplexLink<T>* newnode = new ComplexLink<T>;
		newnode->data = i;
		if (!head)
		{
			head = newnode;
		}
		if (!prior)
		{
			prior = newnode;
		}
		else
		{
			prior->next = newnode;
			prior = prior->next;
		}
		myvec.push_back(newnode);
	}
	prior->next = nullptr;
	random_device r;
	default_random_engine e(r());
	uniform_int_distribution<int> u(0, n - 1);

	ComplexLink<T>* curr = head;
	for (int i = 0;i < n; i++)
	{
		curr->other = myvec[u(e)];
		curr = curr->next;
	}
	vector<ComplexLink<T>*>().swap(myvec);
	return head;
}

//LeetCode No.10
bool isRegularExpMatching(string s, string p)//aaa a*a
{
//	1.dp[i][j] = dp[i - 1][j - 1], if p[j - 1] != '*' && (s[i - 1] == p[j - 1] || p[j - 1] == '.');
//	2.dp[i][j] = dp[i][j - 2], if p[j - 1] == '*' and the pattern repeats for 0 times;
//	3.dp[i][j] = dp[i - 1][j] && (s[i - 1] == p[j - 2] || p[j - 2] == '.'), if p[j - 1] == '*' and the pattern repeats for at least 1 times.

	int m = s.length();
	int n = p.length();
	vector<vector<bool>> dp(m + 1, vector<bool>(n + 1, false));

	dp[0][0] = true;

	for (int i = 0; i <= m; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			if (p[j - 1] != '*')
			{
				dp[i][j] = i > 0 && dp[i - 1][j - 1] && (s[i - 1] == p[j - 1] || p[j - 1] == '.');
			}
			else
			{
				dp[i][j] = dp[i][j - 2] /*&& s[i] != p[j - 2]*  because of 'a' and 'a*a' */ || i > 0 && dp[i - 1][j] && (s[i - 1] == p[j - 2] || p[j - 2] == '.');
			}
		}
	}
	return dp[m][n];
}

vector<string> LongestSubString(string s, string t, int& max)//最长后缀
{
	int m = s.length(), n = t.length();
	vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
	vector<string> res;
	max = 0;
	for (int i = 1; i <= m; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			if (s[i - 1] == t[j - 1])
			{
				dp[i][j] = dp[i - 1][j - 1] + 1;
				if (dp[i][j] > max)
				{
					max = dp[i][j];
					vector<string>().swap(res);
					res.push_back(s.substr(i - max, max));
				}
				else if(dp[i][j] == max)
				{
					res.push_back(s.substr(i - max, max));
				}
			}
			else
			{
				dp[i][j] = 0;
			}
		}
	}
	return res;
}

struct Bar {
	// some data that we want to point to
};

struct Foo : enable_shared_from_this<Foo> 
{
	shared_ptr<Foo> getptr()
	{
		return shared_from_this();
	}
	Bar bar;
};

template <typename T, enable_if_t<is_arithmetic_v<T>, int> = 0>
void is_arithmetic_fun()
{
	cout << "I'm arithmetic." << endl;
}

template <typename T, enable_if_t<is_class_v<T>, int> = 0>
void is_arithmetic_fun()
{
	cout << "I'm class." << endl;
}

class ANIMAL
{
public:
	ANIMAL() :_type("ANIMAL") {};
	virtual void OutPutname() { cout << "ANIMAL" << endl; }
private:
	string _type;
};
class DOG :public ANIMAL
{
public:
	DOG() :_name("大黄"), _type("DOG") {};
	virtual void OutPutname() { cout << _name << endl; }
	void OutPuttype() { cout << _type << endl; }
private:
	string _name;
	string _type;
};


struct BaseClass { 
	virtual const char * behave() { return "Base"; };
	virtual void fa() {} 
	int _a;
};
struct Left : BaseClass { 
	virtual const char * behave() { return "Left"; } 
	virtual void fl() {} 
	int _b;
};
struct Right : BaseClass {
	virtual const char * behave() { return "Right"; } 
	virtual void fr() {}
	int _c;
};
struct MostDerived : Left, Right {
	virtual const char * behave() { return "Derived"; }
	virtual void fd(){}
	int _d;
};
//leetcode 4
double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
	vector<int> result;
	int m = nums1.size(), n = nums2.size();
	int i = 0, j = 0;
	for (; i < m && j < n;)
	{
		nums1[i] <= nums2[j] ? (result.push_back(nums1[i]), i++)
			: (result.push_back(nums2[j]), j++);
	}
	if (i < m)
	{
		for (; i < m; i++)
			result.push_back(nums1[i]);
	}
	else if (j < n)
	{
		for (; j < n; j++)
			result.push_back(nums2[j]);
	}
	double res1 = result[(m + n) / 2];
	double res2 = result[(m + n) / 2 - 1];
	if ((m + n) % 2 == 0)
		return (res1 + res2) / 2;
	else
		return res1/2;
}

//leetcode 4 log(m + n)
/*
		leftpart					rightpart
A[0] A[1] A[2] ... A[i-1] |  A[i] A[i+1] ... A[m-1]
B[0] B[1] B[2] ... B[j-1] |  B[j] B[j+1] ... B[n-1]
*/
double findMedianSortedArrays2(vector<int>& nums1, vector<int>& nums2) {
	int m = nums1.size(), n = nums2.size();
	if (m > n)
	{
		m = m ^ n;
		n = m ^ n;
		m = m ^ n;
		nums1.swap(nums2);
	}
	int left = 0, right = m;
	while (left <= right)
	{
		int i = (left + right) / 2;
		int j = (m + n + 1) / 2 - i;
		if (i > 0 && nums1[i - 1] > nums2[j])
		{
			right = --i;
		}
		else if (i < m && nums2[j - 1] > nums1[i])
		{
			left = ++i;
		}
		else
		{
			if (i == 0)
			{
				left = nums2[j - 1];
			}
			else if (j == 0)
			{
				left = nums1[i - 1];
			}
			else
			{
				left = std::max<int>(nums1[i - 1], nums2[j - 1]);
			}
			if (i == m)
			{
				right = nums2[j];
			}
			else if (j == n)
			{
				right = nums1[i];
			}
			else
			{
				right = std::min<int>(nums1[i], nums2[j]);
			}
			if ((m + n) % 2 == 0)
			{
				return (double(left + right)) / 2;
			}
			else
			{
				return left;
			}
		}
	}
}
//leetcode 23
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};
ListNode* merge2lists(ListNode* l1, ListNode* l2)
{
	if (l1 == nullptr)
	{
		return l2;
	}
	if (l2 == nullptr)
	{
		return l1;
	}
	ListNode* head = nullptr, *prior = nullptr;
	while (l1 && l2)
	{
		if (prior == nullptr)
		{
			prior = l1->val <= l2->val ? l1 : l2;
			head = prior;
		}
		else
		{
			prior->next = l1->val <= l2->val ? l1 : l2;
			prior = prior->next;
		}
		prior == l1 ? l1 = l1->next : l2 = l2->next;
	}
	l1 != nullptr ? prior->next = l1 : true;
	l2 != nullptr ? prior->next = l2 : true;
	return head;
}
ListNode* mergeKLists(vector<ListNode*>& lists) 
{
	if (lists.empty())
	{
		return nullptr;
	}
	while (lists.size() > 1)
	{
		lists.push_back(merge2lists(lists[0], lists[1]));
		lists.erase(lists.begin());
		lists.erase(lists.begin());
	}
	return lists[0];
}
ListNode* makesortedlist(int len)
{
	random_device r;
	default_random_engine e(r());
	uniform_int_distribution<int> u(0, 100);

	vector<int> tmp;
	for (int i = 0; i < len; i++)
	{
		tmp.push_back(u(e));
	}
	sort(tmp.begin(), tmp.end(), [](int x, int y) {return x < y; });
	ListNode* head = nullptr, *prior = nullptr;
	for (int i = 0; i < len; i++)
	{
		ListNode* l = new ListNode(tmp[i]);
		if (prior == nullptr)
		{
			prior = l;
			head = l;
		}
		else
		{
			prior->next = l;
			prior = prior->next;
		}
	}
	return head;
}

//leetcode 25
ListNode* reverseList(ListNode* &head)
{
	if (!head)
	{
		return nullptr;
	}
	ListNode* tail = head, *curp = head->next;
	while (curp)
	{
		ListNode* p = curp->next;
		tail->next = p;
		curp->next = head;
		head = curp;
		curp = p;
	}
	return head;
}
ListNode* reverseKGroupImpl(ListNode* &head, int k) 
{
	ListNode* curhead, *curtail, *curp, *pretail = nullptr;
	if (!head)
	{
		return nullptr;
	}
	if (k == 1 || k == 0)
	{
		return head;
	}
	curhead = head;
	curtail = head;
	curp = head;
	bool flag = false;
	while (curp)
	{
		int i = 0;
		for (; i < k; i++)
		{
			if (curp == curhead)
			{
				curp = curp->next;
				continue;
			}
			if (curp)
			{
				ListNode* p = curp->next;
				curtail->next = p;
				curp->next = curhead;
				curhead = curp;
				curp = p;
			}
			else
			{
				break;
			}
		}
		if (i < k)
		{
			reverseList(curhead);
			return head;
		}
		if (!flag)
		{
			head = curhead;
			flag = true;
		}
		if (pretail)
		{
			pretail->next = curhead;
		}
		pretail = curtail;
		curhead = curtail->next;
		curtail = curhead;
	}
	return head;
}

//leetcode 32
int longestValidParentheses(string s) 
{
	int max = 0, total = 0, curr = -1;
	int n = s.length();
	struct st
	{
		char c;
		int pos;
	};
	vector<st> pair;
	for (int i = 0; i < n; i++)
	{
		if (!pair.empty() && pair.back().c == '(' && s[i] == ')')
		{
			pair.pop_back();
		}
		else
		{
			pair.push_back(st{ s[i], i });
		}
	}
	if (pair.empty())
	{
		return n;
	}
	for (int i = 0; i < pair.size(); i++)
	{
		total = pair[i].pos - curr - 1;
		max = total > max ? total : max;
		curr = pair[i].pos;
	}
	total = n - 1 - curr;
	max = total > max ? total : max;
	return max;
}

//leetcode 37
class SolutionLC37 {
private:
	struct element
	{
		int value;
		bitset<10> nums;		//bitset<n> == 1表示 这个还没设
		int rest;				//剩余多少个数字
		element():value(0), rest(9), nums(0x03fe) {}
	};
	vector<vector<element>> cells;
	vector<pair<int, int>> btstate;
	bool setbit(int i, int j, int v)//设置每一个element
	{
		for (int k = 0; k < 9; k++)
		{
			if (k != j && !updatebitset(i ,k, v))
			{
				return false;
			}
			if (k != i && !updatebitset(k, j, v))
			{
				return false;
			}
			int m = i / 3 * 3 + k / 3;
			int n = j / 3 * 3 + k % 3;
			if (m != i && n != j && !updatebitset(m, n, v))
			{
				return false;
			}
		}
		element& c = cells[i][j];
		c.value = v;
		if (--c.rest < 0)
		{
			return false;
		}
		c.nums.reset(v);
		return true;
	}
	bool updatebitset(int i, int j, int v)
	{
		element& c = cells[i][j];
		if (c.value == v)
		{
			return false;
		}
		if (!c.nums[v])
		{
			return true;
		}
		if (--c.rest < 0)
		{
			return false;
		}
		c.nums.reset(v);
		return true;
	}
	void sortemptycell()
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (!cells[i][j].value)
				{
					btstate.push_back(make_pair(i, j));
				}
			}
		}
		sort(btstate.begin(), btstate.end(), [this](const pair<int, int>& a, const pair<int, int>& b)
		{return cells[a.first][a.second].rest < cells[b.first][b.second].rest; });
	}
	bool trytofillcells(int k)
	{
		if (k >= btstate.size())
		{
			return true;
		}
		int m = btstate[k].first;
		int n = btstate[k].second;
		if (cells[m][n].value)
		{
			return trytofillcells(k + 1);
		}
		vector<vector<element>> snapshot(cells);
		for (int v = 1; v <= 9; v++)
		{
			if (cells[m][n].nums[v])
			{
				if (setbit(m, n, v))
				{
					if (trytofillcells(k + 1))
					{
						return true;
					}
				}
				cells = snapshot;
			}			
		}
		return false;
	}
public:
	SolutionLC37()
	{
		vector<vector<element>> tmp(9, vector<element>(9, element()));
		cells.swap(tmp);
	}
	bool solveSudoku(vector<vector<char>>& board) 
	{
		for (int i = 0;i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				char c = board[i][j];
				if (c != '.' && !setbit(i, j, c - '0'))
				{
					return false;
				}
			}
		}
		sortemptycell();
		if (trytofillcells(0))
		{
			for (int i = 0; i < 9; i++)
			{
				for (int j = 0; j < 9; j++)
				{
					board[i][j] = cells[i][j].value + '0';
				}
			}
			return true;
		}
		return false;
	}
};

//leetcode 41
int firstMissingPositive(vector<int>& nums) 
{
	int len = nums.size();
	if (len == 0)
	{
		return 1;
	}
	for (int i = 0; i < len; i++)
	{
		if (nums[i] == 1 && i != 0)
		{
			swap(nums[0], nums[i]);
			break;
		}
	}
	if (nums[0] != 1)
	{
		return 1;
	}
	else
	{
		for (int i = 1; i < len;)
		{
			if (nums[i] > len || nums[i] <= 0)
			{
				nums[i] = 0;
				i++;
			}
			else
			{
				if (nums[i] - 1 == i)
				{
					i++;
				}
				else if (nums[nums[i] - 1] == nums[i])
				{
					nums[i] = 0;
					i++;
				}
				else
				{
					swap(nums[i], nums[nums[i] - 1]);
				}
			}
		}
		for (int i = 1; i < len; i++)
		{
			if (nums[i] == 0)
			{
				return i + 1;
			}
		}
		return len + 1;
	}
}
//leetcode 42
class SolutionLC42 {
private:
	vector<pair<int, int>> twotops;
	vector<int> peaks;
	
	void find2tops(int start, int end, vector<int>& height)
	{
		if (start >= end)
		{
			return;
		}
		int max1 = 0, max2 = 0, maxcur1 = 0, maxcur2 = 0;
		for (int i = start; i <= end; i++)
		{
			if (height[peaks[i]] >= max1)
			{
				max2 = max1;
				max1 = height[peaks[i]];
				maxcur2 = maxcur1;
				maxcur1 = i;
			}
			else if (height[peaks[i]] > max2)
			{
				max2 = height[peaks[i]];
				maxcur2 = i;
			}
		}
		maxcur1 <= maxcur2 ? max1 = max1 : swap(maxcur1, maxcur2);
		twotops.push_back(make_pair(peaks[maxcur1], peaks[maxcur2]));
		find2tops(start, maxcur1, height);
		find2tops(maxcur2, end, height);
	}
public:
	int trap(vector<int>& height)
	{
		int len = height.size();
		if (len <= 2)
		{
			return 0;
		}

		for (int i = 0; i < len; i++)
		{
			if (i == 0 && height[0] > height[1] || i == len - 1 && height[i] > height[i - 1])
			{
				peaks.push_back(i);
			}
			else if (i > 0 && i < len - 1)
			{
				if (height[i] >= height[i - 1] && height[i] > height[i + 1] || height[i] > height[i - 1] && height[i] >= height[i + 1])
				{
					peaks.push_back(i);
				}
			}
		}
		int peaknum = peaks.size();
		find2tops(0, peaknum - 1, height);
		sort(twotops.begin(), twotops.end(), [this](pair<int, int>& a, pair<int, int>& b) {
			return a.first < b.first;
		});
		int total = 0;
		for (int i = 0; i < twotops.size(); i++)
		{
			int left = twotops[i].first;
			int right = twotops[i].second;
			int top = height[left] < height[right] ? height[left] : height[right];
			for (int j = left; j <= right; j++)
			{
				if (top > height[j])
				{
					total += (top - height[j]);
				}	
			}
		}
		return total;
	}
};

class SolutionLC44 {
public:
	bool isMatch(string s, string p) {
		int lens = s.size();
		int lenp = p.size();
		int i = 0, j = 0, lastp = -1, lasts = -1;
		
		while (i < lens)
		{
			if (s[i] == p[j] || p[j] == '?')
			{
				i++;
				j++;
				continue;
			}
			if (p[j] == '*')
			{
				lastp = j++;
				lasts = i;
				continue;
			}
			if (lastp != -1) //s[i] != p[j] backtrace
			{
				j = lastp + 1;
				i = ++lasts;
				continue;
			}
			return false;
		}
		while (j < lenp && p[j] == '*')
		{
			j++;
		}
		
		return j == lenp;
	}
};

class SolutionLC45 {//-1 初始 -2 不可到达
public:
	vector<int> record;
	int findminlen(int start, vector<int>& nums, int n){
		if (start >= n - 1) {
			return 0;
		}
		if (nums[start] == 0)
		{
			record[start] = -2;
			return -2;
		}	
		int min = -1;
		for (int i = 1; i <= nums[start]; i++) {
			int tmp = record[start + i] == -1 ? findminlen(start + i, nums, n) : record[start + i];
			if (min == -1)
			{
				min = tmp;
			}
			else
			{
				tmp == -2 ? min = min + 0 : (min == -2 ? min = tmp : (tmp < min ? min = tmp : tmp = tmp + 0));
			}
		}
		record[start] = (min == -2 ? min : min + 1);
		return record[start];
	}
	int jump(vector<int>& nums) {
		int len = nums.size();
		if (len == 0 || len == 1) {
			return 0;
		}
		vector<int>(len, -1).swap(record);
		record[len - 1] = 0;
		return findminlen(0, nums, len);
	}
};
//the sulotion above for lc45 is very foolish, do not use dp, use greed is better
class SolutionLC45case2 {
public:
	int jump(vector<int>& nums) {
		int curend = 0, farthest = 0, jumps = 0;
		for (int i = 0; i < nums.size() - 1; i++)
		{
			farthest = max(farthest, i + nums[i]);
			if (i == curend)
			{
				jumps++;
				curend = farthest;
			}
		}
		return jumps;
	}
};

const class my_nullptr_t
{
public:

	/* Return 0 for any class pointer */
	template<typename T>
	operator T*() const
	{
		return 0;
	}

	/* Return 0 for any member pointer */
	template<typename T, typename U>
	operator T U::*() const
	{
		return 0;
	}

	/* Safe boolean conversion */
	operator void*() const
	{
		return 0;
	}

private:

	/* Not allowed to get the address */
	void operator&() const;

} my_nullptr = {};

void fnullptrtest(void* p)
{
	return;
}
template <typename Ptr>
void tmpnullptrtest(Ptr ptr)
{
	return fnullptrtest(ptr);
}

class SolutionLC51 {
private:
	vector<vector<int>> chessboard;
	int nqueens;
	vector<vector<string>> solves;

public:
	void trytofill(int n)//n : no.n row
	{
		if (n == nqueens)
		{
			vector<string> vectmp;
			for (int i = 0; i < n; i++)
			{
				string tmp("");
				for (int j = 0; j < n; j++)
				{
					tmp += (chessboard[i][j] == 0 ? "." : "Q");
				}
				vectmp.push_back(tmp);
			}
			solves.push_back(vectmp);
			return;
		}

		for (int i = 0; i < nqueens; i++)//try to set a queen in this row
		{
			if (checkpos(n, i))
			{
				chessboard[n][i] = 1;
				trytofill(n + 1);
				chessboard[n][i] = 0;//try every position
			}
		}
		//no queen in this row
		return;
	}
	bool checkpos(int m, int n)
	{
		for (int i = 0; i < m; i++)
		{
			if (chessboard[i][n] == 1)
			{
				return false;
			}
		}
		for (int i = m, j = n; i >= 0 && j >= 0; i--, j--)
		{
			if (chessboard[i][j] == 1)
			{
				return false;
			}
		}
		for (int i = m, j = n; i >= 0 && j < nqueens; i--, j++)
		{
			if (chessboard[i][j] == 1)
			{
				return false;
			}
		}
		return true;
	}
	vector<vector<string>> solveNQueens(int n) 
	{
		vector<vector<int>>(n, vector<int>(n, 0)).swap(chessboard);
		nqueens = n;

		trytofill(0);
		return solves;
	}
};

//Leetcode 57
 struct Interval {
      int start;
      int end;
      Interval() : start(0), end(0) {}
      Interval(int s, int e) : start(s), end(e) {}
 };
 
class SolutionLC57 {
private:
	vector<Interval> *res;
public:
	int findornot(int start, int end, int x)
	{

	}
	vector<Interval> insert(vector<Interval>& intervals, Interval newInterval) {
		res = &intervals;
		int intvallen = intervals.size();
		int start = 0, end = intvallen - 1, mid = intvallen / 2;

	}
};