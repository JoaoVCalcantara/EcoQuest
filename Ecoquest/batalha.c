#define _CRT_SECURE_NO_WARNINGS

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "batalha.h"
#include "bestiario.h"
#include "cacador.h"
#include "entidades.h"
#include "config_jogo.h"
#include "config_ui.h"
#include "config_cacador.h"

/* ==== Conteúdo reinserido: frases e funções de popup/lookup de frases ==== */
/* Frases de estudo */
static const char* frases_lobo_guara[] = {
    "Pernas Longas e Esguias - Adaptacao Evolutiva: A caracteristica mais marcante do lobo-guara, suas pernas longas e finas, e uma adaptacao crucial para a locomocao no capim alto do Cerrado e em areas abertas. Essa estrutura permite que ele enxergue acima da vegetacao rasteira e persiga suas presas de forma eficiente nesse tipo de terreno. Essa caracteristica o diferencia dos lobos e raposas de florestas mais densas.",
    "Dieta Onivora (Especializacao em Frutos) - Adaptacao Evolutiva: Diferente da maioria dos grandes canideos, que sao predominantemente carnivoros, o lobo-guara desenvolveu uma dieta onivora, com grande consumo de frutos (chegando a 90% da dieta em certas epocas). Essa dieta e uma resposta a disponibilidade sazonal de recursos no Cerrado. Papel Ecologico: Essa evolucao alimentar fez dele um dispersor de sementes vital para o bioma, especialmente da lobeira (Solanum lycocarpum), estabelecendo uma relacao de mutualismo.",
    "Orelhas Grandes - Adaptacao Evolutiva: Suas orelhas grandes e eretas sao essenciais para detectar presas noturnas (como pequenos roedores e insetos) e para a comunicacao em um ambiente de vegetacao aberta, onde o som pode se propagar melhor do que em uma floresta densa.",
    "Pelagem e Crina - Adaptacao Evolutiva: A pelagem vermelho-dourada ou alaranjada, com a crina de pelos pretos na nuca, ajuda na camuflagem em meio a vegetacao seca do Cerrado. A crina pode ser erizada, o que pode ter evoluido como uma forma de parecer maior para intimidar rivais ou predadores, ja que e um animal geralmente solitario e timido.",
    "Comportamento Solitario - Adaptacao Evolutiva: A vida solitaria e territorialista, formando casais apenas na epoca de reproducao, e uma adaptacao a um ambiente onde os recursos alimentares (frutos e pequenas presas) sao mais dispersos, nao favorecendo a captura em matilhas tipica dos lobos verdadeiros."
};

static const char* frases_jacare[] = {
    "Corpo Hidrodinamico: A forma do corpo e a longa cauda musculosa evoluiram para permitir locomocao e manobrabilidade eficientes na agua. Adaptacoes Aquaticas: Diferente de seus ancestrais, que eram terrestres e podiam ate ser bipedes, os jacares desenvolveram membros mais laterais (postura rastejante) e um focinho mais comprido, adaptacoes para a vida em rios, lagos e pantanos.",
    "Olhos e Narinas no Topo da Cabeca: Uma adaptacao crucial para a emboscada. Isso permite que o jacare permaneca quase totalmente submerso, observando e respirando, enquanto espera a presa.",
    "Pele Resistente: A pele espessa e com placas osseas (osteodermes) oferece protecao contra predadores e ajuda na retencao de agua, uma adaptacao a vida em ambientes sazonais.",
    "Ovo com Casca Rigida (Amniota): Uma das aquisicoes evolutivas mais importantes dos repteis foi o ovo com casca, que protege o embriao da dessecacao, permitindo a reproducao em terra firme, longe da dependencia direta da agua para os ovos.",
    "Eficiencia Termica (Ectotermia): Por serem ectotermicos (dependem do ambiente para regular a temperatura corporal), eles desenvolveram comportamentos como tomar sol nas margens para aquecer-se, o que e energeticamente eficiente e uma adaptacao a climas quentes.",
    "Comportamento Maternal: Uma caracteristica evolutiva notavel e o cuidado parental. As femeas protegem os ninhos e ate transportam os filhotes recem-nascidos para a agua, defendendo-os de predadores por um longo periodo."
};

static const char* frases_boto[] = {
    "Boto Coloracao Rosada: Os botos nao nascem cor-de-rosa; eles nascem cinzentos e a coloracao surge com a idade. A tonalidade rosa e influenciada pela idade, sexo e, principalmente, por cicatrizes de arranhoes em troncos ou disputas entre machos. Isso pode ser uma forma de selecao sexual, onde os machos mais rosados (mais marcados por lutas) sao mais desejaveis.",
    "Flexibilidade Corporal e Pescoco Articulado: Ao contrario dos golfinhos marinhos, cujas vertebras cervicais sao fundidas, as do boto nao sao. Adaptacao Evolutiva: Essa flexibilidade permite que ele vire a cabeca em um angulo de ate 90 graus, essencial para manobrar e cacar em areas alagadas, igapos e entre troncos de arvores, algo inviavel em mar aberto.",
    "Focinho Longo e Dentes Adaptados: O boto possui um focinho mais longo e fino do que a maioria dos golfinhos marinhos, com dentes diferentes na frente (para agarrar) e no fundo da boca (para triturar). Adaptacao Evolutiva: Essa especializacao permite a captura de uma dieta variada, incluindo peixes, tartarugas e caranguejos, adaptando-se aos recursos disponiveis nos rios.",
    "Visao Reduzida e Ecolocalizacao Aprimorada: Embora ainda funcional, a visao do boto e menos desenvolvida do que a de golfinhos marinhos, o que faz sentido em aguas turvas e escuras da Amazonia. Adaptacao Evolutiva: Em compensacao, seu sistema de ecolocalizacao e extremamente sofisticado, permitindo-lhe navegar e cacar com precisao em condicoes de pouca ou nenhuma visibilidade.",
    "Ausencia de Nadadeira Dorsal Proeminente: Em vez de uma barbatana dorsal alta e rigida, o boto tem apenas uma pequena crista dorsal. Adaptacao Evolutiva: Isso facilita a natacao em aguas rasas e a passagem por baixo de galhos e vegetacao densa durante as cheias sazonais da Amazonia.",
    "Cerebro Grande e Complexo: O boto-cor-de-rosa tem o maior cerebro entre os golfinhos de agua doce e, proporcionalmente, utiliza mais capacidade cerebral que os humanos."
};

static const char* frases_onca[] = {
    "Mandibula e Forca de Mordida Excepcionais: Esta e talvez a sua caracteristica evolutiva mais distintiva. A onca-pintada desenvolveu uma cabeca grande e robusta, com musculos mandibulares e dentes caninos extremamente fortes, permitindo-lhe ter uma das mordidas mais potentes entre todos os felinos. Vantagem Evolutiva: Essa forca permite a onca abater presas grandes e, crucialmente, perfurar cranios de mamiferos ou cascos de repites (como tartarugas e jacares), um metodo de caca raro entre felinos, que geralmente matam por estrangulamento.",
    "Constituicao Fisica Musculosa e Membros Curtos: Em contraste com a estrutura mais esguia de guepardos ou leopardos, a onca-pintada tem um corpo atarracado e musculoso, com membros curtos e macicos. Vantagem Evolutiva: Essa estrutura fisica nao e para velocidade em corridas longas, mas sim para forca, agilidade em curtas distancias e a capacidade de nadar e escalar arvores, permitindo-lhe emboscar presas em ambientes densos e proximos a agua.",
    "Padrao de Pelagem (Rosetas): Sua pelagem de cor amarelo-dourada a castanho-avermelhada e coberta por rosetas complexas (manchas circulares com pontos pretos dentro). Vantagem Evolutiva: Este padrao fornece uma camuflagem excelente na vegetacao densa e na luz filtrada de florestas e pantanos, facilitando a caca por emboscada.",
    "Habilidades Aquaticas: Diferente da maioria dos felinos que evitam a agua, a onca-pintada e uma excelente nadadora, uma adaptacao vital para a vida em biomas com muitos corpos de agua, como a Amazonia e o Pantanal, onde pode cacar presas aquaticas e semi-aquaticas."
};

