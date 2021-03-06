#include "cmatrix.h"
#include <math.h>
#include <stdarg.h>
#include <cstdlib>

CMatrix::CMatrix()
{
	nR = nC = 0; values = NULL;
}
CMatrix::~CMatrix()
{
	reset();
}
CMatrix::CMatrix(int nR, int nC, int initialization, double initializationValue) // Beshoy
{
	this->nR = nR;
	this->nC = nC;
	if ((nR*nC) == 0)
	{
		values = NULL; return;
	}
	values = new double*[nR];
	for (int iR = 0; iR < nR; iR++)
	{
		values[iR] = new double[nC];
		for (int iC = 0; iC < nC; iC++)
		{
			switch (initialization)
			{
			case MI_ZEROS: values[iR][iC] = 0;
				break;
			case MI_ONES: values[iR][iC] = 1;
				break;
			case MI_EYE: values[iR][iC] = (iR == iC) ? 1 : 0;
				break;
			case MI_RAND: values[iR][iC] = (rand() % 100);// / 100.0;
				break;
			case MI_VALUE: values[iR][iC] = initializationValue;
				break;
			}
		}
	}
}
CMatrix::CMatrix(int nR, int nC, double first, ...) // Beshoy
{
	this->nR = nR;
	this->nC = nC;
	if ((nR*nC) == 0)
	{
		values = NULL; return;
	}
	values = new double*[nR];
	va_list va;
	va_start(va, first);
	for (int iR = 0; iR < nR; iR++)
	{
		values[iR] = new double[nC];
		for (int iC = 0; iC < nC; iC++)
		{
			values[iR][iC] = (iC == 0 && iR == 0) ? first : va_arg(va, double);
		}
	}
	va_end(va);
}
CMatrix::CMatrix(CMatrix& m) // Beshoy
{
	nR = nC = 0;
	values = NULL;
	copy(m);
}
CMatrix::CMatrix(string s) // Beshoy
{
	nR = nC = 0;
	values = NULL;
	copy(s);
}
CMatrix::CMatrix(double d) // Beshoy
{
	nR = nC = 0;
	values = NULL;
	copy(d);
}

