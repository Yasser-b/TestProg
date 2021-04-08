#include <iostream>
#include <cmath>
#include <vector>
#include <ctime>
#include <fstream>
#include <string>

/*
	Ce code permet de resoudre les problemes d'optimisation donnes dans le document TestProg.
	Il est compose de trois parties principales:
	- La class probleme ou on definie les problemes a partir de la dimention, les limites et la fonction.
	- Les classes solveurs qui permettent de donnes les resultas de l'optimisation avec les valeurs des variables optimales.
	- Fonction main pour lancer le code
*/


using namespace std;

class Problem{
public:
    /*
    -n est la dimension
    -limits est une matrice nx2; pour chaque dimension j, limits[j][0] est la limite inf
     et limits[j][1] la limite sup
    -num_prob le numero de probleme donne
    -variables est n-length tableau qui contient les variables de probleme
    */
    int n;
    vector<vector<float>> limits;
    int num_prob;
    float *variables = new float[n];

    //allouer l'espace memoire pour chaque dimension
    void declare_limits(){
        this->n = num_prob==1 || num_prob==4? 1 : 2;   //Probleme 1 et 4 sont definies sur une dimention, Probleme 2 et 3 sur 2 dimension
        for(int i=0;i<n;i++){
            limits.push_back(vector<float>(2));
        }
    }

    // definir les limites pour chaque probleme
    void define_limits(){
        if (this->num_prob==1 || this->num_prob==4){
            this->limits[0][0] = -2;
            this->limits[0][1] = 2;
        }
        else if (this->num_prob==2 || this->num_prob==3){
            this->limits[0][0] = -100;
            this->limits[0][1] = 100;
            this->limits[1][0] = -100;
            this->limits[1][1] = 100;
        }
    }
    // definir la fonction f pour chaque probleme
    float f(float variables[]){
        if(num_prob==1){ return 2.0+3*variables[0]+4*pow(variables[0],2);}
        if(num_prob==2){ return pow((variables[0]+1),2) +pow((variables[1]-2),2) + pow((variables[0]+3),2) + pow((variables[1]-5),2);}
        if(num_prob==3){ return pow((variables[0]+1),2) +pow((variables[1]-2),2) + pow((variables[0]+3),2) + pow((variables[1]-5),2) +pow((variables[0]+2.5),2) + pow((variables[1]-3.5),2) + pow((variables[0]+4.2),2) +pow((variables[1]-3.7),2);}
        if(num_prob==4){ return exp(2+3*variables[0]+4*pow(variables[0],2));}
    }
};

class Solver1{
public:
    float *solution;        //solution est un tableau de variables qui minimise le probleme
    float return_value;     //return value est la valeur min de la fonction (le resultat demande)
    Solver1(Problem p){
        solution = new float[p.n]; //solution est de dimension n
        solve(p);
    }
    //fonction qui calcule le resultats
    void solve(Problem p){
        for (int i=0; i<p.n; i++){
            solution[i] = (p.limits[i][0]+p.limits[i][1])/2.0;  //calculer le point milieu
        }
        return_value = p.f(solution); // calculer le resultat
    }
};

class Solver2{
public:
    /*
    -search_vector est un vecteur de taille n qui contient la valeur des variables durant le calcul
    -solution donne la derniere combinaison de variables qui minimise le probleme
    -step est un vecteur de taille n qui donne le pas de discritisation pour chaque dimension
    -return_value est la derniere valeur de la fonction f qui minimis le problem
    */
    float *search_vector;
    float *solution;
    float *step;
    float return_value;
    Solver2(Problem p){
        solution = new float[p.n];
        search_vector = new float[p.n];
        step = new float[p.n];
        solve(p);
    }
    void solve(Problem p){
        for(int dim=0; dim<p.n; dim++){
            solution[dim] = p.limits[dim][0];                           //commencer la recherche par les limits
            step[dim] = (p.limits[dim][1]-p.limits[dim][0])/9.0;
        }
        //initialisation de la solution
        return_value = p.f(solution);
        //appeler la fonction recurssive
        recloop(p, p.n-1);
    }

private:
    //  implementation d'une boucle recurssive
    void recloop(Problem p, int k){
        if (k<0){
            float value = p.f(search_vector);           //conserver la valeur min de la fonction
            if(value < return_value){
                return_value = value;
                for(int i=0; i<p.n; i++){
                    solution[i] = search_vector[i];     //conserver les solutions
                }
            }
            return;
        }
        else{
            for (float pt=p.limits[k][0]; pt<=p.limits[k][1]; pt+=step[k]){     //pour chaque dimension, calcler la nouvelle valeur des variables
                search_vector[k] = pt;
                recloop(p, k-1);
            }
        }
    }
};

