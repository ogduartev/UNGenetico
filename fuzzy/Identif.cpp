#include <stdio.h>
#include <string.h>
#include <math.h>

#include<stdio.h>
#include<iostream>
#include<cstring>

#include "fuzzyB.h"
#include "genetico.hpp"
#include "genreal.hpp"

class Patron
{
public:
	Patron()
	{
		Test=0;
	}
	~Patron()
	{
		Entradas.FlushDestroy();
		Salidas.FlushDestroy();
	}

	Arreglo<float> Entradas;
	Arreglo<float> Salidas;
	int Test; // 0=Enteranamiento  1=Test

	BOOL operator==(const Patron &other)
	{
		return (Test==other.Test);
	}
};


class SLD_AG_Modificadores:public Individuo,public SistemaLogicaDifusa
{
public:
	SLD_AG_Modificadores(Arreglo<Patron> *Pat1=NULL,Arreglo<Patron> *Pat2=NULL,Arreglo<Patron> *Pat3=NULL,int numE=3,int numS=3);
	SLD_AG_Modificadores(SLD_AG_Modificadores *Mod);
	~SLD_AG_Modificadores()
	{
		 Genoma.FlushDestroy();

			PatronesEntrenamiento=NULL;
			PatronesPrueba=NULL;
			Patrones=NULL;
	}

//////// funciones virtuales de Individuo
	Individuo* crearCopia();
	void copiarDetalles(Individuo *other);
	double objetivo();
	void codificar();
	void decodificar();
/////////////////////

	Arreglo<Patron> *Patrones;
	Arreglo<Patron> *PatronesEntrenamiento;
	Arreglo<Patron> *PatronesPrueba;

	void crearSLD(int numEnt=3,int numSal=3);
	void entrenarUFijo(); 		// debe entrenar desde cero, con los patrones
	double objetivoTest();
	void crearGenoma();
	void grabaModificadores(char *cad);
};

SLD_AG_Modificadores::SLD_AG_Modificadores(Arreglo<Patron> *Pat1,Arreglo<Patron> *Pat2,Arreglo<Patron> *Pat3,int numE,int numS)
{
	crearSLD(numE,numS);
	Patrones=Pat1;
	PatronesEntrenamiento=Pat2;
	PatronesPrueba=Pat3;
	entrenarUFijo();
	crearGenoma();
	codificar();
}

SLD_AG_Modificadores::SLD_AG_Modificadores(SLD_AG_Modificadores *Mod)
{
	crearSLD(Mod->entradas->numeroConjuntosEnVariable(0),Mod->salidas->numeroConjuntosEnVariable(0));

// A partir de aqui se crean reglas, patrones y genoma

	Patrones=Mod->Patrones;
	PatronesEntrenamiento=Mod->PatronesEntrenamiento;
	PatronesPrueba=Mod->PatronesPrueba;

	motor->BaseReglas->FlushDestroy();
	motor->NumeroReglas=0;

	int i,tam;
	tam=Mod->motor->numeroReglas();
	for(i=0;i<tam;i++)
	{
		Regla *Reg;
		Reg=new Regla(Mod->numeroEntradas(),Mod->numeroSalidas());
		int j,tam2;
		tam2=numeroEntradas();
		for(j=0;j<tam2;j++)
		{
			Reg->conjuntoEntrada(j,Mod->motor->regla(i)->conjuntoEntrada(j));
			Reg->modificador(j,Mod->motor->regla(i)->modificador(j));
		}
		tam2=numeroSalidas();
		for(j=0;j<tam2;j++)
		{
			Reg->conjuntoSalida(j,Mod->motor->regla(i)->conjuntoSalida(j));
		}
		motor->adicionarRegla(Reg);
	}
	crearGenoma();
	codificar();
}

