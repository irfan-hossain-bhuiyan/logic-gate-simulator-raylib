#include <iostream>
#include <vector>
class oo {
public:
  virtual ~oo() { std::cout << "destructor callled  "; }
};

class test : virtual public oo {
private:
  int x;

public:
  test(int x) : x(x) {}
};

int main() {
	std::cout<<"started ";
  std::vector<test> a;
  {
    a.push_back(test(10));
    a.push_back(test(20));
  }
  std::cout<<"finished";
  
}
