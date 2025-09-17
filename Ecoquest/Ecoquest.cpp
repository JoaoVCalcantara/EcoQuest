// Ecoquest.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>   // <- adiciona este
#include <stdbool.h>

#define SCREEN_W 800
#define SCREEN_H 600

int main() {
    if (!al_init()) return -1;
    al_install_keyboard();
    al_init_primitives_addon();
    al_init_font_addon();                 // <- inicializa o addon de fontes

    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_W, SCREEN_H);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);

    // cria a fonte uma vez
    ALLEGRO_FONT* font = al_create_builtin_font();
    if (!font) {
        // tratamento simples caso não consiga criar a fonte
        al_destroy_timer(timer);
        al_destroy_event_queue(queue);
        al_destroy_display(display);
        return -1;
    }

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));

    float x = 100, y = 100;
    const float speed = 4.0;
    const int size = 50;

    float rx = 600, ry = 200;
    float rw = 100, rh = 100;

    bool keys[4] = { false,false,false,false };
    bool running = true, redraw = true;
    bool dentro = false;

    al_start_timer(timer);

    while (running) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_W) keys[0] = true;
            if (ev.keyboard.keycode == ALLEGRO_KEY_S) keys[1] = true;
            if (ev.keyboard.keycode == ALLEGRO_KEY_A) keys[2] = true;
            if (ev.keyboard.keycode == ALLEGRO_KEY_D) keys[3] = true;
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) running = false;
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_W) keys[0] = false;
            if (ev.keyboard.keycode == ALLEGRO_KEY_S) keys[1] = false;
            if (ev.keyboard.keycode == ALLEGRO_KEY_A) keys[2] = false;
            if (ev.keyboard.keycode == ALLEGRO_KEY_D) keys[3] = false;
        }
        else if (ev.type == ALLEGRO_EVENT_TIMER) {
            if (keys[0]) y -= speed;
            if (keys[1]) y += speed;
            if (keys[2]) x -= speed;
            if (keys[3]) x += speed;

            if (x < rx + rw && x + size > rx &&
                y < ry + rh && y + size > ry) {
                dentro = true;
            }
            else {
                dentro = false;
            }

            redraw = true;
        }

        if (redraw && al_is_event_queue_empty(queue)) {
            redraw = false;

            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_filled_rectangle(rx, ry, rx + rw, ry + rh, al_map_rgb(0, 255, 0));
            al_draw_filled_rectangle(x, y, x + size, y + size, al_map_rgb(255, 0, 0));

            if (dentro) {
                // usa a fonte criada uma vez
                al_draw_text(font, al_map_rgb(255, 255, 255),
                    SCREEN_W / 2.0, 20, ALLEGRO_ALIGN_CENTRE,
                    "Voce entrou!");
            }

            al_flip_display();
        }
    }

    // limpeza
    al_destroy_font(font);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_display(display);
    return 0;
}



// Executar programa: Ctrl + F5 ou Menu Depurar > Iniciar Sem Depuração
// Depurar programa: F5 ou menu Depurar > Iniciar Depuração

// Dicas para Começar: 
//   1. Use a janela do Gerenciador de Soluções para adicionar/gerenciar arquivos
//   2. Use a janela do Team Explorer para conectar-se ao controle do código-fonte
//   3. Use a janela de Saída para ver mensagens de saída do build e outras mensagens
//   4. Use a janela Lista de Erros para exibir erros
//   5. Ir Para o Projeto > Adicionar Novo Item para criar novos arquivos de código, ou Projeto > Adicionar Item Existente para adicionar arquivos de código existentes ao projeto
//   6. No futuro, para abrir este projeto novamente, vá para Arquivo > Abrir > Projeto e selecione o arquivo. sln
