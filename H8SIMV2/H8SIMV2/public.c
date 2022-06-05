#include	"iodefine.h"
#include	"typedefine.h"
#include	"public.h"
#include	"ram_ext.h"



/*
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
	_U1		U1;                 
	struct 
	{                             
		_U1	H:4;           
		_U1	L:4;           
	}BIT;                   
}U_H1;                   

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



	U_ET32		R_ET32 ={0};


//	Test BCD CALC   SRC[16] / DES[16] = ANS[16] .... REM[16]  ERR
	Fx_SET(	(_U1*)R_ET32.SRC , (_U1*)"00000000000000009999999999999999"  );
	Fx_SET(	(_U1*)R_ET32.DES , (_U1*)"00000000000000009999999999999999"  );
	Fx_DIV(	R_ET32 );


*/


#define		SIZE	16

#define		LEFT	1
#define		RIGT	3

void FxWait(_U2 time)
{
	while(time--);
}


/*! 
@brief	  	Fx_ZERO
@param		_U1* src 
@return		_U1
@par	 	

	src[] == 0   return  0  else  NOT 0
	
*/
_U1	Fx_ZERO(_U1* src )
{
	_U1		cnt;
	_U1		CHK = 0;	
	for(cnt = 0 ; cnt < SIZE ; cnt++)
	{
		CHK |= *(src+cnt) ;			
	}
	return	CHK;
}


/*! 
@brief	  	Fx_EQR
@param		_U1* src  ,_U1	*des
@return		_U1
@par	 	

	src[] == drs[]   return  0  else  NOT 0
	
*/
_U1	Fx_EQR(_U1* src  ,_U1	*des)
{
	_U1		cnt;
	_U1		CHK = 0;	
	for(cnt = 0 ; cnt < SIZE ; cnt++)
	{
		CHK |=	*(src+cnt) 	^ *(des+cnt);			
	}
	return	CHK;
}




/*! 
@brief	  	Fx_CLR
@param		_U1* src 
@return		void
@par	 	

	src[] = 0
	
*/
void	Fx_CLR(_U1* src )
{
	_U1		cnt;	
	for(cnt = 0 ; cnt < SIZE ; cnt++)
	{
		*(src+cnt) 	= 0;			
	}
}

/*! 
@brief	  	Fx_SFT_L
@param		_U1* src 
@return		void
@par	 	

	src[] <<= 4
	
*/

void	Fx_SFT_L(_U1* src  )
{
	U_SWAP	tmp	= 0;
	_U1		cnt = SIZE-1;
	
	do
	{
		tmp.BIT.M 		= *(src+cnt);
		tmp.U4   	  	<<= 4;	
		*(src+cnt) 		= tmp.BIT.M;
		tmp.BIT.L		= tmp.BIT.H;
				
	}while(cnt--);	
}



/*! 
@brief	  	Fx_SFT_R
@param		_U1* src 
@return		void
@par	 	

	src[] >>= 4
	
*/
void	Fx_SFT_R(_U1* src  )
{
	U_SWAP	tmp	= 0;
	_U1		cnt = 0;
	for(cnt = 0 ; cnt < SIZE ; cnt++)
	{
		tmp.BIT.M 	= *(src+cnt);
		tmp.U4   	>>= 4;	
		*(src+cnt) 	= tmp.BIT.M;
		tmp.BIT.H	= tmp.BIT.L;
	}	
}


/*! 
@brief	  	Fx_A2I1
@param		_U1* src ,_U1* des
@return		void
@par	 	

	SRC[] ＝ DES[H] - '0' + DES[L] - '0'
	
*/
void	Fx_A2I1(_U1* src ,_U1* des  )
{
	U_H1	tmp;
	_U1		cnt;
	for(cnt = 0 ; cnt < SIZE ; cnt++)
	{
		tmp.BIT.H 	= *des ;	des++;
		tmp.BIT.L 	= *des ;	des++;	
		*(src+cnt) 	= tmp.U1;
	}
}

