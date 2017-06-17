#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <limits.h>

#define DEBUG

#define TimeMeasurement
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 実行時間計測

#include <chrono>
#include <ctime>
#ifdef TimeMeasurement
	void TimeMeasurement_Start(std::chrono::system_clock::time_point& StartTime){
		StartTime = std::chrono::system_clock::now();
	}
	void TimeMeasurement_End(std::chrono::system_clock::time_point& StartTime){
		std::chrono::system_clock::time_point EndTime = std::chrono::system_clock::now();
	    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(EndTime-StartTime).count();
		auto sec = msec/1000;
		msec = msec - sec*1000;
		printf("--------------------------------\n");
		printf(" Execution time: %5ld. %03ld sec\n", sec, msec);
		printf("--------------------------------\n");
	}
#else
	void TimeMeasurement_Start(std::chrono::system_clock::time_point& StartTime){
	}
	void TimeMeasurement_End(std::chrono::system_clock::time_point& StartTime){
	}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
	#include <windows.h>						//system("pause");用
	#define SysPauseForWin32() system("pause")	//system("pause > NUL");
#else
	#define SysPauseForWin32() //空白を割り当てる．
#endif

#ifdef DEBUG
	#define printf_d(...)                                        {printf(__VA_ARGS__);}
	#define printf_dbg(...) {printf("%s(%d): ",__func__,__LINE__);printf(__VA_ARGS__);}
	#define printf_dbg_if(Boolean, ...)           if((Boolean)==true){printf_dbg(__VA_ARGS__);}
	#define printf_dbg_if_Exit(Boolean, ...)      if((Boolean)==true){printf_dbg(__VA_ARGS__);exit(-1);}
	#define printf_dbg_if_Stop_Exit(Boolean, ...) if((Boolean)==true){printf_dbg(__VA_ARGS__);SysPauseForWin32();exit(-1);}

	#define if_dbg(Code) {Code}
	#define if_dbg_rhs(Code1, Code2) if((Code1)==true){Code2}
	#define if_dbg_lhs(Code1, Code2) if((Code1)==true){Code2}
#else
	#define printf_dbg(...)
	#define printf_dbg(...)
	#define printf_dbg_if(Boolean, ...)
	#define printf_dbg_if_Exit(Boolean, ...)
	#define printf_dbg_if_Stop_Exit(Boolean, ...)

	#define if_dbg(Code)
	#define if_dbg_rhs(Code1, Code2) (Code1)
	#define if_dbg_lhs(Code1, Code2) (Code2)
#endif

// 常時エラーを出力する必要がある場合
#define printf_ErrMsg(...) {printf("%s(%d): ", __func__, __LINE__); printf(__VA_ARGS__);}
#define printf_ErrMsg_Stop_Exit(...) {printf_ErrMsg(__VA_ARGS__);SysPauseForWin32();exit(-1);}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define FOR(i,k,n) for (unsigned int (i)=(k); (i)<(unsigned int)(n); (i)++)
#define FOReq(i,k,n) for (unsigned int (i)=(k); (i)<=(unsigned int)(n); (i)++)
#define REP(i,n) FOR(i,0,n)
#define REPeq(i,n) FOReq(i,0,n)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#define ALL(x) (x).begin(),(x).end()

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define printv(std_vector)\
	for(auto itr=std_vector.begin(); itr!=std_vector.end(); itr++){std::cout<<*itr<<" ";}std::cout<<std::endl;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
	#define FOPEN_EQ(fp,FileName,option) (fopen_s((fp),(FileName),(option))!=NULL)
	#define fscanf fscanf_s
#else
	#define FOPEN_EQ(fp,FileName,option) ((*(fp)=fopen((FileName),(option)))==NULL)
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// MartixStore

namespace mxs{

	template <typename type> class MatrixStore;

	template <typename type> void zeros(class MatrixStore<type>& Mat);

	template <typename type> class MatrixStore<type>* CopyRect(class MatrixStore<type>*& pMat, unsigned int RowStart, unsigned int RowEnd, unsigned int ColStart, unsigned int ColEnd);

	void printMatrixStore(class MatrixStore         <char>& Mat);
	void printMatrixStore(class MatrixStore<unsigned char>& Mat);
	void printMatrixStore(class MatrixStore          <int>& Mat);
	void printMatrixStore(class MatrixStore <unsigned int>& Mat);
	void printMatrixStore(class MatrixStore        <float>& Mat);
	void printMatrixStore(class MatrixStore       <double>& Mat);
};

//--------------------------------------------------------------------------------------------------------

