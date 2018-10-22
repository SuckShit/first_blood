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
#include "Impl.h"
//#include <boost/filesystem.hpp>
using namespace std;
using namespace placeholders;
//using boost::typeindex::type_id_with_cvr;
#pragma comment(lib,"Dbghelp.lib")

class BaseCs
{
public:
	virtual ~BaseCs() { cout << "Base destruction" << endl; }
};
class DeprivedCs:public BaseCs
{
public:
	~DeprivedCs() { cout << "Deprived destruction" << endl; }
};

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
T& refornottestfun(T&& x)
{
	auto at = forward<T>(x);
	return at;
}

template<typename T>
T& copycontest(T t)
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