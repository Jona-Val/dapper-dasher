#include "raylib.h"

struct AnimData {
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

int main() {
    int windowDimensions[] = {512, 380};
    // int windowWidth{512}; int windowHeight{380}; 
    int frames_ps{60};
    // Initialize Window
    InitWindow(windowDimensions[0], windowDimensions[1], "Dasher");

    // acceleration due to gravity (pixels/frame^2)
    const int gravity{1'000};

    // Player Character
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width/6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0; scarfyData.rec.y = 0;
    scarfyData.pos.x = windowDimensions[1]/2 - scarfyData.rec.width/2;
    scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;
    scarfyData.frame = 0; scarfyData.updateTime = 1.0/12.0;
    scarfyData.runningTime = 0;

    // Nebula aka hazards to jump
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    // animData struct for the nebula
    AnimData nebData{ 
        {0.0, 0.0, nebula.width/8, nebula.height/8},                    // Rectangle Rec
        {windowDimensions[0], windowDimensions[1] - nebula.height/8},   // Vector2 Pos
        0,                                                              // int frame
        1.0/12.0,                                                       // float updateTime
        0.0};                                                           // float runningTime

    // animData struct for the nebula
    AnimData neb2Data{ 
        {0.0, 0.0, nebula.width/8, nebula.height/8},                            // Rectangle Rec
        {windowDimensions[0] + 300, windowDimensions[1] - nebula.height/8},     // Vector2 Pos
        0,                                                                      // int frame
        1.0/16.0,                                                               // float updateTime
        0.0};                                                                   // float runningTime

    AnimData nebulae[2]{ nebData, neb2Data };

    // nebula X velocity (pixels/second) frame independent
    int nebVel{-200};

    int velocity{0};


    // Player aerial status
    bool isInAir{false};


    // Jump velocity (pixels/second)
    const int jumpVel{-600};


    SetTargetFPS(frames_ps);


    while(!WindowShouldClose()) {
        // Time since last frame
        const float deltaTime{GetFrameTime()};

        //Start Drawing
        BeginDrawing();
        ClearBackground(WHITE);

        // perform ground check; scarfy used to be posY, height
        if (scarfyData.pos.y >= windowDimensions[1] - scarfyData.rec.height) {
            // rect is on ground; reset velocity
            velocity = 0;
            isInAir = false;
        }
        else {
            // rect is in the air; apply gravity
            velocity += gravity * deltaTime;
            isInAir = true;
        }

        // Jump Check
        if (IsKeyPressed(KEY_SPACE) && !isInAir) {
            velocity += jumpVel;
        }

        // update position; used to be posY
        scarfyData.pos.y += velocity * deltaTime;

        // update nebula position
        nebulae[0].pos.x += nebVel * deltaTime;

        // update nebula position
        nebulae[1].pos.x += nebVel * deltaTime;

        // UPDATE scarfy's animation frame
        if (!isInAir) {
            // Update running time.
            scarfyData.runningTime += deltaTime;
            if (scarfyData.runningTime >= scarfyData.updateTime) {
                scarfyData.runningTime = 0.0;

                // update animation frame
                scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;
                // Update the frame
                scarfyData.frame++;
                // Reset the frame
                scarfyData.frame %= 6;
            }
        }

        // UPDATE nebula animation frame
        nebulae[0].runningTime += deltaTime;
        if (nebulae[0].runningTime >= nebulae[0].updateTime) {
            nebulae[0].runningTime = 0.0;
            nebulae[0].rec.x = nebulae[0].frame * nebulae[0].rec.width;
            nebulae[0].frame++;
            nebulae[0].frame %= 8;
        }

        // UPDATE nebula animation frame
        nebulae[1].runningTime += deltaTime;
        if (nebulae[1].runningTime >= nebulae[1].updateTime) {
            nebulae[1].runningTime = 0.0;
            nebulae[1].rec.x = nebulae[1].frame * nebulae[1].rec.width;
            nebulae[1].frame++;
            nebulae[1].frame %= 8;
        }

        // Draw Player aka Scarfy
        DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);

        // Draw nebula
        DrawTextureRec(nebula, nebulae[0].rec, nebulae[0].pos, RED);

        // Draw nebula
        DrawTextureRec(nebula, nebulae[1].rec, nebulae[1].pos, GREEN);
        

        // End Drawing
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();
}