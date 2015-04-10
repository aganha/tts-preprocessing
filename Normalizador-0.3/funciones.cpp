#include "funciones.h"

StdVectorFst string2fst(int p, int pf, wstring sInput );
StdVectorFst string2fstOrdinal(int p, int pf, wstring sInput );
//void showFST(char *dirName, char *fileName);
//void showTestFST(int i, StdVectorFst testFst, StdVectorFst resultFst, wstring testName, ofstream &archivoSalida);
//void showEvalFST(int i, StdVectorFst resultFst, wstring testName, wofstream &archivoSalida);
//StdVectorFst optFST(StdVectorFst unionFst);
void printString(StdVectorFst &fst, wstring &resultado);


template <class T>
inline std::wstring to_wstring (const T& t)
{
std::wstringstream ss;
ss << t;
return ss.str();
}


void ReemplazarTexto(wstring &s, const wstring& buscar, const wstring& reemplazar){
	
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


void QuitarPuntos(wstring &a){
	std::size_t i; 
	
	for(i = 0; i < a.length(); i++){
		if (a[i] == L'.')
			a.erase(i, 1); 
		
	}
	
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
		QuitaAcentos(pal[j]);			
	}

	// Busco las 5 palabras posteriores 
	j = 5;

	// retrocedo con el iterador aux cinco palabras
	for (aux = w; aux != palabras->end() && j < 10; j++){
		aux++; 
		if(aux != palabras->end()){
			pal[j] = aux->get_form(); 
			QuitaAcentos(pal[j]);
		}

	}

}

// Determina si es un nro romano y lo expande en caso afirmativo. 
bool EsNroRomano(wstring &s, wstring *pal, wstring &resultado, wstring &regla, StdFst *fstRomanosNat, StdFst *fstRomanosOrdF, 
				 StdFst *fstRomanosOrdM, StdFst *fstRomanosOrdN, fst::SymbolTable *isyms){
	
	StdVectorFst testFST, resultFST, resultFSTOut1; 
	StdFst *fst; 
	int arabigo, i; 
	wstring str; 
	
	

	// Expresion Regular general para números romanos 
	boost::wregex ER (L"(C|I|X|V|M|L|D)+",boost::regex_constants::icase|boost::regex_constants::perl);
	
	// Expresion Regular general para números romanos bajos  
	boost::wregex ERBajos (L"(I|X|V)+", boost::regex_constants::icase|boost::regex_constants::perl);
	
	bool t= false;

	if (boost::regex_match (s, ER, boost::regex_constants::format_perl)){	
		
		arabigo = RomanoTOArabigo(s); 
		
		if (arabigo == -1){
			resultado = L"";
			regla = L"NoValido"; 
			return false; // No es un nro romano válido
		}
		
		
		wstring excluir = L"(dm|vid|Lic|Vic|im|XML|Ll|IC|CCD|CDC|lc|CMLL|XD|DD|ld|Dic|DMX|Ill|ICV|CIC|Mic|IMD|Lim|LM|Idi|ili|IMI|VM|ICC|DMC|CIL|dil|lid|DLC|XM|VCC|VC|vim|Lilli|IMC|VCI|CCM|Mim|CIM|Didi|Cixi|ICI|milli|MMIC|MVC|Dill|VCD|MVD|Dili|CMC|XXL|IIC|XLIX|Ivi|VLC|ixil|Iiii|ILM|Vili|XviD|XXXX|IDM|Divi|CVC|IDC|VX|Vl|ICM|DIMM|Vill|vidi|lvl|IDL|vici|DMD|MDM|Lidl|LMC|ICICI|mild|Cilic|IVIC|IIIc|LCL|VCL|CDM|VDM|ILC|IMM|CDMC|CLM|DDDDDD|VVD|MLC|VDV|MCMXC|IVM|IVC|MIMIC|VDI|Livi|VLDL|IIDX|CVD|Illici|LCM|Ilim|DVC|ICL|LDC|CILL|IMx|MLM|Ilid|DML|Illi|Cici|LMi|villi|iMix|ICD|LCI|XDM|Cicci|Cimi|VDC|iDVD|lilim|Vilm|LLM|VCM|Lill|IIIV|vmd|LCC|ILV|DMV|Ilic|VLM|VMI|LCMV|DMI|dividivi|VLD|VML|DLM|Ilici|Civili|LMV|Cilmi|MVCC|XDCC|LLD|Xil|IIm|DXM|idli|DDM|CLC|MLVM|IIx|ild|IMV|CVM|xxxxx|XIIII|CIDI|IID|MLD|VIIC|Dimdim|Dlimi|MIMD|VXD|Milicic|VMC|Mmmm|CCDMD|Lillix|LVM|dixi|CCCC|Vixil|Midl|Dimmi|MCMXCIX|XCIX|MVM|CVL|ICCCM|CIVI|DVL|CVX|XIC|MXML|milii|Midilli|ICIC|Xim|ICCC|VMX|IIID|DLD|IMViC|XMM|Idd|DMM|DLCI|Livii|LiCl|Vidic|Milici|CDMI|cilli|DCMC|XMI|LCCD|IIci|Milim|Viiv|Dici|VLL|Livilivi|CVCD|CVCC|VMM|Ixim|CLIL|LMD|DXD|Xidi|DVX|CIID|Idil|Viili|VXI|ICID|IIIx|MCMXCIII|illic|Ilicic|cddc|LDD|LLVM|mici|Livid|VIIII|CDVD|XXXXI|XXXXVII|Dimi|LDDC|MCMC|CMCC|IXD|LML|IML|VDL|VVVV|XVl|CCMD|idic|IMIM|XXXXIX|LvMI|IICC|Imil|LXD|Liiv|Micl|Vdd|IIL|xxiiii|LDV|ivil|Limi|ICMC|IIICD|VCDI|IVL|DILIXI|IdV|IICD|Milid|Mimmi|CLD|VXL|XXXIIII|CVCV|cccccc|VCLC|IMMC|Vidili|LCIC|Limm|CDLC|Civl|Lvx|Civici|CCVC|Dilli|MCCCCXXII|Didim|VLCC|dividi|MCMXCV|Cidcli|DDLC|ICVV|LIMICI|Immi|MVLL|DIVC|LMDC|IIMV|VLV|cDIM|lilii|IICV|ICDL|DILD|VCCI|Xlim|CICC|diiv|IIcx|CCIM|CCIC|LDM|CCMI|ICCID|ICCD|VVVVVV|DVM|CIIC|MICV|Micci|CICCM|DCIM|XXXXVIII|Imidi|iXi|Mmmmm|ICCM|LCDM|Milic|xilli|Cild|Cili|DVCD|IDX|CMDC|MCDC|civill|LMM|IVCI|Didic|IMDI|Dicc|MVL|Dividix|CDCL|VCV|MILC|Cimil|MIVI|VDX|MDMX|Imlil|MIXI|Vcx|CVVC|MIDIMIX|Xixi|LXXXX|imli|vicil|lidi|iMCM|Ilmi|IIIII|MDMC|VIDC|DMIX|mxD|clim|DIICC|Cimic|xxxxxX|dicdic|Vilic|CVID|Viivi|vivii|CCMIX|CDMCC|CIMD|Ixxi|Micic|CDCC|LDI|IILI|Vivil|XID|CXXC|Midili|Dilim|ICCV|MCMXCVII|lilili|CCCVC|Milll|IIIDD|Milil|LVC|MCCCCCI|MCCCCIX|MVLM|IMCL|CIICIC|ILMC|VIID|CCCD|VMIC|ciclic|MDIIII|xxxxc|MICC|XMD|Ixiim|mmmmmmmm|LCDLL|LCIII|MCMXCVI|iimx|Vidici|xDDD|VVDM|LVCM|Icil|IDDI|LXXXXVII|CCLM|LMMC|VVC|DLLM|XDCCC|vvl|Vilivili|cdic|Liclic|XvMC|limlim|DCCCCXXX|dddddddd|iiv|DLDI|ccccc|CMCI|XXXL|CCIIIIXX|Limix|MCCCCLXV|DCCCD|Cimilli|limx|CMID|CLMC|McCLIM|MCCCCLXXXV|VXIII|IIIIXIIII|CMVM|MILCCCCLXXXII|CICM|MXM|XVIIIC|CIMM|MCMXCIV|MLCDVVVI|XLIIII|DVCC|Dicivil|CILC|CCML|CCDM|VLI|IDDD|CCXLVIIII|cicl|MICID|LVD|CIIM|Vidil|vivim|Vild|idiv|DMCL|LICC|XLMV|DILIX|ilii|CIVIVM|Lvdi|Vilim|CXLIX|Milx|CCDC|villici|DIDD|Vicc|XXXXVI|lIMCMXLVII|LIIII|IIIM|civll|Illid|IIIIXII|LMLC|Lvivi|MCMXCII|MCCCCXCVII|VdXi|XLM|LMLV|LILIV|Livvi|MIDIM|XVll|CIMCI|Diid|Dimmidi|XMC|CIVC|CCMM|MDCCCLLVIIIIII|Ilil|XXXIXI|LMCD|Ilix|CXXLIII|dddd|MCDXCV|VIXI|LMLM|Xill|Cilix|MCCCCXC|DVDX|IDII|VIVC|LIDD|XVM|dvLM|IVCC|Liivi|Viddi|XLX|VCVV|MCMXCI|IMCC|mildi|IMMMM|XXXXII|MiiM|VDIM|MIIC|CIDIM|XVIIII|MDCXLIX|LCV|CCCCLVIII|climm|Cidli|MCMXCVIII|ICMI|xivil|MCCXXIIII|CVVCC|VLX|XDI|IDDM|XVIll|LLDC|ILIV|MLCD|CMIC|CxVM|MDCCCCVII|VidiVici|VIVIVI|CCDI|DMDC|MCID|Cvil|XXXXXIIII|CXD|CLDC|xDD|Ilci|ICCI|cilvil|ICIM|xilil|Xdmx|xXxxxxx|Llidi|MDLXXXXVII|XXVIIII|IMIC|VMV|ICMM|DIML|IIXL|CIDD|Ivid|XIIX|cLDL|MDXCIX|MCCCCCV|MCCCCLVIIIImi|mil|DVD|CD|di|DC|CV|il|Cid|mill|ml|ID|Lil|midi|cl|Did|CI|LCD|LI|Mimi|LIV|CLIC|LLC|CIVIC|VIL|LVIV|Lili|vv|mili|Dim|VD|vivi|mid|CDI|LDL|DivX|dll|vivid|mm|cm)";
		
		// Expresion Regular para valores a exluir
		boost::wregex excluirER (excluir, boost::regex_constants::icase|boost::regex_constants::perl);
		
		if (boost::regex_match (s, excluirER, boost::regex_constants::format_perl)){	
				resultado = L"";
				regla = L"0-Excluir";
				return false; 
		}
		
			
		// Rangos, períodos, enumeraciones 
		wstring rangos = L"(y|a|al|hasta|-|,)";
			
		// Expresion Regular para Rangos, períodos, enumeraciones
		boost::wregex RangosER (rangos, boost::regex_constants::icase|boost::regex_constants::perl);
		
		// La cadena esta formada por "I, X, V" y la palabra posterior1 es "y,a,al,hasta,-,," y posterior2 es también una cadena de "I, V, X"
		// o La cadena esta formada por "I, X, V" y la palabra anterior1 es "y,a,al,hasta,-,," y anterior2 es también una cadena de "I, V, X"
		if (boost::regex_match (s, ERBajos, boost::regex_constants::format_perl) &&  
			((boost::regex_match (pal[4], RangosER, boost::regex_constants::format_perl) && boost::regex_match (pal[3], ERBajos, boost::regex_constants::format_perl)) ||
			 (boost::regex_match (pal[5], RangosER, boost::regex_constants::format_perl) && boost::regex_match (pal[6], ERBajos, boost::regex_constants::format_perl)))){	

			// Expando como cardinales en todos los casos 
	 
			fst = fstRomanosNat; 
			str = to_wstring(arabigo); 
			testFST = string2fst(0, 0, str); 
			
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);

			Compose(testFST, *fst, &resultFST); 
						
			ShortestPath(resultFST, &resultFSTOut1, 1);
					
			if(resultFSTOut1.NumStates() > 0 )
				printString(resultFSTOut1, resultado);
			else 
				resultado = L""; 
						
			regla = L"1-Rangos";		
			return true; 
				
		}	
				
			
		boost::wregex SigloER(L"(siglo|siglos)+",boost::regex_constants::icase|boost::regex_constants::perl);

		// La palabra anterior es siglo/siglos
		if (boost::regex_match (pal[4], SigloER, boost::regex_constants::format_perl)){	
			
			// Del 1 al 10 lo expando como ordinal, los mayores al 10 como cardinales. 		
			if (arabigo >= 1 && arabigo <= 10) {
				fst = fstRomanosOrdM; 
				str = to_wstring(arabigo); 
				testFST = string2fstOrdinal(0, 0, str); // Lo uso porque tengo problemas concatenando el °
			}else{ 
				fst = fstRomanosNat; 
				str = to_wstring(arabigo); 
				testFST = string2fst(0, 0, str); 
			}
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);

			Compose(testFST, *fst, &resultFST); 
						
			ShortestPath(resultFST, &resultFSTOut1, 1);
					
			if(resultFSTOut1.NumStates() > 0 )
				printString(resultFSTOut1, resultado);
			else
				resultado = L""; 
			
			regla = L"2-Siglos";		
			return true; 
				
		}
		
		
		// Nombres propios masculinos 
		wstring nomMasc = L"(Carlos|Juan|Pedro|Pio|Jose|Fernando|Gregorio|Francisco|Jaime|Guillermo|Luis|Alfonso|Felipe|Maximiliano|Enrique|Pedro|Alejandro|Leopoldo|Federico|Manuel|Constantino|Eduardo|Benedicto|Dario|Jacobo|Jorge|Napoleon|Justiniano|Nicolas|Esteban|Pablo|Roberto|Dionisio|Amadeo|Miguel|Ramiro|Alejo|Teodosio|Sancho|Martin|Ricardo|Jerjes|Leon|Balduino|Alberto|Ladislao|Ptolomeo|Rovira|Teodoro|Clodoveo|Ramses|Jose|Julio|Gustavo|Cristian|Bonifacio|Sixto|Lopez|Paulo|Rodolfo|Andres|Andronico|Mitridates|Ramon|Muhammad|Ivan|Eugenio|Conrado|Haakon|Hugo|David|Alfredo|Augusto|Valentiniano|Mehmed|Bernardo|Adriano|Cosme|Puig|Raimundo|Garces|George|Segismundo|Honorio|Pio|Mohamed|Thutmose|Juba|Lotario|Roger|Seleuco|Francesco|Nabucodonosor|Ermengol|Teobaldo|Bermudo|Magnus|Diaz|Antonio|Demetrio|Giovanni|Constancio|Amaru|Humberto|Ciro|Louis|Casimiro|Gaston|Sanchez|Jaume|Saturno|Teodorico|Alarico|Boris|Lorenzo|Atano|Antigono|Godofredo|Kamehameha|Geza|Celestino|Garcia|Boleslao|Cambises|Eres|Hassan|Phanton|Simeon|Mohammed|Isaac|Amenemhat|Kim|Rakoczi|Niceforo|Estanislao|Sargon|atalo|Sergio|Rogelio|Wenceslao|Amenofis|Silvestre|Moctezuma|Valdemar|Seti|Hamid|Muhammed|oscar|Victor|Otto|Childeberto|Alonso|Guido|Gordiano|al-Rahman|Hieron|Svend|Hisham|Henry|Badley|James|Salmanasar|Claudio|Eumenes|Zhu|Dagoberto|Hasan|Agesilao|Yusuf|Christian|Cleomenes|Vologases|Marti|Torcuato|Alhaken|Tudhalia|Vladimir|Simon|Richard|Mehmet|Sesostris|Hunfredo|Osorkon|Ernesto|Mahmud|SERCOS|Etelredo|Ares|Naqada|Ariarates|Cristian|Anastasio|Marco|Henri|Constance|Dimitri|Bahram|Luis|Sigeberto|Psametico|Ruben|Jupiter|Menelik|Mursil|Yaroslav|Tomas|Belgrano|Pietro|Perdicas|Leonardo|Borrell|Nectanebo|Beyazid|Khan|Ahmed|Idris|Agis|Gusen|Canuto|Carlo|Teodorico|Raniero|Gasherbrum|Ponce|Danilo|Hercules|Nicomedes|Sebastian|Aegon|Amosis|Eudes|Hircano|Tigranes|Wilfredo|Clemente|Inocencio|Filipo|Oton|Berenguer|Mark|Antioco|Panzer|Vladimir)";
		
		// Expresion Regular para nombres propios masculinos
		boost::wregex NomMascER (nomMasc, boost::regex_constants::icase|boost::regex_constants::perl);
				
		// Nro Romano formado por I,V o X y palabra anterior = nombre propio masculino
		if (boost::regex_match (s, ERBajos, boost::regex_constants::format_perl) && boost::regex_match (pal[4], NomMascER, boost::regex_constants::format_perl)){	
			
			// Del 1 al 10 lo expando como ordinal, los mayores al 10 como cardinales. 		
			if (arabigo >= 1 && arabigo <= 10) {
				fst = fstRomanosOrdM; 
				str = to_wstring(arabigo); 
				testFST = string2fstOrdinal(0, 0, str); // Lo uso porque tengo problemas concatenando el ÃÂ°
			}else{ 
				fst = fstRomanosNat; 
				str = to_wstring(arabigo); 
				testFST = string2fst(0, 0, str); 
			}
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);

			Compose(testFST, *fst, &resultFST); 
						
			ShortestPath(resultFST, &resultFSTOut1, 1);
					
			if(resultFSTOut1.NumStates() > 0 )
				printString(resultFSTOut1, resultado);
			else
				resultado = L""; 
			
			regla = L"3-NomPropiosMasc";				
			return true; 
				
		}
		
		// Nombres propios femeninos 
		wstring nomFem = L"(Juana|Mari­a|Isabel|Catalina|Cleopatra|Victoria|Margarita|Elizabeth|Berenice|Beatriz)";
		
		// Expresion Regular para nombres propios masculinos
		boost::wregex NomFemER (nomFem, boost::regex_constants::icase|boost::regex_constants::perl);
				
		// Nro Romano formado por I,V o X y palabra anterior = nombre propio masculino
		if (boost::regex_match (s, ERBajos, boost::regex_constants::format_perl) && boost::regex_match (pal[4], NomFemER, boost::regex_constants::format_perl)){	
			
			// Del 1 al 10 lo expando como ordinal, los mayores al 10 como cardinales. 		
			if (arabigo >= 1 && arabigo <= 10) {
				fst = fstRomanosOrdF; 
				str = to_wstring(arabigo); 
				testFST = string2fstOrdinal(0, 0, str); // Lo uso porque tengo problemas concatenando el ÃÂ°
			}else{ 
				fst = fstRomanosNat; 
				str = to_wstring(arabigo); 
				testFST = string2fst(0, 0, str); 
			}
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);

			Compose(testFST, *fst, &resultFST); 
						
			ShortestPath(resultFST, &resultFSTOut1, 1);
					
			if(resultFSTOut1.NumStates() > 0 )
				printString(resultFSTOut1, resultado);
			else
				resultado = L""; 
							
			regla = L"4-NomPropiosFem";				
			return true; 
				
		}
		
		
		// Titulos de Nobleza Masculinos
		wstring TitNobMasc = L"(rey|conde|duque|marques|emperador|zar|sultan|emir|baron|principe)";
		
		// Expresion Regular para títulos de nobleza masculinos
		boost::wregex TitNobMascER (TitNobMasc, boost::regex_constants::icase|boost::regex_constants::perl);
				
		// Nro Romano formado por I,V o X y cualquiera de las 5 palabra anteriores o posteriores = título de nobleza
		bool t = false; 
		for(i=0; i < 10 && !t; i++)
			t = boost::regex_match (pal[i], TitNobMascER, boost::regex_constants::format_perl);
		
		if (t && boost::regex_match (s, ERBajos, boost::regex_constants::format_perl) ){	
			
			// Del 1 al 10 lo expando como ordinal, los mayores al 10 como cardinales. 		
			if (arabigo >= 1 && arabigo <= 10) {
				fst = fstRomanosOrdM; 
				str = to_wstring(arabigo); 
				testFST = string2fstOrdinal(0, 0, str); // Lo uso porque tengo problemas concatenando el °
			}else{ 
				fst = fstRomanosNat; 
				str = to_wstring(arabigo); 
				testFST = string2fst(0, 0, str); 
			}
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);

			Compose(testFST, *fst, &resultFST); 
						
			ShortestPath(resultFST, &resultFSTOut1, 1);
					
			if(resultFSTOut1.NumStates() > 0 )
				printString(resultFSTOut1, resultado);
			else
				resultado = L""; 
							
			regla = L"5-TitNomblezaMasc";				
			return true; 
				
		}
		
		// Titulos de Nobleza Femeninos
		wstring TitNobFem = L"(reina|condesa|duquesa|marquesa|emperatriz|zarina|baronesa|princesa)";
		
		// Expresion Regular para tí­tulos de nobleza Femeninos
		boost::wregex TitNobFemER (TitNobFem, boost::regex_constants::icase|boost::regex_constants::perl);
				
		// Nro Romano formado por I,V o X y cualquiera de las 5 palabra anteriores o posteriores = título de nobleza femenino
		t = false; 
		for(i=0; i < 10 && !t; i++)
			t = boost::regex_match (pal[i], TitNobFemER, boost::regex_constants::format_perl);
		
		if (t && boost::regex_match (s, ERBajos, boost::regex_constants::format_perl) ){	
			
			// Del 1 al 10 lo expando como ordinal, los mayores al 10 como cardinales. 		
			if (arabigo >= 1 && arabigo <= 10) {
				fst = fstRomanosOrdF; 
				str = to_wstring(arabigo); 
				testFST = string2fstOrdinal(0, 0, str); // Lo uso porque tengo problemas concatenando el °
			}else{ 
				fst = fstRomanosNat; 
				str = to_wstring(arabigo); 
				testFST = string2fst(0, 0, str); 
			}
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);

			Compose(testFST, *fst, &resultFST); 
						
			ShortestPath(resultFST, &resultFSTOut1, 1);
					
			if(resultFSTOut1.NumStates() > 0 )
				printString(resultFSTOut1, resultado);
			else
				resultado = L""; 
			
			regla = L"6-TitNomblezaFem";				
			return true; 
				
		}
				
		// Sustantivos anteriores 
		wstring sustAnt = L"(acto|ccapitulo|division|clase|tomo|vol.|vol|volumen|fase|grado|region|episodio|categoria|apendice|zona|padrino|pag.|pagina|edicion)";
		
		// Expresion Regular para sustantivos anteriores
		boost::wregex sustAntER (sustAnt, boost::regex_constants::icase|boost::regex_constants::perl);
				
		// Nro Romano formado por I,V o X y palabra anterior = sustantivos anteriores
		if (boost::regex_match (s, ERBajos, boost::regex_constants::format_perl) && boost::regex_match (pal[4], sustAntER, boost::regex_constants::format_perl)){	
			
			fst = fstRomanosNat; 
			str = to_wstring(arabigo); 
			testFST = string2fst(0, 0, str); 
		
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);

			Compose(testFST, *fst, &resultFST); 
						
			ShortestPath(resultFST, &resultFSTOut1, 1);
					
			if(resultFSTOut1.NumStates() > 0 )
				printString(resultFSTOut1, resultado);
			else
				resultado = L""; 
				
			regla = L"7-SustAnt";				
			return true; 
				
		}
			
		// Sustantivos posteriores Masculinos  
		wstring sustPostMasc = L"(distrito|congreso|campeonato|cuerpo|premio|festival|ejercito|marques|encuentro|milenio|duque|concurso|milenio|conde|grupo|salon|centenario|certamen|torneo|concilio|frente|sinodo|consejo|curso|baron|batallon|foro|conteo|plan|gobierno|imperio|regimiento|memorial|taller|gran|ciclo|trofeo|siglo|coloquio|seminario|comandante|sucesor|mundial|maraton)";
				
		// Expresion Regular para sustantivos anteriores
		boost::wregex sustPostMascER (sustPostMasc, boost::regex_constants::icase|boost::regex_constants::perl);
				
		// palabra posterior = sustantivos posteriores
		if ( boost::regex_match (pal[5], sustPostMascER, boost::regex_constants::format_perl)){	
			
			fst = fstRomanosOrdN; 
			str = to_wstring(arabigo); 
			testFST = string2fstOrdinal(0, 0, str); // Lo uso porque tengo problemas concatenando el °
			
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);

			Compose(testFST, *fst, &resultFST); 
						
			ShortestPath(resultFST, &resultFSTOut1, 1);
					
			if(resultFSTOut1.NumStates() > 0 )
				printString(resultFSTOut1, resultado);
			else
				resultado = L""; 
			
			regla = L"8-SustPostMasc";				
			return true; 
				
		}
		
		// Sustantivos posteriores Femeninos
		wstring sustPostFem = L"(guerra|legislatura|republica|edicion|division|region|bienal|copa|asamblea|conferencia|edad|exposicion|cumbre|dinastia|internacional|brigada|liga|bandera|feria|muestra|escuadra|fuerza|olimpiada|jornada|marquesa|convencion|duquesa|semana|cirvunnavegacion|condesa|escuela|universidad|batalla|cruzada|circunscripcion|vuelta|fase|promocion|parte|media|carrera|alternativa)";
		
		// Expresion Regular para sustantivos posteriores femeninos
		boost::wregex sustPostFemER (sustPostFem, boost::regex_constants::icase|boost::regex_constants::perl);
				
		//  palabra posterior = sustantivos posterior femenino
		if ( boost::regex_match (pal[5], sustPostFemER, boost::regex_constants::format_perl)){	
					
			fst = fstRomanosOrdF; 
			str = to_wstring(arabigo); 
			testFST = string2fstOrdinal(0, 0, str); // Lo uso porque tengo problemas concatenando el °
						
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);

			Compose(testFST, *fst, &resultFST); 
						
			ShortestPath(resultFST, &resultFSTOut1, 1);
					
			if(resultFSTOut1.NumStates() > 0 )
				printString(resultFSTOut1, resultado);
			else
				resultado = L""; 
							
			regla = L"9-SustPostFem";				
			return true; 
				
		}

		// Caso I
		if (s == L"I" || s == L"i"){
			
			// Palabras en Inglés
			wstring PalIngles = L"(if|all|when|what|how|as|that|here|can|am|but|before|do|everything|say|wish|where|things|now|will|for|who|because|have|should|something|love|want|don't|wanna|can't|know|feel|did|got|was|need|believe|like|just|could|get|gotta|think|hate|saw|dream|won't|see|die|never|kissed|still|call|miss|go|found|ain't|quit|would|heard|guess|thought|belong|knew|had|due|turn|hadn't|you|and|be|with)";
					
			// Expresion Regular para sustantivos anteriores
			boost::wregex PalInglesER (PalIngles, boost::regex_constants::icase|boost::regex_constants::perl);
				
			// Cadena a analizar = "I" y cualquiera de las 5 palabra anteriores o posteriores = palabra en inglés 
			t = false; 
			for(i=0; i < 10 && !t; i++)
				t = boost::regex_match (pal[i], PalInglesER, boost::regex_constants::format_perl);
			
			// Asumo que la "I" no se usa como nro romano. 
			if(t){
				resultado = L"";
				regla = L"I-PalabraInglesa";				
				return false; 
			
			}
						
			//palabra anterior = comillas asumo que no se usa como nro romano. Ej. "I forgot your number" 
			if(pal[4]==L"\""){
				resultado = L"";
				regla = L"I-Comillas";				
				return false; 
			}
			
					
			// Excluir si la palabra anterior es alguna de las siguientes: 
			//string excluirI = "(\+|-|x|\\|=)";
			
			wstring excluirI = L"(-|x|=|[\\+])";
			// Expresion Regular para excluir 
			boost::wregex excluirIER (excluirI, boost::regex_constants::icase|boost::regex_constants::perl);
			
			if ( boost::regex_match (pal[4], excluirIER, boost::regex_constants::format_perl)){
				resultado = L"";
				regla = L"I-MasMenosPor";				
				return false; 
			}
			
			// Excluir el caso i.e.
			if (pal[5]==L"." && (pal[6] == L"e" ||pal[6]==L"E")){
				resultado = L"";
				regla = L"I-ie";				
				return false; 
				
			}
		}
		
			// Caso II
		if (s == L"II"|| s == L"ii") {
			
			// Palabras inmediatas anteriores masculinas
			wstring palAntMascII= L"(de|del|vaticano)";
					
			// Expresion Regular para palabras masculinas anteriores al II 
			boost::wregex palAntMascIIER (palAntMascII, boost::regex_constants::icase|boost::regex_constants::perl);
								
			if ( boost::regex_match (pal[4], palAntMascIIER, boost::regex_constants::format_perl)){
				resultado = L"segundo"; 
				regla = L"II-PalAntMasc";				
				return true; 
			}
			
			// Palabras inmediatas anteriores femeninas
			wstring palAntFemII= L"(la)";
					
			// Expresion Regular para palabras masculinas anteriores al II 
			boost::wregex palAntFemIIER (palAntFemII, boost::regex_constants::icase|boost::regex_constants::perl);
								
			if ( boost::regex_match (pal[4], palAntFemIIER, boost::regex_constants::format_perl)){
				resultado = L"segunda"; 
				regla = L"II-PalAntFem";				
				return true; 
			}
			
		}
		
		// A esta altura no las considero nro romano 
		wstring DefaultNoExpandir = L"(M|mi|C|x|V|d|I|L|CC|MC|Xi|Mix|MD|DX|dl|XL|MV|Dix|mX|CX|CCC|Dv|LV|DCC|MMC|CMI|MDC|Mii|MCD|Div|MCC|mmm|LX|CCI|MCM|MCI|DVI|DCI|cmd|mdl|Viv|MMX|LXX|CMM|CLI|CMV|XLI|LLL|LIX|civ|CDL|XC|DCL|CDDL|LiI|DDL|LVI|DDC|vix|CLV|CDV|MDI|DCM|CLX|MMI|LLI|mll|Cii|mdd|DLX|MCV|MVV|MDX|DDI|IxC|CCL|DXi|CLIX|CCV|cvi|Cdx|lxv|Mixx|CML|VVV|DDD|cdcd|CLL|DCD|CVV|CxI|cxx|CIII|CDD|CXL|DCV|DLI|MLI|dxx|miv|Lixx|CCCI|Mixc|Cixx)";
			
		boost::wregex DefaultNoExpandirER (DefaultNoExpandir, boost::regex_constants::icase|boost::regex_constants::perl);
		
		if ( boost::regex_match (s, DefaultNoExpandirER, boost::regex_constants::format_perl)){
			resultado = L""; 
			regla = L"DefaultNoExpandir";				
			return false; 
		}
		
		// Lo que llega acá lo expando por default
		
		// Del 1 al 10 lo expando como ordinal, los mayores al 10 como cardinales. 		
		if (arabigo >= 1 && arabigo <= 10) {
			fst = fstRomanosOrdM; 
			str = to_wstring(arabigo); 
			testFST = string2fstOrdinal(0, 0, str); // Lo uso porque tengo problemas concatenando el °
		}else{ 
			fst = fstRomanosNat; 
			str = to_wstring(arabigo); 
			testFST = string2fst(0, 0, str); 
		}
		testFST.SetInputSymbols(isyms);
		testFST.SetOutputSymbols(isyms);

		Compose(testFST, *fst, &resultFST); 
					
		ShortestPath(resultFST, &resultFSTOut1, 1);
		
		
		if(resultFSTOut1.NumStates() > 0 )
			printString(resultFSTOut1, resultado);
		else
			resultado = L""; 
		
		regla = L"DefaultExpando";
				
		return true; 
	
	
	}
	
	resultado = L""; 
	regla = L"SinRegla"; 
	return t; 
}