class Solver3{
public:
     /*
    -search_vector est un vecteur de taille n qui contient la valeur des variables durant le calcul
    -solution donne la derniere combinaison de variables qui minimise le probleme
    -step est un vecteur de taille n qui donne le pas de discritisation pour chaque dimension
    -return_value est la derniere valeur de la fonction f qui minimis le problem
    */
    float *search_vector;
    float *solution;
    float *step;
    float return_value;
    Solver3(Problem p){
        solution = new float[p.n];
        search_vector = new float[p.n];
        step = new float[p.n];
        solve(p);
    }
    void solve(Problem p){
        for(int dim=0; dim<p.n; dim++){
            solution[dim] = (p.limits[dim][0]+p.limits[dim][1])/2.0;    //commencer par le point du centre
            step[dim] = (p.limits[dim][1]-p.limits[dim][0])/9.0;
        }
        //initialization de la solution
        return_value = p.f(solution);
        //appeler la fonction recursive
        recloop(p, p.n-1);
    }

private:
    // implementation d'une boucle recurssive
    void recloop(Problem p, int k){
        if (k<0){
            return;
        }
        else{
            for (float pt=p.limits[k][0]; pt<=p.limits[k][1]; pt+=step[k]){     //sur chaque dimension on evalue la valeur de la fonction
                float value = p.f(search_vector);
                if(value < return_value){
                    return_value = value;
                    solution[k] = search_vector[k];                             // ensuite on conserve la valeur optimal de la fonction et de la variable
                }
                search_vector[k] = pt;
            }
            recloop(p, k-1);                                                    //et on passe pour la prochaine dimension
        }
    }
};


class Solver4{
public:
     /*
    -search_vector est un vecteur de taille n qui contient la valeur des variables durant le calcul
    -solution donne la derniere combinaison de variables qui minimise le probleme
    -return_value reçoie la derniere valeur de la fonction f qui minimis le problem
    */
    float *search_vector;
    float *solution;
    float return_value;

    Solver4 (Problem p){
        solution = new float[p.n];
        search_vector = new float[p.n];
        solve(p);
    }

    void solve(Problem p){
        return_value =1000;                                 // on initialise par une grande valeur
        srand(time(0));
        for(int i=0;i<1000;i++){                            // on évalue la fonction sur 1000 points
            for(int dim=0; dim<p.n; dim++){                 //donner des valeurs aléatoire pour chaque variable
                    search_vector[dim] = p.limits[dim][0] + (float (rand())) /( float (RAND_MAX/(p.limits[dim][1]-p.limits[dim][0])));
                }
                float value = p.f(search_vector);
                if(value < return_value){
                    return_value = value;                   //conserver la valeur minimal
                    for(int i=0; i<p.n; i++){
                        solution[i] = search_vector[i];     //conserver les valeurs des variables
                    }
                }

        }
    }

};

class Solver5{
public:
    /*
    -search_vector est un vecteur de taille n qui contient la valeur des variables durant le calcul
    -solution donne la derniere combinaison de variables qui minimise le probleme
    -return_value reçoie la derniere valeur de la fonction f qui minimis le problem
    */
    float *search_vector;
    float *solution;
    float return_value;

    Solver5 (Problem p){
        solution = new float[p.n];
        search_vector = new float[p.n];
        solve(p);
    }