/*! 
@brief	  	Fx_CPY
@param		_U1* src ,_U1* des
@return		void
@par	 	

	SRC[] ←　 DES[]
	
*/

void	Fx_CPY(_U1* src ,_U1* des  )
{
	_U1		cnt;
	for(cnt = 0 ; cnt < SIZE ; cnt++)
	{
		*(src+cnt) 	= *(des+cnt);
	}
}

/*! 
@brief	  	Fx_ADD
@param		U_ET32 *inp
@return		void
@par	 	

	SRC + DES = ANS
	
	Carry = 1 is over calculation
*/


_U1	Fx_ADD(U_ET32 *inp  )
{	
	U_H1	tmp	= {0};
	U_H1	CYA	= {0};
	_U1		cnt = SIZE-1;
	
		
	Fx_CLR( (_U1*) inp->ANS  );
	Fx_CLR( (_U1*) inp->REM  );
	Fx_CLR( (_U1*) inp->DIV  );
	Fx_CLR( (_U1*) inp->MUL  );
	inp->ERR.U1	= 0;

	do
	{
		tmp.U1	=(_U1) inp->SRC[cnt].BIT.L + inp->DES[cnt].BIT.L  + CYA.BIT.L;		
		if ( tmp.U1 >= 10 )		tmp.U1 += 6;
		CYA.BIT.L	=  tmp.BIT.H;		inp->ANS[cnt].BIT.L	=  tmp.BIT.L;
		
		tmp.U1	=(_U1) inp->SRC[cnt].BIT.H + inp->DES[cnt].BIT.H  + CYA.BIT.L;
		if ( tmp.U1 >= 10 )		tmp.U1 += 6;
		CYA.BIT.L	=  tmp.BIT.H;		inp->ANS[cnt].BIT.H	=  tmp.BIT.L;
				
	}while(cnt--);	
	inp->ERR.U1	= CYA.U1;
	return 	inp->ERR.U1	;

}

/*! 
@brief	  	Fx_SUB
@param		U_ET32 *inp
@return		void
@par	 	
	BCD16BYTE subtraction
	SRC []-DES [] Carry = 1 is overcalculated


	SRC-DES = ANS

	If the subtraction is negative, it is 10 or more.
	
	Borrow from the top and subtract from 10 for the bottom = 0x1A
	
	0x100 --0x1A = 0xE6
	
*/
_U1	Fx_SUB(U_ET32 *inp  )
{	
	U_H1	tmp	= {0};
	U_H1	CYA	= {0};
	_U1		cnt = SIZE-1;
	
		
	Fx_CLR( (_U1*) inp->ANS  );
	Fx_CLR( (_U1*) inp->REM  );
	Fx_CLR( (_U1*) inp->DIV  );
	Fx_CLR( (_U1*) inp->MUL  );
	inp->ERR.U1	= 0;

	do
	{
		tmp.U1	=(_U1) inp->SRC[cnt].BIT.L - inp->DES[cnt].BIT.L - CYA.BIT.L;		
		if ( tmp.U1 >= 10 )		tmp.U1 -= 0xE6;
		CYA.BIT.L	=  tmp.BIT.H;		inp->ANS[cnt].BIT.L	=  tmp.BIT.L;
		
		tmp.U1	=(_U1) inp->SRC[cnt].BIT.H - inp->DES[cnt].BIT.H  - CYA.BIT.L;
		if ( tmp.U1 >= 10 )		tmp.U1 -= 0xE6;
		CYA.BIT.L	=  tmp.BIT.H;		inp->ANS[cnt].BIT.H	=  tmp.BIT.L;
				
	}while(cnt--);	
	inp->ERR.U1	= CYA.U1;
	return 	inp->ERR.U1	;
}


