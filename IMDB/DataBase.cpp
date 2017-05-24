#include "stdafx.h" 
#ifdef MEDIR_TEMPO
#include <ctime>
#endif



void Tabela::setNome(std::string val)
{
	 this->Nome = val;
	
}
std::string Tabela::getNome()
{
	return this->Nome;
}
Tabela::Tabela()
{
	 this->listaColunas = new Vector();
	 this->listaLinhasSemIndex= new Vector();
	 this->rowCount=0;
	 this->indexPK = NULL;
	 this->idxColunas = NULL;
	 this->listaFKs = new Vector();
}
Tabela::~Tabela()
{
}
//metodo que adiciona a correlação entre o nome da coluna o eo index do array que o dado estará
void Tabela::addColumn(std::string name)
{
	this->listaColunas->add(name,(void*)(uintptr_t)this->listaColunas->Count());
}
//retorna qual o index do array que a informação estará armazenada.
int Tabela::getColumnIndex(std::string name)
{
	int idx = (int)(intptr_t) this->listaColunas->find(name);
	if (idx == 0)
	{
		No* n = this->listaColunas->first();
		if (!n || std::string(n->getKey()) != name)
		{
			idx = -1;
		}
	}
	return idx;
}
//retorna o total de colunas da tabela
int Tabela::getCoulumCount()
{
	return 	 this->listaColunas->Count();
}
//retorno o numero de registros na tabela
int Tabela::getRowCount()
{
	 return this->rowCount;
}
//overload o metodo abaixo, este recebe um array de strings e a quantidade de registros e armazena da forma correta
void Tabela::addRow(std::string row[], int size)
{
	int idx = 0;
	std::string data = "";
	while (idx < size)
	{
		data += row[idx];
		if (idx < size)
			data += "\t";
		idx++;
	}
	if (data != "") 
		this->addRow(data);
}
//realiza o select count por alguma coluna
int Tabela::countByColumn(std::string columName, std::string keyValue)
{//realiza o select count por alguma coluna
#ifdef MEDIR_TEMPO
	std::clock_t start = std::clock();
#endif
#ifdef 	COLETAR_ESTATISTICA
	int tot = this->getRowCount() / 10;
	double d = 0;
#endif
	int columnsFind = 0;
	int idxColuna =this->getColumnIndex(columName);
	if (idxColuna > -1)
	{
		if (this->indexPK)
		{
		
			if (this->countColunasIdx == 1 && this->idxColunas[0] == idxColuna)
			{// o usuario está buscando pela PK da tabela.

				if (this->indexPK->getByString(keyValue))
				{
					columnsFind++;
				}
			}
			else
			{
#ifdef 	COLETAR_ESTATISTICA
				std::clock_t startMed = std::clock();
				int totalVer = 0;
#endif
				ResgistroHash* r = this->indexPK->getFirstRegister();

				while (r)
				{
					LinhaTabela* l = (LinhaTabela*)r->getValue();
					if (l->getString(idxColuna) == keyValue)
					{
						columnsFind++;
					}
#ifdef 	COLETAR_ESTATISTICA
					if (totalVer%tot == 0)
					{// 10 %
						double duration = (std::clock() - startMed) / (double)CLOCKS_PER_SEC;
						d += duration;
						std::cout << "t " << " " << totalVer <<" " << d << std::endl;
						startMed = std::clock();
					}
					totalVer++;
#endif
					r = this->indexPK->getNextRegister(r);
				}
			}

		}
	}

#ifdef MEDIR_TEMPO
	double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
   std::cout << "tempo de busca na tabela " << " " << duration << " ms" << std::endl;
#endif
	return columnsFind;

}
//cria a estrutura da FK na tabela
void Tabela::createFK(std::string * colunas, int colCount, Tabela * tabelaDestino)
{
	ForeingKey *fk = new ForeingKey(colunas, colCount, tabelaDestino);
	this->listaFKs->add(tabelaDestino->getNome(), fk);
}

