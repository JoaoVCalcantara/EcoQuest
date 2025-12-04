#define _CRT_SECURE_NO_WARNINGS

#include "bestiario.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro_primitives.h>

// ========== INFORMAÇÕES ECOLÓGICAS ==========

InformacoesEcologicas criar_info_ecologica_lobo_guara(void) {
    InformacoesEcologicas info;
    info.nome_cientifico = "Chrysocyon brachyurus";
    info.nivel_trofico = NIVEL_CONSUMIDOR_TERCIARIO;
    info.posicao_cadeia = POSICAO_ONIVORO;
    info.fator_bioacumulacao = 3.5f;
    info.dieta_principal = "Frutas (90%), pequenos mamiferos, aves";
    info.papel_ecologico = "Dispersor de sementes do Cerrado";
    info.status_conservacao = "Quase Ameacado (NT)";
    info.especie_indicadora = true;
    info.biomarcador_de = "Saude do Cerrado";
    return info;
}

InformacoesEcologicas criar_info_ecologica_jacare(void) {
    InformacoesEcologicas info;
    info.nome_cientifico = "Caiman latirostris";
    info.nivel_trofico = NIVEL_CONSUMIDOR_TERCIARIO;
    info.posicao_cadeia = POSICAO_PREDADOR_MEDIO;
    info.fator_bioacumulacao = 5.2f;
    info.dieta_principal = "Peixes, crustaceos, aves aquaticas";
    info.papel_ecologico = "Regulador de populacoes aquaticas";
    info.status_conservacao = "Pouco Preocupante (LC)";
    info.especie_indicadora = true;
    info.biomarcador_de = "Contaminacao por metais pesados";
    return info;
}

InformacoesEcologicas criar_info_ecologica_boto(void) {
    InformacoesEcologicas info;
    info.nome_cientifico = "Inia geoffrensis";
    info.nivel_trofico = NIVEL_CONSUMIDOR_TERCIARIO;
    info.posicao_cadeia = POSICAO_PREDADOR_MEDIO;
    info.fator_bioacumulacao = 7.8f;
    info.dieta_principal = "Peixes diversos, tartarugas, caranguejos";
    info.papel_ecologico = "Predador de topo aquatico da Amazonia";
    info.status_conservacao = "Em Perigo (EN)";
    info.especie_indicadora = true;
    info.biomarcador_de = "Contaminacao por mercurio";
    return info;
}

InformacoesEcologicas criar_info_ecologica_onca(void) {
    InformacoesEcologicas info;
    info.nome_cientifico = "Panthera onca";
    info.nivel_trofico = NIVEL_CONSUMIDOR_QUATERNARIO;
    info.posicao_cadeia = POSICAO_PREDADOR_TOPO;
    info.fator_bioacumulacao = 6.5f;
    info.dieta_principal = "Grandes mamiferos, repteis, peixes";
    info.papel_ecologico = "Regulador maximo da cadeia alimentar";
    info.status_conservacao = "Quase Ameacado (NT)";
    info.especie_indicadora = true;
    info.biomarcador_de = "Saude geral do ecossistema";
    return info;
}

InformacoesEcologicas criar_info_ecologica_harpia(void) {
    InformacoesEcologicas info;
    info.nome_cientifico = "Harpia harpyja";
    info.nivel_trofico = NIVEL_CONSUMIDOR_QUATERNARIO;
    info.posicao_cadeia = POSICAO_PREDADOR_TOPO;
    info.fator_bioacumulacao = 7.2f;
    info.dieta_principal = "Primatas, preguicas, aves";
    info.papel_ecologico = "Predadora de topo do dossel amazonico";
    info.status_conservacao = "Quase Ameacado (NT)";
    info.especie_indicadora = true;
    info.biomarcador_de = "Integridade da floresta primaria";
    return info;
}

InformacoesEcologicas criar_info_ecologica_mico(void) {
    InformacoesEcologicas info;
    info.nome_cientifico = "Leontopithecus rosalia";
    info.nivel_trofico = NIVEL_CONSUMIDOR_SECUNDARIO;
    info.posicao_cadeia = POSICAO_ONIVORO;
    info.fator_bioacumulacao = 3.0f;
    info.dieta_principal = "Frutas, insetos, pequenos vertebrados";
    info.papel_ecologico = "Dispersor de sementes da Mata Atlantica";
    info.status_conservacao = "Em Perigo (EN)";
    info.especie_indicadora = true;
    info.biomarcador_de = "Saude da Mata Atlantica";
    return info;
}

