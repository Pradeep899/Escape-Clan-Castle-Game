#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>
#include <unistd.h>
#define MAX_DOORS 5
#define XP_BAR_LENGTH 20
#define COLOR_RESET 7  
#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_BROWN 6
#define COLOR_LIGHTGRAY 7
#define COLOR_DARKGRAY 8
#define COLOR_LIGHTBLUE 9
#define COLOR_LIGHTGREEN 10
#define COLOR_LIGHTCYAN 11
#define COLOR_LIGHTRED 12
#define COLOR_YELLOW 14
#define COLOR_WHITE 15
#define COLOR_PURPLE 13
#define TYPE_SPEED 30
#define COLOR_LIGHTMAGENTA 16

int player_level = 1;
int player_xp = 0;
int xp_to_next_level = 100;
int round_count = 0;
int enemy_stat_increase = 0;
float PLAYER_STAT_MULTIPLIER = 1.03f;
int ENEMY_STAT_INCREASE = 1;
int total_rounds = 10;

void typeTextCentered(const char *text, int color, int speed);
void typeTextWithNewline(const char *text, int color, int speed);
void showrules();

struct TypeEffectiveness {
    char name[20];
    float effectiveness[12];
};

struct TypeEffectiveness type_effectiveness[] = {
    {"fire", {0.5, 2.0, 2.0, 0.5, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 0.5}},
    {"water", {2.0, 0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 1.0}},
    {"ice", {0.5, 1.0, 0.5, 2.0, 2.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}},
    {"rock", {2.0, 1.0, 2.0, 1.0, 2.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.5}},
    {"flying", {1.0, 1.0, 1.0, 0.5, 1.0, 2.0, 2.0, 1.0, 1.0, 1.0, 1.0, 1.0}},
    {"electric", {1.0, 2.0, 1.0, 1.0, 0.5, 0.5, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0}},
    {"earth", {1.0, 1.0, 1.0, 2.0, 0.5, 2.0, 1.0, 0.5, 1.0, 1.0, 1.0, 1.0}},
    {"plant", {0.5, 2.0, 1.0, 2.0, 0.5, 1.0, 2.0, 0.5, 1.0, 1.0, 1.0, 1.0}},
    {"dark", {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.5, 2.0, 0.5, 1.0}},
    {"psychic", {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 0.5, 1.0, 1.0}},
    {"undead", {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.5, 1.0}},
    {"metal", {2.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.5}}
};

struct status_effect {
    char name[50];
    int duration;
    int rounds_remaining;
    int active;
};

struct ability {
    char name[50];
    int duration;
    int active;
    int rounds_remaining;
};

struct ability abilities[] = {
    {"Critical Slash", 1, 0, 0},
    {"Piercing Blow", 1, 0, 0},
    {"Life Drain", 2, 0, 0},
    {"Iron Wall", 1, 0, 0},
    {"Counter Attack", 1, 0, 0},
    {"Berserker Mode", 1, 0, 0}
};

int noofabilities = sizeof(abilities) / sizeof(abilities[0]);

struct player {
    char name[50];
    int hp;
    int max_hp;
    int stamina;
    int max_stamina;
    int def;
    int spdef;
    int gold;
    int health_potions;
    int stamina_potions;
    int level;
    int xp;
    int xp_to_next_level;
    struct ability active_ability;
    struct status_effect active_status;
};

struct player player1;

struct basicenemy {
    char name[50];
    int hp;
    char attack1[50];
    int power1;
    char attack2[50];
    int power2;
    int def;
    int spdef;
    char type[20];
    struct status_effect attack1_status;
    struct status_effect attack2_status;
    struct status_effect active_status;
};

struct bossenemy {
    char name[50];
    int hp;
    char attack1[50];
    int power1;
    char attack2[50];
    int power2;
    char attack3[50];
    int power3;
    char attack4[50];
    int power4;
};

struct attack {
    char name[50];
    char type[50];
    char attack_type[20];
    int power;
    int staminacost;
    int accuracy;
};

struct door {
    int reward;
};

struct basicenemy basicenemies[] = {
    {"Infernal Hound", 200, "Flame Claw", 30, "Burning Rush", 35, 70, 60, "fire", {"Burn", 2, 0, 0}, {"None", 0, 0, 0}},
    {"Ashborn Phoenix", 220, "Firestorm Dive", 35, "Ember Wing", 40, 65, 70, "fire", {"Burn", 2, 0, 0}, {"None", 0, 0, 0}},
    {"Tidal Serpent", 240, "Aqua Fang", 30, "Tsunami Crash", 35, 80, 70, "water", {"None", 0, 0, 0}, {"None", 0, 0, 0}},
    {"Kraken Maw", 260, "Tentacle Smash", 35, "Dark Vortex", 40, 75, 65, "water", {"None", 0, 0, 0}, {"Confusion", 2, 0, 0}},
    {"Frost Golem", 210, "Frozen Punch", 30, "Blizzard Storm", 35, 85, 60, "ice", {"Freeze", 1, 0, 0}, {"None", 0, 0, 0}},
    {"Glacial Beast", 230, "Frostbite Claw", 35, "Iron Chill", 40, 80, 70, "ice", {"Freeze", 1, 0, 0}, {"None", 0, 0, 0}},
    {"Stone Goliath", 250, "Rock Smash", 35, "Stone Crash", 40, 90, 65, "rock", {"None", 0, 0, 0}, {"None", 0, 0, 0}},
    {"Obsidian Guardian", 270, "Magma Punch", 40, "Lava Wave", 45, 85, 75, "rock", {"Burn", 2, 0, 0}, {"None", 0, 0, 0}},
    {"Sky Reaper", 220, "Nightfall Peck", 30, "Aerial Slash", 35, 75, 70, "flying", {"None", 0, 0, 0}, {"None", 0, 0, 0}},
    {"Tempest Griffin", 240, "Gale Strike", 35, "Tornado Roar", 45, 80, 65, "flying", {"None", 0, 0, 0}, {"Confusion", 2, 0, 0}},
    {"Thunderfang Tiger", 230, "Thunder Bite", 30, "Lightning Rush", 40, 75, 70, "electric", {"Paralysis", 2, 0, 0}, {"None", 0, 0, 0}},
    {"Storm Warden", 250, "Shockwave Cannon", 40, "Iron Charge", 35, 80, 75, "electric", {"Paralysis", 2, 0, 0}, {"None", 0, 0, 0}},
    {"Sand Titan", 260, "Quicksand Grip", 35, "Tremor Stomp", 40, 90, 65, "earth", {"None", 0, 0, 0}, {"None", 0, 0, 0}},
    {"Burrow Beast", 280, "Stone Fang", 40, "Earthquake Slam", 50, 85, 70, "earth", {"None", 0, 0, 0}, {"None", 0, 0, 0}},
    {"Venomroot", 210, "Toxic Whip", 30, "Leech Spores", 35, 75, 65, "plant", {"Poison", 3, 0, 0}, {"None", 0, 0, 0}},
    {"Ancient Treant", 240, "Root Slam", 35, "Nature's Wrath", 45, 80, 70, "plant", {"None", 0, 0, 0}, {"Confusion", 2, 0, 0}},
    {"Shadow Stalker", 220, "Shadow Claw", 30, "Nightmare Slash", 35, 75, 65, "dark", {"None", 0, 0, 0}, {"Confusion", 2, 0, 0}},
    {"Umbral Demon", 250, "Infernal Strike", 40, "Dark Incineration", 45, 80, 70, "dark", {"Burn", 2, 0, 0}, {"None", 0, 0, 0}},
    {"Mind Reaper", 230, "Psywave", 35, "Mind Crush", 40, 75, 70, "psychic", {"Confusion", 2, 0, 0}, {"None", 0, 0, 0}},
    {"Astral Oracle", 260, "Starbeam", 45, "Future Sight", 50, 80, 75, "psychic", {"None", 0, 0, 0}, {"Confusion", 2, 0, 0}},
    {"Haunting Specter", 210, "Ethereal Strike", 30, "Phantom Wave", 35, 75, 65, "undead", {"None", 0, 0, 0}, {"Confusion", 2, 0, 0}},
    {"Bone Lich", 240, "Soul Drain", 40, "Necrotic Blast", 45, 80, 70, "undead", {"None", 0, 0, 0}, {"None", 0, 0, 0}},
    {"Iron Juggernaut", 250, "Steel Slam", 35, "Iron Crush", 40, 90, 65, "metal", {"None", 0, 0, 0}, {"None", 0, 0, 0}},
    {"Titan Automaton", 280, "Titan Slam", 45, "Shockwave Cannon", 50, 85, 75, "metal", {"None", 0, 0, 0}, {"None", 0, 0, 0}}
};

int noofbasicenemies = sizeof(basicenemies)/sizeof(basicenemies[0]);

int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return csbi.dwSize.X;
    }
    return 80;
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void printCenteredText(const char *text, int color) {
    int width = getConsoleWidth();
    int textLength = strlen(text);
    int padding = (width - textLength) / 2;

    setColor(color);
    for (int i = 0; i < padding; i++) {
        printf(" ");
    }
    printf("%s\n", text);
    setColor(COLOR_RESET);
}

