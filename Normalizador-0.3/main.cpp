
#include <fst/fstlib.h>
#include <fst/fst-decl.h>
#include <fst/union.h>

#include "freeling.h"

#include <boost/regex.hpp>

#include <sstream>

using namespace std;

// headers del arbol binario de búsqueda
#include "Avl.h"
#include "Avl2.h"

// Dato que guardo en el arbol que uso de diccionario
#include "diccitem.h"

using namespace fst;

#include "funciones.h" 

// ================
// Declaración de funciones utilizadas
// ================
StdVectorFst string2fst(int p, int pf, wstring sInput );
StdVectorFst string2fstOrdinal(int p, int pf, wstring sInput );
void showFST(char *dirName, char *fileName);
void showTestFST(int i, StdVectorFst testFst, StdVectorFst resultFst, wstring testName, ofstream &archivoSalida);
void showEvalFST(int i, StdVectorFst resultFst, wstring testName, wofstream &archivoSalida);
StdVectorFst optFST(StdVectorFst unionFst);
void printString(StdVectorFst &fst, wstring &resultado);

std::wstring StringToWString(const std::string& s){
 std::wstring temp(s.length(),L' ');
 std::copy(s.begin(), s.end(), temp.begin());
 return temp; 
}




int main(int argc, char *argv[]) {
//Inicializar
	   StdVectorFst testFST, resultFST, resultFSTOutN, resultFSTOut1, testFST1, testFST2, testFST3;
	   StdFst *fst = NULL; 
	
	   int states, start; 
	   	     
	   float p = 0, pf = 0;
	   wstring sTest;
	   int nshortest, fstTagGeneration = 1, cantCaracteres, n = 10;
	   
	   string UbicacionTextoModif, UbicacionTexto;
	   wstringstream archivo;
	   bool stdInput = true, stdOutput = true; 
	   
	   //Listado de palabras 
	   list<freeling::word> palabras;
	   
	   // iterador
	   list<freeling::word>::iterator w; 
	   
	   std::wstring s; 

		// Argumentos
		// -i filename : archivo de entrada 
		// -o fiename : archivo de salida 

		if (argc > 2){
			if(strcasecmp(argv[1], "-i") == 0){
				stdInput = false; 
				UbicacionTexto = argv[2]; 
			}
		
			if(strcasecmp(argv[1], "-o") == 0){
				stdOutput = false; 
				UbicacionTextoModif = argv[2]; 
			}	
		}
	
		if (argc > 4){
			if(strcasecmp(argv[3], "-i") == 0){
				stdInput = false; 
				UbicacionTexto = argv[4]; 
			}
		
			if(strcasecmp(argv[3], "-o") == 0){
				stdOutput = false; 
				UbicacionTextoModif = argv[4]; 
			}	
		}

	   
	   /// Freeling: set locale to an UTF8 compatible locale
	   freeling::util::init_locale(L"es_AR.UTF-8");
	   
	   // Crea un objeto tokenizer de Freeling 	
   	 freeling::tokenizer tk(L"tokenizer.dat");


	   // Arbol con un diccionario de siglas 
	   AvlTree<diccItem> siglas;
	   
	   char ArchivoSiglas[2000]; 
	   strcpy(ArchivoSiglas, "siglas.txt");
	   
	   // Importa el archivo de siglas
	   ImportPalabrasATraducir(&siglas, ArchivoSiglas);
	   
	   // Arbol con un diccionario de abreviaturas
	   AvlTree<diccItem> abrev;
	   
	   char ArchivoAbrev[2000]; 
	   strcpy(ArchivoAbrev, "Abreviaturas.txt");
	   
	   // Importa el archivo de abreviaturas
	   ImportPalabrasATraducir(&abrev, ArchivoAbrev);
	   	   
		
		if (stdInput){
			//Tomo la entrada de standard imput
			archivo << wcin.rdbuf() << L'\0';

		}else{
			//tomo la entrada de un archivo 
			wifstream archivoEntrada (UbicacionTexto.c_str(), ios_base::in);
			if (!archivoEntrada.good()){
				printf("No se puede abrir el archivo: %s\n", UbicacionTexto.c_str());
				return -1; 
			}

			archivo << archivoEntrada.rdbuf() << L'\0';
		}
		

		wofstream archivoSalida1 (UbicacionTextoModif.c_str());
	        
 		string FSTfolder; 

  	FSTfolder = "./WFST/";
		
	//Tabla de simbolos
		string nomarchivo; 
		nomarchivo = FSTfolder + "simbolos.csv"; 
		fst::SymbolTable *isyms = fst::SymbolTable::ReadText(nomarchivo.c_str());
		fst::SymbolTable *osyms = fst::SymbolTable::ReadText(nomarchivo.c_str());

	// Importa los FSTs	

		// Números Naturales 
		nomarchivo = FSTfolder + "out/NaturalesMasc/NaturalesMasc.fst"; 
		StdFst *fstNaturales = StdFst::Read(nomarchivo.c_str());		
	 		
		// Ordinales Femeninos 
		nomarchivo = FSTfolder + "out/OrdinalesFem/Ordinales.fst"; 
		StdFst *fstOrdinalesFem = StdFst::Read(nomarchivo.c_str());		
		
		//Ordinales Masculinos 
		nomarchivo = FSTfolder + "out/OrdinalesMasc/Ordinales.fst"; 
		StdFst *fstOrdinalesMasc = StdFst::Read(nomarchivo.c_str());		
		
		//Ordinales Neutros 
		nomarchivo = FSTfolder + "out/OrdinalesNeutro/Ordinales.fst"; 
		StdFst *fstOrdinalesNeutro = StdFst::Read(nomarchivo.c_str());		
		
		// Números reales
		nomarchivo = FSTfolder + "out/Reales/Reales.fst"; 
		StdFst *fstReales = StdFst::Read(nomarchivo.c_str());		


		// Romanos Naturales 
		nomarchivo = FSTfolder + "out/RomanosNat/Romanos.fst"; 
		StdFst *fstRomanosNat = StdFst::Read(nomarchivo.c_str());		
		
		// Romanos ordinales femeninino
		nomarchivo = FSTfolder + "out/RomanosOrdF/Romanos.fst"; 
		StdFst *fstRomanosOrdF = StdFst::Read(nomarchivo.c_str());		

		// Romanos ordinales masculino
		nomarchivo = FSTfolder + "out/RomanosOrdM/Romanos.fst"; 
		StdFst *fstRomanosOrdM = StdFst::Read(nomarchivo.c_str());		

		//Romanos ordinales neutro
		nomarchivo = FSTfolder + "out/RomanosOrdN/Romanos.fst"; 
		StdFst *fstRomanosOrdN = StdFst::Read(nomarchivo.c_str());		
	
		//Fechas
		nomarchivo = FSTfolder + "out/Fechas/Fechas.fst"; 
		StdFst *fstFechas = StdFst::Read(nomarchivo.c_str());		

		//Horas
		nomarchivo = FSTfolder + "out/Horas/Horas.fst"; 
		StdFst *fstHoras = StdFst::Read(nomarchivo.c_str());		

		//Teléfonos
		nomarchivo = FSTfolder + "out/Telefonos/Telefonos.fst"; 
		StdFst *fstTelefonos = StdFst::Read(nomarchivo.c_str());		

		//Importes 
		nomarchivo = FSTfolder + "out/Importes/Importes.fst"; 
		StdFst *fstImportes = StdFst::Read(nomarchivo.c_str());		

		//Fracciones 
		nomarchivo = FSTfolder + "out/Fracciones/Fracciones.fst"; 
		StdFst *fstFracciones = StdFst::Read(nomarchivo.c_str());		

		//Grados 
		nomarchivo = FSTfolder + "out/Grados/Grados.fst"; 
		StdFst *fstGrados = StdFst::Read(nomarchivo.c_str());		
		
		//Letras 
		nomarchivo = FSTfolder + "out/Letras/Letras.fst"; 
		StdFst *fstLetras = StdFst::Read(nomarchivo.c_str());		
			
							
		// Arreglo con las plabras de contexto 
		wstring pal[10];
		wstring resultado, regla; 
		wstring oracionActual;
		int j, f, CntPalAvanzar; 	

		while (getline (archivo, sTest))
         {
           cantCaracteres = sTest.length();
		   		   
		   if (cantCaracteres > 1)
		   {
			  // tokenize input line into a list of words
			  tk.tokenize(sTest, palabras);

			  oracionActual = L""; 
			  
			  for (w = palabras.begin(); w != palabras.end(); w++) {
		
				// s es la palabra actual   
				s = w->get_form();			
								
				// Obtengo las 5 palabras anteriores y posteriores 
				ObtenerContexto(pal, &palabras, w);
				
				// Inicializo la variable 
				CntPalAvanzar = 0;
				
				// Normaliza la palabra 
				if (EsUnidadMedida(s, pal, resultado, regla, CntPalAvanzar, fstNaturales, fstOrdinalesFem, fstOrdinalesMasc, fstOrdinalesNeutro, fstReales, fstLetras,  &siglas, &abrev, isyms)){
					s = resultado; 
				}else if (EsMoneda(s, pal, resultado, regla, CntPalAvanzar, fstNaturales, fstOrdinalesFem, fstOrdinalesMasc, fstOrdinalesNeutro, fstReales, fstLetras,  &siglas, &abrev,isyms)){
					s = resultado; 
				}else if (EsFecha(s, pal, resultado,  fstFechas, isyms)){ 
					s = resultado; 
				}else if (EsHora(s, pal, resultado,  fstNaturales, isyms)){ // No pude hacer funcionar el fst de horas 
					s = resultado; 
				}else if (ContieneGrados(s, pal, resultado, fstNaturales, fstReales, fstOrdinalesFem, fstOrdinalesMasc, fstOrdinalesNeutro, isyms)){
					s = resultado; 
				}else if (EsNro(s, pal, resultado,regla,fstNaturales, fstOrdinalesFem, fstOrdinalesMasc, fstOrdinalesNeutro, fstReales, fstLetras, &siglas, &abrev, isyms)){
					s = resultado; 
				}else if (EsNroRomano(s, pal, resultado, regla,fstNaturales, fstOrdinalesFem, fstOrdinalesMasc, fstOrdinalesNeutro, isyms)){
					s = resultado; 
				}else if(EsSiglaOAbreviatura(s, &siglas, &abrev, resultado, fstLetras, isyms)){
					s =  resultado; 
				}
				
		
				if (s.compare(L"<S>") == 0 ){

					oracionActual = oracionActual + s;
					archivoSalida1 << s << L'\n'; 

					archivoSalida1.flush();

					// mando a stdout cuando se completó una oración
					if (stdOutput)
						wcout << oracionActual << L'\n';				

					oracionActual = L"";

				}else{ 
					oracionActual = oracionActual + s + L' '; 
					archivoSalida1 << s << L' ';
				}
				

				
				// Avanzo el iterador para saltear las palabras ya procesadas 
				for (f = 0; f < CntPalAvanzar && w != palabras.end(); f++)
					w++; 
				
			 }
		   }
		 }	   
		
		archivoSalida1.close();
		
   return 0;
}


