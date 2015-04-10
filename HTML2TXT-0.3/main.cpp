#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <htmlcxx/html/ParserDom.h>

#include <ctime>

#include <boost/regex.hpp>
  
using namespace std;
using namespace htmlcxx;

void QuitarRef(string &s);
void QuitarTexto(string &s, const string& quitar);
void ProcesaTexto(string & s);
void ReemplazarTexto(string &s, const string& buscar, const string& reemplazar);
void NonBreakingSpaceSepMiles(string &s);
void rtrim(string &str);

static inline bool EsDigito(wchar_t a) {
        return (a=='0'||a=='1' || a=='2'||a=='2'||a=='3'||a=='4'||a=='5'||a=='6'||a=='7'||a=='8'||a=='9');
}

std::wstring StringToWString(const std::string& s){
 std::wstring temp(s.length(),L' ');
 std::copy(s.begin(), s.end(), temp.begin());
 return temp; 
}


	
int main(int argc, char **argv){

		string UbicacionTexto, UbicacionTextoModif, UbicacionArbol;
		
		stringstream Stream;
		string TodoTexto;
		string texto, itemText, textoAux; 
				
		bool esTitulo, esBullet; 
		bool stdInput, stdOutput; 

		stdInput = true;
		stdOutput= true;

		UbicacionTextoModif = "";
		UbicacionArbol = "";
		UbicacionTexto = "";

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

		UbicacionArbol = "arbol.txt";

		
		if (stdInput){
			Stream << cin.rdbuf() << '\0';
		}else{
			//tomo la entrada de un archivo 
			ifstream Archivo (UbicacionTexto.c_str(), ios_base::in);
			
			if (!Archivo.good()){
				printf("No se puede abrir el archivo: %s\n", UbicacionTexto.c_str());
				return -1; 
			}

			Stream << Archivo.rdbuf() << '\0';
		}
			
		ofstream TextoModif (UbicacionTextoModif.c_str());
		ofstream Arbol (UbicacionArbol.c_str());
		
		TodoTexto = (Stream.str().c_str());
		
		// Expresión regular para expresiones numéricas 
		boost::regex nrosER("^([.plus-sign.]|[.hyphen.]|[.space.]|\\-|\\−|\\.|,|[0-9])+$",boost::regex_constants::icase|boost::regex_constants::perl);

		
		HTML::ParserDom parser;
	
		tree<HTML::Node> dom = parser.parseTree(TodoTexto);
		tree<HTML::Node>::iterator it = dom.begin();
		tree<HTML::Node>::iterator end = dom.end();
		
		// imprime el arbol de parsing
		Arbol << dom << endl;
				
		  //Dump all text of the document
  		  it = dom.begin();
		  end = dom.end();
		  string TagName; 
		  int Pdepth, PdepthAux;
		  //int BodyDepth; 
		 
		  // Recorro el árbol hasta encontrar el tag <body>
		  
		  for (; it != end; ++it){
			  TagName = it->tagName();
			  if (strcasecmp(TagName.c_str(), "body") == 0){
					//BodyDepth = dom.depth(it); 
					break; 
					
			  }
		  }
		  
		  std::pair<bool, std::string>  f; 
		  //&& dom.depth(it) <= BodyDepth
		  for (; it != end ; ++it)
		  {
			  
			  
			if (it->isTag()){
				
				TagName = it->tagName();
				
				// Excluyo la tabla de contenidos toc 
				if (strcasecmp(TagName.c_str(), "div") == 0  )
				{
					// Obtengo el atributo id 	
					it->parseAttributes();
					f = it->attribute("id");
					if (f.first){
						if (strcasecmp(f.second.c_str(),"toc")  == 0){
											
							Pdepth = dom.depth(it); 
							
							++it;
							
							while (dom.depth(it) > Pdepth && it != end){
								++it;
							}
					
						}
						
					}
					
				}
				
				
						
				// Imprimo todo el subarbol de estos tags
				if (strcasecmp(TagName.c_str(), "P") == 0 || strcasecmp(TagName.c_str(), "h1") == 0   || 
				    strcasecmp(TagName.c_str(), "h2") == 0 || strcasecmp(TagName.c_str(), "h3") == 0  ||
					strcasecmp(TagName.c_str(), "h4") == 0 || strcasecmp(TagName.c_str(), "h5") == 0 ||
					strcasecmp(TagName.c_str(), "h6") == 0 || strcasecmp(TagName.c_str(), "h7") == 0 ||
					/*strcasecmp(TagName.c_str(), "ol") == 0 || strcasecmp(TagName.c_str(), "ul") == 0 ||*/
					strcasecmp(TagName.c_str(), "li") == 0 || strcasecmp(TagName.c_str(), "dt") == 0
					){
					
					Pdepth = dom.depth(it); 
					
					++it;
					
					
					esTitulo = (strcasecmp(TagName.c_str(), "h1") == 0 || strcasecmp(TagName.c_str(), "h2") == 0 || strcasecmp(TagName.c_str(), "h3") == 0); 
										
					esBullet = (strcasecmp(TagName.c_str(), "li") == 0); 
						
					
					texto = "";				
					
					while (dom.depth(it) > Pdepth && it != end){
						
						TagName = it->tagName();
						
						// Salteo los títulos de las galerias de fotos 
						if (strcasecmp(TagName.c_str(), "div") == 0  )
						{	
							// Excluyo los títulos de las fotos  
							// Obtengo el atributo id 	
							it->parseAttributes();
							f = it->attribute("class");
					
							if (f.first){
							
								if (strcasecmp(f.second.c_str(),"gallerytext")  == 0){
									PdepthAux = dom.depth(it); 
						
									++it;
						
									while (dom.depth(it) > PdepthAux && it != end){
									++it;
									}
									
								}
				
							}
						}
					

						// Proceso los tags sup que correspondan a potencias. Por ejemplo. 10 a las -2 
						// aparece en html como "10<sup>-10</sup>
						
						TagName = it->tagName();
						
						if (strcasecmp(TagName.c_str(), "sup") == 0  )
						{	
							// Obtengo el atributo id 	
							it->parseAttributes();
							f = it->attribute("class");
							
							// Si no tienen class, es puro texto. Es lo que me interesa
							// los que tienen definido una clase son otros objetos, en gral referencias. 
							if (!(f.first)){
								PdepthAux = dom.depth(it); 
								++it;
																
								textoAux = ""; 
								
								while (dom.depth(it) > PdepthAux && it != end){
									
									if ((!it->isTag()) && (!it->isComment()) && strcasecmp(it->text().c_str(), "span") != 0)
									{	
										// concateno el texto
										textoAux = textoAux + it->text();
									
									}
									++it; 
									
								}
								
					
								// Verifico que el superíndice sea una expresión numérica y que el caracter anterior también lo sea 
								if (isdigit(texto[texto.length()-1]) && boost::regex_match (textoAux, nrosER, boost::regex_constants::format_perl))
									texto = texto + "<sup>" + textoAux + "</sup>"; // agrego el tag en el texto de salida para identificarlo como potencia
								else 
									texto = texto + textoAux; 
								
								// Son todos digitos, puntos o comas. 
								
									
							}
						}
						
						TagName = it->tagName();
						
						if ((!it->isTag()) && (!it->isComment()) && strcasecmp(it->text().c_str(), "span") != 0)
						{
							// concateno el texto
							texto = texto + it->text();
								
						}
						
						++it;
					}
					
					if (esTitulo && 
					   (texto.find("Véase también") != string::npos || texto.find("Enlaces externos") != string::npos || texto.find("Referencias") != string::npos 
					   || texto.find("Estadísticas") != string::npos ) )
						break; // termino  
								
						
						
					ProcesaTexto(texto); 
				
					// Agrego un punto al final del título para que se pueda separar mejor 
					if (esTitulo){
						// rtrim
						rtrim(texto);
						
						if (texto.length() > 0)
							texto += '.';
					}


					// Agrego un punto al final del bullet para que se pueda separar mejor
					if (esBullet){
						// rtrim
						rtrim(texto);
						
						if(texto.length()>0){
							if (!(texto[texto.length()-1] == '.')){
								texto += '.';
							}
						
						}
					}


					
					TextoModif << texto << endl; 
			
					if (stdOutput)
						cout << texto << endl;				
				}
				
				// Salteo todos el contenido de estos tags 
				if (strcasecmp(TagName.c_str(), "table") == 0 )
				{
					Pdepth = dom.depth(it); 
					
					++it;
					
					while (dom.depth(it) > Pdepth && it != end){
						++it;
					}
				}
				
				
				
				
			}else{
			
				texto = it->text();
				
				if (texto.find("<!-- /printfooter -->") != string::npos || texto.find("<!-- catlinks -->") != string::npos)
						break;
	
			}
		  }
		  
		  TextoModif << endl; 
		  
		  if (stdOutput)
			cout << endl;	
			
  
	return 0; 

}