    void solve(Problem p){
        return_value =1000;                                     //inisialiser par une grande valeur
        srand(time(0));
        vector<vector<float>> new_limits(p.limits);             // New_limits permet d'avoir les nouvelles limites après chaque étiration
        // mème algorithme que S4 avec répitition 100 fois et actualisation des limites
        for(int j=0;j<100;j++){
            for(int i=0;i<1000;i++){                            // on évalue la fonction sur 1000 points
                for(int dim=0; dim<p.n; dim++){                 //donner des valeurs aléatoire pour chaque variable
                    search_vector[dim] = new_limits[dim][0] + (float (rand())) /( float (RAND_MAX/(new_limits[dim][1]-new_limits[dim][0])));
                }
                float value = p.f(search_vector);
                if(value < return_value){
                    return_value = value;                       //conserver la valeur minimal
                        for(int i=0; i<p.n; i++){
                            solution[i] = search_vector[i];     //conserver les valeurs des variables
                    }
                }
            }
            // actualiser les nouvelles limites
            for( int dim= 0;dim<p.n;dim++){
                if(new_limits[dim][0]< (new_limits[dim][0]+solution[dim])/2.0){ //vérifier que nous sommes toujours dans l'espace de recherche
                    new_limits[dim][0]=(new_limits[dim][0]+solution[dim])/2.0;
                }
                if(new_limits[dim][1] > (new_limits[dim][1]+solution[dim])/2.0){
                    new_limits[dim][1]=(new_limits[dim][1]+solution[dim])/2.0;
                }
            }
        }

    }

};

class Solver6{
public:
     /*
    -search_vector_right/left est un vecteur de taille n qui contient la valeur des variables durant le calcul (à droite et à gauche)
    -solution donne la derniere combinaison de variables qui minimise le probleme
    -step est le pas; larg contien la largeur de l'intervale le plus grand
    -return_value est la derniere valeur de la fonction f qui minimis le problem
    */
    float *search_vector_right;
    float *search_vector_left;
    float *solution;
    float step,larg;
    float return_value;
    int dimension;      // prend la dimesion de la variable de grande interval de recherche
    Solver6(Problem p){
        solution = new float[p.n];
        search_vector_right = new float[p.n];
        search_vector_left = new float[p.n];
        solve(p);
    }
    void solve(Problem p){
        larg=p.limits[0][1]-p.limits[0][0];     //inisialiser la largeur
        for(int dim=0; dim<p.n; dim++){
            solution[dim] = (p.limits[dim][0]+p.limits[dim][1])/2.0;    //inisialiser les variables par le point milieu
            if (larg < p.limits[dim][1]-p.limits[dim][0]){
                larg = p.limits[dim][1]-p.limits[dim][0];   //conserver la grande largeur
                dimension=dim;
            }
            step = larg/10.0;                               //calculer le pas
        }
        //initialization of the solution
        return_value = p.f(solution);

            for (float pt=(p.limits[dimension][0]+p.limits[dimension][1])/2.0; pt<=p.limits[dimension][1]; pt+=step){
                for (int dim=0; dim< p.n; dim++){

                    if(pt > p.limits[dim][1])             // Vérifier que nous sommes toujours dans l'espace de recherche
                        search_vector_right[dim]= p.limits[dim][1];

                    if(pt < p.limits[dim][0])              // Vérifier que nous sommes toujours dans l'espace de recherche
                        search_vector_left[dim]= p.limits[dim][0];

                    else {  search_vector_right[dim]= pt;       //calculer le ponit du droit
                            search_vector_left[dim]= - pt;}     //calculer le ponit du gauche

                }
                if(return_value > p.f(search_vector_left) || return_value > p.f(search_vector_right) ){
                    if(p.f(search_vector_left) < p.f(search_vector_right)){
                        return_value = p.f(search_vector_left);     //retourner la valeur min
                    }
                    else {
                        return_value = p.f(search_vector_right);
                    }
                    }
                }
    }
};

