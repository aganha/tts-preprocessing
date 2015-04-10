
#include <fst/fstlib.h>
//#include <fst/fst-decl.h>
//#include <fst/union.h>


#include "freeling.h"

#include <boost/lexical_cast.hpp>

#include <sstream>

#include "Avl.h"

// Dato que guardo en el arbol que uso de diccionario
#include "diccitem.h"

using namespace fst; 


void ObtenerContexto(wstring* pal, list<freeling::word> *palabras, list<freeling::word>::iterator &w);

int RomanoTOArabigo(wstring n);

bool EsNroRomano(wstring &s, wstring *pal, wstring &resultado, wstring &regla, StdFst *fstRomanosNat, StdFst *fstRomanosOrdF, 
				 StdFst *fstRomanosOrdM, StdFst *fstRomanosOrdN, fst::SymbolTable *isyms);
				 
bool EsNro(wstring &s, wstring *pal, wstring &resultado, wstring &regla, StdFst *fstNat, StdFst *fstOrdF, 
				 StdFst *fstOrdM, StdFst *fstOrdN, StdFst *fstReales, StdFst *fstLetras, AvlTree<diccItem> *siglas, AvlTree<diccItem> *abrev, fst::SymbolTable *isyms);

bool EsUnidadMedida(wstring &s, wstring *pal, wstring &resultado, wstring &regla,  int &CntPalabrasAvanzar, StdFst *fstNat, StdFst *fstOrdF, 
				 StdFst *fstOrdM, StdFst *fstOrdN, StdFst *fstReales, StdFst *fstLetras,  AvlTree<diccItem> *dicc, AvlTree<diccItem> *abrev, fst::SymbolTable *isyms);

bool EsMoneda(wstring &s, wstring *pal, wstring &resultado, wstring &regla,  int &CntPalabrasAvanzar, StdFst *fstNat, StdFst *fstOrdF, 
				 StdFst *fstOrdM, StdFst *fstOrdN, StdFst *fstReales, StdFst *fstLetras,  AvlTree<diccItem> *dicc, AvlTree<diccItem> *abrev, fst::SymbolTable *isyms);
				 
bool ExpandirUnidadMedida(wstring &str, AvlTree<diccItem> *dicc, AvlTree<diccItem> *abrev, wstring &resultado, StdFst *fstLetras, fst::SymbolTable *isyms);

bool ExpandirMoneda(wstring &str, wstring &resultado); 
				 
bool EsFecha(wstring &s, wstring *pal, wstring &resultado,  StdFst *fstFechas, fst::SymbolTable *isyms);				 

bool EsHora(wstring &s, wstring *pal, wstring &resultado,  StdFst *fstHoras, fst::SymbolTable *isyms);

bool ContieneGrados(wstring &s, wstring *pal, wstring &resultado,  StdFst *fstNat, StdFst *fstReales, StdFst *fstOrdinalesFem, StdFst *fstOrdinalesMasc, StdFst *fstOrdinalesNeutro, fst::SymbolTable *isyms);
				 
void QuitaAcentos(wstring &a); 				 

bool EsSiglaOAbreviatura(wstring &s, AvlTree<diccItem> *dicc, AvlTree<diccItem> *abrev,  wstring &resultado, StdFst *fstLetras, fst::SymbolTable *isyms);
	 
void ImportPalabrasATraducir(AvlTree<diccItem> *dicc, char *filename);

bool DeletrearSigla(wstring &s); 

bool esVocal(char c); 

void ReemplazarTexto(wstring &s, const wstring& buscar, const wstring& reemplazar); 

// trim from start
static inline std::wstring &ltrim(std::wstring &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::wstring &rtrim(std::wstring &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::wstring &trim(std::wstring &s) {
        return ltrim(rtrim(s));
}

// 
static inline bool EsDigito(wchar_t a) {
        return (a==L'0'||a==L'1' || a==L'2'||a==L'2'||a==L'3'||a==L'4'||a==L'5'||a==L'6'||a==L'7'||a==L'8'||a==L'9');
}



//Quita los puntos a una cadena
void QuitarPuntos(wstring &a);
