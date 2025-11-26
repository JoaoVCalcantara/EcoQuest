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
#include "config_jogo.h"       // NOVO: Configurações do jogo
#include "config_ui.h"         // NOVO: Configurações de UI
#include "config_cacador.h"

/* ==== Conteúdo reinserido: frases e funções de popup/lookup de frases ==== */
/* Frases de estudo */
static const char* frases_lobo_guara[] = {
    "Pernas Longas e Esguias - Adaptacao Evolutiva: A caracteristica mais marcante do lobo-guara, suas pernas longas e finas, eh uma adaptacao crucial para a locomocao no capim alto do Cerrado e em areas abertas. Essa estrutura permite que ele enxergue acima da vegetacao rasteira e persiga suas presas de forma eficiente nesse tipo de terreno. Essa caracteristica o diferencia dos lobos e raposas de florestas mais densas.",
    "Dieta Onivora (Especializacao em Frutos) - Adaptacao Evolutiva: Diferente da maioria dos grandes canideos, que sao predominantemente carnivoros, o lobo-guara desenvolveu uma dieta onivora, com grande consumo de frutos (chegando a 90% da dieta em certas epocas). Essa dieta e uma resposta a disponibilidade sazonal de recursos no Cerrado. Papel Ecologico: Essa evolucao alimentar fez dele um dispersor de sementes vital para o bioma, especialmente da lobeira (Solanum lycocarpum), estabelecendo uma relacao de mutualismo.",
    "Orelhas Grandes - Adaptacao Evolutiva: Suas orelhas grandes e eretas sao essenciais para detectar presas noturnas (como pequenos roedores e insetos) e para a comunicacao em um ambiente de vegetacao aberta, onde o som pode se propagar melhor do que em uma floresta densa.",
    "Pelagem e Crina - Adaptacao Evolutiva: A pelagem vermelho-dourada ou alaranjada, com a crina de pelos pretos na nuca, ajuda na camuflagem em meio a vegetacao seca do Cerrado. A crina pode ser erizada, o que pode ter evoluido como uma forma de parecer maior para intimidar rivais ou predadores, ja que eh um animal geralmente solitario e timido.",
    "Comportamento Solitario - Adaptacao Evolutiva: A vida solitaria e territorialista, formando casais apenas na epoca de reproducao, eh uma adaptacao a um ambiente onde os recursos alimentares (frutos e pequenas presas) sao mais dispersos, nao favorecendo a captura em matilhas tipica dos lobos verdadeiros."
};

static const char* frases_jacare[] = {
    "Corpo Hidrodinamico: A forma do corpo e a longa cauda musculosa evoluiram para permitir locomocao e manobrabilidade eficientes na agua. Adaptacoes Aquaticas: Diferente de seus ancestrais, que eram terrestres e podiam ate ser bipedes, os jacares desenvolveram membros mais laterais (postura rastejante) e um focinho mais comprido, adaptacoes para a vida em rios, lagos e pantanos.",
    "Olhos e Narinas no Topo da Cabeca: Uma adaptacao crucial para a emboscada. Isso permite que o jacare permaneca quase totalmente submerso, observando e respirando, enquanto espera a presa.",
    "Pele Resistente: A pele espessa e com placas osseas (osteodermes) oferece protecao contra predadores e ajuda na retencao de agua, uma adaptacao a vida em ambientes sazonais.",
    "Ovo com Casca Rigida (Amniota): Uma das aquisicoes evolutivas mais importantes dos repteis foi o ovo com casca, que protege o embriao da desseccao, permitindo a reproducao em terra firme, longe da dependencia direta da agua para os ovos.",
    "Eficiencia Termica (Ectotermia): Por serem ectotermicos (dependem do ambiente para regulator a temperatura corporal), eles desenvolveram comportamentos como tomar sol nas margens para aquecer-se, o que e energeticamente eficiente e uma adaptacao a climas quentes.",
    "Comportamento Maternal: Uma caracteristica evolutiva notavel e o cuidado parental. As femeas protegem os ninhos e ate transportam os filhotes recem-nascidos para a agua, defendendo-os de predadores por um longo periodo."
};