void typeText(const char *text, int color, int speed) {
    setColor(color);
    for (int i = 0; text[i] != '\0'; i++) {
        putchar(text[i]);
        fflush(stdout);
        Sleep(speed);
    }
    setColor(COLOR_RESET);
}

void quickPrint(const char *text, int color) {
    setColor(color);
    printf("%s", text);
    setColor(COLOR_RESET);
}

void typeTextCentered(const char *text, int color, int speed) {
    int width = getConsoleWidth();
    int textLength = strlen(text);
    int padding = (width - textLength) / 2;
    
    setColor(color);
    for (int i = 0; i < padding; i++) {
        printf(" ");
    }
    for (int i = 0; text[i] != '\0'; i++) {
        putchar(text[i]);
        fflush(stdout);
        Sleep(speed);
    }
    printf("\n");
    setColor(COLOR_RESET);
}

void typeTextWithNewline(const char *text, int color, int speed) {
    setColor(color);
    for (int i = 0; text[i] != '\0'; i++) {
        putchar(text[i]);
        fflush(stdout);
        Sleep(speed);
    }
    printf("\n");
    setColor(COLOR_RESET);
}

void slowPrint(const char *text, int delay) {
    for (int i = 0; text[i] != '\0'; i++) {
        putchar(text[i]);
        fflush(stdout);
        Sleep(delay);
    }
}

void clamp_stats(struct player *p) {
    if (p->hp > p->max_hp) p->hp = p->max_hp;
    if (p->stamina > p->max_stamina) p->stamina = p->max_stamina;
    if (p->hp < 0) p->hp = 0;
    if (p->def < 0) p->def = 0;
    if (p->spdef < 0) p->spdef = 0;
}

void showrules() {
    system("cls");
    setColor(COLOR_YELLOW);
    slowPrint("\n=== MONSTER LABYRINTH - RULEBOOK ===\n\n", 30);
    
    setColor(COLOR_LIGHTCYAN);
    slowPrint("1. GAME OVERVIEW\n", 20);
    setColor(COLOR_WHITE);
    slowPrint("   - Survive 10 rounds of challenges\n", 20);
    slowPrint("   - Each round presents mysterious doors\n", 20);
    slowPrint("   - Behind doors: Monsters, rewards, or dead ends\n", 20);
    slowPrint("   - Defeat the final boss to escape!\n\n", 20);
    
    setColor(COLOR_LIGHTCYAN);
    slowPrint("2. DIFFICULTY LEVELS\n", 20);
    setColor(COLOR_WHITE);
    slowPrint("   - ", 20);
    setColor(COLOR_LIGHTGREEN);
    slowPrint("Easy", 20);
    setColor(COLOR_WHITE);
    slowPrint(":  Monsters have 25% reduced stats\n", 20);
    slowPrint("   - ", 20);
    setColor(COLOR_YELLOW);
    slowPrint("Medium", 20);
    setColor(COLOR_WHITE);
    slowPrint(": Standard monster strength\n", 20);
    slowPrint("   - ", 20);
    setColor(COLOR_LIGHTRED);
    slowPrint("Hard", 20);
    setColor(COLOR_WHITE);
    slowPrint(":  Monsters have 25% boosted stats\n\n", 20);
    
    setColor(COLOR_LIGHTCYAN);
    slowPrint("3. PLAYER MECHANICS\n", 20);
    setColor(COLOR_WHITE);
    slowPrint("   - ", 20);
    setColor(COLOR_LIGHTRED);
    slowPrint("HP", 20);
    setColor(COLOR_WHITE);
    slowPrint(": If reaches 0, game over\n", 20);
    slowPrint("   - ", 20);
    setColor(COLOR_LIGHTBLUE);
    slowPrint("Stamina", 20);
    setColor(COLOR_WHITE);
    slowPrint(": Used for attacks (regenerates)\n", 20);
    slowPrint("   - ", 20);
    setColor(COLOR_LIGHTGREEN);
    slowPrint("DEF/SPDEF", 20);
    setColor(COLOR_WHITE);
    slowPrint(": Reduce physical/special damage\n", 20);
    slowPrint("   - ", 20);
    setColor(COLOR_YELLOW);
    slowPrint("Gold", 20);
    setColor(COLOR_WHITE);
    slowPrint(": Buy potions from shop\n\n", 20);
    
    setColor(COLOR_LIGHTCYAN);
    slowPrint("4. ABILITIES (Randomly Gained)\n", 20);
    setColor(COLOR_WHITE);
    slowPrint("   - ", 20);
    setColor(COLOR_LIGHTRED);
    slowPrint("Critical Slash", 20);
    setColor(COLOR_WHITE);
    slowPrint(" (1 round): 50% double damage\n", 20);
    slowPrint("   - ", 20);
    setColor(COLOR_LIGHTBLUE);
    slowPrint("Piercing Blow", 20);
    setColor(COLOR_WHITE);
    slowPrint(" (1 round): Ignore enemy DEF\n", 20);
    slowPrint("   - ", 20);
    setColor(COLOR_LIGHTGREEN);
    slowPrint("Life Drain", 20);
    setColor(COLOR_WHITE);
    slowPrint(" (2 rounds): Heal 50% of damage\n", 20);
    slowPrint("   - ", 20);
    setColor(COLOR_YELLOW);
    slowPrint("Iron Wall", 20);
    setColor(COLOR_WHITE);
    slowPrint(" (1 round): Double your DEF\n", 20);
    slowPrint("   - ", 20);
    setColor(COLOR_LIGHTCYAN);
    slowPrint("Counter Attack", 20);
    setColor(COLOR_WHITE);
    slowPrint(" (1 round): Reflect 30% damage\n", 20);
    slowPrint("   - ", 20);
    setColor(COLOR_LIGHTMAGENTA);
    slowPrint("Berserker Mode", 20);
    setColor(COLOR_WHITE);
    slowPrint(" (1 round): +50% ATK, -50% DEF\n\n", 20);
    
    setColor(COLOR_LIGHTCYAN);
    slowPrint("5. COMBAT SYSTEM\n", 20);
    setColor(COLOR_WHITE);
    slowPrint("   - Turn-based battles\n", 20);
    slowPrint("   - Choose from 5 selected moves\n", 20);
    slowPrint("   - Type effectiveness matters (", 20);
    setColor(COLOR_RED);
    slowPrint("Fire", 20);
    setColor(COLOR_WHITE);
    slowPrint(" > ", 20);
    setColor(COLOR_LIGHTGREEN);
    slowPrint("Plant", 20);
    setColor(COLOR_WHITE);
    slowPrint(" etc.)\n", 20);
    slowPrint("   - Status effects: ", 20);
    setColor(COLOR_RED);
    slowPrint("Burn", 20);
    setColor(COLOR_WHITE);
    slowPrint(", ", 20);
    setColor(COLOR_PURPLE);
    slowPrint("Poison", 20);
    setColor(COLOR_WHITE);
    slowPrint(", ", 20);
    setColor(COLOR_LIGHTCYAN);
    slowPrint("Freeze", 20);
    setColor(COLOR_WHITE);
    slowPrint(", etc.\n\n", 20);
    
    setColor(COLOR_LIGHTCYAN);
    slowPrint("6. DOOR MECHANICS\n", 20);
    setColor(COLOR_WHITE);
    slowPrint("   - 2-5 doors appear each round\n", 20);
    slowPrint("   - Only 1 leads to next round\n", 20);
    slowPrint("   - Others may contain monsters or dead ends\n", 20);
    slowPrint("   - Last door guarantees passage if others fail\n\n", 20);
    
    setColor(COLOR_LIGHTCYAN);
    slowPrint("7. TIPS\n", 20);
    setColor(COLOR_WHITE);
    slowPrint("   - Manage stamina carefully\n", 20);
    slowPrint("   - Use type advantages\n", 20);
    slowPrint("   - Buy potions when low\n", 20);
    slowPrint("   - Status effects can turn the tide\n\n", 20);
    
    setColor(COLOR_YELLOW);
    slowPrint("Press any key to continue...", 20);
    setColor(COLOR_RESET);
    getch();
}

void displayMonsterUI(struct basicenemy *enemy) {
    printf("\n");
    printCenteredText("====================", COLOR_RED);
    printCenteredText(enemy->name, COLOR_LIGHTRED);
    printf("Type: ");
    if (strcmp(enemy->type, "fire") == 0) setColor(COLOR_RED);
    else if (strcmp(enemy->type, "water") == 0) setColor(COLOR_BLUE);
    else if (strcmp(enemy->type, "plant") == 0) setColor(COLOR_LIGHTGREEN);
    else if (strcmp(enemy->type, "electric") == 0) setColor(COLOR_YELLOW);
    else if (strcmp(enemy->type, "ice") == 0) setColor(COLOR_LIGHTCYAN);
    else if (strcmp(enemy->type, "rock") == 0) setColor(COLOR_DARKGRAY);
    else if (strcmp(enemy->type, "flying") == 0) setColor(COLOR_LIGHTBLUE);
    else if (strcmp(enemy->type, "earth") == 0) setColor(COLOR_BROWN);
    else if (strcmp(enemy->type, "dark") == 0) setColor(COLOR_DARKGRAY);
    else if (strcmp(enemy->type, "psychic") == 0) setColor(COLOR_LIGHTMAGENTA);
    else if (strcmp(enemy->type, "undead") == 0) setColor(COLOR_DARKGRAY);
    else if (strcmp(enemy->type, "metal") == 0) setColor(COLOR_LIGHTGRAY);
    else setColor(COLOR_WHITE);
    printf("%s\n", enemy->type);
    setColor(COLOR_RESET);
    printf("HP: %d\n", enemy->hp);
    printCenteredText("====================", COLOR_RED);
}

