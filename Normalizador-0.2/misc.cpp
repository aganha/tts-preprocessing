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
// misc.cpp
// Funciones de soporte para generación de FSTs  
/* =============================================================== */
#include <fst/fstlib.h>
#include <fst/fst-decl.h>
#include <fst/union.h>
#include <string>
#include "utilfst.h"
using namespace fst;
void printAllStrings(StdVectorFst &fst, wofstream &archivoSalida, wstring testName, bool conCosto);

// ================
// Mostrar FST
// ================
void showFST(char *dirName, char *fileName){ 
	char s[200];

// Imprimir FST
	sprintf(s,"fstprint ./WFST/out/%s/%s.fst > ./WFST/out/%s/%s.txt",dirName,fileName,dirName,fileName);
	system(s);

//Dibujar FST
	sprintf(s,"fstdraw -portrait ./WFST/out/%s/%s.fst > ./WFST/out/%s/%s.dot",dirName,fileName,dirName,fileName);
	system(s);
};

// ================
// fst2txt
// ================
void fst2txt(char *dirName, char *fileName){ 
	char s[200];

	sprintf(s,".//WFST//out//%s/%s.txt",dirName,fileName);

    ofstream archivoSalida(s, ofstream::out);

    if( archivoSalida.good() )
    {
       archivoSalida << "Dale Ciclón!!!" << endl;
       archivoSalida.close();
    };
};

// ================
// Mostrar Test FST
// ================

void showTestFST(int i, StdVectorFst testFst, StdVectorFst resultFst, wstring testName, wofstream &archivoSalida){ 
	char s[200];
	int cantEstados;
    bool conCosto = true;

    sprintf(s,"./WFST/out/Test/Test%i.fst",i);
	testFst.Write(s);

	sprintf(s,"Test%i",i);
	showFST("Test", s);

	sprintf(s,"./WFST/out/Result/Result%i.fst",i);
	resultFst.Write(s);

	sprintf(s,"Result%i",i);
	showFST("Result",s);

	sprintf(s,"Result%i",i);

    cantEstados = (resultFst).NumStates();
//
//	archivoSalida << "CANTIDAD DE ESTADOS: " << cantEstados << endl;
   	archivoSalida << "ENTRADA: " << testName << endl;
	archivoSalida << "SALIDA: " << endl;

	if (cantEstados != 0)
	   printAllStrings(resultFst, archivoSalida, testName, conCosto);
	else
		archivoSalida << L"Traducción no encontrada para: " << testName << endl;
};

void showEvalFST(int i, StdVectorFst resultFst, wstring testName, wofstream &archivoSalida){ 
	char s[200];
	int cantEstados;
    bool conCosto = false;

    cantEstados = (resultFst).NumStates();
	if (cantEstados != 0)
	   printAllStrings(resultFst, archivoSalida, testName, conCosto);
	else
		archivoSalida << L"Traducción no encontrada para: " << testName << endl;
};

// ================
// Optimizar FST
// ================
StdVectorFst optFST(StdVectorFst unionFst) {
	StdVectorFst detFst;

	EncodeMapper<StdArc> encoder(kEncodeLabels, ENCODE);
    Encode(&unionFst, &encoder);

//	RmEpsilon(&unionFst);
	Determinize(unionFst,&detFst);
	Minimize(&detFst);

    Decode(&detFst, encoder); 
	RmEpsilon(&detFst);

	return( detFst );
};
// ================
// FST 1Cero = 0
// ================
StdVectorFst gen1Cero(int p, int pf) {
// A vector FST is a general mutable FST
   StdVectorFst cero;

// Agregar estado 0  
   cero.AddState();    
   cero.SetStart(0);  

// Agregar arcos al estado 0
   cero.AddArc(0, StdArc(SYMBOL_0, SYMBOL_eps, p, 1));  

// Agregar estado 1 y setearlo como estado final 
   cero.AddState();
   cero.SetFinal(1, pf);   

   return( cero );
};

