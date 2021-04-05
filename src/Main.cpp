#include "INT.h"
#include "REAL.h"
#include <fstream>
using namespace zju04nycs;
using namespace std;

#define NPV(x) cout<<(#x)<<":"<<(x)<<endl;

void Print(int n=30)
{ 
	cout<<endl;
	for( int i=0;i<n;i++)
	{
		cout<<'*';
	}
	cout<<endl;

}
void Far(int k,ostream &os = cout)
{
	static REAL R = 1;
	static int n = 0;
	if( n > k)
	{
		n = 0;
	}	
	for( int i = n ;i<k;++i)
	{
		R *= (i+1);
	}
	n = k;
	os<<k<<"!:"<<R<<endl;
	
}

void TestPlus(const REAL & N,ostream &os = cout)
{
	REAL S = 0;
	for( REAL I = 1;I<=N; I += 1)
	{
		S += I;
		assert( S == I*(I+1)>>1);
	//	os<<"S("<<I<<")= "<<S<<endl;
	}
}

REAL R1,R2,R;

void Test1()
{
	ofstream out(L"╝Мие▓Р╩н1.txt");
	NPV( REAL(1) >> 4);
	REAL::DisWidth(100);
	do
	{
		cin>>R;
		cout<<"D:"<<R<<endl;
		REAL::DisBase('b');
		cout<<"B"<<R<<endl;
		REAL::DisBase('h');
		cout<<"H:"<<R<<endl;
		REAL::DisBase('D');

	}while( R != 0);
}
void Test2()
{
	REAL::DisWidth(100);
	do
	{
		cout<<"D"<<endl;
		REAL::DisBase('D');
		cin>>R;
		cout<<"D"<<R<<endl;
		REAL::DisBase('b');
		cout<<"B"<<R<<endl;
		REAL::DisBase('h');
		cout<<"H:"<<R<<endl;

		cout<<"B"<<endl;
		REAL::DisBase('b');
		cin>>R;
		cout<<"B"<<R<<endl;
		REAL::DisBase('d');
		cout<<"D"<<R<<endl;
		REAL::DisBase('h');
		cout<<"H:"<<R<<endl;

		cout<<"H"<<endl;
		REAL::DisBase('H');
		cin>>R;
		cout<<"h"<<R<<endl;
		REAL::DisBase('d');
		cout<<"D"<<R<<endl;
		REAL::DisBase('b');
		cout<<"b:"<<R<<endl;

	}while( R!=0);
}

void e(int p = 1000)
{
	REAL Ten_e_p = 1;
	const REAL Ten = 10;
	for( int i = 0;i<p;++i)
	{
		Multiply(REAL::TMP = Ten_e_p,Ten,Ten_e_p);
	}
	int nP = Ten_e_p.Lead();
	int n = 1;
	REAL FAC = 1;
	REAL FAC_N = 1;
	const REAL One;
	while( FAC_N.Lead() < nP+1)
	{
		++n;
		FAC *= n;
		FAC_N = FAC * n;
	}
	int k = - nP - 1 - REAL(n-2).Lead();
	FAC = 1;
	REAL E = 2;
	int r;
	for( int i = 2; i<= n;++i)
	{
		Multiply(REAL::TMP = FAC,i,FAC);
		Plus( REAL::TMP = E,Reverse(FAC,REAL::DIV,k,r),E);
	}
	REAL::DisWidth(p + 2);
	NPV(E);
}
REAL e_m_k(int m,int Tm)
{
	REAL E = 1;
	int n = 1;
	REAL M = 2;
	const REAL One;
	while( M.Lead() < m*(n+1) - Tm + 3)
	{
		++n;
		M*=(n+1);
	}
	int k = Tm - 1 - REAL(n-1).Lead();
	M = 1;
	int r;
	for( int i = 1; i<= n;++i)
	{
		Multiply(REAL::TMP = M,i,M);
		Reverse(M,REAL::DIV,k-m*i,r);
		Plus(REAL::TMP = E,REAL::DIV <<= i*m,E);
	}
	return E;

}

REAL e_spin( int m ,int Tm,int Em)
{
	if( m - 1 == Em)
	{
		REAL R = e_m_k(m-1,Tm - 3);
	//	return R*R.Hold(Tm-1);
		return (R*R).Hold(Tm-1);
	}
	else
	{
		REAL R = e_spin(m-1,Tm-3,Em);
		return (R*R).Hold(Tm-1);
	//	return R*R.Hold(Tm-1);
	}
}