void CMatrix::reset() { // Boula
	if (values) {
		for (int i = 0; i < nR; i++)
			delete[] values[i];
		delete[] values;
	}
	nR = nC = 0;
	values = NULL;
}
void CMatrix::copy(const CMatrix& m) { // Boula
	reset();
	this->nR = m.nR;
	this->nC = m.nC;
	if ((nR*nC) == 0) { values = NULL; return; }
	values = new double*[nR];
	for (int iR = 0; iR < nR; iR++) {
		values[iR] = new double[nC];
		for (int iC = 0; iC < nC; iC++) values[iR][iC] = m.values[iR][iC];
	}
}
void CMatrix::copy(double d) { // Boula
	reset();
	this->nR = 1;
	this->nC = 1;
	values = new double*[1];
	values[0] = new double[1];
	values[0][0] = d;
}
#include <iostream>
void CMatrix::copy(string s) { // Boula
	reset();
	char buffer;

	// Cuts the string from [ to ]
	int indicator1 = s.find('[');
	int indicator2 = s.find(']');
	s = s.substr(indicator1, indicator2 + 1);

	string temp_matrix[100][100]; // this limits the matrix to 100*100
	int c = 0, nr = 0, nc = 0;
	bool trueLine = false;
	bool trueColoum = false;
	for (unsigned int i = 0; i < s.length(); i++) {
		buffer = s[i];
		if (buffer == ' ' && temp_matrix[nr][c].length() != 0) { c++; trueColoum = false; if (c > nc) nc = c; }
		else if (buffer == ';') { if (!trueColoum) nc--; nr++; trueLine = false; c = 0; }
		else if (buffer == '[' || buffer == ']' || buffer == ' ');
		else {
			trueLine = true;
			trueColoum = true;
			temp_matrix[nr][c] += buffer;
		}
	}
	if (!trueLine) nr--;
	if (!trueColoum) nc--;
	nR = nr + 1; nC = nc + 1;
	values = new double*[nR];
	for (int iR = 0; iR < nR; iR++)
	{
		values[iR] = new double[nC];
		for (int iC = 0; iC < nC; iC++)
		{
			values[iR][iC] = atof(temp_matrix[iR][iC].c_str());
		}
	}
}
#include <sstream>
/*template <typename T> string to_string(const T& t) {
ostringstream os;
os << t;
return os.str();
}*/
/*
string to_string(const int& t) {
	ostringstream os;
	os << t;
	return os.str();
}
string to_string(const double& t) {
	ostringstream os;
	os << t;
	return os.str();
}*/
/*string to_string_with_precision(double value, int n = 6)
{
string sValue = to_string(value);
if (value < 0) {
value *= -1;
n++;
}
if (value>99999999.99999999) sValue = sValue.substr(0, n + 10);
else if (value>9999999.99999999) sValue = sValue.substr(0, n + 9);
else if (value>999999.99999999) sValue = sValue.substr(0, n + 8);
else if (value>99999.99999999) sValue = sValue.substr(0, n + 7);
else if (value>9999.99999999) sValue = sValue.substr(0, n + 6);
else if (value>999.99999999) sValue = sValue.substr(0, n + 5);
else if (value>99.99999999) sValue = sValue.substr(0, n + 4);
else if (value>9.99999999) sValue = sValue.substr(0, n + 3);
else sValue = sValue.substr(0, n + 2);
return sValue;
}*/
#include <stdio.h>
//#include <time.h>
string CMatrix::getOriginalString() { // Boula
	string s = "[";
	for (int iR = 0; iR < nR; iR++) {
		for (int iC = 0; iC < nC; iC++) {
			s += " ";
			//s += to_string_with_precision(values[iR][iC], 2);
			s += to_string(values[iR][iC]);
		}
		s += ";";
	}
	s = "[" + s.substr(2, s.length() - 3) + "]";
	return s;
}
string CMatrix::getString(const int columnsToPrintEachTime) { // Boula
															  // support up to 99*99 matrix
	if (nR == 0 || nC == 0) return "\n\t[empty]\n";

	int columnSize = 20 - 2 * columnsToPrintEachTime;
	int printedColumns = 0;
	string s = "\n";
	bool biggerThanSix = nC > columnsToPrintEachTime;
	while (printedColumns < nC) {
		if (biggerThanSix && ((nC - printedColumns) >= columnsToPrintEachTime)) s += "\tColumns " + to_string(printedColumns + 1) + " through " + to_string(printedColumns + columnsToPrintEachTime) + ":\n";
		else
		{
			if (printedColumns + 1 != nC) s += "\tColumns " + to_string(printedColumns + 1) + " through " + to_string(nC) + ":\n";
			else s += "\tColumn " + to_string(printedColumns + 1) + ":\n";
		}
		int columnsToPrint = ((nC - printedColumns) >= columnsToPrintEachTime) ? columnsToPrintEachTime : nC - printedColumns;
		for (int iR = 0; iR < nR; iR++)
		{
			if (biggerThanSix) s += "\tRow " + to_string(iR + 1) + ": "; else s += "\tRow " + to_string(iR + 1) + ": ";
			if (iR < 9 && nR >= 9) s += " "; // 9 is row number 10
			s += "\t";
			for (int iC = 0; iC < columnsToPrint; iC++)
			{
				/*char buffer[70];
				snprintf(buffer, 70, "%g\t", values[iR][printedColumns + iC]);
				s += buffer;*/
				string k;
				if (values[iR][printedColumns + iC] >= 0.00) k = " ";
				k += to_string(values[iR][printedColumns + iC]);
				int sLength = k.length();
				s += k;
				for (int i = sLength; i < columnSize && iC != columnsToPrint - 1; i++) s += " ";
			}
			s += "\n";
		}
		printedColumns += columnsToPrintEachTime; s += "\n";
	}
	return s;
}
CMatrix CMatrix::denominatorOfDiv(float f) { // Boula
	CMatrix temp(nR, nC);
	for (int iR = 0; iR < temp.nR; iR++) {
		for (int iC = 0; iC < temp.nC; iC++) {
			temp.values[iR][iC] = f / values[iR][iC];
		}
	}
	return temp;
}

