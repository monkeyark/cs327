#include <iostream>

class puzzle;

typedef puzzle (*func)(void);

class puzzle {
public:
  func operator()(int i);
};

puzzle function(void) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;

  return puzzle();
}

func puzzle::operator()(int i)
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;

  return function;
}

puzzle p;






































class pzzl_helper;

class pzzl {
public:
  pzzl_helper operator()(void);
};

class pzzl_helper {
public:
  pzzl operator()(int i);
};

pzzl_helper pzzl::operator()(void)
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;

  return pzzl_helper();
}

pzzl pzzl_helper::operator()(int i)
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;

  return pzzl();
}









































class pzl {
public:
  pzl operator()() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    return *this;
  }
};

int main(int argc, char *argv[])
{
  pzl p1;

  p1();
  p1()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()()();


  return 0;
}
