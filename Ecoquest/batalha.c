#define _CRT_SECURE_NO_WARNINGS

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <string.h>
#include "batalha.h"
#include "bestiario.h"

// Estrutura para configurar posição individual de cada animal
typedef struct {
    const char* nome;
    float escala;
    float pos_x;
    float pos_y;
} ConfigAnimal;

// Configurações individuais para cada animal
static const ConfigAnimal configs_animais[] = {
    {"lobo_guara", 0.25f, 250.0f, 325.0f},
    {"lobo guara", 0.25f, 250.0f, 325.0f},
    {"jacare", 0.25f, 275.0f, 150.0f},
    {"boto", 0.25f, 280.0f, 320.0f},
    {"onca", 0.25f, 250.0f, 200.0f}
};

// Configurações de posição do sprite do jogador para cada animal
static const ConfigSpriteJogador configs_jogador[] = {
    {"lobo_guara", 100.0f, 150.0f, 0.5f},
    {"lobo guara", 100.0f, 150.0f, 0.5f},
    {"jacare", 125.0f, 150.0f, 0.5f},
    {"boto", 175.0f, 175.0f, 0.5f},
    {"onca", 175.0f, 175.0f, 0.5f}
};

// Arrays de frases informativas por animal
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
    "Mandibula e Forca de Mordida Excepcionais: Esta e talvez a sua caracteristica evolutiva mais distintiva. A onca-pintada desenvolveu uma cabeca grande e robusta, com musculos mandibulares e dentes caninos extremamente fortes, permitindo-lhe ter uma das mordidas mais potentes entre todos os felinos. Vantagem Evolutiva: Essa forca permite a onca abater presas grandes e, crucialmente, perfurar cranios de mamiferos ou cascos de repteis (como tartarugas e jacares), um metodo de caca raro entre felinos, que geralmente matam por estrangulamento.",
    "Constituicao Fisica Musculosa e Membros Curtos: Em contraste com a estrutura mais esguia de guepardos ou leopardos, a onca-pintada tem um corpo atarracado e musculoso, com membros curtos e macicos. Vantagem Evolutiva: Essa estrutura fisica nao e para velocidade em corridas longas, mas sim para forca, agilidade em curtas distancias e a capacidade de nadar e escalar arvores, permitindo-lhe emboscar presas em ambientes densos e proximos a agua.",
    "Padrao de Pelagem (Rosetas): Sua pelagem de cor amarelo-dourada a castanho-avermelhada e coberta por rosetas complexas (manchas circulares com pontos pretos dentro). Vantagem Evolutiva: Este padrao fornece uma camuflagem excelente na vegetacao densa e na luz filtrada de florestas e pantanos, facilitando a caca por emboscada.",
    "Habilidades Aquaticas: Diferente da maioria dos felinos que evitam a agua, a onca-pintada e uma excelente nadadora, uma adaptacao vital para a vida em biomas com muitos corpos d'agua, como a Amazonia e o Pantanal, onde pode cacar presas aquaticas e semi-aquaticas."
};

// Função auxiliar para obter frases por tipo de animal
static const char** obter_frases_animal(const char* nome_animal, int* total_frases) {
    if (!nome_animal || !total_frases) return NULL;

    char nome_lower[64];
    snprintf(nome_lower, sizeof(nome_lower), "%s", nome_animal);
    for (int i = 0; nome_lower[i]; i++) {
        if (nome_lower[i] >= 'A' && nome_lower[i] <= 'Z') {
            nome_lower[i] = nome_lower[i] + ('a' - 'A');
        }
    }

    // Remove espaços e substitui por underscore para normalização
    char nome_normalizado[64];
    int j = 0;
    for (int i = 0; nome_lower[i] && j < 63; i++) {
        if (nome_lower[i] == ' ') {
            nome_normalizado[j++] = '_';
        }
        else {
            nome_normalizado[j++] = nome_lower[i];
        }
    }
    nome_normalizado[j] = '\0';

    if (strcmp(nome_normalizado, "lobo_guara") == 0) {
        *total_frases = (int)(sizeof(frases_lobo_guara) / sizeof(frases_lobo_guara[0]));
        return frases_lobo_guara;
    }
    else if (strcmp(nome_lower, "jacare") == 0) {
        *total_frases = (int)(sizeof(frases_jacare) / sizeof(frases_jacare[0]));
        return frases_jacare;
    }
    else if (strcmp(nome_lower, "boto") == 0) {
        *total_frases = (int)(sizeof(frases_boto) / sizeof(frases_boto[0]));
        return frases_boto;
    }
    else if (strcmp(nome_lower, "onca") == 0) {
        *total_frases = (int)(sizeof(frases_onca) / sizeof(frases_onca[0]));
        return frases_onca;
    }

    *total_frases = 0;
    return NULL;
}