//core functions
void CMatrix::add(CMatrix& m) //peter
{
	if (nR != m.nR || nC != m.nC)throw("Invalid matrix dimension");
	for (int iR = 0; iR < nR; iR++)
		for (int iC = 0; iC < nC; iC++)
			values[iR][iC] += m.values[iR][iC];
}
void CMatrix::sub(CMatrix& m) //peter
{
	if (nR != m.nR || nC != m.nC)
		throw("Invalid matrix dimension");
	for (int iR = 0; iR < nR; iR++)
		for (int iC = 0; iC < nC; iC++)
			values[iR][iC] -= m.values[iR][iC];
}
void CMatrix::mul(CMatrix& m) //peter
{
	//cout << nR << "*" << nC << " " << m.nR << "*" << m.nC << endl;
	if (nC != m.nR)
		throw("Invalid matrix dimension");
	CMatrix r(nR, m.nC);
	for (int iR = 0; iR < r.nR; iR++)
		for (int iC = 0; iC < r.nC; iC++)
		{
			r.values[iR][iC] = 0;
			for (int k = 0; k < m.nC; k++)
				r.values[iR][iC] += values[iR][k] * m.values[k][iC];
		}
	copy(r);
}
void CMatrix::div(CMatrix& m) {
	//double Determinant = m.getDeterminant();
	//if (Determinant == 0) { cout << "No unique solution\n"; return; throw("No unique solution"); }
	CMatrix x;
	x.copy(m.getInverse());
	mul(x);

}
CMatrix CMatrix::lDiv(CMatrix& m) {
	double Determinant = this->getDeterminant();
	if (Determinant == 0) throw("No unique solution");
	if (nR != m.nR || nC != m.nC)throw("Invalid matrix dimension");
	CMatrix x;
	x.copy(this->getInverse());
	x.mul(m);
	return x;
}
CMatrix CMatrix::dotMul(CMatrix& m) {
	if (nR != m.nR || nC != m.nC)throw("Invalid matrix dimension");
	CMatrix M(nR, nC);
	for (int iR = 0; iR<nR; iR++)
		for (int iC = 0; iC<nC; iC++)
			M.values[iR][iC] = (this->values[iR][iC])*(m.values[iR][iC]);
	return M;
}
CMatrix CMatrix::dotDiv(CMatrix& m) {
	if (nR != m.nR || nC != m.nC)throw("Invalid matrix dimension");
	CMatrix M(nR, nC);
	for (int iR = 0; iR<nR; iR++)
		for (int iC = 0; iC<nC; iC++)
			M.values[iR][iC] = (this->values[iR][iC])/(m.values[iR][iC]);
	return M;
}
CMatrix CMatrix::dotPow(double d) {
	CMatrix M(nR, nC);
	for (int iR = 0; iR<nR; iR++)
		for (int iC = 0; iC<nC; iC++)
			M.values[iR][iC] = pow(this->values[iR][iC], d);
	return M;
}