void ProcesaTexto(string &s){
	QuitarTexto(s, "[editar]");
	QuitarTexto(s, "[cita requerida]");
	QuitarTexto(s, "[nota 1]");
	QuitarTexto(s,"[editar · editar fuente]");
	QuitarRef(s);
	
	// Normalizo el guión, que trae problemas posteriores. 
	ReemplazarTexto(s, "−", "-");
		
	// Normalizo el punto 	
	ReemplazarTexto(s, "·", ".");
		
	// Esto es para números en notación científica. Ej.: 3,4 . 10<sup>-1</sup> ---> 3,4x10<sup>-1</sup>
	ReemplazarTexto(s, "&#160;.&#160;10<sup>", "x10<sup>");
	ReemplazarTexto(s, " . 10<sup>", "x10<sup>");
	
	// Esto es para números en notación científica. Ej.: 3,4.10<sup>-1</sup> ---> 3,4x10<sup>-1</sup>
	ReemplazarTexto(s, ".10<sup>", "x10<sup>");
	
	// Esto es para números en notación científica. Ej.: 3,4 x 10<sup>-1</sup> ---> 3,4x10<sup>-1</sup>
	ReemplazarTexto(s, "&#160;x&#160;10<sup>", "x10<sup>");
	ReemplazarTexto(s, " x 10<sup>", "x10<sup>");
	
	// Reemplaza los espacios que se usan como separador de miles. Ej.: 32 000 000 km ---> 32000000 km 
	NonBreakingSpaceSepMiles(s); 
	
	// Convierte el resto de los non-breaking spaces en espacios 
	ReemplazarTexto(s, "&#160;", " ");
	
	ReemplazarTexto(s, "&amp;", "&");

}

