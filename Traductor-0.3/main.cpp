#include <fstream>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <locale>
#include "freeling.h"
#include <boost/regex.hpp>

using namespace std;

// headers del arbol binario de búsqueda
#include "Avl.h"
#include "Avl2.h" 

// Dato que guardo en el arbol que uso de diccionario
#include "diccitem.h"

// Importa una tupla <palabra, pronunciación> de un archivo de texto y las inserta en un árbol 
bool ImportPalabrasATraducir(AvlTree<diccItem> *dicc, char *filename); 

// Evalúa si una palabra es del idioma inglés 
bool EsPalabraInglesa(wstring &palabra, wstring &pronunciacion, AvlTree<diccItem> &listado, AvlTree<diccItem> &dicc, boost::wregex &ER); 

// Obtiene las 5 palabras anteriores y posteriores a una palabra dada 
void ObtenerContexto(wstring* pal, list<freeling::word> *palabras, list<freeling::word>::iterator &w); 

// Busca una palabra en un árbol que contiene un listado de tuplas <palabra, pronunciación> y devuelve la pronunciación
bool BuscarPronunciacion(wstring &palabra, wstring &pronunciacion, AvlTree<diccItem> &l);


int main(int argc, char **argv)
{
	   
	   string UbicacionTextoModif, UbicacionTexto;
	   wstringstream Stream;
	   	      
	   bool stdInput = true, stdOutput = true;
	   int cantCaracteres; 

	   /// Freeling: set locale to an UTF8 compatible locale
	   freeling::util::init_locale(L"es_AR.UTF-8");
	   
	   //Listado de palabras 
	   list<freeling::word> palabras;
	   
	   // iterador
	   list<freeling::word>::iterator w; 
	   
	   // Arbol de las palabras a Traducir seguro
	   AvlTree<diccItem> listado;

	   // Arbol de las palabras del diccionario fonético 
	   AvlTree<diccItem> dicc;
	   	   
	   std::wstring s, sTest, lower, pronunciacion, pronAux, singular, infinitivo; 
	   
	   // Arreglo de cadenas donde se guardan las 5 palabras anteriores [0-4] y las 5 posteriores [5-9] 
	   // pal[4] es la inmediata anterior y pal[5] es la inmediata posterior. 
	   wstring pal[10];
	   
	   
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
			Stream << wcin.rdbuf() << L'\0';
		}else{
			
			//tomo la entrada de un archivo 
			wifstream Archivo (UbicacionTexto.c_str(), ios_base::in);
			
			if (!Archivo.good()){
				printf("No se puede abrir el archivo: %s\n", UbicacionTexto.c_str());
				return -1; 
			}
			
			Stream << Archivo.rdbuf() << L'\0';
					
		}
			
		// Crea un objeto tokenizer de Freeling 	
 	   freeling::tokenizer tk(L"tokenizer.dat");
					
		wofstream TextoModif (UbicacionTextoModif.c_str());
		
		if (!TextoModif.good()){
			printf("No se puede abrir el archivo: %s\n", UbicacionTexto.c_str());
			return -1; 
		}	
		
		char ArchivoPalabrasIngles[2000]; 
		char ArchivoDiccIngles[2000]; 
		
		strcpy(ArchivoPalabrasIngles, "PalabrasIngles.txt");
		
		strcpy(ArchivoDiccIngles, "DiccIngles.txt");
		
		if(!ImportPalabrasATraducir(&listado, ArchivoPalabrasIngles)){
			printf("No se puede importar el archivo: %s\n", ArchivoPalabrasIngles);
			return -1; 
		}

		if(!ImportPalabrasATraducir(&dicc, ArchivoDiccIngles)){
			printf("No se puede importar el archivo: %s\n", ArchivoDiccIngles);
			return -1; 
		}
				
		// Expresion Regular 
		boost::wregex ER (L"((w)+|(sh)+|(th)+|(ck)+|(wn)+|(oo)+|(ph)+|(my)+|(ou)+|^sc|tion$)",boost::regex_constants::icase|boost::regex_constants::perl);
			
		while (getline (Stream, sTest))
		{
           cantCaracteres = sTest.length();
				   
		   if (cantCaracteres > 1)
		   {
			  // tokenize input line into a list of words
			  tk.tokenize(sTest, palabras);
			   
				for (w = palabras.begin(); w != palabras.end(); w++) {
				
					// s es la palabra actual   
					s = w->get_form();	
					  
					if(s.length()>0){
							
						lower = s; 
				
						// Convierte a minúscula 
						std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
						
						if (EsPalabraInglesa(lower, pronunciacion, listado, dicc, ER)){
							
							// imprimo la pronunciación 
							if(stdOutput)
								wcout << pronunciacion << L' ';
							else
								TextoModif << pronunciacion << L' ';  
							
							goto fin; // uso goto porque break funciona mal en mi compilador. Altera el ciclo del while en lugar del for. 
							
						}
							
						// Veo si es una palabra en plural, terminada en "s", "es".
						// o un verbo en tercera persona del singular.
						//Ejemplo: machines, goes. Busco: machin, machine, go, goe.
						
						if (lower.length()>=4){
							
							/* No lo hago porque pronuncia en inglés palabras que son del español 
						      Ejemplo: dos. Le quito la s y encuentro do y entonces se pronuncia "dus"
	
							if(lower.substr(lower.length()-2, 2) == L"es"){
								
								singular = lower.substr(0, lower.length()-2);
								
								if (EsPalabraInglesa(singular, pronunciacion, listado, dicc, ER)){
									
									// A la pronunciación que obtengo le agrego "es" 
									pronunciacion = pronunciacion + L"es"; 
	
									// imprimo la pronunciación 
									if(stdOutput)
										wcout << pronunciacion << ' ';
									else
										TextoModif << pronunciacion << ' ';  
									
									goto fin;
								
								}
							}
							 */
						
							// Evalúo si termina con "s" 
						/*	if(lower.substr(lower.length()-1, 1) == L"s"){
								
								singular = lower.substr(0, lower.length()-1);
								
								if (EsPalabraInglesa(singular, pronunciacion, listado, dicc, ER)){
									
									// A la pronunciación que obtengo le agrego "es" 
									pronunciacion = pronunciacion + L"s"; 
	
									// imprimo la pronunciación 
									if(stdOutput)
										wcout << pronunciacion << ' ';
									else
										TextoModif << pronunciacion << ' ';  
										
									goto fin;
								
								}
							}
							*/
							
							// Veo si es un verbo en pasado, terminación "ed"
							//Ejemplo: visited. Busco: visit

							if(lower.substr(lower.length()-2, 2) == L"ed"){
								//Prueba quitandole la terminación "ed"
								infinitivo = lower.substr(0, lower.length()-2);
								
								if (EsPalabraInglesa(infinitivo, pronunciacion, listado, dicc, ER)){
									
									// A la pronunciación que obtengo le agrego la pronunciación "id" 
									pronunciacion = pronunciacion + L"id"; 

									// imprimo la pronunciación 
									if(stdOutput)
										wcout << pronunciacion << L' ';
									else
										TextoModif << pronunciacion << L' ';  
										
									goto fin;
								
								}else{
									//Prueba quitandole la terminación "d"
									infinitivo = lower.substr(0, lower.length()-1);
								
									if (EsPalabraInglesa(infinitivo, pronunciacion, listado, dicc, ER)){
										
										// A la pronunciación que obtengo le agrego la pronunciación "id" 
										pronunciacion = pronunciacion + L"id"; 

										// imprimo la pronunciación 
										if(stdOutput)
											wcout << pronunciacion << L' ';
										else
											TextoModif << pronunciacion << L' ';  
											
										goto fin;
									
									}
								}
								
							}
							
							
							// Veo si es un verbo en presente continuo, terminación "ing"
							//Ejemplo: visiting. Busco: visit

							if(lower.substr(lower.length()-3, 3) == L"ing"){
								
								infinitivo = lower.substr(0, lower.length()-3);
								
								if (EsPalabraInglesa(infinitivo, pronunciacion, listado, dicc, ER)){
									
									// A la pronunciación que obtengo le agrego la pronunciación "ing" 
									pronunciacion = pronunciacion + L"ing"; 

									// imprimo la pronunciación 
									if(stdOutput)
										wcout << pronunciacion << L' ';
									else
										TextoModif << pronunciacion << L' ';  
									
									goto fin;
								
								}
							}
							
						
						}
													
						//Caso especial "I"
						if (s == L"I"){
							
							// Obtengo las 5 palabras anteriores y posteriores 
							ObtenerContexto(pal, &palabras, w);
															
							// Si la palabra anterior es inglesa, la considero también inglesa 
							lower = pal[4]; 
							// Convierte a minúscula 
							std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
	
							if (EsPalabraInglesa(lower, pronAux, listado, dicc, ER)){
								
								// La palabra anterior es inglesa, la considero inglesa
								
								// imprimo la pronunciación 
								if(stdOutput)
									wcout << pronunciacion << L' ';
								else
									TextoModif << pronunciacion << L' ';  
								
								goto fin;
																	
							}
								
							// Si la palabra posterior es inglesa, la considero también inglesa 
							lower = pal[5]; 
							
							// Convierte a minúscula 
							std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

							if (EsPalabraInglesa(lower, pronAux, listado, dicc, ER)){
								
								// La palabra anterior es inglesa, la considero inglesa
								
								// imprimo la pronunciación 
								if(stdOutput)
									wcout << pronunciacion << L' ';
								else
									TextoModif << pronunciacion << L' ';  
								
								goto fin; 
								
							}
						}
						
						// No se considera una palabra inglesa, se deja como esta. 
						if (stdOutput){
							if (s.compare(L"<S>") == 0 )
								wcout << s << L'\n'; 
							else 
								wcout << s << L' '; 
									
						}else{
							
							if (s.compare(L"<S>") == 0 )
								TextoModif << s << L'\n'; 
							else 
								TextoModif << s << L' '; 
						}
						fin:;
					}
				}	
			}
		}	   
		
        TextoModif.close();

   return 0;
}

