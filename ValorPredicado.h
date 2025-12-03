#ifndef _ValorPredicado_h_
#define _ValorPredicado_h_

#include <iostream>
#include <list>

class ValorPredicado {
private:
	//NombreVarConst F(X) valorVar marcos F(marcos)
	std::string nombreVarConst; //Constante o variable del predicado, si es variable comienza con MAYUS
	std::string valorVar; //Valor de la variable (en caso de que exista)
public:
	ValorPredicado(std::string nombreVarConst, std::string valorVar);
	std::string getnombreVarConst();
	void setNombreVarConst(std::string nombreVarConst);
	std::string getvalorVar();
	void setValorVar(std::string valorVar);
};

#endif 