CMatrix CMatrix::dotLDiv(CMatrix& m) {
	if (nR != m.nR || nC != m.nC)throw("Invalid matrix dimension");
	CMatrix M(nR, nC);
	for (int iR = 0; iR<nR; iR++)
		for (int iC = 0; iC<nC; iC++)
			M.values[iR][iC] = (m.values[iR][iC])/(this->values[iR][iC]);
	return M;
}
CMatrix CMatrix::getCofactor(int r, int c) //peter
{
	if (nR <= 1 && nC <= 1)
		throw("Invalid matrix dimension");
	CMatrix m(nR - 1, nC - 1);
	for (int iR = 0; iR < m.nR; iR++)
		for (int iC = 0; iC < m.nC; iC++)
		{
			int sR = (iR < r) ? iR : iR + 1;
			int sC = (iC < c) ? iC : iC + 1;
			m.values[iR][iC] = values[sR][sC];
		}
	return m;
}
#include <cmath>
void CMatrix::swapRow(int row1, int row2) {
	double tmp, *p1, *p2;

	if (row1 == row2) return;
	for (int i = 0; i < nC; i++) {
		tmp = values[row1][i];
		values[row1][i] = values[row2][i];
		values[row2][i] = tmp;
	}
}
double CMatrix::getDeterminant() {
	CMatrix m = *this;

	if (m.nR != m.nC)throw("Invalid matrix dimension");
	if (m.nR == 1 && m.nC == 1)
		return m.values[0][0];

	//checking the cases of zero determenant 
	//first case
	for (int i = 0; i < nR; i++)
	{
		for (int k = i + 1; k < nR; k++)
		{
			int zeroflag = 1;
			double Ratio = m.values[i][0] / m.values[k][0];
			int Ratioflag = 1;
			for (int j = 0; j < nC; j++)
			{
				if (Ratio != m.values[i][j] / m.values[k][j]) Ratioflag = 0;
				if (m.values[i][j] != 0) zeroflag = 0;
			}
			if (zeroflag == 1) return 0;
			if (Ratioflag == 1) return 0;
		}
	}
	//second case
	for (int i = 0; i < nR; i++)
	{
		for (int k = i + 1; k < nR; k++)
		{
			int zeroflag = 1;
			double Ratio = m.values[0][i] / m.values[0][k];
			int Ratioflag = 1;
			for (int j = 0; j < nC; j++)
			{
				if (Ratio != m.values[j][i] / m.values[j][k]) Ratioflag = 0;
				if (m.values[j][i] != 0) zeroflag = 0;
			}
			if (zeroflag == 1) return 0;
			if (Ratioflag == 1) return 0;
		}
	}

	int maxRow, dia;
	double max, temp;
	int sign = 1;
	for (int dia = 0; dia < nR; dia++) {
		maxRow = dia, max = abs(m.values[dia][dia]);

		for (int iR = dia + 1; iR < nR; iR++)
			if ((temp = abs(m.values[iR][dia])) > max)
				maxRow = iR, max = temp;

		if ((dia != maxRow) || (abs(m.values[dia][dia]) <= 0.0000000000000000001)) {
			m.swapRow(dia, maxRow);
			sign *= -1;
		}


		for (int iR = dia + 1; iR < nR; iR++) {
			temp = m.values[iR][dia] / m.values[dia][dia];
			for (int iC = dia + 1; iC < nC; iC++)
				m.values[iR][iC] -= temp * m.values[dia][iC];
			m.values[iR][dia] = 0;
		}
	}

	double determinant = 1.0;
	for (int i = 0; i < nR; i++) determinant *= m.values[i][i];

	return sign * determinant;
}
double CMatrix::getDeterminant2() //peter
{
	CMatrix m = *this;
	if (nR != nC)throw("Invalid matrix dimension");
	if (nR == 1 && nC == 1)
		return m.values[0][0];

	//checking the cases of zero determenant 
	//first case
	for (int i = 0; i < nR; i++)
	{
		for (int k = i + 1; k < nR; k++)
		{
			int zeroflag = 1;
			double Ratio = m.values[i][0] / m.values[k][0];
			int Ratioflag = 1;
			for (int j = 0; j < nC; j++)
			{
				if (Ratio != m.values[i][j] / m.values[k][j]) Ratioflag = 0;
				if (m.values[i][j] != 0) zeroflag = 0;
			}
			if (zeroflag == 1) return 0;
			if (Ratioflag == 1) return 0;
		}
	}
	//second case
	for (int i = 0; i < nR; i++)
	{
		for (int k = i + 1; k < nR; k++)
		{
			int zeroflag = 1;
			double Ratio = m.values[0][i] / m.values[0][k];
			int Ratioflag = 1;
			for (int j = 0; j < nC; j++)
			{
				if (Ratio != m.values[j][i] / m.values[j][k]) Ratioflag = 0;
				if (m.values[j][i] != 0) zeroflag = 0;
			}
			if (zeroflag == 1) return 0;
			if (Ratioflag == 1) return 0;
		}
	}

	int i, k, j, flag = 0;
	for (i = 0; i < nR; i++)                    //Pivotisation
		for (k = i + 1; k < nR; k++)
			if ((abs(m.values[i][i]) < abs(m.values[k][i])) || abs(m.values[i][i]) <= 0.0001) {
				flag++;
				for (j = 0; j < nR; j++) {
					double temp = m.values[i][j];
					m.values[i][j] = m.values[k][j];
					m.values[k][j] = temp;
				}
			}

	//cout << "\nThe matrix after Pivotisation is:\n";
	/*for (i = 0; i<nR; i++)            //print the new matrix
	{
	for (j = 0; j<nR; j++)
	cout << m.values[i][j] ;
	cout << "\n";
	}*/
	for (i = 0; i < nR - 1; i++)            //loop to perform the gauss elimination
		for (k = i + 1; k < nR; k++)
		{
			double t = m.values[k][i] / m.values[i][i];
			for (j = 0; j < nR; j++)
				m.values[k][j] = m.values[k][j] - t*m.values[i][j];    //make the elements below the pivot elements equal to zero or elimnate the variables
		}

	/*cout << "\n\nThe matrix after gauss-elimination is as follows:\n";
	for (i = 0; i<nR; i++)            //print the new matrix
	{
	for (j = 0; j < nR; j++)
	cout << m.values[i][j];
	cout << "\n";
	}*/
	double det = 1;
	for (i = 0; i < nR; i++) {
		det = det*m.values[i][i];
	}
	if (flag % 2 == 0) {
		det = det;
	}
	else {
		det = -det;
	}


	return det;

	/*
	double value = 0, m = 1;
	for (int iR = 0; iR<nR; iR++)
	{
	value += m * values[0][iR] * getCofactor(0, iR).getDeterminant(); m *= -1;
	}
	return value;*/
}
CMatrix CMatrix::getInverse() //peter
{
	if (nR != nC)throw("Invalid matrix dimension");
	double Determinant = getDeterminant();

	if (Determinant == 0) throw("Invalid matrix Inverse");
	CMatrix r(nR, nC);
	int m = 1;
	for (int iC = 0; iC < nC; iC++)
	{
		for (int iR = 0; iR < nR; iR++)
		{
			r.values[iC][iR] = m* (getCofactor(iR, iC).getDeterminant()) / Determinant;
			/*//comment
			if (isnan(r.values[iC][iR])) {
			cout << "element " << iC << " " << iR << " is nan! with determinant = " << getCofactor(iR, iC).getDeterminant() << endl;
			cout << getCofactor(iR, iC).getOriginalString() << endl;
			}
			else if (isinf(r.values[iC][iR])) {
			cout << "element " << iC << " " << iR << " is inf! with determinant = " << getCofactor(iR, iC).getDeterminant() << endl;
			cout << getCofactor(iR, iC).getOriginalString() << endl;
			}
			//comment*/
			m *= -1;
		}
		if (nR % 2 == 0)
			m *= -1;
	}

	return r;
}
CMatrix CMatrix::getTranspose() //Mina Magdy
{
	CMatrix M(nC, nR);
	for (int iR = 0; iR < nR; iR++)
		for (int iC = 0; iC < nC; iC++)
			M.values[iC][iR] = values[iR][iC];
	return M;
}

