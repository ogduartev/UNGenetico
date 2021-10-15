#ifndef _GENREAL_CPP
#define _GENREAL_CPP
#endif

#ifndef _GENREAL_HPP
#include "genreal.hpp"
#endif

//////////////////////// GEN REAL (double) //////////////////////

GenReal::GenReal()
{
	Minimo=0.0;
	Maximo=1.0;
	Valor=0.5;
}

void GenReal::crearAleatorio()
{
	double azar;
	azar=(double)((double)rand()/(double)RAND_MAX);
	Valor=Minimo+azar*(Maximo-Minimo);
	if(Valor<Minimo){Valor=Minimo;}
	if(Valor>Maximo){Valor=Maximo;}
}

OperadorMutacion *GenReal::operadorMutacionDefecto()
{
	OperadorMutacion *OM;
	OM=new OperadorMutacionRealUniforme;
	return OM;
}

OperadorCruce *GenReal::operadorCruceDefecto()
{
	OperadorCruce *OC;
	OC=new OperadorCruceRealPlano;
	return OC;
}


//////////////////////// Operadores de Mutación //////////////////////

void OperadorMutacionRealUniforme::mutarGen(Gen *g)
{
	GenReal *gr;
	gr=(GenReal*)g;
	double azar;
	azar=(double)((double)rand()/(double)RAND_MAX);
	gr->Valor=gr->Minimo+azar*(gr->Maximo-gr->Minimo);
	if(gr->Valor<gr->Minimo){gr->Valor=gr->Minimo;}
	if(gr->Valor>gr->Maximo){gr->Valor=gr->Maximo;}
}

void OperadorMutacionRealNoUniforme::mutarGen(Gen *g)
{
	GenReal *gr;
	gr=(GenReal*)g;

	int t,T;
	double r,b,y,delta;

	t=AG->Generacion;
	T=AG->GeneracionMaxima;
	b=B;
	double azar;
	azar=(double)((double)rand()/(double)RAND_MAX);
	r=azar;
	azar=(double)((double)rand()/(double)RAND_MAX);
	if(azar<0.5)
	{
		y=gr->Maximo-gr->Valor;
		delta=y*(1.0-pow(r,pow((1.0-t/T),b)));
		gr->Valor=gr->Valor+delta;
	}else
	{
		y=gr->Valor-gr->Minimo;
		delta=y*(1.0-pow(r,pow((1.0-t/T),b)));
		gr->Valor=gr->Valor-delta;
	}

	if(gr->Valor<gr->Minimo){gr->Valor=gr->Minimo;}
	if(gr->Valor>gr->Maximo){gr->Valor=gr->Maximo;}
}

void OperadorMutacionRealBGA::mutarGen(Gen *g)
{
	GenReal *gr;
	gr=(GenReal*)g;
	double azar;
	double rango,alfa,gamma=0.0;
	rango=Factor*(gr->Maximo-gr->Minimo);

	int i;
	for(i=0;i<16;i++)
	{
		azar=(double)((double)rand()/(double)RAND_MAX);
		if(azar<0.06125){alfa=1.0;}else{alfa=0.0;}
		gamma+=alfa*pow(2,-1*i);
	}
	azar=(double)((double)rand()/(double)RAND_MAX);
	if(azar<0.5)
	{
		gr->Valor=gr->Valor+rango*gamma;
	}else
	{
		gr->Valor=gr->Valor-rango*gamma;
	}

	if(gr->Valor<gr->Minimo){gr->Valor=gr->Minimo;}
	if(gr->Valor>gr->Maximo){gr->Valor=gr->Maximo;}
}

//////////////////////// Operadores de Cruce //////////////////////

void OperadorCruceRealPlano::cruzarGenes(Gen *m,Gen *p, Arreglo<Gen> *h, int numHijos)
{
	GenReal *madre,*padre;
	madre=(GenReal*)m;
	padre=(GenReal*)p;
	Arreglo<GenReal> hijos;
	int i,tam;
	for(i=0;i<numHijos;i++)
	{
		GenReal *gr;
		gr=new GenReal;

		double azar;
		azar=(double)((double)rand()/(double)RAND_MAX);
		gr->Valor=madre->Valor+azar*(padre->Valor-madre->Valor);
		gr->Minimo=madre->Minimo;
		gr->Maximo=madre->Maximo;
		if(gr->Valor<gr->Minimo){gr->Valor=gr->Minimo;}
		if(gr->Valor>gr->Maximo){gr->Valor=gr->Maximo;}

		hijos.Add(gr);
	}

	tam=hijos.GetItemsInContainer();
	for(i=0;i<tam;i++)
	{
		h->Add(hijos.dato(i));
	}
	hijos.FlushDetach();
}