//metodo que realiza o inner join
std::string Tabela::innerJoin(Tabela * tabDest, bool onlyCount)
{
#ifdef MEDIR_TEMPO
	std::clock_t start = std::clock();
#endif
#ifdef 	COLETAR_ESTATISTICA
	int tot = this->getRowCount() / 10;
	double d = 0;
#endif
	std::string retorno;
	ForeingKey* fk = (ForeingKey*)this->listaFKs->find(tabDest->getNome());
	int count = 0;
	if (fk)
	{
#ifdef 	COLETAR_ESTATISTICA
		std::clock_t startMed = std::clock();
		int totalVer = 0;
#endif
		ResgistroHash* r = this->indexPK->getFirstRegister();
		int x = 0;
		while (r)
		{
			LinhaTabela* l = (LinhaTabela*)r->getValue();
			std::string chaveBusca = "";
			x = 0;
			while (x < fk->getKeyCount())
			{
				std::string key = fk->getKeys()[x];
				int idxColuna = this->getColumnIndex(key);
				chaveBusca += l->getString(idxColuna);
				x++;
			}
			std::string Aux = tabDest->findRowInternal(chaveBusca);
			if (Aux != "")
			{ 
				if (onlyCount)
				{
					count++;
				}
				else
		    	retorno += l->dataArray + Aux +"\r\n";
			}
#ifdef 	COLETAR_ESTATISTICA
			if (totalVer%tot == 0)
			{// 10 %
				double duration = (std::clock() - startMed) / (double)CLOCKS_PER_SEC;
				d += duration;
				std::cout << "t " << " " << totalVer << " " << d << std::endl;
				startMed = std::clock();
			}
			totalVer++;
#endif
			r = this->indexPK->getNextRegister(r);
		}


		if (onlyCount)
		{
			
			retorno = std::to_string(count) + " Registros Encontrados \r\n";
		}
		else
		{


		}
	}
	else
	{
		retorno = "Não existe FK para as tabelas";
	}
#ifdef MEDIR_TEMPO
	double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << "tempo do inner join " << " " << duration << " ms" << std::endl;
#endif
	return retorno;
}

