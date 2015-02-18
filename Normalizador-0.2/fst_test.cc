
#include <fst/fstlib.h>
#include <fst/fst-decl.h>
#include <fst/union.h>


#include "freeling.h"

#include "config.h" 

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

int main(int argc, char *argv[]) {

		//Inicializar
	   StdVectorFst testFST, resultFST, resultFSTOutN, resultFSTOut1, testFST1, testFST2, testFST3;
	   StdFst *fst = NULL; 
	
	   int states, start; 
	   
	   bool match; 
	   
	   float p = 0, pf = 0;
	   wstring sTest;
	   int nshortest, fstTagGeneration = 1, cantCaracteres, n = 10;
	   
	   string UbicacionTextoModif, UbicacionTexto;
	   wstringstream archivo;
	   bool stdInput = true, stdOutput = true; 
	   
	   Config c; 

	   //Listado de palabras 
	   list<freeling::word> palabras;
	   
	   // iterador
	   list<freeling::word>::iterator w; 
	   
	   std::wstring s; 
	   
	   /// Freeling: set locale to an UTF8 compatible locale
	   freeling::util::init_locale(L"es_AR.UTF-8");
	   
	   freeling::tokenizer tk(L"tokenizer.dat");
	   
	   // Arbol con un diccionario de siglas 
	   AvlTree<diccItem> siglas;
	   
	   char ArchivoSiglas[200]; 
		
	   strcpy(ArchivoSiglas, "siglas.txt");
	   
	   // Importa el archivo de siglas
	   ImportPalabrasATraducir(&siglas, ArchivoSiglas);
	   
	   // Arbol con un diccionario de abreviaturas
	   AvlTree<diccItem> abrev;
	   
	   char ArchivoAbrev[200]; 
		
	   strcpy(ArchivoAbrev, "Abreviaturas.txt");
	   
	   // Importa el archivo de abreviaturas
	   ImportPalabrasATraducir(&abrev, ArchivoAbrev);
	   	   
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

		if (stdInput){
			//Tomo la entrada de standard imput
			archivo << wcin.rdbuf() << L'\0';

		}else{
			//tomo la entrada de un archivo 
			wifstream archivoEntrada (UbicacionTexto.c_str(), ios_base::in);
			archivo << archivoEntrada.rdbuf() << L'\0';
		}
		
		wofstream archivoSalida1 (UbicacionTextoModif.c_str());
	      

	//Tabla de simbolos
		fst::SymbolTable *isyms = fst::SymbolTable::ReadText("./WFST/simbolos.csv");
		fst::SymbolTable *osyms = fst::SymbolTable::ReadText("./WFST/simbolos.csv");

	// Importa los FSTs	

		// Números Naturales 
		StdFst *fstNaturales = StdFst::Read("./WFST/out/NaturalesMasc/NaturalesMasc.fst");		
	 		
		// Ordinales Femeninos 
		StdFst *fstOrdinalesFem = StdFst::Read("./WFST/out/OrdinalesFem/Ordinales.fst");		
		
		//Ordinales Masculinos 
		StdFst *fstOrdinalesMasc = StdFst::Read("./WFST/out/OrdinalesMasc/Ordinales.fst");		
												
		
		//Ordinales Neutros 
		StdFst *fstOrdinalesNeutro = StdFst::Read("./WFST/out/OrdinalesNeutro/Ordinales.fst");		
		
		// Números reales
		StdFst *fstReales = StdFst::Read("./WFST/out/Reales/Reales.fst");		


		// Romanos Naturales 
		StdFst *fstRomanosNat = StdFst::Read("./WFST/out/RomanosNat/Romanos.fst");		
		
		// Romanos ordinales femeninino
		StdFst *fstRomanosOrdF = StdFst::Read("./WFST/out/RomanosOrdF/Romanos.fst");		

		// Romanos ordinales masculino
		StdFst *fstRomanosOrdM = StdFst::Read("./WFST/out/RomanosOrdM/Romanos.fst");		

		//Romanos ordinales neutro
		StdFst *fstRomanosOrdN = StdFst::Read("./WFST/out/RomanosOrdN/Romanos.fst");		
	
		//Fechas
		StdFst *fstFechas = StdFst::Read("./WFST/out/Fechas/Fechas.fst");		

		//Horas
		StdFst *fstHoras = StdFst::Read("./WFST/out/Horas/Horas.fst");		

		//Teléfonos
		StdFst *fstTelefonos = StdFst::Read("./WFST/out/Telefonos/Telefonos.fst");		

		//Importes 
		StdFst *fstImportes = StdFst::Read("./WFST/out/Importes/Importes.fst");		

		//Fracciones 
		StdFst *fstFracciones = StdFst::Read("./WFST/out/Fracciones/Fracciones.fst");		

		//Grados 
		StdFst *fstGrados = StdFst::Read("./WFST/out/Grados/Grados.fst");		
		
		
		//Letras 
		StdFst *fstLetras = StdFst::Read("./WFST/out/Letras/Letras.fst");		
			
		
		// Lee el archivo de configuración 
		char ConfigFile[200]; 
		strcpy(ConfigFile, "./WFST/fst_test/config.txt");
		c.Read(ConfigFile); 
				
		// Expresión Regular para Números Naturales 		
		wstring NaturalesStr = c.ExpRegular(L"Naturales"); 
		boost::wregex NaturalesRE (NaturalesStr,boost::regex_constants::icase|boost::regex_constants::perl);
		
		// Expresión Regular para Números Reales  		
		wstring RealesStr = c.ExpRegular(wstring(L"Reales")); 
		boost::wregex RealesRE (RealesStr,boost::regex_constants::icase|boost::regex_constants::perl);
		
		// Expresión Regular para Fechas  		
		wstring FechasStr = c.ExpRegular(L"Fechas"); 
		boost::wregex FechasRE (FechasStr,boost::regex_constants::icase|boost::regex_constants::perl);
		
		// Expresión Regular para Horas  		
		wstring HorasStr = c.ExpRegular(L"Horas"); 
		boost::wregex HorasRE (HorasStr,boost::regex_constants::icase|boost::regex_constants::perl);
		
		// Expresión Regular para Fracciones  		
		wstring FraccionesStr = c.ExpRegular(L"Fracciones"); 
		boost::wregex FraccionesRE (FraccionesStr,boost::regex_constants::icase|boost::regex_constants::perl);
		
		// Expresión Regular para Grados   		
		wstring GradosStr = c.ExpRegular(L"Grados"); 
		boost::wregex GradosRE (GradosStr,boost::regex_constants::icase|boost::regex_constants::perl);
		
		// Expresión Regular para Importes   		
		wstring ImportesStr = c.ExpRegular(L"Importes"); 
		boost::wregex ImportesRE (ImportesStr,boost::regex_constants::icase|boost::regex_constants::perl);

		// Expresión Regular para Romanos
		boost::wregex RomanosRE (L"(C|I|X|V|M|L|D)+",boost::regex_constants::icase|boost::regex_constants::perl);
				
					
		// Arreglo con las plabras de contexto 
		wstring pal[10];
		wstring resultado, regla; 
		int j, f, CntPalAvanzar; 	

		while (getline (archivo, sTest))
         {
           cantCaracteres = sTest.length();
		   		   
		   if (cantCaracteres > 1)
		   {
			  // tokenize input line into a list of words
			  tk.tokenize(sTest, palabras);
			   
			  for (w = palabras.begin(); w != palabras.end(); w++) {
		
				// s es la palabra actual   
				s = w->get_form();			
							
				// Analizo la palabra para ver si hay que expandirla 
				match = false;
				
				// Obtengo las 5 palabras anteriores y posteriores 
				ObtenerContexto(pal, &palabras, w);
				
				// Inicializo la variable 
				CntPalAvanzar = 0;
				
				if (EsUnidadMedida(s, pal, resultado, regla, CntPalAvanzar, fstNaturales, fstOrdinalesFem, fstOrdinalesMasc, fstOrdinalesNeutro, fstReales, fstLetras,  &siglas, &abrev, isyms)){
					s = resultado; 
					match = false; 
				}else if (EsMoneda(s, pal, resultado, regla, CntPalAvanzar, fstNaturales, fstOrdinalesFem, fstOrdinalesMasc, fstOrdinalesNeutro, fstReales, fstLetras,  &siglas, &abrev,isyms)){
					s = resultado; 
					match = false; 
				}else if (EsFecha(s, pal, resultado,  fstFechas, isyms)){ 
					s = resultado; 
					match = false; 
				}else if (EsHora(s, pal, resultado,  fstNaturales, isyms)){ // No pude hacer funcionar el fst de horas 
					s = resultado; 
					match = false; 
				}else if (ContieneGrados(s, pal, resultado, fstNaturales, fstReales, fstOrdinalesFem, fstOrdinalesMasc, fstOrdinalesNeutro, isyms)){
					s = resultado; 
					match = false; 
				}else if (EsNro(s, pal, resultado,regla,fstNaturales, fstOrdinalesFem, fstOrdinalesMasc, fstOrdinalesNeutro, fstReales, fstLetras, &siglas, &abrev, isyms)){
					s = resultado; 
					match = false; 
				}else if (EsNroRomano(s, pal, resultado, regla,fstNaturales, fstOrdinalesFem, fstOrdinalesMasc, fstOrdinalesNeutro, isyms)){
					s = resultado; 
					match = false; 
				}else if(EsSiglaOAbreviatura(s, &siglas, &abrev, resultado, fstLetras, isyms)){
					s =  resultado; 
					match = false;
				}
				
				
				if (match){ // Hay que expandir la palabra 
				
				  //Reemplazo los caracteres de acuerdo a lo definido en el archivo de configuración
				  c.Replace(s);
						  
				  testFST = string2fst(p, pf, s); 
				  
				  testFST.SetOutputSymbols(isyms);

				  Compose(testFST, *fst, &resultFST);

				  ShortestPath(resultFST, &resultFSTOut1, nshortest=1);
							  
				  showEvalFST(0, resultFSTOut1, sTest, archivoSalida1);
				
				  							
				}else{ // No se expande la palabra
					
					if (s.compare(L"<S>") == 0 )
						archivoSalida1 << s << L'\n'; 
					else 
						archivoSalida1 << s << L' ';
					
					archivoSalida1.flush();
		
				}
				
				// Avanzo el iterador para saltear las palabras ya procesadas 
				for (f = 0; f < CntPalAvanzar && w != palabras.end(); f++)
					w++; 
				
			 }
		   }
		 }	   
		
		archivoSalida1.close();
		
		//Mando el archivo de salida a stdout
		wstringstream Stream;
		
		wstring TodoTexto;		
		
		wifstream Archivo (UbicacionTextoModif.c_str(), ios_base::in);
		
		Stream << Archivo.rdbuf() << L'\0';
				
		TodoTexto = (Stream.str().c_str());
		
		wcout << TodoTexto << L'\n';				
       

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