// Função para exibir popup informativo
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
    float line_height = al_get_font_line_height(fonte) + 4;

    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_filled_rectangle(0, 0, (float)largura, (float)altura, al_map_rgba(0, 0, 0, 180));

    al_draw_filled_rounded_rectangle(box_x, box_y, box_x + box_w, box_y + box_h, 10, 10, al_map_rgb(240, 240, 230));
    al_draw_rounded_rectangle(box_x, box_y, box_x + box_w, box_y + box_h, 10, 10, al_map_rgb(100, 70, 30), 3);

    al_draw_text(fonte, al_map_rgb(40, 20, 0), box_x + box_w / 2, box_y + 15, ALLEGRO_ALIGN_CENTRE, "=== INFORMACAO DO ANIMAL ===");

    char linha_atual[512] = "";
    char* palavra_ptr = buffer;

    while (*palavra_ptr != '\0') {
        while (*palavra_ptr == ' ') palavra_ptr++;
        if (*palavra_ptr == '\0') break;

        char* fim_palavra = palavra_ptr;
        while (*fim_palavra != '\0' && *fim_palavra != ' ') fim_palavra++;

        char palavra[256];
        size_t len_palavra = fim_palavra - palavra_ptr;
        if (len_palavra >= sizeof(palavra)) len_palavra = sizeof(palavra) - 1;
        strncpy(palavra, palavra_ptr, len_palavra);
        palavra[len_palavra] = '\0';

        char teste[512];
        if (strlen(linha_atual) == 0) {
            strncpy(teste, palavra, sizeof(teste) - 1);
        }
        else {
            snprintf(teste, sizeof(teste), "%s %s", linha_atual, palavra);
        }
        teste[sizeof(teste) - 1] = '\0';

        float w = al_get_text_width(fonte, teste);

        if (w > max_width && strlen(linha_atual) > 0) {
            al_draw_text(fonte, al_map_rgb(0, 0, 0), text_x, text_y, ALLEGRO_ALIGN_LEFT, linha_atual);
            text_y += line_height;
            strncpy(linha_atual, palavra, sizeof(linha_atual) - 1);
        }
        else {
            strncpy(linha_atual, teste, sizeof(linha_atual) - 1);
        }
        linha_atual[sizeof(linha_atual) - 1] = '\0';

        palavra_ptr = fim_palavra;
    }

    if (strlen(linha_atual) > 0) {
        al_draw_text(fonte, al_map_rgb(0, 0, 0), text_x, text_y, ALLEGRO_ALIGN_LEFT, linha_atual);
    }

    al_draw_text(fonte, al_map_rgb(100, 100, 100), box_x + box_w / 2, box_y + box_h - 30, ALLEGRO_ALIGN_CENTRE, "Pressione ENTER para continuar");

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