InformacoesEcologicas criar_info_ecologica_capivara(void) {
    InformacoesEcologicas info;
    info.nome_cientifico = "Hydrochoerus hydrochaeris";
    info.nivel_trofico = NIVEL_CONSUMIDOR_PRIMARIO;
    info.posicao_cadeia = POSICAO_HERBIVORO;
    info.fator_bioacumulacao = 2.5f;
    info.dieta_principal = "Gramineas, plantas aquaticas";
    info.papel_ecologico = "Engenheira de ecossistemas aquaticos";
    info.status_conservacao = "Pouco Preocupante (LC)";
    info.especie_indicadora = true;
    info.biomarcador_de = "Qualidade de areas alagadas";
    return info;
}

InformacoesEcologicas criar_info_ecologica_tubarao(void) {
    InformacoesEcologicas info;
    info.nome_cientifico = "Carcharhinus leucas";
    info.nivel_trofico = NIVEL_CONSUMIDOR_QUATERNARIO;
    info.posicao_cadeia = POSICAO_PREDADOR_TOPO;
    info.fator_bioacumulacao = 8.5f;
    info.dieta_principal = "Peixes, mamiferos aquaticos, aves";
    info.papel_ecologico = "Predador de topo aquatico dulcicola";
    info.status_conservacao = "Quase Ameacado (NT)";
    info.especie_indicadora = true;
    info.biomarcador_de = "Contaminacao de ambientes aquaticos";
    return info;
}

InformacoesEcologicas criar_info_ecologica_tamandua(void) {
    InformacoesEcologicas info;
    info.nome_cientifico = "Myrmecophaga tridactyla";
    info.nivel_trofico = NIVEL_CONSUMIDOR_SECUNDARIO;
    info.posicao_cadeia = POSICAO_PREDADOR_PEQUENO;
    info.fator_bioacumulacao = 2.8f;
    info.dieta_principal = "Formigas, cupins";
    info.papel_ecologico = "Controlador de populacoes de insetos";
    info.status_conservacao = "Vulneravel (VU)";
    info.especie_indicadora = true;
    info.biomarcador_de = "Saude do Cerrado";
    return info;
}

InformacoesEcologicas criar_info_ecologica_humano(void) {
    InformacoesEcologicas info;
    info.nome_cientifico = "Homo sapiens";
    info.nivel_trofico = NIVEL_CONSUMIDOR_QUATERNARIO;
    info.posicao_cadeia = POSICAO_PREDADOR_TOPO;
    info.fator_bioacumulacao = 9.5f;
    info.dieta_principal = "Onivoro (ampla variedade)";
    info.papel_ecologico = "Superpredador e modificador de ecossistemas";
    info.status_conservacao = "Pouco Preocupante (LC) - Populacao estavel";
    info.especie_indicadora = false;
    info.biomarcador_de = "Impacto antropogenico";
    return info;
}

InformacoesEcologicas criar_info_ecologica_pirarucu(void) {
    InformacoesEcologicas info;
    info.nome_cientifico = "Arapaima gigas";
    info.nivel_trofico = NIVEL_CONSUMIDOR_TERCIARIO;
    info.posicao_cadeia = POSICAO_PREDADOR_MEDIO;
    info.fator_bioacumulacao = 5.5f;
    info.dieta_principal = "Peixes, crustaceos, aves aquaticas";
    info.papel_ecologico = "Predador de topo aquatico amazonico";
    info.status_conservacao = "Dados Insuficientes (DD)";
    info.especie_indicadora = true;
    info.biomarcador_de = "Qualidade de aguas amazonicas";
    return info;
}

// ========== FUNÇÕES AUXILIARES ==========

const char* obter_nome_nivel_trofico(NivelTrofico nivel) {
    switch (nivel) {
        case NIVEL_PRODUTOR: return "Produtor";
        case NIVEL_CONSUMIDOR_PRIMARIO: return "Consumidor Primario";
        case NIVEL_CONSUMIDOR_SECUNDARIO: return "Consumidor Secundario";
        case NIVEL_CONSUMIDOR_TERCIARIO: return "Consumidor Terciario";
        case NIVEL_CONSUMIDOR_QUATERNARIO: return "Consumidor Quaternario";
        case NIVEL_DECOMPOSITOR: return "Decompositor";
        default: return "Desconhecido";
    }
}

