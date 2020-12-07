#include <algorithm>
#include <boost/coroutine2/coroutine.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/asio/coroutine.hpp>
#include <boost/type_index.hpp>
#include <condition_variable>
#include <deque>
#include <exception>
#include <fstream>
#include <future>
#include <functional>
#include <iostream>
#include <iomanip>
#include <list>
#include <memory>
#include <queue>
#include <random>
#include <sstream>
#include <set>
#include <shared_mutex>
#include <stack>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <type_traits>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <variant>
#include "bredis.hpp"

using namespace std;

struct Test
{
	int a;
	int b;
	int c;
};
struct Base
{
	virtual void insert(Test t) = 0;
	virtual void print() = 0;
};
struct BaseCmp
{};
struct Test_Cmp_A : public BaseCmp
{
	bool operator()(const Test& l, const Test& r) const
	{
		return l.a < r.a;
	}
};
struct Test_Cmp_B
{
	bool operator()(const Test& l, const Test& r) const
	{
		return l.b < r.b;
	}
};
struct Test_Cmp_C
{
	bool operator()(const Test& l, const Test& r) const
	{
		return l.c < r.c;
	}
};

template<typename T>
struct TestQ : public Base
{
	std::set<Test, T> testq;
	void insert(Test t)
	{
		testq.insert(t);
	}
	void print()
	{
		for (const auto& i : testq)
		{
			std::cout << i.a << " " << i.b << " " << i.c << std::endl;
		}
	}
};

template <typename T>
class SortedQ
{
public:
	SortedQ(std::function<bool(const T&, const T&)> predicate)
		: pred_(std::move(predicate))
	{
		std::cout << "am i right?" << std::endl;
	}
	SortedQ()
	{
		pred_ = [](const T&, const T&) {return true; };
	}
	bool contains_element(T& element)
	{
		return std::binary_search(container_.begin(), container_.end(), element, pred_);
	}

	void insert(T& element)
	{
		if (!contains_element(element))
		{
			container_.insert(
				std::upper_bound(container_.begin(), container_.end(), element, pred_),
				element);
		}
	}

	void insert(T&& element)
	{
		insert(element);
	}

	typename std::list<T>::iterator erase(typename std::list<T>::iterator where)
	{
		return container_.erase(where);
	}

	typename std::list<T>::iterator find(T& element)
	{
		typename std::list<T>::iterator it = std::lower_bound(container_.begin(), container_.end(), element, pred_);
		if (it != container_.end() && !pred_(element, *it))
		{
			return it;
		}
		return container_.end();
	}

	void set_predicate(std::function<bool(const T&, const T&)> new_predicate)
	{
		pred_ = new_predicate;
		std::sort(container_.begin(), container_.end(), pred_);
	}

	void reserve(size_t new_capacity)
	{
		container_.reserve(new_capacity);
	}

	typename std::list<T>::iterator begin()
	{
		return container_.begin();
	}

	typename std::list<T>::iterator end()
	{
		return container_.end();
	}

private:
	std::function<bool(const T&, const T&)> pred_;
	std::list<T> container_;
};

struct Waiting_Member
{
	std::string uuid;
	int score;
	int what;
	Waiting_Member() :
		uuid(""),
		score(0),
		what(0)
	{}
};
struct Waiting_Member_Cmp
{
	bool operator()(const Waiting_Member& l, const Waiting_Member& r)
	{
		return l.score < r.score || l.score == r.score && l.uuid < r.uuid;
	}
};
struct Waiting_Member_Cmp2
{
	bool operator()(const Waiting_Member& l, const Waiting_Member& r)
	{
		return l.what < r.what || l.what == r.what && l.uuid < r.uuid;
	}
};

template <typename T>
class TestClass
{
public:
	void set_predicate(std::function<bool(const T&, const T&)> new_predicate)
	{
		pred_ = new_predicate;
		container_.sort(pred_);
	}
	std::function<bool(const T&, const T&)> pred_;
	std::list<T> container_;
};

boost::asio::coroutine c1;
boost::asio::coroutine c2;

void boo()
{
	BOOST_ASIO_CORO_REENTER(c1)
	{
		BOOST_ASIO_CORO_YIELD

			std::cout << "!!! " << std::endl;

		BOOST_ASIO_CORO_YIELD

			std::cout << "??? " << std::endl;

	}
}
void coo()
{
	BOOST_ASIO_CORO_REENTER(c2)
	{
		BOOST_ASIO_CORO_YIELD

			std::cout << "~~~ " << std::endl;

		BOOST_ASIO_CORO_YIELD

			std::cout << "... " << std::endl;

	}
}

void boo2()
{
//reenter
	switch (::boost::asio::detail::coroutine_ref _coro_value = c1) 
	case -1: 
		if (_coro_value) 
		{ 
			goto terminate_coroutine; 
terminate_coroutine: 
			_coro_value = -1; 
			goto bail_out_of_coroutine; 
bail_out_of_coroutine: 
			break; 
		} 
		else 
	case 0:
//reenter end
	{
//yield

		for (_coro_value = (0 + 1);;) 
	if (_coro_value == 0) 
	{ 
	case (0 + 1): ; 
		break;
	} 
	else 
		switch (_coro_value ? 0 : 1) 
	for (;;) 
	case -1: 
	if (_coro_value) 
	goto terminate_coroutine; 
	else 
	for (;;) 
	case 1: 
	if (_coro_value) 
	goto bail_out_of_coroutine; 
	else 
	case 0:
//yield end
//my code
		std::cout << "!!! " << std::endl;
//my code end
//yield2
		for (_coro_value = (1 + 1);;) 
	if (_coro_value == 0) 
	{ 
	case (1 + 1): ; 
		break; 
	} 
	else 
		switch (_coro_value ? 0 : 1) 
	for (;;) 
	case -1: 
	if (_coro_value) 
	goto terminate_coroutine; 
	else 
	for (;;) 
	case 1: 
	if (_coro_value) 
	goto bail_out_of_coroutine; 
	else 
	case 0:
//yield2 end
//my code
		std::cout << "??? " << std::endl;
//my code end
	}
}
void coo2()
{
	switch (::boost::asio::detail::coroutine_ref _coro_value = c2) 
	case -1: 
		if (_coro_value) 
		{ 
			goto terminate_coroutine; 
terminate_coroutine: 
			_coro_value = -1; 
			goto bail_out_of_coroutine; 
bail_out_of_coroutine: 
			break; 
		} 
		else 
	case 0:
	{
		for (_coro_value = (2 + 1);;) 
			if (_coro_value == 0) 
			{ 
	case (2 + 1): 
		;
		break; 
			} 
			else 
				switch (_coro_value ? 0 : 1) 
				for (;;) 
				case -1: 
				if (_coro_value) 
				goto terminate_coroutine; 
				else 
				for (;;) 
				case 1: 
				if (_coro_value) 
				goto bail_out_of_coroutine;
				else 
				case 0:

			std::cout << "~~~ " << std::endl;

		for (_coro_value = (3 + 1);;) 
		if (_coro_value == 0) 
		{ 
			case (3 + 1): 
				; 
				break; 
		} 
		else 
			switch (_coro_value ? 0 : 1) 
			for (;;) 
			case -1: 
			if (_coro_value) 
			goto terminate_coroutine; 
			else 
			for (;;) 
			case 1: 
			if (_coro_value) 
			goto bail_out_of_coroutine; 
			else 
			case 0:

			std::cout << "... " << std::endl;

	}
}
class BaseC
{
public:
	int x;
private:
	virtual void fun()
	{
		std::cout << "in base" << std::endl;
	}
};
class Derived : public BaseC
{

};

using Fc = void(*)(int x);

void ff(Fc fc)
{
	for (int i = 0; i < 3; i++)
	{
		fc(i);
	}
}

template <typename T>
struct remove_ref
{
	remove_ref() { std::cout << "remove_ref" << std::endl; }
	using type = T;
};
template <typename T>
struct remove_ref<T&>
{
	remove_ref() { std::cout << "remove_ref &" << std::endl; }
	using type = T;
};
template <typename T>
struct remove_ref<T&&>
{
	remove_ref() { std::cout << "remove_ref &&" << std::endl; }
	using type = T;
};
template <class T>
using remove_ref_t = typename remove_ref<T>::type;

//template<typename T>
class Resource
{
public:
	Resource() : size(5),
		pull(std::bind(&Resource::put, this, std::placeholders::_1))
	{}
	void put(boost::coroutines2::coroutine<Resource*>::push_type& push)
	{
		while (true)
		{
			if (this->res.size() + 1 > this->size)
			{
				push(this);
			}
			res.emplace_back(3);
			std::cout << "push 3" << std::endl;
		}
	}

	void get()
	{
		while (true)
		{
			if (this->res.empty())
			{
				pull();
			}
			res.pop_front();
			std::cout << "pop 3" << std::endl;
		}
	}
	size_t len()
	{
		return res.size();
	}
private:
	std::list<int> res;
	int size;
	boost::coroutines2::coroutine<Resource*>::pull_type pull;
};

struct FuckingWorld
{
	int operator()(int x)
	{
		return x;
	}
};

struct Vector2D
{
	int x;
	int y;
	Vector2D(int a, int b):x(a), y(b){}
	Vector2D():x(0),y(0){}
	Vector2D(const Vector2D& vec)
	{
		this->x = vec.x;
		this->y = vec.y;
	}
	Vector2D& operator= (const Vector2D& vec)
	{
		this->x = vec.x;
		this->y = vec.y;
		return *this;
	}
	Vector2D& operator+= (const Vector2D& vector)
	{
		this->x += vector.x;
		this->y += vector.y;
		return *this;
	}
};

template <typename Sig, typename S, typename T>
class Nested;

