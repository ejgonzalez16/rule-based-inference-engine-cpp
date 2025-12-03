#ifndef _Afirmacion_h_
#define _Afirmacion_h_

#include <iostream>
#include <vector>
#include "ValorPredicado.h"

class Afirmacion {
private:
	std::string predicado;
	std::vector<ValorPredicado> valores;
	bool negado;
public:
	Afirmacion();
	Afirmacion(std::string predicado, std::vector<ValorPredicado> valores, bool negado);
	std::string getPredicado();
	std::vector<ValorPredicado> &getValores();
	bool getNegado();
	void setNegado(bool negado);
};

#endif 
