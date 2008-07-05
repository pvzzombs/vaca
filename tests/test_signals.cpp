#include <cassert>
#include <string>

#include "Vaca/Signal.hpp"

using namespace std;
using namespace Vaca;

//////////////////////////////////////////////////////////////////////

namespace test0 {

  void func0_void() { }
  void func1_void(int a) { }
  void func2_void(int a, string b) { }
  int func0_int() { return -1; }
  int func1_int(int a) { return a; }
  int func2_int(int a, int b) { return a+b; }
  string func0_string() { return "empty"; }
  string func1_string(string a) { return a; }
  string func2_string(string a, string b) { return a+b; }

  struct struct0_void { void operator()() { } };
  struct struct1_void { void operator()(int a) { } };
  struct struct2_void { void operator()(int a, string b) { } };
  struct struct0_int { int operator()() { return -1; } };
  struct struct1_int { int operator()(int a) { return a; } };
  struct struct2_int { int operator()(int a, int b) { return a+b; } };
  struct struct0_string { string operator()() { return "empty"; } };
  struct struct1_string { string operator()(string a) { return a; } };
  struct struct2_string { string operator()(string a, string b) { return a+b; } };

  class class0_void { public: void method() { } };
  class class1_void { public: void method(int a) { } };
  class class2_void { public: void method(int a, string b) { } };
  class class0_int { public: int method() { return -1; } };
  class class1_int { public: int method(int a) { return a; } };
  class class2_int { public: int method(int a, int b) { return a+b; } };
  class class0_string { public: string method() { return "empty"; } };
  class class1_string { public: string method(string a) { return a; } };
  class class2_string { public: string method(string a, string b) { return a+b; } };

  void test_coverage()
  {
    Signal0<void>			sig0_void;
    Signal1<void, int>			sig1_void;
    Signal2<void, int, string>		sig2_void;
    Signal0<int>			sig0_int;
    Signal1<int, int>			sig1_int;
    Signal2<int, int, int>		sig2_int;
    Signal0<string>			sig0_string;
    Signal1<string, string>		sig1_string;
    Signal2<string, string, string>	sig2_string;

    sig0_void.connect(&func0_void);
    sig1_void.connect(&func1_void);
    sig2_void.connect(&func2_void);
    sig0_int.connect(&func0_int);
    sig1_int.connect(&func1_int);
    sig2_int.connect(&func2_int);
    sig0_string.connect(&func0_string);
    sig1_string.connect(&func1_string);
    sig2_string.connect(&func2_string);

    sig0_void.connect(struct0_void());
    sig1_void.connect(struct1_void());
    sig2_void.connect(struct2_void());
    sig0_int.connect(struct0_int());
    sig1_int.connect(struct1_int());
    sig2_int.connect(struct2_int());
    sig0_string.connect(struct0_string());
    sig1_string.connect(struct1_string());
    sig2_string.connect(struct2_string());

    class0_void i1; sig0_void.connect(&i1, &class0_void::method);
    class1_void i2; sig1_void.connect(&i2, &class1_void::method);
    class2_void i3; sig2_void.connect(&i3, &class2_void::method);
    class0_int i4; sig0_int.connect(&i4, &class0_int::method);
    class1_int i5; sig1_int.connect(&i5, &class1_int::method);
    class2_int i6; sig2_int.connect(&i6, &class2_int::method);
    class0_string i7; sig0_string.connect(&i7, &class0_string::method);
    class1_string i8; sig1_string.connect(&i8, &class1_string::method);
    class2_string i9; sig2_string.connect(&i9, &class2_string::method);

    sig0_void();
    sig1_void(1);
    sig2_void(32, "32");
    sig0_int();
    sig1_int(1);
    sig2_int(43, 20);
    sig0_string();
    sig1_string("Hello World!");
    sig2_string("Hi", "Bye");

    // with "default_result"
    sig0_int(0);
    sig1_int(1, 0);
    sig2_int(43, 20, 0);
    sig0_string("");
    sig1_string("Hello World!", "");
    sig2_string("Hi", "Bye", "");
  }

}

//////////////////////////////////////////////////////////////////////

namespace test1 {
  