template <typename U, typename V, typename T, typename... Args>
class Nested<T(Args...), U, V>
{};

template <typename Ty>
class Nested0
{};

template <typename Ty, typename Container>
class Nested1
{};

template <typename Ty1, typename Ty2, template<typename> class Container>
class Nested2
{
public:
	Ty1 t1;
	Container<Ty2> t2;
};

template <typename Ty, template<typename> class Container>
class Nested3
{};

template <template<typename, typename> class ContainerO, typename Ty, template<typename> class ContainerI>
class Nested4
{
public:
	ContainerO<Ty, ContainerI<Ty>> s;
};

template <typename Ty>
void new2Darr(int m, int n, Ty*** p)
{
	 *p = new Ty* [m];
	for (int i = 0; i < m; i++)
	{
		(*p)[i] = new Ty[n];
		for (int j = 0; j < n; j++)
		{
			(*p)[i][j] = (i + 1) * (j + 1);
		}
	}
}

template <typename Ty>
void delete2Darr(int m, Ty*** p)
{
	for (int i = 0; i < m; i++)
	{
		delete[] (*p)[i];
	}
	delete[] (*p);
}

template <typename>
class mytask;

template <typename R, typename... Args>
class mytask<R(Args...)>
{
private:
	std::promise<R> pr_;
	std::function<R(Args...)> fn_;
public:
	template <typename... Ts>
	explicit mytask(Ts... ts) : fn_(std::forward<Ts>(ts)...) {}
	mytask(mytask&& m) noexcept : pr_(std::move(m.pr_)), fn_(std::move(m.fn_)) {}
	mytask& operator= (mytask&& m) noexcept
	{
		pr_ = std::move(m.pr_);
		fn_ = std::move(m.fn_);
		return *this;
	}
//	template <typename... Ts>
	void operator()(Args... args)
	{
		pr_.set_value(fn_(std::forward<Args>(args)...));
	}
	std::future<R> get_future()
	{
		return pr_.get_future();
	}
	mytask(const mytask& m) = delete;
	mytask& operator= (const mytask& m) = delete;
};

struct XXX
{
	void F() noexcept {}
};

template <class _Ret, class... _Types>
class func_class
{
public:
	func_class()
	{
		std::cout << "wtf in func_class" << std::endl;
	}
	template <class _Fx, class _Function>
	using _Enable_if_callable_t =
		std::enable_if_t<std::conjunction_v<std::negation<std::is_same<std::decay_t<_Fx>, _Function>>, std::_Is_invocable_r<_Ret, _Fx, _Types...>>,
		int>;
};

template <class>
struct get_function_impl;

template <class _Ret, class... _Types>                                                        
struct get_function_impl<_Ret(_Types...)>                                            
{																							  
    using type = func_class<_Ret, _Types...>;                                                 
};

template <class _Fty>
class myfunction : public get_function_impl<_Fty>::type
{
public:
	template <class _Fx, typename get_function_impl<_Fty>::type::template _Enable_if_callable_t<_Fx&, myfunction> = 0>
	myfunction(_Fx _Func) 
	{
		std::cout << "WTF in myfunction" << std::endl;
	}
};

auto func2(char) -> int (*)()
{
	return nullptr;
}

namespace dkw 
{
	template <class>
	constexpr bool is_reference_wrapper_v = false;
	template <class U>
	constexpr bool is_reference_wrapper_v<std::reference_wrapper<U>> = true;

	template <class T, class Type, class T1, class... Args>
	constexpr decltype(auto) INVOKE(Type T::* f, T1&& t1, Args&&... args)
	{
		if constexpr (std::is_member_function_pointer_v<decltype(f)>) 
		{
			if constexpr (std::is_base_of_v<T, std::decay_t<T1>>)
				return (std::forward<T1>(t1).*f)(std::forward<Args>(args)...);
			else if constexpr (is_reference_wrapper_v<std::decay_t<T1>>)
				return (t1.get().*f)(std::forward<Args>(args)...);
			else
				return ((*std::forward<T1>(t1)).*f)(std::forward<Args>(args)...);
		}
		else 
		{
			static_assert(std::is_member_object_pointer_v<decltype(f)>);
			static_assert(sizeof...(args) == 0);
			if constexpr (std::is_base_of_v<T, std::decay_t<T1>>)
				return std::forward<T1>(t1).*f;
			else if constexpr (is_reference_wrapper_v<std::decay_t<T1>>)
				return t1.get().*f;
			else
				return (*std::forward<T1>(t1)).*f;
		}
	}

	template <class F, class... Args>
	decltype(auto) INVOKE(F&& f, Args&&... args)
	{
		return std::forward<F>(f)(std::forward<Args>(args)...);
	}
}

//template< class F, class... Args>
//constexpr std::invoke_result_t<F, Args...> dkwinvoke(F&& f, Args&&... args)
//noexcept(std::is_nothrow_invocable_v<F, Args...>)
//{
//	return dkw::INVOKE(std::forward<F>(f), std::forward<Args>(args)...);
//}

void throwfun()
{
	std::exception stde("what the xxxx?!");
	throw stde;
}

class BufferXmlReader
{
public:
	const char*			_buf;		///< 缓冲区
	size_t              _len;    	///< 缓冲区长度
	size_t              _pos;		///< 当前位置

public:

	BufferXmlReader() :_buf(nullptr), _len(0), _pos(0) {}

	void reset() { _pos = 0; }

	void setBuffer(const char* buf, size_t len)
	{
		_buf = buf;
		_len = len;
		_pos = 0;
	}

	void setBuffer(const std::vector<char>& buf)
	{
		_buf = &buf[0];
		_len = buf.size();
		_pos = 0;
	}

	bool hasEnd()
	{
		return (_pos >= _len || *(_buf + _pos) == 0);
	}

	bool expect(char ch)
	{
		if (get() == ch)
		{
			_pos++;
			return true;
		}
		return false;
	}

	void skip(size_t i = 1)
	{
		_pos += i;
	}

	char read()
	{
		check(++_pos);
		return *(_buf + _pos - 1);
	}

	char get(size_t pos = 0)
	{
		check(pos + _pos);
		return *(_buf + _pos + pos);
	}

	size_t pos()
	{
		return _pos;
	}

	void check(size_t pos)
	{
		if (pos > _len)
		{
			char s[64];
			snprintf(s, sizeof(s), "buffer[%u] overflow when peekBuf, over %u.", (uint32_t)pos, (uint32_t)_len);
			throw std::exception(s);
		}
	}
};

void A1()
{
	std::cout << "in A1" << std::endl;
}

void A2()
{
	std::cout << "in A2" << std::endl;
}

void B1()
{
	std::cout << "in B1" << std::endl;
}

void B2()
{
	std::cout << "in B2" << std::endl;
}

class ConandOp
{
public:
	ConandOp() : val(10)
	{
		std::cout << "in constructor" << std::endl;
	}
	int operator()()
	{
		std::cout << "in operator()" << std::endl;
		return val;
	}
	operator int()
	{
		std::cout << "in operator int()" << std::endl;
		return val;
	}
private:
	int val;
};

const class my_nullptr_t
{
public:

	/* Return 0 for any class pointer */
	template<typename T>
	operator T* () const
	{
		std::cout << "T*" << std::endl;
		return 0;
	}

	/* Return 0 for any member pointer */
	template<typename T, typename U>
	operator T U::* () const
	{
		std::cout << "T U::*" << std::endl;
		return 0;
	}

	/* Safe boolean conversion */
	operator void* () const
	{
		std::cout << "void*" << std::endl;
		return 0;
	}

private:

	/* Not allowed to get the address */
	void operator&() const;

} my_nullptr = {};

class null_ptr_test
{
public:
	ConandOp* p;
	std::function<void()> f;
};

template <typename T>
class null_ptr_test2
{
public:
	T* p;
};

void thdf(int& x)
{
	std::cout << "x what" << std::endl;
	return;
}

struct Func
{
	std::function<void()>   _func;
};

template <class F, class... Args>
std::shared_ptr<Func> create(F&& f, Args&&... args)
{
	using RetType = decltype(f(args...));

	auto task = std::make_shared<std::packaged_task<RetType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

	std::shared_ptr<Func> fPtr = std::make_shared<Func>();

	fPtr->_func = [task]() {
		(*task)();
	};

	return fPtr;
}

template<typename T> void print_queue(T& q) 
{
	while (!q.empty()) 
	{
		std::cout << q.top() << " ";
		q.pop();
	}
	std::cout << '\n';
}

class Outer
{
public:
	class public_inner
	{
	private:
		int inner_int;
	};
private:
	class private_inner
	{
	private:
		int inner_int;
	};
public:
	friend class friend_inner;

	class friend_inner
	{
	public:
		friend_inner() = default;
		friend_inner(Outer* o)
		{
			o->out_int = 10;
			friend_inner_int = 0;
		}
	private:
		int friend_inner_int;
	};
public:
	void set()
	{
		//pbi->inner_int = 11;
		//pri->inner_int = 12;
		//fri->friend_inner_int = 13;
	}
private:
	int out_int;
	public_inner* pbi;
	private_inner* pri;
	friend_inner* fri;
};

class InsertSolution
{
private:
	std::vector<std::pair<int, int>> Ints;
	std::vector<std::pair<int, int>> Res;
public:
	void solve(std::pair<int, int> oth)
	{
		auto it = Ints.begin();
		while (it != Ints.end())
		{
			if (oth.first > it->second)
			{
				Res.push_back(*it);
				it++;
			}
			else
			{
				break;
			}
		}
		while (it != Ints.end())
		{
			if (oth.second > it->first)
			{
				oth.first = std::min<int>(oth.first, it->first);
				oth.second = std::max<int>(oth.second, it->second);
				it++;
			}
			else
			{
				break;
			}
		}
		Res.push_back(oth);
		while (it != Ints.end())
		{
			Res.push_back(*it);
		}
	}
};