void SLD_AG_Modificadores::crearSLD(int numEnt,int numSal)
{
	 ConjuntoDifuso *cd;
	 Difusor *dif;
	 Variable *var;
	 Norma *And;
	 Norma *Composicion;
	 Norma *Conjuncion;
	 Implicacion *Implica;
	 Concresor *conc;

	 entradas=new Universo(1);
	 salidas=new Universo(1);

///////////// Entrada
	 var=new Variable(numEnt);
	 var->rangoMinimo(-1.000000);
	 var->rangoMaximo(1.000000);

	float dx;
	dx=(1.0-(-1.0))/(numEnt+1);

	int i;
	cd=new ConjuntoL(-1.0, -1.0+dx, -1.0 +2*dx);
	var->adicionarConjuntos(cd);
	for (i=1;i<(numEnt-1);i++){
		cd=new ConjuntoTriangulo(-1.0 +i*dx, -1.0+(i+1)*dx, -1.0 +(i+2)*dx);
		var->adicionarConjuntos(cd);
	}
	cd=new ConjuntoGamma(1.0-2*dx, 1.0-dx, 1.0);
	var->adicionarConjuntos(cd);

	 dif=new DifusorSinglenton(0.000000,0.010000);
	 dif->numeroPuntos(1);
	 var->difusorEntrada(dif);
	 var->nombreVariable("Entrada");
	 var->numeroIntervalos(50);
	 entradas->adicionarVariable(var);

///////////// Salida
	 var=new Variable(numSal);
	 var->rangoMinimo(-.5);
	 var->rangoMaximo(.5);

	dx=(.5-(-.5))/(numSal+1);

	cd=new ConjuntoL(-.5, -.5+dx, -.5 +2*dx);
	var->adicionarConjuntos(cd);
	for (i=1;i<(numSal-1);i++){
		cd=new ConjuntoTriangulo(-.5 +i*dx, -.5+(i+1)*dx, -.5 +(i+2)*dx);
		var->adicionarConjuntos(cd);
	}
	cd=new ConjuntoGamma(.5-2*dx, .5-dx, .5);
	var->adicionarConjuntos(cd);

	 var->nombreVariable("Salida");
	 var->numeroIntervalos(50);
	 salidas->adicionarVariable(var);

//////////////

	 motor=new MaquinaInferencia(entradas,salidas,1);
	 And=new Minimo();
	 Composicion=new Minimo();
	 Implica=new ImplicacionMinimo();
	 motor->and_(And);
	 motor->minComposicion(Composicion);
	 motor->implicacion(Implica);

	 motor->conjuntoEntrada(0,0,0);
	 motor->conjuntoSalida(0,0,0);

	 motor->modificador(0,0,1.000000);

	 concreto=new BloqueConcrecion(motor);
	 Conjuncion=new Maximo();
	 conc=new Altura(motor,0,Conjuncion);
	 concreto->adicionarConcresor(conc);
	 concreto->motor(motor);
	 concreto->conjuncion(Conjuncion);
}

void SLD_AG_Modificadores::crearGenoma()
{
	 int i,tam,j,tam2;
	 tam=motor->numeroReglas();
	 for(i=0;i<tam;i++)
	 {
		tam2=numeroEntradas();
		for(j=0;j<tam2;j++)
		{
			GenReal *g;
			g=new GenReal();
			g->Minimo=0.1;		// modificador entre 0.5 y 4.0
			g->Maximo=4.0;		// modificador entre 0.5 y 4.0
			Genoma.Add(g);
		}
	 }
}

void SLD_AG_Modificadores::grabaModificadores(char *cad)
{
	FILE *arch;
	arch=fopen(cad,"wt");
	if(arch==NULL){return;}
	 int i,tam,j,tam2,cont=0;
	 tam=motor->numeroReglas();
	 for(i=0;i<tam;i++)
	 {
		tam2=numeroEntradas();
		for(j=0;j<tam2;j++)
		{
			fprintf(arch,"%lf ",((GenReal*)Genoma.dato(cont))->Valor);
			cont++;
		}
		fprintf(arch,"\n");
	 }
	 fclose(arch);
}

Individuo* SLD_AG_Modificadores::crearCopia()
{
	Individuo *Ind;
	Ind=new SLD_AG_Modificadores(this);
	return Ind;
}