//metodo que realiza o left join
std::string Tabela::leftJoin(Tabela * tabDest, bool onlyCount)
{
#ifdef MEDIR_TEMPO
	std::clock_t start = std::clock();
#endif
#ifdef 	COLETAR_ESTATISTICA
	int tot = this->getRowCount() / 10;
	double d = 0;
#endif
	std::string retorno;
	ForeingKey* fk = (ForeingKey*)this->listaFKs->find(tabDest->getNome());
	int count = 0;
	if (fk)
	{
#ifdef 	COLETAR_ESTATISTICA
		std::clock_t startMed = std::clock();
		int totalVer = 0;
#endif
		ResgistroHash* r = this->indexPK->getFirstRegister();
		int x = 0;
		while (r)
		{
			LinhaTabela* l = (LinhaTabela*)r->getValue();
			std::string chaveBusca = "";
			x = 0;
			while (x < fk->getKeyCount())
			{
				std::string key = fk->getKeys()[x];
				int idxColuna = this->getColumnIndex(key);
				chaveBusca += l->getString(idxColuna);
				x++;
			}
			std::string Aux = tabDest->findRowInternal(chaveBusca);
			
			if (onlyCount)
			{
				count++;
			}
			else
				retorno += l->dataArray + Aux + "\r\n";
#ifdef 	COLETAR_ESTATISTICA
			if (totalVer%tot == 0)
			{// 10 %
				double duration = (std::clock() - startMed) / (double)CLOCKS_PER_SEC;
				d += duration;
				std::cout << "t " << " " << totalVer << " " << d << std::endl;
				startMed = std::clock();
			}
			totalVer++;
#endif
			r = this->indexPK->getNextRegister(r);
		}


		if (onlyCount)
		{

			retorno = std::to_string(count) + " Registros Encontrados \r\n";
		}

	}
	else
	{
		ResgistroHash* r = this->indexPK->getFirstRegister();
		int x = 0;
		while (r)
		{
			LinhaTabela* l = (LinhaTabela*)r->getValue();
			std::string chaveBusca = "";
			
			if (onlyCount)
			{
				count++;
			}
			else
				retorno += l->dataArray + std::string("\r\n");

			r = this->indexPK->getNextRegister(r);
		}
		if (onlyCount)
		{

			retorno = std::to_string(count) + " Registros Encontrados \r\n";
		}
	}
#ifdef MEDIR_TEMPO
	double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << "tempo do left outer join " << " " << duration << " ms" << std::endl;
#endif
	return retorno;
}
//metodo que realiza o right join
std::string Tabela::rightJoin(Tabela * tabDest, bool onlyCount)
{
#ifdef MEDIR_TEMPO
	std::clock_t start = std::clock();
#endif
#ifdef 	COLETAR_ESTATISTICA
	int tot = this->getRowCount() / 10;
	double d = 0;
	std::clock_t startMed = std::clock();
	int totalVer = 0;
#endif
	std::string retorno;
	ForeingKey* fk = (ForeingKey*)this->listaFKs->find(tabDest->getNome());
	int count = 0;
	
	if (fk)
	{
		tabDest->indexPK->clearVisiteds();//marca todos os registros como não visitados
		ResgistroHash* r = this->indexPK->getFirstRegister();
		int x = 0;
		while (r)
		{
			LinhaTabela* l = (LinhaTabela*)r->getValue();
			std::string chaveBusca = "";
			x = 0;
			while (x < fk->getKeyCount())
			{
				std::string key = fk->getKeys()[x];
				int idxColuna = this->getColumnIndex(key);
				chaveBusca += l->getString(idxColuna);
				x++;
			}
			std::string Aux = tabDest->findRowInternal(chaveBusca,true);
			if (Aux != "")
			{// conta só os registros em ambas as tabelas.
				if (onlyCount)
				{
					count++;
				}
				else
					retorno += l->dataArray + Aux + "\r\n";
			}
#ifdef 	COLETAR_ESTATISTICA
			if (totalVer%tot == 0)
			{// 10 %
				double duration = (std::clock() - startMed) / (double)CLOCKS_PER_SEC;
				d += duration;
				std::cout << totalVer << "\t" << d << std::endl;
				startMed = std::clock();
			}
			totalVer++;
#endif
			r = this->indexPK->getNextRegister(r);
		}

#ifdef 	COLETAR_ESTATISTICA
		tot = tabDest->getRowCount() / 10;
		d = 0;
		startMed = std::clock();
		totalVer = 0;
		std::cout << "segunda etapa..." << std::endl;
#endif

		//itera na tabela da direita para adicionar os registros não contados antes
		if (tabDest->indexPK)
		{
			ResgistroHash* r = tabDest->indexPK->getFirstRegister();
			while (r)
			{
				if (!r->isVisited())
				{
					if (onlyCount)
					{
						count++;
					}
					else
					{
						retorno += ((LinhaTabela*)r->getValue())->dataArray;
						retorno += "\r\n";
					}
				}
		
#ifdef 	COLETAR_ESTATISTICA
				if (totalVer%tot == 0)
				{// 10 %
					double duration = (std::clock() - startMed) / (double)CLOCKS_PER_SEC;
					d += duration;
					std::cout << totalVer << "\t" << d << std::endl;
					startMed = std::clock();
				}
				totalVer++;
#endif
				r = tabDest->indexPK->getNextRegister(r);
			}

		}

		if (onlyCount)
		{

			retorno = std::to_string(count) + " Registros Encontrados \r\n";
		}

	}
	else
	{
		
	}
#ifdef MEDIR_TEMPO
	double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << "tempo do right outer join " << " " << duration << " ms" << std::endl;
#endif
	return retorno;
}