const char* obter_nome_posicao_cadeia(PosicaoCadeiaAlimentar posicao) {
    switch (posicao) {
        case POSICAO_BASE: return "Base (Produtor)";
        case POSICAO_HERBIVORO: return "Herbivoro";
        case POSICAO_PREDADOR_PEQUENO: return "Predador Pequeno";
        case POSICAO_PREDADOR_MEDIO: return "Predador Medio";
        case POSICAO_PREDADOR_TOPO: return "Predador de Topo";
        case POSICAO_ONIVORO: return "Onivoro";
        case POSICAO_DECOMPOSITORES: return "Decompositor";
        default: return "Desconhecido";
    }
}

// ========== GERENCIAMENTO SIMPLIFICADO ==========

Bestiario* criar_bestiario(void) {
    Bestiario* bestiario = (Bestiario*)malloc(sizeof(Bestiario));
    if (!bestiario) {
        fprintf(stderr, "Erro ao alocar memoria para bestiario\n");
        return NULL;
    }
    
    bestiario->total_especies = 0;
    bestiario->especies_desbloqueadas = 0;
    bestiario->humano_desbloqueado = false;
    
    return bestiario;
}

void adicionar_especie_bestiario(Bestiario* bestiario, Animal* animal) {
    if (!bestiario || !animal || bestiario->total_especies >= 10) return;

    int idx = bestiario->total_especies;
    bestiario->entradas[idx].animal_ref = animal;
    bestiario->entradas[idx].desbloqueado = false;

    // Atribui informações ecológicas
    char nome_lower[64];
    snprintf(nome_lower, sizeof(nome_lower), "%s", animal->nome);
    for (int i = 0; nome_lower[i]; i++) {
        if (nome_lower[i] >= 'A' && nome_lower[i] <= 'Z') {
            nome_lower[i] = nome_lower[i] + ('a' - 'A');
        }
    }

    // Normaliza nome com underscore (substitui espaços E HÍFENS)
    char nome_normalizado[64];
    int j = 0;
    for (int i = 0; nome_lower[i] && j < 63; i++) {
        if (nome_lower[i] == ' ' || nome_lower[i] == '-') {
            nome_normalizado[j++] = '_';
        }
        else {
            nome_normalizado[j++] = nome_lower[i];
        }
    }
    nome_normalizado[j] = '\0';

    printf("[BESTIARIO] Adicionando: '%s' | Normalizado: '%s'\n", animal->nome, nome_normalizado);

    if (strcmp(nome_normalizado, "lobo_guara") == 0) {
        bestiario->entradas[idx].info_ecologica = criar_info_ecologica_lobo_guara();
    }
    else if (strcmp(nome_lower, "jacare") == 0) {
        bestiario->entradas[idx].info_ecologica = criar_info_ecologica_jacare();
    }
    else if (strcmp(nome_lower, "boto") == 0) {
        bestiario->entradas[idx].info_ecologica = criar_info_ecologica_boto();
    }
    else if (strcmp(nome_lower, "onca") == 0) {
        bestiario->entradas[idx].info_ecologica = criar_info_ecologica_onca();
    }
    else if (strcmp(nome_lower, "harpia") == 0) {
        bestiario->entradas[idx].info_ecologica = criar_info_ecologica_harpia();
    }
    else if (strcmp(nome_normalizado, "mico_leao_dourado") == 0) {
        bestiario->entradas[idx].info_ecologica = criar_info_ecologica_mico();
    }
    else if (strcmp(nome_lower, "capivara") == 0) {
        bestiario->entradas[idx].info_ecologica = criar_info_ecologica_capivara();
    }
    else if (strcmp(nome_normalizado, "tubarao_touro") == 0) {
        bestiario->entradas[idx].info_ecologica = criar_info_ecologica_tubarao();
    }
    else if (strcmp(nome_normalizado, "tamandua_bandeira") == 0) {
        bestiario->entradas[idx].info_ecologica = criar_info_ecologica_tamandua();
    }
    else if (strcmp(nome_lower, "pirarucu") == 0) {
        bestiario->entradas[idx].info_ecologica = criar_info_ecologica_pirarucu();
    }
    else {
        printf("[AVISO] Animal '%s' nao possui info ecologica cadastrada!\n", animal->nome);
    }

    bestiario->total_especies++;
}