template <typename type>
class OpCopyCol{
private:
public:
	OpCopyCol();
	~OpCopyCol();

	class mxs::MatrixStore<type>* pMS;
	unsigned int CopyColNum;

	class OpCopyCol<type> operator=(class OpCopyCol<type> CC);
};
template <typename type>inline OpCopyCol<type>::OpCopyCol(){}
template <typename type>inline OpCopyCol<type>::~OpCopyCol(){}


template <typename type>
inline class OpCopyCol<type> OpCopyCol<type>::operator=(class OpCopyCol<type> CC)
{
	for(unsigned int p=0; p<this->pMS->rows; p++){
		(*(this->pMS))(p, this->CopyColNum)
		 = (*(CC.pMS))(p,    CC.CopyColNum);
	}
	return *this;
}

//--------------------------------------------------------------------------------------------------------

template <typename type>
class OpCopyRow{
private:
public:
	OpCopyRow();
	~OpCopyRow();

	class mxs::MatrixStore<type>* pMS;
	unsigned int CopyRowNum;

	class OpCopyRow<type> operator=(class OpCopyRow<type> CR);
};
template <typename type>inline OpCopyRow<type>::OpCopyRow(){}
template <typename type>inline OpCopyRow<type>::~OpCopyRow(){}


template <typename type>
inline class OpCopyRow<type> OpCopyRow<type>::operator=(class OpCopyRow<type> CR)
{
	for(unsigned int q=0; q<this->pMS->cols; q++){
		(*(this->pMS))(this->CopyRowNum, q)
		 = (*(CR.pMS))(   CR.CopyRowNum, q);
	}
	return *this;
}

//--------------------------------------------------------------------------------------------------------

template <typename type>
class mxs::MatrixStore{
private:
public:
	type* MatX;
	unsigned int rows;	// 行数
	unsigned int cols;	// 列数

	MatrixStore(unsigned int row, unsigned int col);
	~MatrixStore();

	type& operator()(unsigned int p, unsigned int q);
	type& operator()(         int p, unsigned int q);
	type& operator()(unsigned int p,          int q);
	type& operator()(         int p,          int q);

	class OpCopyCol<type> operator()(const char c_dummy, unsigned int q);
	class OpCopyCol<type> operator()(const char c_dummy,          int q);

	class OpCopyRow<type> operator()(unsigned int p, const char c_dummy);
	class OpCopyRow<type> operator()(         int p, const char c_dummy);
};
template <typename type>
inline mxs::MatrixStore<type>::MatrixStore(unsigned int row, unsigned int col){
	rows = row;
	cols = col;
	MatX = (type*)malloc(sizeof(type)*row*col);
	printf_dbg_if_Stop_Exit(MatX==0, "ERROR: This pointer is not allocated.\n");
}
template <typename type>
inline mxs::MatrixStore<type>::~MatrixStore(){
	free(MatX);
}

template <typename type> inline type& mxs::MatrixStore<type>::operator()(unsigned int p, unsigned int q){ return *(type*)(&MatX[              p + rows*              q]); }
template <typename type> inline type& mxs::MatrixStore<type>::operator()(         int p, unsigned int q){ return *(type*)(&MatX[(unsigned int)p + rows*              q]); }
template <typename type> inline type& mxs::MatrixStore<type>::operator()(unsigned int p,          int q){ return *(type*)(&MatX[              p + rows*(unsigned int)q]); }
template <typename type> inline type& mxs::MatrixStore<type>::operator()(         int p,          int q){ return *(type*)(&MatX[(unsigned int)p + rows*(unsigned int)q]); }

//--------------------------------------------------------------------------------------------------------

// 行のコピー, 例: MatB(1, ':') = MatB(0, ':') = MatA(0, ':');
template <typename type>
inline class OpCopyRow<type> CopyRow(mxs::MatrixStore<type>* pthis, unsigned int p, const char& c_dummy)
{
	class OpCopyRow<type> CR;
	CR.pMS = pthis;
	CR.CopyRowNum = p;

	return CR;
}
template <typename type> inline class OpCopyRow<type> mxs::MatrixStore<type>::operator()(unsigned int p, const char c_dummy){ return CopyRow<type>(this, p, c_dummy); }
template <typename type> inline class OpCopyRow<type> mxs::MatrixStore<type>::operator()(         int p, const char c_dummy){ return CopyRow<type>(this, p, c_dummy); }


