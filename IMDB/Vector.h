#ifndef VECTOR_H
#define VECTOR_H
#include <string>
class No{
friend class Vector;
	No* ptrAnterior;
	No* ptrPosterior;
	char* key;
	
public :
	No* next();
	void* ptrValue;
	No();	
	~No();
	char* getKey();
};

class Vector
{
private:
	    No* ptrStart;
		No* ptrEnd;
		No* findNode(std::string key);
		int count;
public:
	No* first();
	No* last();
	Vector();
	~Vector();
	void add(std::string key, void* ptrValue);
	void remove(std::string key);
	void* find(std::string key);
	int Count();
	
void* removerPrimeiro();
};

#endif