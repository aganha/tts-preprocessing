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

#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <iostream>
#include <fstream>
#include <utility>
#include <list>
#include <tuple>
#include <sstream>

#include <boost/regex.hpp>


#define MAX_REGULAR_EXP 200

using namespace std;

class Config {
	

public:
	// Lista que almacena los reemplazos: <ExpReg, TxtABuscar, TxtAReemplazar>
	list< std::tuple<wstring, wstring, wstring>> reemplazos;

	
	// Arrays de strings con las expresiones regulares que deterinan qué FST se usa. 
	std::wstring NaturalesRE[MAX_REGULAR_EXP];
	std::wstring RealesRE[MAX_REGULAR_EXP]; 
	std::wstring FechasRE[MAX_REGULAR_EXP]; 
	std::wstring HorasRE[MAX_REGULAR_EXP]; 
	std::wstring ImportesRE[MAX_REGULAR_EXP]; 
	std::wstring FraccionesRE[MAX_REGULAR_EXP]; 
	std::wstring GradosRE[MAX_REGULAR_EXP]; 
	
	// Cantidad de Expresiones Regulares
	int cntNaturalesRE; 
	int cntRealesRE; 
	int cntFechasRE; 
	int cntHorasRE; 
	int cntImportesRE; 
	int cntFraccionesRE; 
	int cntGradosRE;
	
	// Lista de abreviaturas: <abrev., AbrevExpandida>
	list<std::pair<wstring, wstring>> abrev;
	
	// Lee desde un archivo las definiciones de expresiones regulares
	int Read(char *filename); 
	
	// Devuelve una expresión regular para un fst 
	wstring ExpRegular(wstring fst); 
	
	// Reemplaza 
	void Replace(wstring &s);
	
	
	Config();
	~Config();
};

#endif // CONFIG_H