// ================
// FST 2Cero = 00
// ================
StdVectorFst gen2Cero(int p, int pf) {
// A vector FST is a general mutable FST
   StdVectorFst cero;

// Agregar estado 0  
   cero.AddState();    
   cero.SetStart(0);  

// Agregar arcos al estado 0
   cero.AddArc(0, StdArc(SYMBOL_0, SYMBOL_eps, p, 1));  

// Agregar estado 1 and its arcs. 
   cero.AddState();
   cero.AddArc(1, StdArc(SYMBOL_0, SYMBOL_eps, p, 2));  

// Agregar estado 2 y setearlo como estado final 
   cero.AddState();
   cero.SetFinal(2, pf);   

   return( cero );
};

// ================
// FST 3Cero = 000
// ================
StdVectorFst gen3Cero(int p, int pf) {
// A vector FST is a general mutable FST
   StdVectorFst cero;

// Agregar estado 0  
   cero.AddState();    
   cero.SetStart(0);  

// Agregar arcos al estado 0
   cero.AddArc(0, StdArc(SYMBOL_0, SYMBOL_eps, p, 1));  

// Agregar estado 1 and its arcs. 
   cero.AddState();
   cero.AddArc(1, StdArc(SYMBOL_0, SYMBOL_eps, p, 2));  

// Agregar estado 2 and its arcs. 
   cero.AddState();
   cero.AddArc(2, StdArc(SYMBOL_0, SYMBOL_eps, p, 3));

// Agregar estado 3 y setearlo como estado final 
   cero.AddState();
   cero.SetFinal(3, pf);   

   return( cero );
};

// ================
// FST 4Cero = 0000
// ================
StdVectorFst gen4Cero(int p, int pf) {
// A vector FST is a general mutable FST
   StdVectorFst cero;

// Agregar estado 0  
   cero.AddState();    
   cero.SetStart(0);  

// Agregar arcos al estado 0
   cero.AddArc(0, StdArc(SYMBOL_0, SYMBOL_eps, p, 1));  

// Agregar estado 1 and its arcs. 
   cero.AddState();
   cero.AddArc(1, StdArc(SYMBOL_0, SYMBOL_eps, p, 2));  

// Agregar estado 2 and its arcs. 
   cero.AddState();
   cero.AddArc(2, StdArc(SYMBOL_0, SYMBOL_eps, p, 3));

// Agregar estado 3 and its arcs. 
   cero.AddState();
   cero.AddArc(3, StdArc(SYMBOL_0, SYMBOL_eps, p, 4));

// Agregar estado 4 y setearlo como estado final 
   cero.AddState();
   cero.SetFinal(4, pf);   

   return( cero );
};

// ================
// FST 5Cero = 00000
// ================
StdVectorFst gen5Cero(int p, int pf) {
// A vector FST is a general mutable FST
   StdVectorFst cero;

// Agregar estado 0  
   cero.AddState();    
   cero.SetStart(0);  

// Agregar arcos al estado 0
   cero.AddArc(0, StdArc(SYMBOL_0, SYMBOL_eps, p, 1));  

// Agregar estado 1 and its arcs. 
   cero.AddState();
   cero.AddArc(1, StdArc(SYMBOL_0, SYMBOL_eps, p, 2));  

// Agregar estado 2 and its arcs. 
   cero.AddState();
   cero.AddArc(2, StdArc(SYMBOL_0, SYMBOL_eps, p, 3));

// Agregar estado 3 and its arcs. 
   cero.AddState();
   cero.AddArc(3, StdArc(SYMBOL_0, SYMBOL_eps, p, 4));

// Agregar estado 4 and its arcs. 
   cero.AddState();
   cero.AddArc(4, StdArc(SYMBOL_0, SYMBOL_eps, p, 5));

// Agregar estado 5 y setearlo como estado final 
   cero.AddState();
   cero.SetFinal(5, pf);   

   return( cero );
};

