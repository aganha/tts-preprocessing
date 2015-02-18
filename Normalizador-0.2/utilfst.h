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
// utilfst.h
// Definiciones de enumeraciones y structs utilizadas por los FSTs 
/* =============================================================== */
#include <fst/fstlib.h>
#include <fst/fst-decl.h>
#include <fst/union.h>
using namespace fst;

/*
 * Funciones de utilidad
 * Conversión  UTF-8 / wstring
 * Declaraciones
 */
char *UnicodeToCodePage(int codePage, const wchar_t *src);
std::string to_utf8(const std::wstring& str);
std::wstring strtowstr(const std::string &str);
// Agregada por Ezequiel Saudino 
std::string CP1252ToUTF8(const char *src);
std::wstring utf8_to_utf16(const std::string& utf8);

// ================
// Definición de símbolos asociados a los FSTs (isyms)
// ================
enum symbol 
{
   SYMBOL_eps = 0,
   SYMBOL_ESP	=	32	,
   SYMBOL_ADMC	=	33	,
   SYMBOL_QUOT	=	34	,
   SYMBOL_SHARP	=	35	,
   SYMBOL_PESOS	=	36	,
   SYMBOL_PERCENT	=	37	,
   SYMBOL_UMPER	=	38	,
   SYMBOL_CUTE	=	39	,
   SYMBOL_PAP	=	40	,
   SYMBOL_PCE	=	41	,
   SYMBOL_AST	=	42	,
   SYMBOL_MAS	=	43	,
   SYMBOL_COM	=	44	,
   SYMBOL_MEN	=	45	,
   SYMBOL_PUN	=	46	,
   SYMBOL_BAR	=	47	,
   SYMBOL_0	=	48	,
   SYMBOL_1	=	49	,
   SYMBOL_2	=	50	,
   SYMBOL_3	=	51	,
   SYMBOL_4	=	52	,
   SYMBOL_5	=	53	,
   SYMBOL_6	=	54	,
   SYMBOL_7	=	55	,
   SYMBOL_8	=	56	,
   SYMBOL_9	=	57	,
   SYMBOL_DPU	=	58	,
   SYMBOL_PYC	=	59	,
   SYMBOL_SME	=	60	,
   SYMBOL_IGUAL	=	61	,
   SYMBOL_SMA	=	62	,
   SYMBOL_PREGC	=	63	,
   SYMBOL_ARROBA	=	64	,
   SYMBOL_A	=	65	,
   SYMBOL_B	=	66	,
   SYMBOL_C	=	67	,
   SYMBOL_D	=	68	,
   SYMBOL_E	=	69	,
   SYMBOL_F	=	70	,
   SYMBOL_G	=	71	,
   SYMBOL_H	=	72	,
   SYMBOL_I	=	73	,
   SYMBOL_J	=	74	,
   SYMBOL_K	=	75	,
   SYMBOL_L	=	76	,
   SYMBOL_M	=	77	,
   SYMBOL_N	=	78	,
   SYMBOL_O	=	79	,
   SYMBOL_P	=	80	,
   SYMBOL_Q	=	81	,
   SYMBOL_R	=	82	,
   SYMBOL_S	=	83	,
   SYMBOL_T	=	84	,
   SYMBOL_U	=	85	,
   SYMBOL_V	=	86	,
   SYMBOL_W	=	87	,
   SYMBOL_X	=	88	,
   SYMBOL_Y	=	89	,
   SYMBOL_Z	=	90	,
   SYMBOL_CAP	=	91	,
   SYMBOL_BARRAINV	=	92	,
   SYMBOL_CAC	=	93	,
   SYMBOL_CIRC	=	94	,
   SYMBOL_GBA	=	95	,
   SYMBOL_APO	=	96	,
   SYMBOL_a	=	97	,
   SYMBOL_b	=	98	,
   SYMBOL_c	=	99	,
   SYMBOL_d	=	100	,
   SYMBOL_e	=	101	,
   SYMBOL_f	=	102	,
   SYMBOL_g	=	103	,
   SYMBOL_h	=	104	,
   SYMBOL_i	=	105	,
   SYMBOL_j	=	106	,
   SYMBOL_k	=	107	,
   SYMBOL_l	=	108	,
   SYMBOL_m	=	109	,
   SYMBOL_n	=	110	,
   SYMBOL_o	=	111	,
   SYMBOL_p	=	112	,
   SYMBOL_q	=	113	,
   SYMBOL_r	=	114	,
   SYMBOL_s	=	115	,
   SYMBOL_t	=	116	,
   SYMBOL_u	=	117	,
   SYMBOL_v	=	118	,
   SYMBOL_w	=	119	,
   SYMBOL_x	=	120	,
   SYMBOL_y	=	121	,
   SYMBOL_z	=	122	,
   SYMBOL_LLA	=	123	,
   SYMBOL_PIPE	=	124	,
   SYMBOL_LLC	=	125	,
   SYMBOL_TILDE	=	126	,
   SYMBOL_ADMA  =   161 ,
   SYMBOL_ace	=	233	,
   SYMBOL_ACE	=	201	,
   SYMBOL_aca	=	225	,
   SYMBOL_aci	=	237	,
   SYMBOL_aco	=	243	,
   SYMBOL_acu	=	250	,
   SYMBOL_ene	=	241	,
   SYMBOL_ENE	=	209	,
   SYMBOL_ORD 	=	176	,
   SYMBOL_ORDB 	=	186	,
   SYMBOL_ACA	=	193	,
   SYMBOL_ACI	=	205	,
   SYMBOL_ACO	=	211	,
   SYMBOL_ACU	=	218 ,
   SYMBOL_COMA =	171 ,
   SYMBOL_COMC =	187 ,
   SYMBOL_PREA =	191 ,
   SYMBOL_DIEA =	196 ,
   SYMBOL_DIEE =	203 ,
   SYMBOL_DIEI =	207 ,
   SYMBOL_DIEO =	214 ,
   SYMBOL_DIEU =	220 ,
   SYMBOL_diea =	228 ,
   SYMBOL_diee =	235 ,
   SYMBOL_diei =	239 ,
   SYMBOL_dieo =	246 ,
   SYMBOL_dieu =	252
 };

struct ordinalesFST
{
	StdVectorFst *pUnidad;
	StdVectorFst *pDecenas;
	StdVectorFst *pCentenas;
    StdVectorFst *pMilesU;
    StdVectorFst *pMilesD;
	StdVectorFst *pMilesC;
	StdVectorFst *pMillones;
};

struct naturalesFST
{
	StdVectorFst *pUnidad;
	StdVectorFst *pDecenas;
	StdVectorFst *pCentenas;
    StdVectorFst *pMilesU;
    StdVectorFst *pMilesD;
	StdVectorFst *pMilesC;
    StdVectorFst *pMillones;
};

struct romanosFST
{
	StdVectorFst *pUnidad;
	StdVectorFst *pDecenas;
	StdVectorFst *pCentenas;
    StdVectorFst *pMiles;
};

enum genero 
{
   GENERO_MASC,
   GENERO_FEM,
   GENERO_NEUTRO
};

enum tipoAbrev 
{
   ABREV_SING,
   ABREV_PLURAL,
   ABREV_MASC,
   ABREV_FEM
};
