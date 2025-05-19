#include <cmath> //ceil, sqrt
#include <cstdlib> //strtoull
#include <bit> //countl_zero
#include <limits>
#include <iostream>
#include <fstream>

unsigned long customStrtoul(std::string s) { //returns 0 for all negative numbers; binds positive overflow to std::numeric_limits<unsigned long>::max(), informing the user of overflow
	int i=0,
		ret=0;

	//ignore leading whitespace
	while(i<s.length() && s[i]==' ')
		i++;
	
	//check for sign
	switch(s[i]) {
		case '-':
		return 0;
		case '+':
		i++;
	}
	
	//read and convert digits, clamp if necessary
	while(i<s.length() && s[i]=='0')
		i++;
	while(i<s.length() && s[i]>='0' && s[i]<='9') {
		if(ret < std::numeric_limits<unsigned long>::max()/10  ||
		(ret == std::numeric_limits<unsigned long>::max()/10  &&  s[i]-'0' <= std::numeric_limits<unsigned long>::max()%10)) {
			ret*=10;
			ret+=s[i]-'0';
		}
		else {
			std::cerr << "The entered number is above the maximum supported by this program. It was set to its allowed maximum (2^32-1)." << std::endl;
			return std::numeric_limits<unsigned long>::max();
		}
		i++;
	}

	return ret;
}

int binaryLength(unsigned long long a) {
	return 8*sizeof(a)-std::countl_zero(a);
}

int main(int argc, char* argv[]) { //optional arguments: 1.: number of square numbers to write to file (default 1'000), 2.: file name (default 'binarySquareList')
	const int defaultSquareNumberCount = 1'000;
	const char* defaultFileName = "binarySquareList";

	unsigned long squareNumberCount;
	const char* fileName;

	if(argc==1)
		squareNumberCount = defaultSquareNumberCount;
	else
		squareNumberCount = customStrtoul(argv[1]);
	const unsigned long long maxSquareNumber = static_cast<unsigned long long>(squareNumberCount)*squareNumberCount;
	
	if(argc>2)
		fileName = argv[2];
	else
		fileName = defaultFileName;

	if(squareNumberCount == 0) {
		std::cerr << "Error reading argument. Please enter a number between 1 and 4294967295 (2^32-1).";
		return 0;
	}
	
	{
		double memory = 0; //the file we create is going to need this much memory; at first, we use this variable to save the number of bits
		int i=1;
		unsigned long long powerOfTwo=1;
		while(powerOfTwo <= maxSquareNumber/2) {
			memory += (std::ceil(std::sqrt(2*powerOfTwo))-std::ceil(std::sqrt(powerOfTwo)))*i;

			i++;
			powerOfTwo*=2;
		}
		memory += (static_cast<double>(squareNumberCount)+1 - std::ceil(std::sqrt(powerOfTwo)))*i;
		memory = std::ceil(memory/8); //conversion from bits to bytes

		char sizeTable[] = "KMG"; //0 -> k, 1 -> M, 2 -> G
		int sizeIndex = -1;
		while(memory >= 1024) {
			memory /= 1024;
			sizeIndex++;
		}
		std::string memoryUnit;
		if(sizeIndex == -1)
			memoryUnit = "B";
		else {
			memoryUnit.append(1, sizeTable[sizeIndex]);
			memoryUnit.append("iB");
		}

		std::cout.precision(3);
		std::cout << "Writing square numbers of 1 (1) to " << squareNumberCount << " (" << maxSquareNumber << ")" << " to file " << fileName << "." << std::endl
							<< "This will take " << memory << " " << memoryUnit << " of memory." << std::endl
							<< "Continue? (y/n): ";
	}

	{
		char c;
		std::cin >> c;
		
		if(c=='n')
			return 0;
		else if(c!='y') {
			std::cerr << "Unexpected input character. Terminating program.";
			return 0;
		}
	}

	unsigned long long currentSquareNumber = 1;
	unsigned long currentNumberIndex = 1;	//cNI²=cSN

	unsigned char currentByte = 0;
	int unusedBits = 8;	//number of unused bits (out of 8) in currentByte


	std::ofstream of;
	of.open(fileName, std::fstream::binary | std::fstream::trunc);

	//loop runs once for every square number
	while (currentNumberIndex <= squareNumberCount) {
		//write current square number to bytes
		
		unsigned long long r = currentSquareNumber;
		//split square number to 1.: the first bits that go to currentByte, and 2.: the rest
		int length = binaryLength(r);
			//if number longer than bits left: shift to the right by [length - unusedBits]
			//continue writing until r doesn't have extra
		while (length > unusedBits) {
			currentByte |= r >> (length -= unusedBits);
			of << currentByte;

			r &= (1ull << length) - 1;

			currentByte = 0;
			unusedBits = 8;
		}
		//if number shorter than bits left: shift to the left by [unusedBits - length]
		currentByte |= r << (unusedBits -= length);
		if(unusedBits == 0) {
			of << currentByte;

			currentByte = 0;
			unusedBits = 8;
		}

		//calculate next square number, according to (n+1)**2 = (n**2) + (2*n + 1)
		currentSquareNumber += 2*currentNumberIndex+1;
		currentNumberIndex++;
	}

	//write remaining bits, leaving the last bits in the file as zeroes
	of << currentByte;

	of.close();

	return 0;
}
