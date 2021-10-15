#ifndef _GENINT_HPP
#define _GENINT_HPP
#endif

#ifndef _GENETICO_HPP
#include"genetico.hpp"
#endif

//////////////////////// GEN ENTERO (long) //////////////////////

class GenEntero:public Gen
{
public:
	GenEntero();
	~GenEntero()
	{
	}

	long Minimo,Maximo; // el valor por defecto está entre 0 y 1
	long Valor;
	void crearAleatorio();
	OperadorMutacion *operadorMutacionDefecto();
	OperadorCruce *operadorCruceDefecto();
};

//////////////////////// Operadores de Mutación //////////////////////

class OperadorMutacionEnteroUniforme:public OperadorMutacion
{
public:
	OperadorMutacionEnteroUniforme(){}
	~OperadorMutacionEnteroUniforme(){}

	void mutarGen(Gen *g);
};

class OperadorMutacionEnteroNoUniforme:public OperadorMutacion
{
public:
	OperadorMutacionEnteroNoUniforme()
	{
		B=0.5;
	}
	~OperadorMutacionEnteroNoUniforme(){}

	double B;

	void mutarGen(Gen *g);
};

class OperadorMutacionEnteroBGA:public OperadorMutacion
{
public:
	OperadorMutacionEnteroBGA()
	{
		Factor=0.1;
	}
	~OperadorMutacionEnteroBGA(){}

	double Factor;

	void mutarGen(Gen *g);
};

//////////////////////// Operadores de Cruce //////////////////////

class OperadorCruceEnteroPlano:public OperadorCruce
{
public:
	OperadorCruceEnteroPlano(){}
	~OperadorCruceEnteroPlano(){}

	void cruzarGenes(Gen *madre,Gen *padre, Arreglo<Gen> *hijos, int numHijos);
};

class OperadorCruceEnteroAritmetico:public OperadorCruce
{
public:
	OperadorCruceEnteroAritmetico()
	{
		Lambda=0.5;
	}
	~OperadorCruceEnteroAritmetico(){}

	double Lambda;

	void cruzarGenes(Gen *madre,Gen *padre, Arreglo<Gen> *hijos, int numHijos);
};

class OperadorCruceEnteroBLX:public OperadorCruce
{
public:
	OperadorCruceEnteroBLX()
	{
		Alfa=0.25;
	}
	~OperadorCruceEnteroBLX(){}

	double Alfa;

	void cruzarGenes(Gen *madre,Gen *padre, Arreglo<Gen> *hijos, int numHijos);
};

class OperadorCruceEnteroLineal:public OperadorCruce
{
public:
	OperadorCruceEnteroLineal(){}
	~OperadorCruceEnteroLineal(){}

	void cruzarGenes(Gen *madre,Gen *padre, Arreglo<Gen> *hijos, int numHijos);
};

class OperadorCruceEnteroDiscreto:public OperadorCruce
{
public:
	OperadorCruceEnteroDiscreto(){}
	~OperadorCruceEnteroDiscreto(){}

	void cruzarGenes(Gen *madre,Gen *padre, Arreglo<Gen> *hijos, int numHijos);
};

class OperadorCruceEnteroIntermedioExtendido:public OperadorCruce
{
public:
	OperadorCruceEnteroIntermedioExtendido()
	{
		Alfa=0.5;
	}
	~OperadorCruceEnteroIntermedioExtendido(){}

	double Alfa;

	void cruzarGenes(Gen *madre,Gen *padre, Arreglo<Gen> *hijos, int numHijos);
};

class OperadorCruceEnteroHeuristico:public OperadorCruce
{
public:
	OperadorCruceEnteroHeuristico(){}
	~OperadorCruceEnteroHeuristico(){}

	void cruzarGenes(Gen *madre,Gen *padre, Arreglo<Gen> *hijos, int numHijos);
};


