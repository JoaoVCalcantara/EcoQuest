#define _CRT_SECURE_NO_WARNINGS

#include "cacador.h"
#include "inimigo.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro_primitives.h>

// ========== FUNÇÕES AUXILIARES ==========

static void sincronizar_dados_cacador(Cacador* cacador) {
    cacador->vida = cacador->dados_batalha.vida_atual;
    cacador->vida_maxima = cacador->dados_batalha.vida_maxima;
    cacador->dano_ataque = cacador->dados_batalha.ataque_base;
    cacador->nome = cacador->dados_batalha.nome;
    cacador->derrotado = cacador->dados_batalha.derrotado;
    cacador->distancia_deteccao = cacador->raio_deteccao;
}

static void sincronizar_dados_para_batalha(Cacador* cacador) {
    cacador->dados_batalha.vida_atual = cacador->vida;
    cacador->dados_batalha.derrotado = cacador->derrotado;
}

static void limitar_posicao_cacador(float* x, float* y, float raio,
    float largura, float altura, JogoCenas cenario) {

    if (*x < raio) *x = raio;
    if (*y < raio) *y = raio;
    if (*x > largura - raio) *x = largura - raio;
    if (*y > altura - raio) *y = altura - raio;

    float min_x = 0.0f, max_x = largura;
    float min_y = 0.0f, max_y = altura;

    switch (cenario) {
    case CENARIO1:
        max_x = 640.0f; max_y = 360.0f; break;
    case CENARIO2:
        min_y = 360.0f; max_x = 640.0f; max_y = altura; break;
    case CENARIO3:
        min_x = 640.0f; max_x = largura; max_y = 360.0f; break;
    case CENARIO4:
        min_x = 640.0f; min_y = 360.0f; break;
    }

    if (*x < min_x + raio) *x = min_x + raio;
    if (*y < min_y + raio) *y = min_y + raio;
    if (*x > max_x - raio) *x = max_x - raio;
    if (*y > max_y - raio) *y = max_y - raio;
}

// Função para evitar que o animal fuja para as bordas
static void evitar_bordas_animal(float animal_x, float animal_y, float* fuga_x, float* fuga_y,
    float largura_mapa, float altura_mapa, JogoCenas cenario) {

    float min_x = 0.0f, max_x = largura_mapa;
    float min_y = 0.0f, max_y = altura_mapa;
    float margem_seguranca = 80.0f;  // Distância mínima das bordas

    // Definir limites do cenário
    switch (cenario) {
    case CENARIO1:
        max_x = 640.0f; max_y = 360.0f; break;
    case CENARIO2:
        min_y = 360.0f; max_x = 640.0f; max_y = altura_mapa; break;
    case CENARIO3:
        min_x = 640.0f; max_x = largura_mapa; max_y = 360.0f; break;
    case CENARIO4:
        min_x = 640.0f; min_y = 360.0f; break;
    }

    // Verificar proximidade com bordas e ajustar direção de fuga
    bool perto_borda_esquerda = animal_x < (min_x + margem_seguranca);
    bool perto_borda_direita = animal_x > (max_x - margem_seguranca);
    bool perto_borda_cima = animal_y < (min_y + margem_seguranca);
    bool perto_borda_baixo = animal_y > (max_y - margem_seguranca);

    // Se está perto de alguma borda, ajusta a direção de fuga para o centro
    if (perto_borda_esquerda || perto_borda_direita || perto_borda_cima || perto_borda_baixo) {
        float centro_x = (min_x + max_x) / 2.0f;
        float centro_y = (min_y + max_y) / 2.0f;

        float dx_centro = centro_x - animal_x;
        float dy_centro = centro_y - animal_y;
        float dist_centro = sqrtf(dx_centro * dx_centro + dy_centro * dy_centro);

        if (dist_centro > 0.1f) {
            // Mesclar direção de fuga original com direção ao centro
            float peso_centro = 0.6f;  // 60% em direção ao centro

            *fuga_x = (*fuga_x * (1.0f - peso_centro)) + (dx_centro / dist_centro * peso_centro);
            *fuga_y = (*fuga_y * (1.0f - peso_centro)) + (dy_centro / dist_centro * peso_centro);

            // Normalizar
            float mag = sqrtf((*fuga_x) * (*fuga_x) + (*fuga_y) * (*fuga_y));
            if (mag > 0.1f) {
                *fuga_x /= mag;
                *fuga_y /= mag;
            }
        }
    }
}

