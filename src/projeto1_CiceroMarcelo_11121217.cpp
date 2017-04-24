// Tempo de retorno: tempo no qual o processo espera para ser finalizado (espera + execução)
// Tempo de resposta: tempo esperando para iniciar execução;
// Tempo de espera: tempo gasto na fila de prontos;
// Para executar, fazer uso do CAT
// EXEMPLO: cat entrada.txt | ./projeto2
// Espera e resposta so sao diferentes em algoritmos de uso compartilhado, nesse exemplo, apenas no RR
// Nos demais, resposta e espera serao iguais

// Cicero Marcelo Louro Leite
// Matricula 11121217
// Universidade Federal da Paraiba
// Ciencia da Computacao - Sistemas Operacionais - Trabalho 1
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <vector>
#include <cstdlib>
using namespace std;
#define quantum 2

struct Process {
  double ent;
  double dur;
  bool done;
  double rt;
  int esp, ret;
};


/* ORDENA POR CHEGADA*/
void selectionChegada(vector<Process>& procVec, int tam)
{
  int pos_min,ent,dur,rt,esp,ret;
  int size=tam;

	for (int i=0; i < tam-1; i++)
	{
	  pos_min = i;
		for (int j=i+1; j < tam; j++)
		{
      if (procVec[j].ent < procVec[pos_min].ent)
        pos_min=j;
		}
    if (pos_min != i)
    {
      ent = procVec[i].ent;
      dur = procVec[i].dur;
      rt = procVec[i].rt;
      esp = procVec[tam].esp;
      ret = procVec[tam].ret;
      procVec[i].ent = procVec[pos_min].ent;
      procVec[i].dur = procVec[pos_min].dur;
      procVec[i].rt = procVec[pos_min].rt;
      procVec[i].esp = procVec[pos_min].esp;
      procVec[i].ret = procVec[pos_min].ret;
      procVec[pos_min].ent = ent;
      procVec[pos_min].dur = dur;
      procVec[pos_min].rt = rt;
      procVec[pos_min].esp = esp;
      procVec[pos_min].ret = ret;
    }
	}
};

//
/* >>>>>> FUNCTION FCFS <<<<<<< */
//
int fcfs (vector<Process>& procVec, int tam)
{
  double rettot = 0, esptot = 0, reptot = 0;
  int tempo=0,espera=0,retorno=0;
  for (int i = 0; i<tam; i++)
  {
    if (procVec[i].ent<=tempo)
    {
      esptot = esptot + (tempo-procVec[i].ent);
      espera = tempo-procVec[i].ent;
      rettot += espera + procVec[i].dur;
      tempo += procVec[i].dur;
    }
    else
    {
      rettot += procVec[i].dur;
      tempo = procVec[i].ent+procVec[i].dur;
    }
    //std::cout << i << std::endl;
    //std::cout << "espera total " << esptot << std::endl; // CALCULAR MEDIA DA ESPERA
    //std::cout << "retorno total " << retorno << std::endl; // CALCULAR MEDIA DE RETORNO

  }
  std::cout.precision(5);
  std::cout << "FCFS " << rettot/tam << /*" Media espera*/" " << esptot/tam << " " << esptot/tam << std::endl;
}

//
/* >>>>>> FUNCTION SJF <<<<<<< */
//
int proximoPros(vector<Process>& procVec, int tam, int tempo)
{
  int prox=-1, maxAtual=9999;
  for (int i=0; i<tam; i++)
  {
    if (procVec[i].ent <= tempo && procVec[i].done==false) // verifica se ja ta na fila  e se ja foi executado
    {
      if (procVec[i].dur < maxAtual)
      {
        prox=i;
        maxAtual = procVec[i].dur;
        //std::cout << "proximo atual sera " << prox << " de duracao " << procVec[i].dur << std::endl;
        //std::cout << "tempo de execucao = " << tempo << std::endl;
      }
    }
  }
  return prox;
}

int sjf (vector<Process>& procVec, int tam)
{
  double rettot = 0, esptot = 0, reptot = 0;
  int tempo=0,espera=0,retorno=0;
  int prox = -1;
  int tam2=tam;
  for (int i=0; i<tam2; i++)
  {
    prox = proximoPros(procVec, tam, tempo);
    if (prox >= 0)
    {
      //std::cout << prox << " executado" << std::endl;
      procVec[prox].done = true;
      procVec[prox].esp = tempo-procVec[prox].ent;
      esptot+=procVec[prox].esp;
      procVec[prox].ret=procVec[prox].esp+procVec[prox].dur;
      rettot = rettot + procVec[prox].ret;
      tempo = tempo + procVec[prox].dur;
      //rettot = rettot + procVec[prox].ent + procVec[prox].dur - tempo;
      prox = -1;
    }
    else
    {
      //std::cout << "nenhum entrou" << std::endl;
      tam2++;
      tempo+=1;
    }
  }
  std::cout.precision(5);
  std::cout << "SJF " << rettot/tam << /*" Media espera SJF*/" " << esptot/tam << " " << esptot/tam << std::endl;
}


