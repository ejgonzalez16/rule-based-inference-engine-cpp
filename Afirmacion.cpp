#include "Afirmacion.h"

Afirmacion::Afirmacion() {}

Afirmacion::Afirmacion(std::string predicado, std::vector<ValorPredicado> valores, bool negado) {
	this->predicado = predicado;
	this->valores = valores;
	this->negado = negado;
}

std::string Afirmacion::getPredicado() {
	return this->predicado;
}

std::vector<ValorPredicado> &Afirmacion::getValores() {
	return this->valores;
}

bool Afirmacion::getNegado() {
	return this->negado;
}

void Afirmacion::setNegado(bool negado) {
	this->negado = negado;
}