//adicona uma string separado por \t no buffer da tabela
//este metodo deverá ser utilizado quando estamos importando do arquivo ou quando estamos inserindo o dado já com os separadores
void Tabela::addRow(std::string row)
{

	LinhaTabela* addRow = new LinhaTabela(this->getCoulumCount());
	char* v = (char*)calloc(row.size() + 1, 1);
	memcpy(v, row.c_str(), row.size());
	addRow->dataArray = v;
	if (this->indexPK != NULL)
	{//verifica se a tabela possui algumaColuna de index para adicionar indexado

		std::string key = "";
		int i = 0;
		while (i < this->countColunasIdx)
		{
			key+= addRow->getString(this->idxColunas[i]);
			i++;
		}
		this->indexPK->insertString(key, addRow);

	}
	else
	{//a tabela não possui index então colocamos em uma lista encadeada simples
		this->listaLinhasSemIndex->add("",addRow);
	}
	rowCount++; 
}
//remove um registro da tabela
bool Tabela::deleteRow(std::string chave)
{
#ifdef MEDIR_TEMPO
	std::clock_t start = std::clock();
#endif
	bool retorno = false;
	if (this->indexPK != NULL)
	{
		retorno =this->indexPK->removeByString(chave);
		if (retorno)//decrementa o contatodor total
			rowCount--;
	}
#ifdef MEDIR_TEMPO
	double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << "tempo de remoção da tabela " << " " << duration << " ms" << std::endl;
#endif
	return retorno;
}
//metodo auxiliar interno para buscar a linha sem exibir mensagem
std::string Tabela::findRowInternal(std::string chave,bool makrRegister)
{
	std::string retorno = "";
	using namespace std;
	if (this->indexPK == NULL)
	{
		cout << "tablea nao possui Primary Key " << endl;

	}
	else
	{
		
		ResgistroHash* r = this->indexPK->getRegisterByString(chave);
		
		
		if (r)
		{
			std::string ret(((LinhaTabela*)r->getValue())->dataArray);
			if (makrRegister)
			{
				r->setVisited(true);
			}
			retorno = ret;
		}
		else
		{
			//	cout <<"chave não encontrada "<<endl;
		}
	}
	return retorno;
}

//metodo de busca na tabela, atualmente só busca através da chave primára e se foi especificado a primary key corretamente.
std::string Tabela::findRow(std::string chave)
{
#ifdef MEDIR_TEMPO
		 std::clock_t start = std::clock();
#endif
		 std::string retorno = "";
		 retorno=  this->findRowInternal(chave);
#ifdef MEDIR_TEMPO
	double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	std::cout<<"tempo de busca na tabela "<<" "<<duration<<" ms"<<std::endl;
#endif
	return retorno;
}
//metodo que move todos os registros que estão inseridos na tabela, para o index
//este metodo é utilizado na importação do arquivo, pois a criacao da chave acontece posteriormente as inserções.
void Tabela::popularIndex(int idxcolunas[],int size)
{
	

#ifdef COUNT_COLISAO
	int total = this->listaLinhasSemIndex->Count();
	std::ofstream out("processamento.txt", std::ofstream::out | std::ofstream::app);
	out << "INSERINDO TABELA " << this->getNome() <<" Qauntidade de linhas: "<<total << std::endl;
	int cnt = 0;
	total = total / 10;
#endif
	LinhaTabela* linha = (LinhaTabela*) this->listaLinhasSemIndex->removerPrimeiro();
	std::string val;
	int idx = 0;
	this->idxColunas = idxcolunas;
	this->countColunasIdx = size;
	while(linha)
	{
		idx = 0;
		val = "";
		while (size > idx)
		{
			val += linha->getString(idxcolunas[idx]);
			idx++;
			
		}
#ifdef COUNT_COLISAO
		cnt++;
		if ((cnt % total) == 0)
		{// a cada 100 linhas 
			out << "total Colisoes para " << cnt << ";" << this->getTotalColisoes() << std::endl;
		}
#endif
		this->indexPK->insertString(val,linha);
        linha  =(LinhaTabela*) this->listaLinhasSemIndex->removerPrimeiro();
	}
#ifdef COUNT_COLISAO
	out.close();
#endif 
}
//metodo responsavel pela criação do index na tavela a partir de uma serie de colunas
void Tabela::createIndex(std::string columns[],int size)
{
	if (this->indexPK == NULL)
	{//cria o index com a quantidade de registros já inseridos na tabela
		this->indexPK = new HashMap((int)std::floor(this->rowCount*1.3));
	
	 int* idxColunas = (int*)malloc(size*sizeof(int));
	 int count =0;
	 while (size > count)
	 {
		std::string nm_coluna = columns[count];

		int idx = this->getColumnIndex(nm_coluna);
		idxColunas[count] = idx;
		count++;
	 }
		popularIndex(idxColunas,size);



	}

}

