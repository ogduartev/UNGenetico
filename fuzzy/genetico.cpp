#ifndef _GENETICO_CPP
#define _GENETICO_CPP
#endif

#ifndef _GENETICO_HPP
#include"genetico.hpp"
#endif

void Gen::ag(AlgoritmoGenetico *Ag)
{
	AG=Ag;
}

Individuo::Individuo()
{
	Probabilidad=0.0;
	Pareja=NULL;
	AG=NULL;
}

Individuo::~Individuo()
{
	Pareja=NULL;
}

void Individuo::mutar()
{
	int i,tam;
	tam=Genoma.GetItemsInContainer();
	for(i=0;i<tam;i++)
	{
		AG->ListaOperadorMutacion->dato(i)->mutar(Genoma.dato(i));
	}
}

void Individuo::crearOperadores(Arreglo<OperadorMutacion> *LM,Arreglo<OperadorCruce> *LC)
{
	OperadorMutacion *OM;
	OperadorCruce *OC;
	int i,tam;
	tam=Genoma.GetItemsInContainer();
	for(i=0;i<tam;i++)
	{
		OM=Genoma.dato(i)->operadorMutacionDefecto();
		OC=Genoma.dato(i)->operadorCruceDefecto();
		LM->Add(OM);
		LC->Add(OC);
	}
}

void Individuo::crearAleatorio()
{
	int i,tam;
	tam=Genoma.GetItemsInContainer();
	for(i=0;i<tam;i++)
	{
		Genoma.dato(i)->crearAleatorio();
	}
}

void Individuo::ag(AlgoritmoGenetico *Ag)
{
	AG=Ag;
	int i,tam;
	tam=Genoma.GetItemsInContainer();
	for(i=0;i<tam;i++)
	{
		Genoma.dato(i)->ag(AG);
	}
}

Poblacion::Poblacion()
{
	Modelo=NULL;
	Elite=NULL;
	AG=NULL;
}

Poblacion::~Poblacion()
{
	Modelo=NULL;
	Elite=NULL;
	AG=NULL;
	Generacion.FlushDestroy();
}

void Poblacion::crearGeneracion(int tam)
{
	int i;
	Generacion.FlushDestroy();
	for(i=0;i<tam;i++)
	{
		Individuo *Ind;
		Ind=Modelo->crearCopia();
		Ind->ag(AG);
		Generacion.Add(Ind);
	}
}

void Poblacion::crearGeneracionAleatoria()
{
	int i,tam;
	tam=Generacion.GetItemsInContainer();
	for(i=0;i<tam;i++)
	{
		Generacion.dato(i)->crearAleatorio();
	}
}

void Poblacion::mutar()
{
	int i,tam;
	tam=Generacion.GetItemsInContainer();
	for(i=0;i<tam;i++)
	{
		Generacion.dato(i)->mutar();
	}
}

void Poblacion::calcularObjetivo()
{
	int i,tam;
	tam=Generacion.GetItemsInContainer();
	for(i=0;i<tam;i++)
	{
		Generacion.dato(i)->calcularObjetivo();
	}
	Arreglo<Individuo> Temp;

}

void Poblacion::ordenar(int descendente)
{
	int i,j,tam,tam2;
	calcularObjetivo();

	Arreglo<Individuo> Temp;
	tam=Generacion.GetItemsInContainer();
	if(tam<=0){return;}
	for(j=0;j<tam;j++)
	{
		int ItemExtremo;
		double Extremo;
		tam2=Generacion.GetItemsInContainer();
		if(tam2<=0){return;}
		Extremo=Generacion.dato(0)->Objetivo;
		ItemExtremo=0;
		for(i=0;i<tam2;i++)
		{
			if(descendente)
			{
				if(Generacion.dato(i)->Objetivo>Extremo)
				{
					Extremo=Generacion.dato(i)->Objetivo;
					ItemExtremo=i;
				}
			}else
			{
				if(Generacion.dato(i)->Objetivo<Extremo)
				{
					Extremo=Generacion.dato(i)->Objetivo;
					ItemExtremo=i;
				}
			}
		}
		Temp.Add(Generacion.dato(ItemExtremo));
		Generacion.Detach(ItemExtremo);
	}
	tam=Temp.GetItemsInContainer();
	for(i=0;i<tam;i++)
	{
		Generacion.Add(Temp.dato(i));
	}
	Temp.FlushDetach();

}

