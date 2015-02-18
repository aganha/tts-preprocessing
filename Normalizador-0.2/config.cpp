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

#include "config.h"

Config::Config()
{
	cntNaturalesRE = 0; 
	cntRealesRE = 0; 
	cntFechasRE = 0;  
	cntHorasRE = 0;  
	cntImportesRE = 0;  
	cntFraccionesRE = 0;  
	cntGradosRE = 0; 
	
}

Config::~Config()
{
}

int Config::Read(char *filename){
	
	wstring linea; 
	wstringstream archivo;
 
	int reading = 0; 
	wstring s1, s2, s3;  

	
	wifstream archivoEntrada(filename, ifstream::in);
	
	archivo << archivoEntrada.rdbuf() << '\0';
	
	if (archivoEntrada.good()){
		
		while(getline(archivo, linea)){
			
			if (linea.length() > 0 ){
			
				wstringstream sin (wstringstream::in);
				sin.str(linea);
				
				if (linea.compare(L"<Reemplazos>")==0)  reading=1;
						else if (linea.compare(L"</Reemplazos>")==0) reading=0;
						else if (linea.compare(L"<ExpRegulares>")==0) reading=2;
						else if (linea.compare(L"</ExpRegulares>")==0) reading=0;
						else if (linea.compare(L"<Abreviaturas>")==0) reading=3;
						else if (linea.compare(L"</Abreviaturas>")==0) reading=0;
						
					else if (reading==1) {
					// importando "Reemplazos" 
									
					getline(sin, s1, L'\t');
					getline(sin, s2, L'\t');
					getline(sin, s3, L'\t');
									
					reemplazos.push_back(make_tuple(s1, s2, s3));
					
					}
					else if (reading==2) {
					// importando "ExpRegulares" 
					getline(sin, s1, L'\t');
					getline(sin, s2, L'\t');
					
					
					// Expresión regular para el FST "Naturales" 
					if (s1.compare(L"Naturales")== 0){
						if (cntNaturalesRE < MAX_REGULAR_EXP){
							NaturalesRE[cntNaturalesRE] = s2; 
							cntNaturalesRE++; 
						}
					}else if (s1.compare(L"Reales")== 0){ // Reales
						if (cntRealesRE < MAX_REGULAR_EXP){
							RealesRE[cntRealesRE] = s2; 
							cntRealesRE++; 
						}
					}else if (s1.compare(L"Fechas")== 0){ // Fechas
						if (cntFechasRE < MAX_REGULAR_EXP){
							FechasRE[cntFechasRE] = s2; 
							cntFechasRE++; 
						}
					}if (s1.compare(L"Horas")== 0){ // Horas
						if (cntHorasRE < MAX_REGULAR_EXP){
							HorasRE[cntHorasRE] = s2; 
							cntHorasRE++; 
						}
					}if (s1.compare(L"Fracciones")== 0){ // Fracciones
						if (cntFraccionesRE < MAX_REGULAR_EXP){
							FraccionesRE[cntFraccionesRE] = s2; 
							cntFraccionesRE++; 
						}
					}if (s1.compare(L"Importes")== 0){ // Importes
						if (cntImportesRE < MAX_REGULAR_EXP){
							ImportesRE[cntImportesRE] = s2; 
							cntImportesRE++; 
						}
					}if (s1.compare(L"Grados")== 0){ // Grados
						if (cntGradosRE < MAX_REGULAR_EXP){
							GradosRE[cntGradosRE] = s2; 
							cntGradosRE++; 
						}
					}
			 
					}else if (reading==3) {
					 // importando abreviaturas
					 getline(sin, s1, L'\t');
					 getline(sin, s2, L'\t');
					 abrev.push_back(make_pair(s1, s2));
				}
				
			}
	   	}

	}else{
		return -1; 
	}
	
	return 0; 
}

wstring Config::ExpRegular(wstring fst){
	wstring result = L""; 
	wstring *p;
	int *cnt; 
	int i; 
	
	
	p = NULL; 
	if (fst.compare(L"Naturales")==0){
		cnt = &cntNaturalesRE; 
		p = NaturalesRE; 
	}else if (fst.compare(L"Reales")==0){
		cnt = &cntRealesRE; 
		p = RealesRE; 
	}else if (fst.compare(L"Fechas")==0){
		cnt = &cntFechasRE; 
		p = FechasRE; 
	}else if (fst.compare(L"Horas")==0){
		cnt = &cntHorasRE; 
		p = HorasRE; 
	}else if (fst.compare(L"Importes")==0){
		cnt = &cntImportesRE; 
		p = ImportesRE; 
	}else if (fst.compare(L"Grados")==0){
		cnt = &cntGradosRE; 
		p = GradosRE; 
	}else if (fst.compare(L"Fracciones")==0){
		cnt = &cntFraccionesRE; 
		p = FraccionesRE; 
	} 
	
		
	if (p){
		for (i = 0; i < *cnt; i++){
			if (i > 0)
				result += L"|(" +  p[i] +  L")"; 
			else
				result = L"(" +  p[i] +  L")"; 
		}
	}
	
	return result; 
}

// Realiza los reemplazos especificados en la seccion <Reemplazos> del archivo de cofiguración 
void Config::Replace(wstring &s){
	
	list< std::tuple<wstring, wstring, wstring>>::iterator i;
	wstring s1, s2, reemplazar; 
	for (i=reemplazos.begin(); i!=reemplazos.end(); i++) {
		
		s1 = std::get<0>(*i);
		s2 = std::get<1>(*i); 
	
		boost::wregex re(std::get<0>(*i));
		boost::wregex buscar(std::get<1>(*i));
		reemplazar = std::get<2>(*i);
		
		// Si matchea con la expresión regular, hago el reemplazo
		if (boost::regex_match (s, re, boost::regex_constants::format_perl)){
			s = boost::regex_replace(s, buscar, reemplazar, boost::match_default|boost::format_perl);
			
		}
				
	}
}