#ifdef COUNT_COLISAO
//totaliza as colisões existentes na tabela, total
int Tabela::getTotalColisoes()
{
	int retorno = -1;
	int count =0;
	if ( this->indexPK)
	{
		ResgistroHash* r = this->indexPK->getFirstRegister();
		retorno = 0;
	    while (r)
		  {
			count++;
			  retorno+= r->getColisionCount();
			  r= this->indexPK->getNextRegister(r);
		  }
	 
	}
	return retorno;
}
//retona a chave que tem o maior numero de colisões.
int Tabela::getMaximoColisoesPorChave(std::string&chave)
{
	int retorno = -1;
	if ( this->indexPK)
	{
	ResgistroHash* r = this->indexPK->getFirstRegister();
	  if (r)
	  {
		  retorno =r->getColisionCount();
		  chave =  std::string(((LinhaTabela*) r->getValue())->dataArray);
		 
		  while ( this->indexPK->getNextRegister(r))
		  {
			  if (retorno  < r->getColisionCount())
			  {
				  retorno =r->getColisionCount();
				    chave =  std::string(((LinhaTabela*) r->getValue())->dataArray);
			  }
			  r=  this->indexPK->getNextRegister(r);
		  }
	  }
	}
	return retorno;

}
#endif



LinhaTabela::LinhaTabela(int size)
{
}
LinhaTabela::~LinhaTabela()
{
}
//retorna o conteudo da lina relativa a coluna.
std::string LinhaTabela::getString(int idxColuna)
{
	using namespace std;
	std::istringstream iss(std::string(this->dataArray));
	string colum;
	 while(idxColuna >-1 && getline(iss, colum, '\t'))
	{//um novo registro da linha
		//l->colunas[siz] = (char*)itemTabela.c_str();
		//coluna[siz] = itemTabela;
		// siz++;
		idxColuna--;
	}
	 size_t tam = colum.find_first_not_of(" ");
	 colum =  colum.substr(tam,colum.size() - tam); //trim inicio
	 tam = colum.find_first_of(" ");
	 colum =  colum.substr(0,tam);
	 return colum;
}

DataBase::DataBase(void)
{
	listaTabelas = new Vector();
}


