#include <stdio.h>
#include <time.h>

typedef struct {
	int sender;
	int receiver;
	int valor;
} transacao;

typedef struct {
	int index;
	int timestamp;
	transacao dado[1000];
	int quantidade_transacoes;
	int hash_anterior;
	int hash;
} bloco;

typedef struct {
	bloco cadeia[100];	
} blockchain;

blockchain mytinyblockchain;
int contador_blocos;

transacao transacoes_pendentes[1000];
int contador_transacoes_pendentes;

void enviar_dinheiro() {
	printf("Enviar dinheiro\n");

	int sender;
	int valor;
    printf("Sender (0 para Alice e 1 para Bob): ");
	scanf("%d", &sender);

	if(sender != 0 && sender != 1) {
	    printf("Escolha 0 (Alice) ou 1 (Bob) como sender\n");
		return;
	}

    printf("Valor: ");
	scanf("%d", &valor);

	if(sender == 0) {
		transacoes_pendentes[contador_transacoes_pendentes].sender = 0;
		transacoes_pendentes[contador_transacoes_pendentes].receiver = 1;
		transacoes_pendentes[contador_transacoes_pendentes].valor = valor;
	}

	if(sender == 1) {
		transacoes_pendentes[contador_transacoes_pendentes].sender = 1;
		transacoes_pendentes[contador_transacoes_pendentes].receiver = 0;
		transacoes_pendentes[contador_transacoes_pendentes].valor = valor;
	}

	contador_transacoes_pendentes++;
}

void exibir_saldo() {
	printf("Exibir saldo\n");
}

void minerar_bloco() {
    printf("Minera bloco\n");

	// LAST BLOCK
	printf("Recebendo ultimo bloco\n");
	bloco current_block = mytinyblockchain.cadeia[contador_blocos - 1];

	// CREATING A NEW BLOCK
	printf("Instanciando novo bloco\n");
	bloco new_block;

	new_block.index = contador_blocos;
    new_block.timestamp = _obter_timestamp();
	new_block.hash_anterior = current_block.hash;
	new_block.hash = _obter_hash(new_block.index, new_block.timestamp,
					             new_block.hash_anterior);

	printf("Novo bloco instanciado, copiando transacoes do ultimo bloco\n");

	int i;
	for(i = 0; i < current_block.quantidade_transacoes; i++){
		new_block.dado[i] = current_block.dado[i];
	}
	new_block.quantidade_transacoes = current_block.quantidade_transacoes;

	printf("Inserindo transacoes pendentes no novo bloco\n");
	printf("Quantidade transacoes pendentes: %d\n", contador_transacoes_pendentes);
	int total_transacoes_novo_bloco = new_block.quantidade_transacoes + contador_transacoes_pendentes;

	for(i = new_block.quantidade_transacoes; i < total_transacoes_novo_bloco; i++) {
		new_block.dado[i].sender = transacoes_pendentes[i - new_block.quantidade_transacoes].sender;
		new_block.dado[i].receiver = transacoes_pendentes[i - new_block.quantidade_transacoes].receiver;
		new_block.dado[i].valor = transacoes_pendentes[i - new_block.quantidade_transacoes].valor;
	}

	new_block.quantidade_transacoes += contador_transacoes_pendentes;

	printf("Registrando novo bloco na blockchain\n");
	mytinyblockchain.cadeia[contador_blocos] = new_block;
	contador_blocos++;
	contador_transacoes_pendentes = 0;

	printf( "Bloco %d registrado. Hash: %d\n" , new_block.index, new_block.hash);
}

void exibir_transacoes_pendentes() {
	printf("Exibir transações pendentes\n");
}

void sair() {
	printf("Sair\n");
}

int _obter_timestamp() {
	return (int)time(NULL);
}

int _obter_hash(int index, int timestamp, int hash_anterior) {
	return timestamp * (hash_anterior + index);
}

void minerar_bloco_genesis() {
	bloco genesis;
	genesis.index = contador_blocos;
	genesis.timestamp = _obter_timestamp();

	//  7 = SATOSHI MANJAMUITO
	//  0 = ALICE
	//  1 = BOB
	genesis.dado[0].sender = 7;
	genesis.dado[0].receiver = 0;
	genesis.dado[0].valor = 1000;

	genesis.dado[1].sender = 7;
	genesis.dado[1].receiver = 1;
	genesis.dado[1].valor = 1000;
	genesis.quantidade_transacoes = 2;

	genesis.hash_anterior = 0;
	genesis.hash = _obter_hash(genesis.index, genesis.timestamp,
							   genesis.hash_anterior);

	mytinyblockchain.cadeia[contador_blocos] = genesis;
	contador_blocos++;
}

int main() {
	contador_blocos = 0;
    contador_transacoes_pendentes = 0;

	minerar_bloco_genesis();

	int opcao = 1;
	do {
		printf("\n\tMyTinyBlockchain\n\n");
		printf("1. Enviar dinheiro\n");
		printf("2. Exibir saldo\n");
		printf("3. Minerar um bloco\n");
		printf("4. Exibir transações pendentes\n");
		printf("5. Sair\n");

		scanf("%d", &opcao);
		system("cls || clear");

		switch(opcao) {
			case 1:
				enviar_dinheiro();
				break;

			case 2:
				exibir_saldo();
				break;

			case 3:
				minerar_bloco();
				break;

			case 4:
				exibir_transacoes_pendentes();
				break;

			case 5:
				sair(); 
				break;

			default:
				printf("Digite uma opcao valida\n");
		}
	} while(opcao != 5);
}