void Poblacion::sumaObjetivo()
{
	SumaObjetivo=0;
	int i,tam;
	tam=Generacion.GetItemsInContainer();
	for(i=0;i<tam;i++)
	{
		SumaObjetivo+=Generacion.dato(i)->Objetivo;
	}
}

void Poblacion::buscaElite(int Maximizar)
{
	int i,tam;
	tam=Generacion.GetItemsInContainer();
	int ItemExtremo;
	double Extremo;
	if(tam<=0){return;}
	Extremo=Generacion.dato(0)->Objetivo;
	ItemExtremo=0;

	for(i=0;i<tam;i++)
	{
		if(Maximizar)
		{
			if(Generacion.dato(i)->Objetivo>Extremo)
			{
				Extremo=Generacion.dato(i)->Objetivo;
				ItemExtremo=i;
			}
		}else
		{
			if(Generacion.dato(i)->Objetivo<Extremo)
			{
				Extremo=Generacion.dato(i)->Objetivo;
				ItemExtremo=i;
			}
		}
	}
	Elite->copiar(Generacion.dato(ItemExtremo));
}

void Poblacion::elitismo()
{
	Generacion.dato(Generacion.GetItemsInContainer()-1)->copiar(Elite);
}

void Poblacion::ag(AlgoritmoGenetico *Ag)
{
	AG=Ag;
	Modelo->ag(Ag);
   Elite->ag(Ag);
	int i,tam;
	tam=Generacion.GetItemsInContainer();
	for(i=0;i<tam;i++)
	{
		Generacion.dato(i)->ag(AG);
	}
}

AlgoritmoGenetico::AlgoritmoGenetico()
{
	GeneracionActual.AG=this;

	MejorEnEstaGeneracion=NULL;
	PeorEnEstaGeneracion=NULL;
	MejorEnLaHistoria=NULL;
	ListaOperadorMutacion=NULL;
	ListaOperadorCruce=NULL;

	OpProbabilidad=new OperadorProbabilidadProporcional;
	OpSeleccion=new OperadorSeleccionEstocasticaRemplazo;
	OpParejas=new OperadorParejasAleatoria;
	OpReproduccion=new OperadorReproduccionDosPadresDosHijos;

	OpProbabilidad->Pob=&GeneracionActual;
	OpSeleccion->Pob=&GeneracionActual;
	OpParejas->Pob=&GeneracionActual;
	OpReproduccion->Pob=&GeneracionActual;

	OpProbabilidad->AG=this;
	OpSeleccion->AG=this;
	OpParejas->AG=this;
	OpReproduccion->AG=this;

	Elitismo=1;
	Maximizar=0;

	Generacion=0;
	GeneracionMaxima=100;
	TamanoPoblacion=10;
	GeneracionDelMejorEnLaHistoria=0;
	EvaluacionMedia=0.0;
	MedidaOnLine=0.0;
	MedidaOffLine=0.0;
	AcumuladoOnLine=0.0;
	AcumuladoOffLine=0.0;
	Desviacion=0.0;

	IndicadorArchivo=1;
	IntervaloSalvar=10;
	sprintf(NombreArchivo,"prueba.txt");
	IndicadorMostrar=0;
	IndicadorMostrarGeneracion=1;
	IndicadorMostrarMejorEnHistoria=1;
	IndicadorMostrarGeneracionMejorHistorico=1;
	IndicadorMostrarMejorEnGeneracion=1;
	IndicadorMostrarPeorEnGeneracion=1;
	IndicadorMostrarMedia=1;
	IndicadorMostrarDesviacion=1;
	IndicadorMostrarOnLine=1;
	IndicadorMostrarOffLine=1;
	CriterioParada=0;
}

