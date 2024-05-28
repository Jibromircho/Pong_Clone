#include "raylib.h"
#include <unistd.h>

#define BORDER 3
#define MAX_OPTIONS 4

int main(void)
{   
    //window settings and what not
    const int screenWidth = 1200;
    const int screenHeight = 800;
    const Rectangle playingField = { 0, (screenHeight - (screenHeight * 0.9)), screenWidth , (screenHeight * 0.9)};
    const Rectangle scoreField = {0, 0, screenWidth , (screenHeight * 0.1)};
    int gameStage = 0; //0 for just started, 1 for active ,2 for game over and 3 for in between rounds
    int selectDificulty = 0; //0 for easy, 1 for mid and 2 for hard 3 for exiting
    const char *menuOptions[MAX_OPTIONS] = { "Easy" , "Medium", "Hard", "Exit"};
    InitWindow(screenWidth, screenHeight, "Pong Clone");
    //score initializers
    int aiScore = 0;
    int playerScore = 0;
    float bounceCounter = 1.0f;

    //paddle settings
    Rectangle player_paddle = { 5, 400, 20, 150 };
    Rectangle ai_paddle = { (screenWidth - 25), 400, 20, 150 };
    float playerPaddleSpeed = 8.0f;
    float aiPaddleSpeed[3] = {3.5f , 5.0f, 7.0f };
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
        if (gameStage == 0 && IsKeyPressed(KEY_DOWN) && selectDificulty <= 3) {selectDificulty++;}
        if (gameStage == 0 && IsKeyPressed(KEY_DOWN) && selectDificulty == 4) {selectDificulty = 0;}
        if (gameStage == 0 && IsKeyPressed(KEY_UP) && selectDificulty >= 0) {selectDificulty--;}
        if (gameStage == 0 && IsKeyPressed(KEY_UP) && selectDificulty == -1) {selectDificulty = 3;}
        if (gameStage == 0 && IsKeyPressed(KEY_ENTER) && selectDificulty <= 2) {

            gameStage = 1;
        }
        else if (gameStage == 0 && selectDificulty == 3 && IsKeyPressed(KEY_ENTER))
        {
            EndDrawing();
            CloseWindow();
            return 0;
        }

        if (IsKeyPressed(KEY_ENTER) && gameStage == 3){
            gameStage = 1;
        }
        if(IsKeyPressed(KEY_ENTER) && gameStage == 2){
            gameStage = 0;
            aiScore = 0;
            playerScore = 0;
            ball_possition = starting_ball_possition;
            ball_speed.x = startingBallSpeed;
            ball_speed.y =startingBallSpeed;
        }
        if (gameStage == 1){time_since_last_collision += GetFrameTime();
        //player movement stuff here
        if (player_paddle.y >= (playingField.y + BORDER) && IsKeyDown(KEY_UP)) player_paddle.y -= playerPaddleSpeed;
        else if ((player_paddle.y + 150) <= screenHeight && IsKeyDown(KEY_DOWN)) player_paddle.y += playerPaddleSpeed;

        //ai movement stuff here
        if ((ai_paddle.y >= 0) && ai_paddle.y > (ball_possition.y + 5)) ai_paddle.y -= aiPaddleSpeed[selectDificulty];
        else if ((ai_paddle.y + 150) <= screenHeight && ai_paddle.y < (ball_possition.y - 155)) ai_paddle.y += aiPaddleSpeed[selectDificulty];
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
                gameStage = 3;
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
                gameStage = 3;
            }
        }}
        //drawing stuff here
        BeginDrawing();

            ClearBackground(DARKGRAY);


            if (gameStage == 0){
                for (int i = 0; i < MAX_OPTIONS; i++)
                {
                    if (i == selectDificulty)
                        {
                            DrawText(menuOptions[i], screenWidth / 2 - MeasureText(menuOptions[i], 20) / 2, screenHeight / 2 + i * 30, 20, RED);
                        }
                    else
                        {
                            DrawText(menuOptions[i], screenWidth / 2 - MeasureText(menuOptions[i], 20) / 2, screenHeight / 2 + i * 30, 20, BLACK);
                        }
                }
            }
            if (gameStage == 1 || gameStage == 3){
                if (gameStage == 3){
                    DrawText("Press ENTER to start round", 240, (screenHeight / 2.2), 50, WHITE);
                }
                DrawText(TextFormat("%d", aiScore),(screenWidth * 0.75), (screenHeight * 0.02), 30, WHITE);
                DrawText(TextFormat("%d",playerScore),(screenWidth * 0.25), (screenHeight * 0.02), 30, WHITE);
                DrawRectangleLinesEx(playingField,BORDER,BLACK);
                DrawRectangleLinesEx(scoreField,BORDER,BLACK);
                DrawLineEx((Vector2){screenWidth / 2, 0}, (Vector2){screenWidth / 2, screenHeight * 0.1f}, (BORDER * 2), BLACK);
                DrawRectangleRec(player_paddle, BLACK);
                DrawCircleV(ball_possition, ball_size,BLACK);
                DrawRectangleRec(ai_paddle, BLACK);
                }
            if (gameStage == 2 && playerScore == 3)
            {
                DrawText("Game Over", 480, (screenHeight / 4), 50, WHITE);
                DrawText("You WIN", 510, (screenHeight / 3), 50, WHITE);
                DrawText("Press ENTER to start over", 240, (screenHeight / 2.2), 50, WHITE);
            }
            if (gameStage == 2 && aiScore == 3)
            {
                DrawText("Game Over", 480, (screenHeight / 4), 50, WHITE);
                DrawText("You LOSE", 490, (screenHeight / 3), 50, WHITE);
                DrawText("Press ENTER to start over", 240, (screenHeight / 2.2), 50, WHITE);
            }
            


        EndDrawing();
    }

    CloseWindow();

    return 0;
}