void SLD_AG_Modificadores::copiarDetalles(Individuo *other)
{

	SLD_AG_Modificadores *SLD;
	SLD=(SLD_AG_Modificadores*)other;

	Genoma.FlushDestroy();
	int i,j,tam,tam2;
	tam=SLD->motor->numeroReglas();
	for(i=0;i<tam;i++)
	{
		tam2=numeroEntradas();
		for(j=0;j<tam2;j++)
		{
			motor->BaseReglas->dato(i)->conjuntoEntrada(j,SLD->motor->BaseReglas->dato(i)->conjuntoEntrada(j));
			motor->BaseReglas->dato(i)->modificador(j,SLD->motor->BaseReglas->dato(i)->modificador(j));
			GenReal *g;
			g=new GenReal;
			g->Valor=motor->BaseReglas->dato(i)->modificador(j);
			Genoma.Add(g);
		}
		tam2=numeroSalidas();
		for(j=0;j<tam2;j++)
		{
			motor->BaseReglas->dato(i)->conjuntoSalida(j,SLD->motor->BaseReglas->dato(i)->conjuntoSalida(j));
		}
	}
}

double SLD_AG_Modificadores::objetivo()
{
	double *ant,*con;
	double error,sumaerror=0.0,sumatotal=0.0;
	ant=new double[numeroEntradas()];
	con=new double[numeroSalidas()];
	int i,tam;
	tam=PatronesEntrenamiento->GetItemsInContainer();
	for(i=0;i<tam;i++)
	{
		int j,tam2;
		tam2=numeroEntradas();
		for(j=0;j<tam2;j++)
		{
			ant[j]=*PatronesEntrenamiento->dato(i)->Entradas.dato(j);
		}
		calcular(ant,con);
		error=con[0]-*PatronesEntrenamiento->dato(i)->Salidas.dato(0);
		error=fabsl(error);
		sumaerror=sumaerror+error;
		sumatotal=sumatotal+fabsl(*PatronesEntrenamiento->dato(i)->Salidas.dato(0));
	}
	delete[] ant;
	delete[] con;
	return (sumaerror/sumatotal);
}

void SLD_AG_Modificadores::codificar()
{
	int i,j,tam,tam2,cont=0;
	tam=motor->numeroReglas();
	for(i=0;i<tam;i++)
	{
		tam2=numeroEntradas();
		for(j=0;j<tam2;j++)
		{
			GenReal *g;
			g=(GenReal*)Genoma.dato(cont);
			g->Valor=motor->regla(i)->modificador(j);
			cont++;
		}
	}
}

void SLD_AG_Modificadores::decodificar()
{
	int i,j,tam,tam2,cont=0;
	tam=motor->numeroReglas();
	for(i=0;i<tam;i++)
	{
		tam2=numeroEntradas();
		for(j=0;j<tam2;j++)
		{
			GenReal *g;
			g=(GenReal*)Genoma.dato(cont);
			motor->regla(i)->modificador(j,g->Valor);
			cont++;
		}
	}
}

void SLD_AG_Modificadores::entrenarUFijo()
{
	int i,tam;
	tam=PatronesEntrenamiento->GetItemsInContainer();
	double *ant,*con;
	ant=new double[numeroEntradas()];
	con=new double[numeroSalidas()];
	for(i=0;i<tam;i++)
	{
		int j,tam2;
		tam2=numeroEntradas();
		for(j=0;j<tam2;j++)
		{
			ant[j]=*PatronesEntrenamiento->dato(i)->Entradas.dato(j);
		}
		tam2=numeroSalidas();
		for(j=0;j<tam2;j++)
		{
			con[j]=*PatronesEntrenamiento->dato(i)->Salidas.dato(j);
		}
		EntrenaUniversoFijo(ant,con);
	}
	delete []ant;
	delete []con;
}

