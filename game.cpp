#include "C:/raylib/raylib/src/raylib.h"
#include <iostream>
#include <deque>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_set>
#include <raymath.h>
#include <cfloat>
#include <fstream>
using namespace std;

const int cellsize = 30;
const int numberofcells = 25;
const int offset = 75;
Music bg;
Sound eat, hit, button;
Color green = {173, 204, 96, 255};
Color darkgreen = {0, 100, 0, 255};
Color aiBackground = {70, 130, 180, 255};
const int screenWidth = 2 * offset + cellsize * numberofcells;
const int screenHeight = 2 * offset + cellsize * numberofcells;

bool powerUpActive = false;
float powerUpTimer = 0;
const float powerUpDuration = 5.0f;
Vector2 powerUpPosition;

const int btnWidth = 200; // JAYESHA
const int btnHeight = 50;
const int btnX = screenWidth / 2 - btnWidth / 2;
bool buttonClicked = false;

Rectangle easybutton = {btnX, 250, btnWidth, btnHeight};
Rectangle mediumbutton = {btnX, 320, btnWidth, btnHeight};
Rectangle hardbutton = {btnX, 390, btnWidth, btnHeight}; // JAYESHA

bool menuScreen = true;
bool gameOverScreen = false;
double lastupdatetime = 0;
bool aiMode = false;

int growthFoodCount = 0;
int shrinkFoodCount = 0;
int negativeFoodCount = 0;
int highScore = 0;
bool paused = false;
double gameStartTime = 0.0;

int loadHighScore(const string &filename) // ZAHRA
{
    ifstream file(filename);
    int score = 0;
    if (file.is_open())
    {
        file >> score;
        file.close();
    }
    return score;
}
void saveHighScore(const string &filename, int score) // ZAHRA
{
    ofstream file(filename);
    if (file.is_open())
    {
        file << score;
        file.close();
    }
}
bool ElementInDeque(Vector2 val, const deque<Vector2> &dq) // JAYESHA
{
    for (const auto &el : dq)
        if (Vector2Equals(val, el))
            return true;
    return false;
}

bool eventtriggered(double interval) // JAYESHA
{
    double currenttime = GetTime();
    if (currenttime - lastupdatetime >= interval)
    {
        lastupdatetime = currenttime;
        return true;
    }
    return false;
}

struct Node // UMAIMA
{
    Vector2 position;
    float gCost = 0, hCost = 0, fCost = 0;
    Node *parent = nullptr;
};

float Heuristic(Vector2 a, Vector2 b)
{
    return abs(a.x - b.x) + abs(a.y - b.y);
}

bool IsCellWalkable(Vector2 cell, const deque<Vector2> &snake) // JAYESHA
{
    if (cell.x < 0 || cell.x >= numberofcells || cell.y < 0 || cell.y >= numberofcells)
        return false;
    return !ElementInDeque(cell, snake);
}

vector<Vector2> AStar(Vector2 start, Vector2 goal, const deque<Vector2> &snake) // UMAIMA
{
    vector<Vector2> path;
    auto cmp = [](Node *a, Node *b)
    { return a->fCost > b->fCost; };
    priority_queue<Node *, vector<Node *>, decltype(cmp)> openSet(cmp);
    unordered_set<string> closed;
    vector<Node *> allNodes;

    auto vec2str = [](Vector2 v)
    {
        return to_string((int)v.x) + "," + to_string((int)v.y);
    };

    Node *startNode = new Node{start};
    startNode->hCost = Heuristic(start, goal);
    startNode->fCost = startNode->hCost;
    openSet.push(startNode);
    allNodes.push_back(startNode);

    vector<Vector2> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    while (!openSet.empty())
    {
        Node *current = openSet.top();
        openSet.pop();

        if (Vector2Equals(current->position, goal))
        {
            while (current)
            {
                path.push_back(current->position);
                current = current->parent;
            }
            reverse(path.begin(), path.end());
            break;
        }

        closed.insert(vec2str(current->position));

        for (Vector2 dir : directions)
        {
            Vector2 neighbor = Vector2Add(current->position, dir);
            if (!IsCellWalkable(neighbor, snake) || closed.count(vec2str(neighbor)))
                continue;

            float g = current->gCost + 1;
            float h = Heuristic(neighbor, goal);
            Node *next = new Node{neighbor, g, h, g + h, current};
            openSet.push(next);
            allNodes.push_back(next);
        }
    }

    for (auto n : allNodes)
        delete n;
    return path;
}