// Determina si es una cadena es un nro romano válido, en caso afirmativo lo convierte a arábigo 
int RomanoTOArabigo(wstring n){
		
	int nbr[50]; 
	int resultado, i, j, cnt; 
	
	wstring s; 
	
	s = n; 
	// Convierto a mayúsculas 
	std::transform(s.begin(), s.end(), s.begin(), ::toupper);
	
	cnt = s.length();	
	
	// Valido que las letras sean I, V, X, L, C, D, M
    // Transformo las letras en un array de números equivalentes
	
	for (i = 0; i < cnt; i++){

		switch(s[i]){
			case L'I': nbr[i] = 1; break;
			case L'V': nbr[i] = 5; break; 
			case L'X': nbr[i] = 10; break;
			case L'L': nbr[i] = 50; break;
			case L'C': nbr[i] = 100; break;
			case L'D': nbr[i] = 500; break;
			case L'M': nbr[i] = 1000; break;
			default: return -1; // No es un nro romano vá¡lido 
		}
	}

	//valido que no e ( repita mas de 3 veces el mismo dígito
	for (i = 0; i < cnt; i++){
		if (i >= 4){
			if (nbr[i] == nbr[i-1] && nbr[i-1] == nbr[i-2] && nbr[i-2] == nbr[i-3])
				return -1; 
		}
	}
	
    // valido las regla de los valores que pueden preceder a otros
    for(i=1;i < cnt; i++){
		
		if (nbr[i-1] < nbr[i]){
			// I debe preceder a V o X sólamente
			// X debe preceder a L o C sólamente
			// C debe preceder a D o M sólamente
			if (!((nbr[i-1] == 1 && (nbr[i] == 5 || nbr[i] == 10)) || (nbr[i-1] == 10 && (nbr[i] == 50 || nbr[i] == 100)) || (nbr[i-1] == 100 && (nbr[i] == 500 || nbr[i] == 1000)))) {
                       return -1; 
			}else{
                //se resta 
                        nbr[i-1] = -1*nbr[i-1]; 
			}
			
		}
	}
	// Verifico que no haya cosas como CCD o CDC
	for(i=0; i < cnt; i++){
		if (nbr[i] < 0){
			for(j=0; j < cnt; j++){
				if (j != i && nbr[j] == nbr[i]*-1)
					return -1;
				
			}
			
		}
	}
	
	// Calculo el nro arabigo 
	resultado = 0; 
	for(i=0; i < cnt; i++)
		resultado += nbr[i]; 
	    
	//Valido que no haya cosas como LL
     if ((resultado == 1 || resultado == 5 || resultado == 10 || resultado == 50 || resultado == 100 || resultado == 500 || resultado == 1000) && cnt > 1) 
		return -1;
	 
	return resultado; 
	
}