int shotballoon(const int mcolors, const std::vector<int>& shotrslt)
{
	int nshots = shotrslt.size();
	int cnt = mcolors;
	std::vector<int> bucket(mcolors + 1, 0);
	int begin = 0, end = 0;
	int len = INT_MAX;
	while (end < nshots)
	{
		if (shotrslt[end] == 0)
		{
			end++;
			begin = end;
			cnt = mcolors;
			bucket.clear();
			continue;
		}
		if (bucket[shotrslt[end]] == 0)
		{
			cnt--;
		}
		bucket[shotrslt[end]]++;
		if (cnt != 0)
		{
			end++;
		}
		while (cnt == 0 && begin < end)
		{
			if (--bucket[shotrslt[begin]] > 0)
			{
				begin++;
			}
			else
			{
				len = end - begin + 1 < len ? end - begin + 1 : len;
				begin = ++end;
				cnt = mcolors;
				bucket.clear();
			}
		}
	}
	return len;
}

bool dpInterleave(const std::string& s1, const std::string& s2, const std::string& s3)
{
	if (s1.size() + s2.size() != s3.size())
	{
		return false;
	}
	std::vector<std::vector<bool>> dp(s1.size() + 1, std::vector<bool>(s2.size() + 1, false));
	for (int i = 0; i < s1.size() + 1; i++)
	{
		for (int j = 0; j < s2.size() + 1; j++)
		{
			if (i == 0 && j == 0)
			{
				dp[i][j] = true;
			}
			else if (i == 0)
			{
				dp[i][j] = s2[j] == s3[j] ? dp[i][j - 1] : false;
			}
			else if (j == 0)
			{
				dp[i][j] = s1[i] == s3[i] ? dp[i - 1][j] : false;
			}
			else
			{
				dp[i][j] = dp[i - 1][j] && s1[i - 1] == s3[i + j - 1] || dp[i][j - 1] && s2[j - 1] == s3[i + j - 1];
			}
		}
	}
	return dp[s1.size()][s2.size()];
}

int numofDPdistinctSubSeq(const std::string& s1, const std::string& s2)
{
	int l1 = s1.length();
	int l2 = s2.length();
	std::vector<std::vector<int>> dp(l1 + 1, std::vector<int>(l2 + 1, 0));

	for (int i = 0; i <= l1; i++)
	{
		dp[i][0] = 1;
	}

	for (int i = 1; i <= l1; i++)
	{
		for (int j = 1; j <= l2; j++)
		{
			if (s1[i - 1] == s2[j - 1])
			{
				dp[i][j] = dp[i - 1][j - 1] + dp[i - 1][j];
			}
			else
			{
				dp[i][j] = dp[i - 1][j];
			}
		}
	}

	for (int i = 0; i <= l1 + 1; i++)
	{
		for (int j = 0; j <= l2 + 1; j++)
		{
			if (i == 0 && j == 0)
			{
				std::cout << std::setw(3) << "\\";
			}
			else if (i == 0 && j== 1 || i == 1 && j == 0)
			{
				std::cout << std::setw(3) << 0;
			}
			else if (i == 0)
			{
				std::cout << std::setw(3) << s2[j - 2];
			}
			else if (j == 0)
			{
				std::cout << std::setw(3) << s1[i - 2];
			}
			else
			{
				std::cout << std::setw(3) << dp[i - 1][j - 1];
			}
		}
		std::cout << std::endl;
	}

	return dp[l1][l2];
}

struct TreeNode {
	int val;
	bool isempty;
	TreeNode* left;
	TreeNode* right;
	TreeNode(int x) : val(x), left(nullptr), right(nullptr), isempty(false) {}
	TreeNode(void* p = nullptr) : isempty(true), val(0), left(nullptr), right(nullptr) {}
	operator bool() { return isempty; }
};

void constructTree(TreeNode** root, std::vector<TreeNode>& vec_tn)
{
	int i = 0;
	int sz = vec_tn.size();
	std::queue<TreeNode*> tn_list;
	if (sz)
	{
		*root = new TreeNode(vec_tn[i]);
		tn_list.push(*root);
	} 
	else
	{
		*root = nullptr;
		return;
	}
	while (!tn_list.empty())
	{
		TreeNode* tmp = tn_list.front();
		tn_list.pop();
		if (++i < sz)
		{
			if (!vec_tn[i])
			{
				tmp->left = new TreeNode(vec_tn[i].val);
				tn_list.push(tmp->left);
			}
		}
		if (++i < sz)
		{
			if (!vec_tn[i])
			{
				tmp->right = new TreeNode(vec_tn[i].val);
				tn_list.push(tmp->right);
			}
		}
	}
}
void destructTree(TreeNode* root)
{
	if (root)
	{
		destructTree(root->left);
		destructTree(root->right);
		delete root;
		root = nullptr;
	}
}

class SolutionLC124 {
private:
	int maxsum;
public:
	SolutionLC124() :maxsum(INT_MIN) {}
	int maxPathSum(TreeNode* root) {
		maxPathSumHelper(root);
		return maxsum;
	}
	int maxPathSumHelper(TreeNode* root)
	{
		if (!root)
		{
			return 0;
		}
		int left = maxPathSumHelper(root->left);
		int right = maxPathSumHelper(root->right);
		maxsum = std::max<int>(std::max<int>(0, left) + std::max<int>(0, right) + root->val, maxsum);
		return std::max<int>(std::max<int>(left, right), 0) + root->val;
	}
};

std::vector<std::vector<std::string>> findLadders(const std::string& beginWord, const std::string& endWord, std::unordered_set<std::string>& wordList) {
	//very interesting problem
	//It can be solved with standard BFS. The tricky idea is doing BFS of paths instead of words!
	//Then the queue becomes a queue of paths.
	std::vector<std::vector<std::string>> ans;
	std::queue<std::vector<std::string>> paths;
	paths.push({ beginWord });
	int level = 1;
	int minLevel = INT_MAX;

	//"visited" records all the visited nodes on this level
	//these words will never be visited again after this level 
	//and should be removed from wordList. This is guaranteed
	// by the shortest path.
	std::unordered_set<std::string> visited;

	while (!paths.empty()) {
		std::vector<std::string> path = paths.front();
		paths.pop();
		if (path.size() > level) {
			//reach a new level
			for (std::string w : visited) wordList.erase(w);
			visited.clear();
			if (path.size() > minLevel)
				break;
			else
				level = path.size();
		}
		std::string last = path.back();
		//find next words in wordList by changing
		//each element from 'a' to 'z'
		for (int i = 0; i < last.size(); ++i) {
			std::string news = last;
			for (char c = 'a'; c <= 'z'; ++c) {
				news[i] = c;
				if (wordList.find(news) != wordList.end()) {
					//next word is in wordList
					//append this word to path
					//path will be reused in the loop
					//so copy a new path
					std::vector<std::string> newpath = path;
					newpath.push_back(news);
					visited.insert(news);
					if (news == endWord) {
						minLevel = level;
						ans.push_back(newpath);
					}
					else
						paths.push(newpath);
				}
			}
		}
	}
	return ans;
}

int longestConsecutiveSequence(const std::vector<int>& nums)
{
	std::unordered_map<int, int> seq_num;
	int max = 0;

	for (const auto& n : nums)
	{
		if (seq_num.find(n) != seq_num.end())
		{
			continue;
		}
		else if (seq_num.find(n - 1) == seq_num.end() && seq_num.find(n + 1) == seq_num.end())
		{
			seq_num[n] = 1;
		}
		else if (seq_num.find(n - 1) == seq_num.end())
		{
			seq_num[n] = seq_num[n + 1] + 1;
			seq_num[n + seq_num[n + 1]] = seq_num[n];
		}
		else if (seq_num.find(n + 1) == seq_num.end())
		{
			seq_num[n] = seq_num[n - 1] + 1;
			seq_num[n - seq_num[n - 1]] = seq_num[n];
		}
		else
		{
			seq_num[n] = seq_num[n - 1] + seq_num[n + 1] + 1;
			seq_num[n + seq_num[n + 1]] = seq_num[n];
			seq_num[n - seq_num[n - 1]] = seq_num[n];
		}
		max = std::max<int>(max, seq_num[n]);
	}
	return max;
}

std::vector<std::vector<std::string>> palindrome(const std::string& s)
{
	int n = s.size();
	std::vector<std::vector<std::vector<std::string>>> result(n + 1);
	result[0].push_back({});
	std::vector<std::vector<bool>> dp(n, std::vector<bool>(n));
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j <= i; j++)
		{
			if (s[i] == s[j] && (i - j <= 2 || dp[j + 1][i - 1]))
			{
				dp[j][i] = true;
				auto str = s.substr(j, i - j + 1);
				for (auto l : result[j])
				{
					l.push_back(str);
					result[i + 1].push_back(l);
				}
			}
		}
	}
	return result[n];
}

class SolutionLC131
{
public:
	bool isPalindrome(const std::string& s)
	{
		int begin = 0;
		int end = s.length();
		while (begin < end)
		{
			if (s[begin++] != s[end--])
			{
				return false;
			}
		}
		return true;
	}
	std::vector<std::vector<std::string>> findsubPalindrome(const std::string& s)
	{
		std::vector<std::vector<std::string>> ret;
		std::vector<std::string> path;
		if (s.empty())
		{
			return ret;
		}
		dfs(0, s, path, ret);
		return ret;
	}
	void dfs(int index, const std::string& s, std::vector<std::string>& path, std::vector<std::vector<std::string>>& ret)
	{
		if (index == s.length())
		{
			ret.push_back(path);
			return;
		}
		for (int i = index; i < s.length(); i++)
		{
			std::string tmp = s.substr(index, i - index + 1);
			if (isPalindrome(tmp))
			{
				path.push_back(tmp);
				dfs(i + 1, s, path, ret);
				path.pop_back();
			}
		}
	}
};