static ConfigAnimal obter_config_animal(const char* nome_animal) {
    ConfigAnimal padrao = { "desconhecido", 0.3f, 250.0f, 280.0f };

    if (!nome_animal) return padrao;

    char nome_lower[64];
    snprintf(nome_lower, sizeof(nome_lower), "%s", nome_animal);
    for (int i = 0; nome_lower[i]; i++) {
        if (nome_lower[i] >= 'A' && nome_lower[i] <= 'Z') {
            nome_lower[i] = nome_lower[i] + ('a' - 'A');
        }
    }

    // Remove espaços e substitui por underscore
    char nome_normalizado[64];
    int j = 0;
    for (int i = 0; nome_lower[i] && j < 63; i++) {
        if (nome_lower[i] == ' ') {
            nome_normalizado[j++] = '_';
        }
        else {
            nome_normalizado[j++] = nome_lower[i];
        }
    }
    nome_normalizado[j] = '\0';

    for (size_t i = 0; i < sizeof(configs_animais) / sizeof(ConfigAnimal); i++) {
        if (strcmp(configs_animais[i].nome, nome_normalizado) == 0 ||
            strcmp(configs_animais[i].nome, nome_lower) == 0) {
            return configs_animais[i];
        }
    }

    return padrao;
}

static ConfigSpriteJogador obter_config_sprite_jogador(const char* nome_animal) {
    ConfigSpriteJogador padrao = { "desconhecido", 100.0f, 350.0f, 0.15f };

    if (!nome_animal) return padrao;

    char nome_lower[64];
    snprintf(nome_lower, sizeof(nome_lower), "%s", nome_animal);
    for (int i = 0; nome_lower[i]; i++) {
        if (nome_lower[i] >= 'A' && nome_lower[i] <= 'Z') {
            nome_lower[i] = nome_lower[i] + ('a' - 'A');
        }
    }

    // Remove espaços e substitui por underscore
    char nome_normalizado[64];
    int j = 0;
    for (int i = 0; nome_lower[i] && j < 63; i++) {
        if (nome_lower[i] == ' ') {
            nome_normalizado[j++] = '_';
        }
        else {
            nome_normalizado[j++] = nome_lower[i];
        }
    }
    nome_normalizado[j] = '\0';

    for (size_t i = 0; i < sizeof(configs_jogador) / sizeof(ConfigSpriteJogador); i++) {
        if (strcmp(configs_jogador[i].nome_animal, nome_normalizado) == 0 ||
            strcmp(configs_jogador[i].nome_animal, nome_lower) == 0) {
            return configs_jogador[i];
        }
    }

    return padrao;
}

RecursosBatalha* carregar_recursos_batalha(const char* caminho_fundo, const char* caminho_caixa_texto, const char* caminho_sprite_animal, const char* caminho_sprite_jogador) {
    RecursosBatalha* recursos = (RecursosBatalha*)malloc(sizeof(RecursosBatalha));
    if (!recursos) {
        fprintf(stderr, "Erro ao alocar memoria para Recursos Batalha\n");
        return NULL;
    }

    recursos->fundo_batalha = al_load_bitmap(caminho_fundo);
    if (!recursos->fundo_batalha) {
        fprintf(stderr, "Erro ao carregar fundo de batalha: %s\n", caminho_fundo);
    }

    recursos->caixa_texto = al_load_bitmap(caminho_caixa_texto);
    if (!recursos->caixa_texto) {
        fprintf(stderr, "Erro ao carregar caixa de texto: %s\n", caminho_caixa_texto);
    }

    recursos->sprite_animal = al_load_bitmap(caminho_sprite_animal);
    if (!recursos->sprite_animal) {
        fprintf(stderr, "Erro ao carregar sprite do animal: %s\n", caminho_sprite_animal);
    }

    recursos->sprite_jogador = al_load_bitmap(caminho_sprite_jogador);
    if (!recursos->sprite_jogador) {
        fprintf(stderr, "Erro ao carregar sprite do jogador: %s\n", caminho_sprite_jogador);
    }

    return recursos;
}

void destruir_recursos_batalha(RecursosBatalha* recursos) {
    if (!recursos) return;

    if (recursos->fundo_batalha) {
        al_destroy_bitmap(recursos->fundo_batalha);
    }
    if (recursos->caixa_texto) {
        al_destroy_bitmap(recursos->caixa_texto);
    }
    if (recursos->sprite_animal) {
        al_destroy_bitmap(recursos->sprite_animal);
    }
    if (recursos->sprite_jogador) {
        al_destroy_bitmap(recursos->sprite_jogador);
    }

    free(recursos);
}

