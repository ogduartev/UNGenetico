#ifndef _GENETICO_HPP
#define _GENETICO_HPP
#endif

#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream.h>
#include<math.h>

#ifndef __ARREGLOS_HPP
#include"arreglos.hpp"
#endif

class Gen;
class OperadorMutacion;
class OperadorCruce;
class OperadorProbabilidad;
class OperadorParejas;
class OperadorReproduccion;
class OperadorSeleccion;
class Individuo;
class Poblacion;
class AlgoritmoGenetico;

class Gen
{
public:
	Gen(){AG=NULL;}
	~Gen(){}
	AlgoritmoGenetico *AG;

	void ag(AlgoritmoGenetico *Ag);

	virtual void crearAleatorio()=0;
	virtual OperadorMutacion *operadorMutacionDefecto()=0;
	virtual OperadorCruce *operadorCruceDefecto()=0;

	int operator==(const Gen &other)
	{
		return (this==&other);
	}
};

class Individuo
{
public:
	Individuo();
	~Individuo();
	AlgoritmoGenetico *AG;

	Arreglo<Gen> Genoma;
	double Objetivo;
	double Probabilidad;
	Individuo *Pareja;

	void calcularObjetivo()
	{
		decodificar();
		Objetivo=objetivo();
	}
	void copiar(Individuo *other)
	{
		Objetivo=other->Objetivo;
		Probabilidad=other->Probabilidad;
		copiarDetalles(other);
	}

	virtual Individuo* crearCopia()=0;
	virtual void copiarDetalles(Individuo *other)=0;
	virtual double objetivo()=0;
	virtual void codificar()=0;
	virtual void decodificar()=0;
	virtual void crearOperadores(Arreglo<OperadorMutacion> *LM,Arreglo<OperadorCruce> *LC);
	void crearAleatorio();
	void mutar();
	void ag(AlgoritmoGenetico *Ag);

	int operator==(const Individuo &other)
	{
		return (this==&other);
	}

};

class Poblacion
{
public:
	Poblacion();
	~Poblacion();
	AlgoritmoGenetico *AG;

	Individuo *Modelo;
	Individuo *Elite;
	Arreglo<Individuo> Generacion;
	double SumaObjetivo;

	void crearGeneracion(int tam);
	void crearGeneracionAleatoria();
	void calcularObjetivo();
	void ordenar(int descendente=0);
	void sumaObjetivo();
	void mutar();
	void buscaElite(int Maximizar);
	void elitismo();

	void ag(AlgoritmoGenetico *Ag);

};

class AlgoritmoGenetico
{
public:
	AlgoritmoGenetico();
	~AlgoritmoGenetico();

	Poblacion GeneracionActual;
	OperadorProbabilidad *OpProbabilidad;
	OperadorSeleccion *OpSeleccion;
	OperadorParejas *OpParejas;
	OperadorReproduccion *OpReproduccion;
	Arreglo<OperadorMutacion> *ListaOperadorMutacion;
	Arreglo<OperadorCruce> *ListaOperadorCruce;
	int Elitismo;
	int Maximizar;
	int TamanoPoblacion;

	Individuo *MejorEnEstaGeneracion;
	Individuo *PeorEnEstaGeneracion;
	Individuo *MejorEnLaHistoria;
	int Conteo;
	int Generacion;
	int GeneracionMaxima;
	int GeneracionDelMejorEnLaHistoria;
	double EvaluacionMedia;
	double MedidaOnLine;
	double MedidaOffLine;
	double AcumuladoOnLine;
	double AcumuladoOffLine;
	double Desviacion;