// Importa el archivo de palabras que se traducen obligatoriamente
bool ImportPalabrasATraducir(AvlTree<diccItem> *dicc, char *filename){

	wstringstream archivo;
	
	wstring sTest, pal, pron; 
	
	diccItem nueva;
	
	Comparable<diccItem> * CompDiccItem;
	
	int cantCaracteres; 
	
	wifstream archivoEntrada (filename, ios_base::in);

	if (!archivoEntrada.good()){
		return false; 
	}
	
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
   
   return true; 
	
}


// Obtiene las 5 palabras anteriores y posteriores para analizar el contexto de una palabra 
// pal debe apuntar a un arreglo de 10 strings donde quedan las palabras anteriores y posteriores 
// anterior5, anterior4,...,anterior1, posterior1, posterior2,...,posterior5
void ObtenerContexto(wstring* pal, list<freeling::word> *palabras, list<freeling::word>::iterator &w){
	
	int j; 
	
	list<freeling::word>::iterator aux;
				
	// Borro todas las palabras del arreglo
	for(j = 0; j < 10; j++)
		pal[j] = L"";


	// Busco las 5 palabras anteriores 
	j = 4;
	// retrocedo con el iterador aux cinco palabras
	for (aux = w; aux != palabras->begin() && j >= 0; j--){
		aux--; 
		pal[j] = aux->get_form(); 
		
	}

	// Busco las 5 palabras posteriores 
	j = 5;

	// retrocedo con el iterador aux cinco palabras
	for (aux = w; aux != palabras->end() && j < 10; j++){
		aux++; 
		if(aux != palabras->end()){
			pal[j] = aux->get_form(); 
		
		}

	}

}

