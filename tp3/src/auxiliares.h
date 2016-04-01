#include    <iostream>
#include    <fstream>
#include    <vector>
#include    <stdio.h>
#include    <string>
#include    <algorithm>
#include    <sstream>
#include    <iomanip>  
#include    <math.h>
//#include    <boost/numeric/ublas/matrix.hpp>
//#include    <boost/numeric/bindings/lapack/ormqr.hpp>

using namespace std;

struct punto {
		double x, y;
};

bool operator==(punto a, punto b) {
		return (a.x==b.x && a.y==b.y);
}

double dist(double a, double b) {
		if (a > b) {return a-b;
		} else {return b-a;}
}

bool pertenece(vector <punto> jugadores, punto p) {
		bool res = false;
		for(int i=0; i<jugadores.size(); i++) {
				if( sqrt(pow(jugadores[i].x - p.x, 2) + pow(jugadores[i].y - p.y, 2)) <= 10) {
						res = true;
				}
		}
		return res;
}


double mover_arquero_caso1(double pos_actual_arquero, double pos_final_pelota, double arco_inf, double arco_sup, double mu)	{
		
		/* Teniendo en cuenta que la distancia de atajada del arquero es de +/- 7 */
		double long_atajada = 7;
		double r = 0;
		
		if(pos_actual_arquero > pos_final_pelota) {
				r = min(mu, pos_actual_arquero - pos_final_pelota);
				r = -r;
		} else {
				r = min(mu, pos_final_pelota - pos_actual_arquero);
		}
		
		double pos_nueva_arquero = pos_actual_arquero + r;
		
		if(pos_nueva_arquero > arco_sup - long_atajada) {
				r = (arco_sup - long_atajada) - pos_actual_arquero;
		}
		
		if(pos_nueva_arquero < arco_inf + long_atajada) {
				r = -(pos_actual_arquero - (arco_inf + long_atajada));
		}
		
		return r;
}

double mover_arquero_caso2(double pos_actual_arquero, double temp_punto_y, double arco_inf, double arco_sup, double mu) {
		
		double centro_arco = (arco_sup + arco_inf)/2;
		double r = 0;
			
		/* Asumimos que la pelota va a salir disparada al centro del arco */
			
		if(pos_actual_arquero > centro_arco) {
				r = min(-mu, pos_actual_arquero - centro_arco);
		}
		
		if(pos_actual_arquero < centro_arco) {
				r = min(mu, centro_arco - pos_actual_arquero);
		}
		
		return r;
}

void actualizarAxy(vector <vector <double> > &A, vector <double> &x, vector <double> &y, punto p_temp, int grado_polinomio, double T) {
    
    x.push_back(p_temp.x);
    y.push_back(p_temp.y);
    vector<double> vtemp(grado_polinomio + 1);
    
    for(int i=0; i<=grado_polinomio; i++) {
        vtemp[i] = pow(p_temp.x, (double)i);
    }
    A.push_back(vtemp);
}


vector<double> cuadrados_minimos_QR(vector <vector <double> > &A, vector <double> &x, vector <double> &y, vector <double> &ax, vector <double> &ay) {
    
    vector<double> z;
    return z;
}


double mover_arquero_QR(double pos_final_pelota, string output) {
    double x;
    return x;
}

double estimar_posicion_final_pelota(vector <double> &ax, vector <double> &ay) {
    double x = 0;
    return x;
}


