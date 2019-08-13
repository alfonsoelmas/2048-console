/*-----------------------------------------------
|	TRABAJO 3-FUNDAMENTOS DE LA PROGRAMACIÓN		|
|			ALFONSO SORIA MUÑOZ					|
|			 2015 - Noviembre   				|
-------------------------------------------------*/

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <string>
	using namespace std;


//=======DECLARACIÓN DE TIPOS Y CONSTANTES=========
const int DIM=4;		//DIMENSION DEL TABLERO
const int LIBRE=1;
const int DFILA=6;		//DIMENSION DE LAS FILAS(*tambien se puede cambiar*)
const string RECORDS="records.txt";
const int	MaxRecords = 10;

typedef enum tAccion{Arriba,Abajo,Derecha,Izquierda,Salir,Nada};

typedef int tTablero[DIM][DIM];

struct		rJugador { string Nombre; int Puntos; };
struct		rRecords { rJugador Jugador[MaxRecords]; int Puntero; };

//=================================================



//======DECLARACIÓN DE FUNCIONES===================
void inicializa(tTablero tablero);										//Inicializa	el	tablero, haciendo que todas las baldosas estén libres, excepto dos elegidas aleatoriamente.
void visualiza(const tTablero tablero, int puntos, int totalPuntos);	//Muestra el tablero, las fichas y la puntuación en consola.
int log2(int num);														//Devuelve el logaritmo en base dos de un número potencia de 2.
void carga(tTablero tablero, int &puntos);								//Carga una partida guardada del tablero.

void colorFondo(int color);												//Cambia el color de fondo al escribir en consola.
void creoFichaNueva(tTablero tablero);									//crea una ficha de valor 2 o 4 en algun punto del tablero y devuelve la posicion en forma filaColumna como un entero.
int valorFichaNueva();													//devuelve un 2 o un 4 aleatoriamente con un 95% de posibilidades de que salga un 2.
int posicionFilaColumna();												//devuelve un numero entre 0 y N-1.
void lineaSuperior();													//Pinta la linea superior del tablero.
void lineaInferior();													//Pinta la linea inferior del tablero.
void lineaIntermedia();													//Pinta la linea intermedia del tablero.
void lineaNormal(int fila, int &columna, const tTablero tablero);		//Pinta una la linea superior de 1 fila de casillas del tablero.
void lineaPuntuacion(int fila, int &columna,const tTablero tablero);	//Pinta la linea de la pintuacion de 1 fila de casillas del tablero.
void filaCasilla(int fila, int &columna,const tTablero tablero);		//Pinta una fila de casillas del tablero.
bool compruebaTablero(tTablero tablero);								//Devuelve un true si tablero está lleno y un false si aún tiene alguna casilla libre.

tAccion leeAccion();													//Lee las teclas y devuelve un tipo tAcción en base a las teclas pulsadas.
void mueveFichas(tTablero tablero, tAccion accion);						//Mueve las fichas del tablero en la dirección inclinada por accion.
void mueveArriba(tTablero tablero);
void mueveAbajo(tTablero tablero);
void mueveIzquierda(tTablero tablero);
void mueveDerecha(tTablero tablero);

void combinaFichas(tTablero tablero, tAccion accion, int &puntos);		//Combina las fichas del tablero y Devuelve el total de puntos conseguido.
int mayor(const tTablero tablero);										//Devuelve el numero mayor del tablero

void nuevoRecord(int puntos);
bool cargaRecords();													
void Guarda(const tTablero Tablero, int Puntos);
//================================================

//=====REALIZACIÓN DE FUNCIONES===================

