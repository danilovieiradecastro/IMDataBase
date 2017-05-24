#ifndef HASH_H
#define HASH_H

class ResgistroHash {
private:
      unsigned int  key;
      void* value;
      ResgistroHash *next;
#ifdef COUNT_COLISAO
	  short colisionCount;
#endif
	  bool visited;
public:
      ResgistroHash(unsigned int  key, void* val);
      unsigned int getKey();
 
      void* getValue();
 
      void setValue(void* value);
 
      ResgistroHash *getNext();
	  bool isVisited();
	  void setVisited(bool val);
      void setNext(ResgistroHash *next);
	  #ifdef COUNT_COLISAO
	  void addColision(int val);
	  int getColisionCount();
#endif
};

 
class HashMap {
private:
      float limite;
      int maxSize;
      int tableSize;
      int size;
      ResgistroHash **table;
	 
 void	insert(unsigned int  key, void* val);
  bool remove(unsigned int  key) ;
  ResgistroHash* get(unsigned int  key);
      void resize();
 unsigned int convertStringToInt(std::string key);
public:
      HashMap(int sizeInicial);
	
	 void setLimite(float limite); 
	 void clearVisiteds();
      
	 void	insertString(std::string key, void* val);
      
	void*	getByString(std::string key);

	ResgistroHash* getRegisterByString(std::string key);
	bool removeByString(std::string key);
     ResgistroHash* getFirstRegister();
	   ResgistroHash* getNextRegister(ResgistroHash* st);
  
      ~HashMap();
};


#endif

