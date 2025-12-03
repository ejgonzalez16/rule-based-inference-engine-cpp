#include "Axioma.h"

Axioma::Axioma(){}

Axioma::Axioma(std::vector<Afirmacion> afirmaciones) {
	this->afirmaciones = afirmaciones;
}

std::vector<Afirmacion>& Axioma::getAfirmaciones() {
	return this->afirmaciones;
}

void Axioma::setAfirmaciones(std::vector<Afirmacion> afirmaciones) {
	this->afirmaciones = afirmaciones;
}

bool Axioma::encontrarYEliminarAfirmacionesNegadas(std::vector<Afirmacion> afirmacionNegada, int k,
												   std::vector<Afirmacion>& afirmacionesExcluidas) {
	bool valido = false;
	int i = 0;
	for (; i < this->afirmaciones.size(); i++) {
		for (int j = 0; j < afirmacionNegada.size(); j++) {
			//Comprobacion de coincidencia entre las afirmaciones del axioma actual y las negadas
			if (this->afirmaciones[i].getPredicado() == afirmacionNegada[j].getPredicado()
				&& this->afirmaciones[i].getNegado() != afirmacionNegada[j].getNegado() &&
				isIgualValorPredicado(afirmaciones[i].getValores(), afirmacionNegada[j].getValores())
				&& isPredicadoVariablesEncontradas(afirmaciones[i].getValores(), afirmacionNegada[j].getValores())) {
				//Se añade a las excluidas la negada
				afirmacionesExcluidas.push_back(afirmacionNegada[j]);
				this->afirmaciones.erase(this->afirmaciones.begin() + i);
				afirmacionNegada.erase(afirmacionNegada.begin() + j);
				j--;//Se decrementa j para compensar el cambio en el tamaño del vector
				i-=2;//Se decrementa i para compensar el cambio en el tamaño del vector y volver a revisar la afirmación actual
				valido = true;
				break;
			}
		}
	}
	//Comprobacion adicional para el caso en que solo hay una afirmación en ambos vectores
	if (this->afirmaciones.size() == afirmacionNegada.size() && this->afirmaciones.size() == 1 &&
		this->afirmaciones[0].getPredicado() == afirmacionNegada[0].getPredicado()
		&& this->afirmaciones[0].getNegado() != afirmacionNegada[0].getNegado() &&
		isIgualValorPredicado(afirmaciones[0].getValores(), afirmacionNegada[0].getValores())) {
		afirmacionesExcluidas.push_back(afirmacionNegada[0]);
		this->afirmaciones.erase(this->afirmaciones.begin());
	}
	return valido;
}

bool Axioma::isIgualValorPredicado(std::vector<ValorPredicado> valores1, std::vector<ValorPredicado> valores2) {
	if (valores1.size() != valores2.size()) {
		return false;
	}
	for (int i = 0; i < valores1.size(); i++) {
		//Comprobacion de igualdad entre los nombres de las variables o constantes y sus valores
		if (valores1[i].getnombreVarConst() != valores2[i].getnombreVarConst()
			&& valores1[i].getnombreVarConst() != valores2[i].getvalorVar() &&
			valores1[i].getvalorVar() != valores2[i].getnombreVarConst()) {
			return false;
		}
	}
	return true;
}

//Verifica si los valores de las variables de los axiomas son válidos.
bool Axioma::isPredicadoVariablesEncontradas(std::vector<ValorPredicado> valores1, std::vector<ValorPredicado> valores2) {
	if (valores1.size() != valores2.size()) {
		return false;
	}
	for (int i = 0; i < valores1.size(); i++) {
		if ((!valorValido(valores1[i]) || !valorValido(valores2[i]))) {
			return false;
		}
	}
	return true;
}

bool Axioma::valorValido(ValorPredicado valorPredicado) {
	if (std::islower(valorPredicado.getnombreVarConst()[0])) {
		return true;
	}
	if (valorPredicado.getvalorVar() != "") {
		return true;
	}
	return false;
}


void Axioma::insertarAfirmacionesMenosUnas(std::vector<Afirmacion> afirmacionesAInsertar, std::vector<Afirmacion> afirmacionesExcluidas) {
	bool enExcluida = false;
	for (int i = 0; i < afirmacionesAInsertar.size(); i++) {
		enExcluida = false;
		for (int j = 0; j < afirmacionesExcluidas.size() && !enExcluida; j++) {
			if (afirmacionesAInsertar[i].getPredicado() == afirmacionesExcluidas[j].getPredicado() &&
				afirmacionesAInsertar[i].getNegado() == afirmacionesExcluidas[j].getNegado() &&
				isIgualValorPredicado(afirmacionesAInsertar[i].getValores(), afirmacionesExcluidas[j].getValores())) {
				enExcluida = true;
			}
		}
		if(!enExcluida){
			this->afirmaciones.push_back(afirmacionesAInsertar[i]);
		}
	}
}

std::string Axioma::getPredicado(int posAfirmacion) {
	return this->afirmaciones[posAfirmacion].getPredicado();
}

int Axioma::sizeValores(int posAfirmacion) {
	return this->afirmaciones[posAfirmacion].getValores().size();
}

std::string Axioma::getNombreVal(int posAfirmacion, int posVal) {
	return this->afirmaciones[posAfirmacion].getValores()[posVal].getnombreVarConst();
}

int Axioma::sizeAfirmaciones() {
	return this->afirmaciones.size();
}

void Axioma::setValorVar(int posAfirmacion, int posVal, std::string valorVar) {
	this->afirmaciones[posAfirmacion].getValores()[posVal].setValorVar(valorVar);
}

std::string Axioma::getValorVar(int posAfirmacion, int posVal) {
	return this->afirmaciones[posAfirmacion].getValores()[posVal].getvalorVar();
}

void Axioma::setNombreVal(int posAfirmacion, int posVal, std::string nombreVal) {
	this->afirmaciones[posAfirmacion].getValores()[posVal].setNombreVarConst(nombreVal);
}

bool Axioma::isAxiomaVarCompleto() {
	for (int i = 0; i < this->afirmaciones.size(); i++) {
		for (int j = 0; j < afirmaciones[i].getValores().size(); j++) {
			if (!valorValido(afirmaciones[i].getValores()[j])) {
				return  false;
			}
		}
	}
	return true;
}