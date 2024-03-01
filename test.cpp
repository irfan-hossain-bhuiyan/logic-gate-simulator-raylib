#include <functional>
template <typename T>
std::function<bool(T,T)> greater_than(std::function<int(T)> converter){
	return [converter](auto x,auto y){return converter(x)<converter(y);};
}