/*
Given a string s, partition s such that every substring of the partition is a palindrome.

Return the minimum cuts needed for a palindrome partitioning of s.

Example:

Input: "aab"
Output: 1
Explanation: The palindrome partitioning ["aa","b"] could be produced using 1 cut.
*/

class SolutionLC132
{
public:
	int minPatitionOfPalindrome(const std::string& s)
	{
		int l = s.length();
		std::vector<int> cut(l + 1, 0);
		for (int i = 0; i <= l; i++)
		{
			cut[i] = i - 1;
		}
		for (int i = 0; i < l; i++)
		{
			for (int j = 0; i + j < l && i - j >= 0 && s[i + j] == s[i - j]; j++)
			{
				cut[i + j + 1] = std::min<int>(cut[i + j + 1], cut[i - j] + 1);
			}
			for (int j = 0; i + j < l && i - 1 - j >= 0 && s[i + j] == s[i - 1 - j]; j++)
			{
				cut[i + j + 1] = std::min<int>(cut[i + j + 1], cut[i - j - 1] + 1);
			}				
		}
		return cut[l];
	}
};

class SolutionLC135
{
public:
	int Candy(const std::vector<int>& rates)
	{
		int len = rates.size();
		if (len <= 1)
			return len;
		std::vector<int> nums(len, 1);

		for (int i = 0; i < len - 1; i++)
		{
			if (rates[i + 1] > rates[i])
			{
				nums[i + 1] = nums[i] + 1;
			}
		}
		for (int j = len - 1; j > 0; j--)
		{
			if (rates[j - 1] > rates[j])
			{
				nums[j - 1] = std::max<int>(nums[j - 1], nums[j] + 1);
			}
		}

		int ret = 0;
		for (int i = 0; i < len; i++)
		{
			ret += nums[i];
		}

		return ret;
	}
};

struct foos { int x; int y[]; };
struct bars { struct foos z; };

#pragma pack(1)
typedef struct {
	int a;
	char b[10];
	char* vector;
} cq;
#pragma pack()

// Definition for a Node.
class Node {
public:
	int val;
	Node* next;
	Node* random;

	Node(int _val) {
		val = _val;
		next = NULL;
		random = NULL;
	}
};

class Solution {
public:
	Node* copyRandomList(Node* head) {
		std::map<Node*, Node*> listmap;
		Node* p = head;
		Node* pre = nullptr;
		while (p) {
			Node* q = new Node(p->val);
			p = p->next;
			listmap.insert(std::make_pair(p, q));
			if (pre) {
				pre->next = q;
			}
			pre = q;
		}
		p = head;
		while (p) {
			listmap[p]->random = listmap[p->random];
		}
	}
};

class SolutionLC140 {
public:
	std::unordered_map<std::string, std::vector<std::string>> m;
	std::vector<std::string> combine(std::string word, std::vector<std::string> res) {
		for (auto& i : res)
		{
			i = i + " " + word;
		}
		return res;
	}
	std::vector<std::string> wordBreak(std::string s, std::unordered_set<std::string>& wordDict) {
		if (m.count(s))
		{
			return m[s];
		}
		std::vector<std::string> result;
		if (wordDict.count(s))
		{
			result.push_back(s);
		}
		for (int i = 1; i < s.size(); i++)
		{
			std::string word = s.substr(i);
			if (wordDict.count(word))
			{
				std::vector<std::string> tmp = combine(word, wordBreak(s.substr(0, i), wordDict));
				result.insert(result.end(), tmp.begin(), tmp.end());
			}
		}
		m[s] = result;
		return result;
	}
	std::vector<std::string> wordBreak(std::string s, std::vector<std::string>& wordDict) {
		std::unordered_set<std::string> wd(wordDict.begin(), wordDict.end());
		return wordBreak(s, wd);
	}
};

class SolutionLC140_2
{
public:
	void dfs(int index, const std::string& s, std::vector<std::string>& path, std::vector<std::vector<std::string>>& ret, const std::unordered_set<std::string>& wd)
	{
		if (index == s.length())
		{
			ret.push_back(path);
			return;
		}
		for (int i = index; i < s.length(); i++)
		{
			std::string tmp = s.substr(index, i - index + 1);
			if (wd.count(tmp))
			{
				path.push_back(tmp);
				dfs(i + 1, s, path, ret, wd);
				path.pop_back();
			}
		}
	}
	std::vector<std::string> wordBreak(const std::string& s, const std::vector<std::string>& wordDict) {
		std::vector<std::string> path;
		std::vector<std::vector<std::string>> ret;
		std::unordered_set<std::string> wd(wordDict.begin(), wordDict.end());
		dfs(0, s, path, ret, wd);
		std::vector<std::string> result;
		for (const auto& i : ret)
		{
			std::string tmp;
			for (const auto& j : i)
			{
				if (!tmp.empty())
				{
					tmp += " ";
				}
				tmp += j;
			}
			result.push_back(tmp);
		}
		return result;
	}
};

class ListNode {
public:
	int key;
	int val;
	ListNode* next;
	ListNode* prior;

	ListNode(int _key, int _val) {
		key = _key;
		val = _val;
		next = NULL;
		prior = NULL;
	}
};

class ListHead {
public:
	ListNode* head;
	ListNode* tail;
	ListHead() :
		head (nullptr),
		tail (nullptr)
	{}
};

class LRUCache {
public:
	LRUCache(int c) : 
		capacity (c > 0 ? c : 1),
		hp (nullptr)
	{}

	int get(int key) {
		if (!hashmap.count(key)) {
			return -1;
		}
		ListNode* p = hashmap[key];
		if (p != hp->head) {
			p->prior->next = p->next;
			if (p->next)
			{
				p->next->prior = p->prior;
			}
			p->next = hp->head;
			hp->head = p;
			p->prior = nullptr;
		}
		return p->val;
	}

	void put(int key, int value) {
		if (!hashmap.count(key)) {
			if (hashmap.size() == capacity) {
				hashmap.erase(hp->tail->key);
				if (hp->tail->prior) {
					hp->tail->prior->next = nullptr;
				}
				ListNode* p = hp->tail;
				hp->tail = hp->tail->prior;
				delete(p);
				p = nullptr;
			}
			ListNode* q = new ListNode(key, value);
			if (!hp->head) {
				hp->head = q;
				hp->tail = q;
				q->next = nullptr;
				q->prior = nullptr;
				return;
			}
			hp->head->prior = q;
			q->prior = nullptr;
			q->next = hp->head;
			hp->head = q;
			hashmap[key] = q;
		}
		else {
			ListNode* p = hashmap[key];
			if (p == hp->head) {
				p->val = value;
				return;
			}
			p->prior->next = p->next;
			if (p->next)
			{
				p->next->prior = p->prior;
			}
			p->next = hp->head;
			hp->head = p;
			p->prior = nullptr;
		}
	}
private:
	int capacity;
	ListHead* hp;
	std::unordered_map<int, ListNode*> hashmap;
};

class SolutionLC164 {
public:
	int maximumGap(const std::vector<int>& nums) {
		size_t sz = nums.size();
		if (sz <= 1) return 0;

		int min = INT_MAX;
		int max = INT_MIN;
		for (const auto& num : nums) {
			min = std::min<int>(min, num);
			max = std::max<int>(max, num);
		}
		int gap = std::ceil((double)(max - min) / (sz - 1));
		if (gap == 0) {
			gap = 1;
		}
		std::vector<int> max_bucket(sz, INT_MIN);
		std::vector<int> min_bucket(sz, INT_MAX);
		for (int i = 0; i < sz; i++) {
			int index = (nums[i] - min) / gap;
			max_bucket[index] = std::max<int>(max_bucket[index], nums[i]);
			min_bucket[index] = std::min<int>(min_bucket[index], nums[i]);
		}
		int prior = min;
		int maxres = INT_MIN;
		for (int i = 0; i < sz; i++) {
			if (min_bucket[i] == INT_MAX && max_bucket[i] == INT_MIN) continue;
			maxres = std::max<int>(min_bucket[i] - prior, maxres);
			prior = max_bucket[i];
		}
		return maxres;
	}
};

class SolutionLC174 {
public:
	int calculateMinimumHP(const std::vector<std::vector<int>>& dungeon) {
		size_t rows = dungeon.size();
		size_t cols = dungeon[0].size();
		if (cols == 1 && rows == 1)
		{
			return dungeon[0][0] < 0 ? 1 - dungeon[0][0] : 1;
		}
		std::vector<std::vector<int>> dp(rows, std::vector<int>(cols, 0));
		for (int i = cols - 1; i >= 0; i--) {
			if (i == cols - 1) {
				dp[rows - 1][i] = dungeon[rows - 1][i] < 0 ? 1 - dungeon[rows - 1][i] : 1;
			}
			else {
				dp[rows - 1][i] = dp[rows - 1][i + 1] - dungeon[rows - 1][i] > 0 ? dp[rows - 1][i + 1] - dungeon[rows - 1][i] : 1;
			}
		}
		for (int i = rows - 1; i >= 0; i--) {
			if (i == rows - 1) {
				dp[i][cols - 1] = dungeon[i][cols - 1] < 0 ? 1 - dungeon[i][cols - 1] : 1;
			}
			else {
				dp[i][cols - 1] = dp[i + 1][cols - 1] - dungeon[i][cols - 1] > 0 ? dp[i + 1][cols - 1] - dungeon[i][cols - 1] : 1;
			}
		}
		if (rows >= 2 && cols >= 2) {
			for (int i = rows - 2; i >= 0; i--) {
				for (int j = cols - 2; j >= 0; j--) {
					dp[i][j] = std::min<int>(dp[i + 1][j] - dungeon[i][j] > 0 ? dp[i + 1][j] - dungeon[i][j] : 1, 
						dp[i][j + 1] - dungeon[i][j] > 0 ? dp[i][j + 1] - dungeon[i][j] : 1);
				}
			}
		}
		
		return dp[0][0];
	}
};

