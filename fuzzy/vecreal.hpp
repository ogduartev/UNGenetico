#ifndef _VECREAL_CPP
#define _VECREAL_CPP
#endif

#ifndef _GENREAL_HPP
#include "genreal.hpp"
#endif

#ifndef _GENBOOL_HPP
#include "genbool.hpp"
#endif

#ifndef _GENINT_HPP
#include "genint.hpp"
#endif

class VectorReal:public Individuo
{
public:
	VectorReal(int dim);
	~VectorReal();

	Individuo* crearCopia();
	void copiarDetalles(Individuo *other);
	double objetivo();
	void codificar();
	void decodificar();

	void operadorMutacion(OperadorMutacion *Op);
	void operadorCruce(OperadorCruce *Op);

	int Dimension;
	double *x;
	long Offset;
	int Bandera;

	void mostrar();
};


