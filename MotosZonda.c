// programa em c que controla a entrada de servicos, concertos e valores recebidos e os guarda em arquivo.
//Registrando tbm o historico financeiro do ultimo mes de uma determinada oficina chamada Vzonda.
// gerenciamento de uma oficina*
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

//struct tipo moto , contem dados do veiculo e defeito
struct RegistroMoto {
    char Nome[20];
    char Modelo[10];
    char Placa[8];
    char Defeito[50];
    char Status;
    float Preco;
};
typedef struct RegistroMoto TpRegMoto;
#define tamanhoVet 50 
TpRegMoto VZonda[tamanhoVet];

//tipo de dado do registro contendo dados do pagamento , usado no arquivo de historico financeiro
struct registroPagamentos{
    char nome[20];
    char data[11];
    float preco;
    char placa[8];
} Financas[1000];

int Quant = -1; //Controla o preenchimento do vetor registro moto
// controle preenchimento vetor pagamento
int controleVetorPagamento = 0; 
//arquivos lógicos que armazenam servicos pendentes e valores recebidos pelos servicos
FILE *servicosPendentes;
FILE *financeiro;
//tamanho de cada vetor para salvamento em arquivo
long int tamanhoRegMoto = 89 * sizeof(char) + sizeof( float );
long int tamanhoRegPag = 39 * sizeof(char) + sizeof( float) ;

void SolicitaServico() {
    /* (1) Solicitar Serviço – quando se insere os dados supracitados, e também os campos status com valor zero
     sinalizando que o serviço ainda não foi feito, preço, também iniciado com zero.  Este deve estar em loop. */
    char Sair = 'S';
    do {
        printf("\n\n >>> Motos Zonda <<< \n\n");
        Quant = Quant + 1;
        printf("Qual o nome do cliente? ");
        scanf(" %[^\n]s", VZonda[Quant].Nome);
        printf("Qual o modelo da moto? ");
        scanf(" %[^\n]s", VZonda[Quant].Modelo);
        printf("Qual a placa da moto? ");
        scanf(" %[^\n]s", VZonda[Quant].Placa);
        printf("Qual o defeito da moto? ");
        scanf(" %[^\n]s", VZonda[Quant].Defeito);
        VZonda[Quant].Status = '0';
        printf("\n\n Deseja inserir novo servico? S|N ");
        scanf(" %c", &Sair);
        Sair = toupper(Sair);
    } while (Sair != 'N');
}

void IniciaServico() {
    /*(2) Iniciar Serviço – quando o status de uma dada moto (placa) é iniciado pelo mecânico e o status para a ser um.*/
    char P[8];
    printf("Placa da moto para iniciar serviço: ");
    scanf(" %[^\n]s", P);
    int Pos = -1; //posição da moto P no vetor
    //busca da moto para iniciar serviço
    for (int Cont = 0; Cont <= Quant; Cont++)
        if (strcmp(VZonda[Cont].Placa, P) == 0)
            Pos = Cont;
    if (Pos == -1)
        printf("Moto não cadastrada!");
    else {
        printf("\n Cliente %d: %s", Pos + 1, VZonda[Pos].Nome);
        printf("\n Modelo: %s", VZonda[Pos].Modelo);
        printf("\n Placa: %s", VZonda[Pos].Placa);
        printf("\n Defeito: %s", VZonda[Pos].Defeito);
        VZonda[Pos].Status = '1';
        printf("\n Status: %c", VZonda[Pos].Status);
    }
}

void RemoverSolicitacao(){
    int i , controle = 0;
    char placa[8];
    printf(" >>>  Remover Solicitacao  <<< \n");
    printf("Digite a placa da moto para cancelar solicitacao:  \n");
    scanf(" %[^\n]s", placa);
    for( i = 0; i < 50; i++){
        if(strcmp(VZonda[i].Placa, placa) == 0){
            VZonda[i].Status = '2';
            controle = i ;
        }
    }
    if(controle == 0)
        printf("Solicitacao nao encontrada!!!");
    else
        printf("Remocao concluida com sucesso!!!");
}

void ConsultarSolicitacoes(){
  printf("\n\n >>> Motos Zonda <<< \n\n");
  if (Quant==-1)
    printf("Nao ha servicos cadastrados.");
  else{   
      int semSolicitacao = 0 ;
      for (int Cont=0; Cont <= Quant; Cont++){
        if(VZonda[Cont].Status == '0' ){
            printf("\n Cliente %d: %s", Cont + 1 , VZonda[Cont].Nome);
            printf("\n Modelo: %s", VZonda[Cont].Modelo);
            printf("\n Placa: %s", VZonda[Cont].Placa);
            printf("\n Defeito: %s", VZonda[Cont].Defeito);
            printf("\n Status: %c",VZonda[Cont].Status);
            if (VZonda[Cont].Preco==0)
                printf("\n Preco: NAO DEFINIDO");       
            else
                printf("\n Preco: %.2f",VZonda[Cont].Preco);
            printf("\n --------------------- \n\n");
            semSolicitacao ++;
        }
      }
      if(semSolicitacao == 0)
            printf("Sem Novas solicitacoes!!!");
    }
}