AlgoritmoGenetico::~AlgoritmoGenetico()
{
	MejorEnEstaGeneracion=NULL;
	PeorEnEstaGeneracion=NULL;
	MejorEnLaHistoria=NULL;

	delete OpProbabilidad;
	delete OpSeleccion;
	delete OpParejas;
	delete OpReproduccion;
}

void AlgoritmoGenetico::modelo(Individuo *Ind)
{
	if(Ind==NULL){return;}
	if(ListaOperadorMutacion==NULL)
	{
		ListaOperadorMutacion=new Arreglo<OperadorMutacion>;
	}else
	{
		ListaOperadorMutacion->FlushDestroy();
	}
	if(ListaOperadorCruce==NULL)
	{
		ListaOperadorCruce=new Arreglo<OperadorCruce>;
	}else
	{
		ListaOperadorCruce->FlushDestroy();
	}
	Ind->crearOperadores(ListaOperadorMutacion,ListaOperadorCruce);

	int i,tam;
	tam=ListaOperadorMutacion->GetItemsInContainer();
	for(i=0;i<tam;i++)
	{
		ListaOperadorMutacion->dato(i)->AG=this;
	}
	tam=ListaOperadorCruce->GetItemsInContainer();
	for(i=0;i<tam;i++)
	{
		ListaOperadorCruce->dato(i)->AG=this;
	}

	if(GeneracionActual.Modelo!=NULL)
	{
		delete GeneracionActual.Modelo;
	}
	if(GeneracionActual.Elite!=NULL)
	{
		delete GeneracionActual.Elite;
	}
	if(MejorEnEstaGeneracion!=NULL)
	{
		delete MejorEnEstaGeneracion;
	}
	if(PeorEnEstaGeneracion!=NULL)
	{
		delete PeorEnEstaGeneracion;
	}
	if(MejorEnLaHistoria!=NULL)
	{
		delete MejorEnLaHistoria;
	}

	GeneracionActual.Modelo=Ind;
	GeneracionActual.Elite=GeneracionActual.Modelo->crearCopia();
	MejorEnEstaGeneracion=GeneracionActual.Modelo->crearCopia();
	PeorEnEstaGeneracion=GeneracionActual.Modelo->crearCopia();
	MejorEnLaHistoria=GeneracionActual.Modelo->crearCopia();

	GeneracionActual.Modelo->AG=this;
	GeneracionActual.Elite->AG=this;
	MejorEnEstaGeneracion->AG=this;
	PeorEnEstaGeneracion->AG=this;
	MejorEnLaHistoria->AG=this;
}

void AlgoritmoGenetico::asignarProbabilidad(int descendente)
{
	if(OpProbabilidad!=NULL)
	{
		OpProbabilidad->asignarProbabilidad(descendente);
	}
}

void AlgoritmoGenetico::seleccionar()
{
	if(OpSeleccion!=NULL)
	{
		OpSeleccion->seleccionar();
	}
}

void AlgoritmoGenetico::asignarParejas()
{
	if(OpParejas!=NULL)
	{
		OpParejas->asignarParejas();
	}
}

void AlgoritmoGenetico::reproducir()
{
	if(OpReproduccion!=NULL)
	{
		OpReproduccion->reproducir();
	}
}

void AlgoritmoGenetico::mutar()
{
	GeneracionActual.mutar();
}

void AlgoritmoGenetico::buscaElite()
{
	if(Elitismo)
	{
		GeneracionActual.buscaElite(Maximizar);
	}
}

void AlgoritmoGenetico::elitismo()
{
	if(Elitismo)
	{
		GeneracionActual.elitismo();
	}
}

