#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/keyboard.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>


void desenhar_quadrados(int width,int height) {
		int margem = width * 0.125;   // tamanho proporcional (12.5% da tela)
		int tamanho = width * 0.125;  // quadrados escalando com a largura

		// canto superior esquerdo
		al_draw_filled_rectangle(0, 0, tamanho, tamanho, al_map_rgb(255, 255, 255));

		// canto superior direito
		al_draw_filled_rectangle(width - tamanho, 0, width, tamanho, al_map_rgb(255, 255, 255));

		// canto inferior esquerdo
		al_draw_filled_rectangle(0, height - tamanho, tamanho, height, al_map_rgb(255, 255, 255));

		// canto inferior direito
		al_draw_filled_rectangle(width - tamanho, height - tamanho, width, height, al_map_rgb(255, 255, 255));
	}

bool circle_rect_collision(float cx, float cy, float r,
	float rx, float ry, float rw, float rh) {
	// encontra o ponto mais próximo do centro do círculo dentro do retângulo
	float closestX = fmax(rx, fmin(cx, rx + rw));
	float closestY = fmax(ry, fmin(cy, ry + rh));
	float dx = cx - closestX;
	float dy = cy - closestY;
	return (dx * dx + dy * dy) <= (r * r);
}

int main() {
	al_init();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_install_keyboard();
	al_init_primitives_addon();


	ALLEGRO_DISPLAY* display = al_create_display(1280, 720);
	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0); // <-- FPS
	ALLEGRO_FONT* fonte = al_load_font("./font.ttf", 50, 0);
	ALLEGRO_BITMAP* bg = al_load_bitmap("./bg.png");
	ALLEGRO_BITMAP* dragao = al_load_bitmap("./dragon.png");

	al_set_window_title(display, "Jogasso Triple AAA");
	ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_start_timer(timer); // inicia o timer

	//Variaveis
	const int width = al_get_display_width(display);
	const int height = al_get_display_height(display);

	float frame = 0.f;
	int current_frame_y = 161;
	
	float velocidade = 8.0f;
	float x = width / 2.0f;
	float y = height / 2.0f;
	float raio = 50.0f;

	bool rodando = true;
	bool ativado = false;
	bool desenhar = false;

	

	//O que aparecerá na tela
	while (rodando) {
		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue, &event);

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			rodando = false;
		}
		else if (event.type == ALLEGRO_EVENT_TIMER) {
			ALLEGRO_KEYBOARD_STATE estado;
			al_get_keyboard_state(&estado);

			if (al_key_down(&estado, ALLEGRO_KEY_W)) {
			
				y -= velocidade;
			}
			if (al_key_down(&estado, ALLEGRO_KEY_S)) {
				
				y += velocidade; 
			}
			if (al_key_down(&estado, ALLEGRO_KEY_A)) {
				 
				 x -= velocidade;

			}
			if (al_key_down(&estado, ALLEGRO_KEY_D)) {
				
				x += velocidade;

			}

			// limitar posição do círculo
			if (x < raio) x = raio;
			if (x > width - raio) x = width - raio;
			if (y < raio) y = raio;
			if (y > height - raio) y = height - raio;

			desenhar = true;
		}
		
		else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			if (event.keyboard.keycode == ALLEGRO_KEY_E) {

				int margem = width * 0.125;
				int tamanho = width * 0.125;
				float q1 = circle_rect_collision(x, y, raio, 0, 0, tamanho, tamanho);
				float q2 = circle_rect_collision(x, y, raio, width - tamanho, 0, width, tamanho);
				float q3 = circle_rect_collision(x, y, raio, 0, height - tamanho, tamanho, height);
				float q4 = circle_rect_collision(x, y, raio, width - tamanho, height - tamanho, width, height);

				if (q1 || q2 || q3 || q4) {
					ativado = true;
				}
				else ativado = false;
			}
		}

		frame += 0.2f;
		if (frame > 3) {
			frame -= 3;

			current_frame_y += 161;
			if (current_frame_y > 161 * 3)
				current_frame_y = 0;
		}
	
		// --- parte do desenho ---
		if (desenhar && al_is_event_queue_empty(event_queue)) {
			desenhar = false;
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_bitmap(bg, 0, 0, 0);

			desenhar_quadrados(width,height);
			al_draw_filled_circle(x, y, raio, al_map_rgb(0, 0, 255));

			if (ativado) {
			
				al_draw_bitmap_region(dragao,191 * (int)frame,current_frame_y,191,161,640,360,0);
			}
			
			al_flip_display();
		}
	}
	al_destroy_bitmap(dragao);
	al_destroy_bitmap(bg);
	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	al_destroy_font(fonte);

	return 0;
}

