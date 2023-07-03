#include <iostream>
#include <thread>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

short int result;

struct topFive{
    string nome;
    int pon;
};

void resp(int *pontos,bool *acertou);
void gerarEquacoes(short int *n1, short int *n2, short int *n3, bool *acertou);
bool usuarioNovo(string user);
void cadastro(string user, string senha, int pontos);
void updatePontos(string user, string senha, int *pontos, short int *posicao);
void pontuacaoAnterior(string *pLinha, int *pontAnterior);
void updatePontos(string user, string senha, int *pontos);
bool verificarAutenticidade(string usu, string sen, string *pLinha, short int *posicao);
map<string, int> listarPontuacao();
vector<pair<int, string>> ordenarRanking(map<string, int> myMap);

int main() {

    short int cTime, op,  cont, p, n1, n2, n3, posicao;
    int pontos , pontAnterior;
    string linha, user, senha, n;
    bool flag, acertou;
    map<string, int> myMap;
    vector<pair<int, string>> sortedVector;
    topFive top[5];
    
    do {
       cout << "\e[H\e[2J";
       cout << "\n\n\t\t      MATH KIDS\n\n";
       cout << "----------------------------------------";
       cout << "\n\t\t\t1 - Entrar";
       cout << "\n\t\t\t2 - Criar conta";
       cout << "\n\t\t\t3 - Sair";
       cout << "\n----------------------------------------\n\n: ";
       cin >> op;
       switch (op) {
            case 1: 
                cout << "\e[H\e[2J";
                cout << "\n----------------------------------------";

                //leitura do login
                getchar();
                cout << "\n\nDigite o seu usuario: ";
                getline(cin, user);
                cout << "\nDigite a sua senha: ";
                getline(cin, senha);

                //verificando credenciais
                flag = verificarAutenticidade(user, senha, &linha, &posicao);

                if(flag){
                    pontuacaoAnterior(&linha, &pontAnterior);
                    do{
                        cTime = 60;
                        pontos = 0;
                        do{
                            cout << "\e[H\e[2J";
                            cout << "\n\n\t\t      MATH KIDS\t\t\t    \n";
                            cout << "\n-----------------------------------------";
                            cout << "\nQuestao: " << pontos+1 << "\t\t     Tempo restante:  " << cTime;
                            cout << "\n-----------------------------------------\n";
                            cTime--;
                            gerarEquacoes(&n1, &n2, &n3, &acertou);
                            thread t1(resp, &pontos, &acertou);
                            t1.detach();
                            cout << "\n-----------------------------------------\n\n= ";
                            sleep(1);    
                        }while(cTime > 0);
                        sleep(1);
                        cout << "\e[H\e[2J";
                        
                        //guardar maior pontuação de cada jogador
                        if(pontAnterior < pontos){
                            updatePontos(user,senha, &pontos, &posicao);
                        }

                        cout << "\n-----------------------------------------";
                        cout << "\n\t\t\t\t  TOP 5";
                        cout << "\n-----------------------------------------";
                         cout << "\n\t\t\tPlayer \tPontos\n";

                        myMap.clear();
                        sortedVector.clear();

                        myMap = listarPontuacao();
                        sortedVector = ordenarRanking(myMap);                    
                        //Rankear
                        cont = -1;                  
                        for (const auto& pair : sortedVector) {
                            cont++;
                            p = pair.first;
                            n = pair.second;
                            if(cont <= 4){
                                top[cont].nome = n;
                                top[cont].pon =  p;
                            }else{ //deslocar 
                                for(int i = 1; i <= 4; i++){
                                    top[i-1].nome = top[i].nome;
                                    top[i-1].pon = top[i].pon;
                                }
                                top[cont-1].nome = n;
                                top[cont-1].pon = p;
                                cont = 4;
                            } 
                        }

                        for(int i = cont; i >= 0; i--){
                            cout << "\t\t\t "<< top[i].nome << "\t  " << top[i].pon << " \n";
                        }
                        cout << "-----------------------------------------\n";
                        sleep(10);
                    }while(true);
                }else{
                    cout << "\nUsuario ou senha invalidos!\n";    
                    cout << "\n----------------------------------------\n";
                    sleep(2);
                    cout << "\e[H\e[2J";
                }        
                
            break;
        case 2: //CADASTRO
            cout << "\e[H\e[2J";
            cout << "\n----------------------------------------\n";
            cout << "\nDigite um username: ";
            getchar();
            getline(cin, user);
            flag = usuarioNovo(user);
            if(flag){
                cout << "\nDigite a senha: ";
                getline(cin,senha);
                cadastro(user, senha, 0);
                cout << "\nUsuario cadastrado com sucesso!\n";
            }else cout << "\nUsername existente, digite outro!\n";
            cout << "\n----------------------------------------\n";
            sleep(2);
            cout << "\e[H\e[2J";
            break;
        case 3: cout << "\nSaindo...";
            break;
        default: cout << "\nOpcao invalida, digite novamente!";
       }
    }while(op != 3);
}
//Verifica se o resultado que o player falou é igual ao da equeção
void resp(int *pontos, bool *acertou){ 
    int resultF;   
    cin >> resultF;
    if(resultF == result){
        *acertou = true;
        *pontos += 1;
    }  
}
//Gera valores e operadores Aleatoriamente
void gerarEquacoes(short int *n1, short int *n2, short int *n3, bool *acertou){  
    if(*acertou){
        srand(time(0));
        *n1 = rand() % 10;
        *n2 = rand() % 10;
        *n3 =  rand() % 4; 
        *acertou = false;
    }
    cout << "\n";
    switch(*n3){
        case 0:  cout << *n1 << " + " << *n2 << "\n";
                result = *n1+*n2;          
            break;
        case 1:  cout << *n1 << " - " << *n2 << "\n";
                result = *n1 - *n2;
            break;
        case 2: cout << *n1 << " *  " << *n2 << "\n";
                result = *n1*(*n2);
            break;
        case 3: if(*n2 == 0) *n2 = 1;
                cout << *n1 << " /  " << *n2 << "\n";
                result = *n1/(*n2);
            break;
    }
}
//Verifica se o usuário já existe
bool usuarioNovo(string user){
    bool flag = true;
    fstream arquivo;
    string linha, linhaAux;
    short int i;

    arquivo.open("data.txt", ios::in);
    while(getline(arquivo,linha)){
        i = 0;
        while(linha[i] != ','){
            linhaAux += linha[i];
            i++;
        }
        if(user == linhaAux) flag = false;
        linhaAux = "";          
    }
    arquivo.close();

    return flag;
}
//Armazenando no txt os dados do novo player
void cadastro(string user, string senha, int pontos){
    fstream arquivo;
    arquivo.open("data.txt", ios::app);
    arquivo << user << ",";
    arquivo << senha << ","<< pontos << ",\n";
    arquivo.close();
}
//Verifica as credenciais para liberar o acesso ao jogo
bool verificarAutenticidade(string usu, string sen, string *pLinha, short int *posicao){
    fstream arquivo;
    bool flag = false;
    string line, linhaAux;
    short int i, linhaAtual = -1;

    arquivo.open("data.txt", ios::in);
    if(arquivo.is_open()){
        while(getline(arquivo,line)){
            i = 0;
            while(line[i] != ','){ //procurar user
                linhaAux += line[i];
                i++;
            }
            linhaAtual++;

            if(linhaAux == usu){//encontrei
                linhaAux = "";
                *posicao = linhaAtual;
                i+=1;
                while(line[i] != ','){
                    linhaAux += line[i];
                    i++;
                }
                if(linhaAux == sen){
                    flag = true;
                    *pLinha = line;
                } 
            }
            linhaAux = "";
        }
        arquivo.close();
    }
    return flag;
}
//Guarda a pontuação anterior
void pontuacaoAnterior(string *pLinha, int *pontAnterior){
    string linhaAux,line = *pLinha;
    short int tam = line.size()-1, i = 0;

    while(i < tam){
        if(line[i] != ',') linhaAux+=line[i];
        else  linhaAux = "";     
        i++;
    }
    *pontAnterior = stoi(linhaAux);
}
//Altera a pontuação do player
void updatePontos(string user, string senha, int *pontos, short int *posicao){
    fstream arquivo, arqTem;
    string linha, linhaAux, date = "data.txt", dateTem = "dataTem.txt";
    short int atual = 0;

    arquivo.open("data.txt", ios::in);
    arqTem.open("dataTem.txt", ios::out);

    if(arquivo.is_open() || arqTem.is_open()){
        while(getline(arquivo, linha)){
            if(atual != *posicao){//Não armazenar a linha removida 
                arqTem << linha << "\n";
            }else{
                arqTem  <<user << ",";
                arqTem << senha << ","<< *pontos << ",\n";
            }
            atual++;
        }
    }

    arquivo.close();
    arqTem.close();
    remove(date.c_str());
    rename(dateTem.c_str(), date.c_str());//aletarndo o nome *do arquivo temporario 
}
//Armazena os dados do txt em um map
map<string, int> listarPontuacao(){
    fstream arquivo;
    map<string, int> myMap;
    string linha, linhaAux,chave;
    short int i, cont, atual, value;

    arquivo.open("data.txt", ios::in);
    while(getline(arquivo,linha)){
        i = 0;
        cont = 0;
        while(linha[i] != ','){ //procurar user
            linhaAux += linha[i];
            i++;
        }
        chave = linhaAux; 
        linhaAux = "";
        do{
            i+=1;
            if(cont == 1) linhaAux = "";
            while(linha[i] != ','){
                linhaAux += linha[i];
                i++;
            }
            cont++;
        }while(cont < 2);

        value = stoi(linhaAux);
        myMap[chave] = value;
        linhaAux = "";
    }
    arquivo.close();
    return myMap;  
}
//Joga os dados do map em um vector e ordena ele
vector<pair<int, string>> ordenarRanking(map<string, int> myMap){
    vector<pair<int, string>> sortedVector;

    for (const auto& pair : myMap) {
        sortedVector.push_back(make_pair(pair.second, pair.first));
    }
    sort(sortedVector.begin(), sortedVector.end());
    
    return sortedVector;
}