void AlgoritmoGenetico::calcularObjetivo()
{
	GeneracionActual.calcularObjetivo();
}

void AlgoritmoGenetico::actualizarMedidas()
{
	int i;
	double AcumuladoMedia=0;
	double mejor,peor;
	int LugarMejor,LugarPeor;
	mejor=peor=GeneracionActual.Generacion.dato(0)->Objetivo;
	LugarMejor=LugarPeor=0;

	for(i=0;i<GeneracionActual.Generacion.GetItemsInContainer();i++)
	{
		double valor;
		valor=GeneracionActual.Generacion.dato(i)->Objetivo;

		AcumuladoMedia+=valor;
		if(((valor<mejor)&&(Maximizar==0))||((valor>mejor)&&(Maximizar==1)))
		{
			mejor=valor;
			LugarMejor=i;
		}
		if(((valor>peor)&&(Maximizar==0))||((valor<peor)&&(Maximizar==1)))
		{
			peor=valor;
			LugarPeor=i;
		}
	}
	EvaluacionMedia=AcumuladoMedia/GeneracionActual.Generacion.GetItemsInContainer();
	MejorEnEstaGeneracion->copiar(GeneracionActual.Generacion.dato(LugarMejor));
	PeorEnEstaGeneracion->copiar(GeneracionActual.Generacion.dato(LugarPeor));

	double valor1,valor2;
	valor1=MejorEnEstaGeneracion->Objetivo;
	valor2=MejorEnLaHistoria->Objetivo;

	if((Generacion==0)||((valor1<valor2)&&(Maximizar==0))||((valor1>valor2)&&(Maximizar==1)))
	{
		MejorEnLaHistoria->copiar(MejorEnEstaGeneracion);
		GeneracionDelMejorEnLaHistoria=Generacion;
	}

	valor2=MejorEnLaHistoria->Objetivo;
	AcumuladoOffLine+=valor2;
	MedidaOffLine=AcumuladoOffLine/(Generacion+1.0);
	AcumuladoOnLine+=EvaluacionMedia;
	MedidaOnLine=AcumuladoOnLine/(Generacion+1.0);
	double acumDesv=0;
	for(i=0;i<GeneracionActual.Generacion.GetItemsInContainer();i++)
	{
		double valor;
		valor=GeneracionActual.Generacion.dato(i)->Objetivo;
		acumDesv=acumDesv+(valor-EvaluacionMedia)*(valor-EvaluacionMedia);
	}
	Desviacion=sqrt(acumDesv/GeneracionActual.Generacion.GetItemsInContainer());
}

void AlgoritmoGenetico::mostrarMedidas()
{
	if(IndicadorMostrarGeneracion)
	{
		cout << "Generacion:" << Generacion << "\n";
	}
	if(IndicadorMostrarMejorEnHistoria)
	{
		cout << "MejorHistórico:" << MejorEnLaHistoria->Objetivo << "\n";
	}
	if(IndicadorMostrarGeneracionMejorHistorico)
	{
		cout << "GeneraciónMejorHistorico:" << GeneracionDelMejorEnLaHistoria << "\n";
	}
	if(IndicadorMostrarMejorEnGeneracion)
	{
		cout << "MejorActual:" << MejorEnEstaGeneracion->Objetivo << "\n";
	}
	if(IndicadorMostrarPeorEnGeneracion)
	{
		cout << "PeorActual:" << PeorEnEstaGeneracion->Objetivo << "\n";
	}
	if(IndicadorMostrarMedia)
	{
		cout << "Media:" << EvaluacionMedia << "\n";
	}
	if(IndicadorMostrarDesviacion)
	{
		cout << "Desviación:" << Desviacion << "\n";
	}
	if(IndicadorMostrarOnLine)
	{
		cout << "OnLine:" << MedidaOnLine << "\n";
	}
	if(IndicadorMostrarOffLine)
	{
		cout << "OffLine:" << MedidaOffLine << "\n";
	}
}

