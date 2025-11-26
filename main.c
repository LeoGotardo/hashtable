#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 10
#define LOAD_FACTOR_THRESHOLD 0.75

typedef struct Node {
    char *cpf;      // Chave de busca
    char *nome;     // Nome da pessoa
    int idade;      // Idade
    struct Node *next;
} Node;

typedef struct {
    Node **buckets;
    int size;
    int count;
} HashTable;

unsigned int hash(const char *key, int tableSize) {
    unsigned int hashValue = 0;
    
    for (int i = 0; key[i] != '\0'; i++) {
        hashValue = (hashValue * 31 + key[i]) % tableSize;
    }
    
    return hashValue;
}

HashTable* createHashTable(int size) {
    HashTable *table = malloc(sizeof(HashTable));
    table->size = size;
    table->count = 0;
    table->buckets = calloc(size, sizeof(Node*));
    return table;
}

float getLoadFactor(HashTable *table) {
    return (float)table->count / table->size;
}

void rehash(HashTable *table) {
    int oldSize = table->size;
    int newSize = oldSize * 2;
    Node **oldBuckets = table->buckets;
    
    printf("\n⚠️  REHASHING NECESSARIO!\n");
    printf("   Fator de carga: %.2f (limite: %.2f)\n", getLoadFactor(table), LOAD_FACTOR_THRESHOLD);
    printf("   Expandindo tabela: %d -> %d buckets\n", oldSize, newSize);
    
    table->size = newSize;
    table->buckets = calloc(newSize, sizeof(Node*));
    
    int redistributed = 0;
    for (int i = 0; i < oldSize; i++) {
        Node *current = oldBuckets[i];
        while (current != NULL) {
            Node *next = current->next;
            
            unsigned int newIndex = hash(current->cpf, newSize);
            
            current->next = table->buckets[newIndex];
            table->buckets[newIndex] = current;
            
            redistributed++;
            current = next;
        }
    }
    
    free(oldBuckets);
    printf("   ✓ %d elementos redistribuidos com sucesso!\n\n", redistributed);
}

void insert(HashTable *table, const char *cpf, const char *nome, int idade) {
    if (getLoadFactor(table) >= LOAD_FACTOR_THRESHOLD) {
        rehash(table);
    }
    
    unsigned int index = hash(cpf, table->size);
    
    // Verifica se o CPF já existe
    Node *current = table->buckets[index];
    while (current != NULL) {
        if (strcmp(current->cpf, cpf) == 0) {
            // Atualiza os dados
            free(current->nome);
            current->nome = strdup(nome);
            current->idade = idade;
            return;
        }
        current = current->next;
    }
    
    // Insere novo elemento
    Node *newNode = malloc(sizeof(Node));
    newNode->cpf = strdup(cpf);
    newNode->nome = strdup(nome);
    newNode->idade = idade;
    newNode->next = table->buckets[index];
    table->buckets[index] = newNode;
    table->count++;
}

Node* search(HashTable *table, const char *cpf) {
    unsigned int index = hash(cpf, table->size);
    Node *current = table->buckets[index];
    
    while (current != NULL) {
        if (strcmp(current->cpf, cpf) == 0) {
            return current;
        }
        current = current->next;
    }
    
    return NULL;
}