void desenhar_menu_batalha(ALLEGRO_FONT* fonte, int opcao, ALLEGRO_DISPLAY* display, Animal* animal, RecursosBatalha* recursos) {
    int largura = al_get_display_width(display);
    int altura = al_get_display_height(display);

    al_clear_to_color(al_map_rgb(0, 0, 0));

    if (recursos && recursos->fundo_batalha) {
        al_draw_scaled_bitmap(recursos->fundo_batalha,
            0, 0,
            (float)al_get_bitmap_width(recursos->fundo_batalha),
            (float)al_get_bitmap_height(recursos->fundo_batalha),
            0, 0, (float)largura, (float)altura, 0);
    }
    else {
        al_clear_to_color(al_map_rgb(0, 0, 0));
    }

    if (recursos && recursos->sprite_jogador) {
        ConfigSpriteJogador config_jogador = obter_config_sprite_jogador(animal->nome);

        float jogador_largura = (float)al_get_bitmap_width(recursos->sprite_jogador);
        float jogador_altura = (float)al_get_bitmap_height(recursos->sprite_jogador);

        al_draw_scaled_bitmap(recursos->sprite_jogador,
            0, 0, jogador_largura, jogador_altura,
            config_jogador.jogador_x, config_jogador.jogador_y,
            jogador_largura * config_jogador.jogador_escala,
            jogador_altura * config_jogador.jogador_escala, 0);
    }

    if (recursos && recursos->sprite_animal) {
        float sprite_largura = (float)al_get_bitmap_width(recursos->sprite_animal);
        float sprite_altura = (float)al_get_bitmap_height(recursos->sprite_animal);

        ConfigAnimal config = obter_config_animal(animal->nome);

        float sprite_x = (largura - sprite_largura * config.escala) / 2.0f + config.pos_x;

        al_draw_scaled_bitmap(recursos->sprite_animal,
            0, 0, sprite_largura, sprite_altura,
            sprite_x, config.pos_y,
            sprite_largura * config.escala, sprite_altura * config.escala, 0);
    }

    float caixa_y = (float)altura - 125.0f;
    float caixa_x_real = 0.0f;
    float caixa_largura_escalada = 0.0f;

    if (recursos && recursos->caixa_texto) {
        float caixa_largura = (float)al_get_bitmap_width(recursos->caixa_texto);
        float caixa_altura = (float)al_get_bitmap_height(recursos->caixa_texto);

        float escala_caixa = 0.3f;
        float offset_caixa_x = 250.0f;

        caixa_x_real = ((largura - caixa_largura * escala_caixa) / 2.0f) + offset_caixa_x;
        caixa_largura_escalada = caixa_largura * escala_caixa;

        al_draw_scaled_bitmap(recursos->caixa_texto,
            0, 0, caixa_largura, caixa_altura,
            caixa_x_real, caixa_y,
            caixa_largura_escalada, caixa_altura * escala_caixa, 0);
    }
    else {
        float cx = largura / 2.0f;
        float cy = altura - 150.0f;
        float tamanho = 200.0f;

        ALLEGRO_COLOR azul_escuro = al_map_rgb(10, 30, 80);
        ALLEGRO_VERTEX vertices[4] = {
            {cx, cy - tamanho / 2, 0, 0, 0, azul_escuro},
            {cx + tamanho / 2, cy, 0, 0, 0, azul_escuro},
            {cx, cy + tamanho / 2, 0, 0, 0, azul_escuro},
            {cx - tamanho / 2, cy, 0, 0, 0, azul_escuro}
        };
        al_draw_filled_polygon((float*)&vertices, 4, azul_escuro);

        caixa_x_real = cx - tamanho / 2;
        caixa_largura_escalada = tamanho;
    }

    float cx = largura / 2.0f;
    char info_text[256];
    snprintf(info_text, sizeof(info_text), "%s - %s", animal->nome, animal->tipo);
    al_draw_text(fonte, al_map_rgb(255, 255, 255), cx, 20, ALLEGRO_ALIGN_CENTRE, info_text);

    snprintf(info_text, sizeof(info_text), "Alimentacao: %d%%", animal->nivel_alimentacao);
    al_draw_text(fonte, al_map_rgb(255, 200, 0), cx, 45, ALLEGRO_ALIGN_CENTRE, info_text);

    float barra_x = cx - 100;
    float barra_y = 65;
    al_draw_filled_rectangle(barra_x, barra_y, barra_x + 200, barra_y + 15, al_map_rgb(50, 50, 50));
    al_draw_filled_rectangle(barra_x, barra_y,
        barra_x + (200 * animal->nivel_alimentacao / 100.0f),
        barra_y + 15, al_map_rgb(255, 200, 0));

    snprintf(info_text, sizeof(info_text), "Estudo: %d%%", animal->experiencia);
    al_draw_text(fonte, al_map_rgb(0, 255, 128), cx, 90, ALLEGRO_ALIGN_CENTRE, info_text);

    al_draw_filled_rectangle(barra_x, 110, barra_x + 200, 125, al_map_rgb(50, 50, 50));
    al_draw_filled_rectangle(barra_x, 110,
        barra_x + (200 * animal->experiencia / 100.0f),
        125, al_map_rgb(0, 255, 128));

    if (animal->domado) {
        al_draw_text(fonte, al_map_rgb(0, 255, 0), cx, altura - 270.0f, ALLEGRO_ALIGN_CENTRE, "DOMADO");
    }
    else {
        al_draw_text(fonte, al_map_rgb(255, 100, 100), cx, altura - 270.0f, ALLEGRO_ALIGN_CENTRE, "SELVAGEM");
    }

    float caixa_x_centro = (caixa_x_real + (caixa_largura_escalada / 2.0f)) - 100;

    float opcoes_y_base = caixa_y + 25;
    float espacamento = 20;

    const char* opcoes[] = { "Alimentar", "Estudar", "Correr" };

    for (int i = 0; i < 3; i++) {
        ALLEGRO_COLOR cor = (i == opcao) ? al_map_rgb(0, 255, 255) : al_map_rgb(0, 0, 0);

        if (i == 1 && !animal->domado) {
            cor = al_map_rgb(255, 0, 0);
        }

        al_draw_text(fonte, cor, caixa_x_centro, opcoes_y_base + (i * espacamento), ALLEGRO_ALIGN_CENTRE, opcoes[i]);
    }

    if (!animal->domado) {
        al_draw_text(fonte, al_map_rgb(255, 255, 100), caixa_x_centro + 100, altura - 35.0f, ALLEGRO_ALIGN_CENTRE,
            "Alimente o animal ate 100% para doma-lo!");
    }
    else if (!animal->estudado) {
        al_draw_text(fonte, al_map_rgb(255, 255, 100), caixa_x_centro + 100, altura - 35.0f, ALLEGRO_ALIGN_CENTRE,
            "Estude o animal ate 100% para completa-lo!");
    }

    al_flip_display();
}

