#include "Impl.h"

struct cpp11impltest::Impl
{
	int xint;
};
cpp11impltest::cpp11impltest():pImpl(std::make_unique<Impl>())
{}
cpp11impltest::~cpp11impltest() = default;//ͷ�ļ���Impl�Ƿ��������� ��������Ĭ�����������ᱨ�� ���Խ�Ĭ��������������Impl����֮�� �ƶ����츳ֵҲһ��