///////////////////////////////////////////////////////////////////////

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
vector<double> resolverSistema(vector< vector<double> >& A){
    double m;	
    
    for(int k = 0; k < (A.size()-1); k++){
        
        for(int i = k+1; i < A.size(); i++){        //busca el multiplicador de la fila
            
            if(A[k][k] < 0.00001 && A[k][k] > -0.00001 ){
              
              int toSwap = k; 
              double maximo = 0;
              
              for(int l = k+1; l < A.size(); l++){
                  
                  if(fabs(A[l][k]) > maximo){
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
            
            for(int j = k; j < A[0].size(); j++){     // asigna valor a cada columna de la fila dada por i
                
                A[i][j] = A[i][j] - m*A[k][j];
            }
        } 
    }
    
    vector<double> solution(A.size());
    double m1; 

    for(int i = A.size()-1; i > -1; i--){           //diagonales
        m1 = (1/A[i][i]);
        double suma = 0;
        for(int j = i+1; j < A[0].size(); j++){     //multiplico por entre medio
            A[i][j] = ((A[i][j])*m1);
        }
        for(int k = i+1; k < A[0].size()-1; k++){
            suma = suma + (A[i][k]*solution[k]);
        }
        solution[i] = (A[i][(A[0].size()-1)] - suma); 
    }
    
    return solution;
}


double armarYEvaluar(vector<double>& coefPol, double val, int tn_minus_1){
    
    return (coefPol[0] * (val-tn_minus_1)* (val-tn_minus_1)* (val-tn_minus_1)) + (coefPol[1]*(val-tn_minus_1)*(val-tn_minus_1)) + (coefPol[2] * (val-tn_minus_1)) + coefPol[3];
    
}


double rootDe(vector<double>& coefPol, double look4root, int tn_minus_1){

    double epsilon = 0.00001;
    double a = tn_minus_1;
    double b = tn_minus_1;
    double c = tn_minus_1;
    /*Elección del b*/
     if(armarYEvaluar(coefPol,a,tn_minus_1 ) - 125 >= 0){
		while(armarYEvaluar(coefPol,b,tn_minus_1 ) - 125 >= 0 && armarYEvaluar(coefPol,c,tn_minus_1) - 125 >= 0  ){
			b++;
			c--;
		}
			if(armarYEvaluar(coefPol,c,tn_minus_1 ) - 125 < 0){
				b = c;
				
				}
	}else{
		while(armarYEvaluar(coefPol,b,tn_minus_1) - 125 < 0 && armarYEvaluar(coefPol,c,tn_minus_1) - 125 < 0 ){
			b++;
			c--;
		}
			if(armarYEvaluar(coefPol,c,tn_minus_1 ) - 125 >= 0){
				b = c;
				
				}
		}
		/*Metodo de la bisección*/
	double x;
	for(int i = 0 ; i<1000; i++){
         x = (a+b)/2.0;
        
        double fx = armarYEvaluar(coefPol,x,tn_minus_1 ) - 125;
	
		if(	fabs(fx) <= epsilon){
			return x;
        } else{
          
          if((fx * (armarYEvaluar(coefPol,a,tn_minus_1 )-125)) >0){
              a = x;
          }else{
              b = x;
          }
        }
    }
	return x;
}

double interpolarSplines(vector<punto>& recorridoPelota) {
    
    /*Sistema con las H's. Pongo una columna más para luego despejar la solución*/
    vector <vector <double> > matrizHxs(recorridoPelota.size()-2, std::vector<double>(recorridoPelota.size() - 1));
    vector <vector <double> > matrizHys(recorridoPelota.size()-2, std::vector<double>(recorridoPelota.size() - 1));
	
     for(int i = 0; i < matrizHxs.size(); i++){
        matrizHxs[i][i] = 2.0/3;    /*(h_i + h_i+1)/3 **** siempre es 2/3, h_i = 1 h_i+1 = 1 siempre.*/
        matrizHys[i][i] = 2.0/3;
    }

    for(int i = 0; i < recorridoPelota.size() - 3; i++){
    
        matrizHxs[i][i+1] = 1.0/6;                /*(h_i/6) donde corresponde en cada matriz de x's e y's*/
        matrizHxs[i+1][i] = 1.0/6;				/*Siempre es un 1/6, h_i = 1 siempre*/
    
        matrizHys[i][i+1] = 1.0/6;
        matrizHys[i+1][i] = 1.0/6;
    }
		
   
    for(int i = 0 ; i < matrizHxs.size(); i++){
        double e_iplus1_x	=  recorridoPelota[i+2].x - recorridoPelota[i+1].x;  /*t_i+1 - t_i siempre es 1, por eso no divido*/
        double e_i_x 		=  recorridoPelota[i+1].x - recorridoPelota[i].x;
        
        double e_iplus1_y	=  recorridoPelota[i+2].y - recorridoPelota[i+1].y;
        double e_i_y 		=  recorridoPelota[i+1].y - recorridoPelota[i].y;
		
		
        matrizHxs[i][matrizHxs[0].size()-1] = e_iplus1_x - e_i_x ; 
        matrizHys[i][matrizHys[0].size()-1] = e_iplus1_y- e_i_y ; 
    }
 
   	
    vector<double> rvectorx = resolverSistema(matrizHxs);       /*Triangulo y resuelvo la matriz Hxs y obtengo [r_1...r_n-1] (X)*/
    vector<double> rvectory = resolverSistema(matrizHys);	    /*Triangulo y resuelvo la matriz Hxs y obtengo [r_1...r_n-1] (Y)*/
    
    
    
    /* Recordar: r_0 = r_n = 0*/
    
    /*Armo el polinomio correspondiente al ultimo spline, el cual va a darme la información sobre el instante en el cual la pelota pasa por el arco*/
    
    //Sn_1 (t) = a_n-1 (t - t_n-1)^3 + 	b_n-1(t-t_n-1)^2 + c_n-1 (t-t_n-1) + d_n-1  PARA X
    //a_n-1 = (r_n - r_n-1) / 6h_n-1
    //b_n-1 = r_n-1 / 2
    //c_n-1 = ((x_n - x_n-1) / h_n-1  ) - (r_n/6  + r_n-1/3) h_n-1
    //d_n-1 = x_n-1
    
    //Sn_1 (t) = a_n-1 (t - t_n-1)^3 + 	b_n-1(t-t_n-1)^2 + c_n-1 (t-t_n-1) + d_n-1  PARA Y
    //a_n-1 = (r_n - r_n-1) / 6h_n-1
    //b_n-1 = r_n-1 / 2
    //c_n-1 = ((y_n - y_n-1) / h_n-1  ) - (r_n/6  + r_n-1/3) h_n-1
    //d_n-1 = y_n-1
   
    double r_n 	   = 0;
    double r_nminus1 = rvectorx[rvectorx.size() -1];
    double h_nminus1 = 1;
    double x_n     = recorridoPelota[recorridoPelota.size() - 1].x;
    double x_nminus1 = recorridoPelota[recorridoPelota.size() - 2].x;
    
    double a_n_minus_1X = (- r_nminus1)/6.0;
    double b_n_minus_1X = r_nminus1 /2.0;
    double c_n_minus_1X = ((x_n -  x_nminus1)) - (( r_nminus1/3.0));
    double d_n_minus_1X = x_nminus1;
    
    vector<double>coeficientesX(4);
    coeficientesX[0] = a_n_minus_1X;
    coeficientesX[1] = b_n_minus_1X;
    coeficientesX[2] = c_n_minus_1X;
    coeficientesX[3] = d_n_minus_1X;  
	
	 
	
    /* Uso las mismas variables que ya cree, para obtener lo necesario para el polinomio de Y*/
    r_n 	= 0;
    r_nminus1 = rvectory[rvectory.size()-1];
    h_nminus1  = 1;
    double y_n     = recorridoPelota[recorridoPelota.size() - 1].y;
    double y_nminus1 = recorridoPelota[recorridoPelota.size() - 2].y;
    
    
    double a_n_minus_1Y = (- r_nminus1 )/(6.0);
    double b_n_minus_1Y = r_nminus1	/2.0;
    double c_n_minus_1Y = ((y_n - y_nminus1)) - ((  r_nminus1/3.0));
    double d_n_minus_1Y = y_nminus1;
    
    vector<double>coeficientesY(4);
    coeficientesY[0] = a_n_minus_1Y;
    coeficientesY[1] = b_n_minus_1Y;
    coeficientesY[2] = c_n_minus_1Y;
    coeficientesY[3] = d_n_minus_1Y;
    
    int t_nminus1 = recorridoPelota.size()-2;
    
	//cout << t_nminus1 << endl;
    
    double root125 = rootDe(coeficientesX, 125, t_nminus1);
    
    if(recorridoPelota.size() % 3 == 0){
		
		string nombreArchivo1 = "T = ";
		string nombreArchivo2 = "T = ";
		
		stringstream stream; 
		int ts =  recorridoPelota.size();
		stream << ts;
		
		string cantPuntos = stream.str();
		  
		nombreArchivo1.append(cantPuntos);
		nombreArchivo2.append(cantPuntos);
		
		nombreArchivo1.append("_plot_t_y(t).out");
		nombreArchivo2.append("_plot_x(t)_y(t).out");
		
		ofstream fichero_salida1(nombreArchivo1);
		
		ofstream fichero_salida2(nombreArchivo2);
		double t = 0.0;
		for(int i = 0; i < 3000; i++){
			
			double evalTnY = armarYEvaluar(coeficientesY, t, t_nminus1);
			double evalTnX = armarYEvaluar(coeficientesX, t, t_nminus1);
			t = t + 0.01;
			
			fichero_salida1<<t<< " " << evalTnY<<endl;
			fichero_salida2<<evalTnX << " " << evalTnY<< endl;
			
			}
    
    
	}
    
    
      //el tercer parametro me hace falta, es el t_n-1 para armar el polinomio del ultimo Spline
		cout << endl;
		cout << endl;
			
		cout << "#PUNTOS: " << recorridoPelota.size() << " ---- RAIZ (instante): " << root125 << endl; 
		cout <<"Posicion en X del arquero : " << armarYEvaluar(coeficientesX, root125, t_nminus1)<< endl;
		double evaluadoEnY = armarYEvaluar(coeficientesY, root125, t_nminus1);
		cout <<"Posicion en Y del arquero : " << evaluadoEnY <<  endl;			
					

    return evaluadoEnY; //el tercer parametro me hace falta, es el t_n-1 para armar el polinomio del ultimo Spline
}
