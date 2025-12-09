# Hash Table com Rehashing Automático

Uma implementação completa de tabela hash em C com encadeamento para tratamento de colisões e rehashing automático quando o fator de carga atinge 75%.

## Características

- **Chave de busca**: CPF
- **Dados armazenados**: Nome e idade
- **Resolução de colisões**: Encadeamento (listas ligadas)
- **Rehashing automático**: Dobra o tamanho da tabela quando o fator de carga ≥ 0.75
- **Operações**: inserção, busca, remoção e análise estatística

## Compilação

```bash
gcc -o hashtable hashtable.c -Wall
```

## Uso

```bash
./hashtable arquivo.txt
```

## Formato do Arquivo de Entrada

O arquivo de entrada deve conter uma pessoa por linha no formato CSV:

```
cpf,nome,idade
123.456.789-01,João Silva,25
987.654.321-00,Maria Santos,30
456.789.123-45,Pedro Oliveira,28
```

## Exemplo de Saída

O programa exibe:

- **Processo de rehashing**: Mostra quando a tabela é expandida
- **Análise da distribuição**: Estatísticas sobre colisões e ocupação
- **Amostra de dados**: Primeiros 20 registros inseridos
- **Testes de busca**: Busca por CPFs específicos
- **Teste de remoção**: Remove um CPF e verifica

## Configurações

- **Tamanho inicial**: 10 buckets
- **Fator de carga limite**: 0.75
- **Função hash**: Multiplicação por 31 com módulo

## Funções Principais

- `insert()`: Insere ou atualiza um registro
- `search()`: Busca um CPF e retorna os dados
- `removeKey()`: Remove um CPF da tabela
- `rehash()`: Redimensiona e redistribui elementos
- `analyzeHashTable()`: Gera estatísticas detalhadas

## Complexidade

- **Busca**: O(1) médio, O(n) pior caso
- **Inserção**: O(1) médio, O(n) com rehashing
- **Remoção**: O(1) médio, O(n) pior caso

## Exemplo de Arquivo de Teste

Crie um arquivo `dados.txt`:

```
123.456.789-01,João Silva,25
987.654.321-00,Maria Santos,30
456.789.123-45,Pedro Oliveira,28
111.222.333-44,Ana Costa,22
555.666.777-88,Carlos Souza,35
```

Execute:

```bash
./hashtable dados.txt
```