// ================
// FST 6Cero = 000000
// ================
StdVectorFst gen6Cero(int p, int pf) {
// A vector FST is a general mutable FST
   StdVectorFst cero;

// Agregar estado 0  
   cero.AddState();    
   cero.SetStart(0);  

// Agregar arcos al estado 0
   cero.AddArc(0, StdArc(SYMBOL_0, SYMBOL_eps, p, 1));  

// Agregar estado 1 and its arcs. 
   cero.AddState();
   cero.AddArc(1, StdArc(SYMBOL_0, SYMBOL_eps, p, 2));  

// Agregar estado 2 and its arcs. 
   cero.AddState();
   cero.AddArc(2, StdArc(SYMBOL_0, SYMBOL_eps, p, 3));

// Agregar estado 3 and its arcs. 
   cero.AddState();
   cero.AddArc(3, StdArc(SYMBOL_0, SYMBOL_eps, p, 4));

// Agregar estado 4 and its arcs. 
   cero.AddState();
   cero.AddArc(4, StdArc(SYMBOL_0, SYMBOL_eps, p, 5));

// Agregar estado 5 and its arcs. 
   cero.AddState();
   cero.AddArc(5, StdArc(SYMBOL_0, SYMBOL_eps, p, 6));

// Agregar estado 6 y setearlo como estado final 
   cero.AddState();
   cero.SetFinal(6, pf);   

   return( cero );
};