void QuitaAcentos(wstring &a){
	
	std::size_t i; 
	
	for(i = 0; i < a.length(); i++){
		
		if (a[i] == L'á')
			a[i] = L'a';
			
		if (a[i] == L'é')
			a[i] = L'e';
			
		if (a[i] == L'í')
			a[i] = L'i';	

		if (a[i] == L'ó')
			a[i] = L'o';	
		
		if (a[i] == L'ú')
			a[i] = L'u';		
			
		if (a[i] == L'ñ')
			a[i] = L'n';		
	}
}



// Determina si es una expresión numerica y la expande en caso afirmativo. 
bool EsNro(wstring &s, wstring *pal, wstring &resultado, wstring &regla, StdFst *fstNat, StdFst *fstOrdF, 
				 StdFst *fstOrdM, StdFst *fstOrdN, StdFst *fstReales, StdFst *fstLetras, AvlTree<diccItem> *siglas, AvlTree<diccItem> *abrev, fst::SymbolTable *isyms){
					 
	StdVectorFst testFST, resultFST, resultFSTOut1,testFST2, resultFST2, resultFSTOut2; ; 
	StdFst *fst; 
	int arabigo, i, num1, num2; 
	wstring str, str2; 
	wstring resultado2, strAux; 
	bool esNegativo; 
	std::size_t pos, pos2; 
	std::size_t j; 


	// Expresion Regular general para expresiones que contienen dígitos 
	boost::wregex ER (L"([0-9]+)",boost::regex_constants::icase|boost::regex_constants::perl);
		
	bool t= false;

	if (!(boost::regex_search (s, ER, boost::regex_constants::format_perl))){	
		resultado = L"";
		regla = L"NoValido"; 
		return false; // No es una expresión numérica
	}
	
	// Es un número entero, sin puntos ni comas. Puede tener signo. Ej. -45, 23, +18
	boost::wregex naturalesER(L"^[[.plus-sign.][.hyphen.]]?[0-9]+$",boost::regex_constants::icase|boost::regex_constants::perl);

	if (boost::regex_match (s, naturalesER, boost::regex_constants::format_perl)){	
		
		esNegativo = (s[0]== L'-');
		
		// Quito el signo + si existe
		ReemplazarTexto(s, L"+", L""); 
		
		// Quito el signo - si existe
		ReemplazarTexto(s, L"-", L""); 
		
		fst = fstNat; 
		
		testFST = string2fst(0, 0, s); 
		
		testFST.SetInputSymbols(isyms);
		testFST.SetOutputSymbols(isyms);

		Compose(testFST, *fst, &resultFST); 
					
		ShortestPath(resultFST, &resultFSTOut1, 1);
				
		if(resultFSTOut1.NumStates() > 0 ){
			
			printString(resultFSTOut1, resultado);
			
			if (esNegativo)
				resultado = L"menos " + resultado; 
				
		}else{
			resultado = L""; 
		}
		
		regla = L"1-NumerosNaturales";		
		return true; 
	}
	// Es un número entero con puntos como separador de miles  Ej.: 10.000 ; -64.898.009
	boost::wregex naturales2ER(L"^[[.plus-sign.][.hyphen.]]?([1-9]){1}([0-9]){0,2}([.period.]([0-9]){3})+$",boost::regex_constants::icase|boost::regex_constants::perl);
	
	// La expresión esta compuesta por dígitos y puntos como separador de miles 
	if (boost::regex_match (s, naturales2ER, boost::regex_constants::format_perl)){	
		
		esNegativo = (s[0]== L'-');
		
		// Quito los puntos
		ReemplazarTexto(s, L".", L""); 
		
		// Quito el signo + si existe
		ReemplazarTexto(s, L"+", L""); 
		
		// Quito el signo - si existe
		ReemplazarTexto(s, L"-", L""); 
				
		fst = fstNat; 
		
		testFST = string2fst(0, 0, s); 
		
		testFST.SetInputSymbols(isyms);
		testFST.SetOutputSymbols(isyms);

		Compose(testFST, *fst, &resultFST); 
					
		ShortestPath(resultFST, &resultFSTOut1, 1);
				
		if(resultFSTOut1.NumStates() > 0 ){
			
			printString(resultFSTOut1, resultado);
			
			if (esNegativo)
				resultado = L"menos " + resultado; 
				
		}else{
			resultado = L""; 
		}
		
		regla = L"1-NumerosNaturales2";		
		return true; 
	}
	
	// Es un número entero con comas como separador de miles  Ej.: 10,000 ; -64,898
	boost::wregex naturales3ER(L"^[[.plus-sign.][.hyphen.]]?([1-9]){1}([0-9]){0,2}(,([0-9]){3})+$",boost::regex_constants::icase|boost::regex_constants::perl);
	
	// La expresión esta compuesta por dígitos y puntos como separador de miles 
	if (boost::regex_match (s, naturales3ER, boost::regex_constants::format_perl)){	
		
		esNegativo = (s[0]== L'-'); 
			
		// Quito las comas que se usan como separador de miles 
		ReemplazarTexto(s, L",", L""); 
		
		// Quito el signo + si existe
		ReemplazarTexto(s, L"+", L""); 
		
		// Quito el signo - si existe
		ReemplazarTexto(s, L"-", L""); 
				
		fst = fstNat; 
		
		testFST = string2fst(0, 0, s); 
		
		testFST.SetInputSymbols(isyms);
		testFST.SetOutputSymbols(isyms);

		Compose(testFST, *fst, &resultFST); 
					
		ShortestPath(resultFST, &resultFSTOut1, 1);
				
		if(resultFSTOut1.NumStates() > 0 ){
			
			printString(resultFSTOut1, resultado);
			
			if (esNegativo)
				resultado = L"menos " + resultado; 
				
		}else{
			resultado = L""; 
		}
		regla = L"1-NumerosNaturales3";		
		return true; 
	}
	
	// Es un número real con puntos (.) como separador de miles y coma como separador decimal. Ej.: 13.903,44 
	boost::wregex realesER(L"^[[.plus-sign.][.hyphen.]]?([1-9]){1}([0-9]){0,2}([.period.]([0-9]){3})+,[0-9]+$",boost::regex_constants::icase|boost::regex_constants::perl);
		
	if (boost::regex_match (s, realesER, boost::regex_constants::format_perl)){	
		
		// Quito los separadores de miles (.)
		ReemplazarTexto(s, L".", L""); 
				
		fst = fstReales; 
		
		testFST = string2fst(0, 0, s); 
		
		testFST.SetInputSymbols(isyms);
		testFST.SetOutputSymbols(isyms);

		Compose(testFST, *fst, &resultFST); 
					
		ShortestPath(resultFST, &resultFSTOut1, 1);
				
		if(resultFSTOut1.NumStates() > 0 )
			printString(resultFSTOut1, resultado);
		else
			resultado = L""; 
		
		regla = L"1-NumerosReales1";		
		return true; 
	}
	
	
	// Es un número real con comas como separador de miles y punto como separador decimal. Ej.: 13,903.44 
	boost::wregex reales2ER(L"^[[.plus-sign.][.hyphen.]]?([1-9]){1}([0-9]){0,2}(,([0-9]){3})+[.period.][0-9]+$",boost::regex_constants::icase|boost::regex_constants::perl);
		
	if (boost::regex_match (s, reales2ER, boost::regex_constants::format_perl)){	
		
		// Quito los separadores de miles (.)
		ReemplazarTexto(s, L",", L""); 
				
		fst = fstReales; 
		
		testFST = string2fst(0, 0, s); 
		
		testFST.SetInputSymbols(isyms);
		testFST.SetOutputSymbols(isyms);

		Compose(testFST, *fst, &resultFST); 
					
		ShortestPath(resultFST, &resultFSTOut1, 1);
				
		if(resultFSTOut1.NumStates() > 0 )
			printString(resultFSTOut1, resultado);
		else
			resultado = L""; 
		
		regla = L"1-NumerosReales2";		
		return true; 
	}
	
	
	// Es un número real con punto o coma como separador decimal. Ej.: 0.98, -34.56, 9,89 -0,78
	boost::wregex reales3ER(L"^[[.plus-sign.][.hyphen.]]?[0-9]+([.period.]|,)[0-9]+$",boost::regex_constants::icase|boost::regex_constants::perl);
		
	if (boost::regex_match (s, reales3ER, boost::regex_constants::format_perl)){	
		
					
		fst = fstReales; 
		
		testFST = string2fst(0, 0, s); 
		
		testFST.SetInputSymbols(isyms);
		testFST.SetOutputSymbols(isyms);

		Compose(testFST, *fst, &resultFST); 
					
		ShortestPath(resultFST, &resultFSTOut1, 1);
				
		if(resultFSTOut1.NumStates() > 0 )
			printString(resultFSTOut1, resultado);
		else
			resultado = L""; 
		
		regla = L"1-NumerosReales3";		
		return true; 
	}
	
	// Es un número en notación científica. Ej. 4,5x10<sup>-3</sup>
	boost::wregex NotCientificaER(L"^[[.plus-sign.][.hyphen.]]?[0-9]+([[.period.]|,][0-9]+)?x10<sup>[[.plus-sign.][.hyphen.]]?[0-9]+</sup>$",boost::regex_constants::icase|boost::regex_constants::perl);
		
	if (boost::regex_match (s, NotCientificaER, boost::regex_constants::format_perl)){	
		
		
		// Obtengo el nro real 
		pos = s.find_first_of(L"x");		
			
		if (pos==std::string::npos) 
			return false; // no debería pasar
			
		str = s.substr(0,pos);
		
				
		fst = fstReales; 
		
		testFST = string2fst(0, 0, str); 
		
		testFST.SetInputSymbols(isyms);
		testFST.SetOutputSymbols(isyms);

		Compose(testFST, *fst, &resultFST); 
					
		ShortestPath(resultFST, &resultFSTOut1, 1);
				
		if(resultFSTOut1.NumStates() > 0 ){
			printString(resultFSTOut1, resultado2);
			resultado = resultado2 + L" por diez a la "; 
		}else{
			resultado = L""; 
			return false; 
			
		}
		
		// Obtengo la potencia
		pos = s.find(L"<sup>");		
			
		if (pos==std::string::npos) 
			return false; // no debería pasar
		
		pos2 = s.find(L"</sup>");
			
		if (pos2==std::string::npos) 
			return false; // no debería pasar
		
		str = s.substr(pos+5,pos2-(pos+5));
		
				
		fst = fstReales; 
		
		testFST = string2fst(0, 0, str); 
		
		testFST.SetInputSymbols(isyms);
		testFST.SetOutputSymbols(isyms);

		Compose(testFST, *fst, &resultFST); 
					
		ShortestPath(resultFST, &resultFSTOut1, 1);
				
		if(resultFSTOut1.NumStates() > 0 ){
			printString(resultFSTOut1, resultado2);
			resultado = resultado + resultado2; 
			
		}else{
			resultado = L""; 
			return false; 
			
		}

		regla = L"NotacionCientifica";		
		return true; 
	}
	
	
	// Es un número con pontencia. Ej. 4<sup>2</sup>
	boost::wregex PotenciaER(L"^[[.plus-sign.][.hyphen.]]?[0-9]+([[.period.]|,][0-9]+)?<sup>[[.plus-sign.][.hyphen.]]?[0-9]+</sup>$",boost::regex_constants::icase|boost::regex_constants::perl);
		
	if (boost::regex_match (s, PotenciaER, boost::regex_constants::format_perl)){	
		
		
		// Obtengo el nro
		pos = s.find(L"<sup>");		
			
		if (pos==std::string::npos) 
			return false; // no debería pasar
			
		str = s.substr(0,pos);
		
				
		fst = fstReales; 
		
		testFST = string2fst(0, 0, str); 
		
		testFST.SetInputSymbols(isyms);
		testFST.SetOutputSymbols(isyms);

		Compose(testFST, *fst, &resultFST); 
					
		ShortestPath(resultFST, &resultFSTOut1, 1);
				
		if(resultFSTOut1.NumStates() > 0 ){
			printString(resultFSTOut1, resultado2);
			resultado = resultado2; 
		}else{
			resultado = L""; 
			return false; 
			
		}
		
		// Obtengo la potencia
		pos = s.find(L"<sup>");		
			
		if (pos==std::string::npos) 
			return false; // no debería pasar
		
		pos2 = s.find(L"</sup>");
			
		if (pos2==std::string::npos) 
			return false; // no debería pasar
		
		str = s.substr(pos+5,pos2-(pos+5));

		num1 = stoi(str); 
		// Expando con ordinal femenino para valores de 1 al 10 y el resto como cardinal 
		// Ej. 10 a la tercera, 10 a la veintidos 
		if (num1 > 0 && num1 <= 10 ){
			fst = fstOrdF;
			testFST = string2fstOrdinal(0, 0, str); 
				
			ReemplazarTexto(s, L"+", L""); 
			
		}else{
			fst = fstReales;
			testFST = string2fst(0, 0, str); 
		}
		
		
		
		testFST.SetInputSymbols(isyms);
		testFST.SetOutputSymbols(isyms);

		Compose(testFST, *fst, &resultFST); 
					
		ShortestPath(resultFST, &resultFSTOut1, 1);
				
		if(resultFSTOut1.NumStates() > 0 ){
			printString(resultFSTOut1, resultado2);
			
			if (num1 == 2){ // caso particular 
				resultado = resultado + L"al cuadrado ";
			}else if (num1 == 3){
				resultado = resultado + L"al cubo ";
			}else{
				resultado = resultado + L"elevado a la " + resultado2;
			}
						
		}else{
			resultado = L""; 
			return false; 
			
		}

		regla = L"Potencia";		
		return true; 
	}
	
	
	
	// La expresión es un período de años. Ejemplo: 1984-1986
	boost::wregex periodoER(L"^[0-9]{4}[-][0-9]{4}$",boost::regex_constants::icase|boost::regex_constants::perl);

	if (boost::regex_match (s, periodoER, boost::regex_constants::format_perl)){	
		
		fst = fstNat; 
				
		// Obtiene el primer número
		str = s.substr(0,4);
		
		// Obtiene el segundo número
		str2 = s.substr(5,4);
		
		//Expando el primer número 
	
		testFST = string2fst(0, 0, str); 
		
		testFST.SetInputSymbols(isyms);
		testFST.SetOutputSymbols(isyms);

		Compose(testFST, *fst, &resultFST); 
					
		ShortestPath(resultFST, &resultFSTOut1, 1);
				
		if(resultFSTOut1.NumStates() > 0 )
			printString(resultFSTOut1, resultado);
		else
			resultado = L""; 
			
		// Expando el segundo número 	

		testFST2 = string2fst(0, 0, str2); 
		
		testFST2.SetInputSymbols(isyms);
		testFST2.SetOutputSymbols(isyms);
		
		Compose(testFST2, *fst, &resultFST2); 
					
		ShortestPath(resultFST2, &resultFSTOut2, 1);
				
		if(resultFSTOut2.NumStates() > 0 )
			printString(resultFSTOut2, resultado2);
		else
			resultado2 = L""; 

		resultado += L" a " + resultado2; 
		regla = L"2-PeriodoAnios";		
		
		return true; 
	}

	// La expresión es ciclo de años. Ejemplo: 1984/1985
	boost::wregex ciclosER(L"^[0-9]{4}[/][0-9]{4}$",boost::regex_constants::icase|boost::regex_constants::perl);

	if (boost::regex_match (s, ciclosER, boost::regex_constants::format_perl)){	
		
		fst = fstNat; 
		
		// Obtiene el primer número
		str = s.substr(0,4);
		
		// Obtiene el segundo número
		str2 = s.substr(5,4);
		
		//Expando el primer número 
	
		testFST = string2fst(0, 0, str); 
		
		testFST.SetInputSymbols(isyms);
		testFST.SetOutputSymbols(isyms);

		Compose(testFST, *fst, &resultFST); 
					
		ShortestPath(resultFST, &resultFSTOut1, 1);
				
		if(resultFSTOut1.NumStates() > 0 )
			printString(resultFSTOut1, resultado);
		else
			resultado = L""; 
			
		// Expando el segundo número 	

		testFST2 = string2fst(0, 0, str2); 
		
		testFST2.SetInputSymbols(isyms);
		testFST2.SetOutputSymbols(isyms);
		
		Compose(testFST2, *fst, &resultFST2); 
					
		ShortestPath(resultFST2, &resultFSTOut2, 1);
				
		if(resultFSTOut2.NumStates() > 0 )
			printString(resultFSTOut2, resultado2);
		else
			resultado2 = L""; 

		resultado += L" " + resultado2; 
		regla = L"3-Ciclos";		
		
		return true; 
	}
	
	// La expresión es ciclo de años. Ejemplo: 1984/85
	boost::wregex ciclos2ER(L"^[0-9]{4}(/|\\-)[0-9]{2}$",boost::regex_constants::icase|boost::regex_constants::perl);

	if (boost::regex_match (s, ciclos2ER, boost::regex_constants::format_perl)){	
		
		fst = fstNat; 
		
		// Obtiene el primer número
		str = s.substr(0,4);
		
		// Obtiene el segundo número
		str2 = s.substr(5,2);
		
		//Convierte la decena del primer número a entero
		num1 =  boost::lexical_cast<int>(s.substr(2,2)); 
		
		
		//Convierte la decena del segundo número a entero
		num2 =  boost::lexical_cast<int>(str2); 
		
		// Veo si se trata de número siguiente o hay que incrementar la centena 
		// Ej. 1999/02 (num2 = 02, num1 = 99 
		if (num2 < num1 ) {
			// Incremento la centena 
			str2 = to_wstring(boost::lexical_cast<int>(s.substr(0,2))+1)+str2; 
		}else{
			str2 = to_wstring(boost::lexical_cast<int>(s.substr(0,2)))+str2; 
			
		}
					
		//Expando el primer número 
	
		testFST = string2fst(0, 0, str); 
		
		testFST.SetInputSymbols(isyms);
		testFST.SetOutputSymbols(isyms);

		Compose(testFST, *fst, &resultFST); 
					
		ShortestPath(resultFST, &resultFSTOut1, 1);
				
		if(resultFSTOut1.NumStates() > 0 )
			printString(resultFSTOut1, resultado);
		else
			resultado = L""; 
			
		// Expando el segundo número 	

		testFST2 = string2fst(0, 0, str2); 
		
		testFST2.SetInputSymbols(isyms);
		testFST2.SetOutputSymbols(isyms);
		
		Compose(testFST2, *fst, &resultFST2); 
					
		ShortestPath(resultFST2, &resultFSTOut2, 1);
				
		if(resultFSTOut2.NumStates() > 0 )
			printString(resultFSTOut2, resultado2);
		else
			resultado2 = L""; 

		resultado += L" " + resultado2; 
		regla = L"3-Ciclos";		
		
		return true; 

	}
	
		
	// La expresión de ordinales. Ejemplo: 8vo, 9.na, 7mo, 4ta., 2.ª 
	boost::wregex ordinalesER(L"^[0-9]+(er|ero|era|do|da|to|ta|vo|va|no|na|mo|ma|(\\.)?ª)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);

	if (boost::regex_match (s, ordinalesER, boost::regex_constants::format_perl)){	
			
		pos = 0;
		
		// Busca la posición del primer caracter que no sea dígito 
		for (j = 0; j < s.length(); j++){
			if( !(EsDigito(s[j])) ){
				pos = j;
				break;
			}
		}
		if (pos == 0)
			return false; // no debería pasar 
		
		// Obtiene el número
		str = s.substr(0,pos);
	
		// Obtiene la terminación: "na", "vo", "mo", etc 
		str2 = s.substr(pos); 
		
		//Determino si es femenino, masculino o neutro 
		
		boost::wregex ordinalesFemER(L"^(era|da|ta|va|na|ma|(\\.)?ª)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
		
		boost::wregex ordinalesMascER(L"^(ero|do|to|vo|no|mo)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
		
		
		if (boost::regex_match (str2, ordinalesFemER, boost::regex_constants::format_perl)){
			fst = fstOrdF; 
		}else if (boost::regex_match (str2, ordinalesMascER, boost::regex_constants::format_perl)){
			fst = fstOrdM; 
		}else{
			fst = fstOrdN; 
		}
			
		// Expando el nro
		testFST = string2fstOrdinal(0, 0, str); 
		testFST.SetInputSymbols(isyms);
		testFST.SetOutputSymbols(isyms);
		Compose(testFST, *fst, &resultFST); 
		ShortestPath(resultFST, &resultFSTOut1, 1);
				
		if(resultFSTOut1.NumStates() > 0 ){
			printString(resultFSTOut1, resultado);
			return true; 			
		}else{
			resultado = L""; 
			return false; 
		}
		
	}

	// Nros separados por - o / o :. Ejemplo: 56/1990  34,5-35,5, 3:4
	boost::wregex NrosSepER(L"^[0-9]+((\\.|,)[0-9]+)*(/|\\-|\\:)[0-9]+((\\.|,)[0-9]+)*$",boost::regex_constants::icase|boost::regex_constants::perl);

	if (boost::regex_match (s, NrosSepER, boost::regex_constants::format_perl)){	
		
		pos = 0;
		// Busca la posición del separador '-' o '/' o ':'
		for (j = 0; j < s.length(); j++){
			if( s[j] == L'-' || s[j]==L'/' || s[j]==L':' ){
				pos = j;
				break;
			}
		}
		
		if (pos == 0)
			return false; // no debería pasar 
		
		// Obtiene el número1
		str = s.substr(0,pos);
	
		// Obtiene el número2 
		str2 = s.substr(pos+1); 
		
		// Separador
		if (s[pos] == L'-'){
			strAux = L"guión "; 
		}else {
			if (s[pos] == L'/')
				strAux = L"barra ";
			else 
				strAux = L" ";
				
		}
				
				
			
		
		// Expando el número1
		if ( EsNro(str, pal, resultado, regla, fstNat, fstOrdF, fstOrdM, fstOrdN, fstReales, fstLetras, siglas, abrev, isyms)){
			
			// Expando el numero2
			if ( EsNro(str2, pal, resultado2, regla, fstNat, fstOrdF, fstOrdM, fstOrdN, fstReales, fstLetras, siglas, abrev, isyms)){
					resultado = resultado + strAux + resultado2; 
					return true; 
			}else{
				return false; 
			}
						
		}else{
			return false; 
		}
	}
	

	// Códigos Alfanuméricos (letras números). Ej.:  AS400 / UTF8/ A4

	boost::wregex CodigoAlfaNumER(L"^[a-z]+[0-9]+$",boost::regex_constants::icase|boost::regex_constants::perl);

	if (boost::regex_match (s, CodigoAlfaNumER, boost::regex_constants::format_perl)){	
		
		// Verifico si es una sigla. Por ejemplo: cm2
		if (EsSiglaOAbreviatura(s, siglas, abrev, resultado, fstLetras, isyms)){
			return true; 
		}

		pos = 0;
		
		// Busca la posición del primer caracter que sea dígito 
		for (j = 0; j < s.length(); j++){
			if( (EsDigito(s[j])) ){
				pos = j;
				break;
			}
		}
		if (pos == 0)
			return false; // no debería pasar 
		
		// Obtiene la parte alfabética
		str = s.substr(0,pos);
	
		// Obtiene la parte numérica 
		str2 = s.substr(pos); 
		
		//Evalúo si hay que deletrearla o queda como esta
		
		resultado = str; 
		// Convierte a mayúsculas
		std::transform(str.begin(), str.end(), str.begin(), ::toupper);
		
		if (str.length() < 2 || DeletrearSigla(str)){
			// Hay que deletrearla
			fst = fstLetras;
			testFST = string2fst(0, 0, str); 
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);
			Compose(testFST, *fst, &resultFST); 
			ShortestPath(resultFST, &resultFSTOut1, 1);
			
			if(resultFSTOut1.NumStates() > 0 ){
				// Cadena deletreada
				printString(resultFSTOut1, resultado);
			}else{
				// Si hubo error dejo la cadena como estaba 
				resultado = str; 
			}
		}

		// Expando el número
		if (str2[0] == L'0'){
			// Si comienza con cero se deletrean los dígitos 
			
			// Hay que deletrearla
			fst = fstLetras;
			testFST = string2fst(0, 0, str2); 
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);
			Compose(testFST, *fst, &resultFST); 
			ShortestPath(resultFST, &resultFSTOut1, 1);
			
			if(resultFSTOut1.NumStates() > 0 ){
				// Cadena deletreada
				printString(resultFSTOut1, resultado2);
				resultado = trim(resultado) + L' ' + trim(resultado2); 
				return true; 
				
			}else{
				// Si hubo error 
				return false; 
			}
			
		}else{
			// Si no comienza con cero se expande como número 
			if ( EsNro(str2, pal, resultado2, regla, fstNat, fstOrdF, fstOrdM, fstOrdN, fstReales, fstLetras, siglas, abrev, isyms)){
				resultado = trim(resultado) + L' ' + trim(resultado2); 
				return true; 
			}else{
				return false; 
			}
		
		}
	}
	
	// Códigos Alfanuméricos (letras separador números). Ej.:  AS-400 / UTF-8/ A_4 / R.3

	boost::wregex CodigoAlfaSepNumER(L"^[a-z]+(-|_|\\.)[0-9]+$",boost::regex_constants::icase|boost::regex_constants::perl);

	if (boost::regex_match (s, CodigoAlfaSepNumER, boost::regex_constants::format_perl)){	
		
		// Verifico si es una sigla.
		if (EsSiglaOAbreviatura(s, siglas, abrev, resultado, fstLetras, isyms)){
			return true; 
		}
		// Quito el separador
		ReemplazarTexto(s, L".", L""); 
		ReemplazarTexto(s, L"_", L""); 
		ReemplazarTexto(s, L"-", L""); 
		
		pos = 0;
		
		// Busca la posición del primer caracter que sea dígito 
		for (j = 0; j < s.length(); j++){
			if( (EsDigito(s[j])) ){
				pos = j;
				break;
			}
		}
		if (pos == 0)
			return false; // no debería pasar 
		
		// Obtiene la parte alfabética
		str = s.substr(0,pos);
	
		// Obtiene la parte numérica 
		str2 = s.substr(pos); 
		
		//Evalúo si hay que deletrearla o queda como esta
		
		resultado = str; 
		// Convierte a mayúsculas
		std::transform(str.begin(), str.end(), str.begin(), ::toupper);
		
		if (str.length() < 2 || DeletrearSigla(str)){
			// Hay que deletrearla
			fst = fstLetras;
			testFST = string2fst(0, 0, str); 
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);
			Compose(testFST, *fst, &resultFST); 
			ShortestPath(resultFST, &resultFSTOut1, 1);
			
			if(resultFSTOut1.NumStates() > 0 ){
				// Cadena deletreada
				printString(resultFSTOut1, resultado);
			}else{
				// Si hubo error dejo la cadena como estaba 
				resultado = str; 
			}
		}

		// Expando el número
		if (str2[0] == L'0'){
			// Si comienza con cero se deletrean los dígitos 
			
			// Hay que deletrearla
			fst = fstLetras;
			testFST = string2fst(0, 0, str2); 
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);
			Compose(testFST, *fst, &resultFST); 
			ShortestPath(resultFST, &resultFSTOut1, 1);
			
			if(resultFSTOut1.NumStates() > 0 ){
				// Cadena deletreada
				printString(resultFSTOut1, resultado2);
				resultado = trim(resultado) + L' ' + trim(resultado2); 
				return true; 
				
			}else{
				// Si hubo error 
				return false; 
			}
			
		}else{
			// Si no comienza con cero se expande como número 
			if ( EsNro(str2, pal, resultado2, regla, fstNat, fstOrdF, fstOrdM, fstOrdN, fstReales, fstLetras, siglas, abrev, isyms)){
				resultado = trim(resultado) + L' ' + trim(resultado2); 
				return true; 
			}else{
				return false; 
			}
		
		}
	}


	
	// Códigos Alfanuméricos (Numeros Letras) . Ej.:  3D /4x 

	boost::wregex CodigoAlfaNum2ER(L"^[0-9]+[a-z]+$",boost::regex_constants::icase|boost::regex_constants::perl);

	if (boost::regex_match (s, CodigoAlfaNum2ER, boost::regex_constants::format_perl)){	
		
		// Verifico  si es una sigla. Por ejemplo: cm2
		if (EsSiglaOAbreviatura(s, siglas, abrev, resultado, fstLetras, isyms)){
			return true; 
		}
	
		
		pos = 0;
		
		// Busca la posición del primer caracter que no sea dígito 
		for (j = 0; j < s.length(); j++){
			if(!(EsDigito(s[j])) ){
				pos = j;
				break;
			}
		}
		if (pos == 0)
			return false; // no debería pasar 
		
		// Obtiene la parte numérica
		str2 = s.substr(0,pos);
	
		// Obtiene la parte alfabética
		str = s.substr(pos); 
		
		//Evalúo si hay que deletrearla o queda como esta
		
		resultado2 = str; 
		
		// Convierte a mayúsculas
		std::transform(str.begin(), str.end(), str.begin(), ::toupper);
		
		if (str.length() < 2 || DeletrearSigla(str)){
			// Hay que deletrearla
			fst = fstLetras;
			testFST = string2fst(0, 0, str); 
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);
			Compose(testFST, *fst, &resultFST); 
			ShortestPath(resultFST, &resultFSTOut1, 1);
			
			if(resultFSTOut1.NumStates() > 0 ){
				// Cadena deletreada
				printString(resultFSTOut1, resultado2);
			}else{
				// Si hubo error dejo la cadena como estaba 
				resultado2 = str; 
			}
		}

		// Expando el número
		if (str2[0] == L'0'){
			// Si comienza con cero se deletrean los dígitos 
			
			// Hay que deletrearla
			fst = fstLetras;
			testFST = string2fst(0, 0, str2); 
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);
			Compose(testFST, *fst, &resultFST); 
			ShortestPath(resultFST, &resultFSTOut1, 1);
			
			if(resultFSTOut1.NumStates() > 0 ){
				// Cadena deletreada
				printString(resultFSTOut1, resultado);
				resultado = trim(resultado) + L' ' + trim(resultado2); 
				return true; 
				
			}else{
				// Si hubo error 
				return false; 
			}
			
		}else{
			// Si no comienza con cero se expande como número 
			if ( EsNro(str2, pal, resultado, regla, fstNat, fstOrdF, fstOrdM, fstOrdN, fstReales, fstLetras, siglas, abrev, isyms)){
				resultado = trim(resultado) + L' ' + trim(resultado2); 
				return true; 
			}else{
				return false; 
			}
		
		}
	}
	
	// Códigos Alfanuméricos (Numeros Separador Letras) . Ej.:  3-D /4_x  

	boost::wregex CodigoAlfaSepNum2ER(L"^[0-9]+(-|_|\\.)[a-z]+$",boost::regex_constants::icase|boost::regex_constants::perl);

	if (boost::regex_match (s, CodigoAlfaSepNum2ER, boost::regex_constants::format_perl)){	
		
		// Verifico  si es una sigla. 
		if (EsSiglaOAbreviatura(s, siglas, abrev, resultado, fstLetras, isyms)){
			return true; 
		}

		// Quito el separador
		ReemplazarTexto(s, L".", L""); 
		ReemplazarTexto(s, L"_", L""); 
		ReemplazarTexto(s, L"-", L""); 

		pos = 0;
		
		// Busca la posición del primer caracter que no sea dígito 
		for (j = 0; j < s.length(); j++){
			if(!(EsDigito(s[j])) ){
				pos = j;
				break;
			}
		}
		if (pos == 0)
			return false; // no debería pasar 
		
		// Obtiene la parte numérica
		str2 = s.substr(0,pos);
	
		// Obtiene la parte alfabética
		str = s.substr(pos); 
		
		//Evalúo si hay que deletrearla o queda como esta
		
		resultado2 = str; 
		
		// Convierte a mayúsculas
		std::transform(str.begin(), str.end(), str.begin(), ::toupper);
		
		if (str.length() < 2 || DeletrearSigla(str)){
			// Hay que deletrearla
			fst = fstLetras;
			testFST = string2fst(0, 0, str); 
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);
			Compose(testFST, *fst, &resultFST); 
			ShortestPath(resultFST, &resultFSTOut1, 1);
			
			if(resultFSTOut1.NumStates() > 0 ){
				// Cadena deletreada
				printString(resultFSTOut1, resultado2);
			}else{
				// Si hubo error dejo la cadena como estaba 
				resultado2 = str; 
			}
		}

		// Expando el número
		if (str2[0] == L'0'){
			// Si comienza con cero se deletrean los dígitos 
			
			// Hay que deletrearla
			fst = fstLetras;
			testFST = string2fst(0, 0, str2); 
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);
			Compose(testFST, *fst, &resultFST); 
			ShortestPath(resultFST, &resultFSTOut1, 1);
			
			if(resultFSTOut1.NumStates() > 0 ){
				// Cadena deletreada
				printString(resultFSTOut1, resultado);
				resultado = trim(resultado) + L' ' + trim(resultado2); 
				return true; 
				
			}else{
				// Si hubo error 
				return false; 
			}
			
		}else{
			// Si no comienza con cero se expande como número 
			if ( EsNro(str2, pal, resultado, regla, fstNat, fstOrdF, fstOrdM, fstOrdN, fstReales, fstLetras, siglas, abrev, isyms)){
				resultado = trim(resultado) + L' ' + trim(resultado2); 
				return true; 
			}else{
				return false; 
			}
		
		}
	}
	
	// Default - se expande deletreando las letras y dígitos 
	
	fst = fstLetras;
	testFST = string2fst(0, 0, s); 
	testFST.SetInputSymbols(isyms);
	testFST.SetOutputSymbols(isyms);
	Compose(testFST, *fst, &resultFST); 
	ShortestPath(resultFST, &resultFSTOut1, 1);
	
	if(resultFSTOut1.NumStates() > 0 ){
		// Cadena deletreada
		printString(resultFSTOut1, resultado);
		return true; 
		
	}else{
		// Si hubo error 
		return false; 
	}

	
	resultado = L""; 
	regla = L"SinRegla"; 
	return t; 
}


