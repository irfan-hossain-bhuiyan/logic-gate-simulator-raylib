
#include <iostream>

class Derived; // Forward declaration of Derived class

class Base {
public:
	int a[32]={0}; 
  Base() { std::cout << "In base construction: " << this << "\n"; }
  virtual ~Base() { std::cout << "In base destruction: " << this << "\n"; }
};

class Derived : public virtual Base {
public:
	int b[100]={0};
  Derived() { std::cout << "In derived construction: " << this << "\n"; }
  ~Derived() { std::cout << "In derived destruction: " << this << "\n"; }
};

int main() {
  Derived *derivedptr = new Derived();
  delete derivedptr;
  return 0;
}