///////operators//////Hanaa
CMatrix CMatrix::operator=(const CMatrix& m)
{
	copy(m);
	return *this;
}
void CMatrix::operator+=(CMatrix& m)
{
	add(m);
}
CMatrix CMatrix::operator+(CMatrix& m)
{
	CMatrix r = *this;
	r += m;
	return r;
}
void CMatrix::operator-=(CMatrix& m)
{
	sub(m);
}
CMatrix CMatrix::operator-(CMatrix& m)
{
	CMatrix r = *this;
	r -= m;
	return r;
}
void CMatrix::operator*=(CMatrix& m)
{
	mul(m);
}
CMatrix CMatrix::operator*(CMatrix& m)
{
	CMatrix r = *this;
	r *= m;
	return r;
}
void CMatrix::operator/=(CMatrix& m)
{
	div(m);
}
CMatrix CMatrix::operator/(CMatrix& m)
{
	CMatrix r = *this;
	r /= m;
	return r;
}

//Geo

CMatrix CMatrix::operator==(CMatrix&m)
{
	if (nR != m.nR || nC != m.nC)throw("Invalid matrix dimension");
	CMatrix A;
	A.nR=m.nR , A.nC=m.nC ;
	for (int iR = 0; iR<nR; iR++)
		for (int iC = 0; iC<nC; iC++)
			A.values[iR][iC] =(values[iR][iC]== m.values[iR][iC])? 1:0;
	return A;
}
CMatrix CMatrix::operator!=(CMatrix&m)
{
	if (nR != m.nR || nC != m.nC)throw("Invalid matrix dimension");
	CMatrix A;
	A.nR=m.nR , A.nC=m.nC ;
	for (int iR = 0; iR<nR; iR++)
		for (int iC = 0; iC<nC; iC++)
			A.values[iR][iC] =(values[iR][iC]== m.values[iR][iC])? 0:1;
	return A;
}

