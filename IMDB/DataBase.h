#ifndef DATABASE_H
#define DATABASE_H
#include <string>
#include "Vector.h"
#include "HashMap.h"
class Tabela
{
	std::string Nome;
	Vector* listaColunas;
	Vector* listaLinhasSemIndex;
	int rowCount;
	HashMap* indexPK;
	void popularIndex(int idxcolunas[],int size);
	int* idxColunas;
	short countColunasIdx;
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
