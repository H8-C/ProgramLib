
typedef union 
{                           
	_U1 U1;               
	struct 
	{                            
		_U1 B7:1;           
		_U1 B6:1;           
		_U1 B5:1;          
		_U1 B4:1;          
		_U1 B3:1;           
		_U1 B2:1;           
		_U1 B1:1;           
		_U1 B0:1;           
	}BIT;                   
}U_U1;                  

typedef union 
{                                    
	_U2 U2;                 
	struct 
	{                             
		U_U1	H;           
		U_U1	L;          
	}BIT;                  
}U_U2;                   

typedef union 
{                                    
	_U4 U4;                 
	struct 
	{                             
		U_U2	H;           
		U_U2	L;           
	}BIT;                   
}U_U4;                   




typedef union 
{                                    
	_U1 U8[8];                 
	struct 
	{                             
		U_U4	H;           
		U_U4	L;           
	}BIT;                   
}U_U8;                   


typedef union 
{                                    
	_U1 U16[16];                 
	struct 
	{                             
		U_U8	H;           
		U_U8	L;           
	}BIT;                   
}U_U16;                   

/////////////////////////////////////////////////
typedef union 
{                                    
	_U1		U1;                 
	struct 
	{                             
		_U1	H:4;           
		_U1	L:4;           
	}BIT;                   
}U_H1;                   


typedef union 
{                                    
	_U2		U2;                 
	struct 
	{                             
		U_H1	H;           
		U_H1	L;           
	}BIT;                   
}U_H2;                   

typedef union 
{                                    
	_U4		U4;                 
	struct 
	{                             
		U_H2	H;           
		U_H2	L;           
	}BIT;                   
}U_H4;                   


typedef union 
{                                    
	_U4		UB4;                 
	struct 
	{                             
		U_H4	H;
		U_H4	L;
	}BIT;                   

}U_H8;                   


typedef union 
{                                    
	_U1		D16[17];		                             
	union 
	{                             
		U_H8	H;                 
		U_H8	L;                 
	}BIT;
}U_H16;                   



typedef union 
{                                    
	_U1		U16[17];                 
	struct 
	{                             
		U_H1	HL[16];               
	}BIT;
	                   
}U_16;                   

typedef union 
{                                    
	_U1		U16[17];                 
	struct 
	{                             
		U_H1	U[16];		               
	}BIT;
}U_B16;                   


typedef union 
{                                    
	_U1		U16[16];                 
	struct 
	{                             
		_U1		H:4;
		_U1		L:4;          
	}B[16];
}U_A16;                   

//////////////////////////////////////////////


typedef union 
{                                    
	_U4		U4;                 
	struct 
	{                             
		_U1		 :4;
		_U1		H:4;          
		_U1		M:8;
		_U1		L:4;          
		_U1		 :8;          
	}BIT;
}U_SWAP;                   


typedef union 
{                                    
	_U1		U16[16];                 
	struct 
	{                             
		_U1		H:4;          
		_U1		L:4;          
	}U1[16];
}U_STR;                   

typedef struct 
{                                    
	U_H1	SRC[16]	;
	U_H1	DES[16]	;
	U_H1	ANS[16]	;
	U_H1	REM[16]	;
	U_H1	MUL[16]	;
	U_H1	DIV[16]	;
	U_U1	ERR		;
}U_ET32;                   



extern	void	Fx_SFT_L(_U1*   );

extern	void	Fx_SFT_R(_U1*   );

extern	void	Fx_SET(_U1*  ,_U1*   );