void welcome(char name[], int hp, int stamina, struct attack attacks[], int noofattacks) {
    typeText("\n----------------------------------------\n", COLOR_YELLOW, TYPE_SPEED);
    typeText("  Welcome, ", COLOR_LIGHTCYAN, TYPE_SPEED);
    typeText(name, COLOR_LIGHTGREEN, TYPE_SPEED);
    typeText("!\n", COLOR_LIGHTCYAN, TYPE_SPEED);
    typeText("  Prepare for battle!\n", COLOR_WHITE, TYPE_SPEED);
    typeText("----------------------------------------\n\n", COLOR_YELLOW, TYPE_SPEED);
}

void select_moves(struct attack move_pool[], int noofattacks, struct attack selected_attacks[], int *noofselectedattacks) {
    printf("\n");
    typeTextCentered("=== MOVE SELECTION ===", COLOR_YELLOW, TYPE_SPEED);
    typeTextWithNewline("Choose 5 unique moves from the following list:", COLOR_LIGHTCYAN, TYPE_SPEED);
    
    int selected_moves[noofattacks];
    for (int i = 0; i < noofattacks; i++) {
        selected_moves[i] = 0;
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < noofattacks; j++) {
            if (strcmp(move_pool[j].type, "Fire") == 0) setColor(COLOR_RED);
            else if (strcmp(move_pool[j].type, "Water") == 0) setColor(COLOR_BLUE);
            else if (strcmp(move_pool[j].type, "Poison") == 0) setColor(COLOR_PURPLE);
            else if (strcmp(move_pool[j].type, "Electric") == 0) setColor(COLOR_YELLOW);
            else if (strcmp(move_pool[j].type, "Ice") == 0) setColor(COLOR_LIGHTCYAN);
            else if (strcmp(move_pool[j].type, "Rock") == 0) setColor(COLOR_DARKGRAY);
            else if (strcmp(move_pool[j].type, "Flying") == 0) setColor(COLOR_LIGHTBLUE);
            else if (strcmp(move_pool[j].type, "Earth") == 0) setColor(COLOR_BROWN);
            else if (strcmp(move_pool[j].type, "Plant") == 0) setColor(COLOR_LIGHTGREEN);
            else if (strcmp(move_pool[j].type, "Dark") == 0) setColor(COLOR_DARKGRAY);
            else if (strcmp(move_pool[j].type, "Psychic") == 0) setColor(COLOR_LIGHTMAGENTA);
            else if (strcmp(move_pool[j].type, "Undead") == 0) setColor(COLOR_DARKGRAY);
            else if (strcmp(move_pool[j].type, "Metal") == 0) setColor(COLOR_LIGHTGRAY);
            else setColor(COLOR_WHITE);
            
            printf("%d - %s", j + 1, move_pool[j].name);
            setColor(COLOR_RESET);
            
            setColor(COLOR_LIGHTCYAN);
            printf(" (Type: ");
            setColor(COLOR_RESET);
            
            if (strcmp(move_pool[j].type, "Fire") == 0) setColor(COLOR_RED);
            else if (strcmp(move_pool[j].type, "Water") == 0) setColor(COLOR_BLUE);
            else if (strcmp(move_pool[j].type, "Poison") == 0) setColor(COLOR_PURPLE);
            else if (strcmp(move_pool[j].type, "Electric") == 0) setColor(COLOR_YELLOW);
            else if (strcmp(move_pool[j].type, "Ice") == 0) setColor(COLOR_LIGHTCYAN);
            else if (strcmp(move_pool[j].type, "Rock") == 0) setColor(COLOR_DARKGRAY);
            else if (strcmp(move_pool[j].type, "Flying") == 0) setColor(COLOR_LIGHTBLUE);
            else if (strcmp(move_pool[j].type, "Earth") == 0) setColor(COLOR_BROWN);
            else if (strcmp(move_pool[j].type, "Plant") == 0) setColor(COLOR_LIGHTGREEN);
            else if (strcmp(move_pool[j].type, "Dark") == 0) setColor(COLOR_DARKGRAY);
            else if (strcmp(move_pool[j].type, "Psychic") == 0) setColor(COLOR_LIGHTMAGENTA);
            else if (strcmp(move_pool[j].type, "Undead") == 0) setColor(COLOR_DARKGRAY);
            else if (strcmp(move_pool[j].type, "Metal") == 0) setColor(COLOR_LIGHTGRAY);
            else setColor(COLOR_WHITE);
            
            printf("%s", move_pool[j].type);
            setColor(COLOR_RESET);
            
            setColor(COLOR_LIGHTCYAN);
            printf(" | Attack Type: ");
            setColor(COLOR_RESET);
            printf("%s", move_pool[j].attack_type);
            
            setColor(COLOR_LIGHTCYAN);
            printf(" | Power: ");
            setColor(COLOR_RESET);
            printf("%d", move_pool[j].power);
            
            setColor(COLOR_LIGHTCYAN);
            printf(" | Stamina: ");
            setColor(COLOR_RESET);
            printf("%d", move_pool[j].staminacost);
            
            setColor(COLOR_LIGHTCYAN);
            printf(" | Accuracy: ");
            setColor(COLOR_RESET);
            printf("%d%%", move_pool[j].accuracy);
            
            printf(")\n");
        }

        int choice;
        setColor(COLOR_LIGHTGREEN);
        printf("Select move %d (1-%d): ", i + 1, noofattacks);
        setColor(COLOR_RESET);
        scanf("%d", &choice);

        while (choice < 1 || choice > noofattacks || selected_moves[choice - 1] == 1) {
            if (choice < 1 || choice > noofattacks) {
                setColor(COLOR_LIGHTRED);
                printf("Invalid choice! Select move %d (1-%d): ", i + 1, noofattacks);
                setColor(COLOR_RESET);
            } else {
                setColor(COLOR_LIGHTRED);
                printf("You have already selected this move! Choose a different move: ");
                setColor(COLOR_RESET);
            }
            scanf("%d", &choice);
        }

        selected_attacks[i] = move_pool[choice - 1];
        selected_moves[choice - 1] = 1;
        setColor(COLOR_LIGHTGREEN);
        printf("You selected ");
        if (strcmp(selected_attacks[i].type, "Fire") == 0) setColor(COLOR_RED);
        else if (strcmp(selected_attacks[i].type, "Water") == 0) setColor(COLOR_BLUE);
        else if (strcmp(selected_attacks[i].type, "Poison") == 0) setColor(COLOR_PURPLE);
        else if (strcmp(selected_attacks[i].type, "Electric") == 0) setColor(COLOR_YELLOW);
        else if (strcmp(selected_attacks[i].type, "Ice") == 0) setColor(COLOR_LIGHTCYAN);
        else if (strcmp(selected_attacks[i].type, "Rock") == 0) setColor(COLOR_DARKGRAY);
        else if (strcmp(selected_attacks[i].type, "Flying") == 0) setColor(COLOR_LIGHTBLUE);
        else if (strcmp(selected_attacks[i].type, "Earth") == 0) setColor(COLOR_BROWN);
        else if (strcmp(selected_attacks[i].type, "Plant") == 0) setColor(COLOR_LIGHTGREEN);
        else if (strcmp(selected_attacks[i].type, "Dark") == 0) setColor(COLOR_DARKGRAY);
        else if (strcmp(selected_attacks[i].type, "Psychic") == 0) setColor(COLOR_LIGHTMAGENTA);
        else if (strcmp(selected_attacks[i].type, "Undead") == 0) setColor(COLOR_DARKGRAY);
        else if (strcmp(selected_attacks[i].type, "Metal") == 0) setColor(COLOR_LIGHTGRAY);
        else setColor(COLOR_WHITE);
        printf("%s", selected_attacks[i].name);
        setColor(COLOR_RESET);
        printf("!\n");
        system("cls");
    }
    *noofselectedattacks = 5;
}