// ========== INICIALIZAÇÃO ==========

void iniciar_cacador(Cacador* cacador, float x, float y, const char* nome,
    JogoCenas cenario, const char* sprite_path,
    const char* fundo_batalha_path) {

    cacador->x = x;
    cacador->y = y;
    cacador->raio = 10.0f;
    cacador->velocidade = 0.7f;  // Aumentado de 0.6f para 0.7f
    cacador->velocidade_perseguicao = 1.3f;  // Aumentado de 0.8f para 1.3f (caçador mais rápido)

    cacador->estado = CACADOR_PATRULHANDO;
    cacador->cenario = cenario;

    cacador->alvo = NULL;
    cacador->patrulha_x = x;
    cacador->patrulha_y = y;
    cacador->tempo_espera = 0.0f;

    cacador->ativo = true;
    cacador->derrotado = false;
    cacador->raio_deteccao = 150.0f;
    cacador->raio_abandono = 250.0f;
    cacador->distancia_deteccao = 150.0f;

    cacador->cooldown_batalha = 0.0f;
    cacador->cooldown_dano = 0.0f;
    cacador->tempo_dano_interval = 1.5f;
    cacador->dano_por_contato = 10;

    cacador->dados_batalha.nome = nome;
    cacador->dados_batalha.vida_maxima = 100;
    cacador->dados_batalha.vida_atual = 100;
    cacador->dados_batalha.ataque_base = 15;
    cacador->dados_batalha.defesa_base = 10;
    cacador->dados_batalha.derrotado = false;
    cacador->dados_batalha.vezes_derrotado = 0;
    cacador->dados_batalha.caminho_sprite = sprite_path;
    cacador->dados_batalha.caminho_fundo_batalha = fundo_batalha_path;

    sincronizar_dados_cacador(cacador);

    cacador->sprite_cacador = NULL;
    cacador->usar_sprite = false;
    cacador->escala_sprite = 0.05f;

    if (sprite_path) {
        cacador->sprite_cacador = criar_sprite(sprite_path);
        cacador->usar_sprite = (cacador->sprite_cacador && cacador->sprite_cacador->carregado);
    }
}

void iniciar_cacador_com_alvo(Cacador* cacador, float x, float y,
    JogoCenas cenario, Bot* alvo, const char* nome,
    const char* sprite_path) {

    const char* fundo = "assets/img/estruturas/selva.png";

    switch (cenario) {
    case CENARIO1: fundo = "assets/img/estruturas/selva.png"; break;
    case CENARIO2: fundo = "assets/img/estruturas/pantano.png"; break;
    case CENARIO3: fundo = "assets/img/estruturas/lago.png"; break;
    case CENARIO4: fundo = "assets/img/estruturas/cerrado.png"; break;
    }

    iniciar_cacador(cacador, x, y, nome, cenario, sprite_path, fundo);
    cacador->alvo = alvo;
}

void iniciar_cacador_chefe(CacadorChefe* chefe, float x, float y,
    JogoCenas cenario, const char* sprite_path,
    const char* fundo_batalha_path) {

    iniciar_cacador(&chefe->base, x, y, "Cacador Chefe", cenario,
        sprite_path, fundo_batalha_path);

    chefe->base.dados_batalha.vida_maxima = 200;
    chefe->base.dados_batalha.vida_atual = 200;
    chefe->base.dados_batalha.ataque_base = 25;
    chefe->base.dados_batalha.defesa_base = 15;
    chefe->base.velocidade_perseguicao = 1.5f;  // Boss mais rápido
    chefe->base.raio_deteccao = 200.0f;
    chefe->base.dano_por_contato = 15;

    sincronizar_dados_cacador(&chefe->base);

    chefe->desbloqueado = false;
    chefe->liberado = false;
    chefe->derrotado = false;
    chefe->base.ativo = false;
    chefe->x_spawn = x;
    chefe->y_spawn = y;
    chefe->cenario_spawn = cenario;
}

