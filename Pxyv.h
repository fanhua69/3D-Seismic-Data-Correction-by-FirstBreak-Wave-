#ifndef tagPXYV
#define tagPXYV
class PXYV{
public:
	long Ph;
	double east;
	double north;
	float Value;  // int have same size with long type.

	void operator =(PXYV &other){
		Ph=other.Ph;
		east=other.east;
		north=other.north;
		Value=other.Value;
	};
};
#endif
