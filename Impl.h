#include <memory>

class cpp11impltest
{
public:
	cpp11impltest();
	~cpp11impltest();
private:
	struct Impl;
	std::unique_ptr<Impl> pImpl;
};