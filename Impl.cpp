#include "Impl.h"

struct cpp11impltest::Impl
{
	int xint;
};
cpp11impltest::cpp11impltest():pImpl(std::make_unique<Impl>())
{}
cpp11impltest::~cpp11impltest() = default;//头文件中Impl是非完整类型 编译生成默认析构函数会报错 所以将默认析构函数放在Impl定义之后 移动构造赋值也一样