// ========== DANO AO ANIMAL ==========

void aplicar_dano_cacador_em_animal(Cacador* cacador, Bot* bot, float delta_time) {
    if (!cacador || !bot || !cacador->ativo || cacador->derrotado || !bot->ativo) {
        return;
    }

    // Verificar colisão
    if (!verificar_colisao_cacador_animal(cacador, bot)) {
        // Se não está em contato, resetar cooldown
        cacador->cooldown_dano = 0.0f;
        return;
    }

    // Atualizar cooldown
    cacador->cooldown_dano -= delta_time;

    // Aplicar dano quando cooldown acabar
    if (cacador->cooldown_dano <= 0.0f) {
        // Aplicar dano ao animal
        if (bot->animal_data.nivel_alimentacao > 0) {
            bot->animal_data.nivel_alimentacao -= cacador->dano_por_contato;

            if (bot->animal_data.nivel_alimentacao < 0) {
                bot->animal_data.nivel_alimentacao = 0;
            }

            printf("[DANO] %s causou %d de dano a %s! Alimentacao: %d%%\n",
                cacador->nome, cacador->dano_por_contato,
                bot->animal_data.nome, bot->animal_data.nivel_alimentacao);
        }

        // Se o animal perdeu toda alimentação, é capturado
        if (bot->animal_data.nivel_alimentacao <= 0) {
            bot->ativo = false;
            printf("[CAPTURA] %s foi capturado por %s!\n",
                bot->animal_data.nome, cacador->nome);
        }

        // Reiniciar cooldown
        cacador->cooldown_dano = cacador->tempo_dano_interval;
    }

    // Empurrar caçador para trás para evitar sobreposição
    float dx = bot->x - cacador->x;
    float dy = bot->y - cacador->y;
    float distancia = sqrtf(dx * dx + dy * dy);

    if (distancia > 0.1f && distancia < (cacador->raio + bot->raio + 5.0f)) {
        float forca_repulsao = 2.0f;
        cacador->x -= (dx / distancia) * forca_repulsao;
        cacador->y -= (dy / distancia) * forca_repulsao;
    }
}

// ========== MOVIMENTO ==========

void cacador_perseguir_alvo(Cacador* cacador, float alvo_x, float alvo_y,
    float delta_time, float largura_mapa, float altura_mapa) {

    float dx = alvo_x - cacador->x;
    float dy = alvo_y - cacador->y;
    float distancia = sqrtf(dx * dx + dy * dy);

    // Distância mínima para não sobrepor o animal
    float distancia_minima = cacador->raio + 15.0f;

    if (distancia > distancia_minima) {
        float vel = cacador->velocidade_perseguicao * delta_time * 60.0f;  // CORRIGIDO: adicionar delta_time
        cacador->x += (dx / distancia) * vel;
        cacador->y += (dy / distancia) * vel;

        limitar_posicao_cacador(&cacador->x, &cacador->y, cacador->raio,
            largura_mapa, altura_mapa, cacador->cenario);
    }
    else {
        // Se está muito perto, manter distância mínima
        cacador->estado = CACADOR_ATACANDO;
    }
}

void cacador_patrulhar(Cacador* cacador, float delta_time,
    float largura_mapa, float altura_mapa) {

    float dx = cacador->patrulha_x - cacador->x;
    float dy = cacador->patrulha_y - cacador->y;
    float distancia = sqrtf(dx * dx + dy * dy);

    if (distancia < 5.0f) {
        cacador->tempo_espera += delta_time;

        if (cacador->tempo_espera >= 2.0f) {
            float min_x = 0.0f, max_x = largura_mapa;
            float min_y = 0.0f, max_y = altura_mapa;
            float margem = 50.0f;

            switch (cacador->cenario) {
            case CENARIO1:
                max_x = 640.0f; max_y = 360.0f; break;
            case CENARIO2:
                min_y = 360.0f; max_x = 640.0f; break;
            case CENARIO3:
                min_x = 640.0f; max_y = 360.0f; break;
            case CENARIO4:
                min_x = 640.0f; min_y = 360.0f; break;
            }

            // Gerar ponto de patrulha com margem de segurança
            cacador->patrulha_x = (min_x + margem) + ((float)rand() / RAND_MAX) * (max_x - min_x - 2 * margem);
            cacador->patrulha_y = (min_y + margem) + ((float)rand() / RAND_MAX) * (max_y - min_y - 2 * margem);
            cacador->tempo_espera = 0.0f;
        }
    }
    else {
        float vel = cacador->velocidade * delta_time * 60.0f;  // CORRIGIDO: adicionar delta_time
        cacador->x += (dx / distancia) * vel;
        cacador->y += (dy / distancia) * vel;

        limitar_posicao_cacador(&cacador->x, &cacador->y, cacador->raio,
            largura_mapa, altura_mapa, cacador->cenario);
    }
}

