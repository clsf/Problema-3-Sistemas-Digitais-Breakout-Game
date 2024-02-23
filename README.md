# Problema-3-Sistemas-Digitais-Breakout-Game
Problema 3 da disciplina de sistemas digitais 

## Introdução
Este é o README do Projeto X, um jogo desenvolvido pela equipe Nirvan, Nicassio, Luis e Cláudia. O Projeto X é um jogo de estilo arcade, inspirado em jogos clássicos como Pong e Breakout.

## Recursos
O Projeto X apresenta os seguintes recursos:

- **Clock:** Mantenha-se no controle do tempo com o relógio integrado.
- **Libs intelfpgaup:** Biblioteca que oferece funcionalidades essenciais para o desenvolvimento do jogo, incluindo drivers e controles.

## Como Utilizar
Instruções sobre como baixar, compilar e executar o jogo podem ser encontradas nesta seção. Certifique-se de revisar as dependências e os pré-requisitos antes de iniciar.

## Libs intelfpgaup

Para estabelecer a comunicação entre o software e o hardware, utilizamos drivers que atuam como uma ponte, utilizando a biblioteca intelfpga. Esses drivers consistem em um conjunto de rotinas ou funções que possibilitam a interação do programa com os componentes específicos da FPGA DE1-SoC, como o acelerômetro, botões e vídeo. Essa abordagem simplifica o processo de leitura e escrita nos registradores, assim como nas configurações desses componentes.

Através do controle proporcionado pelos drivers, conseguimos gerenciar eficientemente os periféricos da FPGA. Isso inclui ajustar parâmetros e iniciar operações específicas fundamentais para o funcionamento do jogo.

A biblioteca "intelfpgaup" desempenhou um papel crucial, servindo como a ferramenta necessária para abstrair muitos detalhes complexos. Essa abstração evitou a necessidade de realizar configurações, leituras e inicializações dos periféricos em níveis mais baixos. Dessa forma, a implementação do jogo foi simplificada e tornada mais acessível, permitindo que os desenvolvedores se concentrassem nas lógicas do jogo em si, em vez de detalhes específicos da FPGA.
### `video.h` (por Luis)
Esta biblioteca trata das operações relacionadas à exibição de vídeo. Consulte a documentação para obter informações detalhadas sobre como utilizar as funções disponíveis.

### `accel.h` (por Nirvan)
A biblioteca `accel.h` é responsável por lidar com aceleração e movimentação no jogo. Certifique-se de entender como integrar essas funcionalidades em seu código para um controle suave.

### `KEY.h` (por Nicassio)
A biblioteca `KEY.h` oferece suporte para lidar com eventos de teclado. Consulte a documentação para entender como mapear e responder a teclas específicas.

## Solução Geral
<div align="center">
    <img src="/img/mainBreakout.drawio.png" alt="Diagrama da main">
     <p>
       Diagrama da main
      </p>
</div>
Para o correto funcionamento do jogo, é essencial entender o papel central desempenhado pelo bloco de código "main". Este bloco é responsável por criar e gerenciar elementos cruciais, como blocos, paredes, bola, barra do jogador e mensagens. Adicionalmente, realiza as configurações iniciais do botão e do acelerômetro da placa, além de estabelecer a conexão com o monitor por meio da interface VGA.

Observando o diagrama, nota-se que o jogo só é iniciado quando o usuário pressiona o botão indicado pela mensagem na tela. Durante a inicialização, são verificadas colisões com as paredes e a barra do jogador para determinar a trajetória subsequente da bola. Adicionalmente, verifica-se se a bola colidiu com blocos ativos na tela. Em caso afirmativo, a pontuação é atualizada, e o bloco correspondente é desativado. É validado também, o acionamento do botão após o inicio, significando que o usuário pausou o jogo.

Outro ponto importante é a verificação da posição da bola em relação à barra do jogador (paddle). Se a bola estiver abaixo da barra, uma mensagem de "game over" é exibida, e o jogo reinicia após o usuário pressionar o botão correspondente. A pontuação também é verificada; atingir uma pontuação igual à quantidade de blocos resulta na vitória. Nesse caso, uma mensagem de vitória é exibida, e o jogo reinicia após o usuário pressionar o botão.

Após todas essas verificações, a tela é continuamente atualizada para refletir visualmente as alterações no estado do jogo. Esse ciclo de verificação e atualização garante uma experiência dinâmica e interativa para o jogador, proporcionando um fluxo suave e envolvente durante o jogo.

## Paddle (Movimento) (por Nirvan)
Descreve como o movimento do paddle é implementado, incluindo as funções disponíveis e como integrá-las ao jogo.

## Bola (Movimento) (por Luis)
Detalhes sobre como a bola se move no jogo. Explique as características específicas do movimento e como isso afeta a jogabilidade.

## Blocos (Geração) (por Nicassio)
Esta seção cobre a lógica por trás da geração de blocos no jogo. Inclua detalhes sobre como os blocos são criados, organizados e destruídos durante o jogo.

## Paredes (por Luis)
Explica a função e a implementação das paredes no jogo, se houver. Isso pode incluir paredes que afetam o movimento da bola ou outras mecânicas de jogo.

## Colisão (por Luis)
Detalhes sobre como a detecção e o tratamento de colisões são implementados no projeto. Isso abrange colisões entre a bola, o paddle, os blocos e outras entidades relevantes.

## Botões (Pause, Continue, Restart) (por Nicassio)
Explica como os botões de pause, continue e restart são implementados e como eles interagem com o fluxo do jogo.

## Textos (Mensagens e Pontuação)
As mensagens no jogo são geradas por meio da estrutura mensagem, que engloba os atributos x, y, mensagem e ativa. As variáveis x e y indicam a posição na tela em que a mensagem será exibida, enquanto a variável mensagem armazena a informação a ser apresentada. O atributo ativa determina se a mensagem deve ser exibida ou não. A lógica de exibição é controlada pelo método "exibirMensagem", que verifica a condição de ativação antes de utilizar a função para escrever a mensagem na tela.

Quanto à pontuação, é gerenciada pela estrutura pontuação, que possui os atributos x, y e pontuação. Os atributos x e y seguem a mesma lógica da estrutura mensagem, indicando a posição na tela. A variável pontuação representa os pontos acumulados pelo usuário. Dois métodos são incorporados a essa estrutura: atualizarPontuação e exibirPontuação. O método atualizarPontuação incrementa os pontos do usuário sempre que é chamado, enquanto o método exibirPontuação atualiza a mensagem da pontuação na tela. Essa abordagem permite um acompanhamento dinâmico dos pontos durante o jogo.