class Snake // JAYESHA
{
public:
    deque<Vector2> body = {{4, 23}, {3, 23}, {2, 23}};
    Vector2 direction = {1, 0};
    bool addbody = false;

    void Reset()
    {
        body = {{4, 23}, {3, 23}, {2, 23}};
        direction = {1, 0};
    }

    void Draw()
    {
        for (auto &segment : body)
        {
            Rectangle r = {offset + segment.x * cellsize, offset + segment.y * cellsize, cellsize, cellsize};
            DrawRectangleRounded(r, 0.5, 8, darkgreen);
        }
    }

    void Update()
    {
        body.push_front(Vector2Add(body[0], direction));
        if (!addbody)
            body.pop_back();
        else
            addbody = false;
    }
};

class Food // JAYESHA
{
public:
    Vector2 position;
    Texture2D texture;

    Food() {}
    Food(deque<Vector2> &snake, const char *filename)
    {
        Image img = LoadImage(filename);
        texture = LoadTextureFromImage(img);
        UnloadImage(img);
        position = GenerateRandomPos(snake);
    }

    virtual ~Food() { UnloadTexture(texture); }

    Vector2 GenerateRandomPos(deque<Vector2> &snake)
    {
        Vector2 pos;
        do
        {
            pos = {(float)GetRandomValue(0, numberofcells - 1), (float)GetRandomValue(0, numberofcells - 1)};
        } while (ElementInDeque(pos, snake));
        return pos;
    }

    virtual void Draw()
    {
        DrawTexture(texture, offset + position.x * cellsize, offset + position.y * cellsize, WHITE);
    }
};

class GrowthFood : public Food // UMAIMA
{
public:
    GrowthFood(deque<Vector2> &snake) : Food(snake, "Graphics/growth.png") {}
    void Apply(Snake &s) { s.addbody = true; }
};

class ShrinkFood : public Food // UMAIMA
{
public:
    ShrinkFood(deque<Vector2> &snake) : Food(snake, "Graphics/shrink.png") {}
    void Apply(Snake &s)
    {
        if (s.body.size() > 1)
            s.body.pop_back();
        else
            s.Reset();
    }
};

class NegativeFood : public Food // UMAIMA
{
public:
    NegativeFood(deque<Vector2> &snake) : Food(snake, "Graphics/negative.png") {}
};

class PowerUpFood // UMAIMA
{
public:
    Vector2 position;
    double spawnTime;
    bool isActive;
    const float duration = 5;
    PowerUpFood()
    {
        isActive = false;
    }

    void Spawn(deque<Vector2> &snake)
    {
        position = GenerateRandomPos(snake);
        spawnTime = GetTime();
        isActive = true;
    }

    Vector2 GenerateRandomPos(deque<Vector2> &snake)
    {
        Vector2 pos;
        do
        {
            pos = {(float)GetRandomValue(0, numberofcells - 1), (float)GetRandomValue(0, numberofcells - 1)};
        } while (ElementInDeque(pos, snake));
        return pos;
    }

    void Draw()
    {
        if (isActive)
        {
            float glow = 2 + sin(GetTime() * 5);
            DrawCircle(offset + position.x * cellsize + cellsize / 2,
                       offset + position.y * cellsize + cellsize / 2,
                       cellsize / 2 - 5 + glow,
                       Fade(GOLD, 0.5f));

            DrawCircle(offset + position.x * cellsize + cellsize / 2,
                       offset + position.y * cellsize + cellsize / 2,
                       cellsize / 2 - 5,
                       GOLD);
        }
    }