void AlgoritmoGenetico::salvar()
{
	if(IndicadorMostrarGeneracion)
	{
		fprintf(arch,"%d ",Generacion);
	}
	if(IndicadorMostrarMejorEnHistoria)
	{
		fprintf(arch,"%le ",MejorEnLaHistoria->Objetivo);
	}
	if(IndicadorMostrarGeneracionMejorHistorico)
	{
		fprintf(arch,"%d ",GeneracionDelMejorEnLaHistoria);
	}
	if(IndicadorMostrarMejorEnGeneracion)
	{
		fprintf(arch,"%le ",MejorEnEstaGeneracion->Objetivo);
	}
	if(IndicadorMostrarPeorEnGeneracion)
	{
		fprintf(arch,"%le ",PeorEnEstaGeneracion->Objetivo);
	}
	if(IndicadorMostrarMedia)
	{
		fprintf(arch,"%le ",EvaluacionMedia);
	}
	if(IndicadorMostrarDesviacion)
	{
		fprintf(arch,"%le ",Desviacion);
	}
	if(IndicadorMostrarOnLine)
	{
		fprintf(arch,"%le ",MedidaOnLine);
	}
	if(IndicadorMostrarOffLine)
	{
		fprintf(arch,"%le ",MedidaOffLine);
	}
	fprintf(arch,"\n");
}

void AlgoritmoGenetico::iniciarOptimizacion()
{
	if(IndicadorArchivo)
	{
		arch=fopen(NombreArchivo,"wt");
		fclose(arch);
	}

	Conteo=0;
	Generacion=0;

	GeneracionDelMejorEnLaHistoria=0;
	EvaluacionMedia=0;
	MedidaOnLine=0;
	MedidaOffLine=0;
	AcumuladoOnLine=0;
	AcumuladoOffLine=0;

//	randomize();
	GeneracionActual.crearGeneracion(TamanoPoblacion);
	GeneracionActual.crearGeneracionAleatoria();
	calcularObjetivo();
	actualizarMedidas();
	if(IndicadorArchivo==1)
	{
		arch=fopen(NombreArchivo,"at");
		salvar();
		fclose(arch);
		if(IndicadorMostrar)
		{
			mostrarMedidas();
		}
	}
	buscaElite();
	asignarProbabilidad(Maximizar);
}

void AlgoritmoGenetico::iterarOptimizacion()
{
	adaptacion();
	seleccionar();
	asignarParejas();
	reproducir();
	mutar();
	elitismo();
	calcularObjetivo();
	asignarProbabilidad(Maximizar);
	buscaElite();
	Generacion++;
	actualizarMedidas();
	Conteo++;
	if((IndicadorArchivo==1)&&(Conteo==IntervaloSalvar))
	{
		Conteo=0;
		arch=fopen(NombreArchivo,"at");
		salvar();
		fclose(arch);
		if(IndicadorMostrar)
		{
			mostrarMedidas();
		}
	}
}

void AlgoritmoGenetico::finalizarOptimizacion()
{
}

void AlgoritmoGenetico::optimizar()
{
	iniciarOptimizacion();

	do
	{
		iterarOptimizacion();
	}while(!parada());

	finalizarOptimizacion();
}

int AlgoritmoGenetico::parada()
{
	int res;
	switch(CriterioParada)
	{
		case 0: res=(Generacion>=GeneracionMaxima);break;
		default: res=(Generacion>=GeneracionMaxima);break;
	}
	return res;
}

void OperadorMutacion::mutar(Gen *g)
{
	double azar;
	azar=(double)((double)rand()/(double)RAND_MAX);
	if(azar<ProbabilidadMutacion)
	{
		mutarGen(g);
	}
}

void OperadorCruce::cruzar(Gen *madre,Gen *padre, Arreglo<Gen> *hijos,int numHijos)
{
	cruzarGenes(madre,padre,hijos,numHijos);
	int i;
	for(i=0;i<numHijos;i++)
	{
		hijos->dato(i)->ag(AG);
	}
}


