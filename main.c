
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <time.h>

typedef struct snake {
    int size;
    int x;
    int y;
} Snake;

typedef struct apple {
    int point;
    int timer;
    int xPosition;
    int yPosition;
} Apple;



void handle_snake_movement(Snake *s, int left, int right, int up, int down) {
    s->x += (right - left);
    s->y += (down - up);

    if (s->x >= 320) {
        s->x = 1;
    }

    if (s->y >= 200) {
        s->y = 1;
    }

    if (s->x <= 0) {
        s->x = 320;
    }

    if (s->y <= 0) {
        s->y = 200;
    }
}

void renderSnake(Snake s) {
    al_draw_filled_rectangle(s.x, s.y, s.x + 10, s.y + 10, al_map_rgb(255, 0, 0));
}

void renderApple(Apple *a) {
    al_draw_filled_rectangle(a->xPosition, a->yPosition, a->xPosition + 10, a->yPosition + 10, al_map_rgb(50, 205, 50));
}

void randApple(Apple *pApple, Snake *s, int x, int y) {
    pApple->xPosition = (rand() % (x / 10)) * 10;
    pApple->yPosition = (rand() % (y / 10)) * 10;
    pApple->timer = (rand() % 600) + 100;
    pApple->point = rand() % 3 + 1;
}


void detectCollision(Apple *a, Snake *s, int x, int y) {
    if (s->x < a->xPosition + 10 &&
        s->x + 10 > a->xPosition &&
        s->y < a->yPosition + 10 &&
        s->y + 10 > a->yPosition) {
        s->size += a->point;
        ALLEGRO_DISPLAY *display = al_get_current_display();
        randApple(a, s, x, y);

    }
}


int main(void) {

    al_init();
    al_install_keyboard();
    al_init_primitives_addon();
    srand(time(NULL));

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();

    ALLEGRO_DISPLAY *display = al_create_display(320, 200);
    int x = al_get_display_width(display);
    int y = al_get_display_height(display);

    ALLEGRO_FONT *font = al_create_builtin_font();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool redraw = true;
    bool done = false;
    ALLEGRO_EVENT event;

    int up, down, left, right;
    up = 0;
    down = 0;
    left = 0;
    right = 0;

    Snake snake = {1, 100, 100};


    Apple *apple = malloc(sizeof(Apple));

    apple->xPosition = 0;
    apple->yPosition = 0;
    apple->timer = 600;
    apple->point = 0;

    randApple(apple, &snake, x, y);



    #define KEYSEEN 1

    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));

    al_start_timer(timer);
    while (1) {

        al_wait_for_event(queue, &event);
        handle_snake_movement(&snake, left, right, up, down);



        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:

                apple->timer--;
                detectCollision(apple, &snake, x, y);

                if (apple->timer < 0) {
                    randApple(apple, &snake, x, y);
                }

                if (key[ALLEGRO_KEY_UP]) {
                    up = 2;
                    down = 0;
                    left = 0;
                    right = 0;
                }

                if (key[ALLEGRO_KEY_DOWN]) {
                    up = 0;
                    down = 2;
                    left = 0;
                    right = 0;
                }

                if (key[ALLEGRO_KEY_LEFT]) {
                    up = 0;
                    down = 0;
                    left = 2;
                    right = 0;
                }

                if (key[ALLEGRO_KEY_RIGHT]) {
                    up = 0;
                    down = 0;
                    left = 0;
                    right = 2;
                }


                if (key[ALLEGRO_KEY_ESCAPE]) {
                    done = true;
                }

                for (int i = 0; i < ALLEGRO_KEY_MAX; i++) {
                    key[i] &= KEYSEEN;
                }

                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] = 0;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = KEYSEEN;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if (done)
            break;

        if (redraw && al_is_event_queue_empty(queue)) {


            al_clear_to_color(al_map_rgb(12, 55, 100));
            al_draw_textf(font, al_map_rgb(255, 255, 255), 1, 1, 0, "X: %d Y: %d score: %d", snake.x, snake.y, snake.size);
            al_draw_textf(font, al_map_rgb(255, 255, 255), 1, 10, 0, "Apple-x: %d Apple-y: %d Timer: %d", apple->xPosition, apple->yPosition, apple->timer);
            al_draw_filled_rectangle(40, 240, 50, 250, al_map_rgb(0, 205, 200));
            renderSnake(snake);
            renderApple(apple);
            al_flip_display();
            redraw = false;
        }
    }

    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    free(apple);

    return 0;
}