void gain_xp(struct player *player, int amount) {
    player->xp += amount;
    printf("You gained %d XP!\n", amount);
    
    printf("XP: [");
    int xp_progress = (int)(((float)player->xp / player->xp_to_next_level) * XP_BAR_LENGTH);
    for (int i = 0; i < XP_BAR_LENGTH; i++) {
        if (i < xp_progress) {
            setColor(COLOR_LIGHTBLUE);
            printf("#");
            setColor(COLOR_RESET);
        } else {
            printf(" ");
        }
    }
    printf("] %d/%d\n", player->xp, player->xp_to_next_level);
    
    while (player->xp >= player->xp_to_next_level) {
        player->xp -= player->xp_to_next_level;
        player->level++;
        player->xp_to_next_level = (int)(player->xp_to_next_level * 1.2);
        
        player->max_hp = (int)(player->max_hp * PLAYER_STAT_MULTIPLIER);
        player->max_stamina = (int)(player->max_stamina * PLAYER_STAT_MULTIPLIER);
        player->def = (int)(player->def * PLAYER_STAT_MULTIPLIER);
        player->spdef = (int)(player->spdef * PLAYER_STAT_MULTIPLIER);
        
        player->hp = player->max_hp;
        player->stamina = player->max_stamina;
        
        printf("\nLEVEL UP! You are now level %d!\n", player->level);
        printf("Your stats have increased by 3%%!\n");
        printf("HP: %d (+%d)\n", player->max_hp, (int)(player->max_hp * 0.03));
        printf("Stamina: %d (+%d)\n", player->max_stamina, (int)(player->max_stamina * 0.03));
        printf("Defense: %d (+%d)\n", player->def, (int)(player->def * 0.03));
        printf("Special Defense: %d (+%d)\n", player->spdef, (int)(player->spdef * 0.03));
    }
}

void scale_enemy_stats(struct basicenemy *enemy, int round_count) {
    enemy->hp += (int)(enemy->hp * 0.05 * round_count);
    enemy->power1 += ENEMY_STAT_INCREASE * round_count;
    enemy->power2 += ENEMY_STAT_INCREASE * round_count;
    enemy->def += ENEMY_STAT_INCREASE * round_count;
    enemy->spdef += ENEMY_STAT_INCREASE * round_count;
    
    if (round_count % 5 == 0) {
        enemy->hp = (int)(enemy->hp * 1.25);
        enemy->power1 = (int)(enemy->power1 * 1.25);
        enemy->power2 = (int)(enemy->power2 * 1.25);
        enemy->def = (int)(enemy->def * 1.25);
        enemy->spdef = (int)(enemy->spdef * 1.25);
        
        printf("\nA MINI-BOSS APPEARS! %s looks stronger than usual!\n", enemy->name);
    }
}

void usePotion(struct player *player1) {
    int choice;
    printf("\n=== Potion Menu ===\n");
    printf("1. Health Potion (%d available)\n", player1->health_potions);
    printf("2. Stamina Potion (%d available)\n", player1->stamina_potions);
    printf("3. Go back\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1 && player1->health_potions > 0) {
        player1->health_potions--;
        player1->hp += 50;
        if (player1->hp > player1->max_hp) player1->hp = player1->max_hp;
        printf("You used a Health Potion! Your HP is now %d.\n", player1->hp);
    } else if (choice == 2 && player1->stamina_potions > 0) {
        player1->stamina_potions--;
        player1->stamina += 20;
        if (player1->stamina > player1->max_stamina) player1->stamina = player1->max_stamina;
        printf("You used a Stamina Potion! Your Stamina is now %d.\n", player1->stamina);
    } else if (choice == 3) {
        printf("Going back...\n");
    } else {
        printf("Invalid choice or no potions available!\n");
    }
}

void shop(struct player *player1) {
    int choice;
    while (1) {
        printf("\n=== Welcome to the Merchant's Shop ===\n");
        printf("1. Health Potion (+50 HP) - 20 gold\n");
        printf("2. Stamina Potion (+20 Stamina) - 15 gold\n");
        printf("3. Exit Shop\n");
        printf("Your gold: %d\n", player1->gold);
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1 && player1->gold >= 20) {
            player1->gold -= 20;
            player1->health_potions++;
            printf("You bought a Health Potion! You now have %d Health Potions.\n", player1->health_potions);
        } else if (choice == 2 && player1->gold >= 15) {
            player1->gold -= 15;
            player1->stamina_potions++;
            printf("You bought a Stamina Potion! You now have %d Stamina Potions.\n", player1->stamina_potions);
        } else if (choice == 3) {
            printf("Exiting the shop...\n");
            break;
        } else {
            printf("Invalid choice or not enough gold!\n");
        }
    }
}

void reward(struct player *player1, struct attack attacks[], int noofattacks) {
    int roll = rand() % 100;
    if (roll < 50) {
        int goldReward = (rand() % 21) + 10;
        player1->gold += goldReward;
        printf("You found %d gold! Updated Gold: %d\n", goldReward, player1->gold);
    } 
    else if (roll < 65) {
        player1->health_potions++;
        printf("You found a Health Potion! You now have %d Health Potions.\n", player1->health_potions);
    } 
    else if (roll < 80) {
        player1->stamina_potions++;
        printf("You found a Stamina Potion! You now have %d Stamina Potions.\n", player1->stamina_potions);
    } 
    else if (roll < 90) {
        int attackIndex = rand() % noofattacks;
        attacks[attackIndex].power += 5;
        printf("Your attack '%s' has been buffed by +5 power! Updated Power: %d\n", attacks[attackIndex].name, attacks[attackIndex].power);
    } 
    else {
        player1->def += 5;
        player1->spdef += 5;
        printf("Your Defense and Special Defense have increased by +5 each!\n");
        printf("Updated Defense: %d, Updated Special Defense: %d\n", player1->def, player1->spdef);
    }
}

void check_ability_gain(struct player *player1) {
    if (player1->active_ability.active == 0) {
        int chance = rand() % 100;
        if (chance < 5) {
            int ability_index = rand() % noofabilities;
            player1->active_ability = abilities[ability_index];
            player1->active_ability.active = 1;
            player1->active_ability.rounds_remaining = player1->active_ability.duration;
            printf("\nYou gained the ability: %s!\n", player1->active_ability.name);
            printf("Effect: ");
            if (strcmp(player1->active_ability.name, "Critical Slash") == 0) {
                printf("Your attacks have a 50%% chance to deal double damage for %d round.\n", player1->active_ability.duration);
            } else if (strcmp(player1->active_ability.name, "Piercing Blow") == 0) {
                printf("Your attacks ignore the enemy's defense for %d round.\n", player1->active_ability.duration);
            } else if (strcmp(player1->active_ability.name, "Life Drain") == 0) {
                printf("You heal for half the damage you deal for %d rounds.\n", player1->active_ability.duration);
            } else if (strcmp(player1->active_ability.name, "Iron Wall") == 0) {
                printf("Your defense is doubled for %d round.\n", player1->active_ability.duration);
            } else if (strcmp(player1->active_ability.name, "Counter Attack") == 0) {
                printf("You reflect 30%% of the damage taken back to the enemy for %d round.\n", player1->active_ability.duration);
            } else if (strcmp(player1->active_ability.name, "Berserker Mode") == 0) {
                printf("Your attack increases by 50%%, but your defense is reduced by 50%% for %d round.\n", player1->active_ability.duration);
            }
        }
    }
}

void roundcleared(struct basicenemy *selected_enemy, struct player *player1, struct attack attacks[], int noofattacks) {
    printf("You have defeated %s! You move to the next round.\n", selected_enemy->name);
    reward(player1, attacks, noofattacks);
    check_ability_gain(player1);
}

void check_ability_expiry(struct player *player1) {
    if (player1->active_ability.active) {
        player1->active_ability.rounds_remaining--;
        if (player1->active_ability.rounds_remaining <= 0) {
            printf("\nYour ability '%s' has expired.\n", player1->active_ability.name);
            player1->active_ability.active = 0;
        }
    }
}

void apply_status_effect(struct status_effect *target, const char *name, int duration) {
    strcpy(target->name, name);
    target->duration = duration;
    target->rounds_remaining = duration;
    target->active = 1;
    printf("%s is now affected by %s!\n", target == &player1.active_status ? "You" : "The enemy", name);
}

void update_status_effect(struct status_effect *status, struct player *player1, struct basicenemy *enemy) {
    if (status->active) {
        status->rounds_remaining--;
        if (status->rounds_remaining <= 0) {
            printf("%s has worn off from %s.\n", status->name, player1 ? "you" : "the enemy");
            status->active = 0;
        } else {
            if (strcmp(status->name, "Poison") == 0) {
                int damage = 10;
                if (player1) {
                    player1->hp -= damage;
                    printf("You take %d damage from Poison!\n", damage);
                } else {
                    enemy->hp -= damage;
                    printf("The enemy takes %d damage from Poison!\n", damage);
                }
            } else if (strcmp(status->name, "Burn") == 0) {
                int damage = 15;
                if (player1) {
                    player1->hp -= damage;
                    player1->def = (int)(player1->def * 0.9);
                    printf("You take %d damage from Burn and your defense is reduced!\n", damage);
                } else {
                    enemy->hp -= damage;
                    enemy->def = (int)(enemy->def * 0.9);
                    printf("The enemy takes %d damage from Burn and its defense is reduced!\n", damage);
                }
            } else if (strcmp(status->name, "Paralysis") == 0) {
                int chance = rand() % 100;
                if (chance < 25) {
                    if (player1) {
                        printf("You are paralyzed and cannot act this turn!\n");
                    } else {
                        printf("The enemy is paralyzed and cannot act this turn!\n");
                    }
                }
            } else if (strcmp(status->name, "Bleed") == 0) {
                int damage = 20;
                if (player1) {
                    player1->hp -= damage;
                    printf("You take %d damage from Bleed!\n", damage);
                } else {
                    enemy->hp -= damage;
                    printf("The enemy takes %d damage from Bleed!\n", damage);
                }
            } else if (strcmp(status->name, "Freeze") == 0) {
                if (player1) {
                    printf("You are frozen and cannot act this turn!\n");
                } else {
                    printf("The enemy is frozen and cannot act this turn!\n");
                }
            } else if (strcmp(status->name, "Confusion") == 0) {
                int chance = rand() % 100;
                if (chance < 50) {
                    if (player1) {
                        int self_damage = (int)(player1->hp * 0.3);
                        player1->hp -= self_damage;
                        printf("You are confused and hurt yourself for %d damage!\n", self_damage);
                    } else {
                        int self_damage = (int)(enemy->hp * 0.3);
                        enemy->hp -= self_damage;
                        printf("The enemy is confused and hurt itself for %d damage!\n", self_damage);
                    }
                }
            }
        }
    }
}