class SolutionLC188 {
public:
	int maxProfit(int k, const std::vector<int>& prices) {
		size_t sz = prices.size();
		if (sz <= 1) {
			return 0;
		}
		if (k >= sz / 2) {
			int maxProf = 0;
			for (int i = 1; i < sz; i++) {
				if (prices[i] > prices[i - 1])
					maxProf += prices[i] - prices[i - 1];
			}
			return maxProf;
		}
		std::vector<std::vector<int>> dp(k + 1, std::vector<int>(sz, 0));
		//dp[k][i] = std::max<int>(dp[k][i - 1], price[i] - price[j] + dp[k - 1][j - 1]);
		for (int kk = 1; kk <= k; kk++) {
			int min = prices[0];
			for (int i = 1; i < sz; i++) {
				min = std::min<int>(min, prices[i] - dp[kk - 1][i - 1]);
				dp[kk][i] = std::max<int>(dp[kk][i - 1], prices[i] - min);
			}
		}
		return dp[k][sz - 1];
	}
};

class SolutionLC212 {
public:
	class TrieNode {
	public:
		bool is_end;
		std::vector<TrieNode*> children;
		TrieNode() {
			is_end = false;
			children = std::vector<TrieNode*>(26, NULL);
		}
	};

	class Trie {
	public:
		TrieNode* getRoot() { return root; }
		Trie(const std::vector<std::string>& words) {
			root = new TrieNode();
			for (int i = 0; i < words.size(); ++i)
				addWord(words[i]);
		}
		~Trie() {
			delWord(root);
		}
		void delWord(TrieNode* n) {
			for (int i = 0; i < 26; i++) {
				if (n->children[i]) {
					delWord(n->children[i]);
				}
			}
			delete(n);
		}
		void addWord(const std::string& word) {
			TrieNode* cur = root;
			for (int i = 0; i < word.size(); ++i) {
				int index = word[i] - 'a';
				if (cur->children[index] == NULL)
					cur->children[index] = new TrieNode();
				cur = cur->children[index];
			}
			cur->is_end = true;
		}
	private:
		TrieNode* root;
	};
	struct position {
		int xpos;
		int ypos;
	};
	struct hash_pos {
		std::size_t operator()(const position& pos) const {
			return std::hash<int>()(pos.xpos) ^ std::hash<int>()(pos.ypos);
		}
	};
	struct hash_pos_eq {
		bool operator()(const position& l, const position& r) const {
			return l.xpos == r.xpos && l.ypos == r.ypos;
		}
	};
	std::vector<std::string> findWords(const std::vector<std::vector<char>>& board, const std::vector<std::string>& words) {
		std::vector<std::string> result;
		if (board.empty() || board[0].empty()) {
			return result;
		}
		Trie trietree(words);
		TrieNode* root = trietree.getRoot();
		std::unordered_set<std::string> restmp;
		for (int i = 0; i < board.size(); i++) {
			for (int j = 0; j < board[0].size(); j++) {
				std::unordered_set<position, hash_pos, hash_pos_eq> visited;
				std::string word = "";
				trymove(root, i, j, board, visited, restmp, word);
			}
		}
		for (const auto& i : restmp) {
			result.push_back(i);
		}
		return result;
	}
	void trymove(
		TrieNode* trn, int posx, int posy, const std::vector<std::vector<char>>& board, 
		std::unordered_set<position, hash_pos, hash_pos_eq>& visited, 
		std::unordered_set<std::string>& result,
		std::string word
	) {
		if (posx < 0 || posx >= board.size() || posy < 0 || posy >= board[0].size()) {
			return;
		}
		position p{ posx, posy };
		if (visited.count(p)) {
			return;
		}
		if (trn->children[board[posx][posy] - 'a']) {
			word += board[posx][posy];
			visited.insert(p);
			trn = trn->children[board[posx][posy] - 'a'];
			if (trn->is_end) {
				result.insert(word);
			}
			trymove(trn, posx, posy + 1, board, visited, result, word);
			trymove(trn, posx + 1, posy, board, visited, result, word);
			trymove(trn, posx, posy - 1, board, visited, result, word);
			trymove(trn, posx - 1, posy, board, visited, result, word);
			visited.erase(p);
		}
	}
};

class SolutionLC214 {
public:
	std::string shortestPalindrome(const std::string& s) {
		size_t sz = s.size();
		std::string result;
		if (sz == 0) return "";
		if (sz == 1) return s;
		if (sz == 2) {
			if (s[0] == s[1]) return s;
			result = s[1] + s;
			return result;
		}
		int pivot = (sz - 1) / 2;
		int low = pivot - 1;
		int high = pivot + 1;
		while (low >= 0 && high < sz && pivot > 0) {
			if (s[low--] != s[high++]) {
				pivot--;
				low = pivot - 1;
				high = pivot + 1;
			}
		}
		int pivot2 = sz / 2 - 1;
		int low2 = pivot2;
		int high2 = pivot2 + 1;
		while (low2 >= 0 && high2 < sz && pivot2 > 0) {
			if (s[low2--] != s[high2++]) {
				pivot2--;
				low2 = pivot2;
				high2 = pivot2 + 1;
			}
		}
		if (pivot > pivot2 || pivot == pivot2 && pivot2 == 0 && s[0] != s[1]) {
			result = s;
			for (int i = pivot * 2 + 1; i < sz; i++) {
				result = s[i] + result;
			}
		}
		else {
			result = s;
			for (int i = 2 * pivot2 + 2; i < sz; i++) {
				result = s[i] + result;
			}
		}
		return result;
	}
};

std::vector<int> getNext(const std::string& s) {
	int sz = s.size();
	std::vector<int> next(sz, 0);
	next[0] = -1;
	int j = 0;
	int k = -1;

	while (j < s.size() - 1) {
		if (k == -1 || s[j] == s[k]) {
			next[++j] = ++k;
		}
		else {
			k = next[k];
		}
	}
	return next;
}

int KMP(const std::string& ts, const std::string& ps) {

	int i = 0;
	int j = 0;
	int szt = ts.size();
	int szp = ps.size();
	std::vector<int> next = getNext(ps);

	while (i < szt && j < szp) {
		if (j == -1 || ts[i] == ps[j]) {
			i++;
			j++;
		}
		else {
			j = next[j];
		}
	}

	if (j == ps.size()) {
		return i - j;
	}
	else {
		return -1;
	}
}

class SolutionLC214Ver2 {
public:
	std::vector<int> getNext(const std::string& s) {
		std::vector<int> next(s.size(), 0);
		next[0] = -1;
		int i = -1, j = 0;
		while (j < s.size() - 1) {
			if (i == -1 || s[i] == s[j]) {
				next[++j] = ++i;
			}
			else {
				i = next[i];
			}
		}
		return next;
	}
	std::string shortestPalindrome(const std::string& s) {
		std::string rev(s);
		std::reverse(rev.begin(), rev.end());
		std::string tmp = s + "#" + rev;
		std::vector<int> next = getNext(tmp);
		std::string result = s.substr(next[tmp.size() - 1] + 1);
		std::reverse(result.begin(), result.end());
		return result + s;
	}
};

/*
*①	p[mirror]=8 > maxright-i=7
*		   mirror  center    i    maxright
*			 |       |		 |      |
*	#a#a#a#a#b#a#a#a#a#a#a#a#b#a#a#a#c???
*	  |______________|______________|
*			  |              |
*			  R				 R
*
*②	p[mirror]=2 < maxright-i=8
*		   mirror  center    i    maxright
*			 |       |		 |       |
*	#a#a#a#c#b#c#a#a#a#a#a#c#b#c#a#a#a???
*	|________________|_______________|
*			  |              |
*			  R				 R
*
*③	p[mirror]=7 == maxright-i=7
*		mirror  center   i   maxright
*		   |      |	     |      |
*	#a#a#c#b#c#a#a#a#a#c#b#c#a#a#?????
*	|_____________|_____________|
*		   |             |
*		   R			 R
*/
class Manacher {
public:
	std::string getLongestPalindrome(const std::string& s) {
		std::string pros = preprocess(s);
		int sz = pros.size();
		int maxright = 0, center = 0, mirror = 0, start = 0;
		int max = INT_MIN;
		std::vector<int> P(sz, 0);				//record right most elements of the palindrome centered at index i 
		for (int i = 0; i < sz; i++) {
			if (i < maxright) {
				mirror = 2 * center - i;
				P[i] = std::min<int>(P[mirror], maxright - i);
			}
			int left = i - 1 - P[i];
			int right = i + 1 + P[i];
			while (left >= 0 && right < sz && pros[left] == pros[right]) {
				left--;
				right++;
				P[i]++;
			}
			if (i + P[i] > maxright) {
				maxright = i + P[i];
				center = i;
			}
			if (P[i] > max) {
				max = P[i];
				start = (i - max) / 2;
			}
		}
		return s.substr(start, max);
	}
	std::string preprocess(const std::string& s) {
		std::string ret = "#";
		for (int i = 0; i < s.size(); i++) {
			ret = ret + s[i] + "#";
		}
		return ret;
	}
};