void OperadorCruceRealAritmetico::cruzarGenes(Gen *m,Gen *p, Arreglo<Gen> *h, int numHijos)
{
	GenReal *madre,*padre;
	madre=(GenReal*)m;
	padre=(GenReal*)p;
	Arreglo<GenReal> hijos;
	double alfa;
	int i,tam;
	for(i=0;i<numHijos;i++)
	{
		if(i%2 == 0){alfa=Lambda;}else{alfa=1.0-Lambda;}
		GenReal *gr;
		gr=new GenReal;

		gr->Valor=alfa*madre->Valor+(1.0-alfa)*(padre->Valor);

		hijos.Add(gr);
	}

	tam=hijos.GetItemsInContainer();
	for(i=0;i<tam;i++)
	{
		h->Add(hijos.dato(i));
	}
	hijos.FlushDetach();
}

void OperadorCruceRealBLX::cruzarGenes(Gen *m,Gen *p, Arreglo<Gen> *h, int numHijos)
{
	GenReal *madre,*padre;
	madre=(GenReal*)m;
	padre=(GenReal*)p;
	Arreglo<GenReal> hijos;
	int i,tam;
	for(i=0;i<numHijos;i++)
	{
		GenReal *gr;
		gr=new GenReal;

		double azar;
		azar=(double)((double)rand()/(double)RAND_MAX);
		gr->Valor=madre->Valor+azar*(padre->Valor-madre->Valor);

		hijos.Add(gr);
	}

	tam=hijos.GetItemsInContainer();
	for(i=0;i<tam;i++)
	{
		h->Add(hijos.dato(i));
	}
	hijos.FlushDetach();
}

void OperadorCruceRealLineal::cruzarGenes(Gen *m,Gen *p, Arreglo<Gen> *h, int numHijos)
{
	GenReal *madre,*padre;
	madre=(GenReal*)m;
	padre=(GenReal*)p;
	Arreglo<GenReal> hijos;
	int i,tam;
	for(i=0;i<numHijos;i++)
	{
		GenReal *gr;
		gr=new GenReal;

		double azar;
		azar=(double)((double)rand()/(double)RAND_MAX);
		gr->Valor=madre->Valor+azar*(padre->Valor-madre->Valor);

		hijos.Add(gr);
	}

	tam=hijos.GetItemsInContainer();
	for(i=0;i<tam;i++)
	{
		h->Add(hijos.dato(i));
	}
	hijos.FlushDetach();
}

void OperadorCruceRealDiscreto::cruzarGenes(Gen *m,Gen *p, Arreglo<Gen> *h, int numHijos)
{
	GenReal *madre,*padre;
	madre=(GenReal*)m;
	padre=(GenReal*)p;
	Arreglo<GenReal> hijos;
	int i,tam;
	for(i=0;i<numHijos;i++)
	{
		GenReal *gr;
		gr=new GenReal;

		double azar;
		azar=(double)((double)rand()/(double)RAND_MAX);
		gr->Valor=madre->Valor+azar*(padre->Valor-madre->Valor);

		hijos.Add(gr);
	}

	tam=hijos.GetItemsInContainer();
	for(i=0;i<tam;i++)
	{
		h->Add(hijos.dato(i));
	}
	hijos.FlushDetach();
}

void OperadorCruceRealIntermedioExtendido::cruzarGenes(Gen *m,Gen *p, Arreglo<Gen> *h, int numHijos)
{
	GenReal *madre,*padre;
	madre=(GenReal*)m;
	padre=(GenReal*)p;
	Arreglo<GenReal> hijos;
	int i,tam;
	for(i=0;i<numHijos;i++)
	{
		GenReal *gr;
		gr=new GenReal;

		double azar;
		azar=(double)((double)rand()/(double)RAND_MAX);
		gr->Valor=madre->Valor+azar*(padre->Valor-madre->Valor);

		hijos.Add(gr);
	}

	tam=hijos.GetItemsInContainer();
	for(i=0;i<tam;i++)
	{
		h->Add(hijos.dato(i));
	}
	hijos.FlushDetach();
}

void OperadorCruceRealHeuristico::cruzarGenes(Gen *m,Gen *p, Arreglo<Gen> *h, int numHijos)
{
	GenReal *madre,*padre;
	madre=(GenReal*)m;
	padre=(GenReal*)p;
	Arreglo<GenReal> hijos;
	int i,tam;
	for(i=0;i<numHijos;i++)
	{
		GenReal *gr;
		gr=new GenReal;

		double azar;
		azar=(double)((double)rand()/(double)RAND_MAX);
		gr->Valor=madre->Valor+azar*(padre->Valor-madre->Valor);

		hijos.Add(gr);
	}

	tam=hijos.GetItemsInContainer();
	for(i=0;i<tam;i++)
	{
		h->Add(hijos.dato(i));
	}
	hijos.FlushDetach();
}