  int func1_counter = 0;
  int op1_counter = 0;
  int ok_counter = 0;
  int cancel_counter = 0;

  void func1()
  {
    func1_counter++;
  }

  struct op1
  {
    int a;
    op1(int a) : a(a) { }
    void operator()() {
      if (a == 9)
	op1_counter++;
    }
  };

  class Q
  {
    int v;
  public:
    Q(int v) : v(v) { }
    void ok() { if (v == 1) ok_counter++; }
    void cancel() { if (v == 2) cancel_counter++; }
  };

  void test_void()
  {
    Signal0<void> s;
    {
      Slot0<void>* a = s.connect(&func1);
      s();
      s();
      s.disconnect(a);
      delete a;
    }
    s();
    s.connect(op1(9));
    s();
    {
      Q q1(1);
      Q q2(2);
      Slot0<void>* b = s.connect(&q1, &Q::ok);
      Slot0<void>* c = s.connect(&q2, &Q::cancel);
      s();
    }
    assert(func1_counter == 2);
    assert(op1_counter == 2);
    assert(ok_counter == 1);
    assert(cancel_counter == 1);
  }

}

//////////////////////////////////////////////////////////////////////

namespace test2 {

  int return_4() { return 4; }
  int return_6() { return 6; }

  void test_int()
  {
    Signal0<int> s;
    s.connect(&return_4);
    assert(s() == 4);
    s.connect(&return_6);
    assert(s() == 6);		// last returned value
  }

}

//////////////////////////////////////////////////////////////////////

namespace test3 {

  void return_check_arg_eq_5(int arg)
  {
    assert(arg == 5);
  }

  struct checker
  {
    int a;
    checker(int a) : a(a) { }
    void operator()(int b) {
      assert(a < b);
    }
  };

  void test_void_int()
  {
    Signal1<void, int> s;
    Slot1<void, int>* a = s.connect(&return_check_arg_eq_5);
    s(5);
    s.disconnect(a);
    delete a;

    s.connect(checker(4));
    s(5);
    s.connect(checker(9));
    s(10);
  }

}

//////////////////////////////////////////////////////////////////////

namespace test4 {

  struct members_eq_args
  {
    int a, b;
    members_eq_args(int a, int b) : a(a), b(b) { }
    void operator()(int c, int d) {
      assert(a+b == c+d);
    }
  };

  int sum_args(int a, int b)
  {
    return a+b;
  }

  class sum_of_squares
  {
  public:
    int sum;
    sum_of_squares() : sum(0) { }
    int add(int x, int m) {
      sum += (x-m)*(x-m);
      return sum;
    }
  };

  void test_void_int_int()
  {
    Signal2<void, int, int> s1;
    s1.connect(members_eq_args(2, 4));
    s1(3, 3);
    s1.connect(members_eq_args(3, 3));
    s1.connect(members_eq_args(-1, 7));
    s1(5, 1);

    Signal2<int, int, int> s2;
    s2.connect(&sum_args);
    assert(s2(2, 4) == 6);

    Signal2<int, int, int> s3;
    sum_of_squares i;
    s3.connect(&i, &sum_of_squares::add);
    assert(s3(4, 1) == 9);
    assert(s3(5, 0) == 9+25);
  }

}

//////////////////////////////////////////////////////////////////////

namespace test5 {

  int multiply_args(int x, int y) { return x*y; }

  class dot_product {
    int u, v;
  public:
    dot_product(int u, int v) : u(u), v(v) { }
    int operator()(int x, int y) { return x*u + y*v; }
  };

  struct sum
  {
    int& operator()(int& result, int newres) {
      result += newres;
      return result;
    }
  };

  void test_mergers()
  {
    Signal2<int, int, int> s;
    s.connect(&multiply_args);
    assert(s(2, 4) == 8);

    s.connect(dot_product(6, -1));
    s.connect(dot_product(3, 2));
    assert(s(5, 6, 0, sum()) == (5*6) + (6*5 + -1*6) + (3*5 + 2*6));
  }

}

//////////////////////////////////////////////////////////////////////

int main()
{
  test0::test_coverage();
  test1::test_void();
  test2::test_int();
  test3::test_void_int();
  test4::test_void_int_int();
  test5::test_mergers();
  return 0;
}
