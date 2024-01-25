#include <iostream>
class m{
	virtual void fn()=0;
};
class a:virtual public m{
	void fn()override{std::cout<<"a";}
};
class b:virtual public m{
	void fn()override{std::cout<<"b";}
};
class t:public a,public b{
	
};
int main(){
	t a();
}