class SolutionLC218 {
public:
	std::vector<std::vector<int>> getSkyline(const std::vector<std::vector<int>>& buildings) {
		int xpos = 0, ypos = 0;
		std::vector<std::vector<int>> result;
		std::multiset<std::pair<int, int>> seq;
		for (auto p : buildings) {
			seq.emplace(std::make_pair(p[0], -p[2]));
			seq.emplace(std::make_pair(p[1], p[2]));
		}
		std::multiset<int> height({ 0 });
		for (auto p : seq) {
			if (p.second < 0)height.emplace(-p.second);
			else height.erase(height.find(p.second));
			if (*height.rbegin() != ypos) {
				xpos = p.first;
				ypos = *height.rbegin();
				std::vector<int> tmp;
				tmp.push_back(xpos);
				tmp.push_back(ypos);
				result.push_back(tmp);
			}
		}
		return result;
	}
};

class SolutionLC224 {
public:
	int calculate(const std::string& s) {
		size_t sz = s.size();
		std::stack<int> operand;
		int op = 0, result = 0;
		int sign = 1;
		for (int i = 0; i < sz; i++) {
			if (s[i] >= '0' && s[i] <= '9') {
				op = op * 10 + s[i] - '0';
			}
			else if (s[i] == '+') {
				result += op * sign;
				sign = 1;
				op = 0;
			}
			else if (s[i] == '-') {
				result += op * sign;
				sign = -1;
				op = 0;
			}
			else if (s[i] == '(') {
				operand.emplace(result);
				operand.emplace(sign);
				result = 0;
				sign = 1;
			}
			else if (s[i] == ')') {
				result += op * sign;
				if (!operand.empty()) {
					result *= operand.top();
					operand.pop();
				}
				if (!operand.empty()) {
					result += operand.top();
					operand.pop();
				}
				op = 0;
			}
			else if (s[i] == ' ') {
				continue;
			}
		}
		return result + op * sign;
	}
};

class SolutionLC233 {
public:
	int countDigitOne(int n) {
		if (n <= 0) return 0;
		int m = n;
		std::vector<int> number;
		while (m) {
			number.push_back(m % 10);
			m /= 10;
		}
		std::reverse(number.begin(), number.end());
		int sz = number.size();
		int result = 1;
		while (--sz) {
			result *= 10;
		}
		if (number[0] == 1) {
			result = n - result + 1;
		}
		for (int i = 1; i < number.size(); i++) {
			int j = 0;
			int bits = number.size() - i;
			int cnt = 1;
			int tmp = 0;
			while (--bits) {
				cnt *= 10;
			}
			while (j < i) {
				tmp = tmp * 10 + number[j++];
			}
			if (number[i] > 1) {
				tmp += 1;
			}
			else if (number[i] == 1) {
				result += n - (tmp * 10 + 1) * cnt + 1;
			}
			result += tmp * cnt;
		}
		return result;
	}
	//traverse each digit of n, if n=3401512
	//for m=100, split n into a=34015 and b=12
	//if a%10==1, #1=a/10*m+(b+1);
	//if a%10==0, #2=a/10*m;
	//if a%10>1; #3=a/10*m+m;
	//In general, #4=(a+8)/10*m+(a % 10 == 1)*(b + 1).
	//For general expression above: 
	//if a%10>1, then (a+8)/10=a/10+1, #4=#3
	//if a%10==0, (a+8)/10=a/10, (a % 10 == 1)=false, #4=#2
	//if a%10==1, (a+8)/10=a/10, (a % 10 == 1)=true, #4=#1
	//This is how "(a+8)/10*m+(a % 10 == 1)*(b + 1)" covers all three conditions.
	int countDigitOne2(int n) {
		int res = 0;
		for (long long m = 1; m <= n; m *= 10)
		{
			int a = n / m, b = n % m;
			res += (a + 8) / 10 * m + (a % 10 == 1) * (b + 1);
		}
		return res;
	}
};

template<typename T>
class receiver {
public:
	receiver(T&& tt) : t(tt) { t++; }
	std::decay_t<T> t;
};

template<typename T>
receiver<T> sender(T&& t){
	return receiver<T>(std::forward<T>(t));
}

//单调队列 往右扫描 碰到比左边大的 左边删掉 因为在右边 你没有机会比他大了 碰到小的放到后面 因为虽然小 但是他在右边 左边即使比他大的也会在他先出范围 右边的有机会成为新的最大的
class SolutionLC239 {
public:
	std::vector<int> maxSlidingWindow(const std::vector<int>& nums, int k) {
		std::deque<int> dq;
		std::vector<int> ans;
		for (int i = 0; i < nums.size(); i++) {
			if (!dq.empty() && dq.front() == i - k) {
				dq.pop_front();
			}
			while (!dq.empty() && nums[dq.back()] < nums[i]) {
				dq.pop_back();
			}
			dq.push_back(i);
			if (i >= k - 1) {
				ans.push_back(nums[dq.front()]);
			}
		}
		return ans;
	}
};

class MedianFinder {
public:
	std::priority_queue<int> max_heap;
	std::priority_queue<int, std::vector<int>, std::greater<std::vector<int>::value_type>> min_heap;

	void addNum(int num) {
		max_heap.push(num);
		min_heap.push(max_heap.top());
		max_heap.pop();
		if (max_heap.size() < min_heap.size()) {
			max_heap.push(min_heap.top());
			min_heap.pop();
		}
	}

	double findMedian() {
		if (max_heap.size() == min_heap.size()) {
			return (min_heap.top() + max_heap.top()) / 2.0;
		}
		return max_heap.top();
	}
};

template <class...>
struct _m_list {};

template <size_t _Idx, class _Ty>
struct _v_init_single_overload {
	using _FTy = _m_list<std::integral_constant<size_t, _Idx>, _Ty>(*)(_Ty);
	operator _FTy();
};

template <class _Indices, class... _Types>
struct _v_init_overload_set_;

template <size_t... _Indices, class... _Types>
struct _v_init_overload_set_<std::index_sequence<_Indices...>, _Types...>
	: _v_init_single_overload<_Indices, _Types>... {};

template <class... _Types>
using _v_init_overload_set = _v_init_overload_set_<std::index_sequence_for<_Types...>, _Types...>;

template <class Enable, class _Ty, class... _Types>
struct _v_init_helper {
};

template <class _Ty, class... _Types>
struct _v_init_helper < std::void_t<decltype(_v_init_overload_set<_Types...>{}(std::declval<_Ty>())) > , _Ty,
	_Types... > {
	using type = decltype(_v_init_overload_set<_Types...>{}(std::declval<_Ty>()));
};

template <class _Ty, class... _Types>
using _v_init_type = std::_Meta_front<std::_Meta_pop_front<typename _v_init_helper<void, _Ty, _Types...>::type>>;

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *	int val;
 *	bool isempty;
 *	TreeNode* left;
 *	TreeNode* right;
 *	TreeNode(int x) : val(x), left(nullptr), right(nullptr), isempty(false) {}
 *	TreeNode(void* p = nullptr) : isempty(true), val(0), left(nullptr), right(nullptr) {}
 *	operator bool() { return isempty; }
 * };
 */
class Solution297 {
public:

	// Encodes a tree to a single string.
	std::string serialize(TreeNode* root) {
		std::stringstream ss;
		s(root, ss);
		return ss.str();
	}

	// Decodes your encoded data to tree.
	TreeNode* deserialize(std::string data) {
		std::stringstream ss(data);
		return d(ss);
	}
private:
	void s(TreeNode* r, std::stringstream& ss) {
		if (r) {
			ss << r->val << " ";
			s(r->left, ss);
			s(r->right, ss);
		}
		else {
			ss << "# ";
		}
	}

	TreeNode* d(std::stringstream& ss) {
		std::string val;
		ss >> val;
		if (val == "#") {
			return nullptr;
		}
		std::stringstream sstmp;
		sstmp << val;
		int v;
		sstmp >> v;
		TreeNode* r = new TreeNode(v);
		r->left = d(ss);
		r->right = d(ss);
		return r;
	}
};

// Your Codec object will be instantiated and called as such:
// Codec ser, deser;
// TreeNode* ans = deser.deserialize(ser.serialize(root));

class Solution301 {
public:
	std::vector<std::string> removeInvalidParentheses(const std::string& s) {
		std::vector<std::string> res;
		dfs(0, 0, "()", s, res);
		return res;
	}
	void dfs(int start, int lastinvalidpa, const char pa[], const std::string& s, std::vector<std::string>& res) {
		int cnt = 0;
		for (int i = start; i < s.size(); i++) {
			if (s[i] == pa[0]) cnt++;
			else if (s[i] == pa[1]) cnt--;
			if (cnt >= 0) continue;
			for (int k = lastinvalidpa; k <= i; k++) {
				if (s[k] == pa[1] && (k == lastinvalidpa || s[k - 1] != pa[1])) {
					std::string ss = s.substr(0, k) + s.substr(k + 1);
					dfs(i, k, pa, ss, res);
				}
			}
			return;
		}
		std::string ss = s;
		std::reverse(ss.begin(), ss.end());
		if (pa[0] == '(') {
			dfs(0, 0, ")(", ss, res);
		}
		else {
			res.push_back(ss);
		}
	}
};

