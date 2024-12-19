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

    const int sizeOfNebulae{6};
    AnimData nebulae[sizeOfNebulae]{};

    for (int i = 0; i < sizeOfNebulae; i++) {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width/8;
        nebulae[i].rec.height = nebula.height/8;
        nebulae[i].pos.y = windowDimensions[1] - nebula.height/8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 1.0/16.0;
        nebulae[i].pos.x = windowDimensions[0] + (i * 300);
    }

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

        // Update position of each nebula
        for (int i = 0; i < sizeOfNebulae; i++) {
            nebulae[i].pos.x += nebVel * deltaTime;
        }

        // update position; used to be posY
        scarfyData.pos.y += velocity * deltaTime;


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


        for (int i = 0; i < sizeOfNebulae; i++) {
            // UPDATE nebula animation frame
            nebulae[i].runningTime += deltaTime;
            if (nebulae[i].runningTime >= nebulae[i].updateTime) {
                nebulae[i].runningTime = 0.0;
                nebulae[i].rec.x = nebulae[i].frame * nebulae[i].rec.width;
                nebulae[i].frame++;
                nebulae[i].frame %= 8;
            }
        }


        for (int i = 0; i < sizeOfNebulae; i++) {
            // Draw nebula
            DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, RED);
        }


        // Draw Player aka Scarfy
        DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        

        // End Drawing
        EndDrawing();
    }


    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();
}