// 列のコピー, 例: MatB(':', 1) = MatB(':', 0) = MatA(':', 0);
template <typename type>
inline class OpCopyCol<type> CopyCol(mxs::MatrixStore<type>* pthis, const char& c_dummy, unsigned int q)
{
	class OpCopyCol<type> CC;
	CC.pMS = pthis;
	CC.CopyColNum = q;

	return CC;
}
template <typename type> inline class OpCopyCol<type> mxs::MatrixStore<type>::operator()(const char c_dummy, unsigned int q){ return CopyCol<type>(this, c_dummy, q); }
template <typename type> inline class OpCopyCol<type> mxs::MatrixStore<type>::operator()(const char c_dummy,          int q){ return CopyCol<type>(this, c_dummy, q); }

//--------------------------------------------------------------------------------------------------------

template <typename type>
inline void mxs::zeros(class MatrixStore<type>& Mat){
	for(unsigned int q=0; q<Mat.cols; q++){
		for(unsigned int p=0; p<Mat.rows; p++){
			Mat(p, q) = (type)0;
		}
	}
}

//--------------------------------------------------------------------------------------------------------

template <typename type>
inline class mxs::MatrixStore<type>* mxs::CopyRect(class mxs::MatrixStore<type>*& pMat, unsigned int RowStart, unsigned int RowEnd, unsigned int ColStart, unsigned int ColEnd){

	class mxs::MatrixStore<type>* pMatBuf = new mxs::MatrixStore<type>(RowEnd-RowStart, ColEnd-ColStart);

	for(unsigned int q=0; q<(ColEnd-ColStart); q++){
		for(unsigned int p=0; p<(RowEnd-RowStart); p++){
			(*pMatBuf)(p, q) = (*pMat)(p+RowStart, q+ColStart);
		}
	}

	return pMatBuf;
}

//--------------------------------------------------------------------------------------------------------

//void mxs::printMatrixStore(class MatrixStore<char>& Mat){
//	for(unsigned int p=0; p<Mat.rows; p++){
//		unsigned int q=0;
//		printf("%d", Mat(p, q));
//		for(q++; q<Mat.cols; q++){
//			printf("\t %d", Mat(p, q));
//		}
//		printf("\n");
//	}
//}
void mxs::printMatrixStore(class MatrixStore<char>& Mat){
	for(unsigned int p=0; p<Mat.rows; p++){
		unsigned int q=0;
		printf("%c", Mat(p, q));
		for(q++; q<Mat.cols; q++){
			printf("\t %c", Mat(p, q));
		}
		printf("\n");
	}
}
void mxs::printMatrixStore(class MatrixStore<unsigned char>& Mat){
	for(unsigned int p=0; p<Mat.rows; p++){
		unsigned int q=0;
		printf("%d", Mat(p, q));
		for(q++; q<Mat.cols; q++){
			printf("\t %d", Mat(p, q));
		}
		printf("\n");
	}
}
void mxs::printMatrixStore(class MatrixStore<int>& Mat){
	for(unsigned int p=0; p<Mat.rows; p++){
		unsigned int q=0;
		printf("%d", Mat(p, q));
		for(q++; q<Mat.cols; q++){
			printf("\t %d", Mat(p, q));
		}
		printf("\n");
	}
}
void mxs::printMatrixStore(class MatrixStore<unsigned int>& Mat){
	for(unsigned int p=0; p<Mat.rows; p++){
		unsigned int q=0;
		printf("%u", Mat(p, q));
		for(q++; q<Mat.cols; q++){
			printf("\t %u", Mat(p, q));
		}
		printf("\n");
	}
}
void mxs::printMatrixStore(class MatrixStore<float>& Mat){
	for(unsigned int p=0; p<Mat.rows; p++){
		unsigned int q=0;
		printf("%f", Mat(p, q));
		for(q++; q<Mat.cols; q++){
			printf("\t %f", Mat(p, q));
		}
		printf("\n");
	}
}
void mxs::printMatrixStore(class MatrixStore<double>& Mat){
	for(unsigned int p=0; p<Mat.rows; p++){
		unsigned int q=0;
		printf("%lf", Mat(p, q));
		for(q++; q<Mat.cols; q++){
			printf("\t %lf", Mat(p, q));
		}
		printf("\n");
	}
}

//--------------------------------------------------------------------------------------------------------