double SLD_AG_Modificadores::objetivoTest()
{
	double *ant,*con;
	double error,sumaerror=0.0,sumatotal=0.0;
	ant=new double[numeroEntradas()];
	con=new double[numeroSalidas()];
	int i,tam;
	tam=PatronesPrueba->GetItemsInContainer();
	for(i=0;i<tam;i++)
	{
		int j,tam2;
		tam2=numeroEntradas();
		for(j=0;j<tam2;j++)
		{
			ant[j]=*PatronesPrueba->dato(i)->Entradas.dato(j);
		}
		calcular(ant,con);
		error=con[0]-*PatronesPrueba->dato(i)->Salidas.dato(0);
		error=fabsl(error);
		sumaerror=sumaerror+error;
		sumatotal=sumatotal+fabsl(con[0]);
	}
	delete[] ant;
	delete[] con;
	return (sumaerror/sumatotal);
}

void crearPatrones(string Archivo,float PorcEntrena,Arreglo<Patron> *Patrones,Arreglo<Patron> *PatronesEntrenamiento,Arreglo<Patron> *PatronesPrueba)
{
	FILE *arch;
	arch=fopen(Archivo.c_str(),"r");
	if(arch==NULL){return;}
	float tmp;
	string strtmp;
	char c='a';
	Patron *Pat;
	Pat=new Patron;
	do
	{
		strtmp="";
		do
		{
			c=fgetc(arch);
			strtmp+=c;
		}while((c!=' ')&&(c!='\n')&&!feof(arch));
		tmp=atof(strtmp.c_str());
		float *pat;
		pat=new float;
		*pat=tmp;
		if(c==' ')
		{
			Pat->Entradas.Add(pat);
		}else
		{
			Pat->Salidas.Add(pat);
			if(Pat->Entradas.GetItemsInContainer()>0)
			{
				Patrones->Add(Pat);
			}
			Pat=new Patron;
		}
	}while(!feof(arch));

	int contador=0,tamEntrena;
	if(PorcEntrena<0.0){PorcEntrena=0.0;}
	if(PorcEntrena>1.0){PorcEntrena=1.0;}
	tamEntrena=PorcEntrena*Patrones->GetItemsInContainer();

//	randomize();

	do
	{
		double azar;
		azar=(double)((double)rand()/(double)RAND_MAX);
		int item;
		item=azar*Patrones->GetItemsInContainer();
		if(Patrones->dato(item)->Test==0)
		{
			Patrones->dato(item)->Test=1;
			contador++;
		}
	}while(contador<tamEntrena);

	int i,tam;
	tam=Patrones->GetItemsInContainer();
	for(i=0;i<tam;i++)
	{
		if(Patrones->dato(i)->Test==1)
		{
			PatronesEntrenamiento->Add(Patrones->dato(i));
		}else
		{
			PatronesPrueba->Add(Patrones->dato(i));
		}
	}
}