void ConcluirServico() {
    //localização do veiculo , adição do valor recebido pelo servico e salvamento em arquivo
    char placa[8];
    printf("Digite a placa da moto que deseja concluir o serviço:\n");
    scanf(" %[^\n]s", placa);
    int achou = -1;
    financeiro = fopen("HistFinancas.dat","a+b");
    fseek(financeiro, tamanhoRegPag ,SEEK_END );
    for(int Cont = 0; Cont <= Quant; Cont++) {
        if (strcmp(VZonda[Cont].Placa, placa) == 0) {
            achou = Cont;
            break;
        }
    }
    if (achou != -1) {
        printf("Nome do dono: %s\n", VZonda[achou].Nome);
        printf("Moto: %s\n", VZonda[achou].Modelo);
        printf("placa: %s \n", VZonda[achou].Placa);
        strcpy(Financas[controleVetorPagamento].nome, VZonda[achou].Nome);
        strcpy(Financas[controleVetorPagamento].placa , VZonda[achou].Placa);
        printf("Valor do concerto: R$ ");
        scanf("%f", &Financas[controleVetorPagamento].preco);
        printf("Data de conclusao: "); 
        scanf(" %[^\n]s",  Financas[controleVetorPagamento].data);
        VZonda[achou].Status = '1';
        printf("Serviço Concluído!");
        fwrite(&Financas[controleVetorPagamento], tamanhoRegPag, 1, financeiro);
        fclose(financeiro);
        controleVetorPagamento++ ;
    } else {
        printf("Moto não cadastrada!");
    }
}

void EncerrarExpediente() {
    //procedimento que vai salvar em arquivos os servicos não finalizados , aqueles com status igual a 0
    servicosPendentes = fopen("Pendentes.dat", "a+b");
    for (int i = 0; i <= Quant; i++) {
        // 0 servico ainda não iniciou , 2 , solicitação de servico cancelada
        if (VZonda[i].Status == '0'  &&  VZonda[i].Status != '2') {           
            fwrite(&VZonda[i], sizeof(TpRegMoto), 1, servicosPendentes);
        }
    }
    printf("Expediente fechado! servicos pendentes armazenado ! \n");
    fclose(servicosPendentes);
}

void historicoFinanceiro(){
    int i = 0;
    float montante = 0;   
    financeiro = fopen("HistFinancas.dat", "a+b");
    while(fread(&Financas[i], tamanhoRegPag, 1, financeiro) == 1){
        printf("Placa Cliente %d : %s \n", i + 1 ,Financas[i].placa);
        printf("Valor recebido: R$ %.2f \n", Financas[i].preco);
        printf("Data: %s \n\n", Financas[i].data);
        montante += Financas[i].preco;
        i++;
    }
    printf("Valor total recebido no mes : R$ %.2f \n" , montante);
    fclose(financeiro); 
}

int main() {
    int Opcao, i = 0 ;
    time_t agora = time(NULL);
    // Converte para a estrutura de tempo local
    struct tm *tempo_local = localtime(&agora);
    // Obtém o dia atual
    int dia_atual = tempo_local->tm_mday;
    servicosPendentes = fopen("Pendentes.dat", "a+b");
    if( dia_atual == 2)
        remove("HistFinancas.dat");

    if (servicosPendentes == NULL) {
        printf("Erro ao abrir o arquivo Pendentes.dat\n");
        return 1;
    }
    //printf("\n\nServicos Pendentes: \n"); 
    while (fread(&VZonda[i], sizeof(TpRegMoto), 1, servicosPendentes) == 1) {
        i++;
        Quant++;
    }
    fclose(servicosPendentes);
    remove("Pendentes.dat");    
    do {
        //Exibicao de menu e leitura da opcao. 	
        printf("\n\n >>> Motos Zonda <<< \n\n");
        printf("1 - Solicitar Servico \n");
        printf("2 - Iniciar Servico \n");
        printf("3 - Remover Solicitacao \n");
        printf("4 - Consultar Solicitacoes \n");
        printf("5 - Concluir Servico \n");
        printf("6 - Encerrar Expediente \n");
        printf("7 - historico Financeiro\n");
        printf("8 - Sair \n\n");
        printf("Digite a opcao desejada: ");
        scanf("%d", &Opcao);
        switch (Opcao) {
            case 1: SolicitaServico(); break;
            case 2: IniciaServico(); break;
            case 3: RemoverSolicitacao(); break;
            case 4: ConsultarSolicitacoes(); break;
            case 5: ConcluirServico(); break;
            case 6: EncerrarExpediente(); break;
            case 7: historicoFinanceiro(); break;
            case 8: break;
        }
    } while (Opcao != 8 );
    return 0;
} 
