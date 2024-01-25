#include <iostream>
class m{
	virtual void fn()=0;
};
class a:public m{
	virtual void fn()=0;
	void fn()override{std::cout<<"a";fn();}
};
class b:public a{
	void fn()override{std::cout<<"b";}
};
int main(){
	b t;
	t.fn();  //output:ab
}