// Determina si es una fecha y la expande 
bool EsFecha(wstring &s, wstring *pal, wstring &resultado,  StdFst *fstFechas, fst::SymbolTable *isyms){

	StdVectorFst testFST, resultFST, resultFSTOut1,testFST2, resultFST2, resultFSTOut2; ; 
	StdFst *fst; 
			
	bool t= false;
	
	// Fecha formato DD-MM-AAAA
	boost::wregex fechas1ER(L"^((0?[1-9])|([1-2][0-9])|(3[0-1]))-((0?[1-9])|(1[0-2]))-(([0-9]{2})|([0-9]{4}))$",boost::regex_constants::icase|boost::regex_constants::perl);

	if (boost::regex_match (s, fechas1ER, boost::regex_constants::format_perl)){	
		
		fst = fstFechas; 

		testFST = string2fst(0, 0, s); 
		
		testFST.SetInputSymbols(isyms);
		testFST.SetOutputSymbols(isyms);

		Compose(testFST, *fst, &resultFST); 
					
		ShortestPath(resultFST, &resultFSTOut1, 1);
				
		if(resultFSTOut1.NumStates() > 0 ){
			printString(resultFSTOut1, resultado);
		}else{
			resultado = L""; 
		}
				
		return true; 
	}
	
	// Fecha formato DD/MM/AAAA
	boost::wregex fechas2ER(L"^((0?[1-9])|([1-2][0-9])|(3[0-1]))/((0?[1-9])|(1[0-2]))/(([0-9]{2})|([0-9]{4}))$",boost::regex_constants::icase|boost::regex_constants::perl);

	if (boost::regex_match (s, fechas2ER, boost::regex_constants::format_perl)){	
		
		fst = fstFechas; 

		testFST = string2fst(0, 0, s); 
		
		testFST.SetInputSymbols(isyms);
		testFST.SetOutputSymbols(isyms);

		Compose(testFST, *fst, &resultFST); 
					
		ShortestPath(resultFST, &resultFSTOut1, 1);
				
		if(resultFSTOut1.NumStates() > 0 ){
			printString(resultFSTOut1, resultado);
		}else{
			resultado = L""; 
		}
				
		return true; 
	}
	
	// Fecha formato DD.MM.AAAA
	boost::wregex fechas3ER(L"^((0?[1-9])|([1-2][0-9])|(3[0-1]))[.period.]((0?[1-9])|(1[0-2]))[.period.](([0-9]{2})|([0-9]{4}))$",boost::regex_constants::icase|boost::regex_constants::perl);

	if (boost::regex_match (s, fechas3ER, boost::regex_constants::format_perl)){	
		
		fst = fstFechas; 

		testFST = string2fst(0, 0, s); 
		
		testFST.SetInputSymbols(isyms);
		testFST.SetOutputSymbols(isyms);

		Compose(testFST, *fst, &resultFST); 
					
		ShortestPath(resultFST, &resultFSTOut1, 1);
				
		if(resultFSTOut1.NumStates() > 0 ){
			printString(resultFSTOut1, resultado);
		}else{
			resultado = L""; 
		}
				
		return true; 
	}
	
	resultado = L""; 
		
	return t; 
}