/*! 
@brief	  	Fx_MUL
@param		U_ET32 *inp
@return		void
@par	 	
	BCD16BYTE multiplication
	
	SRC [] * DES [] Carry = 1 is over calculation


	SRC * DES = ANS
	
	
	BCD correction is required at the time of addition

*/
_U1	Fx_MUL(U_ET32 *inp  )
{	
	U_H1	tmp	= {0};
	U_H1	CYA	= {0};
	_U1		cnt = SIZE-1;
	_U1		LOP = 0;
	_U1		KAI = 0;
	
		
	Fx_CLR( (_U1*) inp->ANS  );
	Fx_CLR( (_U1*) inp->REM  );
	Fx_CLR( (_U1*) inp->DIV  );
	Fx_CLR( (_U1*) inp->MUL  );
	inp->ERR.U1	= 0;
	
	Fx_CPY(	(_U1*)inp->MUL , (_U1*)inp->SRC);
	Fx_CPY(	(_U1*)inp->DIV , (_U1*)inp->DES);

	while( Fx_ZERO( (_U1*) inp->DIV ) )
	{
		KAI = inp->DIV[SIZE-1].BIT.L;
		while( KAI-- )
		{

			CYA.U1 		= 0;
			tmp.U1 		= 0;	
			cnt 		= SIZE-1;
			do
			{
				tmp.U1	=(_U1) inp->MUL[cnt].BIT.L + inp->ANS[cnt].BIT.L  + CYA.BIT.L;		
				if ( tmp.U1 >= 10 )		tmp.U1 += 6;
				CYA.BIT.L	=  tmp.BIT.H;		inp->ANS[cnt].BIT.L	=  tmp.BIT.L;
		
				tmp.U1	=(_U1) inp->MUL[cnt].BIT.H + inp->ANS[cnt].BIT.H  + CYA.BIT.L;
				if ( tmp.U1 >= 10 )		tmp.U1 += 6;
				CYA.BIT.L	=  tmp.BIT.H;		inp->ANS[cnt].BIT.H	=  tmp.BIT.L;
				
			}while(cnt--);
		}
		Fx_SFT_R((_U1*)inp->DIV );
		Fx_SFT_L((_U1*)inp->MUL );	
	}
	inp->ERR.U1	= CYA.U1;
	return 	inp->ERR.U1	;
}

/*! 
@brief	  	Fx_SUB_CHK
@param		U_ET32 *inp
@return		_U1
@par	 	
	Used in division
	If >= in SRC / DES, the return will be 1 except 0
	
	If the subtraction is negative, it is 10 or more.
	
	Borrow from the top and subtract from 10 for the bottom = 0x1A

*/
_U1	Fx_SUB_CHK(U_ET32 *inp  )
{	
	U_H1	tmp	= {0};
	U_H1	CYA	= {0};
	_U1		cnt = SIZE-1;
	_U1		TMP[16]={0};

	//　TMP　=　MUL[]
	Fx_CPY(	(_U1*)TMP, (_U1*)inp->MUL);
	//  MUL[] - DIV[] - carry
	do
	{
		tmp.U1	=(_U1) inp->MUL[cnt].BIT.L - inp->DIV[cnt].BIT.L  - CYA.BIT.L;		
		if ( tmp.U1 >= 10 )		tmp.U1 -= 0xE6;
		CYA.BIT.L	=  tmp.BIT.H;	inp->MUL[cnt].BIT.L	=  tmp.BIT.L;
		
		tmp.U1	=(_U1) inp->MUL[cnt].BIT.H - inp->DIV[cnt].BIT.H  - CYA.BIT.L;
		if ( tmp.U1 >= 10 )		tmp.U1 -= 0xE6;
		CYA.BIT.L	=  tmp.BIT.H;	inp->MUL[cnt].BIT.H	=  tmp.BIT.L;
	}while(cnt--);	
	// carry 
	inp->ERR.U1	= CYA.U1;
	//　Returns MUL[]
	Fx_CPY(	(_U1*)inp->MUL, (_U1*)TMP);
	// carry 
	return 	inp->ERR.U1	;
}



