#include "stdafx.h"
#include "HashMap.h"



ResgistroHash::ResgistroHash(unsigned int  key, void* val) 
{
    this->key = key;
    this->value = val;
    this->next = NULL;
#ifdef COUNT_COLISAO
	  colisionCount = 0;
#endif
	  this->visited = false;
}
//retorna a chave associada ao registro
unsigned int ResgistroHash::getKey() 
{
    return key;
}
//retorna o pronteiro para o objeto armazenado.
void* ResgistroHash::getValue() 
{
    return value;
}
//associa o ponteiro de objeto ao registro
void ResgistroHash::setValue(void* val) 
{
    this->value = val;
}
//retorna o proximo resgistro encadeado, caso tenha ocrrido colisões anteriormente
ResgistroHash *ResgistroHash::getNext() 
{
    return next;
}
bool ResgistroHash::isVisited()
{
	return this->visited;
}
void ResgistroHash::setVisited(bool val)
{
	this->visited = val;
}
//adiciona um registro encadeado em caso de colisão, sempre no ultimo.
void ResgistroHash::setNext(ResgistroHash *next) 
{
    this->next = next;
}

#ifdef COUNT_COLISAO
//preenche o contador de colisões
void ResgistroHash::addColision(int val )
{
	this->colisionCount = val;
}
//retorna o contador de colisões
int ResgistroHash::getColisionCount()
{
	return this->colisionCount ;
}
#endif
//contrutor que aceita como parametro um valor inicial
HashMap::HashMap(int sizeInicial) 
{

	if (sizeInicial <128)
	{ 
		sizeInicial = 128;
	}
	// calcula um valor impar como inicial
	sizeInicial = ((sizeInicial / (int)2) * 2) + 1;
    limite = 0.80f;
    maxSize =(int)(sizeInicial* limite);
    tableSize = sizeInicial;
    size = 0;
    table = new ResgistroHash*[tableSize];
      for (int i = 0; i < tableSize; i++)
          table[i] = NULL;
	//memset(table,NULL,sizeof(ResgistroHash)*tableSize);
}
     
HashMap::~HashMap() 
{
	for (int hash = 0; hash < tableSize; hash++)
	{
		if (table[hash] != NULL) 
		{
			ResgistroHash *prevEntry = NULL;
			ResgistroHash *resgistro = table[hash];
			while (resgistro != NULL) 
			{
				prevEntry = resgistro;
				resgistro = resgistro->getNext();
				delete prevEntry;
			}
		}
	}
    delete[] table;
}
//atualiza o limeite de ocupação da tabela
void HashMap::setLimite(float limite) 
{
    this->limite = limite;
    maxSize = (int) (tableSize * limite);
}
void HashMap::clearVisiteds()
{
	ResgistroHash* r =  this->getFirstRegister();
	while (r)
	{
		r->setVisited(false);
		r = this->getNextRegister(r);
	}
}
//metodo que aumenta o tamanho da lista, caso a ocupção tenha passado do limite.
//metodo doubra o tamanho da lista mais 1 registro.
void HashMap::resize() 
{
    int oldTableSize = tableSize;
    tableSize *= 2;
	tableSize += 1;
    maxSize = (int) (tableSize * limite);
    ResgistroHash **oldTable = table;
    table = new ResgistroHash*[tableSize];
    for (int i = 0; i < tableSize; i++)
            table[i] = NULL;
	//memset(table,NULL,sizeof(ResgistroHash)*tableSize);
    size = 0;
    for (int hash = 0; hash < oldTableSize; hash++)
	{
	    if (oldTable[hash] != NULL) 
		{
			ResgistroHash *oldReg;
            ResgistroHash *resgistro = oldTable[hash];
            while (resgistro != NULL) 
			{
				insert(resgistro->getKey(), resgistro->getValue());
                oldReg = resgistro;
                resgistro = resgistro->getNext();
                delete oldReg;
            }
        }
	}
    delete[] oldTable;
}
 //realiza a busca por uma chave de string.
void*	HashMap::getByString(std::string key)
{
	unsigned int intKey = convertStringToInt(key);
	ResgistroHash* reg = get(intKey);
	if (reg)
	{
		return reg->getValue();
	}
	else
	  return reg;
}
ResgistroHash*	HashMap::getRegisterByString(std::string key)
{
	unsigned int intKey = convertStringToInt(key);
	
	return get(intKey);
}