static const char* frases_boto[] = {
    "Boto Coloracao Rosada: Os botos nao nascem cor-de-rosa; eles nascem cinzentos e a coloracao surge com a idade. A tonalidade rosa e influenciada pela idade, sexo e, principalmente, por cicatrizes de arranhoes em troncos ou disputas entre machos. Isso pode ser uma forma de selecao sexual, onde os machos mais rosados (mais marcados por lutas) sao mais desejaveis.",
    "Flexibilidade Corporal e Pescoco Articulado: Ao contrario dos golfinhos marinhos, cujas vertebras cervicais sao fundidas, as do boto nao sao. Adaptacao Evolutiva: Essa flexibilidade permite que ele vire a cabeca em um angulo de ate 90 graus, essencial para manobrar e cacar em areas alagadas, igapos e entre troncos de arvores, algo inviavel em mar aberto.",
    "Focinho Longo e Dentes Adaptados: O boto possui um focinho mais longo e fino do que a maioria dos golfinhos marinhos, com dentes diferentes na frente (para agarrar) e no fundo da boca (para triturar). Adaptacao Evolutiva: Essa specializacao permite a captura de uma dieta variada, incluindo peixes, tartarugas e caranguejos, adaptando-se aos recursos disponiveis nos rios.",
    "Visao Reduzida e Ecolocalizacao Aprimorada: Embora ainda funcional, a visao do boto e menos desenvolvida do que a de golfinhos marinhos, o que faz sentido em aguas turvas e escuras da Amazonia. Adaptacao Evolutiva: Em compensacao, seu sistema de ecolocalizacao e extremamente sofisticado, permitindo-lhe navegar e cacar com precisao em condicoes de pouca ou nenhuma visibilidade.",
    "Ausencia de Nadadeira Dorsal Proeminente: Em vez de uma barbatana dorsal alta e rigida, o boto tem apenas uma pequena crista dorsal. Adaptacao Evolutiva: Isso facilita a natacao em aguas rasas e a passagem por baixo de galhos e vegetacao densa durante as cheias sazonais da Amazonia.",
    "Cerebro Grande e Complexo: O boto-cor-de-rosa tem o maior cerebro entre os golfinhos de agua doce e, proporcionalmente, utiliza mais capacidade cerebral que os humanos."
};

static const char* frases_onca[] = {
    "Mandibula e Forca de Mordida Excepcionais: Esta e talvez a sua caracteristica evolutiva mais distintiva. A onca-pintada desenvolveu uma cabeca grande e robusta, com musculos mandibulares e dentes caninos extremamente fortes, permitindo-lhe ter uma das mordidas mais potentes entre todos os felinos. Vantagem Evolutiva: Essa forca permite a onca abater presas grandes e, crucialmente, perfurar cranios de mamiferos ou cascos de repites (como tartarugas e jacares), um metodo de caca raro entre felinos, que geralmente matam por estrangulamento.",
    "Constituicao Fisica Musculosa e Membros Curtos: Em contraste com a estrutura mais esguia de guepardos ou leopardos, a onca-pintada tem um corpo atarracado e musculoso, com membros curtos e macicos. Vantagem Evolutiva: Essa estrutura fisica nao e para velocidade em corridas longas, mas sim para forca, agilidade em curtas distancias e a capacidade de nadar e escalar arvores, permitindo-lhe emboscar presas em ambientes densos e proximos a agua.",
    "Padrao de Pelagem (Rosetas): Sua pelagem de cor amarelo-dourada a castanho-avermelhada e coberta por rosetas complexas (manchas circulares com pontos pretos dentro). Vantagem Evolutiva: Este padrao fornece uma camuflagem excellente na vegetacao densa e na luz filtrada de florestas e pantanos, facilitando a caca por emboscada.",
    "Habilidades Aquaticas: Diferente da maioria dos felinos que evitam a agua, a onca-pintada e uma excelente nadadora, uma adaptacao vital para a vida em biomas com muitos corpos d'agua, como a Amazonia e o Pantanal, onde pode cacar presas aquaticas e semi-aquaticas."
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
        nome_normalizado[j++] = (nome_lower[i] == ' ') ? '_' : nome_lower[i];
    }
    nome_normalizado[j] = '\0';

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


