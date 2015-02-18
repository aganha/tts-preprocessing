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

/*
 * Funciones de utilidad
 * Conversión  UTF-8
 * http://www.chilkatsoft.com/p/p_348.asp
 */
//#include <windows.h>
#include <iostream>
#include <vector>
#include <iconv.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>




using namespace std;



std::wstring utf8_to_utf16(const std::string& utf8)
{
    std::vector<unsigned long> unicode;
    size_t i = 0;
    while (i < utf8.size())
    {
        unsigned long uni;
        size_t todo;
        bool error = false;
        unsigned char ch = utf8[i++];
        if (ch <= 0x7F)
        {
            uni = ch;
            todo = 0;
        }
        else if (ch <= 0xBF)
        {
            return (L""); // error
        }
        else if (ch <= 0xDF)
        {
            uni = ch&0x1F;
            todo = 1;
        }
        else if (ch <= 0xEF)
        {
            uni = ch&0x0F;
            todo = 2;
        }
        else if (ch <= 0xF7)
        {
            uni = ch&0x07;
            todo = 3;
        }
        else
        {
            return (L""); // error
        }
        for (size_t j = 0; j < todo; ++j)
        {
            if (i == utf8.size())
                return (L""); // error
            unsigned char ch = utf8[i++];
            if (ch < 0x80 || ch > 0xBF)
                return (L""); // error
            uni <<= 6;
            uni += ch & 0x3F;
        }
        if (uni >= 0xD800 && uni <= 0xDFFF)
            return (L""); // error
        if (uni > 0x10FFFF)
            return (L""); // error
        unicode.push_back(uni);
    }
    std::wstring utf16;
    for (size_t i = 0; i < unicode.size(); ++i)
    {
        unsigned long uni = unicode[i];
        if (uni <= 0xFFFF)
        {
            utf16 += (wchar_t)uni;
        }
        else
        {
            uni -= 0x10000;
            utf16 += (wchar_t)((uni >> 10) + 0xD800);
            utf16 += (wchar_t)((uni & 0x3FF) + 0xDC00);
        }
    }
    return utf16;
}



/*
 * SOLUCION P/convertir Unicode -> Code Page (UTF8)
 * http://www.chilkatsoft.com/p/p_348.asp
*/
/*	
char *UnicodeToCodePage(int codePage, const wchar_t *src)
{
	iconv_t cd;
	
	cd = iconv_open("utf-8", "wchar_t");
	if((int)cd == -1)
		return (0);

	unsigned int len;
	unsigned int target_len;
	char *target;
	char *target_start;
	int len_start;
	int target_len_start;

	len = wcslen(src);
	
	if(!len)
		return (0);

	target_len = 4*len+1;
	target = (char *)calloc(target_len, 1);
	len_start = len;
	target_len_start = target_len;
	target_start = target;
	

	size_t iconv_value;

	iconv_value = iconv(cd, (char**) &src, &len, &target, &target_len);

	if(iconv_value == (size_t)-1)
		return (0);
		
	return target_start;
}
*/
/*
 * SOLUCION P/convertir wstring -> Code Page (UTF8)
 * http://mariusbancila.ro/blog/2008/10/20/writing-utf-8-files-in-c/
 */

std::string to_utf8(const wchar_t* buffer, int len)
{
//		int nChars; 
		
    /*   int nChars = ::WideCharToMultiByte(
                CP_UTF8,
                0,
                buffer,
                len,
                NULL,
                0,
                NULL,
                NULL);*/
       // if (nChars == 0) return "";

        string newbuffer;
      //  newbuffer.resize(nChars) ;
        /*::WideCharToMultiByte(
                CP_UTF8,
                0,
                buffer,
                len,
                const_cast< char* >(newbuffer.c_str()),
                nChars,
                NULL,
                NULL); */

        return newbuffer;
}

std::string to_utf8(const std::wstring& str)
{
        return to_utf8(str.c_str(), (int)str.size());
}


/*
 * SOLUCION P/convertir string -> wstring
 */

std::wstring strtowstr(const std::string &str)
{
    // Convert an ASCII string to a Unicode String
    std::wstring wstrTo;
    wchar_t *wszTo = new wchar_t[str.length() + 1];
    wszTo[str.size()] = L'\0';
    /*MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wszTo, (int)str.length());*/
    wstrTo = wszTo;
    delete[] wszTo;
    return wstrTo;
}

