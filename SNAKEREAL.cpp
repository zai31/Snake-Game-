 #include <iostream>
#include "raylib.h"
#include <deque>
#include <raymath.h>
#include <algorithm>



using namespace std;

Color mauve= {239, 207, 227,255};
Color rose = { 206, 195, 200,255 };

int offset = 75;
int cellnum =30;
int cellsize = 25;

class Food
{
public:

    Vector2 foodp;
    Texture2D texture;
    Food(deque<Vector2> snakeBody)
    {
        Image image = LoadImage("C:\\Users\\zaina\\source\\repos\\SNAKEREAL\\graphics\\apple .png");//copy as path+\*/
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        foodp = generateRandPos();
    }
    void Drawfood()
    {
        DrawTexture(texture, offset + foodp.x * cellsize, offset + foodp.y * cellsize, WHITE);

    }

    Vector2 generateRandPos()
    {
        float x = GetRandomValue(1, cellsize);
        float y = GetRandomValue(1, cellsize);
          return Vector2{ x,y };
    }

    Vector2 randomfoodpos(deque <Vector2> snakeBody)
    {
        foodp = generateRandPos();
        while (checkNewFoodPos( snakeBody,foodp))
        {
            foodp = generateRandPos();
        }
        return foodp;
    }

    

    bool checkNewFoodPos(deque <Vector2> snakebody, Vector2 pos)
    {
        for (int i = 0;i < (int)snakebody.size();i++)
        {
            if (Vector2Equals(snakebody[i], pos))
            {
                return true;
            }
            
        }
    return false;

        /*deque<Vector2>::iterator itr;
        itr = find(snakebody.begin(), snakebody.end(), pos); //cant because its 2dvector
        if (itr != snakebody.end())//found
            return true;
            return false;*/


    }

    ~Food()
    {
        UnloadTexture(texture);

    }
   

   



};



class Snake
{
public:
    

    Vector2 coord = { 0,1 };
    deque<Vector2>body = { Vector2{6,6},Vector2{7,6},Vector2{8,6} };


    void DrawSnake()
    { 
        
        
        for (int i = 0;i < (int)body.size(); i++)
        {
            Rectangle rec = Rectangle{ offset+body[i].x * cellsize, offset + body[i].y * cellsize,(float)cellsize,(float)cellsize };
            DrawRectangleRounded(rec,0.6,1,WHITE);
            
        }
       
        

      
    }

    void SnakeUpdate()
    {


        body.pop_back();
        body.push_front(Vector2Add(body[0], coord));


    }

    void reset()
    {
       body = { Vector2{6,6},Vector2{7,6},Vector2{8,6} };
       coord = { 0,1 };

    }
    

};

class Game
{
   public: 
    Food food = Food(snake.body);
    Snake snake = Snake();
    Music eating;
    Music dying;
    bool display = true;
    int score = 0;


    Game()
    {
        InitAudioDevice();
        eating = LoadMusicStream("C:\\Users\\zaina\\source\\repos\\SNAKEREAL\\sounds\\music_food.mp3");
        dying= LoadMusicStream("C:\\Users\\zaina\\source\\repos\\SNAKEREAL\\sounds\\die.wav");

    }

    ~Game()
    {
        UnloadMusicStream(eating);
        UnloadMusicStream(dying);
        CloseAudioDevice();
    }

    void Draw()
    {
        food.Drawfood();
        snake.DrawSnake();

    }

    double updateT = 0;
    bool eventT(double updateRate)//for controlling snake movement (60/sec is too fast)
    {
        double currentT = GetTime();
        if (currentT - updateT >= updateRate)
        {
            updateT = currentT;
            return true;

        }
        return false;
    }

    
    void update()
    {
        if(display==true)
        { 
        checkFoodCollision();
        CheckCollisionWithTail();
        CheckCollisionWithEdges();
        snake.SnakeUpdate();
        }
    }
    

   
    void snakeincrese()
    {
        
            snake.body.push_back(Vector2Add(snake.body[snake.body.size() - 1], snake.coord));//(int)
        
    }
      

    void checkFoodCollision()

    {
        if (Vector2Equals(snake.body[0], food.foodp))
        { 
            PlayMusicStream(eating);
            food.foodp=food.randomfoodpos(snake.body);
           snakeincrese();
           score++;
        }
    }

   void CheckCollisionWithEdges()
    {
        if (snake.body[0].x == cellnum|| snake.body[0].x == -1)
        {
            GameOver();
            score = 0;
          
        }
        if (snake.body[0].y == cellnum || snake.body[0].y == -1)
        {
            GameOver();
            score = 0;
           
        }
    }

   void CheckCollisionWithTail()
   {
       deque<Vector2> headlessBody = snake.body;
       Vector2 head = headlessBody[0];
       headlessBody.pop_front();
       if (bodycollision(head, headlessBody))
       { 
          
           GameOver();
           score = 0;
          
       }
   }

   bool bodycollision(Vector2 pos,deque <Vector2> snakebody)
   {
       for (int i = 0;i < (int)snakebody.size();i++)
       {
           if (Vector2Equals(snakebody[i], pos))
           {
               return true;
           }

       }
       return false;
   }

    void GameOver()
    {
        PlayMusicStream(dying);
        snake.reset();
        food.foodp = food.generateRandPos();
        display = false;
    }

   
   /* void togglewindow(int h, int w)
    {
        //get curerent smallmon size
        int monitor = GetCurrentMonitor();
        if (IsWindowFullscreen())
        {
            SetWindowSize(w, h);//resize window to it
                ToggleFullscreen();//tcrpings accurhen toglle //otherwise the monitor will togle to fill the window and
        }
        else 
        {
          
            SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
        }

    }
    */
    
};



int main()
{
   

    

    InitWindow(2*offset+cellnum*cellsize, 2 * offset + cellnum * cellsize, "snake Game"); // game window+invisible grid
    SetTargetFPS(60);

     Game game = Game();
   
    
    while (!WindowShouldClose())
    {
      /*  if (IsKeyPressed(KEY_SPACE))
            int display = GetCurrentMonitor();
            game.togglewindow(cellnum,cellsize);
*/
        BeginDrawing();
        ClearBackground(mauve);
        DrawRectangleLinesEx(Rectangle{ (float)offset - 5, (float)offset - 5, (float)cellsize * cellnum + 10, (float)cellsize * cellnum + 10 }, 5, WHITE);
        DrawText("Snake Game", offset - 5, 20, 40, WHITE);
        DrawText("SCORE:", offset+545, 20, 40, DARKGRAY);
        DrawText(TextFormat("%i",game.score), offset+700, 20, 40, DARKGRAY);
        game.Draw();

                if (game.eventT(0.15))
                { 
                    game.update();
                  
                }
                if (IsKeyDown(KEY_RIGHT) && game.snake.coord.x != -1)
                {
                   
                    game.snake.coord = { 1,0 }; 
                    game.display = true;
                }
                if (IsKeyDown(KEY_LEFT) && game.snake.coord.x != 1)
                {
                   
                    game.snake.coord = { -1,0 };
                    game.display = true;
                }
                if (IsKeyDown(KEY_UP) && game.snake.coord.y != 1)
                {
                    
                    game.snake.coord = { 0,-1 };
                    game.display = true;

                }
                if (IsKeyDown(KEY_DOWN) && game.snake.coord.y != -1)
                {
                   
                    game.snake.coord = { 0,1 }; 
                    game.display = true;
                
                }
                

    
        EndDrawing();
    }

    CloseWindow();
    return 0;
}