float get_effectiveness(const char *attacker_type, const char *defender_type) {
    for (int i = 0; i < sizeof(type_effectiveness)/sizeof(type_effectiveness[0]); i++) {
        if (strcasecmp(type_effectiveness[i].name, attacker_type) == 0) {
            for (int j = 0; j < sizeof(type_effectiveness)/sizeof(type_effectiveness[0]); j++) {
                if (strcasecmp(type_effectiveness[j].name, defender_type) == 0) {
                    printf("\nType Matchup: %s → %s = %.1fx\n", 
                          attacker_type, defender_type, 
                          type_effectiveness[i].effectiveness[j]);
                    return type_effectiveness[i].effectiveness[j];
                }
            }
        }
    }
    return 1.0;
}

void display_player_ui(struct player *player) {
    printf("\n");
    printCenteredText("====================", COLOR_GREEN);
    printf("%s (Lv.%d)\n", player->name, player->level);
    
    setColor(COLOR_GREEN);
    printf("HP: [");
    int hp_progress = (int)(((float)player->hp / player->max_hp) * 20);
    for (int i = 0; i < 20; i++) {
        printf(i < hp_progress ? "#" : " ");
    }
    printf("] %d/%d\n", player->hp, player->max_hp);
    
    setColor(COLOR_LIGHTBLUE);
    printf("ST: [");
    int st_progress = (int)(((float)player->stamina / player->max_stamina) * 20);
    for (int i = 0; i < 20; i++) {
        printf(i < st_progress ? "|" : " ");
    }
    printf("] %d/%d\n", player->stamina, player->max_stamina);
    
    setColor(COLOR_LIGHTCYAN);
    printf("XP: [");
    int xp_progress = (int)(((float)player->xp / player->xp_to_next_level) * 20);
    for (int i = 0; i < 20; i++) {
        printf(i < xp_progress ? "#" : " ");
    }
    printf("] %d/%d\n", player->xp, player->xp_to_next_level);
    
    setColor(COLOR_RESET);
    printf("Gold: %d | Potions: %dH/%dS\n", 
           player->gold, player->health_potions, player->stamina_potions);
    printCenteredText("====================", COLOR_GREEN);
}

void battle(struct basicenemy *selected_enemy, struct player *player1, struct attack attacks[], int noofattacks) {
    scale_enemy_stats(selected_enemy, round_count);
    displayMonsterUI(selected_enemy);
    typeText("\nThe battle begins!\n", COLOR_LIGHTRED, TYPE_SPEED);

    while (player1->hp > 0 && selected_enemy->hp > 0) {
        update_status_effect(&player1->active_status, player1, NULL);
        update_status_effect(&selected_enemy->active_status, NULL, selected_enemy);

        int choice = 0;
        while (1) {
            typeText("\nChoose your attack:\n", COLOR_LIGHTCYAN, TYPE_SPEED);
            for (int i = 0; i < noofattacks; i++) {
                if (strcmp(attacks[i].type, "Fire") == 0) setColor(COLOR_RED);
                else if (strcmp(attacks[i].type, "Water") == 0) setColor(COLOR_BLUE);
                else if (strcmp(attacks[i].type, "Poison") == 0) setColor(COLOR_PURPLE);
                else if (strcmp(attacks[i].type, "Electric") == 0) setColor(COLOR_YELLOW);
                else if (strcmp(attacks[i].type, "Ice") == 0) setColor(COLOR_LIGHTCYAN);
                else if (strcmp(attacks[i].type, "Rock") == 0) setColor(COLOR_DARKGRAY);
                else if (strcmp(attacks[i].type, "Flying") == 0) setColor(COLOR_LIGHTBLUE);
                else if (strcmp(attacks[i].type, "Earth") == 0) setColor(COLOR_BROWN);
                else if (strcmp(attacks[i].type, "Plant") == 0) setColor(COLOR_LIGHTGREEN);
                else if (strcmp(attacks[i].type, "Dark") == 0) setColor(COLOR_DARKGRAY);
                else if (strcmp(attacks[i].type, "Psychic") == 0) setColor(COLOR_LIGHTMAGENTA);
                else if (strcmp(attacks[i].type, "Undead") == 0) setColor(COLOR_DARKGRAY);
                else if (strcmp(attacks[i].type, "Metal") == 0) setColor(COLOR_LIGHTGRAY);
                else setColor(COLOR_WHITE);
                
                printf("%d - %s", i + 1, attacks[i].name);
                setColor(COLOR_RESET);
                printf(" (Power: %d | Stamina: %d | Accuracy: %d%%)\n", 
                    attacks[i].power, attacks[i].staminacost, attacks[i].accuracy);
            }
            
            typeText("Select (1-", COLOR_WHITE, TYPE_SPEED);
            printf("%d", noofattacks);
            typeText("): ", COLOR_WHITE, TYPE_SPEED);
            
            char input = getch();
            choice = input - '0';
            if (choice >= 1 && choice <= noofattacks && player1->stamina >= attacks[choice - 1].staminacost) {
                break;
            }
            typeText("Invalid choice or insufficient stamina!\n", COLOR_LIGHTRED, TYPE_SPEED);
        }

        int playeraccuracy = rand() % 100;
        if (playeraccuracy < attacks[choice - 1].accuracy) {
            player1->stamina -= attacks[choice - 1].staminacost;
            int attack_damage = attacks[choice - 1].power + rand() % 10;

            int attack_color = COLOR_WHITE;
            if (strcmp(attacks[choice-1].type, "Fire") == 0) attack_color = COLOR_RED;
            else if (strcmp(attacks[choice-1].type, "Water") == 0) attack_color = COLOR_BLUE;
            else if (strcmp(attacks[choice-1].type, "Poison") == 0) attack_color = COLOR_PURPLE;
            else if (strcmp(attacks[choice-1].type, "Electric") == 0) attack_color = COLOR_YELLOW;
            else if (strcmp(attacks[choice-1].type, "Ice") == 0) attack_color = COLOR_LIGHTCYAN;
            else if (strcmp(attacks[choice-1].type, "Rock") == 0) attack_color = COLOR_DARKGRAY;
            else if (strcmp(attacks[choice-1].type, "Flying") == 0) attack_color = COLOR_LIGHTBLUE;
            else if (strcmp(attacks[choice-1].type, "Earth") == 0) attack_color = COLOR_BROWN;
            else if (strcmp(attacks[choice-1].type, "Plant") == 0) attack_color = COLOR_LIGHTGREEN;
            else if (strcmp(attacks[choice-1].type, "Dark") == 0) attack_color = COLOR_DARKGRAY;
            else if (strcmp(attacks[choice-1].type, "Psychic") == 0) attack_color = COLOR_LIGHTMAGENTA;
            else if (strcmp(attacks[choice-1].type, "Undead") == 0) attack_color = COLOR_DARKGRAY;
            else if (strcmp(attacks[choice-1].type, "Metal") == 0) attack_color = COLOR_LIGHTGRAY;

            setColor(attack_color);
            typeText(attacks[choice - 1].name, attack_color, TYPE_SPEED);
            setColor(COLOR_RESET);

            float effectiveness = get_effectiveness(attacks[choice - 1].type, selected_enemy->type);
            attack_damage *= effectiveness;

            if (effectiveness == 2.0) {
                typeText(" - Super effective!", COLOR_LIGHTGREEN, TYPE_SPEED);
            } else if (effectiveness == 0.5) {
                typeText(" - Not very effective...", COLOR_DARKGRAY, TYPE_SPEED);
            }

            int critchance = rand() % 100;
            if (critchance < 5) {
                setColor(COLOR_YELLOW);
                typeText(" *** CRITICAL HIT! *** ", COLOR_YELLOW, TYPE_SPEED);
                setColor(COLOR_RESET);
                attack_damage *= 2;
            }

            if (strcmp(attacks[choice - 1].attack_type, "Physical") == 0) {
                attack_damage -= selected_enemy->def / 5;
            } else {
                attack_damage -= selected_enemy->spdef / 5;
            }

            if (attack_damage < 0) attack_damage = 0;
            selected_enemy->hp -= attack_damage;
            printf(" - Dealt %d damage!\n", attack_damage);
        } else {
            typeText("Your attack missed!\n", COLOR_DARKGRAY, TYPE_SPEED);
        }

        if (selected_enemy->hp <= 0) {
            selected_enemy->hp = 0;
            displayMonsterUI(selected_enemy);
            typeText("\nYou defeated the ", COLOR_LIGHTGREEN, TYPE_SPEED);
            typeText(selected_enemy->name, COLOR_LIGHTRED, TYPE_SPEED);
            typeText("!\n", COLOR_LIGHTGREEN, TYPE_SPEED);
            
            int base_xp = 50 + (selected_enemy->hp / 10);
            if (round_count % 5 == 0) { 
                base_xp *= 2;
                player1->hp += (int)(player1->max_hp * 0.20);
                printf("You regain 20%% health from defeating a mini-boss!\n");
            }
            gain_xp(player1, base_xp);
            
            break;
        }

        if (selected_enemy->hp > 0) {
            int enemyattack = rand() % 2;
            int damage = rand() % 5 + (enemyattack == 0 ? selected_enemy->power1 : selected_enemy->power2) - (player1->def / 5);
            int accuracy = rand() % 100;

            if (accuracy < 80) {
                if (damage < 0) damage = 0;
                player1->hp -= damage;
                if (player1->hp <= 0) {
                    player1->hp = 0;
                    break;
                }
                setColor(COLOR_LIGHTRED);
                printf("\n%s uses ", selected_enemy->name);
                if (enemyattack == 0) {
                    printf("%s", selected_enemy->attack1);
                } else {
                    printf("%s", selected_enemy->attack2);
                }
                printf("! ");
                setColor(COLOR_RESET);
                printf("You take %d damage.\n", damage);

                if (enemyattack == 0 && strcmp(selected_enemy->attack1_status.name, "None") != 0 && rand() % 100 < 25) {
                    apply_status_effect(&player1->active_status, selected_enemy->attack1_status.name, selected_enemy->attack1_status.duration);
                } else if (enemyattack == 1 && strcmp(selected_enemy->attack2_status.name, "None") != 0 && rand() % 100 < 25) {
                    apply_status_effect(&player1->active_status, selected_enemy->attack2_status.name, selected_enemy->attack2_status.duration);
                }
            } else {
                setColor(COLOR_DARKGRAY);
                printf("\n%s's attack missed!\n", selected_enemy->name);
                setColor(COLOR_RESET);
            }
        }

        printf("\n----------------------------------------\n");
        typeText("Your HP: ", COLOR_LIGHTGREEN, TYPE_SPEED);
        printf("%d", player1->hp);
        typeText(" | Stamina: ", COLOR_LIGHTCYAN, TYPE_SPEED);
        printf("%d\n", player1->stamina);
        typeText("Enemy HP: ", COLOR_LIGHTRED, TYPE_SPEED);
        printf("%d\n", selected_enemy->hp);
        printf("----------------------------------------\n");
    }

    if (player1->hp <= 0) {
        player1->hp = 0;
        typeText("\nYou have been defeated...\n", COLOR_LIGHTRED, TYPE_SPEED);
    }
}

