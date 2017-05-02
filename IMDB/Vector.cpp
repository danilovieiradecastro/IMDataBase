#include "stdafx.h"

#include "Vector.h"
#include <stdlib.h> 
No::No()
{
	ptrAnterior= NULL;
	ptrPosterior= NULL;
	ptrValue = NULL;
}

No::~No()
{
	if (this->key)
	  free(this->key);
}
char * No::getKey()
{
	return this->key;
}
No* No::next()
{
	return this->ptrPosterior;
}
Vector::Vector()
{
	 ptrStart = NULL;
	 ptrEnd = NULL;
	 this->count = 0;
}
int Vector::Count()
{
	return this->count;
}

void Vector::add( std::string key, void* ptrValue)
{
	No* valAdd = new No();
	if (key.size() >0)
	{
		char* v = (char*)calloc(key.size()+1,1);
		memcpy(v,key.c_str(),key.size());
		valAdd->key = (char*)	v;
	}else
	valAdd->key  =NULL;
	valAdd->ptrValue = ptrValue;
	if (ptrEnd != NULL)
	{
		ptrEnd->ptrPosterior = valAdd;
		valAdd->ptrAnterior = ptrEnd;
		ptrEnd = valAdd;
	}
	else
	{
		if (!ptrStart)
		  ptrStart = valAdd;
		ptrEnd  = valAdd;
	}
	 this->count++;


}
void Vector::remove(std::string key)
{
	No* val = findNode(key);
	if (val)
	{
		No* aux = val->ptrAnterior;
		if (aux)
		{
			aux->ptrPosterior = val->ptrPosterior;
			if (aux->ptrPosterior)
			{
			aux->ptrPosterior->ptrAnterior = aux;
			}
			else
			{
				ptrEnd = aux;
			}
		}
		else
		{
			ptrStart = val->ptrPosterior;
			if (!val->ptrPosterior)
			{
		      ptrEnd = aux;
			}
		}

		delete val;
		this->count--;
	}

}
void* Vector::removerPrimeiro()
{
	 No* aux = ptrStart;
	 void* ret = NULL;
	 if (aux )
	 {
		 ret = aux->ptrValue;
		 ptrStart = aux->ptrPosterior;
		 if (ptrStart)
			 ptrStart->ptrAnterior = NULL;
		 delete aux;
		 this->count--;
	 }
	 return ret;
}

No* Vector::first()
{
	 return ptrStart;
}
No* Vector::findNode(std::string key)
{
	 No* aux = ptrStart;
	 while (aux)
	 {
		 
		  if (strcmp(aux->key,key.c_str()) == 0)
		  {
			  break;
		  }
		  aux = aux->ptrPosterior;
	 }
	 return aux;
}
void* Vector::find(std::string key)
{

	No* val = findNode(key);
	if (val)
	  return val->ptrValue;
	else
	  return NULL;
}