class Solver7{
public:
      /*
    -search_vector_right/left est un vecteur de taille n qui contient la valeur des variables durant le calcul (à droite et à gauche)
    -solution donne la derniere combinaison de variables qui minimise le probleme
    -step est le pas; larg contien la largeur de l'intervale le plus grand
    -return_value est la derniere valeur de la fonction f qui minimis le problem
    */
    float *search_vector_right;
    float *search_vector_left;
    float *solution;
    float step,larg;
    float return_value;
    int dimension;                           // prend la dimesion de la variable de grande interval de recherche
    Solver7(Problem p){
        solution = new float[p.n];
        search_vector_right = new float[p.n];
        search_vector_left = new float[p.n];
        solve(p);
    }
    void solve(Problem p){
        //meme algorithme que S6 avec division de pas dans chaque itération
        larg=p.limits[0][1]-p.limits[0][0];
        for(int dim=0; dim<p.n; dim++){
            solution[dim] = (p.limits[dim][0]+p.limits[dim][1])/2.0;    //inisialiser les variables par le point milieu
            if (larg < p.limits[dim][1]-p.limits[dim][0]){      //conserver la grande largeur
                larg = p.limits[dim][1]-p.limits[dim][0];
                dimension=dim;
            }
            step = larg/10.0;
        }
        //initialization de la solution
        return_value = p.f(solution);
           for(step; step>pow(10,-3);step/=10){     //répetition jusqu'avoir un pas < 10^(-3)
                cout<< "step"<< step<<endl;

                for (float pt=(p.limits[dimension][0]+p.limits[dimension][1])/2.0; pt<=p.limits[dimension][1]; pt+=step){
                    for (int dim=0; dim< p.n; dim++){
                        if(pt > p.limits[dim][1]){              // Vérifier que nous sommes toujours dans l'espace de recherche
                            search_vector_right[dim]= p.limits[dim][1];
                        }
                        if(pt < p.limits[dim][0]){              // Vérifier que nous sommes toujours dans l'espace de recherche
                            search_vector_left[dim]= p.limits[dim][0];
                        }
                        else {  search_vector_right[dim]= pt;       //calculer le ponit du gauche
                                search_vector_left[dim]= - pt;      //calculer le ponit du droite
                                }
                    }
                    if(return_value > p.f(search_vector_left) || return_value > p.f(search_vector_right) ){
                        if(p.f(search_vector_left) < p.f(search_vector_right)){
                            return_value = p.f(search_vector_left);
                        }
                        else {
                            return_value = p.f(search_vector_right);//retourner la valeur min
                        }
                    }
                }
            }
    }
};

int main(){
    float num_prob,num_solv;

    cout << "donner le numero du probleme " << endl;
    cin >> num_prob;
    cout << "donner le numero du solveur " << endl;
    cin >> num_solv;

    Problem p;
    p.num_prob = num_prob;
    p.declare_limits();     //declaration des limites
    p.define_limits();      //definition des limites

    string const nomFichier("result.txt");
    ofstream Fichier(nomFichier.c_str());
    if(Fichier){
    //donner la solution selon le numéro du solveur et du problème
     if(num_solv==1){
        Solver1 s(p);
        Fichier <<"resultats_optim="<<s.return_value <<endl;
        for (int i=0; i<p.n; i++){
        Fichier <<"variable[" <<i <<"]: " <<s.solution[i] <<endl;
    }}
    if(num_solv==2){
        Solver2 s(p);
        Fichier <<s.return_value <<endl;
        for (int i=0; i<p.n; i++){
        Fichier <<"dim" <<i <<": " <<s.solution[i] <<endl;
    }}
    if(num_solv==3){
        Solver3 s(p);
        Fichier <<s.return_value <<endl;
        for (int i=0; i<p.n; i++){
        Fichier <<"dim" <<i <<": " <<s.solution[i] <<endl;
    }}
    if(num_solv==4){
        Solver4 s(p);
        Fichier <<s.return_value <<endl;
        for (int i=0; i<p.n; i++){
        Fichier <<"dim" <<i <<": " <<s.solution[i] <<endl;
    }}
    if(num_solv==5){
        Solver5 s(p);
        Fichier <<s.return_value <<endl;
        for (int i=0; i<p.n; i++){
        Fichier <<"dim" <<i <<": " <<s.solution[i] <<endl;
    }}
    if(num_solv==6){
        Solver6 s(p);
        Fichier <<s.return_value <<endl;
        for (int i=0; i<p.n; i++){
        Fichier <<"dim" <<i <<": " <<s.solution[i] <<endl;
    }}
    if(num_solv==7){
        Solver7 s(p);
        Fichier <<s.return_value <<endl;
        for (int i=0; i<p.n; i++){
        Fichier <<"dim" <<i <<": " <<s.solution[i] <<endl;
    }}
    }
    else { cout <<"ERREUR: Impossible d'ouvrir le fichier."<<endl;}

}
