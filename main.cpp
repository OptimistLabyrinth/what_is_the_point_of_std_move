#include <cstdlib>
#include <iostream>

#define GOOGLE_GLOG_DLL_DECL
#include "glog/logging.h"

void test_modern_cpp();

int main() {
	FLAGS_minloglevel = google::GLOG_INFO;
	FLAGS_logtostderr = false;
	FLAGS_alsologtostderr = true;
	google::InitGoogleLogging("cpp_test");
	std::atexit(google::ShutdownGoogleLogging);

	LOG(INFO) << "[" << _MSVC_LANG << "]";

	test_modern_cpp();

	return 0;
}

class A {
public:
	explicit A(int i)
		: id(A::cnt++)
	{
		if (0 < i) {
			str = new char[i];
			len = i;
			for (size_t i = 0; i < len; ++i) {
				*(str + i) = '\0';
			}
		}
		LOG(INFO) << "    parameterized constructor (size: " << len << ", id: " << id << ")";
	}
	A(const A& a)
		: id(A::cnt++)
	{
		if (a.len != -1) {
			str = new char[a.len];
			len = a.len;
			for (size_t i = 0; i < a.len; ++i) {
				str[i] = a.str[i];
			}
		}
		LOG(INFO) << "    copy constructor (size: " << len << ", id: " << id << ")";
	}
	A& operator=(const A& a) {
		if (this != &a) {
			if (a.len != -1) {
				delete[] str;
				len = -1;
			}
			str = new char[a.len];
			len = a.len;
			for (size_t i = 0; i < a.len; ++i) {
				str[i] = a.str[i];
			}
		}
		LOG(INFO) << "    copy assignment operator (size: " << len << ", id: " << id << ")";
		return *this;
	}
	A(A&& a) noexcept
		: id(A::cnt++)
	{
		if (a.str) {
			str = a.str;
			a.str = nullptr;
			len = a.len;
		}
		LOG(INFO) << "    move constructor (size: " << len << ", id: " << id << ")";
	}
	A& operator=(A&& a) noexcept {
		if (this != &a) {
			if (a.str) {
				str = a.str;
				a.str = nullptr;
				len = a.len;
			}
		}
		LOG(INFO) << "    move assignment operator (size: " << len << ", id: " << id << ")";
		return *this;
	}
	~A() {
		LOG(INFO) << "    destructor (size: " << len << ", id: " << id << ")";
		if (len != -1) {
			delete[] str;
			len = -1;
			LOG(INFO) << "    -> really delete[]";
		}
	}

private:
	char* str = nullptr;
	int len = -1;
	unsigned long id = -1;

	static unsigned long cnt;
};

unsigned long A::cnt = 1;

A move_test() {
	LOG(INFO) << __LINE__;
	A a(13);
	LOG(INFO) << __LINE__;
	return a;
}

void test_modern_cpp() {
	LOG(INFO) << __LINE__;
	A a1(7);
	LOG(INFO) << __LINE__;
	A a2 = std::move(a1);
	LOG(INFO) << __LINE__;
	a2 = A(9);
	LOG(INFO) << __LINE__;
	a2 = move_test();
	LOG(INFO) << __LINE__;
}
