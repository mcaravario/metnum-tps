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
#include	"input.h"
#include    <sys/time.h>
using namespace std;
#define INF 1000000000000


string personas[41][10];				//para cada persona, qué imagenes voy a utilizar para el entrenamiento
										//personas[carpeta_correspondiente][num_imagen_correspondiente]		
int metodo;								//estandar = 0, alternativo = 1
int ntest;								//cantidad de tests que voy a efectuar
double filas, columnas, p, nimgp, K;
string src;
double c;

timeval start, end;
void init_time(){
     gettimeofday(&start,NULL);
}

double get_time(){
	     gettimeofday(&end,NULL);
	     return (1000000*(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec))/1000000.0;}											//problema a resolver
		
int main(int argc, char** argv){
		double tiempoTomado;
		/* Leo la entrada y en base a ella armo la matriz X */
		ifstream fichero_entrada(argv[1]);
		ofstream fichero_salida(argv[2]);
		metodo = atoi(argv[3]);
		
		fichero_entrada>>src>>filas>>columnas>>p>>nimgp>>K;
		
		vector <vector <double> > X(p*nimgp, std::vector<double>(filas*columnas));
		double nimg = 0;
		string copia;
		string copia2;
		string temporal;
		for(int i=0; i<p; i++){
				copia2 = src;
				fichero_entrada>>temporal;
				personas[i][0] = temporal;
				copia2.append(personas[i][0]);
				
				for(int j=0; j<nimgp; j++){
						copia = copia2;
						fichero_entrada>>temporal;
						personas[i][j] = temporal;
						
						copia.append(personas[i][j]);		//nro. de imagen de la persona
						copia.append(".in");
						const char *temp = copia.c_str();	//c_str "hace constante" a copia
						
						ifstream entrada(temp);
						for(int j=0; j<filas*columnas; j++){
							entrada>>c;
							X[nimg][j] = c;
						}
						nimg++;
				}
		}
		
		/* Leo los tests que quiero realizar y guardo esas imagenes */
		fichero_entrada>>ntest;
		string nroTest[ntest][2];
		double d;
		vector <vector <double> > sujetos(ntest, std::vector<double>(filas*columnas));
		for(int i=0; i<ntest; i++){
				fichero_entrada>>nroTest[i][0]>>nroTest[i][1];
				(nroTest[i][0]).erase((nroTest[i][0]).end()-1);
				(nroTest[i][0]).erase((nroTest[i][0]).end()-1);
				(nroTest[i][0]).erase((nroTest[i][0]).end()-1);
				(nroTest[i][0]).append("in");
				
				ifstream proxsujeto(nroTest[i][0].c_str());
				for(int j=0; j<filas*columnas; j++){
						proxsujeto>>d;
						sujetos[i][j] = d;
				}
		}
		
		/* Ahora, armo la matriz X y Xt */
		vector<double> mu(filas*columnas);
		for(int j=0; j<filas*columnas; j++){
				mu[j] = 0;
				for(int i=0; i<p*nimgp; i++){
						mu[j]+=X[i][j];
				}
				mu[j] = mu[j]/(p*nimgp);
		}
		
		//tengo X = A
		for(int i=0; i<p*nimgp; i++){
				for(int j=0; j<filas*columnas; j++){
						X[i][j]-=mu[j];
						X[i][j] = X[i][j]/sqrt(p*nimgp - 1);
				}
		}
		
		//tengo Xt = At
		vector <vector <double> > Xt(filas*columnas, std::vector<double>(p*nimgp));
		for(int i=0; i<filas*columnas; i++){
				for(int j=0; j<p*nimgp; j++){
						Xt[i][j] = X[j][i];
						Xt[i][j] = Xt[i][j]/sqrt(p*nimgp - 1);
				}
		}
		
		
		/* Ahora, con el metodo de la potencia, quiero calcular los autovalores de Mx=AtA (iguales a los de A) o AAt(distintos a los de A). En base a esto voy a hallar A = UEVt */
		int M = max(p*nimgp, filas*columnas);
		int m = min(p*nimgp, filas*columnas);
		
		vector <vector <double> > AtA(M, std::vector<double>(M));
		vector <vector <double> > AAt(m, std::vector<double>(m));
		vector <vector <double> > V;
		tupla2 res0, res1;
		double temp;
		
		if(metodo==0){
			init_time(); 
			/* Ahora, quiero calcular At*A = Xt*X y luego aplicar potXdef */
			for(int k=0; k<M; k++){
					for(int t=0; t<M; t++){
							temp = 0;
							for(int j=0; j<m; j++){
								temp += Xt[k][j]*X[j][t];
							}
							AtA[k][t] = temp;
					}
			}
			res0 = potXdef(AtA, K, M);
			V = res0.seg;
		}
		
		if(metodo==1){
			init_time(); 
			/* Ahora, quiero calcular A*At = X*Xt y luego aplicar potXdef */
			for(int k=0; k<m; k++){
					for(int t=0; t<m; t++){
							temp = 0;
							for(int j=0; j<M; j++){
								temp += X[k][j]*Xt[j][t];
							}
							AAt[k][t] = temp;
					}
			}
			res1 = potXdef(AAt, K, m);
			V = calcularV(Xt, res1.seg);									//uso que At*u_i = v_i
		}
		
		if(metodo==0){
				/* Devuelvo los valores singulares de A */
				for(int i=0; i<K; i++){
					fichero_salida<< setprecision(10) << (res0.prim)[i]<<"\n";
					cout<< setprecision(10) << (res0.prim)[i]<<"\n";
				}
				fichero_salida<<"\n";
		}
		
		if(metodo==1){
				/* Devuelvo los valores singulares de A */
				for(int i=0; i<K; i++){
					fichero_salida<< setprecision(10) << (res1.prim)[i]<<"\n";
					cout<< setprecision(10) << (res1.prim)[i]<<"\n";
				}
				fichero_salida<<"\n";
		}
		
		
		
		/* Ahora, leo las imagenes que me pasan para testear y a cada una le aplico la funcion tc y en función a esto le asigno una imagen de la base de datos (que tambien tengo que aplicarles tc) */
		vector< vector<double> > caras;								//aca guardo las caras ya listas para ser analizadas
		for(int i=0; i<sujetos.size(); i++){
				for(int j=0; j<filas*columnas; j++){
						sujetos[i][j] -= mu[j];
						sujetos[i][j]/=sqrt(p*nimgp-1);
				}
				caras.push_back(tc(V, sujetos[i], K));					//APLICAMOS TRANSFORMACION CARACTERISTICA Y INSERTAMOS LISTA DE VECTORES A ANALIZAR (tc esta en input.h) - calculo tc(x)
		}
		
		
		vector< vector<double> > base;
		for(int i=0; i<X.size(); i++){
					base.push_back(tc(V, X[i], K));						//calculo tc(x_i)
		}
		
		/* Ahora, devuelvo por consola el índice j de la imagen de la base para la cual ||tc(x)-tc(x_j)|| es menor */
		double tasaAciertos = 0;
		double distancia, distancia2;
		double media = 0;
		int indice;
		vector<int> comparacion(ntest);
		for(int i=0; i<caras.size(); i++){
				distancia = INF;
				indice = 0;
				for(int j=0; j<base.size(); j++){
						distancia2 = comparar(base[j], caras[i]);
						if(distancia2 < distancia){indice = j;distancia=distancia2;}
				}
				indice = indice/(int)(nimgp) + 1;
				if(indice == i+1){ tasaAciertos++;}
				cout<<distancia<<" "<<indice<<endl;
				media += distancia;
				comparacion.push_back(indice);
		}
		cout<<(media/(double)caras.size())<<endl;
	
		cout<<endl;
		cout<<"La tasa de aciertos es: "<<(tasaAciertos/caras.size())<< endl;
		tiempoTomado = get_time();
		
		cout << "El tiempo que tomo el proceso fue: " << tiempoTomado<< endl;
		return 0;
}


