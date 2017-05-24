#ifndef DATABASE_H
#define DATABASE_H
#include <string>
#include "Vector.h"
#include "HashMap.h"
class Tabela;
class ForeingKey
{
	Tabela* tabDest;
	std::string* keys;
	int keysCount;
public:
	ForeingKey(std::string* keys, int size, Tabela* tab);
	~ForeingKey();
	std::string* getKeys();
	int getKeyCount();
};
class Tabela
{
	std::string Nome;
	Vector* listaColunas;
	Vector* listaLinhasSemIndex;
	Vector* listaFKs;
	int rowCount;
	HashMap* indexPK;
	void popularIndex(int idxcolunas[],int size);
	int* idxColunas;
	short countColunasIdx;
	std::string findRowInternal(std::string chave, bool makrRegister =false);
public:
	void setNome(std::string val);
	std::string getNome();
	Tabela();
	~Tabela();
	void addColumn(std::string name);
	int getCoulumCount();
	int getColumnIndex(std::string name);
	int getRowCount();
	void addRow(std::string row[],int size);
	void addRow(std::string row);
	bool deleteRow(std::string chave);
	void createIndex(std::string columns[],int size);
	std::string findRow(std::string chave);
	int countByColumn(std::string columName, std::string keyValue);
	void createFK(std::string* colunas, int colCount, Tabela* tabelaDestino);
	std::string innerJoin(Tabela* tabDest, bool onlyCount);
	std::string leftJoin(Tabela* tabDest, bool onlyCount);
	std::string rightJoin(Tabela* tabDest, bool onlyCount);
#ifdef COUNT_COLISAO
	int getTotalColisoes();

	int getMaximoColisoesPorChave(std::string&chave);
#endif

};
class LinhaTabela

{
		
public:
	char* dataArray;
	LinhaTabela(int size);
	~LinhaTabela();
	std::string getString(int idxColuna);
private:
	

};

class DataBase
{
	
private:
	Vector* listaTabelas;

public:
	DataBase(void);
	~DataBase(void);

	void criarDBArquivo(std::string caminhoArq);
	Tabela* criarTabela(std::string nomeTabela);
	Tabela* findTable(std::string nomeTabela);

};
#endif
