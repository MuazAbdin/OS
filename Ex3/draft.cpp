#include <iostream>
#include <vector>

typedef std::pair<int, int> IntPair;
typedef std::vector<IntPair> IntPairVec;

int main(int argc, char** argv) {

//	std::cout << "num = " << std::bitset<64>((uint64_t) 1 << 62) << std::endl;
//	std::cout << "num = " << std::bitset<64>((uint64_t) 2 << 62) << std::endl;
//	std::cout << "num = " << std::bitset<64>((uint64_t) 3 << 62) << std::endl;

	IntPairVec intVec;
	for (int i = 0; i <= 3; ++i)
	{
		IntPair intPair1(i, i+1);
		IntPair intPair2(i, i+2);
		IntPair intPair3(i, i+3);
		intVec.push_back(intPair1);
		intVec.push_back(intPair2);
		intVec.push_back(intPair3);
	}

	std::cout << "ALL PAIRS VEC" << std::endl;
	for (auto pair: intVec) {
		std::cout << " (" << pair.first << ", " << pair.second << ") ";
	}
	std::cout << "\n=================" << std::endl;

	std::vector<IntPairVec> shuffled;
	while (!intVec.empty()) {
		IntPair uniPair = intVec.back();
		IntPairVec temp;
		while(!(uniPair.first < intVec.back().first) && !(intVec.back().first < uniPair.first)) {
			IntPair p = intVec.back();
			intVec.pop_back();
			temp.push_back(p);
		}
		shuffled.push_back(temp);
	}

	std::cout << "VEC OF VECs" << std::endl;
	for (auto vec: shuffled) {
		std::cout << "VEC OF " << vec.at(0).first << " -> ";
		for (auto pair: vec) {
			std::cout << " (" << pair.first << ", " << pair.second << ") ";
		}
		std::cout << std::endl;
	}
	std::cout << "=================" << std::endl;


}

