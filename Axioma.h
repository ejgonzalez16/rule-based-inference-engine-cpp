#ifndef _Axioma_h_
#define _Axioma_h_

#include "Afirmacion.h"
#include <vector>
#include <map>

class Axioma {
private:
	std::vector<Afirmacion> afirmaciones;
	bool isIgualValorPredicado(std::vector<ValorPredicado> valores1, std::vector<ValorPredicado> valores2);
public:
	Axioma();
	Axioma(std::vector<Afirmacion> afirmaciones);
	std::vector<Afirmacion> &getAfirmaciones();
	void setAfirmaciones(std::vector<Afirmacion> afirmaciones);
	bool encontrarYEliminarAfirmacionesNegadas(std::vector<Afirmacion> afirmacionNegada, int k,
		std::vector<Afirmacion>& afirmacionesExcluidas);
	void insertarAfirmacionesMenosUnas(std::vector<Afirmacion> afirmacionesAInsertar, std::vector<Afirmacion> afirmacionesExcluidas);
	std::string getPredicado(int posAfirmacion);
	int sizeValores(int posAfirmacion);
	std::string getNombreVal(int posAfirmacion, int posVal);
	void setNombreVal(int posAfirmacion, int posVal, std::string nombreVal);
	int sizeAfirmaciones();
	std::string getValorVar(int posAfirmacion, int posVal);
	void setValorVar(int posAfirmacion, int posVal, std::string valorVar);
	bool valorValido(ValorPredicado valorPredicado);
	bool isPredicadoVariablesEncontradas(std::vector<ValorPredicado> valores1, std::vector<ValorPredicado> valores2);
	bool isAxiomaVarCompleto();
};
#endif 