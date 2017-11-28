#include "cmatrix.h"
#include <cmath>

string solve(string s, string varNames[100], string varContent[100], int& variablesNo) {
	int  counter1 = 0;
	string operation[21] = { "+","-","*","/","+=","-=","*=","/=","\\","\\=","&","|","~",".*","./",".\\",".^",".*=","./=",".^=",".\\=" };
	string operand; string operand2;
	std::size_t found = s.find('(');
	if (found != std::string::npos)
	{
		operand = s.substr(s.find('(') + 1, s.find(')') - s.find('(') - 1);;

	}


	else
	{

		for (int j = 0; j<21; j++)
		{
			std::size_t found1 = s.find(operation[j]);
			if (found1 != std::string::npos)
			{
				operand = s.substr(0, s.find(operation[j]));

				operand2 = s.substr(s.find(operation[j]) + operation[j].length(), s.length());
				int r = operation[j].length();
				// cout<<operand2<<"\n"<<r<<"\n";

			}
		}
	}

	for (int counter = 0; counter<variablesNo; counter++)
	{
		counter1 = 0;
		for (int k = 0; k<variablesNo; k++)
		{
			if (operand == varNames[counter])
			{
				string op = varContent[counter];
				if (op[0] == ('['))
				{
					counter1 = 1; break;
				}
			}
			if (operand2 == varNames[k])
			{
				string op1 = varContent[k];
				if (op1[0] == ('['))
				{
					counter1 = 1; break;
				}
			}
		}if (counter1 == 1) break;
	}

	if (counter1 == 1)
	{
		CMatrix x = solveCMatrix(s, varNames, varContent, variablesNo);
		return x.getOriginalString();
		//goto solveCMatrix();
		//cout<<"pass "<<s<<"to solve cmatrix"<<"\n";
		
	}
	else if (counter1 == 0) {
		double y = solveDouble(s, varNames, varContent, variablesNo);
		return to_string(y);
		//	goto solveDouble();
		//cout<<"pass"<<s<<"to solve double"<<"\n";
	}
	//return s;//to test only




} 

CMatrix solveCMatrix(string s, string varNames[100], string varContent[100] , int& variablesNo, int& outFileSize) {} // Verina /* Takes a string as input and returns a matrix output, Input may contain previous predefined variables and/or normal numbers */

double solveDouble(string s, string varNames[100], string varContent[100] , int& variablesNo, int& outFileSize) {} //  Marina /* Takes a string as input and returns a double output, Input may contain previous predefined variables and/or normal numbers */
