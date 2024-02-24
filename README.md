# Problema-3-Sistemas-Digitais-Breakout-Game
Problema 3 da disciplina de sistemas digitais - Breakout Game em C

## Equipe de Desenvolvimento
- Antonio
- Cláudia
- Luis
- Nirvan

## Tutor
- Thiago Jesus

## Introdução
O projeto foi baseado no desenvolvimento do Jogo "Breakout", utilizando como plataforma o DE1-Soc, este desafio que visa o aprendizado, com a obtenção do conhecimento para programar na linguagem C para um processador que possui a arquitetura ARM.Aliado ao fato de incentivar a utilização das interfaces presente no kit FPGA DE1-Soc, como os botões e o acelerômetro, que servirão como controles para o jogo. 

O Desenvolvimento do jogo foi feito com determinados requisitos.Ser implementado exclusivamente em linguagem C utilizando apenas os componentes disponíveis na placa DE1-SoC, Utilizando a interface VGA para Visualização através de um monitor CRT. 

<div align="center">
  <img src="/img/Interface_Hardware.drawio.png" alt="Fluxograma do hardware">
   <p>
    Fluxograma do hardware
    </p>
</div>

## Recursos
O Projeto X apresenta os seguintes recursos:

- **Clock:** Mantenha-se no controle do tempo com o relógio integrado.
- **Libs intelfpgaup:** Biblioteca que oferece funcionalidades essenciais para o desenvolvimento do jogo, incluindo drivers e controles.

## Como Utilizar
Instruções sobre como baixar, compilar e executar o jogo podem ser encontradas nesta seção. Certifique-se de revisar as dependências e os pré-requisitos antes de iniciar.

## Libs intelfpgaup

Para estabelecer a comunicação entre o software e o hardware, utilizamos drivers que atuam como uma ponte, utilizando a biblioteca intelfpga. Esses drivers consistem em um conjunto de rotinas ou funções que possibilitam a interação do programa com os componentes específicos da FPGA DE1-SoC, como o acelerômetro, botões e vídeo. Essa abordagem simplifica o processo de leitura e escrita nos registradores, assim como nas configurações desses componentes.

Através do controle proporcionado pelos drivers, conseguimos gerenciar eficientemente os periféricos da FPGA. Isso inclui ajustar parâmetros e iniciar operações específicas fundamentais para o funcionamento do jogo.

A biblioteca "intelfpgaup" desempenhou um papel crucial, servindo como a ferramenta necessária para abstrair muitos detalhes complexos. Essa abstração evitou a necessidade de realizar configurações, leituras e inicializações dos periféricos em níveis mais baixos. Dessa forma, a implementação do jogo foi simplificada e tornada mais acessível, permitindo o foco nas lógicas do jogo em si, em vez de detalhes específicos da FPGA.

### `video.h` (por Luis)


