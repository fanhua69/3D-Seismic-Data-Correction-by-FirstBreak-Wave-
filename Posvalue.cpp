	
	#include "posvalue.h"

	PosValue PosValue::operator =(PosValue other){
		pos=other.pos;
		value=other.value;
		return *this;
	}

