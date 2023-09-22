#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define BUFFER_SIZE 4096

char buffer[BUFFER_SIZE];

typedef struct MemoryBlock {
  size_t size;
  bool in_use;
  struct MemoryBlock *next;
} MemoryBlock;

MemoryBlock *head = (MemoryBlock *)buffer;

void initialize_memory() {
  head->size = BUFFER_SIZE - sizeof(MemoryBlock);
  head->in_use = false;
  head->next = NULL;
}

void *aloca(size_t size) {
  MemoryBlock *current = head;
  while (current) {
    if (!current->in_use && current->size >= size + sizeof(MemoryBlock)) {
      // Reservar espaço para um novo MemoryBlock
      MemoryBlock *new_block =
          (MemoryBlock *)((char *)current + sizeof(MemoryBlock) + size);
      new_block->size = current->size - size - sizeof(MemoryBlock);
      new_block->in_use = false;
      new_block->next = current->next;

      current->size = size;
      current->in_use = true;
      current->next = new_block;

      return (char *)current + sizeof(MemoryBlock);
    }
    current = current->next;
  }
  return NULL;
}

void libera(void *ptr) {
  MemoryBlock *block = (MemoryBlock *)((char *)ptr - sizeof(MemoryBlock));
  block->in_use = false;
  while (block->next && !block->next->in_use) {
    block->size += block->next->size + sizeof(MemoryBlock);
    block->next = block->next->next;
  }
}


/*
	Definição da estrutura da lista duplamente encadeada e suas funções
*/

typedef struct Conteudo {
	int32_t chave;
	char *string;
} Conteudo;

typedef struct No {
	struct No *anterior, *proximo;
	struct Conteudo conteudo;
} No;

typedef struct Lista {
	No *primeiro_no, *ultimo_no;
} Lista;

void inserir(Lista *lista, Conteudo conteudo) {
	No *aux = (No *) malloc(sizeof(No));
	No *ultimo = lista->ultimo_no;

	if (ultimo) {
		aux->conteudo = conteudo;
		aux->proximo = ultimo->proximo;
		aux->anterior = ultimo;
		ultimo->proximo->anterior = aux;
		ultimo->proximo = aux;
	} else {
		aux->proximo = aux;
		aux->anterior = aux;
		lista->primeiro_no = aux;
	}

	lista->ultimo_no = aux;
	return;
}

No * buscar(Lista *lista, int32_t chave) {
	No *aux = lista->primeiro_no->proximo;
	lista->primeiro_no->conteudo.chave = chave;

	while (aux->conteudo.chave != chave) {
		aux = aux->proximo;
	}

	if(aux != lista->primeiro_no) {
		return aux;
	}

	return NULL;
}

void alterarChave(Lista *lista, int32_t chave_antiga, int32_t nova_chave) {
	No *aux = buscar(lista, chave_antiga);

	if (aux != NULL) {
		aux->conteudo.chave = nova_chave;
	}
}

void excluir(Lista *lista, int32_t chave) {
	No *aux = buscar(lista, chave);

	if (aux != NULL) {
		aux->anterior->proximo = aux->proximo;
		aux->proximo->anterior = aux->anterior;

		if (lista->primeiro_no == aux) {
			lista->primeiro_no = aux->proximo;
		}

		if (lista->ultimo_no == aux) {
			lista->ultimo_no = aux->anterior;
		}

		free(aux->conteudo.string);	
		free(aux);
	}
}

void limpar(Lista *lista) {
    No *atual = lista->primeiro_no;
    while (atual != NULL) {
        No *prox = atual->proximo;
        if (atual->conteudo.string) {
            free(atual->conteudo.string);
        }
        free(atual);
        atual = prox;
    }
    lista->primeiro_no = NULL;
    lista->ultimo_no = NULL;
}

int main() {
  initialize_memory();
  Lista *lista = (Lista *) malloc(sizeof(Lista));
	lista->primeiro_no = NULL;
	lista->ultimo_no = NULL;
	Conteudo teste;
	teste.chave=1;
	teste.string="asd";
	inserir(lista, teste);
	printf("%p\n", lista->primeiro_no);
	printf("%p\n", lista->ultimo_no);
  libera(lista);
	printf("%p\n", lista->primeiro_no);
	printf("%p\n", lista->ultimo_no);
  return 0;
}
