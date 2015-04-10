#ifndef DICCITEM_H
#define DICCITEM_H


#include <string>

class diccItem {
	
public:
	std::wstring palabra, pronunciacion;
	
	diccItem();
	diccItem(const std::wstring &p, const std::wstring &pron);
	
	~diccItem();
	
	void SetWord(const std::wstring &p, const std::wstring &pron){
		palabra = p; 
		pronunciacion = pron; 
	}
	
	diccItem& operator=(const diccItem&);
	bool operator ==(const diccItem& b) const;
	bool operator <(const diccItem& b) const;
	bool operator >(const diccItem& b) const;
	
};

#endif // DICCITEM_H