// ================
// Convertir caracter a simbolo
// ================
enum symbol  char2symbol (wchar_t iSymbol) {
enum symbol elSimbolo;
   switch(iSymbol)
   {
      case L'0' : elSimbolo = SYMBOL_0 ; break ;
      case L'1' : elSimbolo = SYMBOL_1 ; break ;
      case L'2' : elSimbolo = SYMBOL_2 ; break ;
      case L'3' : elSimbolo = SYMBOL_3 ; break ;
      case L'4' : elSimbolo = SYMBOL_4 ; break ;
      case L'5' : elSimbolo = SYMBOL_5 ; break ;
      case L'6' : elSimbolo = SYMBOL_6 ; break ;
      case L'7' : elSimbolo = SYMBOL_7 ; break ;
      case L'8' : elSimbolo = SYMBOL_8 ; break ;
      case L'9' : elSimbolo = SYMBOL_9 ; break ;
      case L'°' : elSimbolo = SYMBOL_ORD ; break ;
      case L'.' : elSimbolo = SYMBOL_PUN ; break ;
      case L',' : elSimbolo = SYMBOL_COM ; break ;
      case L'+' : elSimbolo = SYMBOL_MAS ; break ;
      case L'-' : elSimbolo = SYMBOL_MEN ; break ;
      case L'a' : elSimbolo = SYMBOL_a ; break ;
      case L'b' : elSimbolo = SYMBOL_b ; break ;
      case L'c' : elSimbolo = SYMBOL_c ; break ;
      case L'd' : elSimbolo = SYMBOL_d ; break ;
      case L'e' : elSimbolo = SYMBOL_e ; break ;
      case L'f' : elSimbolo = SYMBOL_f ; break ;
      case L'g' : elSimbolo = SYMBOL_g ; break ;
      case L'h' : elSimbolo = SYMBOL_h ; break ;
      case L'i' : elSimbolo = SYMBOL_i ; break ;
      case L'j' : elSimbolo = SYMBOL_j ; break ;
      case L'k' : elSimbolo = SYMBOL_k ; break ;
      case L'l' : elSimbolo = SYMBOL_l ; break ;
      case L'm' : elSimbolo = SYMBOL_m ; break ;
      case L'n' : elSimbolo = SYMBOL_n ; break ;
      case L'o' : elSimbolo = SYMBOL_o ; break ;
      case L'p' : elSimbolo = SYMBOL_p ; break ;
      case L'q' : elSimbolo = SYMBOL_q ; break ;
      case L'r' : elSimbolo = SYMBOL_r ; break ;
      case L's' : elSimbolo = SYMBOL_s ; break ;
      case L't' : elSimbolo = SYMBOL_t ; break ;
      case L'u' : elSimbolo = SYMBOL_u ; break ;
      case L'v' : elSimbolo = SYMBOL_v ; break ;
      case L'w' : elSimbolo = SYMBOL_w ; break ;
      case L'x' : elSimbolo = SYMBOL_x ; break ;
      case L'y' : elSimbolo = SYMBOL_y ; break ;
      case L'z' : elSimbolo = SYMBOL_z ; break ;
      case L'ñ' : elSimbolo = SYMBOL_ene ; break ;
      case L'á' : elSimbolo = SYMBOL_aca ; break ;
      case L'é' : elSimbolo = SYMBOL_ace ; break ;
      case L'í' : elSimbolo = SYMBOL_aci ; break ;
      case L'ó' : elSimbolo = SYMBOL_aco ; break ;
      case L'ú' : elSimbolo = SYMBOL_acu ; break ;
      case L'A' : elSimbolo = SYMBOL_A ; break ;
      case L'B' : elSimbolo = SYMBOL_B ; break ;
      case L'C' : elSimbolo = SYMBOL_C ; break ;
      case L'D' : elSimbolo = SYMBOL_D ; break ;
      case L'E' : elSimbolo = SYMBOL_E ; break ;
      case L'F' : elSimbolo = SYMBOL_F ; break ;
      case L'G' : elSimbolo = SYMBOL_G ; break ;
      case L'H' : elSimbolo = SYMBOL_H ; break ;
      case L'I' : elSimbolo = SYMBOL_I ; break ;
      case L'J' : elSimbolo = SYMBOL_J ; break ;
      case L'K' : elSimbolo = SYMBOL_K ; break ;
      case L'L' : elSimbolo = SYMBOL_L ; break ;
      case L'M' : elSimbolo = SYMBOL_M ; break ;
      case L'N' : elSimbolo = SYMBOL_N ; break ;
      case L'O' : elSimbolo = SYMBOL_O ; break ;
      case L'P' : elSimbolo = SYMBOL_P ; break ;
      case L'Q' : elSimbolo = SYMBOL_Q ; break ;
      case L'R' : elSimbolo = SYMBOL_R ; break ;
      case L'S' : elSimbolo = SYMBOL_S ; break ;
      case L'T' : elSimbolo = SYMBOL_T ; break ;
      case L'U' : elSimbolo = SYMBOL_U ; break ;
      case L'V' : elSimbolo = SYMBOL_V ; break ;
      case L'W' : elSimbolo = SYMBOL_W ; break ;
      case L'X' : elSimbolo = SYMBOL_X ; break ;
      case L'Y' : elSimbolo = SYMBOL_Y ; break ;
      case L'Z' : elSimbolo = SYMBOL_Z ; break ;
      case L'Ñ' : elSimbolo = SYMBOL_ENE ; break ;
      case L'Á' : elSimbolo = SYMBOL_ACA ; break ;
      case L'É' : elSimbolo = SYMBOL_ACE ; break ;
      case L'Í' : elSimbolo = SYMBOL_ACI ; break ;
      case L'Ó' : elSimbolo = SYMBOL_ACO ; break ;
      case L'Ú' : elSimbolo = SYMBOL_ACU ; break ;
      case L' ' : elSimbolo = SYMBOL_ESP ; break ;
      case L'/' : elSimbolo = SYMBOL_BAR ; break ;
      case L'(' : elSimbolo = SYMBOL_PAP ; break ;
      case L')' : elSimbolo = SYMBOL_PCE ; break ;
      case L'<' : elSimbolo = SYMBOL_SME ; break ;
      case L'>' : elSimbolo = SYMBOL_SMA ; break ;
      case L'_' : elSimbolo = SYMBOL_GBA ; break ;
	  case L':' : elSimbolo = SYMBOL_DPU ; break ;
	  case L'!' : elSimbolo = SYMBOL_ADMC ; break ;
	  case L'"' : elSimbolo = SYMBOL_QUOT ; break ;
	  case L'#' : elSimbolo = SYMBOL_SHARP ; break ;
	  case L'$' : elSimbolo = SYMBOL_PESOS ; break ;
	  case L'%' : elSimbolo = SYMBOL_PERCENT ; break ;
	  case L'&' : elSimbolo = SYMBOL_UMPER ; break ;
	  case L'*' : elSimbolo = SYMBOL_AST ; break ;
	  case L';' : elSimbolo = SYMBOL_PYC ; break ;
	  case L'=' : elSimbolo = SYMBOL_IGUAL ; break ;
	  case L'?' : elSimbolo = SYMBOL_PREGC ; break ;
	  case L'@' : elSimbolo = SYMBOL_ARROBA ; break ;
	  case L'[' : elSimbolo = SYMBOL_CAP ; break ;
	  case L']' : elSimbolo = SYMBOL_CAC ; break ;
	  case L'^' : elSimbolo = SYMBOL_CIRC ; break ;
	  case L'`' : elSimbolo = SYMBOL_APO ; break ;
	  case L'{' : elSimbolo = SYMBOL_LLA ; break ;
	  case L'|' : elSimbolo = SYMBOL_PIPE ; break ;
	  case L'}' : elSimbolo = SYMBOL_LLC ; break ;
	  case L'~' : elSimbolo = SYMBOL_TILDE ; break ;
	  case L'\'' : elSimbolo = SYMBOL_CUTE ; break ;
	  case L'\\' : elSimbolo = SYMBOL_BARRAINV ; break ;
  	  case L'º' : elSimbolo = SYMBOL_ORDB ; break ;   
  	  case L'¡' : elSimbolo = SYMBOL_ADMA ; break ;
      case L'«' : elSimbolo = SYMBOL_COMA ; break ;
	  case L'»' : elSimbolo = SYMBOL_COMC ; break ;
	  case L'¿' : elSimbolo = SYMBOL_PREA ; break ;
	  case L'Ä' : elSimbolo = SYMBOL_DIEA ; break ;
	  case L'Ë' : elSimbolo = SYMBOL_DIEE ; break ;
	  case L'Ï' : elSimbolo = SYMBOL_DIEI ; break ;
	  case L'Ö' : elSimbolo = SYMBOL_DIEO ; break ;
	  case L'Ü' : elSimbolo = SYMBOL_DIEU ; break ;
	  case L'ä' : elSimbolo = SYMBOL_diea ; break ;
	  case L'ë' : elSimbolo = SYMBOL_diee ; break ;
	  case L'ï' : elSimbolo = SYMBOL_diei ; break ;
	  case L'ö' : elSimbolo = SYMBOL_dieo ; break ;
	  case L'ü' : elSimbolo = SYMBOL_dieu ; break ;
	  default :  elSimbolo = SYMBOL_eps;
   }
   return( elSimbolo ); 
};
// ================
// Convertir string a FST: symbol -> eps
// ================
StdVectorFst string2fstInput(int p, int pf, string sInput ){
StdVectorFst fstOutput;
enum symbol elSimbolo;
char cSymbol;
int state_from = 0, state_to = 1;

// Agregar estado 0 y setearlo como estado inicial  
   fstOutput.AddState();    
   fstOutput.SetStart(0);  

   for (std::size_t i = 0; i <=  sInput.length()-1; i++)
   {
     cSymbol  = sInput.at(i);
     elSimbolo = char2symbol (cSymbol);
	 fstOutput.AddArc(state_from, StdArc(elSimbolo, SYMBOL_eps, p, state_to)); 
     fstOutput.AddState();
	 state_from++;
	 state_to++;
   };

     fstOutput.SetFinal(state_to - 1 , pf);

	 return( fstOutput ); 
};

