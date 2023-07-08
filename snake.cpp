#include <cstdlib>
#include <thread>
#include <vector>
#include <random>
#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>


using namespace std;
SDL_Window* window;
SDL_Renderer* renderer;
const int blockSize = 40;
bool running;
int heights, widths;

struct Coord {
    int x;
    int y;
};

enum yDirection {
    UP = -1,
    yNONE = 0,
    DOWN = 1
};

enum xDirection {
    LEFT = -1,
    xNONE = 0,
    RIGHT = 1
};

struct Direction {
    xDirection x;
    yDirection y;
};
vector<Coord> snake;
vector<Direction> dir;
int score = 0;
Coord food;

void drawFood() {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect rect{food.x,food.y,blockSize-2,blockSize-2};
    SDL_RenderFillRect(renderer, &rect);
}

void drawSnake() {
    for(int i = 0; i < snake.size(); i++) {
        SDL_SetRenderDrawColor(renderer, 255, 150, 0, 255);
        SDL_Rect rect{snake[i].x,snake[i].y,blockSize-2,blockSize-2};
        SDL_RenderFillRect(renderer, &rect);
    }
}

void renderThread() {

    while(running){
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        drawFood();
        drawSnake();
        SDL_RenderPresent(renderer);
    } 
}

void setFood() {
    food = {(rand()%(widths/blockSize))*blockSize,(rand()%(heights/blockSize))*blockSize};
}

void endGame() {
    snake.clear();
    dir.clear();
    score = 0;
    running = false;
}

void resetGame() {
    snake = {{0,0}};
    dir = {{xNONE,yNONE}};
    setFood();
    score = 0;
    running = true;
}

bool checkFoodEaten() {
    if(snake[0].x == food.x && snake[0].y == food.y) {
        setFood();
        score++;
        return true;
    }
    return false;
}

bool checkCollision() {
    if(snake[0].x < 0 || snake[0].x >= widths || snake[0].y < 0 || snake[0].y >= heights) {
        return true;
    }
    for(int i = 1; i < snake.size(); i++) {
        if(snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            return true;
        }
    }
    return false;
}

void moveSnake() {
    snake.insert(snake.begin(),
            {snake[0].x + (blockSize * dir[dir.size() - 1].x), snake[0].y + (blockSize * dir[dir.size() - 1].y)}
        );
    if (!checkFoodEaten()) {
        snake.pop_back();
    }
}

void updateThread() {
    while(running){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                running = false;
            }
            switch(event.key.keysym.sym){
                case SDLK_UP:
                case SDLK_w:
                    if(dir[0].y == yNONE){
                        dir.insert(dir.begin(),Direction{xNONE,UP});
                        dir.pop_back();
                    }
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    if(dir[0].y == yNONE){
                        dir.insert(dir.begin(),Direction{xNONE,DOWN});
                        dir.pop_back();
                    }
                    break;
                case SDLK_LEFT:
                case SDLK_a:
                    if(dir[0].x == xNONE){
                        dir.insert(dir.begin(),Direction{LEFT,yNONE});
                        dir.pop_back();
                    }
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    if(dir[0].x == xNONE){
                        dir.insert(dir.begin(),Direction{RIGHT,yNONE});
                        dir.pop_back();
                    }
                    break;
            }
        }
        moveSnake();
        if (checkCollision()){
            endGame();
            resetGame();
        }
        SDL_Delay(250);
    }
}

int main (int argc, char *argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    srand(time(NULL));
    heights = 800, widths = 1200;
    window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, widths, heights, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    resetGame();

    thread render(renderThread);
    thread update(updateThread);
    render.join();
    update.join();

    return 0;
}