/* NOVA: Função unificada para obter configuração de sprite (usa macros em config_ui.h) */
static ConfigSpriteEntidade obter_config_entidade(const char* nome) {
    ConfigSpriteEntidade padrao = { "desconhecido", 280.0f, 250.0f, 0.25f };
    if (!nome) return padrao;

    /* compara com macros de nomes existentes (assume que NOME_* estão definidos) */
    if (strcmp(nome, NOME_ANIMAL_ONCA) == 0) {
        ConfigSpriteEntidade r = { NOME_ANIMAL_ONCA, ENT_ONCA_X, ENT_ONCA_Y, ENT_ONCA_ESCALA }; return r;
    }
    if (strcmp(nome, NOME_ANIMAL_JACARE) == 0) {
        ConfigSpriteEntidade r = { NOME_ANIMAL_JACARE, ENT_JACARE_X, ENT_JACARE_Y, ENT_JACARE_ESCALA }; return r;
    }
    if (strcmp(nome, NOME_ANIMAL_BOTO) == 0) {
        ConfigSpriteEntidade r = { NOME_ANIMAL_BOTO, ENT_BOTO_X, ENT_BOTO_Y, ENT_BOTO_ESCALA }; return r;
    }
    if (strcmp(nome, NOME_ANIMAL_LOBO) == 0) {
        ConfigSpriteEntidade r = { NOME_ANIMAL_LOBO, ENT_LOBO_X, ENT_LOBO_Y, ENT_LOBO_ESCALA }; return r;
    }
    /* lidar com possível literal alternativo "lobo guara" */
    if (strcmp(nome, "lobo guara") == 0) {
        ConfigSpriteEntidade r = { "lobo guara", ENT_LOBO_X, ENT_LOBO_Y, ENT_LOBO_ESCALA }; return r;
    }

    /* Caçadores (assume NOME_CACADOR_* macros existem) */
    if (strcmp(nome, NOME_CACADOR_SELVA) == 0) {
        ConfigSpriteEntidade r = { NOME_CACADOR_SELVA, ENT_CACADOR_SELVA_X, ENT_CACADOR_SELVA_Y, ENT_CACADOR_SELVA_ESCALA }; return r;
    }
    if (strcmp(nome, NOME_CACADOR_PANTANO) == 0) {
        ConfigSpriteEntidade r = { NOME_CACADOR_PANTANO, ENT_CACADOR_PANTANO_X, ENT_CACADOR_PANTANO_Y, ENT_CACADOR_PANTANO_ESCALA }; return r;
    }
    if (strcmp(nome, NOME_CACADOR_LAGO) == 0) {
        ConfigSpriteEntidade r = { NOME_CACADOR_LAGO, ENT_CACADOR_LAGO_X, ENT_CACADOR_LAGO_Y, ENT_CACADOR_LAGO_ESCALA }; return r;
    }
    if (strcmp(nome, NOME_CACADOR_CERRADO) == 0) {
        ConfigSpriteEntidade r = { NOME_CACADOR_CERRADO, ENT_CACADOR_CERRADO_X, ENT_CACADOR_CERRADO_Y, ENT_CACADOR_CERRADO_ESCALA }; return r;
    }
    if (strcmp(nome, NOME_CACADOR_CHEFE) == 0) {
        ConfigSpriteEntidade r = { NOME_CACADOR_CHEFE, ENT_CACADOR_CHEFE_X, ENT_CACADOR_CHEFE_Y, ENT_CACADOR_CHEFE_ESCALA }; return r;
    }

    /* Fallback: retorna padrao */
    return padrao;
}

/* NOVA: obter configuração do jogador para a tela de batalha
   - Se o nome corresponder a um caçador, usa JOGADOR_CACADOR_* macros
   - Se o nome corresponder a um animal, usa JOGADOR_* macros
   - Se os valores X/Y forem <= 0 usa os ratios (BATALHA_JOGADOR_*) */
