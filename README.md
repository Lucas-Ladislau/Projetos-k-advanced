# Projetos-k-advanced

# Sistema de Rastreamento de Ambulâncias

Este projeto tem como objetivo implementar um sistema de rastreamento de ambulâncias utilizando um módulo GPS Neo-6M para localizar as ambulâncias em tempo real. O sistema exibe essas localizações de forma visual em um mapa, permitindo o acompanhamento de várias ambulâncias simultaneamente.

## Funcionalidades

- **Rastreamento em tempo real**: Utilização do módulo GPS Neo-6M para capturar coordenadas geográficas (latitude e longitude) em tempo real.
- **Acompanhamento de múltiplas ambulâncias**: Permite a visualização de diversas ambulâncias sendo localizadas no mapa simultaneamente.

## Tecnologias Utilizadas

- **Hardware**:
  - Módulo GPS Neo-6M

## Como Funciona

1. **Leitura dos Dados GPS**: O sistema recebe os dados de latitude e longitude provenientes do módulo GPS Neo-6M.
2. **Exibição no Mapa**: As coordenadas são exibidas em tempo real no mapa usando a biblioteca Leaflet.js.
3. **Atualização de Ambulâncias**: O sistema pode mostrar múltiplas ambulâncias, cada uma com um card de identificação, contendo informações como:
   - ID da ambulância
   - Latitude
   - Longiude
4. **Monitoramento**: O sistema permite o monitoramento contínuo das ambulâncias, com atualizações dinâmicas no mapa.

Contribuindo
Se você deseja contribuir para o projeto, fique à vontade para fazer um fork e enviar pull requests.

Passos para Contribuir:
- Faça um fork do projeto
- Crie uma nova branch (git checkout -b minha-nova-feature)
- Faça as alterações desejadas
- Commit suas alterações (git commit -am 'Adicionando nova feature')
- Envie para o repositório remoto (git push origin minha-nova-feature)
- Abra um pull request explicando as mudanças
