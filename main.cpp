#include "raylib.h"
#include <unistd.h>

int main(void)
{
    const int screenWidth = 1200;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Pong Clone");

    //paddle settings
    Rectangle player_paddle = { 20, 200, 20, 300 };
    Rectangle ai_paddle = { (screenWidth - 40), 200, 20, 300 };
    float paddle_speed = 4.5f;
    //ball settings
    float ball_size = 20.0f;
    Vector2 ball_speed = { 4.0f, 4.0f };
    Vector2 starting_ball_possition { screenWidth / 2.0f, screenHeight / 2.0f };
    Vector2 ball_possition = starting_ball_possition;
    //timer variables for a bug i couldt figure out how to fix
    float collision_cooldown = 1.0f;
    float time_since_last_collision = 0.0f;

    SetTargetFPS(60);


    //main game loop
    while (!WindowShouldClose())
    {
        //debugging stuff here
        time_since_last_collision += GetFrameTime();
        //player movement stuff here
        if (player_paddle.y >= 0 && IsKeyDown(KEY_UP)) player_paddle.y -= paddle_speed;
        else if ((player_paddle.y + 300) <= screenHeight && IsKeyDown(KEY_DOWN)) player_paddle.y += paddle_speed;

        //ai movement stuff here
        if ((ai_paddle.y >= 0) && ai_paddle.y > (ball_possition.y + 50)) ai_paddle.y -= paddle_speed;
        else if ((ai_paddle.y + 300) <= screenHeight && ai_paddle.y < (ball_possition.y - 350)) ai_paddle.y += paddle_speed;
        //ball movement here
        ball_possition.x += ball_speed.x;
        ball_possition.y += ball_speed.y;

        // check for up and down wall collision
        if ((ball_possition.y >= (screenHeight - ball_size)) || (ball_possition.y <= ball_size)) ball_speed.y *= -1;
        
        // check for ball to paddle collision
        if (time_since_last_collision >= collision_cooldown)
        {
            {
                if ((CheckCollisionCircleRec(ball_possition,ball_size,player_paddle)) || (CheckCollisionCircleRec(ball_possition,ball_size,ai_paddle)))
                {
                    ball_speed.x *= -1;
                    time_since_last_collision = 0.0f;
                }
            }
        }

        //check death state
        if ((ball_possition.x >= (screenWidth - ball_size)) || (ball_possition.x <= ball_size)) 
        {
            ball_possition = starting_ball_possition;
            sleep(1);
        }
        
        //drawing stuff here
        BeginDrawing();

            ClearBackground(DARKGRAY);

            DrawRectangleRec(player_paddle, BLACK);
            DrawCircleV(ball_possition, ball_size,BLACK);
            DrawRectangleRec(ai_paddle, BLACK);
            


        EndDrawing();
    }

    CloseWindow();

    return 0;
}