void e1(ostream &os = cout)
{
	do
	{
		int p,m;
		cin>>p>>m;
		REAL Ten_e_p = 1;
		const REAL Ten = 10;
		for( int i = 0;i<p+1;++i)
		{
			Multiply(REAL::TMP = Ten_e_p,Ten,Ten_e_p);
		}
		int nP = Ten_e_p.Lead();
		REAL::DisWidth(p + 2);
		REAL::DisFill(true);
		os<<"e("<<p<<")= "<<(REAL::TMP = e_spin(0,-nP,m))<<endl;
//		cout<<"e("<<p<<")= "<<REAL::TMP<<endl;
	}while(1);
}

void Test3()
{
	NPV(REAL(1,-5));
	NPV(REAL(1,-4));
	NPV(REAL(1,-3));
	NPV(REAL(1,-2));
	NPV(REAL(1,-1));
	NPV(REAL(1,0));
	NPV(REAL(1,1));
	NPV(REAL(1,2));
	NPV(REAL(1,3));
	NPV(REAL(1,4));
	NPV(REAL(1,5));
	NPV(REAL(1,6));
}
void Test4()
{
	///	Test2();
	int k;
	do
	{
		cin>>k;
		e(k);
	}while(k);

}
void Test5()
{
	int k;
	REAL::DisWidth(1000);
	do
	{
		cin>>k;
		Far(k);
	}while(k);
}
void Test6()
{
	REAL R;
	do
	{
		cin>>R;
		NPV(R);
	}while(R!=0);
}
REAL Test7_f(REAL x)
{
	return x*x*x - 2*x -5;
}
REAL Test7_f1(REAL x)
{
	return 3*x*x - 2;
}
void Test7()
{
	REAL (*f)(REAL);
	REAL (*f1)(REAL);
	f = Test7_f;
	f1 = Test7_f1;
	REAL a =2;
	REAL b;
	StringToReal("2.1",b);
	NPV(a);
	NPV(b);
	NPV(f(a));
	NPV(f(b));
	NPV(f1(a));
	NPV(f1(b));
	REAL m = f1(a);
	REAL M;
	StringToReal("12.6",M);
	NPV(M/(2*m));
	REAL k = M/(2*m);
	REAL dX = f(b)/m;
	REAL::DisWidth(13);
	NPV(dX);
	NPV(k*dX*dX);
	REAL x1 = b - f(b)/f1(b);
	NPV(x1);
	StringToReal("2.09456",x1);
	NPV(x1);
	NPV(f(x1));
	NPV(f(x1)/m);
	REAL x2 = x1 - f(x1)/f1(x1);
	NPV(x2);

}
void Test8()
{
	REAL::DisWidth(50);
	Far(10);
	Far(1000);
}

REAL Test9_f(const REAL &x)
{
	return 1/(1+x*x);
}
void Test9()
{

	REAL (*f)(const REAL &);
	f = Test9_f;

	int n = 10;
	REAL a = 0,b = 1;
	REAL Xi,Yi,S = 0;
	REAL::DisWidth(4);
	for( int i = 0; i<n;++i)
	{
		Xi = a + (b-a)*(1+2*i)/(2*n);
		NPV(Xi);
		NPV(f(Xi));
		S += f(Xi);
	}
	REAL::DisWidth(5);
	NPV(S*(b-a)/n);
	Print();

	S = 0;
	Xi = a;
	NPV(Xi);
	Yi = f(Xi);
	NPV(Yi);
	S += Yi;

	Xi = b;
	NPV(Xi);
	Yi = f(Xi);
	NPV(Yi);
	S += Yi;
	NPV(S);
	NPV(S/=2);
	Print();

	REAL S1 = 0;
	for( int i = 1; i<= n-1; ++i)
	{
		Xi = a + (b-a)*i/n;
		NPV(Xi);
		Yi = f(Xi);
		NPV(Yi);
		S1+=Yi;
	}
	NPV(S1);
	NPV( S += S1);
}