// Tengo que agregar "a mano" el simbolo de orden ° porque no funciona
StdVectorFst string2fstOrdinal(int p, int pf, wstring sInput ){
StdVectorFst fstOutput;
enum symbol elSimbolo;
char cSymbol;
int state_from = 0, state_to = 1;

// Agregar estado 0 y setearlo como estado inicial  
   fstOutput.AddState();    
   fstOutput.SetStart(0);  

   for (std::size_t i = 0; i <=  sInput.length()-1; i++)
   {
     cSymbol  = sInput.at(i);
     elSimbolo = char2symbol (cSymbol);
	 fstOutput.AddArc(state_from, StdArc(elSimbolo, elSimbolo, p, state_to)); 
     fstOutput.AddState();
	 state_from++;
	 state_to++;
   };

	 elSimbolo = SYMBOL_ORD; 
	 fstOutput.AddArc(state_from, StdArc(elSimbolo, elSimbolo, p, state_to)); 
     fstOutput.AddState();
	 state_from++;
	 state_to++;

     fstOutput.SetFinal(state_to - 1 , pf);
	 fstOutput.SetStart(0);

	 return( fstOutput ); 
};


// ================
// Convertir string a FST: eps -> symbol
// ================
StdVectorFst string2fstOutput(int p, int pf, string sInput ){
StdVectorFst fstOutput;
enum symbol elSimbolo;
char cSymbol;
int state_from = 0, state_to = 1;

// Las salidas deben tener todas el mismo peso independientemente de la longitud del texto (por eso se resetean)
   p = 0;
   pf = 0;

// Agregar estado 0 y setearlo como estado inicial  
   fstOutput.AddState();    
   fstOutput.SetStart(0);  

   for (std::size_t i = 0; i <=  sInput.length()-1; i++)
   {
     cSymbol  = sInput.at(i);
     elSimbolo = char2symbol (cSymbol);
	 fstOutput.AddArc(state_from, StdArc(SYMBOL_eps, elSimbolo, p, state_to)); 
     fstOutput.AddState();
	 state_from++;
	 state_to++;
   };

     fstOutput.SetFinal(state_to - 1 , pf);

	 return( fstOutput ); 
};