// Importa el archivo de Siglas/Abreviaturas 
void ImportPalabrasATraducir(AvlTree<diccItem> *dicc, char *filename){

	wstringstream archivo;
	
	wstring sTest, pal, pron; 
	
	diccItem nueva;
	
	Comparable<diccItem> * CompDiccItem;
	
	int cantCaracteres; 
	
	wifstream archivoEntrada (filename, ios_base::in);
	
	archivo << archivoEntrada.rdbuf() << L'\0';
	
	while (getline(archivo, sTest)){
		
		cantCaracteres = sTest.length();
				   
		if (cantCaracteres != 0){
			
			wstringstream sin (wstringstream::in);
			sin.str(sTest);
			
			// Obtengo el palabra 
			getline(sin, pal, L'\t');
						
			// Y la pronunciación; 
			getline(sin, pron, L'\t');
											
			nueva.SetWord(pal, pron); 
							
			CompDiccItem = new Comparable<diccItem>(nueva); 
			
			// La inserto en el diccionario				
			dicc->Insert(CompDiccItem);
	
		}
	}
	
   archivoEntrada.close();
	
}

// Determina si es una sigla o abreviatura y la expande en caso de ser necesario  
bool EsSiglaOAbreviatura(wstring &s, AvlTree<diccItem> *dicc, AvlTree<diccItem> *abrev,  wstring &resultado, StdFst *fstLetras, fst::SymbolTable *isyms){

	StdVectorFst testFST, resultFST, resultFSTOut1; 
	
	diccItem buscar;
		
	wstring aux; 
		
	Comparable<diccItem> * found = NULL;
		
	diccItem *p;
	
	// Es una abreviatura? 
	aux = s; 

	// Convierte a minúsculas
	std::transform(aux.begin(), aux.end(), aux.begin(), ::tolower);
	
	// Se busca la palabra en el listado de abreviaturas
	buscar.SetWord(aux, L""); 
					
	// Busco la palabra en el diccionario  			
	found = abrev->Search(buscar);
		
	if (found) { // Esta en el diccionario
		p = found->GetKey(); 
			
		// obtengo la pronunciación
		resultado = p->pronunciacion;
		return true; 
	}
	
	// No esta en el listado de abreviaturas 
	// Se busca en el listados de Siglas 
	
	aux = s; 

	// Convierte a mayúsculas
	std::transform(aux.begin(), aux.end(), aux.begin(), ::toupper);

	//Le quito los puntos
	QuitarPuntos(aux); 
		
	buscar.SetWord(aux, L""); 
					
	// Busco la palabra en el diccionario  			
	found = dicc->Search(buscar);
		
	if (found) { // Esta en el diccionario
		p = found->GetKey(); 
			
		// obtengo la pronunciación
		resultado = p->pronunciacion;
		return true; 
	
	}else{
		// determinar si es una sigla y ver si se debe pronunciar o deletrear
		aux = s; 
		QuitarPuntos(aux); 
		
		// Si es menor a 2 letras lo descarto
		if (aux.length() < 2) 
		return false; 
		
		// Expresión regular para siglas, sólo letras mayúsculas
		boost::wregex siglasER(L"^[A-Z]+$", boost::regex::perl);

		if (boost::regex_match (aux, siglasER, boost::regex_constants::format_perl)){	
			
			if(DeletrearSigla(aux)){
							
				testFST = string2fst(0, 0, aux); 
			
				testFST.SetInputSymbols(isyms);
				testFST.SetOutputSymbols(isyms);

				Compose(testFST, *fstLetras, &resultFST); 
						
				ShortestPath(resultFST, &resultFSTOut1, 1);
					
				if(resultFSTOut1.NumStates() > 0 ){
					printString(resultFSTOut1, resultado);
					return true; 
				}else{
					resultado = L""; 
					return false; 
				}
					
			
			}else{
				// Se pronuncia como esta 
				resultado = aux;
				return true; 
			}
		}else{
			return false; 
		}
	}
		
	return false; 
}