REAL Test10_f(const REAL &x)
{
	return 1/(1+x*x);
}
void Test10()
{

	REAL (*f)(const REAL &);
	f = Test10_f;

	int n = 2;
	REAL a = 0,b = 1;
	REAL Xi,Yi,S = 0;
	REAL::DisWidth(4);

	Xi = a;
	NPV(Xi);
	NPV(Yi = f(Xi));
	S += Yi;
	Xi = b;
	NPV(Xi);
	NPV(Yi = f(Xi));
	S += Yi;
	Print();

	for( int i = 1;i<=n-1;++i)
	{
		NPV(Xi = a + (b-a)*i/n);
		NPV(Yi = f(Xi));
		S += 2*Yi;
	}
	Print();

	for( int i =0;i<=n-1;++i)
	{
		NPV(Xi = a + (b-a)*(1+2*i)/(2*n));
		NPV(Yi = f(Xi));
		S += 4*Yi;
	}
	Print();

	REAL::DisWidth(5);
	NPV(S = S*(b-a)/(6*n));
}

void Test11()
{
	REAL a = 1,b=2;
	int n = 10;
	REAL Xi,Yi;
	REAL S = 0;
	REAL::DisWidth(5);
	for( int i = 0;i<n;++i)
	{
		Xi = a + (b-a)*(1+2*i)/(2*n);
		NPV(Xi);
		NPV(Yi = 1/Xi);
		S += Yi;
	}
	NPV(S);
	NPV(S*(b-a)/n);

	Print();
	Print();

	S = 0;
	Xi = a;
	NPV(Xi);
	NPV(Yi = 1/Xi);
	S += Yi;

	Xi = b;
	NPV(Xi);
	NPV(Yi = 1/Xi);
	S += Yi;

	NPV(S);
	Print();
	REAL S1 = 0;
	for( int i = 1;i<=n-1;++i)
	{
		Xi = a + (b-a)*i/n;
		NPV(Xi);
		NPV(Yi = 1/Xi);
		S1 += Yi;
	}
	NPV(S1);
	NPV( S = S/2 + S1);
	NPV( S*(b-a)/n);
	Print();
	Print();

	REAL::DisWidth(6);
	NPV(2.0/(15*(2*4)*(2*4)*(2*4)*(2*4)));
	NPV(2.0/(15*(2*5)*(2*5)*(2*5)*(2*5)));
	Print();
	
	n = 5;

	S = 0;
	for( int i = 1;i<=n-1;++i)
	{
		Xi = a + (b-a)*i/n;
		NPV(Xi);
		NPV(Yi = 1/Xi);
		S += Yi;
	}
	NPV(S);
	Print();

	S1 = 0;
	Xi = a;
	NPV(Xi);
	NPV(Yi = 1/Xi);
	S1 += Yi;

	Xi = b;
	NPV(Xi);
	NPV(Yi = 1/Xi);
	S1 += Yi;
	NPV(S1);
	Print();
	REAL S2;
	for( int i = 0;i<n;++i)
	{
		Xi = a + (b-a)*(1+2*i)/(2*n);
		NPV(Xi);
		NPV(Yi = 1/Xi);
		S2 += Yi;
	}
	NPV(S2);
	Print();

	NPV(S*2);
	NPV(S1);
	NPV(S2*4);
	NPV( S = S*2 + S1 + S2*4);
	REAL::DisWidth(10);
	NPV( S*(b-a)/(6*n));



	
}

void GetPi(int nPrec,REAL &Pi)
{
	int n = 1;
	REAL Five_p = 125;

	while( ((2*n+1)*Five_p).Lead() < 7-nPrec)
	{
		++n;
		Five_p *= 25;
	}
	int k = nPrec - 2 - REAL((n+1)/2).Lead();

	Pi = 0;
	REAL Term = 5;
	REAL Five_p_2 = 25;
	int r;
	for( int i = 1; i<= n; ++i)
	{
		Divide(16,Multiply(Term,(2*i-1),REAL::MUL),REAL::DIV,k,r);
		if( i % 2 == 0)
		{
			REAL::DIV.Minus();
		}
		Plus(REAL::TMP = Pi,REAL::DIV,Pi);
		Multiply(REAL::TMP = Term,Five_p_2,Term);
	}

	Term = 239;
	n = 1;
	REAL N_p = 239*239*239;
	while( ((2*n+1)*N_p).Lead() < 5-nPrec)
	{
		++n;
		N_p *= 239*239;
	}
	k = nPrec - 2 - REAL((n+1)/2).Lead();
	REAL N_p_2 = 239*239;

	for( int i = 1;i<=n;++i)
	{
		Divide(4,Multiply(Term,(2*i-1),REAL::MUL),REAL::DIV,k,r);
		if( i % 2 == 1)
		{
			REAL::DIV.Minus();
		}
		Plus(REAL::TMP = Pi,REAL::DIV,Pi);
		Multiply(REAL::TMP = Term,N_p_2,Term);
	}


}
void EvaluatePi()
{
	int W;
	cin>>W;
	W += 2;
	REAL Ten_p = 1;
	for( int i = 0; i<W;++i)
	{
		Ten_p *= 10;
	}
	int nPrec = -Ten_p.Lead();
	REAL Pi;
	GetPi(nPrec,Pi);
	REAL::DisWidth(W+1);
	cout<<"Pi("<<W-2<<")="<<Pi<<endl;
}