int main()
{
	int Indiv,Gener,NumEntra,NumSal;
	
	cout << "Número de Conjuntos de Entrada: ";
	cin >> NumEntra;
	cout << "Número de Conjuntos de Salida: ";
	cin >> NumSal;
	cout << "Número de Generaciones: ";
	cin >> Gener;
	cout << "Número de Individuos por Generación: ";
	cin >> Indiv;
	
//	NumEntra=3;NumSal=3;Gener=3;Indiv=11;

	SLD_AG_Modificadores *PredictorRio;
	Arreglo<Patron> *Patrones;
	Arreglo<Patron> *PatronesEntrenamiento;
	Arreglo<Patron> *PatronesPrueba;

	Patrones=new Arreglo<Patron>;
	PatronesEntrenamiento=new Arreglo<Patron>;
	PatronesPrueba=new Arreglo<Patron>;

	crearPatrones("datos.txt",.7,Patrones,PatronesEntrenamiento,PatronesPrueba);

	string str;
	char cad[200],cad2[200];
	str="Patrones creados: ";
	sprintf(cad,"%d - ",Patrones->GetItemsInContainer());
	str+=cad;
	sprintf(cad,"%d - ",PatronesEntrenamiento->GetItemsInContainer());
	str+=cad;
	sprintf(cad,"%d \n",PatronesPrueba->GetItemsInContainer());
	str+=cad;
	cout << str;

	float Error;
	PredictorRio=new SLD_AG_Modificadores(Patrones, PatronesEntrenamiento, PatronesPrueba,NumEntra,NumSal);

	sprintf(cad,"Reglas creadas:%d \n Genes: %d \nEntradas: %d\n",PredictorRio->motor->numeroReglas(),PredictorRio->Genoma.GetItemsInContainer(),PredictorRio->numeroEntradas());
	str=cad;
	cout << str;

	Error=PredictorRio->objetivo();
	sprintf(cad,"Error en entrenamiento:%lf \n",Error);
	str=cad;
	cout << str;

	Error=PredictorRio->objetivoTest();
	sprintf(cad,"Error en prueba:%lf \n",Error);
	str=cad;
	cout << str;

//  EL ALgoritmo Genético comienza aqui


	AlgoritmoGenetico MiAg;

	MiAg.TamanoPoblacion=Indiv;
	MiAg.GeneracionMaxima=Gener;
	MiAg.IntervaloSalvar=1;

	MiAg.modelo(PredictorRio);

	printf("iniciando algoritmo...\n");
	printf("Iteración: 0 Objetivo: %lf ObjetivoTest: %lf\n",PredictorRio->objetivo(),PredictorRio->objetivoTest());
	int c=0;
	MiAg.iniciarOptimizacion();
	do
	{
		sprintf(cad,"Iteración: %d ",c+1);
		cout << cad;
		MiAg.iterarOptimizacion();
		c++;
		sprintf(cad2,"Objetivo: %lf ObjetivoTest: %lf\n",MiAg.MejorEnLaHistoria->Objetivo,((SLD_AG_Modificadores*)MiAg.MejorEnLaHistoria)->objetivoTest());
  	cout << cad2;
	}while(!MiAg.parada());
	MiAg.finalizarOptimizacion();

	Error=MiAg.MejorEnLaHistoria->objetivo();
	sprintf(cad,"Error en entrenamiento:%lf \n",Error);
	str=cad;
	cout << str;

	Error=((SLD_AG_Modificadores*)MiAg.MejorEnLaHistoria)->objetivoTest();
	sprintf(cad,"Error en prueba:%lf \n Fin de programa",Error);
	str=cad;
	cout << str;

  sprintf(cad,"modif.txt");
	((SLD_AG_Modificadores*)MiAg.MejorEnLaHistoria)->grabaModificadores(cad);

	//////////// y=f(u)

	FILE *arch1,*arch2;

	arch1=fopen("antes.txt","wt");
	arch2=fopen("despues.txt","wt");
	int i,tam;
	tam=Patrones->GetItemsInContainer();
	double Entra[2],Sale1[2],Sale2[2];
	for(i=0;i<tam;i++)
	{
		Entra[0]=*Patrones->dato(i)->Entradas.dato(0);
		PredictorRio->calcular(Entra,Sale1);
		((SLD_AG_Modificadores*)MiAg.MejorEnLaHistoria)->calcular(Entra,Sale2);
		fprintf(arch1,"%lf %lf\n",Entra[0],Sale1[0]);
		fprintf(arch2,"%lf %lf\n",Entra[0],Sale2[0]);
	}
	fclose(arch1);
	fclose(arch2);

	///////////////y=f(x)

	arch1=fopen("fantes.txt","wt");
	arch2=fopen("fdespues.txt","wt");
	float t;
	for(t=-1.0;t<1.00001;t=t+0.01)
	{
		Entra[0]=t;
		PredictorRio->calcular(Entra,Sale1);
		((SLD_AG_Modificadores*)MiAg.MejorEnLaHistoria)->calcular(Entra,Sale2);
		fprintf(arch1,"%f %f\n",Entra[0],Sale1[0]);
		fprintf(arch2,"%f %f\n",Entra[0],Sale2[0]);
	}
	fclose(arch1);
   fclose(arch2);

	PatronesEntrenamiento->FlushDetach();
	PatronesPrueba->FlushDetach();
	Patrones->FlushDestroy();

	delete PatronesEntrenamiento;
	delete PatronesPrueba;
	delete Patrones;

	delete PredictorRio;
	return 0;
}


