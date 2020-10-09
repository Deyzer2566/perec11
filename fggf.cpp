#include<iostream>
#include<fstream>
#include<vector>
using namespace std;
int main()
{
	//ifstream file("test.txt");
	double d=0,b=0,t=0,p=0;
	cin >> d >> b >> t >> p; 
	int Nmin = double(double(t) * double(d)) / (double(p) * (double(b) - double(d))) + 0.5;//минимальное количество пауз
	int V = t * d / Nmin + 0.9f;
	cout << V;
	system("pause");
}