void rtrim(string &str){
	std::string whitespaces (" \t\f\v\n\r");

	std::size_t found = str.find_last_not_of(whitespaces);
	
	if (found!=std::string::npos)
		str.erase(found+1);
	else
		str.clear();    

}


void QuitarTexto(string &s, const string& quitar){
	
	std::size_t inicio, largo; 
		
	inicio = s.find(quitar); 
	
	while(inicio != string::npos){
		largo = quitar.length();
		s.erase(inicio, largo); 
		inicio = s.find(quitar); 
	}
}


void ReemplazarTexto(string &s, const string& buscar, const string& reemplazar){
	
	std::size_t inicio, largo; 
		
	inicio = s.find(buscar); 
	
	while(inicio != wstring::npos){
		//inserto un espacio antes
		s.insert(inicio, reemplazar);
		inicio +=reemplazar.length();
		largo = buscar.length();
		s.erase(inicio, largo); 
		inicio = s.find(buscar); 
	}
}

// Quita de una cadena referencias numéricas con el formato [###]. Ej [10], [120] 
void QuitarRef(string &s){
	
	std::size_t inicio, fin, i; 
	bool allDigits; 
		
	inicio = s.find('['); 
		
	while(inicio != string::npos){

		fin = s.find(']', inicio);
		
		if (fin != string::npos){
			// Verifico que el contenido de los [], sean todos dígitos o espacios.
			allDigits = true; 
			for(i = inicio + 1; i < fin && allDigits; i++){
				allDigits &= (s[i] == '0' || s[i] == '1' || s[i] == '2' || s[i] == '3' || s[i] == '4' || s[i] == '5' || s[i] == '6'|| 
							s[i] == '7' || s[i] == '8'|| s[i] == '9'|| s[i] == ' ');
				
			}
			
			if (allDigits && fin > inicio ){
				s.erase(inicio, fin-inicio + 1); // elimino la cadena  		
				// Busco el siguiente '['
				inicio = s.find('[');
			}else{
				// Busco el siguiente '['
				inicio = s.find('[', fin);
			}
			
		}else{
			break; // No encontró el ']', termino el ciclo.
		}	
						
	}
	
	
}


