#define _CRT_SECURE_NO_WARNINGS

#include "bestiario.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <allegro5/allegro_primitives.h>

// ========== INFORMAÇÕES ECOLÓGICAS ==========

InformacoesEcologicas criar_info_ecologica_raposa(void) {
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
    
    return bestiario;
}

void adicionar_especie_bestiario(Bestiario* bestiario, Animal* animal) {
    if (!bestiario || !animal || bestiario->total_especies >= 4) return;
    
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
    
    if (strcmp(nome_lower, "raposa") == 0) {
        bestiario->entradas[idx].info_ecologica = criar_info_ecologica_raposa();
    } else if (strcmp(nome_lower, "jacare") == 0) {
        bestiario->entradas[idx].info_ecologica = criar_info_ecologica_jacare();
    } else if (strcmp(nome_lower, "boto") == 0) {
        bestiario->entradas[idx].info_ecologica = criar_info_ecologica_boto();
    } else if (strcmp(nome_lower, "onca") == 0) {
        bestiario->entradas[idx].info_ecologica = criar_info_ecologica_onca();
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

// ========== POPUP SIMPLIFICADO ==========

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
    
    char progresso[128];
    snprintf(progresso, sizeof(progresso), "Especies Desbloqueadas: %d/%d", bestiario->especies_desbloqueadas, bestiario->total_especies);
    al_draw_text(fonte, al_map_rgb(60, 40, 10), box_x + box_w / 2, box_y + 40, ALLEGRO_ALIGN_CENTRE, progresso);
    
    // Lista de espécies em formato compacto
    float y = box_y + 80;
    float x = box_x + 30;
    float line_height = 16;
    
    for (int i = 0; i < bestiario->total_especies; i++) {
        EntradaBestiario* entrada = &bestiario->entradas[i];
        
        if (!entrada->desbloqueado) {
            // Bloqueado
            al_draw_text(fonte, al_map_rgb(80, 80, 80), x, y, ALLEGRO_ALIGN_LEFT, "??? - BLOQUEADO");
            y += line_height * 2;
        } else {
            // Desbloqueado - mostra informações
            InformacoesEcologicas* info = &entrada->info_ecologica;
            
            // Nome
            char linha[512];
            snprintf(linha, sizeof(linha), "%s (%s)", entrada->animal_ref->nome, info->nome_cientifico);
            al_draw_text(fonte, al_map_rgb(0, 100, 0), x, y, ALLEGRO_ALIGN_LEFT, linha);
            y += line_height;
            
            // Nível trófico
            snprintf(linha, sizeof(linha), "  Nivel Trofico: %s (Nivel %d)", 
                     obter_nome_nivel_trofico(info->nivel_trofico), info->nivel_trofico);
            al_draw_text(fonte, al_map_rgb(50, 50, 50), x, y, ALLEGRO_ALIGN_LEFT, linha);
            y += line_height;
            
            // Posição na cadeia
            snprintf(linha, sizeof(linha), "  Posicao: %s", 
                     obter_nome_posicao_cadeia(info->posicao_cadeia));
            al_draw_text(fonte, al_map_rgb(50, 50, 50), x, y, ALLEGRO_ALIGN_LEFT, linha);
            y += line_height;
            
            // Bioacumulação
            ALLEGRO_COLOR cor_bio = info->fator_bioacumulacao > 7.0f ? 
                                    al_map_rgb(200, 0, 0) : al_map_rgb(200, 100, 0);
            snprintf(linha, sizeof(linha), "  Bioacumulacao: %.1f", info->fator_bioacumulacao);
            al_draw_text(fonte, cor_bio, x, y, ALLEGRO_ALIGN_LEFT, linha);
            y += line_height;
            
            // Dieta
            snprintf(linha, sizeof(linha), "  Dieta: %s", info->dieta_principal);
            al_draw_text(fonte, al_map_rgb(50, 50, 50), x, y, ALLEGRO_ALIGN_LEFT, linha);
            y += line_height;
            
            // Papel ecológico
            snprintf(linha, sizeof(linha), "  Papel: %s", info->papel_ecologico);
            al_draw_text(fonte, al_map_rgb(0, 100, 100), x, y, ALLEGRO_ALIGN_LEFT, linha);
            y += line_height;
            
            // Status de conservação
            ALLEGRO_COLOR cor_status = strstr(info->status_conservacao, "Perigo") ? 
                                       al_map_rgb(200, 0, 0) : al_map_rgb(0, 150, 0);
            snprintf(linha, sizeof(linha), "  Status: %s", info->status_conservacao);
            al_draw_text(fonte, cor_status, x, y, ALLEGRO_ALIGN_LEFT, linha);
            y += line_height;
            
            // Bioindicador
            if (info->especie_indicadora) {
                snprintf(linha, sizeof(linha), "  Bioindicador de: %s", info->biomarcador_de);
                al_draw_text(fonte, al_map_rgb(150, 100, 0), x, y, ALLEGRO_ALIGN_LEFT, linha);
                y += line_height;
            }
            
            y += line_height; // Espaço extra entre animais
        }
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