int log2(int num)
{
	int contador=0;
	while(num!=1)
	{
		num=(num/2);
		contador++;
	}
	return contador;
}
//================================================
void colorFondo(int color)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, 15 | color << 4);
}
//================================================
int valorFichaNueva()
{
	int ficha=2,prob;
	prob=rand() % 100;
	if(prob>=95) ficha=4;
	return ficha;
}
//================================================
int posicionFilaColumna()
{
	return rand() % (DIM);
}
//================================================
void creoFichaNueva(tTablero tablero)
{
	bool seHaCreado=false;
	int fila, columna;
	fila   =posicionFilaColumna();
	columna=posicionFilaColumna();
	while(!seHaCreado)
	{
		fila   =posicionFilaColumna();
		columna=posicionFilaColumna();
		if(tablero[fila][columna]==LIBRE)
		{
			tablero[fila][columna]=valorFichaNueva();
			seHaCreado=true;
		}
	}
}
//================================================
void inicializa(tTablero tablero)
{
	for(int i=0; i<DIM; i++)
		for(int j=0; j<DIM; j++) tablero[i][j]=LIBRE;

	creoFichaNueva(tablero);
	creoFichaNueva(tablero);
}
//================================================
void visualiza(const tTablero tablero, int puntos, int totalPuntos)
{
	system("cls");
	int fila=0,columna=0;
	cout<< "Puntos: "<< setw(10) << left << puntos << "Total puntos: "  << totalPuntos << endl;
	if(DIM!=0)
	{
		lineaSuperior();
		//-------- pinta 1 fila de casillas------
		for(int i=0; i<DIM; i++)
		{
			filaCasilla(fila,columna,tablero);
			if(i<DIM-1) lineaIntermedia();
			fila++;
		}
		lineaInferior();
		cout << endl << "Teclas:  Esc        :[  SALIR  ]:" << endl;
		cout		 << "          " << char(24) << endl;
		cout		 << "       <-   ->      :[DESPLAZAR]:"<< endl;
		cout		 << "          " << char(25) << endl;
	}
}
//================================================
void filaCasilla(int fila, int &columna,const tTablero tablero)
{
		columna=0;
		lineaNormal(fila,columna,tablero);
		columna=0;
		lineaPuntuacion(fila,columna,tablero);
		columna=0;
		lineaNormal(fila,columna,tablero);
}

//------------------FUNCIONES DE PINTAR LINEAS---------------------
void lineaPuntuacion(int fila, int &columna,const tTablero tablero)		//CAMBIALO CON SETW QUE ES MEJOR!!!!!!!!!!
{

	colorFondo(0);
	for(int i=0; i<DIM; i++)
	{
		colorFondo(0);
		cout << char(179);
		colorFondo(log2(tablero[fila][columna]));
		if(tablero[fila][columna]<10 && tablero[fila][columna]>1)
		{
			for(int j=0; j<DFILA-2; j++) cout << " ";
			cout << tablero[fila][columna] << " ";
		}
		else if(tablero[fila][columna]<100 && tablero[fila][columna]>10)
		{
			for(int j=0; j<DFILA-3; j++) cout << " ";
			cout << tablero[fila][columna] << " ";
		}
		else if(tablero[fila][columna]<1000 && tablero[fila][columna]>100)
		{
			for(int j=0; j<DFILA-4; j++) cout << " ";
			cout << tablero[fila][columna] << " ";
		}
		else if(tablero[fila][columna]<10000 && tablero[fila][columna]>1000)
		{
			for(int j=0; j<DFILA-5; j++) cout << " ";
			cout << tablero[fila][columna] << " ";
		}
		else if(tablero[fila][columna]==1)
		{
			for(int j=0; j<DFILA; j++) cout << " ";
		}
		columna++;
	}
	colorFondo(0);
	cout << char(179);
	cout << endl;
}
//================================================
void lineaNormal(int fila, int &columna,const tTablero tablero)
{
		int a=columna;
		for(int i=0; i<DIM; i++)
		{
			colorFondo(0);
			cout << char(179);
			colorFondo(log2(tablero[fila][a]));
			for(int j=0; j<DFILA; j++) cout << " ";
			a++;
		}
		colorFondo(0);
		cout << char(179);
		cout << endl;                                               
}
//================================================
void lineaSuperior()
{
		cout << char(218);
		for(int j=0; j<DIM; j++)
		{
			for(int i=0; i<DFILA; i++) cout << char(196);
			if(j!=DIM-1) cout << char(194);
		}
		cout << char(191);
		cout << endl;
}
//================================================
void lineaInferior()
{
	cout << char(192);
		for(int j=0; j<DIM; j++)
		{
			for(int i=0; i<DFILA; i++) cout << char(196);
			if(j!=DIM-1) cout << char(193);
		}
		cout << char(217);
		cout << endl;
}
//================================================
void lineaIntermedia()
{
	cout << char(195);
		for(int j=0; j<DIM; j++)
		{
			for(int i=0; i<DFILA; i++) cout << char(196);
			if(j!=DIM-1) cout << char(197);
		}
		cout << char(180);
		cout << endl;
}
//================================================
tAccion leeAccion()
{
	tAccion tecla;
	int dir;

	do
	{	
		cin.sync();
		dir =_getch();

		if (dir == 0xe0)
		{
			dir = _getch();
			switch(dir)
			{
			case 72:	tecla=Arriba;		break;
			case 80:	tecla=Abajo;		break;
			case 77:	tecla=Derecha;		break;
			case 75:	tecla=Izquierda;	break;
			default:	tecla=Nada;			break;
			}
		}
		else if( dir == 27) tecla=Salir;
		else tecla=Nada;
	}while(tecla==Nada);
	return tecla;
}
//------------------------------------------------------------------------

