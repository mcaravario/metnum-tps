#include	<iostream>
#include	<fstream>
#include	<vector>
#include	<stdio.h>
#include	<string.h>
#include	<iterator>
#include	<algorithm>
#include	<iomanip>  
#include 	<math.h>
#include 	<time.h>
using namespace std;

struct tupla{
		vector<double> avector;
		double avalor;
};

struct tupla2{
		vector<double> prim;
		vector< vector <double> > seg;
};

void Resta(vector< vector <double> > &m1, vector< vector <double> > &m2){
		for(int i=0; i<m1.size(); i++){
				for(int j=0; j<m1.size(); j++){
						m1[i][j] -= m2[i][j];
				}
		}
		return;
}

vector <vector <double> > multiplicar(double lambda, vector<double> v){

		vector <vector <double> > res(v.size(), std::vector<double>(v.size()));
		for(int i=0; i<v.size(); i++){
				for(int j=0; j<v.size(); j++){
						res[i][j] = lambda*v[i]*v[j];
				}
		}
		return res; 
}

double multiplicar2(vector<double> v){
		double res = 0;
		for(int i=0; i<v.size(); i++){
				res += v[i]*v[i];
		}
		return res;
}

void division(vector<double> &v, vector<double> nom, double den){
		
		for(int i=0; i<nom.size(); i++){
				v[i] = nom[i]/den;
		}
		return;
}

vector<double> producto(vector <vector <double> > &B, vector<double> v){
		vector<double> res(B.size());
		double temp;
		for(int i = 0; i<B.size(); i++){
				temp = 0;
				for(int j=0; j<B[0].size(); j++){
						temp += B[i][j]*v[j]; 
				}
				res[i] = temp;
		}
		return res;
}

double producto2(vector <vector <double> > &B, vector<double> v){
		double res = 0;
		vector<double> v2 = producto(B, v);
		for(int i=0; i<v2.size(); i++){
				res += v[i]*v2[i];
		}
		return res;
}

double norma(vector<double> v){
		double res=0;
		for(int i=0; i<v.size(); i++){
				res += v[i]*v[i]; 
		}
		res = sqrt(res);
		return res;
}

tupla aplicarMetPot(vector< vector<double> > &B, vector<double> x0, int niter){
		
		double lambda;
		vector<double> v = x0;
		
		for(int i=0; i<niter; i++){
				division(v, producto(B,v), norma(producto(B,v)));				//voy modificando v
		}
		lambda = producto2(B, v)/multiplicar2(v);;
		
		tupla t;
		t.avalor = lambda;
		t.avector = v;
		return t;
}

tupla2 potXdef(vector <vector <double> > matriz, int K, int tamM){
		
		//la matriz V
		vector <vector <double> > V;
		//las primeras k componentes principales de sigma
		vector <double> E;
		vector <vector <double> > matrizResultante(tamM, std::vector<double>(tamM));
		//vector aleatorio
		vector<double> x0(tamM);
		double t;
		for(int i=0; i<x0.size(); i++){
					t = rand()%100 + 1;
					x0[i] = t;
		}
		
		//ciclo para la deflacion
		int niter = 200;
		for(int i=0; i<K; i++){

				Resta(matriz, matrizResultante);
				
				tupla temp = aplicarMetPot(matriz, x0, niter);
				
				E.push_back(temp.avalor);
				V.push_back(temp.avector);
				
				matrizResultante = multiplicar(temp.avalor, temp.avector);
		}
		
		tupla2 res;
		for(int i=0; i<K; i++){
			(res.prim).push_back(sqrt(E[i]));
			(res.seg).push_back(V[i]);
		}
		
		return res;
}

vector<double> tc(vector< vector <double> > &V, vector<double> &sujeto, double K){

		vector<double> res;
		double temp;
		for(int i=0; i<K; i++){
				temp = 0;
				for(int j=0; j<V[0].size(); j++){
						temp += V[i][j]*sujeto[j];
				}
				res.push_back(temp);
		}
		return res;
}

double comparar(vector<double> v1, vector<double> v2){
		double res = 0;
		for(int i=0; i<v1.size(); i++){
				res += (v1[i]-v2[i])*(v1[i]-v2[i]);
		}
		res = sqrt(res);
		return res;
}

vector< vector<double> > calcularV(vector< vector<double> > &At, vector< vector<double> > &U){
		
		vector< vector <double> > res;
		vector<double> temp;
		double den;
		
		for(int i=0; i<U.size(); i++){
				temp = producto(At, U[i]);
				den = norma(temp);
				for(int i=0; i<temp.size(); i++){
						temp[i] /= den;
				}
				res.push_back(temp);
		}
		return res;
}
