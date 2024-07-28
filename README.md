# Trabalho Prático de Sistemas Operacionais: Gerenciador de processos

## Integrantes
- Maria Eduarda de Pinho Braga
- Arthur Ataíde de Melo Saraiva
- Guilherme Broedel Zorzal
- Eduardo Antunes dos Santos Vieira
- João Gabriel Angelo Bradachi

## FUNCIONAMENTO

### Main =
- processo de controle (codigo de processo pai está na main)
- dentro da main é criado o processo que gerencia processos. O processo pai (main) recebe comandos e envia para o gerenciador de processos via pipe
  - ATENÇAO: O Usuario precisa especificar a entrada
### Gerenciador de Processos =
- fica em loop esperando comandos do processo de controle. Quando recebe os comandos, executa o que precisa
