#ifndef _GENINT_CPP
#define _GENINT_CPP
#endif

#ifndef _GENINT_HPP
#include "genint.hpp"
#endif

//////////////////////// GEN REAL (double) //////////////////////

GenEntero::GenEntero()
{
	Minimo=-10;
	Maximo=10;
	Valor=0;
}

void GenEntero::crearAleatorio()
{
	double azar;
	azar=(double)((double)rand()/(double)RAND_MAX);
	Valor=(long)(Minimo+azar*(Maximo-Minimo));
	if(Valor<Minimo)Valor=Minimo;
	if(Valor>Maximo)Valor=Maximo;
}

OperadorMutacion *GenEntero::operadorMutacionDefecto()
{
	OperadorMutacion *OM;
	OM=new OperadorMutacionEnteroUniforme;
	return OM;
}

OperadorCruce *GenEntero::operadorCruceDefecto()
{
	OperadorCruce *OC;
	OC=new OperadorCruceEnteroPlano;
	return OC;
}

//////////////////////// Operadores de Mutación //////////////////////

void OperadorMutacionEnteroUniforme::mutarGen(Gen *g)
{
	GenEntero *gr;
	gr=(GenEntero*)g;
	double azar;
	azar=(double)((double)rand()/(double)RAND_MAX);
	gr->Valor=(long)(gr->Minimo+azar*(gr->Maximo-gr->Minimo));
	if(gr->Valor<gr->Minimo)gr->Valor=gr->Minimo;
	if(gr->Valor>gr->Maximo)gr->Valor=gr->Maximo;
}

void OperadorMutacionEnteroNoUniforme::mutarGen(Gen *g)
{
	GenEntero *gr;
	gr=(GenEntero*)g;

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

void OperadorMutacionEnteroBGA::mutarGen(Gen *g)
{
	GenEntero *gr;
	gr=(GenEntero*)g;
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

void OperadorCruceEnteroPlano::cruzarGenes(Gen *m,Gen *p, Arreglo<Gen> *h, int numHijos)
{
	GenEntero *madre,*padre;
	madre=(GenEntero*)m;
	padre=(GenEntero*)p;
	Arreglo<GenEntero> hijos;
	int i,tam;
	for(i=0;i<numHijos;i++)
	{
		GenEntero *gr;
		gr=new GenEntero;

		double azar;
		azar=(double)((double)rand()/(double)RAND_MAX);
		gr->Valor=madre->Valor+azar*(padre->Valor-madre->Valor);
		gr->Minimo=madre->Minimo;
      gr->Maximo=madre->Maximo;
		if(gr->Valor<gr->Minimo)gr->Valor=gr->Minimo;
		if(gr->Valor>gr->Maximo)gr->Valor=gr->Maximo;

		hijos.Add(gr);
	}

	tam=hijos.GetItemsInContainer();
	for(i=0;i<tam;i++)
	{
		h->Add(hijos.dato(i));
	}
	hijos.FlushDetach();
}

void OperadorCruceEnteroAritmetico::cruzarGenes(Gen *m,Gen *p, Arreglo<Gen> *h, int numHijos)
{
	GenEntero *madre,*padre;
	madre=(GenEntero*)m;
	padre=(GenEntero*)p;
	Arreglo<GenEntero> hijos;
	double alfa;
	int i,tam;
	for(i=0;i<numHijos;i++)
	{
		if(i%2 == 0){alfa=Lambda;}else{alfa=1.0-Lambda;}
		GenEntero *gr;
		gr=new GenEntero;

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

void OperadorCruceEnteroBLX::cruzarGenes(Gen *m,Gen *p, Arreglo<Gen> *h, int numHijos)
{
	GenEntero *madre,*padre;
	madre=(GenEntero*)m;
	padre=(GenEntero*)p;
	Arreglo<GenEntero> hijos;
	int i,tam;
	for(i=0;i<numHijos;i++)
	{
		GenEntero *gr;
		gr=new GenEntero;

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

void OperadorCruceEnteroLineal::cruzarGenes(Gen *m,Gen *p, Arreglo<Gen> *h, int numHijos)
{
	GenEntero *madre,*padre;
	madre=(GenEntero*)m;
	padre=(GenEntero*)p;
	Arreglo<GenEntero> hijos;
	int i,tam;
	for(i=0;i<numHijos;i++)
	{
		GenEntero *gr;
		gr=new GenEntero;

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

void OperadorCruceEnteroDiscreto::cruzarGenes(Gen *m,Gen *p, Arreglo<Gen> *h, int numHijos)
{
	GenEntero *madre,*padre;
	madre=(GenEntero*)m;
	padre=(GenEntero*)p;
	Arreglo<GenEntero> hijos;
	int i,tam;
	for(i=0;i<numHijos;i++)
	{
		GenEntero *gr;
		gr=new GenEntero;

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

void OperadorCruceEnteroIntermedioExtendido::cruzarGenes(Gen *m,Gen *p, Arreglo<Gen> *h, int numHijos)
{
	GenEntero *madre,*padre;
	madre=(GenEntero*)m;
	padre=(GenEntero*)p;
	Arreglo<GenEntero> hijos;
	int i,tam;
	for(i=0;i<numHijos;i++)
	{
		GenEntero *gr;
		gr=new GenEntero;

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

void OperadorCruceEnteroHeuristico::cruzarGenes(Gen *m,Gen *p, Arreglo<Gen> *h, int numHijos)
{
	GenEntero *madre,*padre;
	madre=(GenEntero*)m;
	padre=(GenEntero*)p;
	Arreglo<GenEntero> hijos;
	int i,tam;
	for(i=0;i<numHijos;i++)
	{
		GenEntero *gr;
		gr=new GenEntero;

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