Para utilização do [monitor](#monitor), como interface de saida do projeto, atraves de um cabo VGA conectado a DE1-SoC foi utilizado uma biblioteca desenvolvida em c *intelfpgaup/video.h* e um drive, ambos disponibilizados pela <a href="https://fpgacademy.org/courses.html">FPGAcademy</a>. O drive usa o arquivo */dev/video* para se comunicar, ao lê-lo é retornado uma *string* de caracteres "ccc rrr", na qual *ccc* é o número de colunas na tela VGA, e *rrr* é o número de linhas. Além disso o drive tem suporte aos seguintes comandos escritos no arquivo, *clear* e *pixel x,y cor*. O comando clear deve apagar a tela VGA definindo todos os pixels no buffer de pixels para a cor 0 (preto). O comando pixel deve definir o pixel na tela VGA nas coordenadas (x, y) para o valor da cor. A biblioteca trabalha em cima do drive, fazendo uso dessas funções básicas para fazer funções mais complexas.

<a name="monitor"></a>
<div align="center">
  <img src="/img/monitor.png" alt="Monitor" width="350">
   <p>
      Monitor Utilizado. Fonte: Os Autores
    </p>
</div>

O Computador DE1-SoC inclui uma porta de saída de vídeo com um controlador VGA que pode ser conectado a um monitor VGA padrão. O controlador VGA suporta uma resolução de tela de 640 × 480. A imagem exibida pelo controlador VGA é derivada de duas fontes: um buffer de pixels e um buffer de caracteres. O buffer de pixels para a porta de saída de vídeo armazena os dados (cores) para cada pixel que é exibido pelo controlador VGA, como ilustrado na [imagem do pixel buffer](#pixel_buffer). Para resolução de 640 × 480, cada um dos valores de pixel no buffer de pixels é copiado tanto para x quanto y.

<a name="pixel_buffer"></a>
<div align="center">
  <img src="/img/pixel_buffer.png" alt="Pixel Buffer" width="350">
   <p>
      Pixel Buffer. Fonte: <a href="https://fpgacademy.org/courses.html">FPGAcademy</a>
    </p>
</div>

Conforme ilustrado na parte a da [imagem do pixel address](#pixel_address), cada cor de pixel possui 16 bits, com cinco bits para os componentes azul e vermelho, e seis bits para o verde. Como mostrado na parte b da [imagem do pixel address](#pixel_address), os pixels são endereçados no buffer de pixels usando a combinação de um endereço base e um deslocamento x,y.

<a name="pixel_address"></a>
<div align="center">
  <img src="/img/pixel_address.png" alt="Pixel Address" width="250">
   <p>
      Pixel Address. Fonte: <a href="https://fpgacademy.org/courses.html">FPGAcademy</a>
    </p>
</div>

Com isso é possivel criar uma imagem escrevendo o valor de uma cor nos endereços dos pixels. Um controlador de buffer de pixels dedicado lê esses dados de pixels da memória e os envia para o display VGA. É possível preparar uma nova imagem para o display VGA sem alterar o conteúdo do buffer de pixels, usando o conceito de duplo buffering. Nesse esquema, dois buffers de pixels estão envolvidos, chamados de buffers frontal e traseiro.


Já a exibição de caracteres de texto funcina de forma semelhante a anterior. O buffer de caracteres é armazenado na memória interna da FPGA no Computador DE1-SoC. A [imagem do buffer de caracteres](#character_buffer) representa o buffer de caracteres para o display VGA, que tem uma resolução de 80 × 60 caracteres. Cada caractere ocupa um bloco de pixels de 8 × 8 na tela. Os caracteres são armazenados em cada uma das localizações mostradas na figura usando seus códigos ASCII; quando você armazena um caractere ASCII no buffer, um padrão correspondente de pixels é gerado automaticamente e exibido usando uma fonte embutida. 

<a name="character_buffer"></a>
<div align="center">
  <img src="/img/character_buffer.png" alt="Character Buffer" width="350">
   <p>
      Character Buffer. Fonte: <a href="https://fpgacademy.org/courses.html">FPGAcademy</a>
    </p>
</div>

#### Funções Utilizadas

Dá biblioteca *intelfpgaup/video.h* foram utilizadas as seguintes funções:
- **video_open():** Solicita o acesso a /dev/video.
- **video_read (&screen_x, &screen_y, &char_x, &char_y):** Obtem o número de linhas e colunas tanto do buffer de pixels quanto do de caracteres.
- **video_clear():** Limpa tela (executa o comando clear do drive).
- **video_box(x1, y1, x2, y2, color):** Desenha um quadrado na tela, utilizada o comando pixel no drive para preencher a figura.
- **video_show():** Copia o conteudo do buffer traseiro para o frontal (atualiza a tela).
- **video_text(x1, y1, "text"):** Escreve uma frase na tela na posição endicada.

- **video_erase():** Apaga os textos na tela.

### `accel.h` (por Nirvan)
A biblioteca `accel.h` é responsável por lidar com aceleração e movimentação no jogo. Certifique-se de entender como integrar essas funcionalidades em seu código para um controle suave.

### `KEY.h` (por Nicassio)
Para utilizar o botão como interface de entrada, presente no kitFPGA foi utilizada a biblioteca *intelfpgaup/key.h* e um drive, ambos disponibilizados pela <a href="https://fpgacademy.org/courses.html">FPGAcademy</a>, mais especificamente no Lab 3: Character Device Drivers.O drive usa o arquivo */dev/KEY*, que serve para identificar o estado dos botões de pressão do kit, para realizar uma melhor utilização do driver foram feitas algumas funções que serão listadas a seguir: 

#### Funções Utilizadas

Dá biblioteca *intelfpgaup/key.h* foram utilizadas as seguintes funções:
- **key_open():** Solicita o acesso a /dev/key, ao SO da placa para que tenha acesso por meio de arquivos, retornando 1 no sucesso e ou 0 na falha.
- **key_read(int * /*data*/):** lê o estado dos botões de pressão através do registrador de captura de borda, modificando o valor de data para 0 caso nenhum botão tenha sido pressionado, ou retornando o número do botão que foi pressionado, variando de 0 a 3 na variável data. 
- **key_close(int * /*data*/):**Fecha o arquivo /dev/key.
 


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

Para fazer o movimento da bola na tela foi criada uma *struct Ball* que possui as posições de cada um dos pontos usados para construção da bola na tela *(x1, y1)* e *(x2,y2)*, alem disso essa estrutura também possui a aceleração de *x1* e a aceleração de *x2* que são utilizados como parametros para o moviment da bola, eles definem o angulo, direção e velocidade. Para isso foi utilizado a função abaixo:

**ball_move(Ball \*ball):** Esta função move a bola na tela, alterando suas coordenadas *(x1, y1)* e *(x2, y2)* de acordo com sua velocidade *(acel_x, acel_y)*. Essa função é responsável por fazer a bola se movimentar pela tela em cada quadro de animação do jogo.

## Blocos (Geração) (por Nicassio)
Esta seção cobre a lógica por trás da geração de blocos no jogo. Inclua detalhes sobre como os blocos são criados, organizados e destruídos durante o jogo.

## Paredes (por Luis)
Explica a função e a implementação das paredes no jogo, se houver. Isso pode incluir paredes que afetam o movimento da bola ou outras mecânicas de jogo.

## Colisão (por Luis)
A colisão entre a bola e os demais objetos presentes na tela, paredes, paddle e blocos foi feita comparando a posição atual da bola em relação aos outros. Quando a bola se encontra dentro dos limites de outro objeto, ou seja quando seu *(x1, y1)* e *(x2, y2)* estvam entre os *(x1, y1)* e *(x2, y2)* do outro objeto a colisão acontece. Para isso foram utilizadas as funções a seguir:

**colide(Ball \*ball, Wall \*wall):** Esta função verifica se houve uma colisão entre a bola e uma parede. Ela calcula as novas coordenadas da bola após o próximo movimento (ball_x1, ball_y1, ball_x2, ball_y2) e verifica se a bola colidiu com a parede. Se ocorrer uma colisão retorna 1 para indicar a colisão.

**colidePaddle(Ball \*ball, Paddle \*paddle):** Esta função verifica se houve uma colisão entre a bola e o *paddle*. Ela calcula as novas coordenadas da bola após o próximo movimento e verifica se a bola colidiu com o remo.

**colisao_blocos(Ball\* ball, Bloco \*blocos):** Esta função verifica se houve uma colisão entre a bola e um dos blocos do cenário. Ela itera sobre todos os blocos no *array* e verifica se houve uma colisão com cada um deles. Se ocorrer uma colisão com algum bloco ativo, a função desativa o bloco e retorna 1 para indicar que ocorreu uma colisão.

**colide_bloco(Ball\* ball, Bloco\* bloco):** Esta função verifica se houve uma colisão entre a bola e um bloco específico. Ela calcula as novas coordenadas da bola após o próximo movimento e verifica se a bola colidiu com o bloco. Se ocorrer uma colisão com o bloco ativo, a função desativa o bloco e retorna 1 para indicar a colisão.

## Botões (Pause, Continue, Restart) (por Nicassio)
Explica como os botões de pause, continue e restart são implementados e como eles interagem com o fluxo do jogo.

## Textos (Mensagens e Pontuação)
As mensagens no jogo são geradas por meio da estrutura mensagem, que engloba os atributos x, y, mensagem e ativa. As variáveis x e y indicam a posição na tela em que a mensagem será exibida, enquanto a variável mensagem armazena a informação a ser apresentada. O atributo ativa determina se a mensagem deve ser exibida ou não. A lógica de exibição é controlada pelo método "exibirMensagem", que verifica a condição de ativação antes de utilizar a função para escrever a mensagem na tela.

Quanto à pontuação, é gerenciada pela estrutura pontuação, que possui os atributos x, y e pontuação. Os atributos x e y seguem a mesma lógica da estrutura mensagem, indicando a posição na tela. A variável pontuação representa os pontos acumulados pelo usuário. Dois métodos são incorporados a essa estrutura: atualizarPontuação e exibirPontuação. O método atualizarPontuação incrementa os pontos do usuário sempre que é chamado, enquanto o método exibirPontuação atualiza a mensagem da pontuação na tela. Essa abordagem permite um acompanhamento dinâmico dos pontos durante o jogo.

