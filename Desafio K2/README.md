# Sistema para Monitoramento em Tempo Real Cardíaco

Este projeto consiste em um sistema para monitoramento em tempo real dos sinais de ECG (Eletrocardiograma) utilizando um sensor AD8232 com ESP32, que envia dados para um servidor local. O sistema também integra o sensor R307 para autenticação biométrica, permitindo a identificação do usuário, e utiliza uma interface em Streamlit para visualização dos dados em tempo real.

## Funcionalidades

- **Monitoramento em Tempo Real**: Exibição dos sinais de ECG ao vivo, recebendo dados a cada segundo.
- **Exibição do ID do Usuário**: Identificação do usuário cujos dados estão sendo exibidos, permitindo o rastreamento de diferentes usuários.
- **Autenticação Biométrica**: Utiliza o sensor R307 para autenticação do usuário por impressão digital antes de iniciar o monitoramento do ECG.
- **Exibição de 100 Valores Recentes**: Exibe apenas os 100 valores mais recentes do sinal de ECG, mantendo o gráfico atualizado e relevante.
- **Atualização Automática**: O gráfico é atualizado automaticamente a cada segundo para refletir os novos dados recebidos do servidor.
  
## Tecnologias Utilizadas

- **Streamlit**: Framework para criação de interfaces web interativas em Python. Utilizado para exibir o gráfico do ECG em tempo real.
- **Plotly**: Biblioteca para criação de gráficos interativos. Utilizada para plotar o gráfico de ECG.
- **Requests**: Biblioteca Python para fazer requisições HTTP. Utilizada para buscar os dados do servidor.
- **Pandas**: Biblioteca de manipulação de dados, utilizada para estruturar e tratar os dados de ECG.
- **Flask/ESP32**: O servidor de backend que coleta os dados do sensor AD8232 (no caso do backend, o servidor está sendo executado no ESP32).
- **R307 (Sensor de Impressão Digital)**: Sensor utilizado para autenticação biométrica do usuário, garantindo que apenas usuários autorizados possam acessar os dados de ECG.

## Como funciona

1. **Leitura do ECG**: O sensor AD8232 conectado ao ESP32 captura os sinais do ECG e os envia para um servidor local. O ESP32 utiliza o protocolo HTTP para fazer essa comunicação.
   
2. **Autenticação do Usuário**: Antes de iniciar o monitoramento dos sinais de ECG, o sistema realiza uma autenticação do usuário através do sensor R307, que valida a impressão digital do usuário.

3. **Recepção dos Dados**: Após a autenticação, o sistema em Python, utilizando a biblioteca `requests`, faz uma requisição ao servidor local a cada segundo para buscar os dados de ECG mais recentes.

4. **Processamento e Exibição**: Os dados recebidos são processados e preparados para visualização. Os dados de ECG são armazenados em um buffer, mantendo apenas os 100 valores mais recentes para evitar sobrecarga de dados.

5. **Interface em Tempo Real**: A interface em `Streamlit` exibe esses dados em tempo real. O gráfico é gerado e atualizado automaticamente a cada segundo utilizando `Plotly`, garantindo a exibição de um sinal de ECG contínuo.

6. **Alteração de ID do Usuário**: O sistema é capaz de identificar e exibir o ID do usuário cujos dados estão sendo exibidos, permitindo a alternância entre usuários em tempo real.

7. **Atualização e Reset**: Quando o ID do usuário muda, o gráfico e os dados são resetados, exibindo os novos dados para o novo usuário.