static const char* frases_harpia[] = {
    "Visao Excepcional - Adaptacao Evolutiva: A harpia possui uma das visoes mais agucadas do reino animal, com olhos grandes adaptados para enxergar presas a grandes distancias mesmo em florestas densas. Pode detectar um macaco a mais de 200 metros de distancia.",
    "Garras Poderosas - Adaptacao Evolutiva: Suas garras podem medir ate 12 cm, do tamanho das garras de um urso-pardo, tornando-a capaz de capturar presas grandes como preguicas e macacos. Essa forca permite perfurar o cranio das presas instantaneamente.",
    "Voo Agil em Floresta Densa - Adaptacao Evolutiva: Apesar do tamanho (envergadura de ate 2 metros), a harpia desenvolveu asas curtas e largas que permitem manobrabilidade excepcional entre arvores, uma adaptacao crucial para cacar no dossel da Amazonia.",
    "Cacadora de Dossel - Papel Ecologico: Como predadora de topo do dossel amazonico, a harpia controla populacoes de primatas e preguicas, sendo essencial para o equilibrio ecologico da floresta tropical.",
    "Nidificacao e Cuidado Parental - Adaptacao Evolutiva: Constroi ninhos enormes (ate 1.5m de diametro) no topo das arvores mais altas e cuida de apenas um filhote por vez durante ate 2 anos, garantindo alta taxa de sobrevivencia."
};

static const char* frases_mico[] = {
    "Pelagem Dourada Vibrante - Adaptacao Evolutiva: A coloracao laranja-dourada brilhante nao e apenas estetica; ela pode ter evoluido como sinalizacao social em seu habitat de Mata Atlantica, facilitando a coesao do grupo em meio a vegetacao densa.",
    "Dieta Onivora Especializada - Adaptacao Evolutiva: Alimenta-se de frutas, insetos, pequenos vertebrados e seiva de arvores. Essa dieta variada permite sobrevivencia em ambientes fragmentados da Mata Atlantica, adaptando-se a diferentes recursos sazonais.",
    "Unhas em Forma de Garra - Adaptacao Evolutiva: Diferente de outros primatas, possui unhas em forma de garra que permitem escalar troncos verticais e extrair insetos da casca das arvores, uma especializacao alimentar importante.",
    "Comunicacao Vocal Complexa - Adaptacao Evolutiva: Desenvolveu um sistema de vocalizacoes sofisticado para manter contato com o grupo em meio a floresta densa, incluindo chamados de alarme especificos para diferentes tipos de predadores.",
    "Especie Bandeira da Conservacao - Papel Ecologico: Como especie carismatica e endemica da Mata Atlantica, o mico-leao-dourado tornou-se simbolo da conservacao brasileira, ajudando a proteger habitats inteiros."
};

static const char* frases_capivara[] = {
    "Maior Roedor do Mundo - Adaptacao Evolutiva: Com ate 65 kg, a capivara evoluiu um tamanho corporal que a torna menos vulneravel a predadores pequenos, mantendo ainda a flexibilidade de herbivoro semi-aquatico.",
    "Adaptacoes Aquaticas - Adaptacao Evolutiva: Membranas interdigitais (como patos) facilitam a natacao, olhos e narinas no topo da cabeca permitem ficar submersa observando predadores, e pode prender a respiracao por ate 5 minutos.",
    "Herbivoria Especifica - Adaptacao Evolutiva: Seus dentes crescem continuamente (como todos roedores), adaptacao essencial para sua dieta de gramineas duras que desgastariam dentes normais rapidamente.",
    "Comportamento Social Complexo - Adaptacao Evolutiva: Vivem em grupos hierarquicos de ate 20 individuos, uma estrategia anti-predacao crucial em ambientes abertos como pantanos, onde a vigilancia coletiva aumenta chances de sobrevivencia.",
    "Engenheira de Ecossistemas - Papel Ecologico: Ao pastar grandes quantidades de vegetacao aquatica e terrestre, a capivara modifica o habitat para outras especies, sendo considerada engenheira de ecossistemas em areas alagadas."
};

static const char* frases_tubarao[] = {
    "Tolerancia a Agua Doce - Adaptacao Evolutiva: Unico tubarao capaz de viver tanto em agua salgada quanto doce, o tubarao-touro desenvolveu rins e glandulas especiais que regulam concentracao de sal no corpo, permitindo navegar rios da Amazonia.",
    "Mandibulas Poderosas - Adaptacao Evolutiva: Possui uma das mordidas mais fortes entre tubaroes, adaptacao para capturar presas grandes e variedades em ambientes diversos (oceano, estuarios, rios), maximizando oportunidades alimentares.",
    "Agressividade e Territorialidade - Adaptacao Evolutiva: Sua reputacao de agressividade pode ser uma estrategia evolutiva para defender territórios ricos em recursos tanto marinhos quanto fluviais, onde a competicao e intensa.",
    "Reproducao Vivipara - Adaptacao Evolutiva: As femeas dao a luz filhotes vivos (viviparidade), e frequentemente usam estuarios e rios como bercarios onde filhotes tem menos predadores, aumentando sobrevivencia da prole.",
    "Predador Oportunista de Topo - Papel Ecologico: Como predador generalista que transita entre ambientes marinhos e dulcicolas, o tubarao-touro conecta cadeias alimentares de diferentes ecossistemas, regulando populacoes de presas em multiplos habitats."
};

static const char* frases_tamandua[] = {
    "Lingua Longa e Pegajosa - Adaptacao Evolutiva: Sua lingua pode se estender ate 60 cm, coberta por saliva pegajosa, adaptacao perfeita para capturar formigas e cupins dentro de tuneis estreitos, sua dieta exclusiva.",
    "Ausencia de Dentes - Adaptacao Evolutiva: Evoluiu sem dentes porque sua dieta de insetos moles nao exige mastigacao; o estomago muscular (como moela de ave) tritura o alimento, uma economia energetica evolutiva significativa.",
    "Garras Poderosas - Adaptacao Evolutiva: Garras dianteiras enormes servem para escavar cupinzeiros e formigueiros duros e tambem para defesa contra predadores como oncas. Essa multifuncionalidade aumentou sua sobrevivencia.",
    "Baixo Metabolismo - Adaptacao Evolutiva: Tem uma das taxas metabolicas mais baixas entre mamiferos, adaptacao ao Cerrado onde recursos podem ser escassos; economiza energia movendo-se lentamente e dormindo ate 16 horas por dia.",
    "Dispersor de Sementes Involuntario - Papel Ecologico: Ao escavar cupinzeiros, revolve solo e espalha sementes que estavam enterradas, contribuindo indiretamente para regeneracao de vegetacao do Cerrado."
};

