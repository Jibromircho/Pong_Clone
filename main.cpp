#include "raylib.h"
#include <unistd.h>

int main(void)
{   
    //window settings and what not
    const int screenWidth = 1200;
    const int screenHeight = 800;
    const Rectangle playingField = { 0, (screenHeight - (screenHeight * 0.9)), screenWidth , (screenHeight * 0.9)};
    const Rectangle scoreField = {0, 0, screenWidth , (screenHeight * 0.1)};
    int border = 3; //border line thickness
    int gameStage = 0; //0 for paused just started or in between rouns, 1 for active ,2 for game over
    
    InitWindow(screenWidth, screenHeight, "Pong Clone");
    //score initializers
    int aiScore = 0;
    int playerScore = 0;
    float bounceCounter = 1.0f;

    //paddle settings
    Rectangle player_paddle = { 5, 400, 20, 150 };
    Rectangle ai_paddle = { (screenWidth - 25), 400, 20, 150 };
    float playerPaddleSpeed = 8.0f;
    float aiPaddleSpeedEasy = 3.5f;
    float aiPaddleSpeedMid = 5.0f;
    float aiPaddleSpeedHard = 7.0f;
    //ball settings
    float ball_size = 10.0f;
    float startingBallSpeed = 4.5f;
    Vector2 ball_speed = { startingBallSpeed, startingBallSpeed };
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
        if (IsKeyPressed(KEY_SPACE) && gameStage != 2){
            gameStage = 1;
        }
        if(IsKeyPressed(KEY_SPACE) && gameStage == 2){
            gameStage = 1;
            aiScore = 0;
            playerScore = 0;
            ball_possition = starting_ball_possition;
            ball_speed.x = startingBallSpeed;
            ball_speed.y =startingBallSpeed;
        }
        if (gameStage == 1){time_since_last_collision += GetFrameTime();
        //player movement stuff here
        if (player_paddle.y >= (playingField.y + border) && IsKeyDown(KEY_UP)) player_paddle.y -= playerPaddleSpeed;
        else if ((player_paddle.y + 150) <= screenHeight && IsKeyDown(KEY_DOWN)) player_paddle.y += playerPaddleSpeed;

        //ai movement stuff here
        if ((ai_paddle.y >= 0) && ai_paddle.y > (ball_possition.y + 5)) ai_paddle.y -= aiPaddleSpeedMid;
        else if ((ai_paddle.y + 150) <= screenHeight && ai_paddle.y < (ball_possition.y - 155)) ai_paddle.y += aiPaddleSpeedMid;
        //ball movement here
        ball_possition.x += ball_speed.x;
        ball_possition.y += ball_speed.y;

        // check for up and down wall collision
        if ((ball_possition.y >= (screenHeight - ball_size)) || (ball_possition.y <= playingField.y + ball_size)) ball_speed.y *= -1;
        
        // check for ball to paddle collision
        if (time_since_last_collision >= collision_cooldown)
        {
            {
                if ((CheckCollisionCircleRec(ball_possition,ball_size,player_paddle)) || (CheckCollisionCircleRec(ball_possition,ball_size,ai_paddle)))
                {
                    ball_speed.x *= -1;
                    time_since_last_collision = 0.0f;
                    ball_speed.x += 0.1f;
                    ball_speed.y += 0.05f;
                }
            }
        }

        //check death state
        if (ball_possition.x >= (screenWidth - ball_size))
        {
            ball_possition = starting_ball_possition;
            playerScore++;
            if (playerScore >= 3)
            {
                gameStage = 2;
            }
            else
            {
                gameStage = 0;
            }
            //for debugging
        }
        else if (ball_possition.x <= ball_size)
        {
            ball_possition = starting_ball_possition;
            aiScore++;
            if (aiScore >= 3)
            {
                gameStage = 2;
            }
            else
            {
                gameStage = 0;
            }
        }}
        //drawing stuff here
        BeginDrawing();

            ClearBackground(DARKGRAY);


            DrawText(TextFormat("%d", aiScore),(screenWidth * 0.75), (screenHeight * 0.02), 30, WHITE);
            DrawText(TextFormat("%d",playerScore),(screenWidth * 0.25), (screenHeight * 0.02), 30, WHITE);

            DrawRectangleLinesEx(playingField,border,BLACK);
            DrawRectangleLinesEx(scoreField,border,BLACK);
            DrawLineEx((Vector2){screenWidth / 2, 0}, (Vector2){screenWidth / 2, screenHeight * 0.1f}, (border * 2), BLACK);
            DrawRectangleRec(player_paddle, BLACK);
            DrawCircleV(ball_possition, ball_size,BLACK);
            DrawRectangleRec(ai_paddle, BLACK);
            if (gameStage == 0){
                DrawText("Press SPACE to start game", 250, (screenHeight / 2.2), 50, WHITE);
            }
            if (gameStage == 2 && playerScore == 3)
            {
                DrawText("Game Over", 480, (screenHeight / 4), 50, WHITE);
                DrawText("You WIN", 510, (screenHeight / 3), 50, WHITE);
                DrawText("Press SPACE to start over", 240, (screenHeight / 2.2), 50, WHITE);
            }
            if (gameStage == 2 && aiScore == 3)
            {
                DrawText("Game Over", 480, (screenHeight / 4), 50, WHITE);
                DrawText("You LOSE", 490, (screenHeight / 3), 50, WHITE);
                DrawText("Press SPACE to start over", 240, (screenHeight / 2.2), 50, WHITE);
            }
            


        EndDrawing();
    }

    CloseWindow();

    return 0;
}