///////////////////////////////////////////////////////////////////
/////////////////////  OPERADORES       ////////////////////
///////////////////////////////////////////////////////////////////

void OperadorProbabilidadProporcional::asignarProbabilidad(int descendente)
{
	Pob->ordenar(descendente);
	Pob->sumaObjetivo();
	int i,tam;
	double SumaObjetivo=0.0;
	tam=Pob->Generacion.GetItemsInContainer();
	for(i=0;i<tam;i++)
	{
		if(!descendente)
		{
			if(Pob->Generacion.dato(i)->Objetivo!=0)
			{
				SumaObjetivo+=1.0/Pob->Generacion.dato(i)->Objetivo;
			}
		}else
		{
			SumaObjetivo+=Pob->Generacion.dato(i)->Objetivo;
		}
	}
	if(SumaObjetivo==0.0)
	{
		tam=Pob->Generacion.GetItemsInContainer();
		for(i=0;i<tam;i++)
		{
			Pob->Generacion.dato(i)->Probabilidad=1.0/(double)tam;
		}
	}else
	{
		int flag=0;
		tam=Pob->Generacion.GetItemsInContainer();
		for(i=0;i<tam;i++)
		{
			if(!descendente)
			{
				if(Pob->Generacion.dato(i)->Objetivo!=0)
				{
					Pob->Generacion.dato(i)->Probabilidad=(1.0/Pob->Generacion.dato(i)->Objetivo)/SumaObjetivo;
				}else
				{
					Pob->Generacion.dato(i)->Probabilidad=1.0;
					flag++;
				}
			}else
			{
				Pob->Generacion.dato(i)->Probabilidad=Pob->Generacion.dato(i)->Objetivo/SumaObjetivo;
			}
		}
		if(flag!=0)
		{
			for(i=0;i<tam;i++)
			{
				if(Pob->Generacion.dato(i)->Probabilidad==1.0)
				{
					Pob->Generacion.dato(i)->Probabilidad=1.0/(double)flag;
				}else
				{
					Pob->Generacion.dato(i)->Probabilidad=0.0;
				}
			}
		}
	}
}

void OperadorSeleccionEstocasticaRemplazo::seleccionar()
{
	int i,j,tam;
	double *angulo;
	double azar;
	double suma=0;
	double angulomenor,angulomayor;

	tam=Pob->Generacion.GetItemsInContainer();
	angulo=new double[tam];

	for(i=0;i<tam;i++)
	{
		suma=suma+Pob->Generacion.dato(i)->Probabilidad;
		angulo[i]=suma;
	}
	Arreglo<Individuo> *Npob;
	Npob=new Arreglo<Individuo>;
	for(i=0;i<tam;i++)
	{
		azar=(double)((double)rand()/(double)RAND_MAX);
		for(j=0;j<tam;j++)
		{
			if(j==0)
			{
				angulomenor=0.0;
			}else
			{
				angulomenor=angulo[j-1];
			}

			if(j==tam-1)
			{
				angulomayor=1.0;
			}else
			{
				angulomayor=angulo[j];
			}
			if((azar>angulomenor)&&(azar<=angulomayor))
			{
				Individuo *ind;
				ind=Pob->Modelo->crearCopia();
				ind->copiar(Pob->Generacion.dato(j));
				Npob->Add(ind);
				j=tam;
			}
		}
	}
	for(i=0;i<tam;i++)
	{
		if(i<Npob->GetItemsInContainer())
		{
			Pob->Generacion.dato(i)->copiar(Npob->dato(i));
		}else
		{
			Pob->Generacion.dato(i)->copiar(Npob->dato(Npob->GetItemsInContainer()-1));
		}
	}

	Npob->FlushDestroy();
	delete Npob;
	delete[] angulo;
}

