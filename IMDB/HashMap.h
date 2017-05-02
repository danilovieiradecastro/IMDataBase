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
public:
      ResgistroHash(unsigned int  key, void* val);
      unsigned int getKey();
 
      void* getValue();
 
      void setValue(void* value);
 
      ResgistroHash *getNext();
 
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
  void remove(unsigned int  key) ;
  void* get(unsigned int  key);
      void resize();
 unsigned int convertStringToInt(std::string key);
public:
      HashMap(int sizeInicial);
	
	 void setLimite(float limite); 
 
      
	 void	insertString(std::string key, void* val);
      
	void*	getByString(std::string key);

     ResgistroHash* getFirstRegister();
	   ResgistroHash* getNextRegister(ResgistroHash* st);
  
      ~HashMap();
};


#endif