static const char* frases_pirarucu[] = {
    "Maior Peixe de Escamas de Agua Doce - Adaptacao Evolutiva: Pode atingir ate 3 metros e pesar 200 kg, tamanho que evoluiu como vantagem competitiva na Amazonia, permitindo-lhe dominar outros peixes e escapar de predadores menores.",
    "Respiracao Aerea Obrigatoria - Adaptacao Evolutiva: Desenvolveu a capacidade de respirar ar atmosferico diretamente (atraves de uma bexiga natatoria modificada), adaptacao crucial para sobreviver em aguas pobres em oxigenio da varzea amazonica durante a seca.",
    "Lingua Ossea (Dentes na Lingua) - Adaptacao Evolutiva: Possui uma placa ossea na lingua com pequenos dentes que, junto com dentes no ceu da boca, tritura presas como peixes e crustaceos, uma especializacao alimentar unica.",
    "Cuidado Parental (Macho) - Adaptacao Evolutiva: Os machos protegem agressivamente ninhos e filhotes durante os primeiros meses, um comportamento raro entre peixes que aumenta dramaticamente a sobrevivencia da prole em ambientes com muitos predadores.",
    "Escamas Enormes (Armadura Natural) - Adaptacao Evolutiva: Suas escamas podem atingir 6 cm e sao extremamente duras, funcionando como armadura contra predadores como piranhas e jacares jovens, e historicamente usadas por indigenas como lixas e utensilios.",
    "Predador de Topo Aquatico - Papel Ecologico: Como um dos maiores peixes da Amazonia, o pirarucu regula populacoes de peixes menores, sendo essencial para o equilibrio dos ecossistemas aquaticos da regiao."
};

/* Função que retorna array de frases e total com normalização de nome */
static const char** obter_frases_animal(const char* nome_animal, int* total_frases) {
    if (!nome_animal || !total_frases) return NULL;

    char nome_lower[64];
    snprintf(nome_lower, sizeof(nome_lower), "%s", nome_animal);
    for (int i = 0; nome_lower[i]; i++) {
        if (nome_lower[i] >= 'A' && nome_lower[i] <= 'Z') nome_lower[i] += ('a' - 'A');
    }

    char nome_normalizado[64];
    int j = 0;
    for (int i = 0; nome_lower[i] && j < (int)sizeof(nome_normalizado) - 1; i++) {
        // Substituir espacos E hifens por underscores
        if (nome_lower[i] == ' ' || nome_lower[i] == '-') {
            nome_normalizado[j++] = '_';
        } else {
            nome_normalizado[j++] = nome_lower[i];
        }
    }
    nome_normalizado[j] = '\0';

    printf("[DEBUG] Nome original: '%s' | Nome normalizado: '%s'\n", nome_animal, nome_normalizado);

    if (strcmp(nome_normalizado, "lobo_guara") == 0 || strcmp(nome_lower, "lobo guara") == 0) {
        *total_frases = (int)(sizeof(frases_lobo_guara) / sizeof(frases_lobo_guara[0]));
        return frases_lobo_guara;
    }
    else if (strcmp(nome_lower, "jacare") == 0 || strcmp(nome_normalizado, "jacare") == 0) {
        *total_frases = (int)(sizeof(frases_jacare) / sizeof(frases_jacare[0]));
        return frases_jacare;
    }
    else if (strcmp(nome_lower, "boto") == 0 || strcmp(nome_normalizado, "boto") == 0) {
        *total_frases = (int)(sizeof(frases_boto) / sizeof(frases_boto[0]));
        return frases_boto;
    }
    else if (strcmp(nome_lower, "onca") == 0 || strcmp(nome_normalizado, "onca") == 0 || strcmp(nome_lower, "onca-pintada") == 0) {
        *total_frases = (int)(sizeof(frases_onca) / sizeof(frases_onca[0]));
        return frases_onca;
    }
    else if (strcmp(nome_lower, "harpia") == 0) {
        *total_frases = (int)(sizeof(frases_harpia) / sizeof(frases_harpia[0]));
        return frases_harpia;
    }
    else if (strcmp(nome_normalizado, "mico_leao_dourado") == 0) {
        *total_frases = (int)(sizeof(frases_mico) / sizeof(frases_mico[0]));
        return frases_mico;
    }
    else if (strcmp(nome_lower, "capivara") == 0) {
        *total_frases = (int)(sizeof(frases_capivara) / sizeof(frases_capivara[0]));
        return frases_capivara;
    }
    else if (strcmp(nome_lower, "tubarao touro") == 0 || strcmp(nome_normalizado, "tubarao_touro") == 0) {
        *total_frases = (int)(sizeof(frases_tubarao) / sizeof(frases_tubarao[0]));
        return frases_tubarao;
    }
    else if (strcmp(nome_normalizado, "tamandua_bandeira") == 0) {
        *total_frases = (int)(sizeof(frases_tamandua) / sizeof(frases_tamandua[0]));
        return frases_tamandua;
    }
    else if (strcmp(nome_lower, "pirarucu") == 0) {
        *total_frases = (int)(sizeof(frases_pirarucu) / sizeof(frases_pirarucu[0]));
        return frases_pirarucu;
    }

    printf("[AVISO] Animal '%s' nao possui frases cadastradas!\n", nome_animal);
    *total_frases = 0;
    return NULL;
}

/* Popup que exibe texto longo e espera ENTER/SPACE/ESC ou fechar display */
static void mostrar_popup_estudo(ALLEGRO_FONT* fonte, ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE* event_queue, const char* texto) {
    if (!display || !texto) return;
    int largura = al_get_display_width(display);
    int altura = al_get_display_height(display);

    float box_w = largura * 0.75f;
    float box_h = altura * 0.65f;
    float box_x = (largura - box_w) / 2.0f;
    float box_y = (altura - box_h) / 2.0f;

    char buffer[4096];
    strncpy(buffer, texto, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    float text_x = box_x + 20;
    float text_y = box_y + 50;
    float max_width = box_w - 40;
    float line_height = (fonte ? al_get_font_line_height(fonte) : 12.0f) + 4;

    al_clear_to_color(al_map_rgb(COR_FUNDO_PRETO_R, COR_FUNDO_PRETO_G, COR_FUNDO_PRETO_B));
    al_draw_filled_rectangle(0, 0, (float)largura, (float)altura, al_map_rgba(COR_FUNDO_PRETO_R, COR_FUNDO_PRETO_G, COR_FUNDO_PRETO_B, 180));

    al_draw_filled_rounded_rectangle(box_x, box_y, box_x + box_w, box_y + box_h, 10, 10, al_map_rgb(240, 240, 230));
    al_draw_rounded_rectangle(box_x, box_y, box_x + box_w, box_y + box_h, 10, 10, al_map_rgb(100, 70, 30), 3);

    al_draw_text(fonte, al_map_rgb(40, 20, 0), box_x + box_w / 2, box_y + 15, ALLEGRO_ALIGN_CENTRE, "=== INFORMACAO DO ANIMAL ===");

    char linha_atual[512] = "";
    char* palavra_ptr = buffer;
    while (*palavra_ptr) {
        while (*palavra_ptr == ' ') palavra_ptr++;
        if (!*palavra_ptr) break;
        char* fim_palavra = palavra_ptr;
        while (*fim_palavra && *fim_palavra != ' ') fim_palavra++;

        size_t len_palavra = (size_t)(fim_palavra - palavra_ptr);
        if (len_palavra > 255) len_palavra = 255;
        char palavra[256];
        strncpy(palavra, palavra_ptr, len_palavra);
        palavra[len_palavra] = '\0';

        char teste[512];
        if (linha_atual[0] == '\0') {
            strncpy(teste, palavra, sizeof(teste) - 1);
            teste[sizeof(teste) - 1] = '\0';
        }
        else {
            snprintf(teste, sizeof(teste), "%s %s", linha_atual, palavra);
        }

        float w = al_get_text_width(fonte, teste);
        if (w > max_width && linha_atual[0] != '\0') {
            al_draw_text(fonte, al_map_rgb(COR_TEXTO_PRETO_R, COR_TEXTO_PRETO_G, COR_TEXTO_PRETO_B), text_x, text_y, ALLEGRO_ALIGN_LEFT, linha_atual);
            text_y += line_height;
            strncpy(linha_atual, palavra, sizeof(linha_atual) - 1);
            linha_atual[sizeof(linha_atual) - 1] = '\0';
        }
        else {
            strncpy(linha_atual, teste, sizeof(linha_atual) - 1);
            linha_atual[sizeof(linha_atual) - 1] = '\0';
        }
        palavra_ptr = fim_palavra;
    }
    if (linha_atual[0] != '\0')
        al_draw_text(fonte, al_map_rgb(COR_TEXTO_PRETO_R, COR_TEXTO_PRETO_G, COR_TEXTO_PRETO_B), text_x, text_y, ALLEGRO_ALIGN_LEFT, linha_atual);

    al_draw_text(fonte, al_map_rgb(COR_TEXTO_CINZA_R, COR_TEXTO_CINZA_G, COR_TEXTO_CINZA_B), box_x + box_w / 2, box_y + box_h - 30, ALLEGRO_ALIGN_CENTRE, MSG_PRESSIONE_ENTER_CONTINUAR);
    al_flip_display();

    if (event_queue) {
        bool esperando = true;
        while (esperando) {
            ALLEGRO_EVENT ev;
            al_wait_for_event(event_queue, &ev);
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER ||
                    ev.keyboard.keycode == ALLEGRO_KEY_SPACE ||
                    ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    esperando = false;
                }
            }
            else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                esperando = false;
            }
        }
    }
    else {
        al_rest(3.0);
    }
}