void OperadorParejasAleatoria::asignarParejas()
{
	int i,tam,tamciclo;
	int pareja;
	double azar;
	tam=Pob->Generacion.GetItemsInContainer();
	for(i=0;i<tam;i++)
	{
		Pob->Generacion.dato(i)->Pareja=NULL; /* NULL significa sin pareja, otro valor es el puntero de la pareja */
	}
	if(tam%2==0)
	{
		tamciclo=tam;
	}else
	{
		tamciclo=tam-1;
	}
	for(i=0;i<tamciclo;i++)
	{
		if(Pob->Generacion.dato(i)->Pareja==NULL)
		{
			int loop=0;
			do
			{
				azar=(double)((double)rand()/(double)RAND_MAX);
				pareja=(int)((tamciclo)*azar);
				loop++;
				if(Pob->Generacion.dato(pareja)->Pareja==NULL&&pareja!=i)
				{
					Pob->Generacion.dato(i)->Pareja=Pob->Generacion.dato(pareja);
					Pob->Generacion.dato(pareja)->Pareja=Pob->Generacion.dato(i);
				}
				if(loop>3000)
				{
					Pob->Generacion.dato(i)->Pareja=Pob->Generacion.dato(tam-1);
				}
			}while(Pob->Generacion.dato(i)->Pareja==NULL);
		}
	}
}

void OperadorReproduccionCrucePlano::reproducir()
{
	int i,j,tam,tamGenoma;
	tamGenoma=Pob->Modelo->Genoma.GetItemsInContainer();
	tam=Pob->Generacion.GetItemsInContainer();
	for(i=0;i<tam;i++)
	{
		if(Pob->Generacion.dato(i)->Pareja!=NULL)
		{
			Individuo *Padre,*Madre;
			int puntoCruce;
			double azar;
			Padre=Pob->Generacion.dato(i);
			Madre=Pob->Generacion.dato(i)->Pareja;
			azar=(double)((double)rand()/(double)RAND_MAX);
			puntoCruce=(int)((tamGenoma)*azar);
			for(j=puntoCruce;j<tamGenoma;j++)
			{
				Gen *temp;
				temp=Padre->Genoma.dato(j);
				Padre->Genoma.Detach(j);
				Padre->Genoma.AddAt(j,Madre->Genoma.dato(j));
				Madre->Genoma.Detach(j);
				Madre->Genoma.AddAt(j,temp);
			}
			Padre->Pareja=NULL;
			Madre->Pareja=NULL;
			Padre->calcularObjetivo();
			Madre->calcularObjetivo();
		}
	}
}

void OperadorReproduccionDosPadresDosHijos::reproducir()
{
	int i,j,tam,tamGenoma;
	tamGenoma=Pob->Modelo->Genoma.GetItemsInContainer();
	tam=Pob->Generacion.GetItemsInContainer();
	for(i=0;i<tam;i++)
	{
		if(Pob->Generacion.dato(i)->Pareja!=NULL)
		{
			Individuo *Padre,*Madre;

			Padre=Pob->Generacion.dato(i);
			Madre=Pob->Generacion.dato(i)->Pareja;
			for(j=0;j<tamGenoma;j++)
			{
				Arreglo<Gen> Hijos;
				Gen *GenPadre,*GenMadre;
				GenPadre=Padre->Genoma.dato(j);
				GenMadre=Madre->Genoma.dato(j);
				OperadorCruce *OC;
				OC=AG->ListaOperadorCruce->dato(j);
				OC->cruzar(GenMadre,GenPadre,&Hijos,2);
				Padre->Genoma.Destroy(j);
				Madre->Genoma.Destroy(j);
				Padre->Genoma.AddAt(j,Hijos.dato(0));
				Madre->Genoma.AddAt(j,Hijos.dato(1));
				Hijos.FlushDetach();
			}

			Padre->decodificar();
			Madre->decodificar();
			Padre->calcularObjetivo();
			Madre->calcularObjetivo();

		}
	}
}

