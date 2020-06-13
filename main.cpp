#include <cstdlib>
#include <iostream>

#ifdef _MSVC_LANG
#define GOOGLE_GLOG_DLL_DECL
#endif
#include "glog/logging.h"

void test_modern_cpp();

int main() {
	FLAGS_minloglevel = google::GLOG_INFO;
	FLAGS_logtostderr = false;
	FLAGS_alsologtostderr = true;
	google::InitGoogleLogging("cpp_test");
	std::atexit(google::ShutdownGoogleLogging);

#ifdef _MSVC_LANG
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "[" << _MSVC_LANG << "]";
#else
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "[" << __cplusplus << "]";
#endif
	
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
		LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "    parameterized constructor (id: " << id << ", size: " << len << ")";
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
		LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "    copy constructor (id: " << id << ", size: " << len << ")";
	}
	A& operator=(const A& a) {
		bool is_really_deleted = false;
		if (this != &a) {
			if (a.len != -1) {
				delete[] str;
				len = -1;
				is_really_deleted = true;
			}
			str = new char[a.len];
			len = a.len;
			for (size_t i = 0; i < a.len; ++i) {
				str[i] = a.str[i];
			}
		}
		LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "    copy assignment operator (id: " << id << ", size: " << len << ")";
		if (is_really_deleted) {
			LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "    -> delete[] in copy assignment";
		}
		return *this;
	}
	A(A&& a) noexcept
		: id(A::cnt++)
	{
		// below block is the same as => [ *this = std::move(a) ];
		if (a.len != -1) {
			str = a.str;
			len = a.len;
			a.str = nullptr;
			a.len = -1;
		}
		LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "    move constructor (id: " << id << ", size: " << len << ")";
	}
	A& operator=(A&& a) noexcept {
		bool is_really_deleted = false;
		if (this != &a) {
			if (len != -1) {
				delete[] str;
				len = -1;
				is_really_deleted = true;
			}
			if (a.len != -1) {
				str = a.str;
				len = a.len;
				a.str = nullptr;
				a.len = -1;
			}
		}
		LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "    move assignment operator (id: " << id << ", size: " << len << ")";
		if (is_really_deleted) {
			LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "    -> delete[] in move assignment";
		}
		return *this;
	}
	~A() {
		LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "    destructor (id: " << id << ", size: " << len << ")";
		if (len != -1) {
			delete[] str;
			len = -1;
			LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "    -> delete[] in destructor";
		}
	}

	char* str = nullptr;
	int len = -1;
	unsigned long id = -1;

	static unsigned long cnt;
};

unsigned long A::cnt = 1;

A move_test_01(A a) {
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << __LINE__;
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "> a.str: " << (a.str != nullptr ? a.str : "nul") << "... a.id: " << a.id;
	return a;
}

A move_test_02() {
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << __LINE__;
	A a(13);
	a.str[0] = 'L';
	a.str[1] = 'A';
	a.str[2] = '?';
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "> a.str: " << (a.str != nullptr ? a.str : "nul") << "... a.id: " << a.id;
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << __LINE__;
	return a;
}

void test_modern_cpp() {
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << __LINE__;
	A a1(7);
	a1.str[0] = 'a';
	a1.str[1] = '%';
	a1.str[2] = '9';
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << __LINE__;
	A a2 = A(17);
	a2.str[0] = 'X';
	a2.str[1] = 'O';
	a2.str[2] = 'R';
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "a1.str: " << (a1.str != nullptr ? a1.str : "nul") << "... ai.id: " << a1.id;
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "a2.str: " << (a2.str != nullptr ? a2.str : "nul") << "... a2.id: " << a2.id;
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << __LINE__;
	A a3 = a1;
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "a1.str: " << (a1.str != nullptr ? a1.str : "nul") << "... ai.id: " << a1.id;
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "a2.str: " << (a2.str != nullptr ? a2.str : "nul") << "... a2.id: " << a2.id;
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "a3.str: " << (a3.str != nullptr ? a3.str : "nul") << "... a3.id: " << a3.id;
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << __LINE__;
	A a4 = std::move(a1);
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "a1.str: " << (a1.str != nullptr ? a1.str : "nul") << "... ai.id: " << a1.id;
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "a2.str: " << (a2.str != nullptr ? a2.str : "nul") << "... a2.id: " << a2.id;
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "a3.str: " << (a3.str != nullptr ? a3.str : "nul") << "... a3.id: " << a3.id;
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "a4.str: " << (a4.str != nullptr ? a4.str : "nul") << "... a4.id: " << a4.id;
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << __LINE__;
	a3 = move_test_01(a2);
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "a1.str: " << (a1.str != nullptr ? a1.str : "nul") << "... ai.id: " << a1.id;
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "a2.str: " << (a2.str != nullptr ? a2.str : "nul") << "... a2.id: " << a2.id;
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "a3.str: " << (a3.str != nullptr ? a3.str : "nul") << "... a3.id: " << a3.id;
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "a4.str: " << (a4.str != nullptr ? a4.str : "nul") << "... a4.id: " << a4.id;
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << __LINE__;
	a4 = move_test_01(std::move(a2));
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "a1.str: " << (a1.str != nullptr ? a1.str : "nul") << "... ai.id: " << a1.id;
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "a2.str: " << (a2.str != nullptr ? a2.str : "nul") << "... a2.id: " << a2.id;
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "a3.str: " << (a3.str != nullptr ? a3.str : "nul") << "... a3.id: " << a3.id;
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "a4.str: " << (a4.str != nullptr ? a4.str : "nul") << "... a4.id: " << a4.id;
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << __LINE__;
	a3 = move_test_02();
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "a1.str: " << (a1.str != nullptr ? a1.str : "nul") << "... ai.id: " << a1.id;
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "a2.str: " << (a2.str != nullptr ? a2.str : "nul") << "... a2.id: " << a2.id;
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "a3.str: " << (a3.str != nullptr ? a3.str : "nul") << "... a3.id: " << a3.id;
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << "a4.str: " << (a4.str != nullptr ? a4.str : "nul") << "... a4.id: " << a4.id;
	LOG(INFO) << (size_t(__LINE__) < 100 ? " " : "") << __LINE__;
}
