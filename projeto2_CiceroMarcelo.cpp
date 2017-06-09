// Cicero Marcelo Louro Leite
// Matricula 11121217
// Universidade Federal da Paraiba
// Ciencia da Computacao - Sistemas Operacionais - Trabalho 2
// Compilar no terminal: g++ projeto2_CiceroMarcelo.cpp -o projeto2_CiceroMarcelo
// Executar no terminal: cat entrada.txt | ./projeto2_CiceroMarcelo
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct Pag {
  int id;
};

int ram;


/* ---------- */
/* FIFO */
/* ---------- */
int fifo(vector<Pag>& pagVec, int tam, int sizeram)
{
  int y=0, prox=0, troca=0;
  int array[sizeram];
  for (int i=0; i<sizeram;i++) // Inicializa array da ram
  {
    array[i] = -1;
  }
  bool flag = false;
  for (int i=0; i<tam; i++) // for para todas as paginas
  {
    for (int y=0; y<sizeram;y++) // verifica se ja esta na ram
    {
      if (pagVec[i].id == array[y])
      {
        flag = true; // avisa que esta na ram
      }
    }
    if (flag == false) // Caso nao esteja
    {
      if (prox >= sizeram)
      {
        prox = 0;
      }
      array[prox] = pagVec[i].id; // primeiro que entrou sai para o proximo entrar
      troca+=1;
      prox++;
    }
    flag=false;
  }
  std::cout << "FIFO " << troca << std::endl;
}
//
//

/* ---------- */
/* OTIMO */
/* ---------- */
int otimo(vector<Pag>& pagVec, int tam, int sizeram)
{
  int y=0, vai_sair=-2, troca=0, max=-1;
  int array[sizeram];
  for (int i=0; i<sizeram;i++) // Inicializa array da ram
  {
    array[i] = -1;
  }
  bool flag = false;
  bool fl2 = false;
  for (int i=0; i<tam; i++) // for para todas as paginas
  {//for1
    for (int y=0; y<sizeram;y++) // verifica se ja esta na ram
    {//for2
      if (pagVec[i].id == array[y])
      {
        flag = true; // avisa que esta na ram
      }
    }//for2
    if (flag == false) // Caso nao esteja
    {//if1
      for (int auxram=0; auxram<sizeram;auxram++) // varre a ram
      {//for2
        for (int auxvec=i+1; auxvec<tam; auxvec++) // varre array para cada pagina na ram
        {//for3
          if (array[auxram]==pagVec[auxvec].id) // verifica se achou no array quem ja esta na ram
          {//if2
            fl2=true; // verifica q a pagina atual esta na ram
            if (auxvec>max) // caso apareca depois do atual "mais longe", grava a nova posicao
            {//if3
              max=auxvec;
              vai_sair=auxram;
            }//if3
            break;
          }//if2
        }//for3
        if (fl2==false) // se a pagina da ram que esta sendo comparada nao aparece de novo, ela saira
        {
          vai_sair=auxram;
          break;
        }
        fl2=false;
      }//for2
      array[vai_sair]=pagVec[i].id; // faz a troca em si
      troca+=1; // incrementa troca
    }//if1
    flag=false;   //
    max=-2;       // Reseta valores para proxima comparacao
    vai_sair=-2;  //
  }//for1
  std::cout << "OTM " << troca << std::endl;
}//otimo
//
//
// LRU funciona como o OTIMO, porem comparando para "tras" do array de paginas, a parte ja lida
/* ---------- */
/* LRU */
/* ---------- */
int lru(vector<Pag>& pagVec, int tam, int sizeram)
{
  int y=0, vai_sair=-2, troca=0, min=999;
  int array[sizeram];
  for (int i=0; i<sizeram;i++) // Inicializa array da ram
  {
    array[i] = -1;
  }
  bool flag = false;
  bool fl2 = false;
  for (int i=0; i<tam; i++) // for para todas as paginas
  {//for1
    for (int y=0; y<sizeram;y++) // verifica se ja esta na ram
    {//for2
      if (pagVec[i].id == array[y])
      {
        flag = true; // avisa que esta na ram
      }
    }//for2
    if (flag == false) // Caso nao esteja
    {//if1
      for (int auxram=0; auxram<sizeram;auxram++) // comparar com a ram
      {//for2
        for (int auxvec=i-1; auxvec>=0; auxvec--) // comparar com o que ja passou
        {//for3
          if (array[auxram]==pagVec[auxvec].id) //se encontra a pagina atual na ram
          {//if2
            fl2=true;
            if (auxvec<min) // caso a comparada agora apareceu a mais tempo que a gravada anterior
            {//if3
              min=auxvec; // grava aonde esta
              vai_sair=auxram;
            }//if3
            break;
          }//if2
        }//for3
        if (fl2==false)
        {
          vai_sair=auxram;
          break;
        }
        fl2=false;
      }//for2
      array[vai_sair]=pagVec[i].id;
      troca+=1;
    }//if1
    flag=false; //
    min=999;    // REseta variaveis para proxima comparacao
    vai_sair=-2;//
  }//for1
  std::cout << "LRU " << troca << std::endl;
}//otimo
//
//

/* ---------- */
/* MAIN */
/* ---------- */
int main()
{
  std::vector<Pag> pagVec; // cria vector de paginas
  int x;
  int tam=0;
  std::cin >> ram; // PRimeira linha eh o tamanho da ram
  while(std::cin >> x) // le do arquivo pelo cat cmd
  {
    pagVec.push_back(Pag());//
    pagVec[tam].id = x;     // atribui o numero das paginas para as posicoes do vetor
    tam++;                  //
  }
  fifo(pagVec, tam, ram);
  otimo(pagVec, tam, ram);
  lru(pagVec, tam, ram);
}
