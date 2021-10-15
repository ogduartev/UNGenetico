#ifndef _GENREAL_HPP
#define _GENREAL_HPP
#endif

#ifndef _GENETICO_HPP
#include"genetico.hpp"
#endif

//////////////////////// GEN REAL (double) //////////////////////

class GenReal:public Gen
{
public:
	GenReal();
	~GenReal()
	{
	}

	double Minimo,Maximo; // el valor por defecto está entre 0 y 1
	double Valor;
	void crearAleatorio();
	OperadorMutacion *operadorMutacionDefecto();
	OperadorCruce *operadorCruceDefecto();
};

//////////////////////// Operadores de Mutación //////////////////////

class OperadorMutacionRealUniforme:public OperadorMutacion
{
public:
	OperadorMutacionRealUniforme(){}
	~OperadorMutacionRealUniforme(){}

	void mutarGen(Gen *g);
};

class OperadorMutacionRealNoUniforme:public OperadorMutacion
{
public:
	OperadorMutacionRealNoUniforme()
	{
		B=0.5;
	}
	~OperadorMutacionRealNoUniforme(){}

	double B;

	void mutarGen(Gen *g);
};

class OperadorMutacionRealBGA:public OperadorMutacion
{
public:
	OperadorMutacionRealBGA()
	{
		Factor=0.1;
	}
	~OperadorMutacionRealBGA(){}

	double Factor;

	void mutarGen(Gen *g);
};

//////////////////////// Operadores de Cruce //////////////////////

class OperadorCruceRealPlano:public OperadorCruce
{
public:
	OperadorCruceRealPlano(){}
	~OperadorCruceRealPlano(){}

	void cruzarGenes(Gen *madre,Gen *padre, Arreglo<Gen> *hijos, int numHijos);
};

class OperadorCruceRealAritmetico:public OperadorCruce
{
public:
	OperadorCruceRealAritmetico()
	{
		Lambda=0.5;
	}
	~OperadorCruceRealAritmetico(){}

	double Lambda;

	void cruzarGenes(Gen *madre,Gen *padre, Arreglo<Gen> *hijos, int numHijos);
};

class OperadorCruceRealBLX:public OperadorCruce
{
public:
	OperadorCruceRealBLX()
	{
		Alfa=0.25;
	}
	~OperadorCruceRealBLX(){}

	double Alfa;

	void cruzarGenes(Gen *madre,Gen *padre, Arreglo<Gen> *hijos, int numHijos);
};

class OperadorCruceRealLineal:public OperadorCruce
{
public:
	OperadorCruceRealLineal(){}
	~OperadorCruceRealLineal(){}

	void cruzarGenes(Gen *madre,Gen *padre, Arreglo<Gen> *hijos, int numHijos);
};

class OperadorCruceRealDiscreto:public OperadorCruce
{
public:
	OperadorCruceRealDiscreto(){}
	~OperadorCruceRealDiscreto(){}

	void cruzarGenes(Gen *madre,Gen *padre, Arreglo<Gen> *hijos, int numHijos);
};

class OperadorCruceRealIntermedioExtendido:public OperadorCruce
{
public:
	OperadorCruceRealIntermedioExtendido()
	{
		Alfa=0.5;
	}
	~OperadorCruceRealIntermedioExtendido(){}

	double Alfa;

	void cruzarGenes(Gen *madre,Gen *padre, Arreglo<Gen> *hijos, int numHijos);
};

class OperadorCruceRealHeuristico:public OperadorCruce
{
public:
	OperadorCruceRealHeuristico(){}
	~OperadorCruceRealHeuristico(){}

	void cruzarGenes(Gen *madre,Gen *padre, Arreglo<Gen> *hijos, int numHijos);
};