//realiza a busca pela chave calculada internamente e transforma em inteiro.
//retorna o objeto armazenado.
ResgistroHash* HashMap::get(unsigned int  key)
{
    int hash = (key % tableSize);
    if (table[hash] == NULL)
            return NULL;
    else 
	{
        ResgistroHash *resgistro = table[hash];
        while (resgistro != NULL && resgistro->getKey() != key)
            resgistro = resgistro->getNext();
        
            return resgistro;
    }
}

// transforma uma string em inteiro gerando um hash utilizado a função escrita por Robert Sedgwick's no livro "Algorithms in C" 
 unsigned int HashMap::convertStringToInt(std::string key)
 {
	 //usando o RSHASH
  unsigned int b    = 378551;
    unsigned int a    = 63689;
    unsigned int hash = 0;

    for(std::size_t i = 0; i < key.length(); i++)
    {
        hash = hash * a + key[i];
        a    = a * b;
    }

    return hash ;//(hash & 0x7FFFFFFF);
 }
 //adiciona um novo item associado com uma chave em string
void HashMap::insertString(std::string key, void* val)
{// overload para inserir como string
	unsigned int intKey = convertStringToInt(key);
	insert(intKey,val);
}
//retorna o primeiro registro no vetor interno. 
//metodo utilizado para contar colisões.
 ResgistroHash* HashMap::getFirstRegister()
 {
	 int idx =0;
	 while (table[idx] == NULL && idx <=tableSize)
	 {
		 idx++;
	 }
	 	 if (idx>=tableSize)
		 return NULL;
	 return table[idx];

 }
 //retorna o proximo registro a partir do informado.
 //utilizado para contar o numero de colisões.
 int val = 0;
  ResgistroHash* HashMap::getNextRegister(ResgistroHash* st)
 {
	 if (st == NULL)
		 return NULL;
	 int idx =0;
	 if (st->getNext())
	 {
		 val++;
		 return st->getNext();
	 }
	 idx = (st->getKey() % tableSize)+1;
	 while (table[idx] == NULL && idx <=tableSize)
	 {
		 idx++;
	 }
	 if (idx>=tableSize)
		 return NULL;
	 val++;
	 return table[idx];

 }
//adiciona um novo registro a partir de uma chave numerica.
void HashMap::insert(unsigned int key, void* val) 
{
    int hash = (key % tableSize);
    if (table[hash] == NULL) 
	{
            table[hash] = new ResgistroHash(key, val);
            size++;
    }
	else 
	{
#ifdef COUNT_COLISAO
     int colision = 1;
#endif
		
	  ResgistroHash *registro = table[hash];
        while (registro->getNext() != NULL)
		{
            registro = registro->getNext();
#ifdef COUNT_COLISAO
	 colision++;
#endif
		}
        if (registro->getKey() == key)
            registro->setValue(val);
        else 
		{
			 ResgistroHash * newReg  =new ResgistroHash(key, val);
            registro->setNext(newReg);
#ifdef COUNT_COLISAO
			newReg->addColision(colision);
#endif
            size++;
        }
    }
    if (size >= maxSize)
            resize();
}

bool HashMap::removeByString(std::string key)
{
	unsigned int intKey = convertStringToInt(key);
	return remove(intKey);
}

 //remove um registro da tabela, utilizand a chave inteira.
bool HashMap::remove(unsigned int  key) 
{
    int hash = (key % tableSize);
	bool retorno = false;
    if (table[hash] != NULL) 
	{
        ResgistroHash *prevEntry = NULL;
        ResgistroHash *resgistro = table[hash];
        while (resgistro->getNext() != NULL && resgistro->getKey() != key) 
		{
            prevEntry = resgistro;
            resgistro = resgistro->getNext();
        }
        if (resgistro->getKey() == key) 
		{
            if (prevEntry == NULL) 
			{
                ResgistroHash *nextEntry = resgistro->getNext();
                delete resgistro;
                table[hash] = nextEntry;
            } 
			else 
			{
                ResgistroHash *next = resgistro->getNext();
                delete resgistro;
                prevEntry->setNext(next);
            }
            size--;
			retorno = true;
        }
    }
	return retorno;
}


