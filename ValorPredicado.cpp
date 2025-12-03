#include "ValorPredicado.h"

ValorPredicado::ValorPredicado(std::string nombreVarConst, std::string valorVar) {
	this->nombreVarConst = nombreVarConst;
	this->valorVar = valorVar;
}

std::string ValorPredicado::getnombreVarConst() {
	return this->nombreVarConst;
}

void ValorPredicado::setNombreVarConst(std::string nombreVarConst) {
	this->nombreVarConst = nombreVarConst;
}

std::string ValorPredicado::getvalorVar() {
	return this->valorVar;
}

 void ValorPredicado::setValorVar(std::string valorVar) {
	 this->valorVar = valorVar;
}