static ConfigSpriteJogador obter_config_sprite_jogador(const char* nome_animal) {
    ConfigSpriteJogador padrao = { "desconhecido", 0.0f, 0.0f, BATALHA_JOGADOR_ESCALA };
    if (!nome_animal) return padrao;

    /* Caçadores */
    if (strcmp(nome_animal, NOME_CACADOR_SELVA) == 0) {
        ConfigSpriteJogador r = { NOME_CACADOR_SELVA, JOGADOR_CACADOR_SELVA_X, JOGADOR_CACADOR_SELVA_Y, JOGADOR_CACADOR_SELVA_ESCALA };
        return r;
    }
    if (strcmp(nome_animal, NOME_CACADOR_PANTANO) == 0) {
        ConfigSpriteJogador r = { NOME_CACADOR_PANTANO, JOGADOR_CACADOR_PANTANO_X, JOGADOR_CACADOR_PANTANO_Y, JOGADOR_CACADOR_PANTANO_ESCALA };
        return r;
    }
    if (strcmp(nome_animal, NOME_CACADOR_LAGO) == 0) {
        ConfigSpriteJogador r = { NOME_CACADOR_LAGO, JOGADOR_CACADOR_LAGO_X, JOGADOR_CACADOR_LAGO_Y, JOGADOR_CACADOR_LAGO_ESCALA };
        return r;
    }
    if (strcmp(nome_animal, NOME_CACADOR_CERRADO) == 0) {
        ConfigSpriteJogador r = { NOME_CACADOR_CERRADO, JOGADOR_CACADOR_CERRADO_X, JOGADOR_CACADOR_CERRADO_Y, JOGADOR_CACADOR_CERRADO_ESCALA };
        return r;
    }
    if (strcmp(nome_animal, NOME_CACADOR_CHEFE) == 0) {
        ConfigSpriteJogador r = { NOME_CACADOR_CHEFE, JOGADOR_CACADOR_CHEFE_X, JOGADOR_CACADOR_CHEFE_Y, JOGADOR_CACADOR_CHEFE_ESCALA };
        return r;
    }

    /* Animais */
    if (strcmp(nome_animal, NOME_ANIMAL_ONCA) == 0) {
        ConfigSpriteJogador r = { NOME_ANIMAL_ONCA, JOGADOR_ONCA_X, JOGADOR_ONCA_Y, JOGADOR_ONCA_ESCALA };
        return r;
    }
    if (strcmp(nome_animal, NOME_ANIMAL_JACARE) == 0) {
        ConfigSpriteJogador r = { NOME_ANIMAL_JACARE, JOGADOR_JACARE_X, JOGADOR_JACARE_Y, JOGADOR_JACARE_ESCALA };
        return r;
    }
    if (strcmp(nome_animal, NOME_ANIMAL_BOTO) == 0) {
        ConfigSpriteJogador r = { NOME_ANIMAL_BOTO, JOGADOR_BOTO_X, JOGADOR_BOTO_Y, JOGADOR_BOTO_ESCALA };
        return r;
    }
    if (strcmp(nome_animal, NOME_ANIMAL_LOBO) == 0) {
        ConfigSpriteJogador r = { NOME_ANIMAL_LOBO, JOGADOR_LOBO_X, JOGADOR_LOBO_Y, JOGADOR_LOBO_ESCALA };
        return r;
    }
    if (strcmp(nome_animal, "lobo guara") == 0 || strcmp(nome_animal, "lobo_guara") == 0) {
        ConfigSpriteJogador r = { "lobo guara", JOGADOR_LOBO_GUARA_X, JOGADOR_LOBO_GUARA_Y, JOGADOR_LOBO_GUARA_ESCALA };
        return r;
    }

    /* Fallback: usa ratios e escala padrão */
    ConfigSpriteJogador fallback = { nome_animal, -1.0f, -1.0f, BATALHA_JOGADOR_ESCALA };
    return fallback;
}

/* (o restante do arquivo permanece inalterado; funções que chamam as novas
   obter_config_* continuam funcionando — por exemplo: criar_oponente_* e
   desenhar_batalha_unificada.) */

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
    oponente->config_sprite = obter_config_entidade(animal->nome);
    
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
    oponente->config_sprite = obter_config_entidade(cacador->nome);
    
    return oponente;
}