void carga(tTablero tablero, int &puntos)
{
	ifstream leo;
	string archivo;
	int dimension;
	cout << "Escribe el nombre y la extension del archivo: ";
	cin >> archivo;
	leo.open(archivo);

	if(leo.is_open())
	{
		leo >> dimension;
		if(dimension==DIM)
		{
			for(int i=0; i<DIM; i++)
				for(int j=0; j<DIM; j++)
					leo >> tablero[i][j];

			leo >> puntos;
		}
		else
		{
			inicializa(tablero);
			puntos=0;
		}
	}
	else
	{
		inicializa(tablero);
		puntos=0;
	}
	system("cls");
}
//================================================
bool compruebaTablero(tTablero tablero)
{
	bool comprobacion=true;
	int i=0;
	while(i<DIM && comprobacion)
	{
		int j=0;
		while(j<DIM && comprobacion)
		{
			if(tablero[i][j]==1) comprobacion=false;
			j++;
		}
		i++;
	}
	return comprobacion;
}
//================================================
void mueveFichas(tTablero tablero, tAccion accion)
{
	switch (accion)
	{
	case Arriba:	mueveArriba(tablero);		break;
	case Abajo:		mueveAbajo(tablero);		break;
	case Derecha:	mueveDerecha(tablero);		break;
	case Izquierda:	mueveIzquierda(tablero);	break;
	}
}
//================================================
void mueveArriba(tTablero tablero)
{
	for(int i=0; i<DIM; i++)		//Recorrido de columnas
		for(int j=0; j<DIM; j++)	
			if(j>0 && tablero[j][i]!=1 && tablero[j-1][i]==1)
			{
				int a=j-1;
				while(a>0 && tablero[a][i]==1) a=a-1;
				if(a!=0 || tablero[a][i]!=1)a=a+1;
				tablero[a][i]=tablero[j][i];
				tablero[j][i]=1;
			}
}
//================================================
void mueveAbajo(tTablero tablero)
{
	for(int i=0; i<DIM; i++)		//Recorrido de columnas
		for(int j=DIM-1; j>=0; j--)
			if(j<DIM-1 && tablero[j][i]!=1 && tablero[j+1][i]==1)
			{
				int a=j+1;
				while(a<DIM-1 && tablero[a][i]==1) a=a+1;
				if(a!=DIM-1 || tablero[a][i]!=1)a=a-1;
				tablero[a][i]=tablero[j][i];
				tablero[j][i]=1;
			}
}
//================================================
void mueveIzquierda(tTablero tablero)
{
	for(int i=0; i<DIM; i++)		//Recorrido de filas
		for(int j=0; j<DIM; j++)	
			if(j>0 && tablero[i][j]!=1 && tablero[i][j-1]==1)
			{
				int a=j-1;
				while(a>0 && tablero[i][a]==1) a=a-1;
				if(a!=0 || tablero[i][a]!=1)a=a+1;
				tablero[i][a]=tablero[i][j];
				tablero[i][j]=1;
			}
}
//================================================
void mueveDerecha(tTablero tablero)
{
	for(int i=0; i<DIM; i++)		//Recorrido de filas
		for(int j=DIM-1; j>=0; j--)
			if(j<DIM-1 && tablero[i][j]!=1 && tablero[i][j+1]==1)
			{
				int a=j+1;
				while(a<DIM-1 && tablero[i][a]==1) a=a+1;
				if(a!=DIM-1 || tablero[i][a]!=1)a=a-1;
				tablero[i][a]=tablero[i][j];
				tablero[i][j]=1;
			}
}
//================================================
void combinaFichas(tTablero tablero, tAccion accion, int &puntos)
{
	puntos=0;
	switch (accion)
	{
	case Arriba:
		for (int i=0; i<DIM; i++)
			for (int j=1; j<DIM; j++)
				if ( tablero[j][i]==tablero[j-1][i] && tablero[j][i]!=1)
				{
					tablero[j-1][i] += tablero[j][i];
					tablero[j][i]	 = 1;
					puntos			+= tablero[j-1][i];
				}
		break;

	case Abajo:
		for (int i=0; i<DIM; i++)
			for (int j=DIM-2; j>=0; j--)
				if ( tablero[j][i]==tablero[j+1][i] && tablero[j][i]!=1)
				{
					tablero[j+1][i] += tablero[j][i];
					tablero[j][i]	 = 1;
					puntos			+= tablero[j+1][i];
				}
		break;

	case Derecha:
		for (int i=0; i<DIM; i++)
			for (int j=DIM-2; j>=0; j--)
				if ( tablero[i][j]==tablero[i][j+1] && tablero[i][j]!=1)
				{
					tablero[i][j+1] += tablero[i][j];
					tablero[i][j] = 1;
					puntos += tablero[i][j+1];
				}
		break;

	case Izquierda:
		for (int i=0; i<DIM; i++)
			for (int j=1; j<DIM; j++)
				if ( tablero[i][j]==tablero[i][j-1] && tablero[i][j]!=1)
				{
					tablero[i][j-1] += tablero[i][j];
					tablero[i][j] = 1;
					puntos += tablero[i][j-1];
				}
		break;
	}
}
//================================================
int mayor(const tTablero tablero)
{
	int i=0;
	int mayor=0;
	while(i<DIM && mayor!=2048)
	{
		int j=0;
		while(j<DIM && mayor!=2048)
		{ // Busqueda
			if (tablero[i][j] > mayor)	mayor = tablero [i][j]; // Recorrido de filas columnas y sus valores
			j++;
		}
		i++;
	}
	return mayor;
}
//=======================================================================================
// Muestra el contenido del archivo records.txt.
//=======================================================================================
void MuestraRecords()
{
	ifstream	ArchivoL;					//Lectura
	bool		FinDatos = false;
	string		Jugador;
	int			Puntos;

	ArchivoL.open(RECORDS);			// Apertura
	if (ArchivoL.is_open())
	{
		cout << endl;
		cout << "=================================="	<< endl;
		cout << "        2048 TOP-TEN Players      "	<< endl;
		cout << "=================================="	<< endl;
		cout << "        Jugador - Puntuacion      "		<< endl;
		cout << "----------------------------------"	<< endl;
		while(!FinDatos)
		{
			ArchivoL >> Jugador;
			if (Jugador == "???") FinDatos = true;
			else
			{
				ArchivoL >> Puntos;
				cout << setfill (' ') << setw(15) << Jugador << " - ";
				cout << Puntos << endl;
			}
		}
		ArchivoL.close();								// Cierre del archivo de lectura
	}
	else cout << "Error al cargar el archivo\n";
	_getch();
}
//=======================================================================================
// Carga del archivo de mejores puntuaciones (records.txt) una lista de nombres y puntuaciones,
// pide al usuario su nombre, inserta en la lista el nombre y los puntos del nuevo record, y
// finalmente guarda en el archivo la lista actualizada (hasta diez records; si ya había diez,
// el de menor puntuación desaparece de la lista).
//=======================================================================================
void NuevoRecord(int Puntos)
{
	rRecords	ListaRecords;
	bool		FinDatos = false;
	string		Nombre;														//Para capturar
	int			PuntosRecordMenor = 0;										//Puntos del Record mas pequeño
	int			PunteroRecordMenor = 0;										//Puntero del Record mas pequeño
	ifstream	ArchivoL;													//Lectura
	ofstream	ArchivoE;													//Escritura

	ListaRecords.Puntero=0;													//Inicializamos el Contador de fichas
	ArchivoL.open(RECORDS);													// Apertura

	if (ArchivoL.is_open())													// Cargamos la lista de records...
	{
		while((ListaRecords.Puntero < MaxRecords) && !FinDatos)							// Leemos los 'MaxRecords' primeros
		{
			ArchivoL >> ListaRecords.Jugador[ListaRecords.Puntero].Nombre;				// Lee el nombre
			if (ListaRecords.Jugador[ListaRecords.Puntero].Nombre == "???") FinDatos = true;
			else
			{
				ArchivoL >> ListaRecords.Jugador[ListaRecords.Puntero].Puntos;			// Lee los puntos
				ListaRecords.Puntero++;
			}
		}
		if (ListaRecords.Puntero > 0)													// Si hay algun record...
		{
			PuntosRecordMenor  = ListaRecords.Jugador[ListaRecords.Puntero-1].Puntos;	// Asignamos el ultimo valor
			PunteroRecordMenor = ListaRecords.Puntero-1;									// Asignamos el ultimo valor
		}
		else																			// No hay records
		{
			PuntosRecordMenor  = 0;														// Asignamos puntos 0
			PunteroRecordMenor = 0;														// Asignamos posicion 0
		}
		ArchivoL.close();																// Cierre del archivo de lectura
	}

	//--------------------------------------------------------------
	int i;
	for ( i=0; i<ListaRecords.Puntero; i++)											// Buscamos la puntuacion menor...
	{
		if (ListaRecords.Jugador[i].Puntos <= PuntosRecordMenor)						// si la puntuacion menor de los records..
		{
			PuntosRecordMenor  = ListaRecords.Jugador[i].Puntos;						// Asignamos la puntuacion menor
			PunteroRecordMenor = i;														// Asignamos el puntero
		}
	}

	if 	(PuntosRecordMenor < Puntos || ListaRecords.Puntero<MaxRecords)							// Si la Puntuacion es mayor que el menor de los records...
	{
		cout << "...y estas entre los 10 mejores!!" << endl;
		cout << "Dime tu nombre: ";
		getline(cin, Nombre);
		//--------------------------------------------------------------------------------------------
		//Quito blancos del nombre
		//--------------------------------------------------------------------------------------------
		size_t posicion; while((posicion = Nombre.find(' ')) != string::npos)Nombre.erase(posicion, 1);
		//--------------------------------------------------------------------------------------------
		
		if (ListaRecords.Puntero > 9) i=PunteroRecordMenor;
		else { i=ListaRecords.Puntero; ListaRecords.Puntero++; }

		ListaRecords.Jugador[i].Nombre = Nombre;						//Sustituímos el record menor
		ListaRecords.Jugador[i].Puntos = Puntos;
	}
	else
	{
		cout << "...pero no estas entre los 10 mejores. :(" << endl;
	}

	cout << Nombre << endl;

	ArchivoE.open(RECORDS);																// Guardamos el Fichero

	for (int i = 0; i < ListaRecords.Puntero; i++)
	{
		ArchivoE << ListaRecords.Jugador[i].Nombre << " ";
		ArchivoE << ListaRecords.Jugador[i].Puntos << endl;
	}
	ArchivoE << "???" << endl;															// Centinela final
	ArchivoE.close();
	cout << "Archivo de records actualizado" << endl;
}
//=======================================================================================
// Pide un nombre de archivo y guarda en él la dimensión del tablero, su contenido (por filas) y los
// puntos conseguidos hasta ese momento
//=======================================================================================
void Guarda(const tTablero Tablero, int Puntos)
{
	string	NombreArchivo;
	cout << "Guardar Archivo Como...: ";
	cin >> NombreArchivo;

	ofstream Escribo;
	Escribo.open(NombreArchivo);

		Escribo << DIM << endl;
		for (int i=0; i<DIM; i++)
			for (int j=0; j<DIM; j++)
				Escribo << Tablero[i][j] << endl;
		Escribo << Puntos;
		Escribo.close();
		cout << "Tablero Guardado...";
}
//=====================================

