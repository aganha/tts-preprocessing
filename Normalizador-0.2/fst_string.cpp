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

/* =============================================================== */
// fst_string.cpp
// Funciones de soporte para convertir FSTs en strings  
/* =============================================================== */
#include <fst/fstlib.h>
#include <fst/fst-decl.h>
#include <fst/union.h>
#include <string>
#include <iostream>
#include <vector>
#include <fst/vector-fst.h>
#include <fst/weight.h>
#include "utilfst.h"
using namespace fst;

// ================
// Convert int to string; from Bjarne Stroustrup's FAQ
// ================
string itos(int i) { 
   stringstream s;
   string txt;

   int j = 65;
   char c = (char)i;
   if (c != '\0')
   {
	   s << c ;
       txt = s.str();
   };


   return txt;
}

// ================
// Convert vector to string; 
// ================
string vectorToString(vector<int> v) {

   if(v.size() == 0)
      return "<>";
   string result = itos(v[0]) ;

   for(std::size_t i = 1; i < v.size(); i++) {
      result = result + itos(v[i]);
   }

   return result;
}

// ================
// Convert vector to string UTF8; 
// ================
wstring vectorToStringUTF8(vector<int> v) {

   if(v.size() == 0)
      return L"<>";
	  
   string result = itos(v[0]) ;

   for(std::size_t i = 1; i < v.size(); i++) {
      result = result + itos(v[i]);
   }

	string utf8str; 
	
	// Convierto a UTF8
	utf8str = CP1252ToUTF8(result.c_str());
	
	
	
//	return utf8str; 
	
	return utf8_to_utf16(utf8str); 
	
//	std::wstring wStr = strtowstr(result);
//	std::string utf8Resuslt = to_utf8(wStr); 
//	return utf8Result;
	
}

// ================
// Imprimir todas las cadenas (Auxiliar)
// ================
void printAllStringsHelper(StdVectorFst& fst, int state, vector<int> str, TropicalWeight cost, wofstream &archivoSalida, bool conCosto)  {


   // Si el estado es estado final retornar el string 
   if(fst.Final(state) != TropicalWeight::Zero())      //.Final(state) = Peso del estado final 
   {   
	   //archivoSalida << vectorToString(str);   //Convertir el vector en string
	   archivoSalida << vectorToStringUTF8(str);   //Convertir el vector en string UTF8

	   if (conCosto == true)
	   { 
		   //archivoSalida << " COSTO: " << Times(cost,fst.Final(state)); 
	   };  
	   
	   //archivoSalida << endl;   
	   //archivoSalida << ' ';   
   }; 

   // ArcIterator = An arc iterator doc is used to access the transitions leaving an FST state.
   // ArcIterator <VectorFst <StdArc> > aiter(fst,state) = aiter es el iterador de arcos en el estado 'state' del 'fst'
   // aiter.Done() = End of iterator
   // aiter.Next() = Advance to next arc (when !Done)  
   for(ArcIterator <VectorFst <StdArc> > aiter(fst,state); ! aiter.Done(); aiter.Next()) {
	  StdArc arc = aiter.Value();                     //.Value() = Current arc (when !Done) 
      str.push_back(arc.olabel);                      //.push_back = Appends given element to the end of the container. 
                                                      //arc.olabel, arc.nextstate = atributos del arco 'arc'  
      printAllStringsHelper(fst, arc.nextstate, str, Times(cost, arc.weight.Value()), archivoSalida, conCosto); //recusive step for the next state
      str.pop_back();                                //Removes the last element in the vector, effectively reducing the vector
	                                                 //size by one and invalidating all iterators and references to it.  
   }
}


// ================
// Imprimir todas las cadenas 
// ================
// a bad idea if there are loops :)
//void printAllStrings(StdVectorFst &fst, char *dirName, char *fileName, string testName) {
void printAllStrings(StdVectorFst &fst, wofstream &archivoSalida, wstring testName, bool conCosto) {

   
      vector<int> str(0);
      TropicalWeight tw(TropicalWeight::One());
	  
      printAllStringsHelper(fst, fst.Start(), str, tw, archivoSalida, conCosto);
}

// ================
// Imprimir todas las cadenas (Auxiliar)
// ================
void printAllStringsHelper2(StdVectorFst& fst, int state, vector<int> str, TropicalWeight cost, wstring &resultado, bool conCosto)  {


   // Si el estado es estado final retornar el string 
   if(fst.Final(state) != TropicalWeight::Zero())      //.Final(state) = Peso del estado final 
   {   
	   //archivoSalida << vectorToString(str);   //Convertir el vector en string
	   resultado  =  vectorToStringUTF8(str);   //Convertir el vector en string UTF8
 
	   //archivoSalida << endl;   
	   //archivoSalida << ' ';   
   }; 

   // ArcIterator = An arc iterator doc is used to access the transitions leaving an FST state.
   // ArcIterator <VectorFst <StdArc> > aiter(fst,state) = aiter es el iterador de arcos en el estado 'state' del 'fst'
   // aiter.Done() = End of iterator
   // aiter.Next() = Advance to next arc (when !Done)  
   for(ArcIterator <VectorFst <StdArc> > aiter(fst,state); ! aiter.Done(); aiter.Next()) {
	  StdArc arc = aiter.Value();                     //.Value() = Current arc (when !Done) 
      str.push_back(arc.olabel);                      //.push_back = Appends given element to the end of the container. 
                                                      //arc.olabel, arc.nextstate = atributos del arco 'arc'  
      printAllStringsHelper2(fst, arc.nextstate, str, Times(cost, arc.weight.Value()), resultado, conCosto); //recusive step for the next state
      str.pop_back();                                //Removes the last element in the vector, effectively reducing the vector
	                                                 //size by one and invalidating all iterators and references to it.  
   }
}


void printString(StdVectorFst &fst, wstring &resultado) {

   
      vector<int> str(0);
      TropicalWeight tw(TropicalWeight::One());
	  
      printAllStringsHelper2(fst, fst.Start(), str, tw, resultado, false);
}
