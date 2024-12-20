#include "raylib.h"

struct AnimData {
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround (AnimData data, int windowHeight) {
    return data.pos.y >= windowHeight - data.rec.height;

}

AnimData* updateAnimData (AnimData *data, float dT, int maxFrame) {
    // update running time
    data->runningTime += dT;
    if (data->runningTime >= data->updateTime) {
        data->runningTime = 0.0;
        // update animation frame
        data->rec.x = data->frame * data->rec.width;
        data->frame++;
        data->frame %= maxFrame;
    }

    return data;
}

void updateAnimData(AnimData *data) {
    data->pos.x = 2; // Then just use function with the & operator, no need to create excess copies
}

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
    scarfyData.pos.x = windowDimensions[0]/2 - scarfyData.rec.width/2;
    scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;
    scarfyData.frame = 0; scarfyData.updateTime = 1.0/12.0;
    scarfyData.runningTime = 0.0;

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

    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX{};
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX{};
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX{};


    // Player aerial status
    bool isInAir{};


    // Jump velocity (pixels/second)
    const int jumpVel{-600};


    SetTargetFPS(frames_ps);


    while(!WindowShouldClose()) {
        // Time since last frame
        const float deltaTime{GetFrameTime()};

        //Start Drawing
        BeginDrawing();
        ClearBackground(WHITE);


        // This looks like it should be a function ergo TODO
        bgX -= 20 * deltaTime;
        if (bgX <= -background.width * 2) {
            bgX = 0.0;
        }
        mgX -= 40 * deltaTime;
        if (mgX <= -midground.width * 2) {
            mgX = 0.0;
        }
        fgX -= 80 * deltaTime;
        if (fgX <= -foreground.width * 2) {
            fgX = 0.0;
        }
        
        // Draw Background
        Vector2 bg_1Pos{bgX, 0.0};
        DrawTextureEx(background, bg_1Pos, 0.0, 2.0, WHITE);
        Vector2 bg_2Pos{bgX + background.width*2, 0.0};
        DrawTextureEx(background, bg_2Pos, 0.0, 2.0, WHITE);

        Vector2 mg_1Pos{mgX, 0.0};
        DrawTextureEx(midground, mg_1Pos, 0.0, 2.0, WHITE);
        Vector2 mg_2Pos{mgX + midground.width * 2, 0.0};
        DrawTextureEx(midground, mg_2Pos, 0.0, 2.0, WHITE);

        Vector2 fg_1Pos{fgX, 0.0};
        DrawTextureEx(foreground, fg_1Pos, 0.0, 2.0, WHITE);
        Vector2 fg_2Pos{fgX + foreground.width * 2, 0.0};
        DrawTextureEx(foreground, fg_2Pos, 0.0, 2.0, WHITE);

        // perform ground check; scarfy used to be posY, height
        if (isOnGround(scarfyData, windowDimensions[1])) {
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
            updateAnimData(&scarfyData, deltaTime, 6);
        }


        for (int i = 0; i < sizeOfNebulae; i++) {
            // UPDATE nebula animation frame
            updateAnimData(&nebulae[i], deltaTime, 8);
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
    UnloadTexture(foreground);
    UnloadTexture(midground);
    UnloadTexture(background);
    CloseWindow();
}