
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
#define SCREEN_X 960
#define SCREEN_Y 600

typedef struct BodyPart {
    int direction;
    int x;
    int y;
} BodyPart;

typedef struct snake {
    Queue *body;
    int score;
    int speed;
    bool upSeed;
} Snake;

typedef struct apple {
    int point;
    int timer;
    int xPosition;
    int yPosition;
} Apple;

bool gameOver = false;

void handle_snake_movement(Snake *s, int movement) {
   int size = s->body->queueInterface->size(s->body);
   BodyPart* bp = (BodyPart *)s->body->queueInterface->deTail(s->body);
   BodyPart* head = (BodyPart *) s->body->queueInterface->pop(s->body);


   if (size == 1) {
       head = bp;
   }

   bp->x = head->x;
   bp->y = head->y;

   if (s->score < 30) {
       s->speed  = 10;
   }

   if (s->score > 40) {
       s->speed = 13;
   }

   if (s-> score > 60) {
       s->speed = 16;
   }

   if (s->score > 80) {
       s->speed = 19;
   }

   if (s->score > 100) {
       s->speed = 21;
   }

   if (s->score > 120) {
       s->speed = 23;
   }

   if (s->score > 150) {
       s-> speed = 25;
   }

   if (s->score > 200) {
       s->speed = 27;
   }

   if (s->score > 300) {
       s->speed = 30;
   }




   if (movement == UP) {
       bp-> y -= s->speed;
   } else if (movement == DOWN) {
       bp-> y += s->speed;
   } else if (movement == LEFT) {
       bp-> x -= s->speed;
   } else if (movement == RIGHT) {
       bp-> x += s->speed;
   }

   if (bp->x > SCREEN_X) {
       bp->x = 15;
   }

   if (bp->x < -15) {
       bp->x = SCREEN_X - 15;
   }

   if (head->y < -15) {
       bp->y = SCREEN_Y - 15;
   }

   if (head->y > SCREEN_Y) {
       bp->y = -15;
   }

   if (size > 1) {
       s->body->queueInterface->front(s->body, head, sizeof(BodyPart));
   }
   s->body->queueInterface->front(s->body, bp, sizeof(BodyPart));
}

void renderSnake(Snake *s) {
    Iterator *itr = s->body->queueInterface->createIterator(s->body, sizeof(BodyPart));
    while (itr->interface->current(itr) != NULL) {
        BodyPart *bp = (BodyPart *) itr->interface->current(itr);
        al_draw_filled_rectangle(bp->x, bp->y, bp->x + 30, bp->y + 30, al_map_rgb(255, 0, 0));
        itr->interface->next(itr);
    };
}

void renderApple(Apple *a) {
    al_draw_filled_rectangle(a->xPosition, a->yPosition, a->xPosition + 30, a->yPosition + 30, al_map_rgb(50, 205, 50));
}

void randApple(Apple *pApple) {
    pApple->xPosition = (rand() % (SCREEN_X / 30)) * 30;
    pApple->yPosition = (rand() % (SCREEN_Y / 30)) * 30;
    pApple->timer = (rand() % 600) + 100;
    pApple->point = rand() % 3 + 1;
}

void hitSelf(Snake *s) {
    if (s->body->queueInterface->size(s->body) < 5) {
        return;
    }

    Iterator *itr = s->body->queueInterface->createIterator(s->body, sizeof(BodyPart));
    BodyPart head = *((BodyPart*)itr->interface->current(itr));
    itr->interface->next(itr);
    BodyPart *body = (BodyPart *)itr->interface->current(itr);

    while (body != NULL) {

        if (head.x < body->x + s->speed && head.x +s->speed > body->x &&
            head.y < body->y + s->speed && head.y + s->speed > body->y) {
            gameOver = true;
        }
        itr->interface->next(itr);
        body = itr->interface->current(itr);
    }
}