	int IndicadorArchivo;
	int IntervaloSalvar;
	FILE *arch;
	char NombreArchivo[400];
	int IndicadorMostrar;
	int IndicadorMostrarGeneracion;
	int IndicadorMostrarMejorEnHistoria;
	int IndicadorMostrarGeneracionMejorHistorico;
	int IndicadorMostrarMejorEnGeneracion;
	int IndicadorMostrarPeorEnGeneracion;
	int IndicadorMostrarMedia;
	int IndicadorMostrarDesviacion;
	int IndicadorMostrarOnLine;
	int IndicadorMostrarOffLine;
	int CriterioParada; //0=GeneracionMaxima

	void asignarProbabilidad(int descendente);
	void seleccionar();
	void asignarParejas();
	void reproducir();
	void mutar();
	void calcularObjetivo();
	void actualizarMedidas();
	void buscaElite();
	void elitismo();
	void optimizar();
	void iniciarOptimizacion();
	void iterarOptimizacion();
	void finalizarOptimizacion();
	int parada();

	virtual void adaptacion(){}

	void modelo(Individuo *Ind);

	void mostrarMedidas();
	void salvar();
};

class OperadorMutacion
{
public:
	OperadorMutacion()
	{
		AG=NULL;
      ProbabilidadMutacion=0.1;
	}
	~OperadorMutacion(){}
	AlgoritmoGenetico *AG;

	double ProbabilidadMutacion;

	void mutar(Gen *g);
	virtual void mutarGen(Gen *g)=0;

	int operator==(const OperadorMutacion &other)
	{
		return (this==&other);
	}
};

class OperadorCruce
{
public:
	OperadorCruce(){AG=NULL;}
	~OperadorCruce(){}
	AlgoritmoGenetico *AG;

	void cruzar(Gen *madre,Gen *padre, Arreglo<Gen> *hijos,int numHijos);
	virtual void cruzarGenes(Gen *madre,Gen *padre, Arreglo<Gen> *hijos,int numHijos)=0;

	int operator==(const OperadorCruce &other)
	{
		return (this==&other);
	}
};

class OperadorProbabilidad
{
public:
	OperadorProbabilidad(){AG=NULL;}
	~OperadorProbabilidad(){}
	AlgoritmoGenetico *AG;

	virtual void asignarProbabilidad(int descendente)=0;

	Poblacion *Pob;
};

class OperadorSeleccion
{
public:
	OperadorSeleccion(){AG=NULL;}
	~OperadorSeleccion(){}
	AlgoritmoGenetico *AG;

	virtual void seleccionar()=0;

	Poblacion *Pob;
};

class OperadorParejas
{
public:
	OperadorParejas(){AG=NULL;}
	~OperadorParejas(){}
	AlgoritmoGenetico *AG;

	virtual void asignarParejas()=0;

	Poblacion *Pob;
};

class OperadorReproduccion
{
public:
	OperadorReproduccion(){AG=NULL;}
	~OperadorReproduccion(){}
	AlgoritmoGenetico *AG;

	virtual void reproducir()=0;

	Poblacion *Pob;
};

class OperadorReproduccionDosPadresDosHijos:public OperadorReproduccion
{
public:
	OperadorReproduccionDosPadresDosHijos(){}
	~OperadorReproduccionDosPadresDosHijos(){}

	void reproducir();
};

class OperadorProbabilidadProporcional:public OperadorProbabilidad
{
public:
	OperadorProbabilidadProporcional(){}
	~OperadorProbabilidadProporcional(){}

	void asignarProbabilidad(int descendente);
};

class OperadorSeleccionEstocasticaRemplazo:public OperadorSeleccion
{
public:
	OperadorSeleccionEstocasticaRemplazo(){}
	~OperadorSeleccionEstocasticaRemplazo(){}

	void seleccionar();
};

class OperadorParejasAleatoria:public OperadorParejas
{
public:
	OperadorParejasAleatoria(){}
	~OperadorParejasAleatoria(){}

	void asignarParejas();
};

class OperadorReproduccionCrucePlano:public OperadorReproduccion
{
public:
	OperadorReproduccionCrucePlano(){}
	~OperadorReproduccionCrucePlano(){}

	void reproducir();
};