/* NOVA: Detecta o cenário de batalha baseado no fundo usado */
static JogoCenas detectar_cenario_por_fundo(const char* caminho_fundo) {
    if (!caminho_fundo) return CENARIO1;
    
    if (strstr(caminho_fundo, "selva") || strstr(caminho_fundo, "SELVA")) return CENARIO1;
    if (strstr(caminho_fundo, "pantano") || strstr(caminho_fundo, "PANTANO")) return CENARIO2;
    if (strstr(caminho_fundo, "lago") || strstr(caminho_fundo, "LAGO")) return CENARIO3;
    if (strstr(caminho_fundo, "cerrado") || strstr(caminho_fundo, "CERRADO")) return CENARIO4;
    
    return CENARIO1; // fallback
}

/* NOVA: Obter configuração por cenário */
static void obter_config_por_cenario(JogoCenas cenario, 
                                     float* jogador_x, float* jogador_y, float* jogador_escala,
                                     float* oponente_x, float* oponente_y, float* oponente_escala,
                                     bool eh_cacador) {
    switch (cenario) {
    case CENARIO1: // SELVA
        *jogador_x = SELVA_JOGADOR_X;
        *jogador_y = SELVA_JOGADOR_Y;
        *jogador_escala = SELVA_JOGADOR_ESCALA;
        if (eh_cacador) {
            *oponente_x = SELVA_CACADOR_X;
            *oponente_y = SELVA_CACADOR_Y;
            *oponente_escala = SELVA_CACADOR_ESCALA;
        } else {
            *oponente_x = SELVA_ANIMAL_X;
            *oponente_y = SELVA_ANIMAL_Y;
            *oponente_escala = SELVA_ANIMAL_ESCALA;
        }
        break;
        
    case CENARIO2: // PANTANO
        *jogador_x = PANTANO_JOGADOR_X;
        *jogador_y = PANTANO_JOGADOR_Y;
        *jogador_escala = PANTANO_JOGADOR_ESCALA;
        if (eh_cacador) {
            *oponente_x = PANTANO_CACADOR_X;
            *oponente_y = PANTANO_CACADOR_Y;
            *oponente_escala = PANTANO_CACADOR_ESCALA;
        } else {
            *oponente_x = PANTANO_ANIMAL_X;
            *oponente_y = PANTANO_ANIMAL_Y;
            *oponente_escala = PANTANO_ANIMAL_ESCALA;
        }
        break;
        
    case CENARIO3: // LAGO
        *jogador_x = LAGO_JOGADOR_X;
        *jogador_y = LAGO_JOGADOR_Y;
        *jogador_escala = LAGO_JOGADOR_ESCALA;
        if (eh_cacador) {
            *oponente_x = LAGO_CACADOR_X;
            *oponente_y = LAGO_CACADOR_Y;
            *oponente_escala = LAGO_CACADOR_ESCALA;
        } else {
            *oponente_x = LAGO_ANIMAL_X;
            *oponente_y = LAGO_ANIMAL_Y;
            *oponente_escala = LAGO_ANIMAL_ESCALA;
        }
        break;
        
    case CENARIO4: // CERRADO
        *jogador_x = CERRADO_JOGADOR_X;
        *jogador_y = CERRADO_JOGADOR_Y;
        *jogador_escala = CERRADO_JOGADOR_ESCALA;
        if (eh_cacador) {
            *oponente_x = CERRADO_CACADOR_X;
            *oponente_y = CERRADO_CACADOR_Y;
            *oponente_escala = CERRADO_CACADOR_ESCALA;
        } else {
            *oponente_x = CERRADO_ANIMAL_X;
            *oponente_y = CERRADO_ANIMAL_Y;
            *oponente_escala = CERRADO_ANIMAL_ESCALA;
        }
        break;
        
    default:
        // Fallback para SELVA
        *jogador_x = SELVA_JOGADOR_X;
        *jogador_y = SELVA_JOGADOR_Y;
        *jogador_escala = SELVA_JOGADOR_ESCALA;
        *oponente_x = eh_cacador ? SELVA_CACADOR_X : SELVA_ANIMAL_X;
        *oponente_y = eh_cacador ? SELVA_CACADOR_Y : SELVA_ANIMAL_Y;
        *oponente_escala = eh_cacador ? SELVA_CACADOR_ESCALA : SELVA_ANIMAL_ESCALA;
        break;
    }
}

/* ========== FUNÇÕES DE CRIAÇÃO DE OPONENTES ========== */

OponenteBatalha* criar_oponente_animal(Animal* animal) {
    if (!animal) return NULL;
    
    OponenteBatalha* oponente = (OponenteBatalha*)malloc(sizeof(OponenteBatalha));
    if (!oponente) return NULL;
    
    oponente->tipo = BATALHA_ANIMAL;
    oponente->nome = animal->nome;
    oponente->vida_atual = animal->nivel_alimentacao;
    oponente->vida_maxima = 100;
    oponente->animal = animal;
    oponente->cacador = NULL;
    oponente->sprite = NULL;
    oponente->caminho_fundo = animal->caminho_fundo_batalha;
    
    // Config sprite básica (será sobrescrita pelas configs por cenário)
    oponente->config_sprite.nome_entidade = animal->nome;
    oponente->config_sprite.entidade_x = 0.0f;
    oponente->config_sprite.entidade_y = 0.0f;
    oponente->config_sprite.entidade_escala = 0.25f;
    
    return oponente;
}

OponenteBatalha* criar_oponente_cacador(Cacador* cacador) {
    if (!cacador) return NULL;
    
    OponenteBatalha* oponente = (OponenteBatalha*)malloc(sizeof(OponenteBatalha));
    if (!oponente) return NULL;
    
    oponente->tipo = BATALHA_CACADOR;
    oponente->nome = cacador->nome;
    oponente->vida_atual = cacador->vida;
    oponente->vida_maxima = cacador->vida_maxima;
    oponente->animal = NULL;
    oponente->cacador = cacador;
    oponente->sprite = NULL;
    oponente->caminho_fundo = cacador->dados_batalha.caminho_fundo_batalha;
    
    // Config sprite básica (será sobrescrita pelas configs por cenário)
    oponente->config_sprite.nome_entidade = cacador->nome;
    oponente->config_sprite.entidade_x = 0.0f;
    oponente->config_sprite.entidade_y = 0.0f;
    oponente->config_sprite.entidade_escala = 0.30f;
    
    return oponente;
}

