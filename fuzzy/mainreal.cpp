#ifndef _MAININT_CPP
#define _MAININT_CPP
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

	void mostrar();

	int Dimension;
	double *x;
};

VectorReal::VectorReal(int dim)
{
	Dimension=dim;
	x=new double[Dimension];
	for(int i=0;i<Dimension;i++)
	{
		GenReal *gen;
		gen= new GenReal;
		gen->Minimo=-2.0*Dimension;
		gen->Maximo=2.0*Dimension+1;
		Genoma.Add(gen);
	}
}

VectorReal::~VectorReal()
{
	Genoma.FlushDestroy();
	delete[] x;
}


Individuo* VectorReal::crearCopia()
{
	Individuo *Ind;
	Ind=new VectorReal(Dimension);
	return Ind;
}
/*
void VectorReal::copiarDetalles(Individuo *other)
{
	VectorReal *VR;
	VR=(VectorReal*)other;

	Dimension=VR->Dimension;
	Genoma.FlushDestroy();
	delete[] x;
	x=new long[Dimension];
	int i;
	for(i=0;i<Dimension;i++)
	{
		x[i]=VR->x[i];
		GenReal *gen;
		gen= new GenReal;
		Genoma.Add(gen);
	}

	codificar();
}
*/
void VectorReal::copiarDetalles(Individuo *other)
{
// Supone que los dos objetos son de la misma Dimensión
	VectorReal *VR;
	VR=(VectorReal*)other;
	for(int i=0;i<Dimension;i++)
	{
		x[i]=VR->x[i];
	}
	codificar();
}


void VectorReal::codificar()
{
	for(int i=0;i<Dimension;i++)
	{
		GenReal *g;
		g=(GenReal*)Genoma.dato(i);
		g->Valor=x[i];
	}
}

void VectorReal::decodificar()
{
	for(int i=0;i<Dimension;i++)
	{
		GenReal *g;
		g=(GenReal*)Genoma.dato(i);
		x[i]=g->Valor;
	}
}

double VectorReal::objetivo()
{
	double res=0.0;
	for(int i=0;i<Dimension;i++)
	{
		res+=((x[i]-(i))*(x[i]-(i)));
	}
	return sqrt(res);
}

void VectorReal::mostrar()
{
	calcularObjetivo();
	for(int i=0;i<Dimension;i++)
	{
		cout << x[i] << " ";
	}
	cout << ":" << Objetivo << "\n";
}

int main()
{
// Unas variables auxiliares:
	int Dim,Tamano,Generacion;
	cout << "Ejemplo de Aplicación de los Algoritmos Genéticos:\n\n";
	cout << "Dimensión del problema: ";cin >> Dim;
	cout << "Tamaño de la Población: ";cin >> Tamano;
	cout << "Número máximo de generaciones: ";cin >> Generacion;

// Definicion del algoritmo, y asignacion del modelo
// En esta forma se define el problema a optimizar:
// En este Ejemplo, "VectorReal" es una clase que
// hereda las propiedades de "Individuo"

	VectorReal *Mod;
	Mod=new VectorReal(Dim);

	AlgoritmoGenetico MiAg;
	
	MiAg.modelo(Mod);
	MiAg.TamanoPoblacion=Tamano;
	MiAg.GeneracionMaxima=Generacion;

// Ejecución del Algoritmo:
// Ejecución contínua inhabilitada
//	cout << "Optimizando...\n";
//	MiAg.optimizar();

// Utilizacion Paso a Paso del Algoritmo
	int c=0;
	MiAg.iniciarOptimizacion();
	do
	{
		MiAg.iterarOptimizacion();
		// aqui pueden emplearse los resultados de la iteracion
		// para cualquier cosa, por ejemplo:
		c++;cout << c << ":";
		VectorReal *VR;
		VR=(VectorReal*)MiAg.MejorEnLaHistoria;
		VR->mostrar();
	}while(!MiAg.parada());
return 0;
	MiAg.finalizarOptimizacion();
	cout << "\n";

	MiAg.GeneracionActual.ordenar(0);
	int casos;
	if (Tamano<5){casos=Tamano;}else{casos=5;}
	cout << "Estos son los mejores " << casos << " individuos:\n";
	for(int i=0;i<casos;i++)
	{
		VectorReal *VR;
		VR=(VectorReal*)MiAg.GeneracionActual.Generacion.dato(i);
		VR->mostrar();
	}

   MiAg.mostrarMedidas();
   return 0;
}