//
/* >>>>>> FUNCTION RR <<<<<<< */
//
int rr (vector<Process>& procVec, int tam)
{
  int restante = tam;
  double resposta=0, retorno=0, espera=0, tempo=0;
  int fg=0, i=0, laco=0;
  while(i<tam)
  {
    for (int j=0; j<tam;)
    {
      //verifica se o processo atual ja terminou a execucao
      if (procVec[j].done==true)
      {
        j++;
      }
      //verifica se o processo atual vai terminar a execucao agora
      else if(procVec[j].rt<=quantum && procVec[j].rt>0 && procVec[j].ent<=tempo)
      {
        if (procVec[j].rt==procVec[j].dur)
        {
          resposta=resposta+tempo-procVec[j].ent;
        }
        //std::cout << "***executou " << procVec[j].rt << " do processo " << j << std::endl;
        tempo+=procVec[j].rt;
        procVec[j].esp=tempo-procVec[j].ent-procVec[j].dur;
        procVec[j].ret=procVec[j].dur+procVec[j].esp;
        procVec[j].rt = procVec[j].rt - quantum;
        fg = 1;
        i++;
        procVec[j].done = true;
        //std::cout << "processo finalizado " << j << std::endl;
        //std::cout << "tempo executando " << tempo << std::endl;
        j++;
      }
      //verifica se o processo atual ainda nao terminou e nem vai terminar agora
      else if (procVec[j].rt > quantum && procVec[j].ent<=tempo)
      {
        if (procVec[j].rt==procVec[j].dur)
        {
          resposta=resposta+tempo-procVec[j].ent;
        }
        tempo+=quantum;
        //std::cout << "***executou q" << quantum << " do processo " << j << std::endl;
        //std::cout << "tempo executando " << tempo << std::endl;
        procVec[j].rt = procVec[j].rt - quantum;
        //std::cout << "falta " << procVec[j].rt << " do processo " << j << std::endl;
        j++;
        fg = 1;
      }
      //verifica se nesse tempo nao ha nenhum processo querendo executar
      else if (laco>0)
      {
        //std::cout << "entrou no break" << std::endl;
        break;
      }
      //verifica se o processo atual ja entra em execucao ou ainda nao
      else if (procVec[j].ent > tempo )
      {
        //std::cout << "processo " << j << " ainda nao entra" << std::endl;
        j=0;
        laco++;
      }
    }
    //Verifica se nennum processo entrou, se nao, aumenta 1 no tempo e volta a verificar
    if (fg==0)
    {
      //std::cout << "nao entra ngm" << std::endl;
      tempo++;
      //std::cout << "tempo " << tempo << std::endl;
    }
    fg=0;
    laco=0;
  }
  //std::cout << tempo << std::endl;
  for (int aux=0; aux<tam; aux++)
  {
    //std::cout << aux << "espera " << procVec[aux].esp << " : retorno " << procVec[aux].ret << std::endl;
    retorno=retorno+procVec[aux].ret;
    espera=espera+procVec[aux].esp;
  }
  std::cout.precision(5);
  std::cout << "RR " << retorno/tam << /*" Media resposta RR*/" " << resposta/tam << /*" Media espera RR*/" " << espera/tam << std::endl;

}

//
/* >>>>>>> MAIN <<<<<<< */
//
int main()
{
  std::vector<Process> procVec; // cria vector de processos
  double x, y;
  int tam=0;
  while(std::cin >> x >> y) // le do arquivo pelo cat cmd
  //while (tam<5)
  {
    procVec.push_back(Process());
    procVec[tam].ent = x;
    procVec[tam].dur = y;
    procVec[tam].done = false;
    procVec[tam].esp = 0;
    procVec[tam].ret = 0;
    tam++;
  }
  for (int j=0; j<tam; j++)
  {
    // std::cout << procVec[j].ent << " e " << procVec[j].dur << std::endl;
    procVec[j].rt = procVec[j].dur;
  }
  selectionChegada(procVec, tam);
  // for (int j=0; j<tam; j++)
  // {
  //   std::cout << procVec[j].ent << " " << procVec[j].dur << std::endl;
  // }
  fcfs(procVec, tam);
  sjf(procVec, tam);
  for (int aux=0; aux<tam; aux++)
  {
    procVec[aux].done = false;
    procVec[aux].ret = 0;
    procVec[aux].esp = 0;
  }
  rr(procVec, tam);


}