void destruir_oponente_batalha(OponenteBatalha* oponente) {
    if (oponente) free(oponente);
}

/* ========== DESENHO UNIFICADO ========== */

static void aplicar_ajustes_animal_especifico(const char* nome_animal,
    float* oponente_y,
    float* oponente_escala) {
    if (!nome_animal) return;

    char nome_lower[64];
    snprintf(nome_lower, sizeof(nome_lower), "%s", nome_animal);
    for (int i = 0; nome_lower[i]; i++) {
        if (nome_lower[i] >= 'A' && nome_lower[i] <= 'Z')
            nome_lower[i] += ('a' - 'A');
    }

    // Mico-leão-dourado: diminuir 50px na altura (aumentar Y)
    if (strstr(nome_lower, "mico") != NULL) {
        *oponente_y += MICO_OFFSET_Y;
    }

    // Harpia: aumentar escala
    if (strstr(nome_lower, "harpia") != NULL) {
        *oponente_escala += HARPIA_ESCALA_EXTRA;
    }

    // Jacaré: abaixar 35px na altura (aumentar Y)
    if (strstr(nome_lower, "jacare") != NULL) {
        *oponente_y += JACARE_OFFSET_Y;
    }
}

void desenhar_batalha_unificada(ALLEGRO_FONT* fonte, int opcao, ALLEGRO_DISPLAY* display,
                               OponenteBatalha* oponente, entidade* jogador,
                               RecursosBatalha* recursos, int vida_jogador, bool turno_jogador) {
    if (!display || !oponente) return;

    int largura = al_get_display_width(display);
    int altura  = al_get_display_height(display);
    al_clear_to_color(al_map_rgb(COR_FUNDO_PRETO_R, COR_FUNDO_PRETO_G, COR_FUNDO_PRETO_B));

    // Fundo
    if (recursos && recursos->fundo_batalha) {
        al_draw_scaled_bitmap(recursos->fundo_batalha, 0, 0,
            (float)al_get_bitmap_width(recursos->fundo_batalha),
            (float)al_get_bitmap_height(recursos->fundo_batalha),
            0, 0, (float)largura, (float)altura, 0);
    }

    // Detecta cenário e obtém posições
    JogoCenas cenario = detectar_cenario_por_fundo(oponente->caminho_fundo);
    bool eh_cacador = (oponente->tipo == BATALHA_CACADOR);
    
    float jogador_x_cfg, jogador_y_cfg, jogador_escala_cfg;
    float oponente_x_cfg, oponente_y_cfg, oponente_escala_cfg;
    
    obter_config_por_cenario(cenario, 
                            &jogador_x_cfg, &jogador_y_cfg, &jogador_escala_cfg,
                            &oponente_x_cfg, &oponente_y_cfg, &oponente_escala_cfg,
                            eh_cacador);

    if (oponente->tipo == BATALHA_ANIMAL && oponente->nome) {
    aplicar_ajustes_animal_especifico(oponente->nome, &oponente_y_cfg, &oponente_escala_cfg);
}

    // Desenhar jogador com posição do cenário
    bool tem_jogador = false;
    float pj_w = 0.0f, pj_h = 0.0f;
    if (recursos && recursos->sprite_jogador) {
        float jw = (float)al_get_bitmap_width(recursos->sprite_jogador);
        float jh = (float)al_get_bitmap_height(recursos->sprite_jogador);

        pj_w = jw * jogador_escala_cfg;
        pj_h = jh * jogador_escala_cfg;

        float jogador_x = jogador_x_cfg - pj_w / 2.0f;
        float jogador_y = jogador_y_cfg - pj_h / 2.0f;

        tem_jogador = true;
        al_draw_scaled_bitmap(recursos->sprite_jogador, 0, 0, jw, jh,
                              jogador_x, jogador_y, pj_w, pj_h, 0);
    }

    // Desenhar oponente com posição do cenário
    bool tem_oponente = false;
    float op_w = 0.0f, op_h = 0.0f;
    ALLEGRO_BITMAP* sprite_oponente = NULL;
    
    if (oponente->tipo == BATALHA_ANIMAL && recursos && recursos->sprite_animal) {
        sprite_oponente = recursos->sprite_animal;
    } else if (oponente->tipo == BATALHA_CACADOR && recursos && recursos->sprite_cacador) {
        sprite_oponente = recursos->sprite_cacador;
    }
    
    float op_x = 0.0f, op_y = 0.0f;
    if (sprite_oponente) {
        float ow = (float)al_get_bitmap_width(sprite_oponente);
        float oh = (float)al_get_bitmap_height(sprite_oponente);
        
        op_w = ow * oponente_escala_cfg;
        op_h = oh * oponente_escala_cfg;
        
        op_x = oponente_x_cfg - op_w / 2.0f;
        op_y = oponente_y_cfg - op_h / 2.0f;
        
        tem_oponente = true;
        al_draw_scaled_bitmap(sprite_oponente, 0, 0, ow, oh,
                              op_x, op_y, op_w, op_h, 0);
    }

    // Barras
    const float bar_w = 160.0f;
    const float bar_h = 12.0f;
    const float bar_gap = 8.0f;
    const float gap_between = 6.0f;

    // Barra do oponente
    if (tem_oponente && oponente->vida_maxima > 0) {
        int vida_op = oponente->vida_atual;
        if (vida_op < 0) vida_op = 0;
        if (vida_op > oponente->vida_maxima) vida_op = oponente->vida_maxima;
        float p = (float)vida_op / (float)oponente->vida_maxima;
        float center_x = op_x + op_w / 2.0f;
        
        if (oponente->tipo == BATALHA_ANIMAL && oponente->animal) {
            // Barra de alimentação
            float alim_x = center_x - bar_w / 2.0f;
            float alim_y = op_y - (2 * bar_h + bar_gap + gap_between);
            
            int alim = oponente->animal->nivel_alimentacao;
            if (alim < 0) alim = 0;
            if (alim > 100) alim = 100;
            float p_alim = (float)alim / 100.0f;
            
            al_draw_filled_rectangle(alim_x, alim_y, alim_x + bar_w, alim_y + bar_h,
                                     al_map_rgb(50, 50, 50));
            al_draw_filled_rectangle(alim_x, alim_y, alim_x + bar_w * p_alim, alim_y + bar_h,
                                     al_map_rgb(COR_TEXTO_VERDE_CLARO_R, COR_TEXTO_VERDE_CLARO_G, COR_TEXTO_VERDE_CLARO_B));
            al_draw_rectangle(alim_x, alim_y, alim_x + bar_w, alim_y + bar_h,
                              al_map_rgb(COR_TEXTO_BRANCO_R, COR_TEXTO_BRANCO_G, COR_TEXTO_BRANCO_B), 1.0f);
            
            // Barra de estudo
            float estudo_x = center_x - bar_w / 2.0f;
            float estudo_y = alim_y + bar_h + gap_between;
            
            int exp = oponente->animal->experiencia;
            if (exp < 0) exp = 0;
            if (exp > 100) exp = 100;
            float p_estudo = (float)exp / 100.0f;
            
            al_draw_filled_rectangle(estudo_x, estudo_y, estudo_x + bar_w, estudo_y + bar_h,
                                     al_map_rgb(50, 50, 50));
            al_draw_filled_rectangle(estudo_x, estudo_y, estudo_x + bar_w * p_estudo, estudo_y + bar_h,
                                     al_map_rgb(100, 150, 255));
            al_draw_rectangle(estudo_x, estudo_y, estudo_x + bar_w, estudo_y + bar_h,
                              al_map_rgb(COR_TEXTO_BRANCO_R, COR_TEXTO_BRANCO_G, COR_TEXTO_BRANCO_B), 1.0f);
        } else {
            // Para caçadores: 1 barra (vida)
            float bx = center_x - bar_w / 2.0f;
            float by = op_y - bar_h - bar_gap;
            al_draw_filled_rectangle(bx, by, bx + bar_w, by + bar_h, al_map_rgb(50, 50, 50));
            al_draw_filled_rectangle(bx, by, bx + bar_w * p, by + bar_h, al_map_rgb(220, 40, 40));
            al_draw_rectangle(bx, by, bx + bar_w, by + bar_h, al_map_rgb(COR_TEXTO_BRANCO_R, COR_TEXTO_BRANCO_G, COR_TEXTO_BRANCO_B), 1.0f);
        }
    }

    // Barra do jogador
    if (tem_jogador) {
        int vida_j = vida_jogador;
        if (vida_j < 0) vida_j = 0;
        if (vida_j > 100) vida_j = 100;
        float p = (float)vida_j / 100.0f;
        float center_x = jogador_x_cfg;
        float bx = center_x - bar_w / 2.0f;
        float by = jogador_y_cfg - pj_h / 2.0f - bar_h - bar_gap;
        al_draw_filled_rectangle(bx, by, bx + bar_w, by + bar_h, al_map_rgb(50, 50, 50));
        al_draw_filled_rectangle(bx, by, bx + bar_w * p, by + bar_h, al_map_rgb(COR_VIDA_ALTA_R, COR_VIDA_ALTA_G, COR_VIDA_ALTA_B));
        al_draw_rectangle(bx, by, bx + bar_w, by + bar_h, al_map_rgb(COR_TEXTO_BRANCO_R, COR_TEXTO_BRANCO_G, COR_TEXTO_BRANCO_B), 1.0f);
    }

    // Nome
    al_draw_text(fonte, al_map_rgb(COR_TEXTO_BRANCO_R, COR_TEXTO_BRANCO_G, COR_TEXTO_BRANCO_B),
                 largura / 2.0f, 20.0f, ALLEGRO_ALIGN_CENTRE,
                 oponente->nome ? oponente->nome : "Oponente");

    // Caixa de opções
    float caixa_x = (float)largura - 300.0f;
    float caixa_y = (float)altura - 125.0f;
    float caixa_w = 0.0f, caixa_h = 0.0f;
    if (recursos && recursos->caixa_texto) {
        float bw = (float)al_get_bitmap_width(recursos->caixa_texto);
        float bh = (float)al_get_bitmap_height(recursos->caixa_texto);
        float escala_box = BATALHA_CAIXA_ESCALA;
        caixa_w = bw * escala_box;
        caixa_h = bh * escala_box;
        if (tem_oponente) {
            caixa_x = op_x + op_w / 2.0f - caixa_w / 2.0f;
            caixa_y = op_y + op_h + BATALHA_CAIXA_OFFSET_Y;
            if (caixa_x < BATALHA_CAIXA_MIN_MARGIN_X) caixa_x = BATALHA_CAIXA_MIN_MARGIN_X;
            if (caixa_x + caixa_w > largura - BATALHA_CAIXA_MIN_MARGIN_X) 
                caixa_x = largura - caixa_w - BATALHA_CAIXA_MIN_MARGIN_X;
            if (caixa_y + caixa_h > altura - BATALHA_CAIXA_MIN_MARGIN_Y) 
                caixa_y = altura - caixa_h - BATALHA_CAIXA_MIN_MARGIN_Y;
        }
        al_draw_scaled_bitmap(recursos->caixa_texto, 0, 0, bw, bh,
                              caixa_x, caixa_y, caixa_w, caixa_h, 0);
    }

    // Opções
    float opt_x = caixa_x + (caixa_w > 0 ? caixa_w / 2.0f : 110.0f) - 30.0f;
    float opt_y = caixa_y + 25.0f;
    
    if (oponente->tipo == BATALHA_ANIMAL && oponente->animal) {
        const char* opcoes[] = { "Alimentar", "Estudar", "Correr" };
        for (int i = 0; i < 3; i++) {
            ALLEGRO_COLOR cor;
            if (i == opcao) cor = al_map_rgb(COR_TEXTO_CIANO_R, COR_TEXTO_CIANO_G, COR_TEXTO_CIANO_B);
            else if (i == 1 && !oponente->animal->domado) cor = al_map_rgb(120, 0, 0);
            else cor = al_map_rgb(COR_TEXTO_PRETO_R, COR_TEXTO_PRETO_G, COR_TEXTO_PRETO_B);
            al_draw_text(fonte, cor, opt_x, opt_y + i * 22.0f, ALLEGRO_ALIGN_CENTRE, opcoes[i]);
        }
        
        if (!oponente->animal->domado) {
            al_draw_text(fonte, al_map_rgb(COR_TEXTO_AMARELO_R, COR_TEXTO_AMARELO_G, COR_TEXTO_AMARELO_B), opt_x, caixa_y - 10.0f, 
                        ALLEGRO_ALIGN_CENTRE, "Dome antes de estudar");
        } else if (!oponente->animal->estudado) {
            al_draw_text(fonte, al_map_rgb(COR_TEXTO_VERDE_CLARO_R, COR_TEXTO_VERDE_CLARO_G, COR_TEXTO_VERDE_CLARO_B), opt_x, caixa_y - 10.0f, 
                        ALLEGRO_ALIGN_CENTRE, "Estude para concluir");
        }
    } else if (oponente->tipo == BATALHA_CACADOR) {
        const char* opcoes[] = { "Atacar", "Defender", "Fugir" };
        for (int i = 0; i < 3; i++) {
            ALLEGRO_COLOR cor = !turno_jogador ? al_map_rgb(COR_TEXTO_CINZA_R, COR_TEXTO_CINZA_G, COR_TEXTO_CINZA_B) :
                (i == opcao ? al_map_rgb(COR_TEXTO_CIANO_R, COR_TEXTO_CIANO_G, COR_TEXTO_CIANO_B) : al_map_rgb(COR_TEXTO_PRETO_R, COR_TEXTO_PRETO_G, COR_TEXTO_PRETO_B));
            al_draw_text(fonte, cor, opt_x, opt_y + i * 22.0f, ALLEGRO_ALIGN_CENTRE, opcoes[i]);
        }
        
        if (oponente->cacador && oponente->cacador->derrotado)
            al_draw_text(fonte, al_map_rgb(COR_TEXTO_VERMELHO_R, COR_TEXTO_VERMELHO_G, COR_TEXTO_VERMELHO_B), opt_x, caixa_y - 10.0f, 
                        ALLEGRO_ALIGN_CENTRE, "Derrotado");
        else if (!turno_jogador)
            al_draw_text(fonte, al_map_rgb(COR_TEXTO_OURO_R, COR_TEXTO_OURO_G, COR_TEXTO_OURO_B), opt_x, caixa_y - 10.0f, 
                        ALLEGRO_ALIGN_CENTRE, "Aguardando");
        else
            al_draw_text(fonte, al_map_rgb(COR_TEXTO_VERDE_CLARO_R, COR_TEXTO_VERDE_CLARO_G, COR_TEXTO_VERDE_CLARO_B), opt_x, caixa_y - 10.0f, 
                        ALLEGRO_ALIGN_CENTRE, "Seu turno");
    }

    al_flip_display();
}