void finalmonster(struct player *player1, struct attack attacks[], int noofattacks) {
    struct bossenemy bossenemies[] = {
        {"Abyssal Tyrant", 700, "Shadow Dominion", 30, "Abyssal Grasp", 40, "Doom Wave", 50, "Soul Eater", 80},
        {"Time Reaver", 700, "Temporal Slash", 25, "Chrono Blast", 35, "Time Lock", 40, "Reality Shift", 70}
    };
    
    int selectedboss = rand() % 2;
    struct bossenemy *boss = &bossenemies[selectedboss];
    
    typeText("\nThe ground trembles beneath your feet...\n", COLOR_LIGHTRED, TYPE_SPEED);
    typeText("A monstrous presence emerges from the darkness!\n", COLOR_LIGHTRED, TYPE_SPEED);
    
    printf("\n");
    printCenteredText("====================", COLOR_LIGHTMAGENTA);
    printCenteredText(boss->name, COLOR_LIGHTMAGENTA);
    printCenteredText("THE FINAL BOSS", COLOR_LIGHTRED);
    printf("HP: %d\n", boss->hp);
    printCenteredText("====================", COLOR_LIGHTMAGENTA);
    
    while (player1->hp > 0 && boss->hp > 0) {
        int choice = 0;
        while (1) {
            typeText("\nChoose your attack:\n", COLOR_LIGHTCYAN, TYPE_SPEED);
            for (int i = 0; i < noofattacks; i++) {
                if (strcmp(attacks[i].type, "Fire") == 0) setColor(COLOR_RED);
                else if (strcmp(attacks[i].type, "Water") == 0) setColor(COLOR_BLUE);
                else if (strcmp(attacks[i].type, "Poison") == 0) setColor(COLOR_PURPLE);
                else if (strcmp(attacks[i].type, "Electric") == 0) setColor(COLOR_YELLOW);
                else if (strcmp(attacks[i].type, "Ice") == 0) setColor(COLOR_LIGHTCYAN);
                else if (strcmp(attacks[i].type, "Rock") == 0) setColor(COLOR_DARKGRAY);
                else if (strcmp(attacks[i].type, "Flying") == 0) setColor(COLOR_LIGHTBLUE);
                else if (strcmp(attacks[i].type, "Earth") == 0) setColor(COLOR_BROWN);
                else if (strcmp(attacks[i].type, "Plant") == 0) setColor(COLOR_LIGHTGREEN);
                else if (strcmp(attacks[i].type, "Dark") == 0) setColor(COLOR_DARKGRAY);
                else if (strcmp(attacks[i].type, "Psychic") == 0) setColor(COLOR_LIGHTMAGENTA);
                else if (strcmp(attacks[i].type, "Undead") == 0) setColor(COLOR_DARKGRAY);
                else if (strcmp(attacks[i].type, "Metal") == 0) setColor(COLOR_LIGHTGRAY);
                else setColor(COLOR_WHITE);
                
                printf("%d - %s", i + 1, attacks[i].name);
                setColor(COLOR_RESET);
                printf(" (Power: %d | Stamina: %d)\n", attacks[i].power, attacks[i].staminacost);
            }
            
            typeText("Select (1-", COLOR_WHITE, TYPE_SPEED);
            printf("%d", noofattacks);
            typeText("): ", COLOR_WHITE, TYPE_SPEED);
            
            char input = getch();
            choice = input - '0';
            if (choice >= 1 && choice <= noofattacks && player1->stamina >= attacks[choice - 1].staminacost) {
                break;
            }
            typeText("Invalid choice or insufficient stamina!\n", COLOR_LIGHTRED, TYPE_SPEED);
        }

        int playeraccuracy = rand() % 100;
        if (playeraccuracy < attacks[choice - 1].accuracy) {
            player1->stamina -= attacks[choice - 1].staminacost;
            int attack_damage = attacks[choice - 1].power;

            int attack_color = COLOR_WHITE;
            if (strcmp(attacks[choice-1].type, "Fire") == 0) attack_color = COLOR_RED;
            else if (strcmp(attacks[choice-1].type, "Water") == 0) attack_color = COLOR_BLUE;
            else if (strcmp(attacks[choice-1].type, "Poison") == 0) attack_color = COLOR_PURPLE;
            else if (strcmp(attacks[choice-1].type, "Electric") == 0) attack_color = COLOR_YELLOW;
            else if (strcmp(attacks[choice-1].type, "Ice") == 0) attack_color = COLOR_LIGHTCYAN;
            else if (strcmp(attacks[choice-1].type, "Rock") == 0) attack_color = COLOR_DARKGRAY;
            else if (strcmp(attacks[choice-1].type, "Flying") == 0) attack_color = COLOR_LIGHTBLUE;
            else if (strcmp(attacks[choice-1].type, "Earth") == 0) attack_color = COLOR_BROWN;
            else if (strcmp(attacks[choice-1].type, "Plant") == 0) attack_color = COLOR_LIGHTGREEN;
            else if (strcmp(attacks[choice-1].type, "Dark") == 0) attack_color = COLOR_DARKGRAY;
            else if (strcmp(attacks[choice-1].type, "Psychic") == 0) attack_color = COLOR_LIGHTMAGENTA;
            else if (strcmp(attacks[choice-1].type, "Undead") == 0) attack_color = COLOR_DARKGRAY;
            else if (strcmp(attacks[choice-1].type, "Metal") == 0) attack_color = COLOR_LIGHTGRAY;

            setColor(attack_color);
            typeText(attacks[choice - 1].name, attack_color, TYPE_SPEED);
            setColor(COLOR_RESET);

            int critchance = rand() % 100;
            if (critchance < 10) {
                attack_damage *= 2;
                setColor(COLOR_LIGHTRED);
                typeText(" CRITICAL HIT!", COLOR_LIGHTRED, TYPE_SPEED);
                setColor(COLOR_RESET);
                
                if (rand() % 100 < 30) {
                    int bonus = rand() % 2;
                    if (bonus == 0) {
                        int heal = 30 + rand() % 21;
                        player1->hp += heal;
                        typeText(" The strike revitalizes you!", COLOR_LIGHTGREEN, TYPE_SPEED);
                        printf(" +%d HP!", heal);
                    } else {
                        int stam = 15 + rand() % 16;
                        player1->stamina += stam;
                        typeText(" You feel energized!", COLOR_LIGHTCYAN, TYPE_SPEED);
                        printf(" +%d Stamina!", stam);
                    }
                }
            }

            if (strcmp(attacks[choice - 1].attack_type, "Physical") == 0) {
                attack_damage -= 15;
            } else {
                attack_damage -= 10;
            }

            if (attack_damage < 5) attack_damage = 5;
            boss->hp -= attack_damage;
            printf(" - Dealt %d damage!\n", attack_damage);
        } else {
            typeText("Your attack missed!\n", COLOR_DARKGRAY, TYPE_SPEED);
        }

        if (boss->hp <= 0) {
            boss->hp = 0;
            printf("\n");
            printCenteredText("====================", COLOR_LIGHTMAGENTA);
            typeText("You have defeated ", COLOR_LIGHTGREEN, TYPE_SPEED);
            typeText(boss->name, COLOR_LIGHTMAGENTA, TYPE_SPEED);
            typeText("!\n", COLOR_LIGHTGREEN, TYPE_SPEED);
            printCenteredText("THE CURSE IS LIFTED!", COLOR_YELLOW);
            printCenteredText("====================", COLOR_LIGHTMAGENTA);
            typeText("\nCongratulations! You have escaped the cursed castle!\n", COLOR_YELLOW, TYPE_SPEED);
            exit(0);
            return;
        }

        if (boss->hp > 0) {
            int attackchoice;
            if (boss->hp > bossenemies[selectedboss].hp / 2) {
                attackchoice = rand() % 3;
            } else {
                attackchoice = 3;
            }

            char* attackname;
            int basedamage;
            switch (attackchoice) {
                case 0: 
                    attackname = boss->attack1;
                    basedamage = boss->power1;
                    break;
                case 1:
                    attackname = boss->attack2; 
                    basedamage = boss->power2;
                    break;
                case 2:
                    attackname = boss->attack3;
                    basedamage = boss->power3;
                    break;
                case 3:
                    attackname = boss->attack4;
                    basedamage = boss->power4;
                    break;
            }

            int damage = basedamage + rand() % 11 - (player1->def / 6);
            if (damage < 10) damage = 10;

            setColor(COLOR_LIGHTMAGENTA);
            printf("\n%s uses ", boss->name);
            typeText(attackname, COLOR_LIGHTMAGENTA, TYPE_SPEED);
            printf("! ");
            setColor(COLOR_RESET);
            printf("You take %d damage.\n", damage);
            player1->hp -= damage;

            if (boss->hp < bossenemies[selectedboss].hp / 4) {
                typeText("The boss is enraged!\n", COLOR_LIGHTRED, TYPE_SPEED);
                damage = (int)(damage * 1.5);
                printf("Extra damage! You take %d damage.\n", damage);
                player1->hp -= damage;
            }
        }

        printf("\n----------------------------------------\n");
        typeText("Your HP: ", COLOR_LIGHTGREEN, TYPE_SPEED);
        printf("%d", player1->hp);
        typeText(" | Stamina: ", COLOR_LIGHTCYAN, TYPE_SPEED);
        printf("%d\n", player1->stamina);
        typeText("Boss HP: ", COLOR_LIGHTMAGENTA, TYPE_SPEED);
        printf("%d\n", boss->hp);
        printf("----------------------------------------\n");
    }

    if (player1->hp <= 0) {
        player1->hp = 0;
        printf("\n");
        printCenteredText("====================", COLOR_RED);
        typeText("You have been defeated by ", COLOR_LIGHTRED, TYPE_SPEED);
        typeText(boss->name, COLOR_LIGHTMAGENTA, TYPE_SPEED);
        typeText("...\n", COLOR_LIGHTRED, TYPE_SPEED);
        printCenteredText("THE CURSE REMAINS!", COLOR_RED);
        printCenteredText("====================", COLOR_RED);
    }
}