void atualizar_cacador(Cacador* cacador, float animal_x, float animal_y,
    bool animal_ativo, float delta_time,
    float largura_mapa, float altura_mapa) {

    if (!cacador->ativo || cacador->derrotado) return;

    // Atualizar cooldown de batalha
    if (cacador->cooldown_batalha > 0.0f) {
        cacador->cooldown_batalha -= delta_time;
        if (cacador->cooldown_batalha < 0.0f) {
            cacador->cooldown_batalha = 0.0f;
        }
        // IMPORTANTE: Continuar patrulhando mesmo durante cooldown
        cacador_patrulhar(cacador, delta_time, largura_mapa, altura_mapa);
        return;
    }

    bool animal_detectado = animal_ativo &&
        verificar_deteccao_animal(cacador, animal_x, animal_y);

    switch (cacador->estado) {
    case CACADOR_PATRULHANDO:
        if (animal_detectado) {
            cacador->estado = CACADOR_PERSEGUINDO;
            cacador->alvo_x = animal_x;
            cacador->alvo_y = animal_y;
        }
        else {
            cacador_patrulhar(cacador, delta_time, largura_mapa, altura_mapa);
        }
        break;

    case CACADOR_PERSEGUINDO:
    case CACADOR_ATACANDO:
        if (!animal_ativo) {
            cacador->estado = CACADOR_PATRULHANDO;
            cacador->cooldown_dano = 0.0f;
        }
        else {
            float dx = animal_x - cacador->x;
            float dy = animal_y - cacador->y;
            float distancia = sqrtf(dx * dx + dy * dy);

            if (distancia > cacador->raio_abandono) {
                cacador->estado = CACADOR_PATRULHANDO;
                cacador->cooldown_dano = 0.0f;
            }
            else {
                cacador->alvo_x = animal_x;
                cacador->alvo_y = animal_y;
                cacador_perseguir_alvo(cacador, animal_x, animal_y, delta_time,
                    largura_mapa, altura_mapa);
            }
        }
        break;

    case CACADOR_ESPERANDO:
        cacador->tempo_espera -= delta_time;
        if (cacador->tempo_espera <= 0.0f) {
            cacador->estado = CACADOR_PATRULHANDO;
        }
        break;
    }
}

void atualizar_cacador_chefe(CacadorChefe* chefe, entidade* jogador,
    float largura_mapa, float altura_mapa, float delta_time) {

    if (!chefe->liberado || chefe->derrotado) return;

    if (!chefe->base.ativo) {
        chefe->base.ativo = true;
        chefe->base.x = chefe->x_spawn;
        chefe->base.y = chefe->y_spawn;
    }

    float dx = jogador->x - chefe->base.x;
    float dy = jogador->y - chefe->base.y;
    float distancia = sqrtf(dx * dx + dy * dy);

    // Distância mínima para não sobrepor o jogador
    float distancia_minima = chefe->base.raio + jogador->raio + 10.0f;

    if (distancia > distancia_minima) {
        float vel = chefe->base.velocidade_perseguicao * delta_time * 60.0f;  // CORRIGIDO: adicionar delta_time
        chefe->base.x += (dx / distancia) * vel;
        chefe->base.y += (dy / distancia) * vel;
    }

    limitar_posicao_cacador(&chefe->base.x, &chefe->base.y, chefe->base.raio,
        largura_mapa, altura_mapa, chefe->base.cenario);
}

// ========== DESENHO ==========