#define printm(var)\
	printf("%s = \n",(#var));mxs::printMatrixStore((var))

//--------------------------------------------------------------------------------------------------------

/*

	How to use this?

int main(){

	mxs::MatrixStore<double> MatA(3, 3); mxs::zeros(MatA);
	mxs::MatrixStore<double> MatB(3, 3); mxs::zeros(MatB);

	for(unsigned int q=0; q<MatA.cols; q++){
		for(unsigned int p=0; p<MatA.rows; p++){
			MatA(p, q) = (MatA.cols)*p + q;
		}
	}

	printm(MatA); printf("\n");

	MatB(':', 0) = MatA(':', 1);
//	MatB(':', 1) = MatA(':', 2);
//	MatB(':', 2) = MatA(':', 0);

	printm(MatB); printf("\n");

//	MatB(0, ':') = MatA(0, ':');
	MatB(1, ':') = MatB(0, ':') = MatA(0, ':');
//	MatB(1, ':') = MatA(1, ':');
//	MatB(2, ':') = MatA(2, ':');

	printm(MatB);

	return 0;
}

 */

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
//	typedef __int8 INT8;
	typedef __int16 INT16;
	typedef __int32 INT32;
	typedef __int64 INT64;

	typedef unsigned __int8 UINT8;
	typedef unsigned __int16 UINT16;
	typedef unsigned __int32 UINT32;
	typedef unsigned __int64 UINT64;
#else

	#include <stdint.h>		// intXX_t, uintXX_t が定義されている

	typedef int8_t INT8;
	typedef int16_t INT16;
	typedef int32_t INT32;
	typedef int64_t INT64;

	typedef uint8_t UINT8;
	typedef uint16_t UINT16;
	typedef uint32_t UINT32;
	typedef uint64_t UINT64;
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//using namespace std;
//using namespace mxs;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//http://poj.org/problem?id=2386

//蟻本p35
//http://d.hatena.ne.jp/aomori-ringo2/20101105/1288941874
//http://d.hatena.ne.jp/liqz/20120329/1333007357
//https://eagletmt.github.io/contests/blog/poj-2386/

//#define UseStdIn
//#define UseStdOut
//#define UseTestData


void read(FILE* FpIn, mxs::MatrixStore<char>& MatX){
	REP(p, MatX.rows){
		REP(q, MatX.cols){
			fscanf(FpIn, "%c", &(MatX(p, q)));
		}
		fscanf(FpIn, "\n");
	}
}


void DFS(mxs::MatrixStore<char>& MatX, unsigned int p, unsigned int q){

	MatX(p, q) = '.';

	for(int dq=q-1; dq<=(int)q+1; dq++){
		for(int dp=p-1; dp<=(int)p+1; dp++){
			if( dp>0 && dq>0 ){
				if( MatX(dp, dq)=='W' ){
					MatX(dp, dq) = '.';
					DFS(MatX, dp, dq);
				}
			}
		}
	}
}


unsigned int solve(mxs::MatrixStore<char>& MatX){

	unsigned int result = 0;

	REP(q, MatX.cols){
		REP(p, MatX.rows){
			if(MatX(p, q)=='W'){
				result++;
				DFS(MatX, p, q);
			}
		}
	}

	return result;
}


int main(){

	std::chrono::system_clock::time_point TimeBuf;
	TimeMeasurement_Start(TimeBuf);

	// -------------------------------------------------------------------------------------------------

	FILE* FpIn;
#ifdef UseStdIn
	FpIn = stdin;
#else
	#ifdef UseTestData
		const char* PathIn = R"(./input/XXXXXXXXXXXXXX.txt)";
	#else
		const char* PathIn = R"(./input/POJ-2386-input.txt)";
	#endif
	if_dbg_rhs(FOPEN_EQ(&FpIn, PathIn, "r"), printf_ErrMsg("FOPEN was failed.\n"); return -1;);
#endif

	// -------------------------------------------------------------------------------------------------

	FILE* FpOut;
#ifdef UseStdOut
	FpOut = stdout;
#else
	#ifdef UseTestData
		const char* PathOut = R"(./XXXXXXXXXXXXXXX.txt)";
	#else
		const char* PathOut = R"(./POJ-2386-output.txt)";
	#endif
	if_dbg_rhs(FOPEN_EQ(&FpOut, PathOut, "w"), printf_ErrMsg("FOPEN was failed.\n"); return -1;);
#endif

	// -------------------------------------------------------------------------------------------------

	unsigned int N, M;
	fscanf(FpIn, "%u %u\n", &N, &M); printf_dbg("N: %u, M: %u\n", N, M);
	mxs::MatrixStore<char> MatX(N, M); mxs::zeros(MatX);

	read(FpIn, MatX);
	printm(MatX);

	unsigned int result = solve(MatX);
	printm(MatX);

	printf("result: %d\n", result);

	// -------------------------------------------------------------------------------------------------

	fclose(FpIn );
	fclose(FpOut);
	TimeMeasurement_End(TimeBuf);
	SysPauseForWin32();
	return 0;
}