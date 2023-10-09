
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <time.h>
#include "lib/dsa/linkedList/linkedList.h"
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

typedef struct BodyPart {
    int direction;
    int x;
    int y;
} BodyPart;

typedef struct snake {
    Queue *body;
} Snake;

typedef struct apple {
    int point;
    int timer;
    int xPosition;
    int yPosition;
} Apple;



void handle_snake_movement(Snake *s, int movement) {
   BodyPart* bp = (BodyPart *)s->body->queueInterface->deTail(s->body);
   bp->direction = movement;
   s->body->queueInterface->push(s->body, (void*) bp);
}

void renderSnake(Snake *s) {
    Iterator *itr = s->body->queueInterface->createIterator(s->body);
    while (itr->interface->hasNext(itr)) {
        BodyPart *bp = (BodyPart*) itr->interface->current(itr);
        al_draw_filled_rectangle(bp->x, bp->y, bp->x + 30, bp->y + 30, al_map_rgb(255, 0, 0));
        itr->interface->next(itr);
    }
}

void renderApple(Apple *a) {
    al_draw_filled_rectangle(a->xPosition, a->yPosition, a->xPosition + 30, a->yPosition + 30, al_map_rgb(50, 205, 50));
}

void randApple(Apple *pApple, int x, int y) {
    pApple->xPosition = (rand() % (x / 30)) * 30;
    pApple->yPosition = (rand() % (y / 30)) * 30;
    pApple->timer = (rand() % 600) + 100;
    pApple->point = rand() % 3 + 1;
}


void detectCollision(Apple *a, Snake *s, int x, int y) {
    if (s->body->queueInterface-> x < a->xPosition + 30 &&
        s->x + 30 > a->xPosition &&
        s->y < a->yPosition + 30 &&
        s->y + 30 > a->yPosition) {
        s->size += a->point;
        randApple(a, x, y);

    }
}


int main(void) {

    al_init();
    al_install_keyboard();
    al_init_primitives_addon();
    srand(time(NULL));

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();

    ALLEGRO_DISPLAY *display = al_create_display(960, 600);
    int x = al_get_display_width(display);
    int y = al_get_display_height(display);

    ALLEGRO_FONT *font = al_create_builtin_font();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool redraw = true;
    bool done = false;
    ALLEGRO_EVENT event;

    int direction = 0;

    Snake *snake = malloc(sizeof(Snake));
    Apple *apple = malloc(sizeof(Apple));
    bodyPart body = {0, 0, 0};
    QueueInterface *anInterface = snake->body.interface;
    snake->body.interface->create(anInterface, &body, sizeof(bodyPart));

    apple->xPosition = 0;
    apple->yPosition = 0;
    apple->timer = 600;
    apple->point = 0;

    randApple(apple, x, y);



    #define KEYSEEN 1

    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));

    al_start_timer(timer);
    while (1) {

        al_wait_for_event(queue, &event);
        handle_snake_movement(&snake,movement);



        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:

                apple->timer--;
                detectCollision(apple, snake, x, y);

                if (apple->timer < 0) {
                    randApple(apple, x, y);
                }

                if (key[ALLEGRO_KEY_UP]) {
                    up = 6;
                    down = 0;
                    left = 0;
                    right = 0;
                }

                if (key[ALLEGRO_KEY_DOWN]) {
                    up = 0;
                    down = 6;
                    left = 0;
                    right = 0;
                }

                if (key[ALLEGRO_KEY_LEFT]) {
                    up = 0;
                    down = 0;
                    left = 6;
                    right = 0;
                }

                if (key[ALLEGRO_KEY_RIGHT]) {
                    up = 0;
                    down = 0;
                    left = 0;
                    right = 6;
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
    free(snake);

    return EXIT_SUCCESS;
}