// Determina si es una Hora y la expande 
bool EsHora(wstring &s, wstring *pal, wstring &resultado,  StdFst *fstNat, fst::SymbolTable *isyms){

	StdVectorFst testFST, resultFST, resultFSTOut1,testFST2, resultFST2, resultFSTOut2; ; 
	StdFst *fst; 
	
	std::size_t pos, pos2; 
	wstring horas, minutos, segundos, resultAux; 
			
	bool t= false;
	
	// Fecha formato HH-MM-SS
	boost::wregex horasER(L"^(([0-1]?[0-9])|2[0-3]):[0-5][0-9](:[0-5][0-9])?$",boost::regex_constants::icase|boost::regex_constants::perl);

	if (boost::regex_match (s, horasER, boost::regex_constants::format_perl)){	
		
		// No pude hacer funcionar el fst de horas, lo hago con el de naturales 
		
		// Obtengo los grados 
		pos = s.find(L":");		
		
		if (pos==std::string::npos) 
				return false; // no debería pasar
		
		horas = s.substr(0,pos);
		
		if (horas == L"00")
			horas = L"0";
		
		// Expande las horas 	
		fst = fstNat; 
		testFST = string2fst(0, 0, horas); 
		testFST.SetInputSymbols(isyms);
		testFST.SetOutputSymbols(isyms);
		Compose(testFST, *fst, &resultFST); 
		ShortestPath(resultFST, &resultFSTOut1, 1);
				
		if(resultFSTOut1.NumStates() > 0 ){
			printString(resultFSTOut1, resultAux);
			resultado = resultAux + L"horas";
			
		}else{
			resultado = L""; 
			return false; 
		}
		
		// Obtengo los minutos y segundos
		pos2 = s.find(L":", pos+1);		
		
		if (pos2!=std::string::npos){
			// Tiene segundos 
			minutos = s.substr(pos+1, pos2-pos-1);
			segundos = s.substr(pos2+1);	
		}else{ 
			// no tiene segundos
			minutos = s.substr(pos+1);
			segundos = L"";	
		}
		
		if (minutos == L"00")
			minutos = L"0";
		
		if (segundos == L"00")
			segundos = L"0";	
		
		// Los minutos y segundos son 0, expando sólo horas. Ejemplo: 17:00hs queda "diecisiete horas" 
		if (minutos == L"0" && (segundos == L"" || segundos == L"0") )	
			return true; 
					
		// Expande los minutos
		fst = fstNat; 
		testFST = string2fst(0, 0, minutos); 
		testFST.SetInputSymbols(isyms);
		testFST.SetOutputSymbols(isyms);
		Compose(testFST, *fst, &resultFST); 
		ShortestPath(resultFST, &resultFSTOut1, 1);
				
		if(resultFSTOut1.NumStates() > 0 ){
			printString(resultFSTOut1, resultAux);
			resultado = resultado + L" " + resultAux + L"minutos";
			
		}else{
			resultado = L""; 
			return false; 
		}
		
		if (segundos != L""){
			
			if (segundos == L"00")
				segundos = L"0";
				
			// Expande los segundos
			fst = fstNat; // puede ser un número con punto o coma decimal
			testFST = string2fst(0, 0, segundos); 
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);
			Compose(testFST, *fst, &resultFST); 
			ShortestPath(resultFST, &resultFSTOut1, 1);
					
			if(resultFSTOut1.NumStates() > 0 ){
				printString(resultFSTOut1, resultAux);
				resultado = resultado + L" " + resultAux + L"segundos";
				return true; 
			}else{
				resultado = L""; 
				return false; 
			}
		
		}else{
			return true; 
		}

	}
		
	resultado = L""; 
		
	return t; 
}


// Determina si es una expresión de moneda 
// Ejemplo: $56.99 , U$S334 
bool EsMoneda(wstring &s, wstring *pal, wstring &resultado, wstring &regla,  int &CntPalabrasAvanzar, StdFst *fstNat, StdFst *fstOrdF, 
				 StdFst *fstOrdM, StdFst *fstOrdN, StdFst *fstReales, StdFst *fstLetras,  AvlTree<diccItem> *dicc, AvlTree<diccItem> *abrev, fst::SymbolTable *isyms){
	

	StdVectorFst testFST, resultFST, resultFSTOut1,testFST2, resultFST2, resultFSTOut2; ; 
	StdFst *fst; 
	int arabigo, i, num1, num2; 
	wstring str, str2; 
	wstring resultado2; 
	bool esNegativo; 
	std::size_t pos, pos2; 
	std::size_t j; 

	// Monedas - símbolo adelante. Ej.: $3.990 USD34,89 
	boost::wregex monedasER(L"^(\\$|€|£|¢|¥|U\\$S|US\\$|U\\$D|USD|AR\\$|ARS|AUD|CAD|BRL|CLP|EUR|JPY|MXN|GBP|CHF)([0-9]|[.period]|,)+(([.period.]|,)[0-9]+)?$",boost::regex_constants::icase|boost::regex_constants::perl);

	if (boost::regex_match (s, monedasER, boost::regex_constants::format_perl)){	
		
		// Obtengo el símbolo
		pos = 0;
		
		// Busca la posición del primer caracter que sea dígito 
		for (j = 0; j < s.length(); j++){
			if( (EsDigito(s[j])) ){
				pos = j;
				break;
			}
		}
	
		if (pos == 0)
			return false; // no debería pasar 
		
		// Obtiene el símbolo 
		str = s.substr(0,pos);
		
		// Obtiene el nro 
		str2 = s.substr(pos); 
		
		// Expando el nro
		if ( EsNro(str2, pal, resultado, regla, fstNat, fstOrdF, fstOrdM, fstOrdN, fstReales, fstLetras, dicc, abrev, isyms)){
						
			if (ExpandirMoneda(str, resultado2)){ 
				resultado = resultado2 + L' ' + resultado; 
				return true; 
			}
						
			return true; 
			
		}else{
			// No se pudo expandir el nro - no debería ocurrir
			return false; 
		}
		
	}
	
	// Monedas - símbolo atrás. Ej.: 3.990$ 34,89USD
 
	boost::wregex monedas2ER(L"^([0-9]|[.period]|,)+(([.period.]|,)[0-9]+)?(\\$|€|£|¢|¥|U\\$S|US\\$|U\\$D|USD|AR\\$|ARS|AUD|CAD|BRL|CLP|EUR|JPY|MXN|GBP|CHF)$",boost::regex_constants::icase|boost::regex_constants::perl);

	if (boost::regex_match (s, monedas2ER, boost::regex_constants::format_perl)){	
		
		// Obtengo el nro
		pos = 0;
		
		// Busca la posición del primer caracter que no sea dígito, punto o coma 
		for (j = 0; j < s.length(); j++){
			if( !(EsDigito(s[j]) || s[j] == L'.' || s[j] == L',') ){
				pos = j;
				break;
			}
		}
	
		if (pos == 0)
			return false; // no debería pasar 
		
		// Obtiene el nro 
		str2 = s.substr(0,pos);
		
		// Obtiene el simbolo 
		str = s.substr(pos); 
		
		// Expando el nro, llamo a esta misma función 
		if ( EsNro(str2, pal, resultado, regla, fstNat, fstOrdF, fstOrdM, fstOrdN, fstReales, fstLetras, dicc, abrev, isyms)){
						
			if (ExpandirMoneda(str, resultado2)){ 
				resultado = resultado2 + L' ' + resultado; 
				return true; 
			}
			return true; 
			
		}else{
			// No se pudo expandir el nro - no debería ocurrir
			return false; 
		}
		
	}
	
	
	// Evalúa si el símbolo de moneda y el nro estan en palabras distintas (separadas por espacio)

	//simbolos
	boost::wregex simbolomonedaER(L"^(\\$|€|£|¢|¥|U\\$S|US\\$|U\\$D|USD|AR\\$|ARS|AUD|CAD|BRL|CLP|EUR|JPY|MXN|GBP|CHF)$",boost::regex_constants::icase|boost::regex_constants::perl);
	
	// nro
	boost::wregex nroER(L"^([0-9]|[.period]|,)+(([.period.]|,)[0-9]+)?$",boost::regex_constants::icase|boost::regex_constants::perl);
  
	// Ver si la palabra actual es el simbolo de moneda y la siguiente es el nro 
	if (boost::regex_match (s, simbolomonedaER, boost::regex_constants::format_perl)
		&& boost::regex_match (pal[5], nroER, boost::regex_constants::format_perl) ){

		// Expando el nro
		if ( EsNro(pal[5], pal, resultado, regla, fstNat, fstOrdF, fstOrdM, fstOrdN, fstReales, fstLetras, dicc, abrev, isyms)){
						
			if (ExpandirMoneda(s, resultado2)){ 
				resultado = resultado2 + L' ' + resultado; 
				
				// Se expandió la palabra actual y la siguiente, hay que avanzar una palabra más 
				CntPalabrasAvanzar = 1; 
				
				return true; 
			}
				
			// Se expandió la palabra actual y la siguiente, hay que avanzar una palabra más 
			CntPalabrasAvanzar = 1; 
		
			return true; 
			
		}else{
			// No se pudo expandir el nro - no debería ocurrir
			return false; 
		}
	
	}
  
  
    // Ver si la palabra actual es el nro y la siguiente es el símbolo 
	if (boost::regex_match (pal[5], simbolomonedaER, boost::regex_constants::format_perl)
		&& boost::regex_match (s, nroER, boost::regex_constants::format_perl) ){

		// Expando el nro
		if ( EsNro(s, pal, resultado, regla, fstNat, fstOrdF, fstOrdM, fstOrdN, fstReales, fstLetras, dicc, abrev, isyms)){
						
			if (ExpandirMoneda(pal[5], resultado2)){ 
				resultado = resultado2 + L' ' + resultado; 
				
				// Se expandió la palabra actual y la siguiente, hay que avanzar una palabra más 
				CntPalabrasAvanzar = 1; 
				
				return true; 
			}
				
			// Se expandió la palabra actual y la siguiente, hay que avanzar una palabra más 
			CntPalabrasAvanzar = 1; 
		
			return true; 
			
		}else{
			// No se pudo expandir el nro - no debería ocurrir
			return false; 
		}
	
	}
   
	return false; 
}



// Determina si es una expresión con unidades de medida
// Ejemplo: 90kg, 189KWh, 23,4m2  