void monster(struct player *player1, struct attack attacks[], int noofattacks) {
    int totalenemies = sizeof(basicenemies) / sizeof(basicenemies[0]);
    int selectedenemy = rand() % totalenemies;
    struct basicenemy *selected_enemy = &basicenemies[selectedenemy];
    printf("\nA wild %s appears!\n", selected_enemy->name);
    printf("HP: %d\n", selected_enemy->hp);
    switch (selectedenemy) {
        case 0:
            printf("\"You cannot escape the darkness...\"\n");
            break;
        case 1:
            printf("\"The poison in my veins will soon be in yours...\"\n");
            break;
        case 2:
            printf("\"Your fate is sealed! My claws hunger for blood!\"\n");
            break;
        case 3:
            printf("\"You dare face the flames of the underworld? Burn!\"\n");
            break;
        case 4:
            printf("\"The storm answers only to me! Feel its fury!\"\n");
            break;
        default:
            printf("\"An unknown force looms over you...\"\n");
    }
    battle(selected_enemy, player1, attacks, noofattacks);
}

void pass(int round, struct player *player1, struct attack attacks[], int noofattacks) {
    if(round < total_rounds){
        printf("Congratulations! You have chosen wisely and found the safe passage. The door creaks open, revealing a path forward. You advance to Round %d!\n", round + 1);
    }
    else if(round == total_rounds){
        printf("Congratulations! Against all odds, you have braved the mysterious doors, outwitted danger, and conquered every challenge before you.\n");
        printf("The final door creaks open, revealing a blinding light… freedom at last!\n\n");
        printf("But wait… wait… something is wrong.\n");
        printf("The ground trembles beneath your feet. A deafening roar echoes through the halls.\n");
        printf("The shadows twist and churn as an enormous figure emerges from the darkness.\n\n");
        printf("A monstrous presence looms before you—far more terrifying than anything you've faced before.\n");
        printf("Your greatest challenge… has just begun.\n");
        finalmonster(player1, attacks, noofattacks);
    }
}

void wall(struct player *player1) {
    printf("You hit a solid wall. The path is blocked! Try another door.\n");
    int damage = rand() % 10;
    if(player1->hp > damage){
        player1->hp -= damage;
        printf("\n----------------------------------------\n");
        printf("Remaining health : %d\n", player1->hp);
        printf("----------------------------------------\n");
    }
}

void select_door(int round, int totaldoors, struct player *player1, struct attack attacks[], int noofattacks) {
    char num;
    int choice;
    int isopened[totaldoors];
    int door_contents[totaldoors];
    int doors_remaining = totaldoors;

    for (int i = 0; i < totaldoors; i++) {
        isopened[i] = 0;
        door_contents[i] = 0;
    }

    switch(totaldoors) {
        case 2:
            door_contents[0] = 1;
            door_contents[1] = 2;
            break;
        case 3:
            door_contents[0] = 1;
            door_contents[1] = 0;
            door_contents[2] = 2;
            break;
        case 4:
            door_contents[0] = 1;
            door_contents[1] = 0;
            door_contents[2] = 0;
            door_contents[3] = 2;
            break;
        case 5:
            door_contents[0] = 1;
            door_contents[1] = 0;
            door_contents[2] = 0;
            door_contents[3] = 0;
            door_contents[4] = 2;
            break;
    }

    for (int i = 0; i < totaldoors; i++) {
        int j = rand() % totaldoors;
        int temp = door_contents[i];
        door_contents[i] = door_contents[j];
        door_contents[j] = temp;
    }

    typeTextWithNewline("\nYou stand before multiple mysterious doors.", COLOR_LIGHTCYAN, TYPE_SPEED);
    typeTextWithNewline("Each one holds a different fate - some lead to safety, others to danger.", COLOR_LIGHTCYAN, TYPE_SPEED);
    typeTextWithNewline("Choose wisely!", COLOR_LIGHTCYAN, TYPE_SPEED);

    while (doors_remaining > 0) {
        typeText("Which door will you open? ( ", COLOR_WHITE, TYPE_SPEED);
        for (int i = 0; i < totaldoors; i++) {
            if (isopened[i] == 0) {
                printf("%d ", i + 1);
            }
        }
        printf("): ");
        
        num = getch();
        choice = num - '0';

        while (choice < 1 || choice > totaldoors || isopened[choice - 1] == 1) {
            typeText("Invalid choice! Choose again ( ", COLOR_LIGHTRED, TYPE_SPEED);
            for (int i = 0; i < totaldoors; i++) {
                if (isopened[i] == 0) {
                    printf("%d ", i + 1);
                }
            }
            printf("): ");
            num = getch();
            choice = num - '0';
        }
        
        isopened[choice - 1] = 1;
        doors_remaining--;
        
        switch(door_contents[choice - 1]) {
            case 0:
                wall(player1);
                break;
            case 1:
                typeTextWithNewline("\nA fierce monster lurks behind this door!", COLOR_LIGHTRED, TYPE_SPEED);
                monster(player1, attacks, noofattacks);
                return;
            case 2:
                typeTextWithNewline("\nYou found the passage!", COLOR_LIGHTGREEN, TYPE_SPEED);
                pass(round, player1, attacks, noofattacks);
                return;
        }
    }
}