// ========== BATALHA UNIFICADA ==========

void iniciar_batalha_unificada(ALLEGRO_FONT* fonte, OponenteBatalha* oponente,
                              ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_DISPLAY* display,
                              entidade* jogador, Bestiario* bestiario) {
    if (!oponente || !jogador) return;

    bool batalhando = true;
    int opcao = 0;
    int vida_jogador = VIDA_MAXIMA_JOGADOR;
    bool turno_jogador = (oponente->tipo == BATALHA_CACADOR);

    const char* caminho_caixa = PATH_CAIXA_TEXTO;
    const char* caminho_jog = PATH_JOGADOR_IDLE_DIR;
    const char* caminho_fundo = oponente->caminho_fundo ? oponente->caminho_fundo : PATH_ESTRUTURA_SELVA;
    
    char caminho_sprite_oponente[256];
    if (oponente->tipo == BATALHA_ANIMAL) {
        snprintf(caminho_sprite_oponente, sizeof(caminho_sprite_oponente), 
                "assets/img/animais/%s.png", oponente->nome);
        // Converter para lowercase e substituir espaços, hífens e outros caracteres especiais por underscores
        for (size_t i = 0; caminho_sprite_oponente[i]; i++) {
            if (caminho_sprite_oponente[i] >= 'A' && caminho_sprite_oponente[i] <= 'Z')
                caminho_sprite_oponente[i] += ('a' - 'A');
            // Substituir espaços, hífens e outros caracteres especiais por underscore
            if (caminho_sprite_oponente[i] == ' ' || 
                caminho_sprite_oponente[i] == '-' ||
                caminho_sprite_oponente[i] == '\xC3') // Caractere especial UTF-8 (ã, á, etc)
                caminho_sprite_oponente[i] = '_';
        }
        printf("[DEBUG] Caminho sprite animal gerado: %s\n", caminho_sprite_oponente);
    } else {
        snprintf(caminho_sprite_oponente, sizeof(caminho_sprite_oponente), 
                "%s", oponente->cacador->dados_batalha.caminho_sprite);
    }

    RecursosBatalha* recursos = carregar_recursos_batalha_unificada(
        caminho_fundo, caminho_caixa, caminho_sprite_oponente, caminho_jog);

    // Variáveis específicas para animais
    int total_frases = 0;
    const char** frases_disponiveis = NULL;
    bool* frases_exibidas = NULL;
    
    if (oponente->tipo == BATALHA_ANIMAL) {
        frases_disponiveis = obter_frases_animal(oponente->nome, &total_frases);
        if (total_frases > 0) frases_exibidas = (bool*)calloc((size_t)total_frases, sizeof(bool));
    }

    desenhar_batalha_unificada(fonte, opcao, display, oponente, jogador, recursos, vida_jogador, turno_jogador);

    while (batalhando) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_TIMER) continue;

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (oponente->tipo == BATALHA_ANIMAL && oponente->animal) {
                switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_W:
                case ALLEGRO_KEY_UP:
                    do { opcao = (opcao + 2) % 3; } while (opcao == 1 && !oponente->animal->domado);
                    break;
                case ALLEGRO_KEY_S:
                case ALLEGRO_KEY_DOWN:
                    do { opcao = (opcao + 1) % 3; } while (opcao == 1 && !oponente->animal->domado);
                    break;
                case ALLEGRO_KEY_ENTER:
                case ALLEGRO_KEY_SPACE:
                    if (opcao == 0) {
                        oponente->animal->nivel_alimentacao += 100;
                        if (oponente->animal->nivel_alimentacao >= 100) {
                            oponente->animal->nivel_alimentacao = 100;
                            oponente->animal->alimentado = true;
                            oponente->animal->domado = true;
                            opcao = 1;
                        }
                        oponente->vida_atual = oponente->animal->nivel_alimentacao;
                    } else if (opcao == 1) {
                        if (!oponente->animal->domado) break;
                        if (total_frases > 0 && frases_exibidas && frases_disponiveis) {
                            int restantes = 0;
                            for (int i = 0; i < total_frases; i++)
                                if (!frases_exibidas[i]) restantes++;
                            if (restantes > 0) {
                                int idx;
                                do { idx = rand() % total_frases; } while (frases_exibidas[idx]);
                                mostrar_popup_estudo(fonte, display, event_queue, frases_disponiveis[idx]);
                                frases_exibidas[idx] = true;
                            } else {
                                for (int i = 0; i < total_frases; i++) frases_exibidas[i] = false;
                                int idx = rand() % total_frases;
                                mostrar_popup_estudo(fonte, display, event_queue, frases_disponiveis[idx]);
                                frases_exibidas[idx] = true;
                            }
                        }
                        oponente->animal->experiencia += 15;
                        if (oponente->animal->experiencia >= 100) {
                            oponente->animal->experiencia = 100;
                            oponente->animal->estudado = true;
                            if (bestiario) desbloquear_entrada_bestiario(bestiario, oponente->animal->nome);
                            batalhando = false;
                        }
                    } else if (opcao == 2) {
                        batalhando = false;
                    }
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    batalhando = false;
                    break;
                }
            } else if (oponente->tipo == BATALHA_CACADOR && oponente->cacador && turno_jogador) {
                switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_W:
                case ALLEGRO_KEY_UP:
                    opcao = (opcao + 2) % 3;
                    break;
                case ALLEGRO_KEY_S:
                case ALLEGRO_KEY_DOWN:
                    opcao = (opcao + 1) % 3;
                    break;
                case ALLEGRO_KEY_ENTER:
                case ALLEGRO_KEY_SPACE:
                    switch (opcao) {
                    case 0: {
                        int dano = 200 + (rand() % 10 - 5);
                        if (dano < 0) dano = 0;
                        oponente->cacador->vida -= dano;
                        oponente->vida_atual = oponente->cacador->vida;
                        turno_jogador = false;
                        desenhar_batalha_unificada(fonte, opcao, display, oponente, jogador, recursos, vida_jogador, turno_jogador);
                        al_rest(1.0);
                        if (oponente->cacador->vida > 0) {
                            vida_jogador -= oponente->cacador->dano_ataque;
                            al_rest(0.7);
                        }
                        turno_jogador = true;
                        break;
                    }
                    case 1:
                        turno_jogador = false;
                        desenhar_batalha_unificada(fonte, opcao, display, oponente, jogador, recursos, vida_jogador, turno_jogador);
                        al_rest(1.0);
                        vida_jogador -= oponente->cacador->dano_ataque / 2;
                        al_rest(0.5);
                        turno_jogador = true;
                        break;
                    case 2:
                        batalhando = false;
                        break;
                    }
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    batalhando = false;
                    break;
                }
                
                if (oponente->cacador->vida <= 0 || vida_jogador <= 0) {
                    batalhando = false;
                }
            }
            
            desenhar_batalha_unificada(fonte, opcao, display, oponente, jogador, recursos, vida_jogador, turno_jogador);
        }
    }

    // Resultado para caçadores
    if (oponente->tipo == BATALHA_CACADOR && oponente->cacador) {
        if (oponente->cacador->vida <= 0) {
            oponente->cacador->derrotado = true;
            oponente->cacador->ativo = false;
            al_clear_to_color(al_map_rgb(COR_FUNDO_VITORIA_R, COR_FUNDO_VITORIA_G, COR_FUNDO_VITORIA_B));
            al_draw_text(fonte, al_map_rgb(COR_TEXTO_AMARELO_R, COR_TEXTO_AMARELO_G, COR_TEXTO_AMARELO_B), 
                        POS_TEXTO_CENTRO_X, POS_TEXTO_DERROTA_TITULO_Y, ALLEGRO_ALIGN_CENTRE, MSG_VOCE_VENCEU);
            al_draw_text(fonte, al_map_rgb(COR_TEXTO_CINZA_CLARO_R, COR_TEXTO_CINZA_CLARO_G, COR_TEXTO_CINZA_CLARO_B), 
                        POS_TEXTO_CENTRO_X, POS_TEXTO_DERROTA_DESC_Y, ALLEGRO_ALIGN_CENTRE, MSG_PRESSIONE_ENTER);
            al_flip_display();
            while (true) {
                ALLEGRO_EVENT ev2;
                al_wait_for_event(event_queue, &ev2);
                if (ev2.type == ALLEGRO_EVENT_KEY_DOWN && ev2.keyboard.keycode == ALLEGRO_KEY_ENTER) break;
                if (ev2.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;
            }
        } else if (vida_jogador <= 0) {
            al_clear_to_color(al_map_rgb(COR_FUNDO_DERROTA_R, COR_FUNDO_DERROTA_G, COR_FUNDO_DERROTA_B));
            al_draw_text(fonte, al_map_rgb(COR_TEXTO_VERMELHO_R, COR_TEXTO_VERMELHO_G, COR_TEXTO_VERMELHO_B), 
                        POS_TEXTO_CENTRO_X, POS_TEXTO_DERROTA_TITULO_Y, ALLEGRO_ALIGN_CENTRE, MSG_VOCE_DERROTADO);
            al_draw_text(fonte, al_map_rgb(COR_TEXTO_CINZA_CLARO_R, COR_TEXTO_CINZA_CLARO_G, COR_TEXTO_CINZA_CLARO_B), 
                        POS_TEXTO_CENTRO_X, POS_TEXTO_DERROTA_DESC_Y, ALLEGRO_ALIGN_CENTRE, MSG_PRESSIONE_ENTER);
            al_flip_display();
            while (true) {
                ALLEGRO_EVENT ev2;
                al_wait_for_event(event_queue, &ev2);
                if (ev2.type == ALLEGRO_EVENT_KEY_DOWN && ev2.keyboard.keycode == ALLEGRO_KEY_ENTER) break;
                if (ev2.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;
            }
        }
        oponente->cacador->cooldown_batalha = COOLDOWN_BATALHA_CACADOR;
    }

    if (frases_exibidas) free(frases_exibidas);
    destruir_recursos_batalha(recursos);
}

// ========== FUNÇÕES LEGADAS (mantidas para compatibilidade) ==========

RecursosBatalha* carregar_recursos_batalha(const char* caminho_fundo, const char* caminho_caixa_texto, 
                                           const char* caminho_sprite_animal, const char* caminho_sprite_jogador) {
    RecursosBatalha* recursos = (RecursosBatalha*)malloc(sizeof(RecursosBatalha));
    if (!recursos) {
        fprintf(stderr, "Erro ao alocar memoria para Recursos Batalha\n");
        return NULL;
    }
    recursos->fundo_batalha   = al_load_bitmap(caminho_fundo);
    recursos->caixa_texto     = al_load_bitmap(caminho_caixa_texto);
    recursos->sprite_animal   = al_load_bitmap(caminho_sprite_animal);
    recursos->sprite_jogador  = al_load_bitmap(caminho_sprite_jogador);
    recursos->sprite_cacador  = NULL;
    return recursos;
}

RecursosBatalha* carregar_recursos_batalha_cacador(const char* caminho_fundo, const char* caminho_caixa_texto,
                                                   const char* caminho_sprite_cacador, const char* caminho_sprite_jogador) {
    RecursosBatalha* r = (RecursosBatalha*)malloc(sizeof(RecursosBatalha));
    if (!r) {
        fprintf(stderr, "Erro ao alocar memoria para Recursos Batalha Cacador\n");
        return NULL;
    }
    r->fundo_batalha  = al_load_bitmap(caminho_fundo);
    r->caixa_texto    = al_load_bitmap(caminho_caixa_texto);
    r->sprite_cacador = al_load_bitmap(caminho_sprite_cacador);
    r->sprite_jogador = al_load_bitmap(caminho_sprite_jogador);
    r->sprite_animal  = NULL;
    return r;
}

RecursosBatalha* carregar_recursos_batalha_unificada(const char* caminho_fundo, const char* caminho_caixa_texto,
                                                     const char* caminho_sprite_oponente, const char* caminho_sprite_jogador) {
    RecursosBatalha* r = (RecursosBatalha*)malloc(sizeof(RecursosBatalha));
    if (!r) {
        fprintf(stderr, "Erro ao alocar memoria para Recursos Batalha Unificada\n");
        return NULL;
    }
    r->fundo_batalha  = al_load_bitmap(caminho_fundo);
    r->caixa_texto    = al_load_bitmap(caminho_caixa_texto);
    r->sprite_animal  = al_load_bitmap(caminho_sprite_oponente);
    r->sprite_cacador = al_load_bitmap(caminho_sprite_oponente);
    r->sprite_jogador = al_load_bitmap(caminho_sprite_jogador);
    return r;
}

void destruir_recursos_batalha(RecursosBatalha* r) {
    if (!r) return;
    if (r->fundo_batalha)  al_destroy_bitmap(r->fundo_batalha);
    if (r->caixa_texto)    al_destroy_bitmap(r->caixa_texto);
    if (r->sprite_animal)  al_destroy_bitmap(r->sprite_animal);
    if (r->sprite_jogador) al_destroy_bitmap(r->sprite_jogador);
    if (r->sprite_cacador) al_destroy_bitmap(r->sprite_cacador);
    free(r);
}

// Funções legadas redirecionam para a nova implementação
void desenhar_menu_batalha(ALLEGRO_FONT* fonte, int opcao, ALLEGRO_DISPLAY* display, 
                          Animal* animal, RecursosBatalha* recursos) {
    OponenteBatalha* oponente = criar_oponente_animal(animal);
    if (!oponente) return;
    desenhar_batalha_unificada(fonte, opcao, display, oponente, NULL, recursos, 100, false);
    destruir_oponente_batalha(oponente);
}

void iniciar_batalha(ALLEGRO_FONT* fonte, Animal* animal, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_DISPLAY* display) {
    OponenteBatalha* oponente = criar_oponente_animal(animal);
    if (!oponente) return;
    
    entidade jogador_dummy = {0};
    iniciar_batalha_unificada(fonte, oponente, event_queue, display, &jogador_dummy, NULL);
    destruir_oponente_batalha(oponente);
}

void iniciar_batalha_com_bestiario(ALLEGRO_FONT* fonte, Animal* animal,
                                   ALLEGRO_EVENT_QUEUE* event_queue,
                                   ALLEGRO_DISPLAY* display,
                                   Bestiario* bestiario) {
    OponenteBatalha* oponente = criar_oponente_animal(animal);
    if (!oponente) return;
    
    entidade jogador_dummy = {0};
    iniciar_batalha_unificada(fonte, oponente, event_queue, display, &jogador_dummy, bestiario);
    destruir_oponente_batalha(oponente);
}

void desenhar_menu_batalha_cacador(ALLEGRO_FONT* fonte, int opcao, ALLEGRO_DISPLAY* display,
                                   Cacador* cacador, entidade* jogador, RecursosBatalha* recursos,
                                   int vida_jogador, bool turno_jogador) {
    OponenteBatalha* oponente = criar_oponente_cacador(cacador);
    if (!oponente) return;
    desenhar_batalha_unificada(fonte, opcao, display, oponente, jogador, recursos, vida_jogador, turno_jogador);
    destruir_oponente_batalha(oponente);
}

void iniciar_batalha_cacador_visual(ALLEGRO_FONT* fonte, Cacador* cacador,
                                    ALLEGRO_EVENT_QUEUE* event_queue,
                                    ALLEGRO_DISPLAY* display, entidade* jogador) {
    OponenteBatalha* oponente = criar_oponente_cacador(cacador);
    if (!oponente) return;
    iniciar_batalha_unificada(fonte, oponente, event_queue, display, jogador, NULL);
    destruir_oponente_batalha(oponente);
}