void detectCollision(Apple *a, Snake *s, int movement) {
    Iterator *itr = s->body->queueInterface->createIterator(s->body, sizeof(BodyPart));
    BodyPart *head = itr->interface->current(itr);
    if (head->x < a->xPosition + 30 &&
        head->x + 30 > a->xPosition &&
        head->y < a->yPosition + 30 &&
        head->y + 30 > a->yPosition) {
        BodyPart* tail = (BodyPart *)s->body->queueInterface->deTail(s->body);
        BodyPart* bp = malloc(sizeof(BodyPart));
        BodyPart* higherScore;


        if (tail->direction == UP) {
            bp->direction = movement;
            bp->x = tail->x;
            bp->y = tail->y - s->speed;
            if (s->score > 30) {
                higherScore = malloc(sizeof(BodyPart));
                higherScore->direction = movement;
                higherScore->x = bp->x;
                higherScore->y = bp->y - s->speed;
                bp = higherScore;
            }
        } else if (tail->direction == DOWN) {
            bp->direction = movement;
            bp->x = tail->x;
            bp->y = tail->y + s->speed;
            if (s->score > 30) {
                higherScore = malloc(sizeof(BodyPart));
                higherScore->direction = movement;
                higherScore->x = bp->x;
                higherScore->y = bp->y + s->speed;
                bp = higherScore;

            }
        } else if (tail->direction == LEFT) {
            bp->direction = movement;
            bp->x = tail->x - s->speed;
            bp->y = tail->y;
            if (s->score > 30) {
                higherScore = malloc(sizeof(BodyPart));
                higherScore->direction = movement;
                higherScore->x = bp->x - s->speed;
                higherScore->y = bp->y;
                bp = higherScore;
            }
        } else if (tail->direction == RIGHT) {
            bp->direction = movement;
            bp->x = tail->x + s->speed;
            bp->y = tail->y;
            if (s->score > 30) {
                higherScore = malloc(sizeof(BodyPart));
                higherScore->direction = movement;
                higherScore->x = bp->x + s->speed;
                higherScore->y = bp->y;
            }
        }

        s->body->queueInterface->push(s->body, tail, sizeof(BodyPart));
        s->body->queueInterface->push(s->body, bp, sizeof(BodyPart));
        if (s->score > 30) {
            s->body->queueInterface->push(s->body, higherScore, sizeof(BodyPart));
        }
        free(bp);
        s->score += a->point;
        randApple(a);
    }
}


int main(void) {

    al_init();
    al_install_keyboard();
    al_init_primitives_addon();
    srand(time(NULL));

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();

    ALLEGRO_DISPLAY *display = al_create_display(SCREEN_X, SCREEN_Y);
    ALLEGRO_FONT *font = al_create_builtin_font();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool redraw = true;
    bool done = false;
    ALLEGRO_EVENT event;

    int usrMovement = 1;

    Snake *snake = malloc(sizeof(Snake));
    snake->speed = 7;
    snake->score = 0;
    Apple *apple = malloc(sizeof(Apple));
    BodyPart body = {1, 0, 0};
    Queue *sQueue = malloc(sizeof(Queue));
    Queue ref = createQueue(&body, sizeof(BodyPart));
    *sQueue = ref;
    snake->body = sQueue;
    apple->xPosition = 0;
    apple->yPosition = 0;
    apple->timer = 600;
    apple->point = 1;

    randApple(apple);

    #define KEYSEEN 1

    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));

    al_start_timer(timer);
    while (1) {
        al_wait_for_event(queue, &event);
        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:

                apple->timer--;

                if (apple->timer < 0) {
                    randApple(apple);
                }

                if (key[ALLEGRO_KEY_UP]) {
                    if (usrMovement == DOWN) {
                        usrMovement = DOWN;
                    } else {
                        usrMovement = UP;
                    }
                }


                if (key[ALLEGRO_KEY_DOWN]) {
                    if (usrMovement == UP) {
                        usrMovement = UP;
                    } else {
                        usrMovement = DOWN;

                    }
                }

                if (key[ALLEGRO_KEY_LEFT]) {
                    if (usrMovement == RIGHT) {
                        usrMovement = RIGHT;
                    } else {
                        usrMovement = LEFT;
                    }
                }

                if (key[ALLEGRO_KEY_RIGHT]) {
                    if (usrMovement == LEFT) {
                        usrMovement = LEFT;
                    } else {
                        usrMovement = RIGHT;
                    }
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
            hitSelf(snake);
            if (gameOver) {
                char *strin;
                asprintf(&strin, "game over!");
                al_clear_to_color(al_map_rgb(12, 55, 100));
                renderSnake(snake);
                al_draw_text(font, al_map_rgb(255, 255, 255), 10, 100, 0, strin);
                al_flip_display();
                redraw = false;
            } else {

                al_clear_to_color(al_map_rgb(12, 55, 100));
                renderSnake(snake);

                char *strin;
                asprintf(&strin, "Snake Score:%d  Snake Speed: %d", snake->score, snake->speed);
                al_draw_text(font, al_map_rgb(255, 255, 255), 10, 100, 0, strin);
                detectCollision(apple, snake, usrMovement);
                handle_snake_movement(snake, usrMovement);
                renderApple(apple);
                al_flip_display();
                redraw = false;
            }
        }
    }

    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    free(apple);
    free(snake);
    free(sQueue);

    return EXIT_SUCCESS;
}