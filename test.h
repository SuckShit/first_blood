#include <utility>
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
#include <array>
#include <dbghelp.h>
#include <type_traits>
#include "Impl.h"
//#include <boost/filesystem.hpp>
using namespace std;
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