void destruir_oponente_batalha(OponenteBatalha* oponente) {
    if (oponente) free(oponente);
}

/* ========== DESENHO UNIFICADO ========== */

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

    // Jogador (lado esquerdo) - usa o padrão de ConfigSpriteJogador se disponível
    bool tem_jogador = false;
    float jogador_x = 0.0f, jogador_y = 0.0f, pj_w = 0.0f, pj_h = 0.0f;
    if (recursos && recursos->sprite_jogador) {
        float jw = (float)al_get_bitmap_width(recursos->sprite_jogador);
        float jh = (float)al_get_bitmap_height(recursos->sprite_jogador);

        // Tenta obter configuração específica do oponente (se houver)
        ConfigSpriteJogador conf_jog = obter_config_sprite_jogador(oponente ? oponente->nome : NULL);

        // escala: prioridade -> configuração específica -> macro padrão
        float escala_jog = (conf_jog.jogador_escala > 0.0f) ? conf_jog.jogador_escala : BATALHA_JOGADOR_ESCALA;

        pj_w = jw * escala_jog;
        pj_h = jh * escala_jog;

        // posição: se houver valores explícitos na config do jogador (pixels), usa-os,
        // caso contrário usa ratios definidos em config_ui.h
        if (conf_jog.jogador_x > 0.0f || conf_jog.jogador_y > 0.0f) {
            jogador_x = conf_jog.jogador_x - pj_w / 2.0f;
            jogador_y = conf_jog.jogador_y - pj_h / 2.0f;
        } else {
            jogador_x = (largura * BATALHA_JOGADOR_X_RATIO) - pj_w / 2.0f;
            jogador_y = (altura  * BATALHA_JOGADOR_Y_RATIO) - pj_h / 2.0f;
        }

        tem_jogador = true;
        al_draw_scaled_bitmap(recursos->sprite_jogador, 0, 0, jw, jh,
                              jogador_x, jogador_y, pj_w, pj_h, 0);
    }

    // Oponente (lado direito)
    bool tem_oponente = false;
    float op_x = 0.0f, op_y = 0.0f, op_w = 0.0f, op_h = 0.0f;
    ALLEGRO_BITMAP* sprite_oponente = NULL;
    
    if (oponente->tipo == BATALHA_ANIMAL && recursos && recursos->sprite_animal) {
        sprite_oponente = recursos->sprite_animal;
    } else if (oponente->tipo == BATALHA_CACADOR && recursos && recursos->sprite_cacador) {
        sprite_oponente = recursos->sprite_cacador;
    }
    
    if (sprite_oponente) {
        ConfigSpriteEntidade conf = oponente->config_sprite;
        float ow = (float)al_get_bitmap_width(sprite_oponente);
        float oh = (float)al_get_bitmap_height(sprite_oponente);
        op_w = ow * conf.entidade_escala;
        op_h = oh * conf.entidade_escala;
        op_x = (largura - op_w) / 2.0f + conf.entidade_x;
        op_y = conf.entidade_y;
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
        float center_x = jogador_x + pj_w / 2.0f;
        float bx = center_x - bar_w / 2.0f;
        float by = jogador_y - bar_h - bar_gap;
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
        float escala_box = 0.3f;
        caixa_w = bw * escala_box;
        caixa_h = bh * escala_box;
        if (tem_oponente) {
            caixa_x = op_x + op_w / 2.0f - caixa_w / 2.0f;
            caixa_y = op_y + op_h + 10.0f;
            if (caixa_x < 0) caixa_x = 0;
            if (caixa_x + caixa_w > largura) caixa_x = largura - caixa_w;
            if (caixa_y + caixa_h > altura - 5.0f) caixa_y = altura - caixa_h - 5.0f;
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
        for (size_t i = 0; caminho_sprite_oponente[i]; i++) {
            if (caminho_sprite_oponente[i] >= 'A' && caminho_sprite_oponente[i] <= 'Z')
                caminho_sprite_oponente[i] += ('a' - 'A');
            if (caminho_sprite_oponente[i] == ' ')
                caminho_sprite_oponente[i] = '_';
        }
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
                        int dano = 100 + (rand() % 10 - 5);
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