void desenhar_cacador(const Cacador* cacador, ALLEGRO_COLOR cor,
    float camera_x, float camera_y) {

    if (!cacador->ativo || cacador->derrotado) return;

    float cacador_x_tela = (cacador->x - camera_x) * ZOOM_FACTOR;
    float cacador_y_tela = (cacador->y - camera_y) * ZOOM_FACTOR;

    // Desenhar sprite se disponível
    if (cacador->usar_sprite && cacador->sprite_cacador) {
        desenhar_sprite_camera(
            cacador->sprite_cacador,
            cacador->x,
            cacador->y,
            camera_x,
            camera_y,
            cacador->escala_sprite,
            ZOOM_FACTOR
        );
    }
    else {
        // Fallback sem borda circular
        al_draw_filled_circle(cacador_x_tela, cacador_y_tela,
            cacador->raio * ZOOM_FACTOR, cor);
    }

    // Indicador de estado (REMOVIDO círculo de perseguição/ataque)

    // Barra de vida estilizada
    float barra_largura = 35.0f * ZOOM_FACTOR;
    float barra_altura = 4.0f * ZOOM_FACTOR;
    float barra_x = cacador_x_tela - barra_largura / 2.0f;
    float barra_y = cacador_y_tela - (cacador->raio + 18.0f) * ZOOM_FACTOR;

    // Fundo da barra
    al_draw_filled_rounded_rectangle(
        barra_x - 1, barra_y - 1,
        barra_x + barra_largura + 1, barra_y + barra_altura + 1,
        2, 2, al_map_rgba(0, 0, 0, 180)
    );

    al_draw_filled_rounded_rectangle(
        barra_x, barra_y,
        barra_x + barra_largura, barra_y + barra_altura,
        2, 2, al_map_rgb(50, 50, 50)
    );

    // Barra de vida vermelha
    float vida_proporcao = (float)cacador->vida / (float)cacador->vida_maxima;
    al_draw_filled_rounded_rectangle(
        barra_x, barra_y,
        barra_x + barra_largura * vida_proporcao, barra_y + barra_altura,
        2, 2, al_map_rgb(220, 20, 20)
    );

    // Borda
    al_draw_rounded_rectangle(
        barra_x, barra_y,
        barra_x + barra_largura, barra_y + barra_altura,
        2, 2, al_map_rgb(255, 255, 255), 1.0f
    );
}

// ========== COLISÃO ==========

bool verificar_colisao_cacador_jogador(entidade* jogador, const Cacador* cacador) {
    if (!cacador->ativo || cacador->derrotado) return false;

    float dx = jogador->x - cacador->x;
    float dy = jogador->y - cacador->y;
    float distancia = sqrtf(dx * dx + dy * dy);

    return distancia <= (jogador->raio + cacador->raio);
}

bool verificar_colisao_cacador_animal(const Cacador* cacador, const Bot* bot) {
    if (!cacador->ativo || cacador->derrotado || !bot || !bot->ativo) return false;

    float dx = bot->x - cacador->x;
    float dy = bot->y - cacador->y;
    float distancia = sqrtf(dx * dx + dy * dy);

    return distancia <= (bot->raio + cacador->raio + 5.0f);
}

bool verificar_deteccao_animal(const Cacador* cacador, float animal_x, float animal_y) {
    float dx = animal_x - cacador->x;
    float dy = animal_y - cacador->y;
    float distancia = sqrtf(dx * dx + dy * dy);

    return distancia <= cacador->raio_deteccao;
}

// ========== BATALHA ==========


// ========== VERIFICAÇÕES ==========

bool verificar_desbloqueio_chefe(Cacador cacadores[], int total_cacadores) {
    int derrotados = 0;
    for (int i = 0; i < total_cacadores; i++) {
        if (cacadores[i].derrotado) derrotados++;
    }
    return derrotados >= 4;
}

bool verificar_liberacao_boss(int cacadores_derrotados) {
    return cacadores_derrotados >= 4;
}

// ========== LIMPEZA ==========

void destruir_cacador(Cacador* cacador) {
    if (!cacador) return;
    if (cacador->sprite_cacador) {
        destruir_sprite(cacador->sprite_cacador);
        cacador->sprite_cacador = NULL;
    }
}