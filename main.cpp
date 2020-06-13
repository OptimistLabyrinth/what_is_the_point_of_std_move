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
		LOG(INFO) << "    parameterized constructor (id: " << id << ", size: " << len << ")";
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
		LOG(INFO) << "    copy constructor (id: " << id << ", size: " << len << ")";
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
		LOG(INFO) << "    copy assignment operator (id: " << id << ", size: " << len << ")";
		return *this;
	}
	A(A&& a) noexcept
		: id(A::cnt++)
	{
		// below block is the same with this => [ *this = std::move(a) ];
		if (a.len != -1) {
			std::swap(str, a.str);
			std::swap(len, a.len);
		}
		LOG(INFO) << "    move constructor (id: " << id << ", size: " << len << ")";
	}
	A& operator=(A&& a) noexcept {
		if (this != &a) {
			if (a.len != -1) {
				std::swap(str, a.str);
				std::swap(len, a.len);
			}
		}
		LOG(INFO) << "    move assignment operator (id: " << id << ", size: " << len << ")";
		return *this;
	}
	~A() {
		LOG(INFO) << "    destructor (id: " << id << ", size: " << len << ")";
		if (len != -1) {
			delete[] str;
			len = -1;
			LOG(INFO) << "    -> really delete[]";
		}
	}

	char* str = nullptr;
	int len = -1;
	unsigned long id = -1;

	static unsigned long cnt;
};

unsigned long A::cnt = 1;

A move_test_01(A a) {
	LOG(INFO) << __LINE__;
	return a;
}

A move_test_02() {
	LOG(INFO) << __LINE__;
	A a(13);
	a.str[0] = 'L';
	a.str[1] = 'A';
	a.str[2] = '?';
	LOG(INFO) << __LINE__;
	return a;
}

void test_modern_cpp() {
	LOG(INFO) << __LINE__;
	A a1(7);
	a1.str[0] = 'a';
	a1.str[1] = '%';
	a1.str[2] = '9';
	LOG(INFO) << __LINE__;
	A a2 = A(17);
	a2.str[0] = 'X';
	a2.str[1] = 'O';
	a2.str[2] = 'R';
	LOG(INFO) << "a1.str: " << (a1.str != nullptr ? a1.str : "nul") << "... ai.id: " << a1.id;
	LOG(INFO) << "a2.str: " << (a2.str != nullptr ? a2.str : "nul") << "... a2.id: " << a2.id;
	LOG(INFO) << __LINE__;
	A a3 = a1;
	LOG(INFO) << "a1.str: " << (a1.str != nullptr ? a1.str : "nul") << "... ai.id: " << a1.id;
	LOG(INFO) << "a2.str: " << (a2.str != nullptr ? a2.str : "nul") << "... a2.id: " << a2.id;
	LOG(INFO) << "a3.str: " << (a3.str != nullptr ? a3.str : "nul") << "... a3.id: " << a3.id;
	LOG(INFO) << __LINE__;
	A a4 = std::move(a1);
	LOG(INFO) << "a1.str: " << (a1.str != nullptr ? a1.str : "nul") << "... ai.id: " << a1.id;
	LOG(INFO) << "a2.str: " << (a2.str != nullptr ? a2.str : "nul") << "... a2.id: " << a2.id;
	LOG(INFO) << "a3.str: " << (a3.str != nullptr ? a3.str : "nul") << "... a3.id: " << a3.id;
	LOG(INFO) << "a4.str: " << (a4.str != nullptr ? a4.str : "nul") << "... a4.id: " << a4.id;
	LOG(INFO) << __LINE__;
	A a5 = move_test_01(a2);
	LOG(INFO) << "a1.str: " << (a1.str != nullptr ? a1.str : "nul") << "... ai.id: " << a1.id;
	LOG(INFO) << "a2.str: " << (a2.str != nullptr ? a2.str : "nul") << "... a2.id: " << a2.id;
	LOG(INFO) << "a3.str: " << (a3.str != nullptr ? a3.str : "nul") << "... a3.id: " << a3.id;
	LOG(INFO) << "a4.str: " << (a4.str != nullptr ? a4.str : "nul") << "... a4.id: " << a4.id;
	LOG(INFO) << "a5.str: " << (a5.str != nullptr ? a5.str : "nul") << "... a5.id: " << a5.id;
	LOG(INFO) << __LINE__;
	A a6 = move_test_01(std::move(a2));
	LOG(INFO) << "a1.str: " << (a1.str != nullptr ? a1.str : "nul") << "... ai.id: " << a1.id;
	LOG(INFO) << "a2.str: " << (a2.str != nullptr ? a2.str : "nul") << "... a2.id: " << a2.id;
	LOG(INFO) << "a3.str: " << (a3.str != nullptr ? a3.str : "nul") << "... a3.id: " << a3.id;
	LOG(INFO) << "a4.str: " << (a4.str != nullptr ? a4.str : "nul") << "... a4.id: " << a4.id;
	LOG(INFO) << "a5.str: " << (a5.str != nullptr ? a5.str : "nul") << "... a5.id: " << a5.id;
	LOG(INFO) << "a6.str: " << (a6.str != nullptr ? a6.str : "nul") << "... a6.id: " << a6.id;
	LOG(INFO) << __LINE__;
	a3 = move_test_02();
	LOG(INFO) << "a1.str: " << (a1.str != nullptr ? a1.str : "nul") << "... ai.id: " << a1.id;
	LOG(INFO) << "a2.str: " << (a2.str != nullptr ? a2.str : "nul") << "... a2.id: " << a2.id;
	LOG(INFO) << "a3.str: " << (a3.str != nullptr ? a3.str : "nul") << "... a3.id: " << a3.id;
	LOG(INFO) << "a4.str: " << (a4.str != nullptr ? a4.str : "nul") << "... a4.id: " << a4.id;
	LOG(INFO) << "a5.str: " << (a5.str != nullptr ? a5.str : "nul") << "... a5.id: " << a5.id;
	LOG(INFO) << "a6.str: " << (a6.str != nullptr ? a6.str : "nul") << "... a6.id: " << a6.id;
	LOG(INFO) << __LINE__;
}
