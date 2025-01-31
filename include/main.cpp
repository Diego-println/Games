#include "raylib.h"
#include <iostream>
#include <deque>
#include "raymath.h"
/*
commad used to execute the main file: 
 g++ -Wall -I"C:/raylib_games/include" -L"C:/raylib_games/lib" main.cpp -o main -lraylib -lgdi32 -lwinmm
*/
using namespace std;
Color green ={173, 204, 96, 255};
Color darkGreen ={43, 51, 24, 255};
Color blue={0, 121, 241, 255};

int cellSize=30;
int cellCount=25;
int offSet=75;
double lastUpdateTime = 0;

bool elementDeque(Vector2 element, deque<Vector2> deque){
    for (unsigned int i = 0; i < deque.size(); i++)
    {
        if (Vector2Equals(deque[i], element))
        {
            return true;
        }
        
    }
    return false;
    
}

static bool allowMove = false;

bool evenTriggered(double interval){
    double currentTime = GetTime();
    if (currentTime-lastUpdateTime>=interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}


class Snake{

    public:
        deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        Vector2 direction = {1, 0};
        bool addSegment = false;

        void Draw(){
            for(unsigned int i=0; i<body.size(); i++){
                float x=body[i].x;
                float y=body[i].y;

                Rectangle segment = Rectangle{offSet+ x*cellSize, offSet+ y*cellSize, (float)cellSize, (float)cellSize};
                DrawRectangleRounded(segment, 0.5, 6, blue);
            }
        }
        void Update(){
            body.push_front(Vector2Add(body[0], direction));

            if (addSegment == true)
            {
                addSegment = false;
            }else{

                body.pop_back();
            }
            

        }
        void Roset(){
            body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
            direction={1, 0};
        }
};

class Food{
    public:
    //atributos
        Vector2 position={5, 6};
        Texture2D texture;

        Food(deque<Vector2> snakeBody){
            Image image = LoadImage("image.png");
            ImageResize(&image, cellSize, cellSize);
            texture = LoadTextureFromImage(image);
            UnloadImage(image);
            position = GenerateRandamPos(snakeBody);
        }
        ~Food(){
            UnloadTexture(texture);
        }

        void Draw(){

            DrawTexture(texture, offSet+ position.x*cellSize, offSet+ position.y*cellSize, WHITE);
        };

        Vector2 GenerateRamdonCell(){
            float x = GetRandomValue(0, cellCount-1);
            float y = GetRandomValue(0, cellCount-1);
            return Vector2{x, y};
        }

        Vector2 GenerateRandamPos(deque<Vector2> snakeBody){

            Vector2 position = GenerateRamdonCell();
            while (elementDeque(position, snakeBody))
            {
                position = GenerateRamdonCell();
            }
            return position;
        }

};

class Game{
    public:
        Snake snake = Snake();
        Food food = Food(snake.body);
        int score = 0;
        bool running = true;
        Sound eatSound;
        Sound edgesSound;

        Game(){
            InitAudioDevice();
            eatSound = LoadSound("eating_food.mp3");
            edgesSound = LoadSound("hit.mp3");
        }

        ~Game(){
            UnloadSound(eatSound);
            UnloadSound(edgesSound);
            CloseAudioDevice();
        }

        void Draw()
        {
            food.Draw();
            snake.Draw();
        }
        void Update()
        {
            if (running)
            {
            snake.Update();
            CheckCollisionWithFood();
            CheckCollisionWithEdges();
            CheckColisionWithTail();
            }
            
        }
        void CheckCollisionWithFood()
        {
            if (Vector2Equals(snake.body[0], food.position))
            {
                food.position = food.GenerateRandamPos(snake.body);
                snake.addSegment = true;
                score++;
                PlaySound(eatSound);
            }
            
        }

        void CheckCollisionWithEdges(){
            if (snake.body[0].x == cellCount || snake.body[0].x ==-1)
            {
                GameOver();
            }
            if (snake.body[0].y == cellCount || snake.body[0].y ==-1)
            {
                GameOver();
            }
        }

        void CheckColisionWithTail(){
            deque <Vector2> headlessBody = snake.body;
            headlessBody.pop_front();
            if (elementDeque(snake.body[0], headlessBody))
            {
                GameOver();
            }
            
        }

        void GameOver(){
            snake.Roset();
            food.position = food.GenerateRandamPos(snake.body);
            running = false;
            score=0;
            PlaySound(edgesSound);
        }
};

int main()
{
    cout<<"Starting the game"<<endl;
    InitWindow(2*offSet + cellSize*cellCount, 2*offSet + cellSize*cellCount, "Snake");

    SetTargetFPS(55);
    Game game = Game();

    while (WindowShouldClose()==false)
    {
        BeginDrawing();
        if(evenTriggered(0.2)){
            game.Update();
            allowMove = true;
        }
        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1 && allowMove)
        {
            game.snake.direction = {0, -1};
            game.running = true;
            allowMove = false;
        }
        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1 && allowMove)
        {
            game.snake.direction = {0, 1};
            game.running = true;
            allowMove = false;
        }
        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1 && allowMove)
        {
            game.snake.direction = {-1, 0};
            game.running = true;
            allowMove = false;
        }
        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1 && allowMove)
        {
            game.snake.direction = {1, 0};
            game.running = true;
            allowMove = false;
        }

        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{(float)offSet-5, (float)offSet-5, (float)cellSize*cellCount+10, (float)cellSize*cellCount+10}, 5, darkGreen);
        DrawText("Pixel Snake", offSet-5, 20, 38, darkGreen);
        DrawText(TextFormat("%i", game.score), offSet-5, offSet+cellSize*cellCount+10, 40, darkGreen);
        game.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