void desbloquear_entrada_bestiario(Bestiario* bestiario, const char* nome_animal) {
    if (!bestiario || !nome_animal) return;
    
    for (int i = 0; i < bestiario->total_especies; i++) {
        if (strcmp(bestiario->entradas[i].animal_ref->nome, nome_animal) == 0) {
            if (!bestiario->entradas[i].desbloqueado) {
                bestiario->entradas[i].desbloqueado = true;
                bestiario->especies_desbloqueadas++;
                printf("[BESTIARIO] %s desbloqueado!\n", nome_animal);
            }
            break;
        }
    }
}

void destruir_bestiario(Bestiario* bestiario) {
    if (bestiario) {
        free(bestiario);
    }
}

void desbloquear_humano_bestiario(Bestiario* bestiario) {
    if (!bestiario) return;
    
    if (!bestiario->humano_desbloqueado) {
        bestiario->humano_desbloqueado = true;
        printf("[BESTIARIO] Humano desbloqueado no bestiario!\n");
    }
}

// ========== POPUP SIMPLIFICADO ==========

// Função auxiliar para quebrar texto em linhas
static void desenhar_texto_quebrado(ALLEGRO_FONT* fonte, ALLEGRO_COLOR cor, float x, float* y, 
                                     float max_width, float line_height, const char* texto) {
    if (!texto || !fonte) return;
    
    char buffer[512];
    strncpy(buffer, texto, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    char linha_atual[256] = "";
    char* palavra_ptr = buffer;
    
    while (*palavra_ptr) {
        // Pula espaços iniciais
        while (*palavra_ptr == ' ') palavra_ptr++;
        if (!*palavra_ptr) break;
        
        // Encontra fim da palavra
        char* fim_palavra = palavra_ptr;
        while (*fim_palavra && *fim_palavra != ' ') fim_palavra++;
        
        size_t len_palavra = (size_t)(fim_palavra - palavra_ptr);
        if (len_palavra > 200) len_palavra = 200;
        
        char palavra[256];
        strncpy(palavra, palavra_ptr, len_palavra);
        palavra[len_palavra] = '\0';
        
        // Testa se cabe na linha atual
        char teste[512];
        if (linha_atual[0] == '\0') {
            strncpy(teste, palavra, sizeof(teste) - 1);
            teste[sizeof(teste) - 1] = '\0';
        } else {
            snprintf(teste, sizeof(teste), "%s %s", linha_atual, palavra);
        }
        
        float w = al_get_text_width(fonte, teste);
        if (w > max_width && linha_atual[0] != '\0') {
            // Desenha linha atual e começa nova
            al_draw_text(fonte, cor, x, *y, ALLEGRO_ALIGN_LEFT, linha_atual);
            *y += line_height;
            strncpy(linha_atual, palavra, sizeof(linha_atual) - 1);
            linha_atual[sizeof(linha_atual) - 1] = '\0';
        } else {
            strncpy(linha_atual, teste, sizeof(linha_atual) - 1);
            linha_atual[sizeof(linha_atual) - 1] = '\0';
        }
        
        palavra_ptr = fim_palavra;
    }
    
    // Desenha última linha
    if (linha_atual[0] != '\0') {
        al_draw_text(fonte, cor, x, *y, ALLEGRO_ALIGN_LEFT, linha_atual);
        *y += line_height;
    }
}

void mostrar_bestiario_popup(Bestiario* bestiario, ALLEGRO_FONT* fonte, ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE* event_queue) {
    if (!bestiario || !display || !fonte) return;
    
    int largura = al_get_display_width(display);
    int altura = al_get_display_height(display);
    
    float box_w = largura * 0.85f;
    float box_h = altura * 0.85f;
    float box_x = (largura - box_w) / 2.0f;
    float box_y = (altura - box_h) / 2.0f;
    
    // Desenha overlay escuro
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_filled_rectangle(0, 0, (float)largura, (float)altura, al_map_rgba(0, 0, 0, 200));
    
    // Caixa principal
    al_draw_filled_rounded_rectangle(box_x, box_y, box_x + box_w, box_y + box_h, 10, 10, al_map_rgb(240, 240, 230));
    al_draw_rounded_rectangle(box_x, box_y, box_x + box_w, box_y + box_h, 10, 10, al_map_rgb(100, 70, 30), 3);
    
    // Título
    al_draw_text(fonte, al_map_rgb(40, 20, 0), box_x + box_w / 2, box_y + 15, ALLEGRO_ALIGN_CENTRE, "=== BESTIARIO ECOLOGICO ===");
    
    int total_para_exibir = bestiario->total_especies + (bestiario->humano_desbloqueado ? 1 : 0);
    char progresso[128];
    snprintf(progresso, sizeof(progresso), "Especies Desbloqueadas: %d/%d", 
             bestiario->especies_desbloqueadas + (bestiario->humano_desbloqueado ? 1 : 0), 
             total_para_exibir);
    al_draw_text(fonte, al_map_rgb(60, 40, 10), box_x + box_w / 2, box_y + 40, ALLEGRO_ALIGN_CENTRE, progresso);
    
    // Lista de espécies em formato compacto
    float y = box_y + 75;
    float x = box_x + 20;
    float line_height = al_get_font_line_height(fonte) + 2;
    float max_width = box_w - 40;
    
    for (int i = 0; i < bestiario->total_especies; i++) {
        EntradaBestiario* entrada = &bestiario->entradas[i];
        
        // Verifica se ainda cabe na caixa
        if (y > box_y + box_h - 50) break;
        
        if (!entrada->desbloqueado) {
            al_draw_text(fonte, al_map_rgb(80, 80, 80), x, y, ALLEGRO_ALIGN_LEFT, "??? - BLOQUEADO");
            y += line_height * 1.5f;
        } else {
            InformacoesEcologicas* info = &entrada->info_ecologica;
            
            // Nome
            char linha[512];
            snprintf(linha, sizeof(linha), "%s (%s)", entrada->animal_ref->nome, info->nome_cientifico);
            desenhar_texto_quebrado(fonte, al_map_rgb(0, 100, 0), x, &y, max_width, line_height, linha);
            
            // Nível trófico
            snprintf(linha, sizeof(linha), "  Nivel: %s", obter_nome_nivel_trofico(info->nivel_trofico));
            desenhar_texto_quebrado(fonte, al_map_rgb(50, 50, 50), x, &y, max_width, line_height, linha);
            
            // Dieta
            snprintf(linha, sizeof(linha), "  Dieta: %s", info->dieta_principal);
            desenhar_texto_quebrado(fonte, al_map_rgb(50, 50, 50), x, &y, max_width, line_height, linha);
            
            // Papel ecológico
            snprintf(linha, sizeof(linha), "  Papel: %s", info->papel_ecologico);
            desenhar_texto_quebrado(fonte, al_map_rgb(0, 100, 100), x, &y, max_width, line_height, linha);
            
            // Status
            ALLEGRO_COLOR cor_status = strstr(info->status_conservacao, "Perigo") || strstr(info->status_conservacao, "Vulneravel") ? 
                                       al_map_rgb(200, 0, 0) : al_map_rgb(0, 150, 0);
            snprintf(linha, sizeof(linha), "  Status: %s", info->status_conservacao);
            desenhar_texto_quebrado(fonte, cor_status, x, &y, max_width, line_height, linha);
            
            y += line_height * 0.5f; // Espaço entre animais
        }
    }
    
    // Humano (se desbloqueado)
    if (bestiario->humano_desbloqueado && y < box_y + box_h - 50) {
        InformacoesEcologicas info_humano = criar_info_ecologica_humano();
        char linha[512];
        
        snprintf(linha, sizeof(linha), "Humano (%s)", info_humano.nome_cientifico);
        desenhar_texto_quebrado(fonte, al_map_rgb(100, 0, 100), x, &y, max_width, line_height, linha);
        
        snprintf(linha, sizeof(linha), "  Nivel: %s", obter_nome_nivel_trofico(info_humano.nivel_trofico));
        desenhar_texto_quebrado(fonte, al_map_rgb(50, 50, 50), x, &y, max_width, line_height, linha);
        
        snprintf(linha, sizeof(linha), "  Papel: %s", info_humano.papel_ecologico);
        desenhar_texto_quebrado(fonte, al_map_rgb(0, 100, 100), x, &y, max_width, line_height, linha);
    }
    
    // Instrução
    al_draw_text(fonte, al_map_rgb(100, 100, 100), box_x + box_w / 2, box_y + box_h - 25, 
                 ALLEGRO_ALIGN_CENTRE, "Pressione ENTER ou ESC para fechar");
    
    al_flip_display();
    
    // Aguarda input
    if (event_queue) {
        bool esperando = true;
        while (esperando) {
            ALLEGRO_EVENT ev;
            al_wait_for_event(event_queue, &ev);
            
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER || 
                    ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE ||
                    ev.keyboard.keycode == ALLEGRO_KEY_B) {
                    esperando = false;
                }
            } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                esperando = false;
            }
        }
    }
}