//==========FUNCION MAIN===============
int main()
{
	srand(unsigned int (time(NULL)));
	colorFondo(0);

	int puntos=0, totalPuntos=0;
	char a,b;
	tTablero tablero;
	tAccion accion;
	bool pierde=false;
	bool otra=true;
	bool ini=true;

	while(otra)
	{
		pierde=false;
		otra=true;
		system("cls");
		if(ini)
		{
			cout << "Desea cargar una partida guardada?(S/N): ";
			a=_getche(); cout << endl;
			if(a=='S'||a=='s')	carga(tablero, totalPuntos);
			else				inicializa(tablero);
		}

		do
		{
			totalPuntos+=puntos;
			visualiza(tablero,puntos,totalPuntos);
			accion=leeAccion();
			mueveFichas(tablero,accion);
			combinaFichas(tablero,accion,puntos);
			mueveFichas(tablero,accion);
			if(mayor(tablero)>=2024)
			{
				totalPuntos+=puntos;
				visualiza(tablero,puntos,totalPuntos);
			}
			if(!compruebaTablero(tablero) && accion!=Salir)	creoFichaNueva(tablero);
			else if(compruebaTablero(tablero))	pierde=true;
		}while(accion!=Salir && !pierde && mayor(tablero)<2024);

		if(pierde)
		{
			system("cls");
			cout << "Has perdido, puntuacion maxima: " << totalPuntos;
			cout << endl << "Otra partida?(S/N)"; 
			cin.sync();
			b=_getch();
			if(b=='S'||b=='s') {otra=true; ini=true;}
			else otra=false;
		}
		else if(mayor(tablero)>=2024)
		{
			cout << "Has ganado!\n";
			NuevoRecord(totalPuntos);	//ARREGLAR
			MuestraRecords();			//ARREGLAR
			otra=false;
		}
		else if(accion==Salir)
		{
			cout << "Esta seguro de que quiere salir? S/N: ";	
			b=_getch(); cout << endl;
			if(b=='S'||b=='s')
			{
				cout << "Desea guardar la partida? S/N: ";	
				b=_getch(); cout << endl;
				otra=false;
				if(b=='S'||b=='s')	Guarda(tablero,totalPuntos);
			}
			else {otra=true; ini=false;}
		}
	}
	system("cls");
	cout << "Adios! Hasta la proxima!";
	_getch();
	return 0;
}
/*
*/
//=====================================