    void Update()
    {
        if (isActive && GetTime() - spawnTime >= duration)
        {
            isActive = false;
        }
    }

    bool CheckCollision(Vector2 snakeHead) // JAYESHA,UMAIMA
    {
        if (isActive && Vector2Equals(position, snakeHead))
        {
            isActive = false;
            return true;
        }
        return false;
    }
};

class Game
{
public:
    Snake snake;
    Food food;
    GrowthFood growth;
    ShrinkFood shrink;
    NegativeFood negative1, negative2;
    int score = 0;
    bool running = true;
    PowerUpFood powerUp;

    Game() : food(snake.body, "Graphics/food.png"), // JAYESHA
             growth(snake.body),
             shrink(snake.body),
             negative1(snake.body),
             negative2(snake.body)
    {
    }

    ~Game()
    {
    }

    void Draw() // JAYESHA
    {
        food.Draw();
        growth.Draw();
        shrink.Draw();
        negative1.Draw();
        negative2.Draw();
        snake.Draw();
        powerUp.Draw();
    }

    void Update() // ALL
    {

        if (!running)
            return;

        snake.Update();
        Vector2 head = snake.body[0];

        if (Vector2Equals(head, food.position))
        {
            food.position = food.GenerateRandomPos(snake.body);
            snake.addbody = true;
            score++;
            growthFoodCount++; // increment on black food
            PlaySound(eat);
        }

        if (Vector2Equals(head, growth.position))
        {
            growth.Apply(snake);
            growth.position = growth.GenerateRandomPos(snake.body);
            score += 2;
            growthFoodCount++; // increment on blue food
            PlaySound(eat);
        }

        if (Vector2Equals(head, shrink.position))
        {
            if (snake.body.size() <= 1)
            {
                running = false;
                gameOverScreen = true;
                PlaySound(hit);
                return;
            }
            shrink.Apply(snake);
            shrink.position = shrink.GenerateRandomPos(snake.body);
            shrinkFoodCount++;
            PlaySound(hit);
        }

        if (Vector2Equals(head, negative1.position) || Vector2Equals(head, negative2.position))
        {
            if (Vector2Equals(head, negative1.position))
                negative1.position = negative1.GenerateRandomPos(snake.body);
            else
                negative2.position = negative2.GenerateRandomPos(snake.body);
            if (score == 0)
            {
                running = false;
                gameOverScreen = true;
                PlaySound(hit);
                return;
            }
            score--;
            negativeFoodCount++;
            PlaySound(hit);
        }

        if (head.x < 0 || head.y < 0 || head.x >= numberofcells || head.y >= numberofcells ||
            ElementInDeque(head, deque<Vector2>(snake.body.begin() + 1, snake.body.end())))
        {
            if (score > highScore)
            {
                highScore = score;
                saveHighScore("highscore.txt", highScore);
            }
            running = false;
            gameOverScreen = true;
            PlaySound(hit);
        }

        powerUp.Update();

        if (!powerUp.isActive && GetRandomValue(0, 1000) < 5)
        {
            powerUp.Spawn(snake.body);
        }

        if (powerUp.CheckCollision(snake.body[0]))
        {
            score += 5;
            PlaySound(eat);
        }
    }

    void Restart() // ALL
    {
        buttonClicked = false;
        score = 0;
        aiMode = false;
        growthFoodCount = shrinkFoodCount = negativeFoodCount = 0;
        snake.Reset();
        food.position = food.GenerateRandomPos(snake.body);
        growth.position = growth.GenerateRandomPos(snake.body);
        shrink.position = shrink.GenerateRandomPos(snake.body);
        negative1.position = negative1.GenerateRandomPos(snake.body);
        negative2.position = negative2.GenerateRandomPos(snake.body);
        running = true;
        gameOverScreen = false;
        gameStartTime = GetTime();
    }
};