/*! 
@brief	  	Fx_DIV
@param		U_ET32 *inp
@return		void
@par	 	

	SRC / DES = ANS   			DES 0 error

	SRC / DES = ANS				SRC = DES   ANS=1  AMA=0 

	SRC / DES = ANS				SRC < DES   ANS=0  AMA=SRC 

	SRC / DES = ANS	
	
	This division is a 16-byte array
	
	The calculation of the answer is what I learned in the elementary school
	
	Draw src [] and DES [] head to head The answer is how many times you can pull
	
	The algorithm is
	
		Carry des [] up to the maximum digit in advance
	
		This number of carry will be the end of the later calculation
	
		The answer (ANS []) is the number of times src []-= DES [] is repeated
	
		If you cannot close, carry down DES [] and carry up ANS []

	Repeat this
*/
_U1	Fx_DIV(U_ET32 *inp  )
{	
	U_H1	tmp	= {0};
	U_H1	CYA	= {0};
	_U1		cnt = SIZE-1;
	_U1		LOP = 0;
	_U1		KAI = 0;
		
	//Initial
	Fx_CLR( (_U1*) inp->ANS  );
	Fx_CLR( (_U1*) inp->REM  );
	Fx_CLR( (_U1*) inp->DIV  );
	Fx_CLR( (_U1*) inp->MUL  );
	inp->ERR.U1	= 0;
	
	
	// Work
	Fx_CPY(	(_U1*)inp->MUL , (_U1*)inp->SRC);
	Fx_CPY(	(_U1*)inp->DIV , (_U1*)inp->DES);
	
	//　Division by zero
	if ( ! Fx_ZERO( (_U1*) inp->DES ) )
	{
		inp->ERR.U1	= 0x80;					
		return 	inp->ERR.U1	;
	}
	//　X == Y　
	if ( ! Fx_EQR( (_U1*) inp->SRC ,(_U1*) inp->DES ) ) 
	{
		inp->ANS[SIZE-1].BIT.L	= 1;			
		return 	inp->ERR.U1	;
	}
	// X <　Y　
	if ( Fx_SUB_CHK( inp  ) )
	{
		Fx_CLR( (_U1*) inp->ANS  );
		Fx_CPY( (_U1*) inp->REM ,(_U1*) inp->SRC  );
		return 	inp->ERR.U1	;
	}
	//　divisor Carry Up
	while(! inp->DIV[0].BIT.H)
	{
		Fx_SFT_L((_U1*)inp->DIV );	
		LOP++;	
	}	
	//　Carry down
	do	
	{
		//　Answer Sift Left
		Fx_SFT_L((_U1*)inp->ANS );	
		//　if X >= Y　then　0
		while( ! Fx_SUB_CHK( inp )  )
		{
			//Array X[] -= Y[]
			CYA.U1 		= 0;
			tmp.U1 		= 0;	
			cnt 		= SIZE-1;
			do
			{
				tmp.U1	=(_U1) inp->MUL[cnt].BIT.L - inp->DIV[cnt].BIT.L  - CYA.BIT.L;		
				if ( tmp.U1 >= 10 )		tmp.U1 -= 0xE6;
				CYA.BIT.L	=  tmp.BIT.H;	inp->MUL[cnt].BIT.L	=  tmp.BIT.L;
	
				tmp.U1	=(_U1) inp->MUL[cnt].BIT.H - inp->DIV[cnt].BIT.H  - CYA.BIT.L;
				if ( tmp.U1 >= 10 )		tmp.U1 -= 0xE6;
				CYA.BIT.L	=  tmp.BIT.H;	inp->MUL[cnt].BIT.H	=  tmp.BIT.L;
			
			}while(cnt--);	
			// Answer addition
			inp->ANS[SIZE-1].BIT.L++;
		}
		// Divide digit down
		Fx_SFT_R((_U1*)inp->DIV );
	}while(LOP--);
	// Remaining value of subtraction
	Fx_CPY(	(_U1*)inp->REM , (_U1*)inp->MUL);
	// carry 
	inp->ERR.U1	= CYA.U1;	
	return 	inp->ERR.U1	;

}