class Solution312 {
public:
	int maxCoins(const std::vector<int>& nums) {
		std::vector<int> coins;
		int cnt = 0;
		coins.push_back(1);
		cnt++;
		for (const auto& i : nums) {
			if (i > 0) {
				coins.push_back(i);
				cnt++;
			}
		}
		coins.push_back(1);
		cnt++;
		std::vector<std::vector<int>> dp(cnt, std::vector<int>(cnt, 0));
		for (int i = 2; i < cnt; i++) {
			for (int left = 0; left < cnt - i; left++) {
				int right = left + i;
				for (int k = left + 1; k < right; k++) {
					dp[left][right] = std::max<int>(dp[left][right], coins[left]*coins[k]*coins[right] + dp[left][k] + dp[k][right]);
				}
			}
		}
		return dp[0][cnt - 1];
	}
};

class Solution315 {
private:
	std::vector<std::pair<int, int>> vp;
	using ivp = std::vector<std::pair<int, int>>::iterator;
	void merge(ivp ib, ivp ie, std::vector<int>& res) {
		if (ie - ib <= 1) {
			return;
		}
		ivp im = ib + (ie - ib) / 2;
		merge(ib, im, res);
		merge(im, ie, res);
		for (ivp ii = ib, ij = im; ii < im; ii++) {
			while (ij < ie && ii->first > ij->first) {
				ij++;
			}
			res[ii->second] += ij - im;
		}
		std::inplace_merge(ib, im, ie);
	}
public:
	std::vector<int> countSmaller(const std::vector<int>& nums) {
		for (int i = 0; i < nums.size(); i++) {
			vp.push_back(std::make_pair(nums[i], i));
		}
		std::vector<int> res(nums.size(), 0);
		merge(vp.begin(), vp.end(), res);
		return res;
	}
};

class Solution327 {
public:
	int countRangeSum(const std::vector<int>& nums, int lower, int upper) {
		std::vector<int> sums(nums.size() + 1, 0);
		for (int i = 0; i < nums.size(); i++) {
			sums[i + 1] = sums[i] + nums[i];
		}
		return cntmerge(sums, 0, sums.size(), lower, upper);
	}
	int cntmerge(std::vector<int>& sums, int b, int e, int lower, int upper) {
		if (e - b <= 1) return 0;
		int m = (b + e) / 2;
		int cnt = cntmerge(sums, b, m, lower, upper) + cntmerge(sums, m, e, lower, upper);
		for (int i = b, j = m, k = m; i < m; i++) {
			while (j < e && sums[j] - sums[i] < lower) {
				j++;
			}
			while (k < e && sums[k] - sums[i] <= upper) {
				k++;
			}
			cnt += k - j;
		}
		std::inplace_merge(sums.begin() + b, sums.begin() + m, sums.begin() + e);
		return cnt;
	}
};

class Solution329 {
public:
	int longestIncreasingPath(const std::vector<std::vector<int>>& matrix) {
		if (matrix.size() == 0) return 0;
		int maxlen = 0;
		std::vector<std::vector<int>> startp(matrix.size(), std::vector<int>(matrix[0].size(), 0));
		for (int i = 0; i < startp.size(); i++) {
			for (int j = 0; j < startp[0].size(); j++) {
				if (startp[i][j]) continue;
				int cnt = countlen(i, j, matrix, startp);
				maxlen = cnt > maxlen ? cnt : maxlen;
			}
		}
		return maxlen;
	}
	int countlen(int i, int j, const std::vector<std::vector<int>>& m, std::vector<std::vector<int>>& s) {
		int left = 0, right = 0, up = 0, down = 0;
		if (j + 1 < s[0].size() && m[i][j + 1] > m[i][j]) {
			if (s[i][j + 1] > 0) {
				right = s[i][j + 1];
			}
			else {
				right = countlen(i, j + 1, m, s);
			}
		}
		if (i + 1 < s.size() && m[i + 1][j] > m[i][j]) {
			if (s[i + 1][j] > 0) {
				down = s[i + 1][j];
			}
			else {
				down = countlen(i + 1, j, m, s);
			}
		}
		if (j - 1 >= 0 && m[i][j - 1] > m[i][j]) {
			if (s[i][j - 1] > 0) {
				left = s[i][j - 1];
			}
			else {
				left = countlen(i, j - 1, m, s);
			}
		}
		if (i - 1 >= 0 && m[i - 1][j] > m[i][j]) {
			if (s[i - 1][j] > 0) {
				up = s[i - 1][j];
			}
			else {
				up = countlen(i - 1, j, m, s);
			}
		}
		int max = std::max<int>(std::max<int>(left, right), std::max<int>(up, down));
		s[i][j] = max + 1;

		return s[i][j];
	}
};

class Solution336 {
public:
	std::vector<std::vector<int>> palindromePairs(const std::vector<std::string>& words) {
		std::vector<std::vector<int>> pairA(26, std::vector<int>()), pairB(26, std::vector<int>());
		std::vector<std::vector<int>> res;
		int cnt = 0;
		for (int i = 0; i < words.size(); i++) {
			if (words[i] == "") {
				for (int j = 0; j < 26; j++) {
					pairA[i].push_back(i);
					pairB[i].push_back(i);
				}
			}
			else {
				pairA[words[i][0] - 'a'].push_back(i);
				pairB[words[i][words[i].size() - 1] - 'a'].push_back(i);
			}
		}
		for (int i = 0; i < 26; i++) {
			for (int j = 0; j < pairA[i].size(); j++) {
				for (int k = 0; k < pairB[i].size(); k++) {
					if (words[pairA[i][j]] != words[pairB[i][k]]) {
						std::string strA = words[pairA[i][j]] + words[pairB[i][k]];
						std::string strB = words[pairB[i][k]] + words[pairA[i][j]];
						if (checkp(strA)) {
							std::vector<int> vec_res;
							vec_res.push_back(pairA[i][j]);
							vec_res.push_back(pairB[i][k]);
							res.push_back(vec_res);
						}
						if (checkp(strB)) {
							std::vector<int> vec_res;
							vec_res.push_back(pairB[i][k]);
							vec_res.push_back(pairA[i][j]);
							res.push_back(vec_res);
						}
					}
				}
			}
		}
		return res;
	}
	bool checkp(const std::string& s) {
		for (int i = 0, j = s.size() - 1; i <= j; i++, j--) {
			if (s[i] != s[j]) {
				return false;
			}
		}
		return true;
	}
};


/*
Case1: If s1 is a blank string, then for any string that is palindrome s2, s1+s2 and s2+s1 are palindrome.

Case 2: If s2 is the reversing string of s1, then s1+s2 and s2+s1 are palindrome.

Case 3: If s1[0:cut] is palindrome and there exists s2 is the reversing string of s1[cut+1:] , then s2+s1 is palindrome.

Case 4: Similiar to case3. If s1[cut+1: ] is palindrome and there exists s2 is the reversing string of s1[0:cut] , then s1+s2 is palindrome.
*/
class Solution336Ver2 {
public:
	std::vector<std::vector<int>> palindromePairs(const std::vector<std::string>& words) {
		std::vector<std::vector<int>> res;
		int sz = words.size();
		std::unordered_map<std::string, int> dict;
		for (int i = 0; i < sz; i++) {
			dict[words[i]] = i;
		}
		if (dict.find("") != dict.end()) {
			for (int i = 0; i < sz; i++) {
				if (checkp(words[i])) {
					if (dict[""] == dict[words[i]]) continue;
					std::vector<int> vec_res;
					vec_res.push_back(dict[""]);
					vec_res.push_back(dict[words[i]]);
					res.push_back(vec_res);
					std::vector<int>().swap(vec_res);
					vec_res.push_back(dict[words[i]]);
					vec_res.push_back(dict[""]);
					res.push_back(vec_res);
				}
			}
		}
		for (int i = 0; i < sz; i++) {
			std::string tmp = words[i];
			int wsz = words[i].size();
			std::reverse(tmp.begin(), tmp.end());
			if (dict.find(tmp) != dict.end() && dict[tmp] != dict[words[i]]) {
				std::vector<int> vec_res;
				vec_res.push_back(dict[tmp]);
				vec_res.push_back(dict[words[i]]);
				res.push_back(vec_res);
			}
			for (int j = 0; j <= wsz - 2; j++) {
				if (checkp(words[i].substr(0, j + 1))) {
					std::string str = words[i].substr(j + 1);
					std::reverse(str.begin(), str.end());
					if (dict.find(str) != dict.end() && dict[str] != dict[words[i]]) {
						std::vector<int> vec_res;
						vec_res.push_back(dict[str]);
						vec_res.push_back(dict[words[i]]);
						res.push_back(vec_res);
					}
				}
				if (checkp(words[i].substr(j + 1))) {
					std::string str = words[i].substr(0, j + 1);
					std::reverse(str.begin(), str.end());
					if (dict.find(str) != dict.end() && dict[str] != dict[words[i]]) {
						std::vector<int> vec_res;
						vec_res.push_back(dict[words[i]]);
						vec_res.push_back(dict[str]);
						res.push_back(vec_res);
					}
				}
			}
		}
		return res;
	}
	bool checkp(const std::string& s) {
		for (int i = 0, j = s.size() - 1; i <= j; i++, j--) {
			if (s[i] != s[j]) {
				return false;
			}
		}
		return true;
	}
};

class Solution354 {
public:
	int maxEnvelopes(const std::vector<std::vector<int>>& envelopes) {
		std::vector<std::pair<int, int>> ev;
		for (const auto& e : envelopes) {
			ev.push_back(std::make_pair(e[0], e[1]));
		}
		std::sort(ev.begin(), ev.end(), [](const std::pair<int, int>& left, std::pair<int, int>& right) {
			return left.first < right.first || left.first == right.first && left.second > right.second;
		});
		std::vector<int> res;
		for (const auto& e : ev) {
			auto it = std::lower_bound(res.begin(), res.end(), e.second);
			if (it == res.end()) {
				res.push_back(e.second);
			}
			else if (e.second < *it) {
				*it = e.second;
			}
		}
		return res.size();
	}
};