int main() // ALL
{
    InitWindow(2 * offset + cellsize * numberofcells, 2 * offset + cellsize * numberofcells, "Snake AI Game");
    SetTargetFPS(60); // default fps
    highScore = loadHighScore("highscore.txt");
    Game game;
    float updateInterval = 0.2f;
    InitAudioDevice();
    button = LoadSound("Sounds/button.mp3");
    bg = LoadMusicStream("Sounds/bg2.ogg");
    eat = LoadSound("Sounds/eat.mp3");
    hit = LoadSound("Sounds/wall.mp3");
    PlayMusicStream(bg);
    SetMusicVolume(bg, 0.7f);
    SetSoundVolume(eat, 1.0f);
    SetSoundVolume(hit, 1.0f);

    while (!WindowShouldClose())
    {
        UpdateMusicStream(bg);
        if (IsKeyPressed(KEY_P))
        {
            paused = !paused;
        }
        double elapsed = 0.0;
        if (!menuScreen && !gameOverScreen)
        {
            if (!paused)
            {
                elapsed = GetTime() - gameStartTime;
            }
        }
        BeginDrawing();
        if (aiMode)
            ClearBackground(aiBackground); // Change to AI background color
        else
            ClearBackground(green); // Default background color

        if (menuScreen)
        {
            DrawText("Welcome to AI Snake Game", offset, offset - 40, 30, DARKGREEN);
            DrawText("Rules:", offset, offset, 25, BLACK);
            DrawText("- Red: -1 point, no size change", offset, offset + 30, 20, RED);
            DrawText("- Orange: no score change, -1 size", offset, offset + 60, 20, ORANGE);
            DrawText("- Blue: +2 points, +1 size", offset, offset + 90, 20, BLUE);
            DrawText("- Black: +1 point, +1 size", offset, offset + 120, 20, BLACK);
            DrawText("Mode: ", offset, offset + 150, 30, DARKPURPLE);
            DrawRectangleRec(easybutton, DARKGREEN);
            DrawText("Easy", easybutton.x + 70, easybutton.y + 15, 20, BLACK);

            DrawRectangleRec(mediumbutton, DARKGREEN);
            DrawText("Medium", mediumbutton.x + 55, mediumbutton.y + 15, 20, BLACK);

            DrawRectangleRec(hardbutton, DARKGREEN);
            DrawText("Hard", hardbutton.x + 70, hardbutton.y + 15, 20, BLACK);

            Vector2 mouseposition = GetMousePosition();
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsKeyPressed(KEY_ENTER))
            {
                if (CheckCollisionPointRec(mouseposition, easybutton) || IsKeyPressed(KEY_ENTER))
                {
                    PlaySound(button);
                    SetTargetFPS(60);
                    updateInterval = 0.2f;
                    buttonClicked = true;
                    menuScreen = false;
                    lastupdatetime = GetTime();
                    gameStartTime = GetTime();
                }
                if (CheckCollisionPointRec(mouseposition, mediumbutton))
                {
                    PlaySound(button);
                    SetTargetFPS(120);
                    updateInterval = 0.1f;
                    buttonClicked = true;
                    menuScreen = false;
                    lastupdatetime = GetTime();
                    gameStartTime = GetTime();
                }
                if (CheckCollisionPointRec(mouseposition, hardbutton))
                {
                    PlaySound(button);
                    SetTargetFPS(180);
                    updateInterval = 0.05f;
                    buttonClicked = true;
                    menuScreen = false;
                    lastupdatetime = GetTime();
                    gameStartTime = GetTime();
                }
                if (IsKeyPressed(KEY_ENTER))
                {
                    menuScreen = false;
                    updateInterval = 0.2f;
                }
            }
            if (IsKeyPressed(KEY_ENTER) && !buttonClicked)
            {
                menuScreen = false;
            }
        }
        else if (gameOverScreen)
        {
            DrawText("Game Over!", (GetScreenWidth() - MeasureText("Game Over!", 40)) / 2, offset + 100, 40, RED);
            DrawText(TextFormat("Total Score: %i", game.score),
                     (GetScreenWidth() - MeasureText(TextFormat("Total Score: %i", game.score), 25)) / 2, offset + 150, 25, DARKGRAY);
            DrawText(TextFormat("High Score: %i", highScore),
                     (GetScreenWidth() - MeasureText(TextFormat("High Score: %i", highScore), 25)) / 2,
                     offset + 180, 25, DARKGREEN);
            DrawText(TextFormat("Growth Foods Eaten: %i", growthFoodCount),
                     (GetScreenWidth() - MeasureText(TextFormat("Growth Foods Eaten: %i", growthFoodCount), 25)) / 2, offset + 220, 25, BLUE);
            DrawText(TextFormat("Shrink Foods Eaten: %i", shrinkFoodCount),
                     (GetScreenWidth() - MeasureText(TextFormat("Shrink Foods Eaten: %i", shrinkFoodCount), 25)) / 2, offset + 255, 25, ORANGE);
            DrawText(TextFormat("Negative Foods Eaten: %i", negativeFoodCount),
                     (GetScreenWidth() - MeasureText(TextFormat("Negative Foods Eaten: %i", negativeFoodCount), 25)) / 2, offset + 290, 25, RED);
            DrawText("Press R to Restart or ESC to Exit", (GetScreenWidth() - MeasureText("Press R to Restart or ESC to Exit", 25)) / 2, offset + 350, 25, DARKGRAY);
            if (IsKeyPressed(KEY_R))
            {
                game.Restart();
                gameOverScreen = false;
                menuScreen = true;
            }
        }
        else
        {
            if (!paused && eventtriggered(updateInterval))
            {
                if (aiMode)
                {
                    vector<Vector2> foodTargets = {
                        game.food.position,
                        game.growth.position,
                        game.shrink.position,
                        game.negative1.position,
                        game.negative2.position};

                    // Optionally filter by priority or penalty
                    float minDist = FLT_MAX;
                    Vector2 target = game.food.position;

                    for (auto &f : foodTargets)
                    {
                        float dist = Heuristic(game.snake.body[0], f);
                        if (dist < minDist)
                        {
                            minDist = dist;
                            target = f;
                        }
                    }

                    vector<Vector2> path = AStar(game.snake.body[0], target, game.snake.body);
                    if (path.size() > 1)
                    {
                        Vector2 step = Vector2Subtract(path[1], game.snake.body[0]);
                        game.snake.direction = step;
                    }
                }
                game.Update();
            }

            if (!paused && !aiMode)
            {
                if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
                    game.snake.direction = {0, -1};
                if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
                    game.snake.direction = {0, 1};
                if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
                    game.snake.direction = {-1, 0};
                if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
                    game.snake.direction = {1, 0};
            }

            if (IsKeyPressed(KEY_SPACE))
                aiMode = !aiMode;

            DrawText(TextFormat("Time: %.1f s", elapsed), offset, 20, 20, DARKGRAY);
            DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, numberofcells * cellsize + 10, numberofcells * cellsize + 10}, 5, darkgreen);
            DrawText(TextFormat("Score: %i", game.score), offset, offset + numberofcells * cellsize + 10, 25, BLACK);
            DrawText(TextFormat("Growth: %i | Shrink: %i | Negative: %i   Press P to pause", growthFoodCount, shrinkFoodCount, negativeFoodCount),
                     offset + 250, offset + numberofcells * cellsize + 10, 20, DARKGRAY);
            DrawText(aiMode ? "Mode: AI (SPACE to switch)" : "Mode: Manual (SPACE to switch)", offset + 300, 20, 20, DARKGRAY);
            game.Draw();
        }
        if (paused)
        {
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));
            int w = MeasureText("PAUSED", 40);
            DrawText("PAUSED", screenWidth / 2 - w / 2, screenHeight / 2 - 40 / 2, 40, RED);
            DrawText("Press P to resume", screenWidth / 2 - MeasureText("Press P to resume", 20) / 2,
                     screenHeight / 2 + 40, 20, LIGHTGRAY);
        }

        EndDrawing();
    }
    UnloadSound(button);
    UnloadSound(eat);
    UnloadSound(hit);
    UnloadMusicStream(bg);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