void iniciar_batalha(ALLEGRO_FONT* fonte, Animal* animal, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_DISPLAY* display) {
    bool batalhando = true;
    int opcao = 0;

    const char* caminho_caixa_texto = "assets/img/estruturas/caixa_de_texto.png";
    const char* caminho_fundo = animal->caminho_fundo_batalha ? animal->caminho_fundo_batalha : "assets/img/estruturas/selva.png";
    const char* caminho_sprite_jogador = "assets/img/Heroi/idle_right.png";

    char caminho_sprite_animal[256];
    snprintf(caminho_sprite_animal, sizeof(caminho_sprite_animal), "assets/img/animais/%s.png", animal->nome);

    for (size_t i = 0; caminho_sprite_animal[i]; i++) {
        if (caminho_sprite_animal[i] >= 'A' && caminho_sprite_animal[i] <= 'Z') {
            caminho_sprite_animal[i] = caminho_sprite_animal[i] + ('a' - 'A');
        }
        if (caminho_sprite_animal[i] == ' ') {
            caminho_sprite_animal[i] = '_';
        }
    }

    RecursosBatalha* recursos = carregar_recursos_batalha(caminho_fundo, caminho_caixa_texto, caminho_sprite_animal, caminho_sprite_jogador);

    int total_frases = 0;
    const char** frases_disponiveis = obter_frases_animal(animal->nome, &total_frases);
    bool* frases_exibidas = NULL;

    if (total_frases > 0) {
        frases_exibidas = (bool*)calloc((size_t)total_frases, sizeof(bool));
    }

    desenhar_menu_batalha(fonte, opcao, display, animal, recursos);

    while (batalhando) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER) {
            continue;
        }

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_W:
            case ALLEGRO_KEY_UP:
                do {
                    opcao = (opcao + 2) % 3;
                } while (opcao == 1 && !animal->domado);
                break;

            case ALLEGRO_KEY_S:
            case ALLEGRO_KEY_DOWN:
                do {
                    opcao = (opcao + 1) % 3;
                } while (opcao == 1 && !animal->domado);
                break;

            case ALLEGRO_KEY_ENTER:
            case ALLEGRO_KEY_SPACE:
                switch (opcao) {
                case 0:
                    printf("Opcao selecionada: Alimentar\n");
                    animal->nivel_alimentacao += 15;

                    if (animal->nivel_alimentacao >= 100) {
                        animal->nivel_alimentacao = 100;
                        animal->alimentado = true;
                        animal->domado = true;
                        printf("%s foi domado!\n", animal->nome);
                        opcao = 1;
                    }
                    break;

                case 1:
                    if (!animal->domado) {
                        printf("O animal precisa estar domado para ser estudado!\n");
                        break;
                    }

                    printf("Opcao selecionada: Estudar\n");

                    if (total_frases > 0 && frases_exibidas && frases_disponiveis) {
                        int frases_restantes = 0;
                        for (int i = 0; i < total_frases; i++) {
                            if (!frases_exibidas[i]) frases_restantes++;
                        }

                        if (frases_restantes > 0) {
                            int idx_escolhido = -1;
                            int tentativas = 0;

                            do {
                                idx_escolhido = rand() % total_frases;
                                tentativas++;
                            } while (frases_exibidas[idx_escolhido] && tentativas < total_frases * 2);

                            if (!frases_exibidas[idx_escolhido]) {
                                mostrar_popup_estudo(fonte, display, event_queue, frases_disponiveis[idx_escolhido]);
                                frases_exibidas[idx_escolhido] = true;
                            }
                        }
                        else {
                            for (int i = 0; i < total_frases; i++) {
                                frases_exibidas[i] = false;
                            }
                            int idx = rand() % total_frases;
                            mostrar_popup_estudo(fonte, display, event_queue, frases_disponiveis[idx]);
                            frases_exibidas[idx] = true;
                        }
                    }

                    animal->experiencia += 15;

                    if (animal->experiencia >= 100) {
                        animal->experiencia = 100;
                        animal->estudado = true;
                        printf("%s foi completamente estudado!\n", animal->nome);
                        batalhando = false;
                    }
                    break;

                case 2:
                    printf("Voce correu da batalha!\n");
                    batalhando = false;
                    break;
                }
                break;

            case ALLEGRO_KEY_ESCAPE:
                batalhando = false;
                break;
            }

            desenhar_menu_batalha(fonte, opcao, display, animal, recursos);
        }
    }

    if (frases_exibidas) {
        free(frases_exibidas);
    }

    destruir_recursos_batalha(recursos);
}

