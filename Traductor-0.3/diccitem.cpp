#include "diccitem.h"

diccItem::diccItem()
{
	palabra = L""; 
	pronunciacion = L""; 
}

diccItem::diccItem(const std::wstring &p, const std::wstring &pron){
	palabra = p; 
	pronunciacion = pron; 
}


diccItem::~diccItem()
{
}
	
bool diccItem::operator ==(const diccItem& b) const{
	return (palabra.length() == b.palabra.length() && palabra.compare(b.palabra) == 0);
}
bool diccItem::operator <(const diccItem& b)const {
	return (palabra.compare(b.palabra) < 0);
}
bool diccItem::operator >(const diccItem& b)const{
	return (palabra.compare(b.palabra) > 0);
}

diccItem& diccItem::operator=(const diccItem& b){
	pronunciacion = b.pronunciacion;
	palabra = b.palabra;
	return *this; 
}

