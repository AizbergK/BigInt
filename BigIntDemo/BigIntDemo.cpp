import BigInt;
import std;

int main()
{
	long long llb{ 123 };
	BigInt a{ "348975" }, b{ llb };
	
	std::cout << "a	:" << a << std::endl;
	std::cout << "b	:" << b << std::endl;
	std::cout << "sum	:" << a + b << std::endl;
	std::cout << "subt	:" << a - b << std::endl;
	std::cout << "mult	:" << a * b << std::endl;
	std::cout << "div	:" << a / b << std::endl;
	std::cout << "mod	:" << a % b << std::endl;

	return 0;
}