// Evalúa si una palabra es del idioma inglés 
bool EsPalabraInglesa(wstring &palabra, wstring &pronunciacion, AvlTree<diccItem> &listado, AvlTree<diccItem> &dicc, boost::wregex &ER){ 

	if (BuscarPronunciacion(palabra, pronunciacion, listado)){
		return true; 
	
	}else{
		// No esta en el listado de palabras frecuentes 
		// Evalúo las reglas de palabras en inglés 
		// Excluyo letras sueltas. 
		
		if ( palabra.length() > 1 &&  boost::regex_search(palabra, ER, boost::regex_constants::format_perl)){
			// Cumple alguna de las reglas 
			
			if (BuscarPronunciacion(palabra, pronunciacion, dicc)){
				// Además esta en el diccionario
				return true; 
			}
		}
	}
	
	return false; 
} 


bool BuscarPronunciacion(wstring &palabra, wstring &pronunciacion, AvlTree<diccItem> &l){
	
	diccItem buscar;
	diccItem *p;

	Comparable<diccItem> * found = NULL;

	//Defino el valor a buscar 
	buscar.SetWord(palabra, L""); 
	
	// Vacío la cadena donde queda la pronunciación 
	pronunciacion = L""; 
	
	// Busco la palabra en el árbol
	found = l.Search(buscar);
	
	if (found) { 
		p = found->GetKey(); //Obtengo la pronunciación 
		// Copio la pronunciación 
		pronunciacion =  p->pronunciacion; 
		return true; 
	
	}
	
	return false; 
}
