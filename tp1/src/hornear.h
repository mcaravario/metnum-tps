#ifndef HORNEAR_HPP_
#define HORNEAR_HPP_
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream> 

using namespace std;
namespace metnum{
//HONEAR (gausselimin + solver, devuelve el vector soluciones), mostrarMatriz, calcularIsoterma)
void mostrarMatriz(vector< vector<double> >& matrix){
int fil = matrix.size();
int col = matrix[0].size(); 
	for(int i = 0; i < fil; i++){
		for(int j =0; j < col; j++){
			cout << matrix[i][j]<<"    ";
		}
		cout << "\n";		
		col = matrix[0].size();
		}		
	return;	
}
vector<double> hornear(vector< vector<double> > A){
double m;	
	for(int k = 0; k < (A.size()-1); k++){
		
		for(int i = k+1; i < A.size(); i++){ //busca el multiplicador de la fila
			if(A[k][k] < 0.00001 && A[k][k] > -0.00001 ){
				int toSwap = k; 
				double maximo = 0; 	
				for(int l = k+1; l < A.size(); l++){
					if(abs(A[l][k]) > maximo){
						maximo = A[l][k];
						toSwap = l;					
					}
				}
					if(maximo != 0){
						 for(int t = 0; t < A[0].size(); t++){
							double guardo1 = A[k][t];
							A[k][t] = A[toSwap][t];
							A[toSwap][t] = guardo1; 						
						}
						
					}
			}
			
			m = (A[i][k]/A[k][k]);							
		
			for(int j = k; j < A[0].size(); j++){ 			// asigna valor a cada columna de la fila dada por i	
				A[i][j] = A[i][j] - m*A[k][j];			
			}
		} 
	}
vector<double> solution(A.size());
double m1; 

for(int i = A.size()-1; i > -1; i--){  //diagonales
	m1 = (1/A[i][i]);
	double suma = 0;
	for(int j = i+1; j < A[0].size(); j++){ //multiplico por entre medio
			A[i][j] = ((A[i][j])*m1);
			
		}
	for(int k = i+1; k < A[0].size()-1; k++){			
			suma = suma + (A[i][k]*solution[k]);
	
	 
		}
	solution[i] = (A[i][(A[0].size()-1)] - suma); 
	}

	return solution;
}
vector<double> calcularIsoterma(vector<double>& solucion, double isoterma, double rint, double rext, int mrat, int nang){
		
		vector<double> isot(nang);
		double dradio = (rext-rint)/(mrat-1);
		
		for(int i = 0; i< nang; i++){
			int j = i;
			double primerMenor = 0; 	//supongo qeu no hay temps bajo cero 
				while(j < (mrat*nang) && (primerMenor>-0.00001 && primerMenor<0.00001)){ 
					//use busqueda lineal para encontrar la isoterma y luego calcular mediante el j, el radio.	
					if(((solucion[j]-isoterma) > -0.00001 && (solucion[j]-isoterma) < 0.00001) || (solucion[j]-isoterma < -0.00001)){
						primerMenor = 	solucion[j];				
					}
					
				j = j + nang;
				} 
	
				j = j-nang;
				if(j < nang){           //caso en que la isoterma esta en la pared interna
					isot[i] = rint;				
				}else{
				if(((j + nang -(nang*mrat))>0) || ( ((j + nang -(nang*mrat))> -0.00001 &&(j + nang -(nang*mrat)) < 0.00001))){ 	
		//caso en que la isoterma esta en la pared externa	
							if(((solucion[j]-isoterma) > -0.00001 && (solucion[j]-isoterma) < 0.00001) || ((solucion[j]-isoterma) > 0 ) ){
								isot[i] =rext;
								}else{
						isot[i] =((rext - (rint + (dradio*(j-nang)/nang)))/2 + rint + dradio*(j-nang)/nang);
							
							}
													
						}else{ 			//caso que no esta en los bordes de las paredes
						if((solucion[j]-isoterma) > -0.00001 && (solucion[j]-isoterma) < 0.00001 ){
					isot[i] =(rint + dradio * (j/nang));								
							}else{
			isot[i] =(((rint + (dradio*(j)/nang)) - (rint + (dradio*(j-nang)/nang)))/2 + rint + dradio*(j-nang)/nang);
															
							}
						}						
					}
					
			}

		return isot;
	}

//LO DE FACTORIZACION LU, de aca se desprende factorizacionL, factorizacionU, despejarL, despejarU

void diagonalizar(vector< vector<double> >& A){								
		
	for(int k=0; k < A.size(); k++){						// con k recorro la diagonal y le pongo 1's
		
		A[k][k] = 1;
	}
	return;
}

void triangularInf(vector< vector<double> >& A){
	
	for(int k = 0; k < A.size(); k++){	// con k recorro la diagonal y con j voy recorriendo la fila para ir poniendole ceros
		
		for(int j = k+1; j < A.size(); j++){
			
				A[k][j] = 0;
				
		}
	
		
	}
	
	return;
}


void factorizacionL(vector< vector<double> >& A){	
double m;	
	for(int k = 0; k < (A.size()-1); k++){
		
		for(int i = k+1; i < A.size(); i++){ //busca el multiplicador de la fila
			if(A[k][k] < 0.00001 && A[k][k] > -0.00001){
				int toSwap = k; 
				double maximo = 0; 	
				for(int l = k+1; l < A.size(); l++){
					if(abs(A[l][k]) > maximo){
						maximo = A[l][k];
						toSwap = l;					
					}
				}
					if(maximo != 0){
						 for(int t = 0; t < A[0].size(); t++){
							double guardo1 = A[k][t];
							A[k][t] = A[toSwap][t];
							A[toSwap][t] = guardo1; 						
						}
						
					}
			}
		m = (A[i][k]/A[k][k]);							
		
		for(int j = k; j < A[0].size(); j++){// asigna valor a cada columna de la fila dada por i	
				A[i][j] = A[i][j] - m*A[k][j];
				
				
							
		}
		
		A[i][k] = m;
							
		
		} 
	}
	
	diagonalizar(A);													
														
	triangularInf(A);
	return;
}

void factorizacionU(vector< vector<double> >& A){	
double m;	
	for(int k = 0; k < (A.size()-1); k++){
		
		for(int i = k+1; i < A.size(); i++){ //busca el multiplicador de la fila
			if(A[k][k] < 0.00001 && A[k][k] > -0.00001){
				int toSwap = k; 
				double maximo = 0; 	
				for(int l = k+1; l < A.size(); l++){
					if(abs(A[l][k]) > maximo){
						maximo = A[l][k];
						toSwap = l;					
					}
				}
					if(maximo != 0){
						 for(int t = 0; t < A[0].size(); t++){
							double guardo1 = A[k][t];
							A[k][t] = A[toSwap][t];
							A[toSwap][t] = guardo1; 						
						}
						
					}
			}
		m = (A[i][k]/A[k][k]);							
		
		for(int j = k; j < A[0].size(); j++){// asigna valor a cada columna de la fila dada por i	
				A[i][j] = A[i][j] - m*A[k][j];
				
				
							
		}
	
		}
	}return;
}

vector<double> despejarU(vector< vector<double> > U){
vector<double> solution(U.size());
double m1; 

for(int i = U.size()-1; i > -1; i--){  //diagonales
	m1 = (1/U[i][i]);
	double suma = 0;
	for(int j = i+1; j < U[0].size(); j++){ //multiplico por entre medio
			U[i][j] = ((U[i][j])*m1);
			
		}
	for(int k = i+1; k < U[0].size()-1; k++){			
			suma = suma + (U[i][k]*solution[k]);
	
	 
		}
	solution[i] = (U[i][(U[0].size()-1)] - suma); 
	}

	return solution;
}

	
vector<double> despejarL(vector< vector<double> > C){		
	
	vector<double> solucion(C.size());
	for(int i = 0; i < C.size(); i++){
		
		double acum = 0;
		
		for(int h = 0; h<i; h++){
			
			acum = (C[i][h]*(solucion[h])) + acum;
		}
		solucion[i] = C[i][C.size()] - acum;
			
			}
	
		
	return solucion;
	
}
}

	

#endif