// Agregada por Ezequiel Saudino 
string CP1252ToUTF8(const char *src)
{
	iconv_t cd;
	
	cd = iconv_open("utf-8", "CP1252");
	
	if(cd == ((iconv_t) -1))
		return ("");

	size_t len;
	size_t target_len;
	char *target;
	char *target_start;
	size_t len_start;
	size_t target_len_start;

	len = strlen(src);
	
	if(!len)
		return ("");

	target_len = len+1;
	target = (char *)calloc(target_len, 1);
	len_start = len;
	target_len_start = target_len;
	target_start = target;
	
	size_t iconv_value;

	iconv_value = iconv(cd, (char**) &src, &len, &target, &target_len);

	if(iconv_value == (size_t)-1)
		return ("");
		
	string res = string(target_start);
	
	free(target_start);
	
	return (res);
		
}


/*
 
  http://www.unix.com/programming/10594-howto-convert-ascii-utf-8-back-c.html 
    
 */

/*
#include <stdio.h>                                                                                           
#include <stdlib.h>                                                                                          
unsigned char* ASCIItoUNICODE (unsigned char ch);                                                            
unsigned int* ConvertString (unsigned char *string);                                                         
void UnicodePrint(unsigned int* Message);                                                                    
                                                                                                             
                                                                                                             
int main()                                                                                                   
{                                                                                                            
        unsigned char text[] = "Ïðèâåò,world" ;                                                              
        unsigned int *UnMess;                                                                                
        printf("Content-type: text/plain\n\n\n");                                                            
                                                                                                             
                                                                                                             
        UnMess = ConvertString(text);                                                                        
        UnicodePrint(UnMess);                                                                                
                                                                                                             
}                                                                                                            
                                                                                                             
unsigned char* ASCIItoUNICODE (unsigned char ch)                                                             
{                                                                                                            
unsigned char Val[2];                                                                                        
if ((ch < 192)&&(ch != 168)&&(ch != 184))  {Val[0] = 0; Val[1] = ch;    return Val;}                         
if (ch == 168) {Val[0] = 208;   Val[1] = 129;   return Val;}                                                 
if (ch == 184) {Val[0] = 209;   Val[1] = 145;   return Val;}                                                 
if (ch < 240)  {Val[0] = 208;   Val[1] = ch-48; return Val;}                                                 
if (ch < 249)  {Val[0] = 209;   Val[1] = ch-112;        return Val;}                                         
}                                                                                                            
unsigned int* ConvertString (unsigned char *string)                                                          
{                                                                                                            
    unsigned int size=0, *NewString;                                                                         
        unsigned char* Uni;                                                                                  
    while (string[size++]!=0);                                                                               
        NewString = (unsigned int*)malloc(sizeof(unsigned int)*2*size-1);                                    
        NewString[0]=2*size-1;                                                                               
        size=0;                                                                                              
        while (string[size]!=0)                                                                              
        {                                                                                                    
            Uni = ASCIItoUNICODE(string[size]);                                                              
            NewString[2*size+1]=Uni[0];                                                                      
            NewString[2*size+2]=Uni[1];                                                                      
            size++;                                                                                          
        }                                                                                                    
        return NewString;                                                                                    
}                                                                                                            
                                                                                                             
void UnicodePrint(unsigned int* Message)     
{                                                                                                            
        int size, i;                                                                                         
                                                                                                             
        size=Message[0];                                                                                     
        if (Message[1] == 0) i=2;                                                                            
                else i=1;                                                                                    
                                                                                                             
                while (i<size)                                                                               
                {                                                                                            
                printf("%C",Message[i]);                                                                     
                        i++;                                                                                 
                }                                                                                            
}






#include <iconv.h>

iconv_t cd;
char out[7];
wchar_t in = CODE_POINT_VALUE;
size_t inlen = sizeof(in), outlen = sizeof(out);

cd = iconv_open("utf-8", "wchar_t");

iconv(cd, (char **)&in, &inl, &out, &outlen);

iconv_close(cd);

But I fear that wchar_t might not represent Unicode code points, but arbitrary values.. EDIT: I guess you can do it by simply using a Unicode source:

uint16_t in = UNICODE_POINT_VALUE;
cd = iconv_open("utf-8", "ucs-2");
*/