// ================
// Convertir string a FST: symbol -> symbol
// ================
StdVectorFst string2fst(int p, int pf, wstring sInput ){
StdVectorFst fstOutput;
enum symbol elSimbolo;
char cSymbol;
int state_from = 0, state_to = 1;

// Agregar estado 0 y setearlo como estado inicial  
   fstOutput.AddState();    
   fstOutput.SetStart(0);  

   for (std::size_t i = 0; i <=  sInput.length()-1; i++)
   {
     cSymbol  = sInput.at(i);
     elSimbolo = char2symbol (cSymbol);
	 fstOutput.AddArc(state_from, StdArc(elSimbolo, elSimbolo, p, state_to)); 
     fstOutput.AddState();
	 state_from++;
	 state_to++;
   };

     fstOutput.SetFinal(state_to - 1 , pf);

	 return( fstOutput ); 
};

// ================
// FST Epsilon = 0 (peso = 1)
// ================
StdVectorFst genEpsilon() {
// A vector FST is a general mutable FST
   StdVectorFst epsilon;

// Agregar estado 0  
   epsilon.AddState();    
   epsilon.SetStart(0);  

// Agregar arcos al estado 0
   epsilon.AddArc(0, StdArc(SYMBOL_eps, SYMBOL_eps, 1, 1));  

// Agregar estado 1 y setearlo como estado final 
   epsilon.AddState();
   epsilon.SetFinal(1, 3);   

   return( epsilon );
};

// ================
// FST Digitos = 0 - 9
// ================
StdVectorFst genDigitos(int p, int pf) {
   StdVectorFst inicialDig[10], fstOutputDig[10], digitos;
   int nSymbol;
   string sOutputDig[10];

// Inicializar dígitos
   sOutputDig[0] = "cero ";
   sOutputDig[1] = "uno ";
   sOutputDig[2] = "dos ";
   sOutputDig[3] = "tres ";
   sOutputDig[4] = "cuatro ";
   sOutputDig[5] = "cinco ";
   sOutputDig[6] = "seis ";
   sOutputDig[7] = "siete ";
   sOutputDig[8] = "ocho ";
   sOutputDig[9] = "nueve ";

   nSymbol = SYMBOL_0; 

 for (int j = 0; j <=  9; j++)
 {
// Generar fst con inicial
   inicialDig[j].AddState();    
   inicialDig[j].SetStart(0);  

// Agregar arcos al estado 0
   inicialDig[j].AddArc(0, StdArc(nSymbol, SYMBOL_eps, p, 1)); //0 -> eps .... 9 -> eps
   
// Set state 1 final weight. 
   inicialDig[j].AddState();    
   inicialDig[j].SetFinal(1, pf);  

// Pasar al siguiente simbolo
   nSymbol++;

// Generar fst con expansión de la inicial
   fstOutputDig[j] = string2fstOutput(p, pf, sOutputDig[j]); 

// Juntar inicial con su expansión
   Concat(&inicialDig[j], fstOutputDig[j]);

   inicialDig[j] = optFST(inicialDig[j]);

// Union de todas las iniciales
   Union(&digitos, inicialDig[j]);
 };

   return( digitos );
};