DataBase::~DataBase(void)
{
}
//cria uma instancia de tabela.
Tabela* DataBase::criarTabela(std::string nomeTabela)
{
	Tabela*  tabl = new Tabela();
	tabl->setNome(nomeTabela);
	this->listaTabelas->add(tabl->getNome(),tabl);
	return tabl;

}
//reliza uma busca sequencial entre as tabelas.
Tabela* DataBase::findTable(std::string nomeTabela)
{
	 return (Tabela*)listaTabelas->find(nomeTabela);
}
//realiza o processamento do arquivo .sql do postgree.
void DataBase::criarDBArquivo(std::string path)
{
	using namespace std;
	string local = "usda.sql";
	if (path != "")
	{
		local = path;
	}
	 ifstream myfile(local.c_str());

	// myfile.open (local, std::ifstream::in);
	
  if (myfile)  // same as: if (myfile.good())
    {
		string line;
		string itemTabela;
		std::istringstream isst ;
		bool criandoTabela = false;
		bool inserindoTabela = false;
		bool alterandoTabela = false;
		std::size_t found = -1;
			std::size_t foundAux;
	Tabela* tabl = NULL;
	int linha =0;
	string coluna;
	char*pch;char*pchAux;
		string nomeTabela;
#ifdef MEDIR_TEMPO
		 std::clock_t start = std::clock();
#endif
		 
    while (getline( myfile, line ))  // same as: while (getline( myfile, line ).good())
      {
		  linha++;

		  if (!criandoTabela && !inserindoTabela&& !alterandoTabela)
		  {
			   found = line.find("CREATE TABLE");
			if (found !=  string::npos)
			{//encontrou o create table, procurar o nome
				found+= 12;
				foundAux =  line.find(" ",found+1);
				if (foundAux!=string::npos)
				{//já sabemos o nome da tabela aqui
					
					nomeTabela = line.substr(found+1,foundAux-(found+1));
					criandoTabela = true;
					continue;
				}
				 cout <<"create table sem o nome da tabela: "<<linha<<endl;
			}
			found =  line.find("COPY");
			if (found !=string::npos)
			{
				 string tabelaInseir = line.substr(found+5,line.size());
				 tabelaInseir = tabelaInseir.substr(0,tabelaInseir.find_first_of(" "));
			 	  tabl = this->findTable(tabelaInseir);
				  if (tabl)
				  inserindoTabela = true;
				  else
					cout <<"tablea nao encontrada para inserir: "<<linha<<endl;
				  continue;
			}
			found =  line.find("ALTER TABLE ONLY");
			if (found !=string::npos)
			{
				 string tabelaModificar = line.substr(found+17,line.size());
				 found = tabelaModificar.find_first_of(" ");
				 found =  found ==string::npos ? tabelaModificar.size():found;
				 tabelaModificar = tabelaModificar.substr(0,found);
			 	  tabl =  this->findTable(tabelaModificar);
				
				  if (tabl)
				  {
				  alterandoTabela = true;
				  }
				  else
				  {
					   cout <<"tablea nao encontrada para modificar: "<<linha<<endl;
				  }
				  
				  continue;
			}
		  }
		  else
		  {
			  if (criandoTabela)
			  {//preciso ler o nome das colunas
				  if (line.find(";")==string::npos)
				  {
					  if (tabl == NULL)
						  tabl = criarTabela(nomeTabela);
					   found =  line.find_first_not_of(" ");
						coluna = line.substr(found,line.size());
						found = coluna.find_first_of(" ");
						coluna = coluna.substr(0,found);
					  tabl->addColumn(coluna);
					  continue;
				  }
				  else
				  {
					  //encerrou uma tabela
					  tabl = NULL;
					  criandoTabela= false;
					  continue;
				  }
			  }
			  else if (alterandoTabela)
			  {
				  found =  line.find("PRIMARY KEY (");
			      if (found !=string::npos)
				  {
						
#ifdef MEDIR_TEMPO
		 std::clock_t startIdx = std::clock();
#endif
					  string colunaModificar = line.substr(found+13,line.size()-(found+13)); 
					 colunaModificar = colunaModificar.substr(0,colunaModificar.find_last_of(")"));
					 pch = (char*) colunaModificar.c_str();
					 string* colunas = new string[tabl->getCoulumCount()];
					 int i =0;
					 while(pch)
					 {
						pchAux = strchr( pch,',');
						while (isspace(*pch))
								pch++;
						if (pchAux)
						{
							
						  colunas[i] =  colunaModificar.substr(pch -colunaModificar.c_str(),pchAux-pch);
						  pchAux++;
						}
						else
							colunas[i] =  colunaModificar.substr(pch -colunaModificar.c_str(),colunaModificar.size());
						
						pch = pchAux;
						i++;
					 }
				

					
						  tabl->createIndex(colunas,i);
						
#ifdef MEDIR_TEMPO
	double duration = ( std::clock() - startIdx ) ;
	cout<<"tempo de popular index tabela "<< tabl->getNome()<<" "<<duration<<endl;

#endif
#ifdef COUNT_COLISAO
	cout<<"total Colisoes "<< tabl->getTotalColisoes()<<" "<<endl;
	std::string row;
	int cnt =tabl->getMaximoColisoesPorChave(row);
	cout<<"chave com mais colisoes  "<<cnt <<" "<<row<<endl;
#endif
				
					 alterandoTabela =false;
				  }
				  else if ((found =line.find("FOREIGN KEY (") )!= string::npos)
				  {//precisamos criar uma FK desta tabela;

					  string colunaModificar = line.substr(found + 13, line.size() - (found + 13));
					  colunaModificar = colunaModificar.substr(0, colunaModificar.find_last_of(")"));
					  colunaModificar = colunaModificar.substr(0, colunaModificar.find_last_of(")"));
					  pch = (char*)colunaModificar.c_str();
					  string* colunasOrigem = new string[tabl->getCoulumCount()];
					  int i = 0;
					  while (pch)
					  {
						  pchAux = strchr(pch, ',');
						  while (isspace(*pch))
							  pch++;
						  if (pchAux)
						  {

							  colunasOrigem[i] = colunaModificar.substr(pch - colunaModificar.c_str(), pchAux - pch);
							  pchAux++;
						  }
						  else
							  colunasOrigem[i] = colunaModificar.substr(pch - colunaModificar.c_str(), colunaModificar.size());

						  pch = pchAux;
						  i++;
					  }

					  found = line.find("REFERENCES ");
					  string tabelaModificar = line.substr(found + 11, line.size());
					  found = tabelaModificar.find_first_of("(");
					  found = found == string::npos ? tabelaModificar.size() : found;
					  tabelaModificar = tabelaModificar.substr(0, found);
					  Tabela* tabDest = this->findTable(tabelaModificar);
					  if (tabDest)
					  {
						  tabl->createFK(colunasOrigem, i, tabDest);
					  }
					  else
					  { 
						  cout << "Erro aou criar a FK";
					  }
					  alterandoTabela = false;
				  }
				 
			  }
		  }



		  if (inserindoTabela && line.find("\\.")==string::npos)//estou lendo uma linha de inserção
		  {

			  tabl->addRow(line);   
		  }
		  else if(inserindoTabela)
		  {
			   inserindoTabela = false;
		  }
      }
    myfile.close();
	//delete( myfile);
#ifdef MEDIR_TEMPO
	double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	cout<<"tempo de leitura do arquivo "<<duration<<endl;
	No* noBusca = listaTabelas->first();
	while (noBusca)
	{
		cout<<"tabela: "<<((Tabela*)noBusca->ptrValue)->getNome()<< " colunas: "<<((Tabela*)noBusca->ptrValue)->getCoulumCount()<<" linhas: "<<((Tabela*)noBusca->ptrValue)->getRowCount()<<endl; 
		noBusca =noBusca->next();
 	 
	}
#endif
    }
  else cout <<"não foi possivel abrir o arquivo"<<endl;

}

ForeingKey::ForeingKey(std::string * keys, int size, Tabela * tab)
{
	this->keys = keys;
	this->keysCount = size;
	this->tabDest = tab;
}

ForeingKey::~ForeingKey()
{
}

std::string* ForeingKey::getKeys()
{
	return this->keys;
}

int ForeingKey::getKeyCount()
{
	return this->keysCount;
}