CMatrix CMatrix::operator> (CMatrix&m)
{
	if (nR != m.nR || nC != m.nC)throw("Invalid matrix dimension");
	CMatrix A;
	A.nR=m.nR , A.nC=m.nC ;
	for (int iR = 0; iR<nR; iR++)
		for (int iC = 0; iC<nC; iC++)
			A.values[iR][iC] =(values[iR][iC]> m.values[iR][iC])? 1:0;
	return A;
}
CMatrix CMatrix::operator< (CMatrix&m)
{
	if (nR != m.nR || nC != m.nC)throw("Invalid matrix dimension");
	CMatrix A;
	A.nR=m.nR , A.nC=m.nC ;
	for (int iR = 0; iR<nR; iR++)
		for (int iC = 0; iC<nC; iC++)
			A.values[iR][iC] =(values[iR][iC]< m.values[iR][iC])? 1:0;
	return A;
}
CMatrix CMatrix::operator>=(CMatrix&m)
{
	if (nR != m.nR || nC != m.nC)throw("Invalid matrix dimension");
	CMatrix A;
	A.nR=m.nR , A.nC=m.nC ;
	for (int iR = 0; iR<nR; iR++)
		for (int iC = 0; iC<nC; iC++)
			A.values[iR][iC] =(values[iR][iC]>= m.values[iR][iC])? 1:0;
	return A;
}
CMatrix CMatrix::operator<=(CMatrix&m)
{
	if (nR != m.nR || nC != m.nC)throw("Invalid matrix dimension");
	CMatrix A;
	A.nR=m.nR , A.nC=m.nC ;
	for (int iR = 0; iR<nR; iR++)
		for (int iC = 0; iC<nC; iC++)
			A.values[iR][iC] =(values[iR][iC]<= m.values[iR][iC])? 1:0;
	return A;
}

CMatrix CMatrix::operator& (CMatrix&m)
{
	if (nR != m.nR || nC != m.nC)throw("Invalid matrix dimension");
	CMatrix A;
	A.nR=m.nR , A.nC=m.nC ;
	for (int iR = 0; iR<nR; iR++)
		for (int iC = 0; iC<nC; iC++)
			A.values[iR][iC] = values[iR][iC] & m.values[iR][iC];
	return A;
}
CMatrix CMatrix::operator| (CMatrix&m)
{
	if (nR != m.nR || nC != m.nC)throw("Invalid matrix dimension");
	CMatrix A;
	A.nR=m.nR , A.nC=m.nC ;
	for (int iR = 0; iR<nR; iR++)
		for (int iC = 0; iC<nC; iC++)
			A.values[iR][iC] = values[iR][iC] | m.values[iR][iC];
	return A;
}

CMatrix CMatrix::operator-()
{
for(int iR=0;iR<nR;iR++)
	for(int iC=0;iC<nC;iC++)
		values[iR][iC] = -values[iR][iC];
return *this;
}

istream& operator >> (istream &is, CMatrix& m)
{
string s;
getline(is, s, ']');
s+="]";
m = CMatrix(s);
return is;
}

ostream& operator << (ostream &os, CMatrix& m)
{
os<<m.getString();
return os;
}



CMatrix::operator const string()
{
return getOriginalString();
}

//pre post increment and decrement

CMatrix CMatrix::operator++()
{
for(int iR=0;iR<nR;iR++)
	for(int iC=0;iC<nC;iC++)
		values[iR][iC] ++;
return *this;
}

CMatrix CMatrix::operator++(int)
{
CMatrix c =*this ;
for(int iR=0;iR<nR;iR++)
	for(int iC=0;iC<nC;iC++)
		values[iR][iC] ++;
return c ;
}

CMatrix CMatrix::operator--()
{
for(int iR=0;iR<nR;iR++)
	for(int iC=0;iC<nC;iC++)
		values[iR][iC] --;
return *this;
}