void removeKey(HashTable *table, const char *cpf) {
    unsigned int index = hash(cpf, table->size);
    Node *current = table->buckets[index];
    Node *prev = NULL;
    
    while (current != NULL) {
        if (strcmp(current->cpf, cpf) == 0) {
            if (prev == NULL) {
                table->buckets[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current->cpf);
            free(current->nome);
            free(current);
            table->count--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

void analyzeHashTable(HashTable *table) {
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║          ANALISE DA DISTRIBUICAO DA HASH TABLE            ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    int totalElements = 0;
    int usedBuckets = 0;
    int maxCollisions = 0;
    int emptyBuckets = 0;
    
    for (int i = 0; i < table->size; i++) {
        int count = 0;
        Node *current = table->buckets[i];
        
        while (current != NULL) {
            count++;
            totalElements++;
            current = current->next;
        }
        
        if (count > 0) {
            usedBuckets++;
            if (count > maxCollisions) {
                maxCollisions = count;
            }
        } else {
            emptyBuckets++;
        }
    }
    
    printf("ESTATISTICAS GERAIS:\n");
    printf("  Total de elementos inseridos: %d\n", totalElements);
    printf("  Tamanho da tabela: %d buckets\n", table->size);
    printf("  Fator de carga: %.2f (limite: %.2f)\n", getLoadFactor(table), LOAD_FACTOR_THRESHOLD);
    printf("  Buckets utilizados: %d/%d\n", usedBuckets, table->size);
    printf("  Buckets vazios: %d/%d\n", emptyBuckets, table->size);
    printf("  Taxa de ocupacao: %.2f%%\n", (usedBuckets * 100.0) / table->size);
    printf("  Maior cadeia (colisoes): %d elementos\n", maxCollisions);
    printf("  Media de elementos por bucket usado: %.2f\n", 
           usedBuckets > 0 ? (double)totalElements / usedBuckets : 0);
    
    printf("\nBUCKETS COM MULTIPLOS ELEMENTOS (COLISOES):\n");
    int collisionCount = 0;
    for (int i = 0; i < table->size; i++) {
        int count = 0;
        Node *current = table->buckets[i];
        
        while (current != NULL) {
            count++;
            current = current->next;
        }
        
        if (count > 1) {
            collisionCount++;
            printf("  [%3d] -> %d elementos: ", i, count);
            current = table->buckets[i];
            int printed = 0;
            while (current != NULL && printed < 3) {
                printf("'%s'", current->cpf);
                if (current->next != NULL && printed < 2) printf(", ");
                current = current->next;
                printed++;
            }
            if (count > 3) printf("... (+%d)", count - 3);
            printf("\n");
        }
    }
    
    if (collisionCount == 0) {
        printf("  Nenhuma colisao detectada! Distribuicao perfeita!\n");
    } else {
        printf("\n  Total de buckets com colisoes: %d\n", collisionCount);
    }
}

void printSampleData(HashTable *table, int numSamples) {
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║              AMOSTRA DE DADOS ARMAZENADOS                  ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    int count = 0;
    printf("%-20s %-25s %-10s %-10s\n", "CPF", "NOME", "IDADE", "INDICE");
    printf("─────────────────────────────────────────────────────────────────────────────\n");
    
    for (int i = 0; i < table->size && count < numSamples; i++) {
        Node *current = table->buckets[i];
        while (current != NULL && count < numSamples) {
            printf("%-20s %-25s %-10d %-10d\n", 
                   current->cpf, current->nome, current->idade, i);
            current = current->next;
            count++;
        }
    }
}

void freeHashTable(HashTable *table) {
    for (int i = 0; i < table->size; i++) {
        Node *current = table->buckets[i];
        while (current != NULL) {
            Node *temp = current;
            current = current->next;
            free(temp->cpf);
            free(temp->nome);
            free(temp);
        }
    }
    free(table->buckets);
    free(table);
}

int loadDataFromFile(HashTable *table, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("✗ Erro ao abrir arquivo '%s'\n", filename);
        return 0;
    }
    
    char cpf[20];
    char nome[100];
    int idade;
    int count = 0;
    
    while (fscanf(file, " %19[^,],%99[^,],%d\n", cpf, nome, &idade) == 3) {
        insert(table, cpf, nome, idade);
        count++;
        
        if (count % 20 == 0) {
            printf("  [%3d] Fator de carga: %.2f | Tamanho: %d buckets\n", 
                   count, getLoadFactor(table), table->size);
        }
    }
    
    fclose(file);
    return count;
}

int main(int argc, char *argv[]) {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║     HASH TABLE COM REHASHING AUTOMATICO                   ║\n");
    printf("║          Busca por CPF - Armazena Nome e Idade            ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    HashTable *table = createHashTable(INITIAL_SIZE);
    
    printf("\nConfiguracao inicial:\n");
    printf("  Tamanho inicial: %d buckets\n", INITIAL_SIZE);
    printf("  Limite de fator de carga: %.2f\n", LOAD_FACTOR_THRESHOLD);
    printf("  Chave de busca: CPF\n");
    printf("  Rehashing ocorrera automaticamente quando necessario\n");
    
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║              CARREGANDO DADOS                              ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    int totalRegistros = 0;
    
    if (argc >= 2) {
        printf("\nLendo dados do arquivo: %s\n\n", argv[1]);
        totalRegistros = loadDataFromFile(table, argv[1]);
        
        if (totalRegistros == 0) {
            printf("\n⚠️  Nenhum dado carregado.\n");
        } else {
            printf("\n✓ %d registros carregados do arquivo!\n", totalRegistros);
        }
    } else {
        printf("\n⚠️  Nenhum arquivo fornecido.\n");
        printf("Uso: %s <arquivo.txt>\n", argv[0]);
        printf("Formato do arquivo: cpf,nome,idade\n");
    }
    
    if (totalRegistros > 0) {
        analyzeHashTable(table);
        printSampleData(table, 20);
        
        // Testes de busca
        printf("\n╔════════════════════════════════════════════════════════════╗\n");
        printf("║                  TESTES DE BUSCA POR CPF                   ║\n");
        printf("╚════════════════════════════════════════════════════════════╝\n\n");
        
        // Pega alguns CPFs da tabela para testar
        const char *testesCPF[] = {
            "123.456.789-01",
            "456.654.789-98",
            "901.109.234-43",
            "000.000.000-00",  // CPF que não existe
            "999.999.999-99"   // CPF que não existe
        };
        
        for (int i = 0; i < 5; i++) {
            Node *result = search(table, testesCPF[i]);
            
            if (result != NULL) {
                printf("✓ CPF '%s' encontrado\n", testesCPF[i]);
                printf("  Nome: %s\n", result->nome);
                printf("  Idade: %d anos\n\n", result->idade);
            } else {
                printf("✗ CPF '%s' NAO encontrado\n\n", testesCPF[i]);
            }
        }
        
        // Teste de remoção
        printf("╔════════════════════════════════════════════════════════════╗\n");
        printf("║                  TESTE DE REMOCAO                          ║\n");
        printf("╚════════════════════════════════════════════════════════════╝\n\n");
        
        const char *cpfRemover = "123.456.789-01";
        printf("Removendo CPF '%s'...\n", cpfRemover);
        
        Node *antes = search(table, cpfRemover);
        if (antes != NULL) {
            printf("Antes: %s, %d anos\n", antes->nome, antes->idade);
        }
        
        removeKey(table, cpfRemover);
        
        Node *depois = search(table, cpfRemover);
        if (depois == NULL) {
            printf("✓ CPF '%s' foi removido com sucesso!\n", cpfRemover);
            printf("  Elementos restantes: %d\n", table->count);
            printf("  Fator de carga atual: %.2f\n", getLoadFactor(table));
        }
        
        analyzeHashTable(table);
    }
    
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                  LIBERANDO MEMORIA                         ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    freeHashTable(table);
    printf("✓ Memoria liberada com sucesso!\n");
    
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║              PROGRAMA FINALIZADO                           ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    return 0;
}