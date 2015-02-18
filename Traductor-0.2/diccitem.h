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