CMatrix CMatrix::operator--(int)
{
CMatrix c = *this ;
for(int iR=0;iR<nR;iR++)
	for(int iC=0;iC<nC;iC++)
		values[iR][iC] --;
return c ;
}


//Hanaa



CMatrix sin(const CMatrix& m) 
{

	CMatrix M(m.nR, m.nC);
	for (int iR = 0; iR<m.nR; iR++)
		for (int iC = 0; iC<m.nC; iC++)
			M.values[iR][iC] = sin(m.values[iR][iC]);
	return M;

}
CMatrix cos(const CMatrix& m)
{

	CMatrix M(m.nR, m.nC);
	for (int iR = 0; iR<m.nR; iR++)
		for (int iC = 0; iC<m.nC; iC++)
			M.values[iR][iC] = cos(m.values[iR][iC]);
	return M;

}
CMatrix tan(const CMatrix& m)
{

	CMatrix M(m.nR, m.nC);
	for (int iR = 0; iR<m.nR; iR++)
		for (int iC = 0; iC<m.nC; iC++)
			M.values[iR][iC] = tan(m.values[iR][iC]);
	return M;

}
CMatrix csc(const CMatrix& m)
{

	CMatrix M(m.nR, m.nC);
	for (int iR = 0; iR<m.nR; iR++)
		for (int iC = 0; iC<m.nC; iC++)
			M.values[iR][iC] = 1.0/sin(m.values[iR][iC]);
	return M;

}
CMatrix sec(const CMatrix& m)
{

	CMatrix M(m.nR, m.nC);
	for (int iR = 0; iR<m.nR; iR++)
		for (int iC = 0; iC<m.nC; iC++)
			M.values[iR][iC] = 1.0/cos(m.values[iR][iC]);
	return M;

}
CMatrix cot(const CMatrix& m)
{

	CMatrix M(m.nR, m.nC);
	for (int iR = 0; iR<m.nR; iR++)
		for (int iC = 0; iC<m.nC; iC++)
			M.values[iR][iC] = 1.0/tan(m.values[iR][iC]);
	return M;

}

CMatrix asin(const CMatrix& m)
{

	CMatrix M(m.nR, m.nC);
	for (int iR = 0; iR<m.nR; iR++)
		for (int iC = 0; iC<m.nC; iC++)
			M.values[iR][iC] = asin(m.values[iR][iC]);
	return M;

}
CMatrix acos(const CMatrix& m)
{

	CMatrix M(m.nR, m.nC);
	for (int iR = 0; iR<m.nR; iR++)
		for (int iC = 0; iC<m.nC; iC++)
			M.values[iR][iC] = acos(m.values[iR][iC]);
	return M;

}
CMatrix atan(const CMatrix& m)
{

	CMatrix M(m.nR, m.nC);
	for (int iR = 0; iR<m.nR; iR++)
		for (int iC = 0; iC<m.nC; iC++)
			M.values[iR][iC] = atan(m.values[iR][iC]);
	return M;

}
CMatrix acsc(const CMatrix& m)
{

	CMatrix M(m.nR, m.nC);
	for (int iR = 0; iR<m.nR; iR++)
		for (int iC = 0; iC<m.nC; iC++)
			M.values[iR][iC] = asin(1.0/m.values[iR][iC]);
	return M;

}
CMatrix asec(const CMatrix& m)
{

	CMatrix M(m.nR, m.nC);
	for (int iR = 0; iR<m.nR; iR++)
		for (int iC = 0; iC<m.nC; iC++)
			M.values[iR][iC] = acos(1.0/m.values[iR][iC]);
	return M;

}
CMatrix acot(const CMatrix& m)
{

	CMatrix M(m.nR, m.nC);
	for (int iR = 0; iR<m.nR; iR++)
		for (int iC = 0; iC<m.nC; iC++)
			M.values[iR][iC] = atan(1.0/m.values[iR][iC]);
	return M;

}