bool EsUnidadMedida(wstring &s, wstring *pal, wstring &resultado, wstring &regla,  int &CntPalabrasAvanzar, StdFst *fstNat, StdFst *fstOrdF, 
				 StdFst *fstOrdM, StdFst *fstOrdN, StdFst *fstReales, StdFst *fstLetras,  AvlTree<diccItem> *dicc, AvlTree<diccItem> *abrev, fst::SymbolTable *isyms){
				 
	StdVectorFst testFST, resultFST, resultFSTOut1,testFST2, resultFST2, resultFSTOut2; ; 
	StdFst *fst; 
	int arabigo, i, num1, num2; 
	wstring str, str2; 
	wstring resultado2; 
	bool esNegativo; 
	std::size_t pos, pos2; 
	std::size_t j; 
	
	// En este argumento se informa a la función llamadora cuántas palabras extra hay que avanzar dado que ya han sido normalizadas.
	CntPalabrasAvanzar = 0;


	// Expresion Regular general para expresiones que contienen dígitos 
	boost::wregex ER (L"([0-9]+)",boost::regex_constants::icase|boost::regex_constants::perl);
		
	if (!(boost::regex_search (s, ER, boost::regex_constants::format_perl))){	
		resultado = L"";
		return false; // No es una expresión numérica
	}

	// Expresion Regular para expresiones numéricas con unidades de medida en la misma palabra 
	boost::wregex unidadMedidaER (L"^[[.plus-sign.][.hyphen.]]?[0-9]+(([.period.]|,)[0-9]+)*(ac|a\\.c\\.|dc|d\\.c\\.|km|mts|m|M|cm|cms|mm|UA|mph|mp/h|kmh|km/h|m/s|mts/s|m/seg|mts/seg|m/segs|mts/segs|h|hs|min|mins|'|′|s|seg|segs|′′|''|″|”|\"|msnm|ft|km2|km²|mi2|mi²|ha|has|m2|m²|mts2|mts²|cm2|cm²cms2|cms²|mm2|mm²|m3|m³|mts3|mts³|cc|ccc|cm³|cm3|cms³|cms3|mm³|mm3|l|lts|lt|ml|m3/s|m³/s|tn|kg|kgs|grs|gr|mg|lb|lbs|hab|hab/km2|hab/km²|hab/mi2|hab/mi²|GPa|MPa|kPa|hPa|Pa|bar|mbar|PSI|mmHg|atm|Ghz|Mhz|khz|hz|FM|AM|RPM|hp|W|kW|MW|GW|Wh|W/h|kWh|kW/h|MWh|MW/h|GWh|GW/h|J|kJ|MJ|GJ|N|kN|MN|GN|kcal|cal|BTU|kDa|Da|A|mA|dB|²|³|Kb|Mb|Gb|Tb|Kbps|Mbps|Gbps|Mbit|pts)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);

	if (boost::regex_match (s, unidadMedidaER, boost::regex_constants::format_perl)){	
		
		// Obtengo el símbolo
		pos = 0;
		
		// Busca la posición del primer caracter que no sea dígito o punto o coma o signo: + o -   
		for (j = 0; j < s.length(); j++){
			if( !(EsDigito(s[j]) || s[j] == L'.' || s[j] == L',' || s[j] == L'+' || s[j] == L'-')  ){
				pos = j;
				break;
			}
		}
		
		if (pos == 0)
			return false; // no debería pasar 
		
		// Obtiene el nro 
		str2 = s.substr(0,pos);
		
		// Obtiene el símbolo 
		str = s.substr(pos); 
		
		// Expando el nro
		if ( EsNro(str2, pal, resultado, regla, fstNat, fstOrdF, fstOrdM, fstOrdN, fstReales, fstLetras, dicc, abrev, isyms)){
			
			if (ExpandirUnidadMedida(str, dicc, abrev, resultado2, fstLetras, isyms))
				resultado = resultado + resultado2; 
			
			return true; 
		}else{
			return false; 
		}
	}

	// Evalúa si el nro y el símbolo de medida estan en palabras distintas (separadas por espacio)  
	// Ejemplo: 120 km  10,3 m/s
	
	// Expresion Regular para expresiones numéricas 
	boost::wregex nroER (L"^[[.plus-sign.][.hyphen.]]?[0-9]+(([.period.]|,)[0-9]+)*$",boost::regex_constants::icase|boost::regex_constants::perl);
	
	// Expresion Regular para el símbolo de la unidade de medida (pal[5] primera posterior) 
	boost::wregex simboloER (L"^(ac|a\\.c\\.|dc|d\\.c\\.|km|mts|m|M|cm|cms|mm|UA|mph|mp/h|kmh|km/h|m/s|mts/s|m/seg|mts/seg|m/segs|mts/segs|h|hs|min|mins|'|′|s|seg|segs|′′|''|″|”|\"|msnm|ft|km2|km²|mi2|mi²|ha|has|m2|m²|mts2|mts²|cm2|cm²cms2|cms²|mm2|mm²|m3|m³|mts3|mts³|cc|ccc|cm³|cm3|cms³|cms3|mm³|mm3|l|lts|lt|ml|m3/s|m³/s|tn|kg|kgs|grs|gr|mg|lb|lbs|hab|hab/km2|hab/km²|hab/mi2|hab/mi²|GPa|MPa|kPa|hPa|Pa|bar|mbar|PSI|mmHg|atm|Ghz|Mhz|khz|hz|FM|AM|RPM|hp|W|kW|MW|GW|Wh|W/h|kWh|kW/h|MWh|MW/h|GWh|GW/h|J|kJ|MJ|GJ|N|kN|MN|GN|kcal|cal|BTU|kDa|Da|A|mA|dB|²|³|Kb|Mb|Gb|Tb|Kbps|Mbps|Gbps|Mbit|pts)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	
	if (boost::regex_match (s, nroER, boost::regex_constants::format_perl) && 
		boost::regex_match (pal[5], simboloER, boost::regex_constants::format_perl) ) {	
			
		// Expando el nro
		if ( EsNro(s, pal, resultado, regla, fstNat, fstOrdF, fstOrdM, fstOrdN, fstReales, fstLetras, dicc, abrev, isyms)){
			
			if (ExpandirUnidadMedida(pal[5], dicc, abrev, resultado2, fstLetras, isyms))
				resultado = resultado + resultado2; 
			
			// Se expandió la palabra actual y la siguiente, hay que avanzar una palabra más 
			CntPalabrasAvanzar = 1; 
			
			return true; 
			
		}else{
			return false; 
		}
	
	}

	
	// Evalúa si el nro es en notación científica
	// Ejemplo: 0.5x10<sup>-3</sup> kg
		
	// Es un número en notación científica. Ej. 4,5x10<sup>-3</sup>
	boost::wregex NotCientificaER(L"^[[.plus-sign.][.hyphen.]]?[0-9]+([[.period.]|,][0-9]+)?x10<sup>[[.plus-sign.][.hyphen.]]?[0-9]+</sup>$",boost::regex_constants::icase|boost::regex_constants::perl);
	
	if (boost::regex_match (s, NotCientificaER, boost::regex_constants::format_perl) && 
		boost::regex_match (pal[5], simboloER, boost::regex_constants::format_perl) ) {	
			
		// Expando el nro
		if ( EsNro(s, pal, resultado, regla, fstNat, fstOrdF, fstOrdM, fstOrdN, fstReales, fstLetras, dicc, abrev, isyms)){
			
			if (ExpandirUnidadMedida(pal[5], dicc, abrev, resultado2, fstLetras, isyms))
				resultado = resultado + resultado2; 
			
			// Se expandió la palabra actual y la siguiente, hay que avanzar una palabra más 
			CntPalabrasAvanzar = 1; 
			
			return true; 
			
		}else{
			return false; 
		}
	
	}

	return false; 
}



// Proceso las expresiones que tienen el símbolo de grado = ° o de orden º
// Puede ser : 
// temperaturas Ej. 20°C. 59°F 
// Coordenadas: 50°20'40"S
// Número ordinal: "el 2° aniversario..." 
// ángulos: 360°. 

bool ContieneGrados(wstring &s, wstring *pal, wstring &resultado,  StdFst *fstNat, StdFst *fstReales, StdFst *fstOrdinalesFem, StdFst *fstOrdinalesMasc, StdFst *fstOrdinalesNeutro, fst::SymbolTable *isyms){

	StdVectorFst testFST, resultFST, resultFSTOut1,testFST2, resultFST2, resultFSTOut2; ; 
	StdFst *fst; 
	wstring grados, minutos, segundos, nro; 		
	wstring resultAux; 
	bool t= false;
	std::size_t pos, pos2; 
	
	resultado = L""; 
	
	
	// Si la palabra no contiene el símbolo ° º no se continúa
	boost::wregex gradoER(L"(°|º)",boost::regex_constants::icase|boost::regex_constants::perl);
	
	if (not(boost::regex_search (s, gradoER, boost::regex_constants::format_perl)))
		return false; 
		
		
	// Ordinales. Ejemplos:  3.° 4.º o 2º
	boost::wregex ordinalER(L"^(([0-9]+(\\.)?º)|([0-9]+[.period.]°))$",boost::regex_constants::icase|boost::regex_constants::perl);

	if (boost::regex_match (s, ordinalER, boost::regex_constants::format_perl)){	
		
		boost::wregex palAntMascOrdinalER(L"^(el|del)$",boost::regex_constants::icase|boost::regex_constants::perl);
		boost::wregex palAntFemOrdinalER(L"^(la)$",boost::regex_constants::icase|boost::regex_constants::perl);
		
		
		// Si la palabra anterior es "el". Ejemplo: " el 2º regimiento ..."
		if (boost::regex_match (pal[4], palAntMascOrdinalER, boost::regex_constants::format_perl)){

			// Quito los puntos
			ReemplazarTexto(s, L".", L""); 
			
			// Obtengo el nro
			pos = s.find_first_of(L"º");		
			
			if (pos==std::string::npos) {
				
				pos = s.find_first_of(L"°");		
				
				if (pos==std::string::npos)
					return false; // no debería pasar
			}
			
			nro = s.substr(0,pos);
			
			// Expande el nro con el FST ordinales neutro
			fst = fstOrdinalesNeutro; 
			testFST = string2fstOrdinal(0, 0, nro); 
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);
			Compose(testFST, *fst, &resultFST); 
			ShortestPath(resultFST, &resultFSTOut1, 1);
					
			if(resultFSTOut1.NumStates() > 0 ){
				printString(resultFSTOut1, resultado);
				return true;
			}else{
				resultado = L""; 
				return false; 
			}
			
			// Si la palabra anterior es "la". Ejemplo: " la 2º campaña ..."
		}else if(boost::regex_match (pal[4], palAntFemOrdinalER, boost::regex_constants::format_perl)){
			
				// Quito los puntos
			ReemplazarTexto(s, L".", L""); 
			
			// Obtengo el nro
			pos = s.find_first_of(L"º");		
			
			if (pos==std::string::npos) {
				
				pos = s.find_first_of(L"°");		
				
				if (pos==std::string::npos)
					return false; // no debería pasar
			}
			
			nro = s.substr(0,pos);
			
			// Expande el nro con el FST ordinales femenino
			fst = fstOrdinalesFem; 
			testFST = string2fstOrdinal(0, 0, nro); 
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);
			Compose(testFST, *fst, &resultFST); 
			ShortestPath(resultFST, &resultFSTOut1, 1);
					
			if(resultFSTOut1.NumStates() > 0 ){
				printString(resultFSTOut1, resultado);
				return true;
			}else{
				resultado = L""; 
				return false; 
			}
		
		}else{ //Expando con ordinales neutro 
			
			// Quito los puntos
			ReemplazarTexto(s, L".", L""); 
			
			// Obtengo el nro
			pos = s.find_first_of(L"º");		
			
			if (pos==std::string::npos) {
				
				pos = s.find_first_of(L"°");		
				
				if (pos==std::string::npos)
					return false; // no debería pasar
			}
			
			nro = s.substr(0,pos);
			
			// Expande el nro con el FST ordinales Neutro
			fst = fstOrdinalesNeutro;
			testFST = string2fstOrdinal(0, 0, nro); 
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);
			Compose(testFST, *fst, &resultFST); 
			ShortestPath(resultFST, &resultFSTOut1, 1);
					
			if(resultFSTOut1.NumStates() > 0 ){
				printString(resultFSTOut1, resultado);
				return true;
			}else{
				resultado = L""; 
				return false; 
			}
		
			
		}
	}
	
	
	// Temperaturas: 29,4°C, 60.8°F 
	boost::wregex TemperaturasER(L"^[[.plus-sign.][.hyphen.]]?[0-9]+([[.period.],][0-9]+)?(°|º)(C|F)$",boost::regex_constants::icase|boost::regex_constants::perl);

	if (boost::regex_match (s, TemperaturasER, boost::regex_constants::format_perl)){	
		
		// Reemplazo º por ° para trabajar con un único símbolo de grado 
		ReemplazarTexto(s, L"º", L"°");
		
		// Obtengo los grados 
		pos = s.find(L"°");		
		
		if (pos==std::string::npos) 
				return false; // no debería pasar
		
		grados = s.substr(0,pos);
		
		// Expande los grados 	
		fst = fstReales; 
		testFST = string2fst(0, 0, grados); 
		testFST.SetInputSymbols(isyms);
		testFST.SetOutputSymbols(isyms);
		Compose(testFST, *fst, &resultFST); 
		ShortestPath(resultFST, &resultFSTOut1, 1);
				
		if(resultFSTOut1.NumStates() > 0 ){
			printString(resultFSTOut1, resultAux);
			resultado = resultAux + L"grados";
			return true;
		}else{
			resultado = L""; 
			return false; 
		}
		
		if (s.find(L"C")!=std::string::npos || s.find(L"c")!=std::string::npos){
			resultado = resultado + L" celcius"; 
			return true; 
		}
		
		if (s.find(L"F")!=std::string::npos || s.find(L"f")!=std::string::npos){
			resultado = resultado + L" farenjeit"; 
			return true; 
		}
		
		return true; 
	}
	
	
	// Temp/coordenadas: 29,4°, 34.9°, 23.4455° 
	// No es un ordinal porque tiene . o coma decimal 
	boost::wregex GradoSoloER(L"^[[.plus-sign.][.hyphen.]]?[0-9]+([[.period.],][0-9]+)(°|º)$",boost::regex_constants::icase|boost::regex_constants::perl);

	if (boost::regex_match (s, GradoSoloER, boost::regex_constants::format_perl)){	

		// Reemplazo º por ° para trabajar con un único símbolo de grado 
		ReemplazarTexto(s, L"º", L"°");
					
		// Obtengo los grados 
		pos = s.find_first_of(L"°");		
		
		if (pos==std::string::npos) 
				return false; // no debería pasar
		
		grados = s.substr(0,pos);
		
		// Expande los grados 	
		fst = fstReales; 
		testFST = string2fst(0, 0, grados); 
		testFST.SetInputSymbols(isyms);
		testFST.SetOutputSymbols(isyms);
		Compose(testFST, *fst, &resultFST); 
		ShortestPath(resultFST, &resultFSTOut1, 1);
				
		if(resultFSTOut1.NumStates() > 0 ){
			printString(resultFSTOut1, resultAux);
			resultado = resultAux + L"grados";
			return true;
		}else{
			resultado = L""; 
			return false; 
		}
		
		return true; 
	}

	
	// N°1, N°345
	boost::wregex NumeroER(L"^N(°|º)[0-9]+$",boost::regex_constants::icase|boost::regex_constants::perl);

	if (boost::regex_match (s, NumeroER, boost::regex_constants::format_perl)){	
				
		nro = s.substr(2, s.length()-2);
		
		// Expande el nro
		fst = fstNat; 
		testFST = string2fst(0, 0, nro); 
		testFST.SetInputSymbols(isyms);
		testFST.SetOutputSymbols(isyms);
		Compose(testFST, *fst, &resultFST); 
		ShortestPath(resultFST, &resultFSTOut1, 1);
				
		if(resultFSTOut1.NumStates() > 0 ){
			printString(resultFSTOut1, resultAux);
			resultado = L"número " + resultAux;
			return true;
		}else{
			resultado = L""; 
			return false; 
		}
				
		return true; 
	}
	
	
	// Coordenada en formato grados minutos [segundos] [N|S|E|O] Ej. 39°20'S , 48°40'34"S
	boost::wregex CoordenadasER(L"^[[.plus-sign.][.hyphen.]]?[0-9]+([[.period.]|,][0-9]+)?(°|º)[[.plus-sign.][.hyphen.]]?[0-9]+([[.period.]|,][0-9]+)?['|′|’]([[.plus-sign.][.hyphen.]]?[0-9]+([[.period.]|,][0-9]+)?([′′|''|″|”[.quotation-mark.]]))?(S|N|E|O|W)?$",boost::regex_constants::icase|boost::regex_constants::perl);

	if (boost::regex_match (s, CoordenadasER, boost::regex_constants::format_perl)){	
		
		// Reemplazo º por ° para trabajar con un único símbolo de grado 
		ReemplazarTexto(s, L"º", L"°");
	
		// Normalizo los caracteres
		ReemplazarTexto(s, L"”", L"\""); 
		ReemplazarTexto(s, L"''", L"\""); 
		ReemplazarTexto(s, L"′′", L"\""); 
		ReemplazarTexto(s, L"″", L"\""); 
		ReemplazarTexto(s, L"′", L"'"); 
		ReemplazarTexto(s, L"’", L"'"); 
				 
		// Obtengo los grados 
		pos = s.find_first_of(L"°");		
		
		if (pos==std::string::npos) 
				return false; // no debería pasar
		
		grados = s.substr(0,pos);
		
		// Expande los grados 	
		fst = fstReales; 
		testFST = string2fst(0, 0, grados); 
		testFST.SetInputSymbols(isyms);
		testFST.SetOutputSymbols(isyms);
		Compose(testFST, *fst, &resultFST); 
		ShortestPath(resultFST, &resultFSTOut1, 1);
				
		if(resultFSTOut1.NumStates() > 0 ){
			printString(resultFSTOut1, resultAux);
			resultado = resultAux + L"grados";
			
		}else{
			resultado = L""; 
			return false; 
		}
		
		// Obtengo los minutos 
		pos2 = s.find_first_of(L"'");		
		
		if (pos2==std::string::npos) {
				return false; // no debería pasar 
		}
				
		minutos = s.substr(pos+1, pos2-pos-1);
		
		// Expande los minutos
		fst = fstReales; 
		testFST = string2fst(0, 0, minutos); 
		testFST.SetInputSymbols(isyms);
		testFST.SetOutputSymbols(isyms);
		Compose(testFST, *fst, &resultFST); 
		ShortestPath(resultFST, &resultFSTOut1, 1);
				
		if(resultFSTOut1.NumStates() > 0 ){
			printString(resultFSTOut1, resultAux);
			resultado = resultado + L" " + resultAux + L"minutos";
			
		}else{
			resultado = L""; 
			return false; 
		}

		// Obtengo los segundos (si hay)
		pos = s.find_first_of(L"\"");		
		
		if (pos!=std::string::npos) {
			segundos = s.substr(pos2+1, pos-pos2-1);
			
			// Expande los segundos
			fst = fstReales; // puede ser un número con punto o coma decimal
			testFST = string2fst(0, 0, segundos); 
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);
			Compose(testFST, *fst, &resultFST); 
			ShortestPath(resultFST, &resultFSTOut1, 1);
					
			if(resultFSTOut1.NumStates() > 0 ){
				printString(resultFSTOut1, resultAux);
				resultado = resultado + L" " + resultAux + L"segundos";
				
			}else{
				resultado = L""; 
				return false; 
			}
		}
		
		// Expando N = norte, S = sur, E = este, O = oeste
		
		if (s.find(L"N")!=std::string::npos || s.find(L"n")!=std::string::npos){
			resultado = resultado + L" norte"; 
			return true; 
		}
		
		if (s.find(L"S")!=std::string::npos || s.find(L"s")!=std::string::npos){
			resultado = resultado + L" sur"; 
			return true; 
		}
		if (s.find(L"E")!=std::string::npos || s.find(L"e")!=std::string::npos){
			resultado = resultado + L" este"; 
			return true; 
		}
		
		if (s.find(L"O")!=std::string::npos || s.find(L"o")!=std::string::npos || s.find(L"W")!=std::string::npos || s.find(L"w")!=std::string::npos){
			resultado = resultado + L" oeste"; 
			return true; 
		}
		
		
		return true; 
	}
	
	
	// Coordenada sólo con grados e inicial de punto cardinal Ej. 39°S , 48°N
	boost::wregex Coordenadas2ER(L"^[[.plus-sign.][.hyphen.]]?[0-9]+([[.period.]|,][0-9]+)?(°|º)(S|N|E|O|W)$",boost::regex_constants::icase|boost::regex_constants::perl);

	if (boost::regex_match (s, Coordenadas2ER, boost::regex_constants::format_perl)){	
		
		// Reemplazo º por ° para trabajar con un único símbolo de grado 
		ReemplazarTexto(s, L"º", L"°");


		// Obtengo los grados 
		pos = s.find_first_of(L"°");		
		
		if (pos==std::string::npos) 
				return false; // no debería pasar
		
		grados = s.substr(0,pos);
		
		// Expande los grados 	
		fst = fstReales; 
		testFST = string2fst(0, 0, grados); 
		testFST.SetInputSymbols(isyms);
		testFST.SetOutputSymbols(isyms);
		Compose(testFST, *fst, &resultFST); 
		ShortestPath(resultFST, &resultFSTOut1, 1);
				
		if(resultFSTOut1.NumStates() > 0 ){
			printString(resultFSTOut1, resultAux);
			resultado = resultAux + L"grados";
			
		}else{
			resultado = L""; 
			return false; 
		}
		
		
		// Expando N = norte, S = sur, E = este, O = oeste
		
		if (s.find(L"N")!=std::string::npos || s.find(L"n")!=std::string::npos){
			resultado = resultado + L" norte"; 
			return true; 
		}
		
		if (s.find(L"S")!=std::string::npos || s.find(L"s")!=std::string::npos){
			resultado = resultado + L" sur"; 
			return true; 
		}
		if (s.find(L"E")!=std::string::npos || s.find(L"e")!=std::string::npos){
			resultado = resultado + L" este"; 
			return true; 
		}
		
		if (s.find(L"O")!=std::string::npos || s.find(L"o")!=std::string::npos || s.find(L"W")!=std::string::npos || s.find(L"w")!=std::string::npos){
			resultado = resultado + L" oeste"; 
			return true; 
		}
		
		
		return true; 
	}

	// Caso ambiguo: Ej.: 1°,  25°
	// Puede ser: primero, primer, primera / veinticinco grados, vigésimo quinto
	
	boost::wregex gralER(L"(^[0-9]+(°|º)$)",boost::regex_constants::icase|boost::regex_constants::perl);

	if (boost::regex_match (s, gralER, boost::regex_constants::format_perl)){	
		
		// Reemplazo º por ° para trabajar con un único símbolo de grado 
		ReemplazarTexto(s, L"º", L"°");
				
		boost::wregex palAntMascER(L"^(el|del)$",boost::regex_constants::icase|boost::regex_constants::perl);
		boost::wregex palAntFemER(L"^(la)$",boost::regex_constants::icase|boost::regex_constants::perl);
				
		// Si la palabra anterior es "el". Ejemplo: " el 2° regimiento ..."
		if (boost::regex_match (pal[4], palAntMascER, boost::regex_constants::format_perl)){

			// Obtengo el nro
			pos = s.find_first_of(L"°");		
			
			if (pos==std::string::npos) 
					return false; // no debería pasar
			
			nro = s.substr(0,pos);
			
			// Expande el nro con el FST ordinales neutro
			fst = fstOrdinalesNeutro; 
			testFST = string2fstOrdinal(0, 0, nro); 
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);
			Compose(testFST, *fst, &resultFST); 
			ShortestPath(resultFST, &resultFSTOut1, 1);
					
			if(resultFSTOut1.NumStates() > 0 ){
				printString(resultFSTOut1, resultado);
				return true;
			}else{
				resultado = L""; 
				return false; 
			}
			
			// Si la palabra anterior es "la". Ejemplo: " la 2° campaña ..."
		}else if(boost::regex_match (pal[4], palAntFemER, boost::regex_constants::format_perl)){
			
			// Obtengo el nro
			pos = s.find_first_of(L"°");		
			
			if (pos==std::string::npos) 
					return false; // no debería pasar
			
			nro = s.substr(0,pos);
			
			// Expande el nro con el FST ordinales femenino
			fst = fstOrdinalesFem; 
			testFST = string2fstOrdinal(0, 0, nro); 
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);
			Compose(testFST, *fst, &resultFST); 
			ShortestPath(resultFST, &resultFSTOut1, 1);
					
			if(resultFSTOut1.NumStates() > 0 ){
				printString(resultFSTOut1, resultado);
				return true;
			}else{
				resultado = L""; 
				return false; 
			}
		
		}
		

		boost::wregex palPostMascER(L"^(regimiento|ejercito|batallon|lugar|aniversario|escuadron|puesto|distrito|grado|dan|comando|festival|grupo|departamento|piso|dpto|dto)$",boost::regex_constants::icase|boost::regex_constants::perl);
					
		// Si la palabra posterior es alguna de las enumeradas. Ejemplo: " el 4° ejército ..."
		// Se expande como ordinal neutro 
		if (boost::regex_match (pal[5], palPostMascER, boost::regex_constants::format_perl)){

			// Obtengo el nro
			pos = s.find_first_of(L"°");		
			
			if (pos==std::string::npos) 
					return false; // no debería pasar
			
			nro = s.substr(0,pos);
			
			// Expande el nro con el FST ordinales neutro
			fst = fstOrdinalesNeutro; 
			testFST = string2fstOrdinal(0, 0, nro); 
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);
			Compose(testFST, *fst, &resultFST); 
			ShortestPath(resultFST, &resultFSTOut1, 1);
					
			if(resultFSTOut1.NumStates() > 0 ){
				printString(resultFSTOut1, resultado);
				return true;
			}else{
				resultado = L""; 
				return false; 
			}
		}
		
		boost::wregex palPostFemER(L"(escuadra|division|categoria)",boost::regex_constants::icase|boost::regex_constants::perl);	
		// Si la palabra posterior es alguna de las enumeradas. Ejemplo: " el 4° ejército ..."
		// Se expande como ordinal femenino
		if (boost::regex_match (pal[5], palPostFemER, boost::regex_constants::format_perl)){

			// Obtengo el nro
			pos = s.find_first_of(L"°");		
			
			if (pos==std::string::npos) 
					return false; // no debería pasar
			
			nro = s.substr(0,pos);
			
			// Expande el nro con el FST ordinales femenino
			fst = fstOrdinalesFem; 
			testFST = string2fstOrdinal(0, 0, nro); 
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);
			Compose(testFST, *fst, &resultFST); 
			ShortestPath(resultFST, &resultFSTOut1, 1);
					
			if(resultFSTOut1.NumStates() > 0 ){
				printString(resultFSTOut1, resultado);
				return true;
			}else{
				resultado = L""; 
				return false; 
			}
		}
		
		
		
		boost::wregex palPostTempCoordER(L"(^(norte|sur|este|oeste|west|n|n[.period.]|s|s[.period.]|e|e[.period.]|o|o[.period.]|w|w[.period.]|c|f|c[.period.]|f[.period.])$)",boost::regex_constants::icase|boost::regex_constants::perl);	
		boost::wregex palPostNroER(L"[0-9]+",boost::regex_constants::icase|boost::regex_constants::perl);	
		boost::wregex palCoordGradosER(L"(palarelos|paralelo|latitud|ecuador|gira|girar|longitud|meridiano|lat|coordenadas|sur|norte|este|oeste|angulo|inclinacion|temperatura|temp|temp[.period.])",boost::regex_constants::icase|boost::regex_constants::perl);
		
		
		// Si la palabra posterior es alguna de las enumeradas o un número se asume que es una coordenada o temperatura
		// o algunas de las de contexto refiere a coordenadas o grados
		if (boost::regex_match (pal[5], palPostTempCoordER, boost::regex_constants::format_perl)
			|| boost::regex_search (pal[5], palPostNroER, boost::regex_constants::format_perl)
			|| boost::regex_search (pal[0], palCoordGradosER, boost::regex_constants::format_perl)
			|| boost::regex_search (pal[1], palCoordGradosER, boost::regex_constants::format_perl)
			|| boost::regex_search (pal[2], palCoordGradosER, boost::regex_constants::format_perl)
			|| boost::regex_search (pal[3], palCoordGradosER, boost::regex_constants::format_perl)
			|| boost::regex_search (pal[4], palCoordGradosER, boost::regex_constants::format_perl)
			|| boost::regex_search (pal[5], palCoordGradosER, boost::regex_constants::format_perl)
			|| boost::regex_search (pal[6], palCoordGradosER, boost::regex_constants::format_perl)
			|| boost::regex_search (pal[7], palCoordGradosER, boost::regex_constants::format_perl)
			|| boost::regex_search (pal[8], palCoordGradosER, boost::regex_constants::format_perl)
			|| boost::regex_search (pal[9], palCoordGradosER, boost::regex_constants::format_perl)
			){

			// Obtengo el nro
			pos = s.find_first_of(L"°");		
			
			if (pos==std::string::npos) 
					return false; // no debería pasar
			
			nro = s.substr(0,pos);
			
			// Expande el nro con el FST Naturales
			fst = fstNat; 
			testFST = string2fst(0, 0, nro); 
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);
			Compose(testFST, *fst, &resultFST); 
			ShortestPath(resultFST, &resultFSTOut1, 1);
					
			if(resultFSTOut1.NumStates() > 0 ){
				printString(resultFSTOut1, resultAux);
				resultado = resultAux + L"grados";
				return true;
								
			}else{
				resultado = L""; 
				return false; 
			}
		}				
		

		boost::wregex palAntPostOrdenNeutroER(L"(articulo|teniente|regimientos|regimiento|cumpleaños|tomo|humberto|oficial|capitulo|Art[.period.]|cuartel|capitan|subteniente|soldado|batallon)",boost::regex_constants::icase|boost::regex_constants::perl);
	
		// Si la palabra Anterior es alguna de las enumeradas. Se asume que es un número ordinal. Ej. "artículo 4°"
		if (boost::regex_match (pal[4], palAntPostOrdenNeutroER, boost::regex_constants::format_perl)||
			boost::regex_match (pal[5], palAntPostOrdenNeutroER, boost::regex_constants::format_perl)){

			// Obtengo el nro
			pos = s.find_first_of(L"°");		
			
			if (pos==std::string::npos) 
					return false; // no debería pasar
			
			nro = s.substr(0,pos);
			
			// Expande el nro con el FST ordinales neutro
			fst = fstOrdinalesNeutro; 
			testFST = string2fstOrdinal(0, 0, nro); 
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);
			Compose(testFST, *fst, &resultFST); 
			ShortestPath(resultFST, &resultFSTOut1, 1);
					
			if(resultFSTOut1.NumStates() > 0 ){
				printString(resultFSTOut1, resultado);
				return true;
								
			}else{
				resultado = L""; 
				return false; 
			}
		}				
		
		boost::wregex palAntPostOrdenFemER(L"(edicion|comisaria|posicion|seccion|version)",boost::regex_constants::icase|boost::regex_constants::perl);		
		
		// Si la palabra Anterior es alguna de las enumeradas. Se asume que es un número ordinal. Ej. "edicion 3°"
		if (boost::regex_match (pal[4], palAntPostOrdenFemER, boost::regex_constants::format_perl) ||
			boost::regex_match (pal[5], palAntPostOrdenFemER, boost::regex_constants::format_perl)){

			// Obtengo el nro
			pos = s.find_first_of(L"°");		
			
			if (pos==std::string::npos) 
					return false; // no debería pasar
			
			nro = s.substr(0,pos);
			
			// Expande el nro con el FST Naturales
			fst = fstOrdinalesFem; 
			testFST = string2fstOrdinal(0, 0, nro); 
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);
			Compose(testFST, *fst, &resultFST); 
			ShortestPath(resultFST, &resultFSTOut1, 1);
					
			if(resultFSTOut1.NumStates() > 0 ){
				printString(resultFSTOut1, resultado);
				return true;
								
			}else{
				resultado = L""; 
				return false; 
			}
		}

		// Angulos comunes
		boost::wregex angulosER(L"(180°|90°|360°|45°|30°|120°|270°)",boost::regex_constants::icase|boost::regex_constants::perl);

		if (boost::regex_match (s, angulosER, boost::regex_constants::format_perl)){	
						
			// Obtengo los grados 
			pos = s.find_first_of(L"°");		
			
			if (pos==std::string::npos) 
					return false; // no debería pasar
			
			grados = s.substr(0,pos);
			
			// Expande los grados 	
			fst = fstNat; 
			testFST = string2fst(0, 0, grados); 
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);
			Compose(testFST, *fst, &resultFST); 
			ShortestPath(resultFST, &resultFSTOut1, 1);
					
			if(resultFSTOut1.NumStates() > 0 ){
				printString(resultFSTOut1, resultAux);
				resultado = resultAux + L"grados";
				return true;
			}else{
				resultado = L""; 
				return false; 
			}
		}

		// Regla por defecto 
		// Si es menor o igual a 20° asumo que es un ordinal neutro
		// Si es mayor a 20° es una expresión de temp o coordenadas. 
						
		// Obtengo el nro
		pos = s.find_first_of(L"°");		
			
		if (pos==std::string::npos) 
			return false; // no debería pasar
			
		grados = s.substr(0,pos);
	
		if (stoi(grados) <=  20 ){ 
			
			// Expande el nro con el FST ordinales neutro
			fst = fstOrdinalesNeutro; 
			testFST = string2fstOrdinal(0, 0, grados); 
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);
			Compose(testFST, *fst, &resultFST); 
			ShortestPath(resultFST, &resultFSTOut1, 1);
					
			if(resultFSTOut1.NumStates() > 0 ){
				printString(resultFSTOut1, resultado);
				return true;
								
			}else{
				resultado = L""; 
				return false; 
			}
		
			
		}else{	
			fst = fstNat; 
			testFST = string2fst(0, 0, grados); 
			
			// Expande los grados 	
			fst = fstNat; 
			testFST = string2fst(0, 0, grados); 
			testFST.SetInputSymbols(isyms);
			testFST.SetOutputSymbols(isyms);
			Compose(testFST, *fst, &resultFST); 
			ShortestPath(resultFST, &resultFSTOut1, 1);
					
			if(resultFSTOut1.NumStates() > 0 ){
				printString(resultFSTOut1, resultAux);
				resultado = resultAux + L"grados";
				return true; 
			}else{
				resultado = L""; 
				return false; 
			}
		}
			
		return false; 
	}
			
	resultado = L""; 
		
	return t; 
}




