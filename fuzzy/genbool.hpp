#ifndef _GENBOOL_HPP
#define _GENBOOL_HPP
#endif

#ifndef _GENETICO_HPP
#include"genetico.hpp"
#endif

//////////////////////// GEN BOOL (int) //////////////////////

class GenBool:public Gen
{
public:
	GenBool();
	~GenBool()
	{
	}

	int Valor;	// el valor es 0 ó 1
	void crearAleatorio();
	OperadorMutacion *operadorMutacionDefecto();
	OperadorCruce *operadorCruceDefecto();
};

//////////////////////// Operadores de Mutación //////////////////////

class OperadorMutacionBoolUniforme:public OperadorMutacion
{
public:
	OperadorMutacionBoolUniforme(){}
	~OperadorMutacionBoolUniforme(){}

	void mutarGen(Gen *g);
};

//////////////////////// Operadores de Cruce //////////////////////

class OperadorCruceBoolPlano:public OperadorCruce
{
public:
	OperadorCruceBoolPlano(){}
	~OperadorCruceBoolPlano(){}

	void cruzarGenes(Gen *madre,Gen *padre, Arreglo<Gen> *hijos, int numHijos);
};