/*
Given a non-empty 2D matrix matrix and an integer k, find the max sum of a rectangle in the matrix such that its sum is no larger than k.
suppose cols is large than rows
*/
class Solution363 {
public:
	int maxSumSubmatrix(const std::vector<std::vector<int>>& matrix, int k) {
		if (matrix.empty()) return 0;
		int rows = matrix.size(), cols = matrix[0].size(), res = INT_MIN;
		for (int i = 0; i < cols; ++i) {
			std::vector<int> sums(rows, 0);
			for (int j = i; j < cols; ++j) {
				for (int k = 0; k < rows; ++k) {
					sums[k] += matrix[k][j];
				}

				std::set<int> records;
				records.insert(0);
				int curSum = 0, curMax = INT_MIN;
				for (int sum : sums) {
					curSum += sum;
					std::set<int>::iterator it = records.lower_bound(curSum - k);
					if (it != records.end()) curMax = std::max(curMax, curSum - *it);
					records.insert(curSum);
				}
				res = std::max(res, curMax);
			}
		}
		return res;
	}
};

template<typename T>
struct CheckType
{
	template<typename U = T, typename std::enable_if<
		std::is_same<U, int>::value, int>::type = 0>
		U read() { return 42; }

	template<typename U = T, typename std::enable_if<
		std::is_same<U, double>::value, int>::type = 0>
		U read() { return 3.14; }

	void read() {}
};

void BitFindFC(uint64_t* bits, int first, int last, int* index) {
	int    elmtIdx, bitIdx;
	*index = -1;

	if (first > last) {
		return;
	}

	for (elmtIdx = first / 64; elmtIdx <= last / 64; elmtIdx++) {
		if (bits[elmtIdx] == 18446744073709551615u) {
			continue;
		}

		for (bitIdx = ((elmtIdx == first / 64) ? first % 64 : 0);
			bitIdx < 64;
			bitIdx++) {
			if (!(bits[elmtIdx] & (((uint64_t)1) << bitIdx))) {
				if ((bitIdx + elmtIdx * 64) <= last) {
					*index = bitIdx + elmtIdx * 64;
				}
				return;
			}
		}
	}
}

class RandomizedCollection {
public:
	//m[nums[i].first][nums[i].second] == i;
	/** Initialize your data structure here. */
	RandomizedCollection() {

	}

	/** Inserts a value to the collection. Returns true if the collection did not already contain the specified element. */
	bool insert(int val) {
		auto r = mp.find(val) == mp.end();
		mp[val].push_back(nums.size());
		nums.push_back(std::make_pair(val, mp[val].size() - 1));
		return r;
	}

	/** Removes a value from the collection. Returns true if the collection contained the specified element. */
	bool remove(int val) {
		auto r = mp.find(val) != mp.end();
		if (r) {
			auto last = nums.back();
			mp[last.first][last.second] = mp[val].back();
			nums[mp[val].back()] = last;
			mp[val].pop_back();
			if (mp[val].empty()) mp.erase(val);
			nums.pop_back();
		}
		return r;
	}

	/** Get a random element from the collection. */
	int getRandom() {
		std::random_device rd;
		std::uniform_int_distribution<int> dist(0, nums.size() - 1);
		return nums[dist(rd)].first;
	}
private:
	std::unordered_map<int, std::vector<int>> mp;
	std::vector<std::pair<int, int>> nums;
};

class SolutionLC391 {
public:
	//BL1 BR2 TL4 TR8
	//T junction and cross are valid (BL/BR TL/TR BR/TR BL/TL BL/BR/TL/TR)
	bool isRectangleCover(std::vector<std::vector<int>>& rectangles) {
		int minx = INT_MAX, maxx = INT_MIN, miny = INT_MAX, maxy = INT_MIN;
		for (const auto& rc : rectangles) {
			minx = std::min(minx, rc[0]);
			maxx = std::max(maxx, rc[2]);
			miny = std::min(miny, rc[1]);
			maxy = std::max(maxy, rc[3]);

			if (!insertCorner(rc[0], rc[1], 1)) return false;//BL
			if (!insertCorner(rc[2], rc[3], 8)) return false;//TR
			if (!insertCorner(rc[0], rc[3], 4)) return false;//TL
			if (!insertCorner(rc[2], rc[1], 2)) return false;//BR
		}

		bool check[16] = { false };
		check[3] = check[5] = check[10] = check[12] = check[15] = true;

		for (auto itx = corners.begin(); itx != corners.end(); itx++) {
			int x = itx->first;
			for (auto ity = itx->second.begin(); ity != itx->second.end(); ity++) {
				int y = ity->first;
				if (((x != minx && x != maxx) || (y != miny && y != maxy)) && !check[ity->second]) return false;
			}
		}
		return true;
	}
	bool insertCorner(int x, int y, int direction) {
		if (corners[x][y] & direction) return false;
		corners[x][y] |= direction;
		return true;
	}
private:
	std::unordered_map<int, std::unordered_map<int, int>> corners;
};

struct ListNode2 {
	ListNode2* next;
	int val;
};

bool removelastk(ListNode2* l, int k) {
	int n = k + 1;
	ListNode2* p1 = l,  *p2 = l;
	while (p2&&--n) {
		p2 = p2->next;
	}
	if (!p2 && n == 1) {
		l = l->next;
		return true;
	}
	else if (!p2 && n > 1) return false;
	else {
		while (p2 && p2->next) {
			p2 = p2->next;
			p1 = p1->next;
		}
		p1->next = p1->next->next;
	}
}

class Solution297V2 {
private:
	TreeNode* deserializeHelper(string& data) {
		if (data.back() == '#') {
			data.pop_back();
			if (data.size())
				data.pop_back();
			return nullptr;
		}
		int n = 0;
		for (int i = data.size() - 1, j = 1; i >= 0 && data[i] != ','; i--, j *= 10) {
			if (data[i] == '-')
				n = -n;
			else
				n += (data[i] - '0') * j;
			data.pop_back();
		}
		if (data.size())
			data.pop_back();
		TreeNode* root = new TreeNode(n);
		root->right = deserializeHelper(data);
		root->left = deserializeHelper(data);
		return root;
	}
public:

	// Encodes a tree to a single string.
	string serialize(TreeNode* root) {
		if (!root)
			return "#";
		string left = serialize(root->left);
		string right = serialize(root->right);
		return left + ',' + right + ',' + to_string(root->val);
	}

	// Decodes your encoded data to tree.
	TreeNode* deserialize(string data) {
		string copy = data;
		return deserializeHelper(copy);
	}
};

class SolutionLC105 {
public:
	TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
		int rootindex = 0;
		return buildhelper(preorder, inorder, rootindex, 0, inorder.size() - 1);
	}
	TreeNode* buildhelper(vector<int>& preorder, vector<int>& inorder, int& rootindex, int start, int end) {
		if (rootindex >= preorder.size() || start > end) return nullptr;
		TreeNode* t = new TreeNode(preorder[rootindex]);
		int pos = std::distance(inorder.begin(), std::find(inorder.begin() + start, inorder.begin() + end, preorder[rootindex]));
		rootindex++;
		t->left = buildhelper(preorder, inorder, rootindex, start, pos - 1);
		t->right = buildhelper(preorder, inorder, rootindex, pos + 1, end);
		return t;
	}
};

void levelTraversal(TreeNode* t) {
	std::queue<TreeNode*> q;
	if (t) {
		q.push(t);
		while (!q.empty()) {
			//visit
			TreeNode* tmp = q.front();
			q.pop();
			TreeNode* left = tmp->left, * right = tmp->right;
			if (left) q.push(left);
			if (right) q.push(right);
		}
	}
}

int partitionv(std::vector<int>& nums, int low, int high) {
	int i = low;
	int j = high;
	while (true) {
		while (i < high && nums[i] < nums[low]) {
			i++;
		}
		while (low < j && nums[j] > nums[low]) {
			j--;
		}
		if (i >= j) break;
		std::swap(nums[i], nums[j]);
	}
	std::swap(nums[low], nums[j]);
	return j;
}

int findKth(std::vector<int>& nums, int k) {
	int high = nums.size() - 1;
	int low = 0;
	k = nums.size() - k;
	while (low < high) {
		int pos = partitionv(nums, low, high);
		if (k < pos) {
			high = pos - 1;
		}
		else if (k > pos) {
			low = pos + 1;
		}
		else {
			break;
		}
	}
	return nums[k];
}

int minPathLength(std::vector<std::vector<int>> grids, int tr, int tc) {
	std::vector<std::vector<int>> direction = { {1,0},{-1,0},{0,1},{0,-1} };
	int m = grids.size();
	int n = grids[0].size();
	int pathLength = 0;
	if (m == 0 || tr < 0 || tc < 0)
		return -1;
	std::queue<std::pair<int, int>> queue;
	queue.push(std::pair<int, int>(0, 0));
	while (!queue.empty()) {
		int size = queue.size(); //队列中是否有元素
		pathLength++; //每循环一次，长度加一
		while (size-- > 0) {
			std::pair<int, int> cur = queue.front();
			queue.pop();
			int cr = cur.first; //当前点的行坐标
			int cc = cur.second; //当前点的列坐标
			grids[cr][cc] = 0; //标记当前点已经访问过
			for (auto d : direction) {
				int nr = cr + d[0];
				int nc = cc + d[1]; //下一个点的横纵坐标
				if (nr < 0 || nr >= m || nc < 0 || nc >= n || grids[nr][nc] == 0) {
					continue;
				}
				if (nr == tr && nc == tc) {
					return pathLength;
				}
				queue.push(std::pair<int, int>(nr, nc));
			}
		}
	}
	return -1;
}