CMatrix sinh(const CMatrix& m)
{

	CMatrix M(m.nR, m.nC);
	for (int iR = 0; iR<m.nR; iR++)
		for (int iC = 0; iC<m.nC; iC++)
			M.values[iR][iC] = sinh(m.values[iR][iC]);
	return M;

}
CMatrix cosh(const CMatrix& m)
{

	CMatrix M(m.nR, m.nC);
	for (int iR = 0; iR<m.nR; iR++)
		for (int iC = 0; iC<m.nC; iC++)
			M.values[iR][iC] = cosh(m.values[iR][iC]);
	return M;

}
CMatrix tanh(const CMatrix& m)
{

	CMatrix M(m.nR, m.nC);
	for (int iR = 0; iR<m.nR; iR++)
		for (int iC = 0; iC<m.nC; iC++)
			M.values[iR][iC] = tanh(m.values[iR][iC]);
	return M;

}
CMatrix csch(const CMatrix& m)
{

	CMatrix M(m.nR, m.nC);
	for (int iR = 0; iR<m.nR; iR++)
		for (int iC = 0; iC<m.nC; iC++)
			M.values[iR][iC] = 1.0 / sinh(m.values[iR][iC]);
	return M;

}
CMatrix sech(const CMatrix& m)
{

	CMatrix M(m.nR, m.nC);
	for (int iR = 0; iR<m.nR; iR++)
		for (int iC = 0; iC<m.nC; iC++)
			M.values[iR][iC] = 1.0 / cosh(m.values[iR][iC]);
	return M;

}
CMatrix coth(const CMatrix& m)
{

	CMatrix M(m.nR, m.nC);
	for (int iR = 0; iR<m.nR; iR++)
		for (int iC = 0; iC<m.nC; iC++)
			M.values[iR][iC] = 1.0 / tanh(m.values[iR][iC]);
	return M;

}

CMatrix asinh(const CMatrix& m)
{

	CMatrix M(m.nR, m.nC);
	for (int iR = 0; iR<m.nR; iR++)
		for (int iC = 0; iC<m.nC; iC++)
			M.values[iR][iC] = asinh(m.values[iR][iC]);
	return M;

}
CMatrix acosh(const CMatrix& m)
{

	CMatrix M(m.nR, m.nC);
	for (int iR = 0; iR<m.nR; iR++)
		for (int iC = 0; iC<m.nC; iC++)
			M.values[iR][iC] = acosh(m.values[iR][iC]);
	return M;

}
CMatrix atanh(const CMatrix& m)
{

	CMatrix M(m.nR, m.nC);
	for (int iR = 0; iR<m.nR; iR++)
		for (int iC = 0; iC<m.nC; iC++)
			M.values[iR][iC] = atanh(m.values[iR][iC]);
	return M;

}
CMatrix acsch(const CMatrix& m)
{

	CMatrix M(m.nR, m.nC);
	for (int iR = 0; iR<m.nR; iR++)
		for (int iC = 0; iC<m.nC; iC++)
			M.values[iR][iC] = asinh(1.0 / m.values[iR][iC]);
	return M;

}
CMatrix asech(const CMatrix& m)
{

	CMatrix M(m.nR, m.nC);
	for (int iR = 0; iR<m.nR; iR++)
		for (int iC = 0; iC<m.nC; iC++)
			M.values[iR][iC] = acosh(1.0 / m.values[iR][iC]);
	return M;

}
CMatrix acoth(const CMatrix& m)
{

	CMatrix M(m.nR, m.nC);
	for (int iR = 0; iR<m.nR; iR++)
		for (int iC = 0; iC<m.nC; iC++)
			M.values[iR][iC] = atanh(1.0 / m.values[iR][iC]);
	return M;

}

CMatrix exp(const CMatrix& m)
{

	CMatrix M(m.nR, m.nC);
	for (int iR = 0; iR<m.nR; iR++)
		for (int iC = 0; iC<m.nC; iC++)
			M.values[iR][iC] = exp(m.values[iR][iC]);
	return M;

}
CMatrix log(const CMatrix& m)
{

	CMatrix M(m.nR, m.nC);
	for (int iR = 0; iR<m.nR; iR++)
		for (int iC = 0; iC<m.nC; iC++)
			M.values[iR][iC] = log(m.values[iR][iC]);
	return M;

}
CMatrix log10(const CMatrix& m)
{

	CMatrix M(m.nR, m.nC);
	for (int iR = 0; iR<m.nR; iR++)
		for (int iC = 0; iC<m.nC; iC++)
			M.values[iR][iC] = log10(m.values[iR][iC]);
	return M;

}