void iniciar_batalha_com_bestiario(ALLEGRO_FONT* fonte, Animal* animal,
    ALLEGRO_EVENT_QUEUE* event_queue,
    ALLEGRO_DISPLAY* display,
    Bestiario* bestiario) {
    bool batalhando = true;
    int opcao = 0;

    const char* caminho_caixa_texto = "assets/img/estruturas/caixa_de_texto.png";
    const char* caminho_fundo = animal->caminho_fundo_batalha ? animal->caminho_fundo_batalha : "assets/img/estruturas/selva.png";
    const char* caminho_sprite_jogador = "assets/img/Heroi/idle_right.png";

    char caminho_sprite_animal[256];
    snprintf(caminho_sprite_animal, sizeof(caminho_sprite_animal), "assets/img/animais/%s.png", animal->nome);

    for (size_t i = 0; caminho_sprite_animal[i]; i++) {
        if (caminho_sprite_animal[i] >= 'A' && caminho_sprite_animal[i] <= 'Z') {
            caminho_sprite_animal[i] = caminho_sprite_animal[i] + ('a' - 'A');
        }
        if (caminho_sprite_animal[i] == ' ') {
            caminho_sprite_animal[i] = '_';
        }
    }

    RecursosBatalha* recursos = carregar_recursos_batalha(caminho_fundo, caminho_caixa_texto, caminho_sprite_animal, caminho_sprite_jogador);

    int total_frases = 0;
    const char** frases_disponiveis = obter_frases_animal(animal->nome, &total_frases);
    bool* frases_exibidas = NULL;

    if (total_frases > 0) {
        frases_exibidas = (bool*)calloc((size_t)total_frases, sizeof(bool));
    }

    desenhar_menu_batalha(fonte, opcao, display, animal, recursos);

    while (batalhando) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER) {
            continue;
        }

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_W:
            case ALLEGRO_KEY_UP:
                do {
                    opcao = (opcao + 2) % 3;
                } while (opcao == 1 && !animal->domado);
                break;

            case ALLEGRO_KEY_S:
            case ALLEGRO_KEY_DOWN:
                do {
                    opcao = (opcao + 1) % 3;
                } while (opcao == 1 && !animal->domado);
                break;

            case ALLEGRO_KEY_ENTER:
            case ALLEGRO_KEY_SPACE:
                switch (opcao) {
                case 0:
                    printf("Opcao selecionada: Alimentar\n");
                    animal->nivel_alimentacao += 15;

                    if (animal->nivel_alimentacao >= 100) {
                        animal->nivel_alimentacao = 100;
                        animal->alimentado = true;
                        animal->domado = true;
                        printf("%s foi domado!\n", animal->nome);
                        opcao = 1;
                    }
                    break;

                case 1:
                    if (!animal->domado) {
                        printf("O animal precisa estar domado para ser estudado!\n");
                        break;
                    }

                    printf("Opcao selecionada: Estudar\n");

                    if (total_frases > 0 && frases_exibidas && frases_disponiveis) {
                        int frases_restantes = 0;
                        for (int i = 0; i < total_frases; i++) {
                            if (!frases_exibidas[i]) frases_restantes++;
                        }

                        if (frases_restantes > 0) {
                            int idx_escolhido = -1;
                            int tentativas = 0;

                            do {
                                idx_escolhido = rand() % total_frases;
                                tentativas++;
                            } while (frases_exibidas[idx_escolhido] && tentativas < total_frases * 2);

                            if (!frases_exibidas[idx_escolhido]) {
                                mostrar_popup_estudo(fonte, display, event_queue, frases_disponiveis[idx_escolhido]);
                                frases_exibidas[idx_escolhido] = true;
                            }
                        }
                        else {
                            for (int i = 0; i < total_frases; i++) {
                                frases_exibidas[i] = false;
                            }
                            int idx = rand() % total_frases;
                            mostrar_popup_estudo(fonte, display, event_queue, frases_disponiveis[idx]);
                            frases_exibidas[idx] = true;
                        }
                    }

                    animal->experiencia += 15;

                    if (animal->experiencia >= 100) {
                        animal->experiencia = 100;
                        animal->estudado = true;
                        printf("%s foi completamente estudado!\n", animal->nome);

                        if (bestiario) {
                            desbloquear_entrada_bestiario(bestiario, animal->nome);
                        }

                        batalhando = false;
                    }
                    break;

                case 2:
                    printf("Voce correu da batalha!\n");
                    batalhando = false;
                    break;
                }
                break;

            case ALLEGRO_KEY_ESCAPE:
                batalhando = false;
                break;
            }

            desenhar_menu_batalha(fonte, opcao, display, animal, recursos);
        }
    }

    if (frases_exibidas) {
        free(frases_exibidas);
    }

    destruir_recursos_batalha(recursos);
}