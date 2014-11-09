
class Address {
public:
    Address(string fileName, int offset): fileName(fileName), offset(offset) {}
	string getfileName() { return fileName; }
	int getOffset() { return offset; }
	
private:
	string fileName;
	int offset;
}