void num_doors(int round, struct door doors[], struct player *player1, struct attack attacks[], int noofattacks) {
    if (round == total_rounds) {
        pass(round, player1, attacks, noofattacks);
        return;
    }

    int totaldoors = 2 + rand() % 4;
    select_door(round, totaldoors, player1, attacks, noofattacks);
    check_ability_gain(player1);

    int choice;
    printf("\n=== Round %d Completed ===\n", round);
    printf("What would you like to do next?\n");
    printf("1. Access Shop\n");
    printf("2. Use Potion\n");
    printf("3. Continue to Next Round\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        shop(player1);
    } else if (choice == 2) {
        usePotion(player1);
    } else if (choice == 3) {
        printf("Continuing to the next round...\n");
    } else {
        printf("Invalid choice! Continuing to the next round...\n");
    }
    if (round % 3 == 0) {
        int hp_gain = 20 + rand() % 10;
        int stam_gain = 10 + rand() % 5;
        
        player1->hp += hp_gain;
        player1->stamina += stam_gain;
        clamp_stats(player1);
        
        printf("\nYou feel refreshed after surviving another round!\n");
        printf("HP: +%d, Stamina: +%d\n", hp_gain, stam_gain);
    }
}

void easymode(struct basicenemy *basicenemies) {
    for(int i = 0; i < noofbasicenemies; i++) {
        basicenemies[i].hp *= 0.7;
        basicenemies[i].power1 *= 0.8;
        basicenemies[i].power2 *= 0.8;
        basicenemies[i].def *= 0.8;
        basicenemies[i].spdef *= 0.8;
    }
    player1.hp = 600;
    player1.max_hp = 600;
    player1.stamina = 250;
    player1.max_stamina = 250;
    player1.def = 80;
    player1.spdef = 80;
    player1.gold = 150;
    ENEMY_STAT_INCREASE = 1; 
    total_rounds = 10;
}

void mediummode(struct basicenemy *basicenemies) {
    player1.hp = 500;
    player1.max_hp = 500;
    player1.stamina = 200;
    player1.max_stamina = 200;
    player1.def = 70;
    player1.spdef = 70;
    player1.gold = 100;
    ENEMY_STAT_INCREASE = 2;
    total_rounds = 15;
}

void hardmode(struct basicenemy *basicenemies) {
    for(int i = 0; i < noofbasicenemies; i++) {
        basicenemies[i].hp *= 1.3;
        basicenemies[i].power1 *= 1.2;
        basicenemies[i].power2 *= 1.2;
        basicenemies[i].def *= 1.2;
        basicenemies[i].spdef *= 1.2;
    }
    player1.hp = 400;
    player1.max_hp = 400;
    player1.stamina = 150;
    player1.max_stamina = 150;
    player1.def = 60;
    player1.spdef = 60;
    player1.gold = 50;
    ENEMY_STAT_INCREASE = 3;
    total_rounds = 20;
}

void startgame(struct basicenemy *basicenemies) {
    int choice;
    typeTextWithNewline("\nStarting the game...", COLOR_LIGHTCYAN, TYPE_SPEED); 
    typeTextWithNewline("\nSelect your difficulty level:", COLOR_LIGHTCYAN, TYPE_SPEED);
    typeTextWithNewline("1. Easy Mode", COLOR_LIGHTGREEN, TYPE_SPEED);
    typeTextWithNewline("2. Medium Mode", COLOR_YELLOW, TYPE_SPEED);
    typeTextWithNewline("3. Hard Mode", COLOR_LIGHTRED, TYPE_SPEED);
    typeText("Enter your choice (1 for Easy, 2 for Medium, 3 for Hard): ", COLOR_WHITE, TYPE_SPEED);
    
    char input = getch();
    choice = input - '0';
    printf("%d\n", choice);

    if (choice == 1) {
        typeTextWithNewline("\nYou selected Easy Mode. Good luck!", COLOR_LIGHTGREEN, TYPE_SPEED);
        easymode(basicenemies);
    } else if (choice == 2) {
        typeTextWithNewline("\nYou selected Medium Mode. Brace yourself!", COLOR_YELLOW, TYPE_SPEED);
        mediummode(basicenemies);
    } else if (choice == 3) {
        typeTextWithNewline("\nYou selected Hard Mode. You're brave!", COLOR_LIGHTRED, TYPE_SPEED);
        hardmode(basicenemies);
    } else {
        typeTextWithNewline("\nInvalid choice! Defaulting to Medium Mode.", COLOR_YELLOW, TYPE_SPEED);
        mediummode(basicenemies);
    }
    Sleep(1000);
}

int main() {
    system("cls");
    typeTextCentered("ESCAPE FROM THE CURSED CASTLE", COLOR_YELLOW, TYPE_SPEED);
    
    int choice;
    while (1) {
        typeTextWithNewline("\n=== WELCOME TO THE GAME ===", COLOR_LIGHTCYAN, TYPE_SPEED);
        typeTextWithNewline("1. Start Game", COLOR_WHITE, TYPE_SPEED);
        typeTextWithNewline("2. Check Rules", COLOR_WHITE, TYPE_SPEED);
        typeTextWithNewline("3. Exit", COLOR_WHITE, TYPE_SPEED);
        typeText("Enter your choice: ", COLOR_LIGHTGREEN, TYPE_SPEED);
        
        char input = getch();
        choice = input - '0';
        printf("%d\n", choice);

        if (choice == 1) {
            startgame(basicenemies);
            break;
        } else if (choice == 2) {
            showrules();
        } else if (choice == 3) {
            typeTextWithNewline("\nExiting the game. Goodbye!", COLOR_LIGHTGREEN, TYPE_SPEED);
            return 0;
        } else {
            typeTextWithNewline("\nInvalid choice. Please enter 1, 2, or 3.", COLOR_LIGHTRED, TYPE_SPEED);
        }
    }
    srand(time(0));
    printf("Enter player name (max 49 characters): ");
    fgets(player1.name, 50, stdin);
    player1.name[strcspn(player1.name, "\n")] = 0;
    
    player1.hp = player1.max_hp;
    player1.stamina = player1.max_stamina;
    player1.def = player1.def;
    player1.spdef = player1.spdef;
    player1.gold = player1.gold;
    player1.health_potions = 1;
    player1.stamina_potions = 1;
    player1.level = 1;
    player1.xp = 0;
    player1.xp_to_next_level = 100;
    player1.active_ability.active = 0;
    player1.active_ability.rounds_remaining = 0;

    struct attack move_pool[] = {
        {"Flame Slash", "Fire", "Physical", 30, 6, 85},
        {"Inferno Blast", "Fire", "Special", 35, 7, 80},
        {"Burning Rush", "Fire", "Physical", 25, 5, 90},
        {"Tidal Wave", "Water", "Special", 35, 7, 75},
        {"Aqua Fang", "Water", "Physical", 30, 6, 85},
        {"Hydro Slash", "Water", "Physical", 30, 5, 80},
        {"Frozen Strike", "Ice", "Physical", 35, 7, 85},
        {"Blizzard Storm", "Ice", "Special", 40, 8, 70},
        {"Icicle Crash", "Ice", "Physical", 30, 6, 80},
        {"Thunder Strike", "Electric", "Physical", 30, 6, 80},
        {"Lightning Bolt", "Electric", "Special", 35, 7, 75},
        {"Volt Punch", "Electric", "Physical", 25, 5, 85},
        {"Razor Edge", "Fighting", "Physical", 35, 7,75},
        {"Iron Fist", "Fighting", "Physical", 30, 6, 80},
        {"Counter Strike", "Fighting", "Physical", 40, 8, 70},
        {"Toxic Whip", "Poison", "Physical", 25, 5, 90},
        {"Venom Strike", "Poison", "Physical", 30, 6, 85},
        {"Acid Spray", "Poison", "Special", 30, 5, 80},
        {"Mind Shock", "Psychic", "Special", 40, 12, 70},
        {"Psywave", "Psychic", "Special", 35, 10, 75},
        {"Telekinetic Crush", "Psychic", "Physical", 35, 8, 75},
        {"Holy Beam", "Light", "Special", 40, 8, 80},
        {"Luminous Slash", "Light", "Physical", 30, 6, 80},
        {"Radiant Pulse", "Light", "Special", 35, 7, 70},
        {"Shadow Claw", "Dark", "Physical", 30, 7, 85},
        {"Nightmare Pulse", "Dark", "Special", 35, 8, 80},
        {"Eclipse Strike", "Dark", "Physical", 35, 6, 75},
        {"Wind Cutter", "Wind", "Physical", 30, 7, 85},
        {"Hurricane Blast", "Wind", "Special", 40, 7, 70},
        {"Storm Strike", "Wind", "Physical", 30, 6, 80},
        {"Stone Crash", "Earth", "Physical", 35, 7, 75},
        {"Landslide", "Earth", "Special", 40, 7, 70},
        {"Earthquake Slam", "Earth", "Physical", 30, 5, 70}
    };

    int noofattacks = sizeof(move_pool) / sizeof(move_pool[0]);
    struct attack selected_attacks[5];
    int noofselectedattacks = 0;

    select_moves(move_pool, noofattacks, selected_attacks, &noofselectedattacks);

    welcome(player1.name, player1.hp, player1.stamina, selected_attacks, noofselectedattacks);
    struct door doors[MAX_DOORS];
    for (int round = 1; round <= total_rounds; round++) {
        round_count = round;
        display_player_ui(&player1);
        num_doors(round, doors, &player1, selected_attacks, noofselectedattacks);
        
        if (player1.hp <= 0) {
            break;
        }
    }
}