// Quita de una cadena referencias numéricas con el formato [Nota #]. Ej [Nota 1]
void QuitarRef2(string &s){
	
	std::size_t inicio, fin, i; 
	bool allDigits; 
		
	inicio = s.find('['); 
		
	while(inicio != string::npos){

		fin = s.find(']', inicio);
		
		if (fin != string::npos){
			// Verifico que el contenido de los [], sean todos dígitos o espacios.
			allDigits = true; 
			for(i = inicio + 1; i < fin && allDigits; i++){
				allDigits &= (s[i] == '0' || s[i] == '1' || s[i] == '2' || s[i] == '3' || s[i] == '4' || s[i] == '5' || s[i] == '6'|| 
							s[i] == '7' || s[i] == '8'|| s[i] == '9'|| s[i] == ' ');
				
			}
			
			if (allDigits && fin > inicio ){
				s.erase(inicio, fin-inicio + 1); // elimino la cadena  		
				// Busco el siguiente '['
				inicio = s.find('[');
			}else{
				// Busco el siguiente '['
				inicio = s.find('[', fin);
			}
			
		}else{
			break; // No encontró el ']', termino el ciclo.
		}	
						
	}
	
	
}




void NonBreakingSpaceSepMiles(string &s){
	
	std::size_t inicio, i; 
	bool condicion; 
		
	inicio = s.find("&#160;"); 
		
	while(inicio != string::npos){

		
		//Verifico que el caracter anterior sea un dígito 
		if (inicio > 0 ){ 
			
			i = inicio-1;
			
			condicion = (s[i] == '0' || s[i] == '1' || s[i] == '2' || s[i] == '3' || s[i] == '4' || s[i] == '5' || s[i] == '6'|| 
							s[i] == '7' || s[i] == '8'|| s[i] == '9');
		
			
			if (condicion){
				
				//Verifico que los tres caracteres siguientes también sean dígitos
				// Muevo el indice a continuación del "&#160;" non-breaking spacea
				i = inicio + 6;
				if ( i+2 < s.length() ){
					
					condicion &= (s[i] == '0' || s[i] == '1' || s[i] == '2' || s[i] == '3' || s[i] == '4' || s[i] == '5' || s[i] == '6'|| 
							s[i] == '7' || s[i] == '8'|| s[i] == '9');
					
					i++; 
					
					condicion &= (s[i] == '0' || s[i] == '1' || s[i] == '2' || s[i] == '3' || s[i] == '4' || s[i] == '5' || s[i] == '6'|| 
							s[i] == '7' || s[i] == '8'|| s[i] == '9');
					
					i++; 
					
					condicion &= (s[i] == '0' || s[i] == '1' || s[i] == '2' || s[i] == '3' || s[i] == '4' || s[i] == '5' || s[i] == '6'|| 
							s[i] == '7' || s[i] == '8'|| s[i] == '9');
					
					if(condicion){
						//Es separador de miles. Ejemplo 1 000 000, quito el espacio
						s.erase(inicio, 6 ); // elimino la cadena 
						
						// Busco el siguiente 
						inicio = s.find("&#160;");
						
					}else{
						// Busco el siguiente 
						inicio = s.find("&#160;", inicio+6);
						
					}
					
				}else{
					//Ya estoy en el final de la cadena
					break; 
				}
			}else{
				// Busco el siguiente 
				inicio = s.find("&#160;", inicio+6);
			}
		}else{
			// Busco el siguiente 
			inicio = s.find("&#160;", inicio+6);
		}
								
	}
	
	
}
