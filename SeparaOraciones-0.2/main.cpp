// Copyright 2015 Universidad de Buenos Aires
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <fstream>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <locale>


#include "freeling.h"


using namespace std;

void PrintResults (wofstream &sout, list<freeling::sentence > &ls, bool stdOuput);

std::wstring StringToWString(const std::string& s)
 {
 std::wstring temp(s.length(),L' ');
 std::copy(s.begin(), s.end(), temp.begin());
 return temp; 
 }
	
	
int main(int argc, char **argv)
{
		string UbicacionTextoModif, UbicacionTexto;
		wstringstream Stream;
		
		wstring linea;
		int cantCaracteres;
		
		bool stdInput = true, stdOutput = true; 
				
		list<freeling::word> palabras;
		list<freeling::sentence> oraciones;
		
		/// Freeling: set locale to an UTF8 compatible locale
		freeling::util::init_locale(L"es_AR.UTF-8");
				
			
		freeling::tokenizer tk(L"tokenizer.dat");
	
		freeling::splitter sp(L"splitter.dat");
		
			
		UbicacionTexto = "HtmlToTxt.txt";
		UbicacionTextoModif = "TextoSeparado.txt";
		
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
			Stream << wcin.rdbuf() << '\0';
		}else{
		
			//tomo la entrada de un archivo 
			wifstream Archivo (UbicacionTexto.c_str(), ios_base::in);
			Stream << Archivo.rdbuf() << '\0';		
		}
		
		wofstream TextoModif (UbicacionTextoModif.c_str());
		
				
		while (getline (Stream, linea))
        {
		   cantCaracteres = linea.length();
		   
		   if (cantCaracteres > 1 ){
			   			   		  				
			    // tokenize input line into a list of words
			    tk.tokenize(linea, palabras);
			   	
				// accumulate list of words in splitter buffer, returning a list of sentences.
				sp.split(palabras, true, oraciones);
	
		
				PrintResults (TextoModif, oraciones, stdOutput);   
		   }
		 }
				
	return 0; 
}
 
void PrintResults (wofstream &sout, list<freeling::sentence > &ls, bool stdOutput) {
  
  freeling::sentence::const_iterator w;
  list<freeling::sentence>::iterator is;
  
  	for (is = ls.begin (); is != ls.end (); is++) {
    
	  for (w = is->begin(); w != is->end (); w++) {
		
		  sout << w->get_form() << L' ';			
		  
		  if (stdOutput)
				wcout << w->get_form() << L' ';			
		
	  }
    // sentence separator: blank line.
    sout << L"<S>" << L'\n';
	sout.flush();
	
	if (stdOutput)
		wcout << L"<S>" << L'\n';
    }

}