bool DeletrearSigla(wstring &s){
	
	int i; 
	int largo = s.length(); 
	bool sePronuncia; 
	bool cond;
	
	// Si la sigla esta compuesta por vocales únicamente, entonces se pronuncia
	boost::wregex vocalesER(L"^[AEIOU]$",boost::regex_constants::perl);
	
	if (boost::regex_match (s, vocalesER, boost::regex_constants::format_perl))
		return false; 
	
	
	//Si la sigla es de dos letras, casi siempre se deletrea 
//	if (largo <= 2) 
//		return true; 

	//Si la sigla tiene mas de 4 letras, se suele pronunciar 	
	if (largo > 4)
		return false; 
	
	// Si para cada consonante existe una vocal antes o después, entonces se pronuncia. 
	sePronuncia = true;
	
	for(i = 0; i < largo && sePronuncia; i++ ){
	
		if (!esVocal(s[i])){

			cond = false; 	
			
			if( i > 0 )
				cond |= esVocal(s[i-1]); 
			
			if(i <largo-1)
				cond |= esVocal(s[i+1]);
		}else{
			cond = true; 
		}

		sePronuncia &= cond;
	}
		
	return !sePronuncia; 	
}

bool esVocal(char c){
	c = toupper(c);
	
	return (c == L'A' || c == L'E' ||c == L'I' ||c == L'O' ||c == L'U'); 
}