void testInt() {
	INT n;
	assert(n.isZero());
	assert(!n.isPositive());
	assert(!n.isNegative());
	cout << "zero test passed" << std::endl;

	INT one = 1;
	INT one_ = INT("1");
	assert(one == one_);
	assert(!(one != one_));
	cout << "one test passed" << std::endl;

	std::string s1 = "12345";
	INT x1 = INT(s1);
	std::string x1_s = x1.toString();
	std::cout << "x1:" <<  x1_s << std::endl;
	assert(x1_s == s1);
	cout << "simple string constructor test passed" << std::endl;

	long long p2 = 123456789012345333LL;
	INT x2 = INT(p2);
	auto x2_s = x2.toString();
	std::cout << "x2:" << x2_s << std::endl;
	std::cout << "p2:"<< p2 << std::endl;
	assert(std::to_string(p2) == x2_s);
	cout << "long long constructor test passed" << std::endl;

	INT x3 = INT("123456789012345333");
	std::cout << "x3:" << x3.toString() << std::endl;

	std::string s4 = "12345678901234";
	INT x4 = INT(s4);
	auto x4_s = x4.toString();
	std::cout << "x4:" << x4_s << std::endl;
	assert(x4_s == s4);
	cout << "string constructor test4 passed" << std::endl;

	std::string s5 = "32433443423434783432432423423423423423423423423423423423472364723647236476274623764723674623746764237647264723476767236472364782374827384723478324324324243343243243242342432423432434";
	INT x5 = INT(s5);
	std::cout << "x5:" << x5.toString() << std::endl;
	assert(x5.toString() == s5);
	cout << "long string constructor test5 passed" << std::endl;

	std::cout << "x4*x5:" << (x4 * x5).toString() << std::endl;
	std::cout << "x5*x4:" << (x5 * x4).toString() << std::endl;

	assert(x4 * x5 == x5 * x4);
	assert((x4 * x5).toString() == (x5 * x4).toString());
	cout << "multiplication communication law test passed" << std::endl;

	std::string s6 = "400412878166003164546735534092609172319140330852536603355429981136254443797343719815103359446869847213482227876740797726949481901457564006563326790056796257840594244649848624543981509868730249924";
	INT x6 = INT(s6);
	auto x6_s = x6.toString();
	cout << "x6:" << x6_s << endl;
	assert(x6_s == s6);
	cout << "long string constructor test 6 passed" << endl;

	INT N = INT("100");
	INT sum;
	for (INT i = 0; i <= N; i = i + 1) {
		sum += i;
	}
	auto sum_s = sum.toString();
	std::cout << "sum:" << sum_s << std::endl;
	assert(sum == N * (N + 1) / 2);
	cout << "sum test 1 passed" << endl;

	INT N2 = INT("200000");
	INT sum2;
	for (INT i = 0; i <= N2; i = i + 1) {
		sum2 += i;
	}
	auto sum2_s = sum2.toString();
	std::cout << "sum2:" << sum2_s << std::endl;
	assert(sum2 == N2 * (N2 + 1) / 2);
	cout << "sum test 2 passed" << endl;

	std::string s7 = "101010101010101101010010101";
	INT x7 = INT(s7, 2);
	auto x7_s = x7.toString(2);
	cout << "x7:" << x7_s << std::endl;
	assert(x7_s == s7);
	cout << "base 2 string construtor test passed\n";

	INT y7 = x7 ^ x7;
	cout << "y7:" << y7.toString() << std::endl;
	assert(y7.isZero());
	cout << "xor test7 passed" << endl;
}

int main()
{
	///	Test2();
	//Test6();
	//e1();
	//Test7();
	//Test8();
	//Test9();
	//Test10();
	//Test11();
	//EvaluatePi();
	testInt();
	return 0;
	

}