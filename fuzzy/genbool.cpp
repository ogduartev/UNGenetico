#ifndef _GENBOOL_CPP
#define _GENBOOL_CPP
#endif

#ifndef _GENBOOL_HPP
#include"genbool.hpp"
#endif

//////////////////////// GEN BOOL (int) //////////////////////

GenBool::GenBool()
{
	Valor=0;
}

void GenBool::crearAleatorio()
{
	double azar;
	azar=(double)((double)rand()/(double)RAND_MAX);
	if(azar<0.5)
	{
		Valor=0;
	}else
	{
		Valor=1;
	}
}

OperadorMutacion *GenBool::operadorMutacionDefecto()
{
	OperadorMutacion *OM;
	OM=new OperadorMutacionBoolUniforme;
	return OM;
}

OperadorCruce *GenBool::operadorCruceDefecto()
{
	OperadorCruce *OC;
	OC=new OperadorCruceBoolPlano;
	return OC;
}

//////////////////////// Operadores de Mutación //////////////////////

void OperadorMutacionBoolUniforme::mutarGen(Gen *g)
{
	GenBool *gr;
	gr=(GenBool*)g;
	double azar;
	azar=(double)((double)rand()/(double)RAND_MAX);
	if(azar<0.5)
	{
		gr->Valor=0;
	}else
	{
		gr->Valor=1;
	}
}

//////////////////////// Operadores de Cruce //////////////////////

void OperadorCruceBoolPlano::cruzarGenes(Gen *m,Gen *p, Arreglo<Gen> *h, int numHijos)
{
	GenBool *madre,*padre;
	madre=(GenBool*)m;
	padre=(GenBool*)p;
	Arreglo<GenBool> hijos;
	int i,tam;
	for(i=0;i<numHijos;i++)
	{
		GenBool *gr;
		gr=new GenBool;

		double azar;
		azar=(double)((double)rand()/(double)RAND_MAX);
		if(azar<0.5)
		{
			gr->Valor=madre->Valor;
		}else
		{
			gr->Valor=padre->Valor;
		}

		hijos.Add(gr);
	}

	tam=hijos.GetItemsInContainer();
	for(i=0;i<tam;i++)
	{
		h->Add(hijos.dato(i));
	}
	hijos.FlushDetach();
}


