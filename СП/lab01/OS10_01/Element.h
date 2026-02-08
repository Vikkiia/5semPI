#pragma once

#include <Windows.h>
#include <iostream>

namespace ht
{
	struct Element   
	{
		Element();
		Element(const void* key, int keyLength);                                             
		Element(const void* key, int keyLength, const void* payload, int  payloadLength);    
		Element(const Element* oldElement, const void* newPayload, int  newPayloadLength);  
		const void* key;                 
		int         keyLength;           
		const void* payload;           
		int         payloadLength;       
	};

	int getSizeElement(
		int   maxKeyLength,                
		int   maxPayloadLength);           
}