// Expande los símbolos de unidades de medida
// Ejemplo: kg --> kilogramos , m2 ---> metros cuadrados 
bool ExpandirUnidadMedida(wstring &str, AvlTree<diccItem> *dicc, AvlTree<diccItem> *abrev, wstring &resultado, StdFst *fstLetras, fst::SymbolTable *isyms){
	
	// a.c. 
	boost::wregex acER(L"^(ac|a\\.c\\.)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, acER, boost::regex_constants::format_perl)){
		resultado = L"antes de cristo";
		return true; 
	}

	// d.c. 
	boost::wregex dcER(L"^(dc|d\\.c\\.)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, dcER, boost::regex_constants::format_perl)){
		resultado = L"después de cristo";
		return true; 
	}

	// Kilómetros
	boost::wregex kmER(L"^(km)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, kmER, boost::regex_constants::format_perl)){
		resultado = L"kilómetros";
		return true; 
	}
	
	// Metros
	boost::wregex metrosER(L"^(mts)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, metrosER, boost::regex_constants::format_perl)){
		resultado = L"metros";
		return true; 
	}
	
	// m minúscula asumo que es metros también
	if (str == L"m" || str == L"m."){
		resultado = L"metros";
		return true; 
	}
	
	// Centímetros
	boost::wregex centimetrosER(L"^(cm|cms)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, centimetrosER, boost::regex_constants::format_perl)){
		resultado = L"centímetros";
		return true; 
	}
	
	// milímetros
	if (str == L"mm" || str == L"mm."){
		resultado = L"milímetros";
		return true; 
	}
	
	// UA
	if (str == L"UA"){
		resultado = L"unidades astronómicas";
		return true; 
	}

	// mph
	boost::wregex mphER(L"^(mph|mp/h)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, mphER, boost::regex_constants::format_perl)){
		resultado = L"millas por hora";
		return true; 
	}

	// kmh
	boost::wregex kmhER(L"^(kmh|km/h)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, kmhER, boost::regex_constants::format_perl)){
		resultado = L"kilómetros por hora";
		return true; 
	}

	// m/s
	boost::wregex mpsER(L"^(m/s|mts/s|m/seg|mts/seg|m/segs|mts/segs)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, mpsER, boost::regex_constants::format_perl)){
		resultado = L"metros por segundo";
		return true; 
	}

	// horas
	boost::wregex horasER(L"^(h|hs)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, horasER, boost::regex_constants::format_perl)){
		resultado = L"horas";
		return true; 
	}


	// minutos
	boost::wregex minER(L"^(min|mins|'|′)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, minER, boost::regex_constants::format_perl)){
		resultado = L"minutos";
		return true; 
	}

	// segundos
	boost::wregex segER(L"^(s|seg|segs|′′|''|″|”|\")(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, segER, boost::regex_constants::format_perl)){
		resultado = L"segundos";
		return true; 
	}
	
	// msnm
	boost::wregex msnmER(L"^(msnm)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, msnmER, boost::regex_constants::format_perl)){
		resultado = L"metros sobre el nivel del mar";
		return true; 
	}
		
	// ft
	boost::wregex ftER(L"^(ft)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, ftER, boost::regex_constants::format_perl)){
		resultado = L"pies";
		return true; 
	}
	
		// km2
	boost::wregex km2ER(L"^(km2|km²)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, km2ER, boost::regex_constants::format_perl)){
		resultado = L"kilómetros cuadrados";
		return true; 
	}
	
	// mi2
	boost::wregex mi2ER(L"^(mi2|mi²)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, mi2ER, boost::regex_constants::format_perl)){
		resultado = L"millas cuadradas";
		return true; 
	}
	
	// hectáreas 
	boost::wregex hectareasER(L"^(ha|has)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, hectareasER, boost::regex_constants::format_perl)){
		resultado = L"hectáreas";
		return true; 
	}
		
	// Metros cuadrados
	boost::wregex metros2ER(L"^(m2|m²|mts2|mts²)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, metros2ER, boost::regex_constants::format_perl)){
		resultado = L"metros cuadrados";
		return true; 
	}
	
	// Centímetros cuadrados 
	boost::wregex centimetros2ER(L"^(cm2|cm²|cms2|cms²)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, centimetros2ER, boost::regex_constants::format_perl)){
		resultado = L"centímetros cuadrados";
		return true; 
	}
	
	// milímetros cuadrados 
	boost::wregex milimetros2ER(L"^(mm2|mm²)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, milimetros2ER, boost::regex_constants::format_perl)){
		resultado = L"milímetros cuadrados";
		return true; 
	}
		
	// Metro cúbico
	boost::wregex metros3ER(L"^(m3|m³|mts3|mts³)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, metros3ER, boost::regex_constants::format_perl)){
		resultado = L"metros cúbicos";
		return true; 
	}
	
	// Centímetros cúbicos
	boost::wregex centimetros3ER(L"^(cc|ccc|cm³|cm3|cms³|cms3)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, centimetros3ER, boost::regex_constants::format_perl)){
		resultado = L"centímetros cúbicos";
		return true; 
	}
	
	// Milímetros cúbicos
	boost::wregex milimetros3ER(L"^(mm³|mm3)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, milimetros3ER, boost::regex_constants::format_perl)){
		resultado = L"milímetros cúbicos";
		return true; 
	}
	

	// litros
	boost::wregex litrosER(L"^(l|lts|lt)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, litrosER, boost::regex_constants::format_perl)){
		resultado = L"litros";
		return true; 
	}	
	// mililitros
	boost::wregex mililitrosER(L"^(ml)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, mililitrosER, boost::regex_constants::format_perl)){
		resultado = L"mililitros";
		return true; 
	}
	
	// m3/s
	boost::wregex M3SER(L"^(m3/s|m³/s)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, M3SER, boost::regex_constants::format_perl)){
		resultado = L"metros cúbicos por segundo";
		return true; 
	}
	
	// tn
	boost::wregex tnER(L"^(tn)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, tnER, boost::regex_constants::format_perl)){
		resultado = L"toneladas";
		return true; 
	}	
			
	// Kilogramos
	boost::wregex kgER(L"^(kg|kgs)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, kgER, boost::regex_constants::format_perl)){
		resultado = L"kilogramos";
		return true; 
	}
	
	// gramos
	boost::wregex gramosER(L"^(grs|gr)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, gramosER, boost::regex_constants::format_perl)){
		resultado = L"gramos";
		return true; 
	}	
	
	// miligramos
	boost::wregex miligramosER(L"^(mg)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, miligramosER, boost::regex_constants::format_perl)){
		resultado = L"miligramos";
		return true; 
	}
	
	// lb
	boost::wregex lbER(L"^(lb|lbs)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, lbER, boost::regex_constants::format_perl)){
		resultado = L"libras";
		return true; 
	}
	
	// habitantes
	boost::wregex habitantesER(L"^(hab)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, habitantesER, boost::regex_constants::format_perl)){
		resultado = L"habitantes";
		return true; 
	}
	
	// Hab/km2
	boost::wregex HabKm2ER(L"^(hab/km2|hab/km²)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, HabKm2ER, boost::regex_constants::format_perl)){
		resultado = L"habitantes por kilómetro cuadrado";
		return true; 
	}

	// Hab/mi2
	boost::wregex HabMi2ER(L"^(hab/mi2|hab/m²)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, HabMi2ER, boost::regex_constants::format_perl)){
		resultado = L"habitantes por milla cuadrada";
		return true; 
	}

	// GPa
	boost::wregex GPaER(L"^(GPa)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, GPaER, boost::regex_constants::format_perl)){
		resultado = L"yiga pascales";
		return true; 
	}
	
	
	// MPa
	boost::wregex MPaER(L"^(MPa)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, MPaER, boost::regex_constants::format_perl)){
		resultado = L"megapascales";
		return true; 
	}

	// kPa
	boost::wregex kPaER(L"^(kPa)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, kPaER, boost::regex_constants::format_perl)){
		resultado = L"kilopascales";
		return true; 
	}

	// hPa
	boost::wregex hPaER(L"^(hPa)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, hPaER, boost::regex_constants::format_perl)){
		resultado = L"hectopascales";
		return true; 
	}

	// Pa
	boost::wregex PaER(L"^(Pa)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, PaER, boost::regex_constants::format_perl)){
		resultado = L"pascales";
		return true; 
	}
	
	// bar
	boost::wregex barER(L"^(bar)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, barER, boost::regex_constants::format_perl)){
		resultado = L"bares";
		return true; 
	}
	
	// mbar
	boost::wregex mbarER(L"^(mbar)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, mbarER, boost::regex_constants::format_perl)){
		resultado = L"milibares";
		return true; 
	}
	
	// PSI
	boost::wregex PSIER(L"^(PSI)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, PSIER, boost::regex_constants::format_perl)){
		resultado = L"pe ese i";
		return true; 
	}
	
	// mmHg
	boost::wregex mmHgER(L"^(mmHg)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, mmHgER, boost::regex_constants::format_perl)){
		resultado = L"milímetros de mercurio";
		return true; 
	}
	
	// atm
	boost::wregex atmER(L"^(atm)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, atmER, boost::regex_constants::format_perl)){
		resultado = L"atmósferas";
		return true; 
	}
	
	
	// Ghz
	boost::wregex GhzER(L"^(Ghz)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, GhzER, boost::regex_constants::format_perl)){
		resultado = L"yigajertz";
		return true; 
	}


	// Mhz
	boost::wregex MhzER(L"^(Mhz)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, MhzER, boost::regex_constants::format_perl)){
		resultado = L"megajertz";
		return true; 
	}
	
	// Khz
	boost::wregex KhzER(L"^(khz)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, KhzER, boost::regex_constants::format_perl)){
		resultado = L"kilojertz";
		return true; 
	}
		
	// hz
	boost::wregex hzER(L"^(hz)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, hzER, boost::regex_constants::format_perl)){
		resultado = L"jertz";
		return true; 
	}
	
	// FM
	boost::wregex FMER(L"^(FM)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, FMER, boost::regex_constants::format_perl)){
		resultado = L"efe eme";
		return true; 
	}
	
	// AM
	boost::wregex AMER(L"^(AM)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, AMER, boost::regex_constants::format_perl)){
		resultado = L"a eme";
		return true; 
	}
	
	// RPM
	boost::wregex RPMER(L"^(RPM)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, RPMER, boost::regex_constants::format_perl)){
		resultado = L"revoluciones por minuto";
		return true; 
	}
	
	
	// hp
	boost::wregex hpER(L"^(hp)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, hpER, boost::regex_constants::format_perl)){
		resultado = L"caballos de fuerza";
		return true; 
	}

	// kW
	boost::wregex kwER(L"^(kW)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, kwER, boost::regex_constants::format_perl)){
		resultado = L"kilovatios";
		return true; 
	}
	
	// MW
	boost::wregex MWER(L"^(MW)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, MWER, boost::regex_constants::format_perl)){
		resultado = L"megavatios";
		return true; 
	}
	
	// GW
	boost::wregex GWER(L"^(GW)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, GWER, boost::regex_constants::format_perl)){
		resultado = L"yigavatios";
		return true; 
	}
	
	// Wh
	boost::wregex WhER(L"^(Wh|W/h)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, WhER, boost::regex_constants::format_perl)){
		resultado = L"vatios hora";
		return true; 
	}
	
	
	// KWh
	boost::wregex kwhER(L"^(kWh|kW/h)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, kwhER, boost::regex_constants::format_perl)){
		resultado = L"kilvatios hora";
		return true; 
	}
	
	// MWh
	boost::wregex mwhER(L"^(MWh|MW/h)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, mwhER, boost::regex_constants::format_perl)){
		resultado = L"megavatios hora";
		return true; 
	}
	
	
	// GWh
	boost::wregex gwhER(L"^(GWh|GW/h)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, gwhER, boost::regex_constants::format_perl)){
		resultado = L"yigavatios hora";
		return true; 
	}
	
	// Joule o Julio
	boost::wregex julioER(L"^(J)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, julioER, boost::regex_constants::format_perl)){
		resultado = L"julios";
		return true; 
	}
	
	// kj
	boost::wregex kjER(L"^(kJ)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, kjER, boost::regex_constants::format_perl)){
		resultado = L"kilojulios";
		return true; 
	}
	
	// MJ
	boost::wregex MJER(L"^(MJ)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, MJER, boost::regex_constants::format_perl)){
		resultado = L"megajulios";
		return true; 
	}
	
	
	// GJ
	boost::wregex GJER(L"^(GJ)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, GJER, boost::regex_constants::format_perl)){
		resultado = L"yigajulios";
		return true; 
	}
	
	// kN
	boost::wregex knER(L"^(kN)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, knER, boost::regex_constants::format_perl)){
		resultado = L"kiloñutons";
		return true; 
	}
		
	// MN
	boost::wregex MNER(L"^(MN)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, MNER, boost::regex_constants::format_perl)){
		resultado = L"megañutons";
		return true; 
	}
	
	// GN
	boost::wregex GNER(L"^(GN)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, GNER, boost::regex_constants::format_perl)){
		resultado = L"yigañutons";
		return true; 
	}
	
	// kcal
	boost::wregex kcalER(L"^(kcal)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, kcalER, boost::regex_constants::format_perl)){
		resultado = L"kilocalorías";
		return true; 
	}
	
	
	// cal
	boost::wregex calER(L"^(cal)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, calER, boost::regex_constants::format_perl)){
		resultado = L"calorías";
		return true; 
	}
	
	
	// BTU
	boost::wregex BTUER(L"^(BTU)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, BTUER, boost::regex_constants::format_perl)){
		resultado = L"be te us";
		return true; 
	}
	
	// kDa
	boost::wregex kDaER(L"^(kDa)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, kDaER, boost::regex_constants::format_perl)){
		resultado = L"kilodaltons";
		return true; 
	}
	
	// Da
	boost::wregex DaER(L"^(Da)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, DaER, boost::regex_constants::format_perl)){
		resultado = L"daltons";
		return true; 
	}
	
	
	// Amperes
	boost::wregex AmperesER(L"^(A)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, AmperesER, boost::regex_constants::format_perl)){
		resultado = L"amperes";
		return true; 
	}
	
	// mA
	boost::wregex mAER(L"^(mA)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, mAER, boost::regex_constants::format_perl)){
		resultado = L"miliamperes";
		return true; 
	}
	
	// dB
	boost::wregex dBER(L"^(dB)(\\.)?$",boost::regex_constants::perl);
	if (boost::regex_match (str, dBER, boost::regex_constants::format_perl)){
		resultado = L"decibeles";
		return true; 
	}
	
	// ²
	boost::wregex cuadradoER(L"^(²)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, cuadradoER, boost::regex_constants::format_perl)){
		resultado = L"al cuadrado";
		return true; 
	}
	
	// ³
	boost::wregex cuboER(L"^(³)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, cuboER, boost::regex_constants::format_perl)){
		resultado = L"al cubo";
		return true; 
	}
	
		
	// Kb
	boost::wregex KbER(L"^(Kb)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, KbER, boost::regex_constants::format_perl)){
		resultado = L"kilobaits";
		return true; 
	}
	// Mb
	boost::wregex MbER(L"^(Mb)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, MbER, boost::regex_constants::format_perl)){
		resultado = L"megabaits";
		return true; 
	}
		
	// GB
	boost::wregex GbER(L"^(Gb)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, GbER, boost::regex_constants::format_perl)){
		resultado = L"yigabaits";
		return true; 
	}
	
	// TB
	boost::wregex TbER(L"^(Tb)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, TbER, boost::regex_constants::format_perl)){
		resultado = L"terabaits";
		return true; 
	}
	
	// Kbps
	boost::wregex KbpsER(L"^(Kbps)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, KbpsER, boost::regex_constants::format_perl)){
		resultado = L"kilobits por segundo";
		return true; 
	}
	
	// Mbps
	boost::wregex MbpsER(L"^(Mbps)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, MbpsER, boost::regex_constants::format_perl)){
		resultado = L"megabits por segundo";
		return true; 
	}
	
	// Gbps
	boost::wregex GbpsER(L"^(Gbps)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, GbpsER, boost::regex_constants::format_perl)){
		resultado = L"yigabits por segundo";
		return true; 
	}
	
	
	// Mbit
	boost::wregex MbitER(L"^(Mbit)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, MbitER, boost::regex_constants::format_perl)){
		resultado = L"megabits";
		return true; 
	}
	
	// Pts
	boost::wregex PtsER(L"^(pts)(\\.)?$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, PtsER, boost::regex_constants::format_perl)){
		resultado = L"puntos";
		return true; 
	}
	
	// Default - lo trato como sigla/abreviatura
	return EsSiglaOAbreviatura(str, dicc, abrev, resultado, fstLetras, isyms); 

}



// Expande los símbolos de monedas
bool ExpandirMoneda(wstring &str, wstring &resultado){

	// Pesos 
	boost::wregex pesosER(L"^(\\$|AR\\$|ARS)$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, pesosER, boost::regex_constants::format_perl)){
		resultado =  L"pesos"; 
		return true; 
	}
	
	// Dólares  
	boost::wregex dolaresER(L"^(U\\$S|US\\$|U\\$D|USD)$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, dolaresER, boost::regex_constants::format_perl)){
		resultado =  L"dólares"; 
		return true; 
	}
	
	// Euros
	boost::wregex eurosER(L"^(€|EUR)$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, eurosER, boost::regex_constants::format_perl)){	
		resultado =  L"euros"; 
		return true; 
	}
	
	// Libras
	boost::wregex librasER(L"^(£|GBP)$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, librasER, boost::regex_constants::format_perl)){	
		resultado =  L"libras"; 
		return true; 
	}
	
	// Francos Suizos
	boost::wregex francosER(L"^CHF$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, francosER, boost::regex_constants::format_perl)){	
		resultado =  L"francos suizos"; 
		return true; 
	}
	
	// Yenes
	boost::wregex yenesER(L"^(¥|JPY)$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, yenesER, boost::regex_constants::format_perl)){	
		resultado =  L"yenes"; 
		return true; 
	}
	
	// Dólares canadienses
	boost::wregex cadER(L"^(CAD)$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, cadER, boost::regex_constants::format_perl)){	
		resultado =  L"dólares canadienses"; 
		return true; 
	}
	
	// Dólares australianos
	boost::wregex audER(L"^(AUD)$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, audER, boost::regex_constants::format_perl)){	
		resultado = L"dólares australianos"; 
		return true; 
	}
	
	// Reales Brasileños
	boost::wregex brlER(L"^(BRL)$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, brlER, boost::regex_constants::format_perl)){	
		resultado = L"reales"; 
		return true; 
	}
	
	// Pesos mejicanos
	boost::wregex mxnER(L"^(MXN)$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, mxnER, boost::regex_constants::format_perl)){	
		resultado = L"pesos mejicanos"; 
		return true; 
	}
	
	// Pesos chilenos
	boost::wregex clpER(L"^(CLP)$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, clpER, boost::regex_constants::format_perl)){	
		resultado = L"pesos chilenos"; 
		return true; 
	}
	
	// Centavos 
	boost::wregex centavosER(L"^(¢)$",boost::regex_constants::icase|boost::regex_constants::perl);
	if (boost::regex_match (str, centavosER, boost::regex_constants::format_perl)){	
		resultado = L"centavos"; 
		return true; 
	}
	
	resultado = L"";
	return false; 
}
