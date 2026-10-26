#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <glew.h>
#include <glfw3.h>

#define MAX_COLS 20
#define MAX_ROWS 20

#define TILE_EMPTY ' '
#define TILE_WALL '#'
#define TILE_PLAYER '@'
#define TILE_BOX '$'
#define TILE_TARGET '.'
#define TILE_BOX_ON_TARGET '+'
#define TILE_PLAYER_ON_TARGET '&'

#define NUM_LEVELS 5

static const char* levels[NUM_LEVELS][MAX_ROWS] = {
    {
        "    #####",
        "    #   #",
        "    #$  #",
        "  ###  $##",
        "  #  $ $ #",
        "### # ## #   ######",
        "#   # ## #####  ..#",
        "# $  $          ..#",
        "##### ### #@##  ..#",
        "    #     #########",
        "    #######",
        NULL
    },
    {
        "#########",
        "##    ..#",
        "#.$$$ ###",
        "#  @$.###",
        "#.$$$ ###",
        "##    ..#",
        "#########",
        NULL
    },
    {
        "  ###  ",
        " ##. # ",
        "#  $@##",
        "#.$ $.#",
        "## $  #",
        " # .## ",
        "  ###  ",
        NULL
    },
    {
        "  ########  ",
        " #.#    #.# ",
        "#  #$  $#  #",
        "#  #  $ #  #",
        "#  # $@ #  #",
        "#  . ## .  #",
        "#  #    #  #",
        "# $      $ #",
        "# .######. #",
        " ##      ## ",
        NULL
    },
    {
        "   ##########   ",
        "   #..  #   #   ",
        "   #..      #   ",
        "   #..  #  #### ",
        "  #######  #  ##",
        "  #            #",
        "  #  #  ##  #  #",
        "#### ##  #### ##",
        "#  $  ##### #  #",
        "# # $  $  # $  #",
        "# @$  $   #   ##",
        "#### ## ####### ",
        "   #    #       ",
        "   ######       ",
        NULL
    }
};

static const int static_solutions[NUM_LEVELS][1000] = {
    {
        0,2,2,2,0,0,0,2,0,2,2,1,2,2,1,1,1,3,3,3,3,3,3,3,3,3,3,3,3,3,0,3,1,2,2,2,2,2,2,
        2,2,2,2,2,2,2,2,0,2,2,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,3,0,2,0,3,1,2,2,2,2,2,2,
        2,2,0,0,0,2,0,2,2,1,1,1,0,0,2,2,1,1,1,1,3,3,3,3,3,3,3,3,3,3,3,3,3,2,2,2,2,2,2,
        2,2,0,0,0,2,2,0,2,1,1,1,0,0,2,2,1,1,1,3,3,3,3,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,0,
        0,0,2,2,0,0,0,2,1,1,1,1,1,0,0,2,2,1,1,1,3,3,3,3,3,3,3,3,3,3,3,0,3,1,2,1,3,0,2,
        2,2,2,2,2,2,0,0,0,2,2,0,0,0,3,1,1,2,2,1,1,1,1,3,3,3,0,0,0,2,2,0,2,1,1,1,0,0,2,
        2,1,1,1,3,3,3,3,3,3,3,3,3,3,3,1,3,0,2,0,3,-1
    },
    {
        0,1,1,3,2,2,3,0,0,2,3,3,2,2,1,1,1,3,3,3,3,2,2,2,2,0,0,0,0,3,3,3,3,2,2,1,1,1,2,
        1,3,3,2,0,0,3,0,2,0,3,1,1,2,1,1,3,0,-1
    },
    {
        1,2,2,3,0,2,1,1,1,3,0,2,0,0,3,3,1,3,1,2,0,0,2,2,1,3,3,2,1,-1
    },
    {
        3,1,3,3,3,1,1,1,2,0,0,0,0,0,0,1,1,1,2,2,1,2,2,2,0,0,0,3,0,0,3,3,1,1,1,1,1,2,1,
        3,3,2,0,0,3,3,3,1,1,1,2,0,0,0,2,2,0,0,0,2,2,1,3,0,3,1,1,1,1,2,1,3,3,2,0,0,3,3,
        3,0,0,2,1,3,1,2,1,1,2,2,0,0,0,0,0,0,2,2,2,1,1,1,1,1,0,2,2,2,1,1,1,3,0,0,0,0,0,
        0,1,1,1,3,3,1,3,1,2,2,3,0,0,0,0,3,3,1,2,0,2,1,1,1,0,2,2,2,1,1,1,3,0,0,0,3,3,1,
        3,1,2,2,3,0,0,2,2,1,1,0,0,2,0,0,3,1,2,1,3,-1
    },
    {
        2,0,0,3,3,1,3,1,2,3,1,1,3,3,3,0,0,2,0,2,3,1,3,3,0,2,1,1,1,2,2,2,0,0,3,3,3,2,2,
        2,2,0,0,2,2,1,1,3,3,3,3,0,2,3,0,2,0,0,0,3,3,3,3,3,3,3,3,3,1,1,1,1,1,2,2,0,3,1,
        3,0,0,0,0,3,0,2,2,2,1,2,0,0,0,2,0,0,3,3,1,2,2,2,2,2,2,0,2,1,3,3,3,3,3,1,1,1,2,
        2,2,2,2,1,1,1,1,1,2,2,2,0,0,3,3,2,2,1,1,3,3,3,0,0,0,0,2,0,3,3,3,3,3,3,3,3,3,0,
        3,1,1,1,1,3,1,2,1,2,2,0,3,1,3,0,0,0,0,3,0,2,2,2,1,2,0,0,0,2,0,0,3,3,1,2,2,2,2,
        2,0,2,1,3,3,3,3,1,1,1,2,2,2,2,2,1,1,1,1,1,2,0,1,2,2,0,0,3,3,1,3,0,0,0,2,0,3,3,
        3,3,3,3,3,3,3,0,3,1,1,1,1,3,1,2,1,2,2,0,3,1,3,0,0,0,0,3,0,2,2,2,1,2,0,0,0,2,0,
        0,3,3,1,2,2,2,2,1,2,0,3,0,2,2,3,3,1,3,3,1,1,1,2,2,2,2,2,1,1,1,1,1,1,1,3,3,3,0,
        0,2,2,3,3,1,1,2,2,2,0,0,0,0,0,0,2,0,3,3,3,3,3,3,3,3,3,0,3,1,1,1,1,3,1,2,1,2,2,
        0,3,1,3,0,0,0,0,3,0,2,2,2,1,2,0,0,0,2,0,0,3,3,1,2,2,2,2,1,2,0,3,0,2,1,3,3,3,1,
        1,1,2,2,2,2,2,1,1,1,1,1,3,3,3,0,2,2,1,2,0,0,0,0,2,0,3,3,3,3,3,3,3,3,3,0,3,1,1,
        1,1,3,1,2,1,2,2,0,3,1,3,0,0,0,0,3,0,2,2,2,1,2,0,0,0,2,0,0,3,3,1,2,2,2,2,2,2,3,
        3,3,3,1,1,1,2,2,2,2,2,1,1,1,1,3,3,3,3,3,1,2,2,2,2,3,3,1,1,2,2,2,0,0,0,0,0,0,2,
        0,3,3,3,3,3,3,3,3,3,0,3,1,1,1,1,3,1,2,1,2,2,0,3,1,3,0,0,0,0,3,0,2,2,2,1,2,0,0,
        0,2,0,0,3,3,1,2,2,2,2,2,-1
    }
};

typedef struct
{
    char grid[MAX_ROWS][MAX_COLS + 1];
    int  rows;
    int  cols;
    int  player_row;
    int  player_col;
    int  current_level;
} GameState;

static GameState g_game;

#define MAX_HISTORY 1000
static GameState history[MAX_HISTORY];
static int       history_count = 0;

static void push_history(void)
{
    if (history_count < MAX_HISTORY)
    {
        history[history_count++] = g_game;
    }
}

static int pop_history(void)
{
    if (history_count > 0)
    {
        g_game = history[--history_count];
        return 1;
    }
    return 0;
}

static void clear_history(void)
{
    history_count = 0;
}

static int g_step_count = 0;

#define MAX_SOLUTION_LEN 1000
static int solution_moves[MAX_SOLUTION_LEN];
static int solution_len = 0;

static int dr_arr[4] = { -1, 1,  0, 0 };
static int dc_arr[4] = { 0, 0, -1, 1 };

static int bot_active = 0;
static int bot_step = 0;
static double bot_last_t = 0.0;
static double bot_interval = 0.18;
static int generator_mode = 0;

static void load_solution(void)
{
    solution_len = 0;
    if (g_game.current_level < 0 || g_game.current_level >= NUM_LEVELS)
    {
        return;
    }
    const int* src = static_solutions[g_game.current_level];
    int i = 0;
    while (i < MAX_SOLUTION_LEN && src[i] != -1)
    {
        solution_moves[i] = src[i];
        i++;
    }
    solution_len = i;
}

static void apply_move(GameState* s, int move)
{
    int mdr = dr_arr[move];
    int mdc = dc_arr[move];
    int nr = s->player_row + mdr;
    int nc = s->player_col + mdc;
    if (nr < 0 || nr >= s->rows || nc < 0 || nc >= s->cols)
    {
        return;
    }
    char dest = s->grid[nr][nc];
    if (dest == TILE_WALL)
    {
        return;
    }
    if (dest == TILE_BOX || dest == TILE_BOX_ON_TARGET)
    {
        int br = nr + mdr;
        int bc = nc + mdc;
        if (br < 0 || br >= s->rows || bc < 0 || bc >= s->cols)
        {
            return;
        }
        char beh = s->grid[br][bc];
        if (beh == TILE_WALL || beh == TILE_BOX || beh == TILE_BOX_ON_TARGET)
        {
            return;
        }
        s->grid[nr][nc] = (dest == TILE_BOX_ON_TARGET) ? TILE_TARGET : TILE_EMPTY;
        s->grid[br][bc] = (beh == TILE_TARGET) ? TILE_BOX_ON_TARGET : TILE_BOX;
    }
    char cur = s->grid[s->player_row][s->player_col];
    s->grid[s->player_row][s->player_col] =
        (cur == TILE_PLAYER_ON_TARGET) ? TILE_TARGET : TILE_EMPTY;
    s->grid[nr][nc] =
        (s->grid[nr][nc] == TILE_TARGET) ? TILE_PLAYER_ON_TARGET : TILE_PLAYER;
    s->player_row = nr;
    s->player_col = nc;
}

static void load_level_into(GameState* s, int idx)
{
    const char** src = levels[idx];
    s->current_level = idx;
    s->rows = 0;
    s->cols = 0;
    for (int r = 0; r < MAX_ROWS; r++)
    {
        if (src[r] == NULL)
        {
            break;
        }
        int len = (int)strlen(src[r]);
        if (len > s->cols)
        {
            s->cols = len;
        }
        memset(s->grid[r], ' ', MAX_COLS);
        memcpy(s->grid[r], src[r], len);
        s->grid[r][MAX_COLS] = '\0';
        s->rows++;
        for (int c = 0; c < len; c++)
        {
            if (src[r][c] == TILE_PLAYER || src[r][c] == TILE_PLAYER_ON_TARGET)
            {
                s->player_row = r;
                s->player_col = c;
            }
        }
    }
}

static void load_level(int idx)
{
    load_level_into(&g_game, idx);
    g_step_count = 0;
    bot_active = 0;
    bot_step = 0;
    generator_mode = 0;
    load_solution();
}

static int check_win(void)
{
    for (int r = 0; r < g_game.rows; r++)
    {
        for (int c = 0; c < g_game.cols; c++)
        {
            if (g_game.grid[r][c] == TILE_BOX)
            {
                return 0;
            }
        }
    }
    return 1;
}

static int count_boxes_on_target(const GameState* s)
{
    int n = 0;
    for (int r = 0; r < s->rows; r++)
    {
        for (int c = 0; c < s->cols; c++)
        {
            if (s->grid[r][c] == TILE_BOX_ON_TARGET)
            {
                n++;
            }
        }
    }
    return n;
}

static int hint_step = 0;
static int hint_hl_r = -1;
static int hint_hl_c = -1;
static int hint_shown = 0;

static GameState hint_checkpoint;
static int       hint_checkpoint_history = 0;

static void hint_clear_display(void)
{
    hint_shown = 0;
    hint_hl_r = -1;
    hint_hl_c = -1;
}

static void hint_full_reset(void)
{
    hint_step = 0;
    hint_checkpoint_history = 0;
    hint_clear_display();
}

static void hint_display(void)
{
    if (hint_step >= solution_len)
    {
        return;
    }
    int move = solution_moves[hint_step];
    hint_hl_r = g_game.player_row + dr_arr[move];
    hint_hl_c = g_game.player_col + dc_arr[move];
    if (hint_hl_r < 0 || hint_hl_r >= g_game.rows ||
        hint_hl_c < 0 || hint_hl_c >= g_game.cols)
    {
        hint_hl_r = -1;
        hint_hl_c = -1;
        return;
    }
    hint_shown = 1;
    hint_checkpoint = g_game;
    hint_checkpoint_history = history_count;
}

static void hint_on_key_h(void)
{
    if (solution_len == 0 || g_game.current_level < 0)
    {
        return;
    }
    if (hint_step >= solution_len)
    {
        return;
    }
    if (hint_shown)
    {
        return;
    }
    if (history_count < hint_checkpoint_history)
    {
        hint_step = 0;
        hint_checkpoint_history = 0;
    }
    hint_display();
}

static void hint_on_player_move(int direction)
{
    hint_clear_display();
    if (direction < 0)
    {
        return;
    }
    if (hint_step >= solution_len)
    {
        return;
    }
    int expected = solution_moves[hint_step];
    if (direction == expected)
    {
        hint_step++;
        hint_checkpoint_history = history_count;
    }
}

static void move_player(int mdr, int mdc)
{
    if (bot_active)
    {
        return;
    }

    int nr = g_game.player_row + mdr;
    int nc = g_game.player_col + mdc;
    if (nr < 0 || nr >= g_game.rows || nc < 0 || nc >= g_game.cols)
    {
        return;
    }
    char dest = g_game.grid[nr][nc];
    if (dest == TILE_WALL)
    {
        return;
    }
    if (dest == TILE_BOX || dest == TILE_BOX_ON_TARGET)
    {
        int br = nr + mdr;
        int bc = nc + mdc;
        if (br < 0 || br >= g_game.rows || bc < 0 || bc >= g_game.cols)
        {
            return;
        }
        char beh = g_game.grid[br][bc];
        if (beh == TILE_WALL || beh == TILE_BOX || beh == TILE_BOX_ON_TARGET)
        {
            return;
        }
    }

    push_history();

    int direction = -1;
    if (mdr == -1 && mdc == 0)
    {
        direction = 0;
    }
    if (mdr == 1 && mdc == 0)
    {
        direction = 1;
    }
    if (mdr == 0 && mdc == -1)
    {
        direction = 2;
    }
    if (mdr == 0 && mdc == 1)
    {
        direction = 3;
    }

    if (dest == TILE_BOX || dest == TILE_BOX_ON_TARGET)
    {
        int br = nr + mdr;
        int bc = nc + mdc;
        char beh = g_game.grid[br][bc];
        g_game.grid[nr][nc] = (dest == TILE_BOX_ON_TARGET) ? TILE_TARGET : TILE_EMPTY;
        g_game.grid[br][bc] = (beh == TILE_TARGET) ? TILE_BOX_ON_TARGET : TILE_BOX;
    }
    char cur = g_game.grid[g_game.player_row][g_game.player_col];
    g_game.grid[g_game.player_row][g_game.player_col] =
        (cur == TILE_PLAYER_ON_TARGET) ? TILE_TARGET : TILE_EMPTY;
    g_game.grid[nr][nc] =
        (g_game.grid[nr][nc] == TILE_TARGET) ? TILE_PLAYER_ON_TARGET : TILE_PLAYER;
    g_game.player_row = nr;
    g_game.player_col = nc;
    g_step_count++;

    hint_on_player_move(direction);
}

static void bot_start(void)
{
    if (solution_len == 0)
    {
        return;
    }
    load_level(g_game.current_level);
    clear_history();
    load_solution();
    hint_full_reset();
    bot_active = 1;
    bot_step = 0;
    bot_last_t = glfwGetTime();
}

static void update_bot(void)
{
    if (!bot_active)
    {
        return;
    }
    if (bot_step >= solution_len)
    {
        bot_active = 0;
        return;
    }
    double now = glfwGetTime();
    if (now - bot_last_t < bot_interval)
    {
        return;
    }
    bot_last_t = now;
    apply_move(&g_game, solution_moves[bot_step++]);
    g_step_count++;
    if (check_win())
    {
        bot_active = 0;
        int next = (g_game.current_level + 1) % NUM_LEVELS;
        load_level(next);
        clear_history();
        hint_full_reset();
    }
}

static int is_corner_deadlock(const GameState* s, int r, int c)
{
    int wu = (r <= 0) || (s->grid[r - 1][c] == TILE_WALL);
    int wd = (r >= s->rows - 1) || (s->grid[r + 1][c] == TILE_WALL);
    int wl = (c <= 0) || (s->grid[r][c - 1] == TILE_WALL);
    int wr = (c >= s->cols - 1) || (s->grid[r][c + 1] == TILE_WALL);
    return (wu || wd) && (wl || wr);
}

static int is_wall_deadlock(const GameState* s, int r, int c)
{
    int wu = (r <= 0) || (s->grid[r - 1][c] == TILE_WALL);
    int wd = (r >= s->rows - 1) || (s->grid[r + 1][c] == TILE_WALL);
    if (wu || wd)
    {
        int has = (s->grid[r][c] == TILE_TARGET);
        for (int cc = c - 1; cc >= 0 && !has; cc--)
        {
            if (s->grid[r][cc] == TILE_WALL)
            {
                break;
            }
            int sw = wu ? (cc <= 0 || s->grid[r - 1][cc] == TILE_WALL)
                : (cc >= s->cols - 1 || s->grid[r + 1][cc] == TILE_WALL);
            if (!sw)
            {
                has = 1;
                break;
            }
            if (s->grid[r][cc] == TILE_TARGET)
            {
                has = 1;
            }
        }
        for (int cc = c + 1; cc < s->cols && !has; cc++)
        {
            if (s->grid[r][cc] == TILE_WALL)
            {
                break;
            }
            int sw = wu ? (cc <= 0 || s->grid[r - 1][cc] == TILE_WALL)
                : (cc >= s->cols - 1 || s->grid[r + 1][cc] == TILE_WALL);
            if (!sw)
            {
                has = 1;
                break;
            }
            if (s->grid[r][cc] == TILE_TARGET)
            {
                has = 1;
            }
        }
        if (!has)
        {
            return 1;
        }
    }
    int wl = (c <= 0) || (s->grid[r][c - 1] == TILE_WALL);
    int wr2 = (c >= s->cols - 1) || (s->grid[r][c + 1] == TILE_WALL);
    if (wl || wr2)
    {
        int has = (s->grid[r][c] == TILE_TARGET);
        for (int rr = r - 1; rr >= 0 && !has; rr--)
        {
            if (s->grid[rr][c] == TILE_WALL)
            {
                break;
            }
            int sw = wl ? (c <= 0 || s->grid[rr][c - 1] == TILE_WALL)
                : (c >= s->cols - 1 || s->grid[rr][c + 1] == TILE_WALL);
            if (!sw)
            {
                has = 1;
                break;
            }
            if (s->grid[rr][c] == TILE_TARGET)
            {
                has = 1;
            }
        }
        for (int rr = r + 1; rr < s->rows && !has; rr++)
        {
            if (s->grid[rr][c] == TILE_WALL)
            {
                break;
            }
            int sw = wl ? (c <= 0 || s->grid[rr][c - 1] == TILE_WALL)
                : (c >= s->cols - 1 || s->grid[rr][c + 1] == TILE_WALL);
            if (!sw)
            {
                has = 1;
                break;
            }
            if (s->grid[rr][c] == TILE_TARGET)
            {
                has = 1;
            }
        }
        if (!has)
        {
            return 1;
        }
    }
    return 0;
}

static int is_safe_for_box(const GameState* s, int r, int c)
{
    return !is_corner_deadlock(s, r, c) && !is_wall_deadlock(s, r, c);
}

#define MAXB 8              
#define BFS_LIMIT 1000000
#define HASH_SIZE (1 << 21)

typedef struct
{
    unsigned short player;
    unsigned short box[MAXB];
} PState;

static unsigned int hash_state(const PState* p)
{
    unsigned int h = 2166136261u;
    const unsigned char* b = (const unsigned char*)p;
    for (size_t i = 0; i < sizeof(PState); i++)
    {
        h ^= b[i];
        h *= 16777619u;
    }
    return h;
}

static int visited_insert(int* table, const PState* pool, int new_index)
{
    unsigned int h = hash_state(&pool[new_index]) & (HASH_SIZE - 1);
    while (table[h] != -1)
    {
        if (memcmp(&pool[table[h]], &pool[new_index], sizeof(PState)) == 0)
        {
            return 0;
        }
        h = (h + 1) & (HASH_SIZE - 1);
    }
    table[h] = new_index;
    return 1;
}

static int is_solvable(const GameState* s)
{
    static const int dr[4] = { -1, 1, 0, 0 };
    static const int dc[4] = { 0, 0, -1, 1 };

    char wall[MAX_ROWS][MAX_COLS] = { 0 };
    char target[MAX_ROWS][MAX_COLS] = { 0 };
    PState start;
    memset(&start, 0, sizeof(start));
    int nb = 0;

    for (int r = 0; r < s->rows; r++){
        for (int c = 0; c < s->cols; c++){
            char ch = s->grid[r][c];
            wall[r][c] = (ch == TILE_WALL);
            target[r][c] = (ch == TILE_TARGET || ch == TILE_BOX_ON_TARGET || ch == TILE_PLAYER_ON_TARGET);
            if (ch == TILE_BOX || ch == TILE_BOX_ON_TARGET){
                if (nb == MAXB) { return 0; }
                start.box[nb++] = (unsigned short)(r * MAX_COLS + c);
            }
        }
    }
    start.player = (unsigned short)(s->player_row * MAX_COLS + s->player_col);
    for (int i = 1; i < nb; i++) {
        unsigned short v = start.box[i];
        int j = i - 1;
        while (j >= 0 && start.box[j] > v) { start.box[j + 1] = start.box[j]; j--; }
        start.box[j + 1] = v;
    }

    PState* pool = (PState*)malloc((size_t)BFS_LIMIT * sizeof(PState));
    int* table = (int*)malloc((size_t)HASH_SIZE * sizeof(int));
    if (!pool || !table) { free(pool); free(table); return 0; }
    memset(table, 0xFF, (size_t)HASH_SIZE * sizeof(int)); 

    int head = 0, tail = 0;
    pool[tail] = start;
    visited_insert(table, pool, tail);
    tail++;

    while (head < tail)
    {
        PState cur = pool[head++];

        int win = 1;
        for (int i = 0; i < nb && win; i++)
        {
            win = target[cur.box[i] / MAX_COLS][cur.box[i] % MAX_COLS];
        }
        if (win) { free(pool); free(table); return 1; }

        int pr = cur.player / MAX_COLS, pc = cur.player % MAX_COLS;
        for (int d = 0; d < 4; d++)
        {
            int nr = pr + dr[d], nc = pc + dc[d];
            if (nr < 0 || nr >= s->rows || nc < 0 || nc >= s->cols || wall[nr][nc]) { continue; }

            PState nx = cur;
            int pos = nr * MAX_COLS + nc;
            int bi = -1;
            for (int i = 0; i < nb; i++) { if (cur.box[i] == pos) { bi = i; break; } }
            if (bi >= 0)
            {
                int br = nr + dr[d], bc = nc + dc[d];
                if (br < 0 || br >= s->rows || bc < 0 || bc >= s->cols || wall[br][bc]) { continue; }
                int bpos = br * MAX_COLS + bc;
                int blocked = 0;
                for (int i = 0; i < nb; i++) { if (cur.box[i] == bpos) { blocked = 1; break; } }
                if (blocked) { continue; }
                nx.box[bi] = (unsigned short)bpos;
                for (int i = bi; i > 0 && nx.box[i - 1] > nx.box[i]; i--)
                {
                    unsigned short t = nx.box[i]; nx.box[i] = nx.box[i - 1]; nx.box[i - 1] = t;
                }
                for (int i = bi; i < nb - 1 && nx.box[i + 1] < nx.box[i]; i++)
                {
                    unsigned short t = nx.box[i]; nx.box[i] = nx.box[i + 1]; nx.box[i + 1] = t;
                }
            }
            nx.player = (unsigned short)pos;

            if (tail >= BFS_LIMIT) { continue; }
            pool[tail] = nx;
            if (visited_insert(table, pool, tail)) { tail++; }
        }
    }
    free(pool); free(table);
    return 0;
}

static int generate_level(GameState* out)
{
    int W = 7 + rand() % 3;
    int H = 7 + rand() % 3;
    int NB = 2 + rand() % 2;
    out->rows = H;
    out->cols = W;
    out->current_level = -1;
    for (int r = 0; r < H; r++)
    {
        for (int c = 0; c < W; c++)
        {
            out->grid[r][c] = (r == 0 || r == H - 1 || c == 0 || c == W - 1) ? TILE_WALL : TILE_EMPTY;
        }
        out->grid[r][W] = '\0';
    }
    int extra = rand() % 3;
    for (int k = 0; k < extra * 10; k++)
    {
        int r = 1 + rand() % (H - 2);
        int c = 1 + rand() % (W - 2);
        if (out->grid[r][c] != TILE_EMPTY)
        {
            continue;
        }
        out->grid[r][c] = TILE_WALL;
        int bad = 0;
        for (int dr = -1; dr <= 1 && !bad; dr++)
        {
            for (int dc = -1; dc <= 1 && !bad; dc++)
            {
                int nr = r + dr;
                int nc = c + dc;
                if (nr > 0 && nr < H - 1 && nc > 0 && nc < W - 1 &&
                    out->grid[nr][nc] == TILE_EMPTY)
                {
                    if (is_corner_deadlock(out, nr, nc))
                    {
                        bad = 1;
                    }
                }
            }
        }
        if (bad)
        {
            out->grid[r][c] = TILE_EMPTY;
        }
        if (!bad && k >= extra)
        {
            break;
        }
    }
    int tc[MAX_ROWS * MAX_COLS];
    int tn = 0;
    for (int r = 1; r < H - 1; r++)
    {
        for (int c = 1; c < W - 1; c++)
        {
            if (out->grid[r][c] == TILE_EMPTY && !is_corner_deadlock(out, r, c))
            {
                tc[tn++] = r * MAX_COLS + c;
            }
        }
    }
    if (tn < NB * 2 + 1)
    {
        return 0;
    }
    for (int i = tn - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int t = tc[i];
        tc[i] = tc[j];
        tc[j] = t;
    }
    for (int i = 0; i < NB; i++)
    {
        int r = tc[i] / MAX_COLS;
        int c = tc[i] % MAX_COLS;
        out->grid[r][c] = TILE_TARGET;
    }
    int bc2[MAX_ROWS * MAX_COLS];
    int bn = 0;
    for (int r = 1; r < H - 1; r++)
    {
        for (int c = 1; c < W - 1; c++)
        {
            if (out->grid[r][c] == TILE_EMPTY)
            {
                bc2[bn++] = r * MAX_COLS + c;
            }
        }
    }
    for (int i = bn - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int t = bc2[i];
        bc2[i] = bc2[j];
        bc2[j] = t;
    }
    int placed = 0;
    for (int i = 0; i < bn && placed < NB; i++)
    {
        int r = bc2[i] / MAX_COLS;
        int c = bc2[i] % MAX_COLS;
        if (is_safe_for_box(out, r, c))
        {
            out->grid[r][c] = TILE_BOX;
            placed++;
        }
    }
    if (placed < NB)
    {
        return 0;
    }
    int pp = 0;
    for (int i = 0; i < bn && !pp; i++)
    {
        int r = bc2[i] / MAX_COLS;
        int c = bc2[i] % MAX_COLS;
        if (out->grid[r][c] == TILE_EMPTY)
        {
            out->grid[r][c] = TILE_PLAYER;
            out->player_row = r;
            out->player_col = c;
            pp = 1;
        }
    }
    return pp;
}

static int generate_solvable_level_impl(GameState* out)
{
    for (int i = 0; i < 500; i++)
    {
        if (generate_level(out) && is_solvable(out))
        {
            return 1;
        }
    }
    return 0;
}

static int generate_solvable_level(GameState* out)
{
    clock_t t0 = clock();
    int ok = generate_solvable_level_impl(out);
    printf("generate_solvable_level: %.1f ms\n", (clock() - t0) * 1000.0 / CLOCKS_PER_SEC);
    return ok;
}

static void draw_digit(int d, float cx, float cy, float h)
{
    float w = h * 0.5f;
    float x0 = cx - w * 0.5f;
    float x1 = cx + w * 0.5f;
    float y0 = cy - h * 0.5f;
    float y1 = cy + h * 0.5f;
    float ym = cy;
    int map[10][7] = {
        {1,1,1,0,1,1,1},
        {0,0,1,0,0,1,0},
        {1,0,1,1,1,0,1},
        {1,0,1,1,0,1,1},
        {0,1,1,1,0,1,0},
        {1,1,0,1,0,1,1},
        {1,1,0,1,1,1,1},
        {1,0,1,0,0,1,0},
        {1,1,1,1,1,1,1},
        {1,1,1,1,0,1,1}
    };
    int* s = map[d];
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    if (s[0])
    {
        glVertex2f(x0, y0);
        glVertex2f(x1, y0);
    }
    if (s[1])
    {
        glVertex2f(x0, y0);
        glVertex2f(x0, ym);
    }
    if (s[2])
    {
        glVertex2f(x1, y0);
        glVertex2f(x1, ym);
    }
    if (s[3])
    {
        glVertex2f(x0, ym);
        glVertex2f(x1, ym);
    }
    if (s[4])
    {
        glVertex2f(x0, ym);
        glVertex2f(x0, y1);
    }
    if (s[5])
    {
        glVertex2f(x1, ym);
        glVertex2f(x1, y1);
    }
    if (s[6])
    {
        glVertex2f(x0, y1);
        glVertex2f(x1, y1);
    }
    glEnd();
}

static void draw_step_counter(void)
{
    char buf[12];
    snprintf(buf, sizeof(buf), "%d", g_step_count);
    int len = (int)strlen(buf);
    float dh = 0.65f;
    float dw = 0.40f;
    float gap = 0.08f;
    float cols = (float)g_game.cols;
    float cx = cols - 0.25f;
    float cy = 0.55f;
    float tw = len * (dw + gap);
    glColor3f(0.10f, 0.10f, 0.15f);
    glRectf(cx - tw - 0.15f, cy - dh * 0.5f - 0.10f,
        cx + 0.10f, cy + dh * 0.5f + 0.10f);
    glColor3f(0.90f, 0.90f, 0.95f);
    float x = cx - 0.10f;
    for (int i = len - 1; i >= 0; i--)
    {
        draw_digit(buf[i] - '0', x - dw * 0.5f, cy, dh);
        x -= dw + gap;
    }
}

static void render(void)
{
    glClearColor(0.12f, 0.12f, 0.18f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, g_game.cols, g_game.rows, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    for (int r = 0; r < g_game.rows; r++)
    {
        for (int c = 0; c < g_game.cols; c++)
        {
            char tile = g_game.grid[r][c];
            float x = (float)c;
            float y = (float)r;
            if (tile != ' ')
            {
                glColor3f(0.25f, 0.25f, 0.30f);
                glRectf(x + 0.05f, y + 0.05f, x + 0.95f, y + 0.95f);
            }
            switch (tile)
            {
            case TILE_WALL:
                glColor3f(0.55f, 0.55f, 0.60f);
                glRectf(x + 0.03f, y + 0.03f, x + 0.97f, y + 0.97f);
                break;
            case TILE_TARGET:
                glColor3f(0.95f, 0.85f, 0.20f);
                glBegin(GL_TRIANGLE_FAN);
                glVertex2f(x + 0.5f, y + 0.5f);
                for (int i = 0; i <= 20; i++)
                {
                    float a = 2.0f * 3.14159f * i / 20;
                    glVertex2f(x + 0.5f + 0.3f * cosf(a), y + 0.5f + 0.3f * sinf(a));
                }
                glEnd();
                break;
            case TILE_PLAYER_ON_TARGET:
                glColor3f(0.95f, 0.85f, 0.20f);
                glBegin(GL_TRIANGLE_FAN);
                glVertex2f(x + 0.5f, y + 0.5f);
                for (int i = 0; i <= 20; i++)
                {
                    float a = 2.0f * 3.14159f * i / 20;
                    glVertex2f(x + 0.5f + 0.3f * cosf(a), y + 0.5f + 0.3f * sinf(a));
                }
                glEnd();
            case TILE_PLAYER:
                glColor3f(0.20f, 0.80f, 0.40f);
                glBegin(GL_TRIANGLES);
                glVertex2f(x + 0.5f, y + 0.08f);
                glVertex2f(x + 0.15f, y + 0.78f);
                glVertex2f(x + 0.85f, y + 0.78f);
                glEnd();
                break;
            case TILE_BOX:
                glColor3f(0.85f, 0.50f, 0.10f);
                glRectf(x + 0.1f, y + 0.1f, x + 0.9f, y + 0.9f);
                break;
            case TILE_BOX_ON_TARGET:
                glColor3f(0.65f, 0.20f, 0.85f);
                glRectf(x + 0.1f, y + 0.1f, x + 0.9f, y + 0.9f);
                break;
            default:
                break;
            }
        }
    }

    if (hint_shown && hint_hl_r >= 0 && hint_hl_c >= 0 &&
        hint_hl_r < g_game.rows && hint_hl_c < g_game.cols)
    {
        glColor3f(0.20f, 0.65f, 1.0f);
        glRectf((float)hint_hl_c + 0.08f, (float)hint_hl_r + 0.08f,
            (float)hint_hl_c + 0.92f, (float)hint_hl_r + 0.92f);
    }

    if (bot_active)
    {
        glColor3f(1.0f, 0.25f, 0.25f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0.5f, 0.5f);
        for (int i = 0; i <= 16; i++)
        {
            float a = 2.0f * 3.14159f * i / 16;
            glVertex2f(0.5f + 0.3f * cosf(a), 0.5f + 0.3f * sinf(a));
        }
        glEnd();
    }

    draw_step_counter();
}

static void key_callback(GLFWwindow* win, int key, int sc, int action, int mods)
{
    (void)sc;
    (void)mods;
    if (action != GLFW_PRESS && action != GLFW_REPEAT)
    {
        return;
    }

    switch (key)
    {
    case GLFW_KEY_UP:
    case GLFW_KEY_W:
        move_player(-1, 0);
        break;
    case GLFW_KEY_DOWN:
    case GLFW_KEY_S:
        move_player(1, 0);
        break;
    case GLFW_KEY_LEFT:
    case GLFW_KEY_A:
        move_player(0, -1);
        break;
    case GLFW_KEY_RIGHT:
    case GLFW_KEY_D:
        move_player(0, 1);
        break;

    case GLFW_KEY_H:
        if (!generator_mode && solution_len > 0 && !bot_active)
        {
            if (hint_shown)
            {
                break;
            }
            if (hint_step > 0 && history_count != hint_checkpoint_history)
            {
                while (history_count > hint_checkpoint_history && history_count > 0)
                {
                    g_game = history[--history_count];
                    if (g_step_count > 0)
                    {
                        g_step_count--;
                    }
                }
                g_game = hint_checkpoint;
                history_count = hint_checkpoint_history;
            }
            hint_on_key_h();
        }
        break;

    case GLFW_KEY_R:
        if (generator_mode)
        {
            if (generate_solvable_level(&g_game))
            {
                clear_history();
                g_step_count = 0;
                solution_len = 0;
                hint_full_reset();
            }
        }
        else
        {
            load_level(g_game.current_level);
            clear_history();
            hint_full_reset();
        }
        break;

    case GLFW_KEY_N:
        if (!generator_mode && g_game.current_level + 1 < NUM_LEVELS)
        {
            load_level(g_game.current_level + 1);
            clear_history();
            hint_full_reset();
        }
        break;

    case GLFW_KEY_P:
        if (!generator_mode && g_game.current_level > 0)
        {
            load_level(g_game.current_level - 1);
            clear_history();
            hint_full_reset();
        }
        break;

    case GLFW_KEY_F1:
        generator_mode = 0;
        bot_active = 0;
        hint_full_reset();
        load_level(0);
        clear_history();
        break;

    case GLFW_KEY_U:
        if (!bot_active && pop_history())
        {
            if (g_step_count > 0)
            {
                g_step_count--;
            }
            hint_clear_display();
            if (history_count < hint_checkpoint_history && hint_step > 0)
            {
                hint_step--;
            }
        }
        break;

    case GLFW_KEY_B:
        if (!generator_mode && solution_len > 0)
        {
            if (bot_active)
            {
                bot_active = 0;
            }
            else
            {
                hint_full_reset();
                bot_start();
            }
        }
        break;

    case GLFW_KEY_G:
        if (generate_solvable_level(&g_game))
        {
            generator_mode = 1;
            bot_active = 0;
            solution_len = 0;
            clear_history();
            g_step_count = 0;
            hint_full_reset();
            printf("Generator mode. F1 to return, R to regenerate.\n");
        }
        else
        {
            printf("Generation failed.\n");
        }
        break;

    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(win, GLFW_TRUE);
        break;
    }

    if (check_win())
    {
        if (generator_mode)
        {
            printf("Complete! Steps: %d\n", g_step_count);
            if (generate_solvable_level(&g_game))
            {
                clear_history();
                g_step_count = 0;
                solution_len = 0;
                hint_full_reset();
            }
            else
            {
                printf("Generation failed.\n");
            }
        }
        else
        {
            printf("Level %d complete! Steps: %d\n", g_game.current_level + 1, g_step_count);
            int next = (g_game.current_level + 1) % NUM_LEVELS;
            load_level(next);
            clear_history();
            hint_full_reset();
        }
    }
}

static void run_time(int count)
{
    GameState g;
    int ok = 0;
    srand(12345);
    clock_t t0 = clock();
    for (int i = 0; i < count; i++)
    {
        ok += generate_solvable_level_impl(&g);
    }
    double ms = (clock() - t0) * 1000.0 / CLOCKS_PER_SEC;
    printf("levels=%d ok=%d total=%.1f ms avg=%.2f ms/level\n", count, ok, ms, ms / count);
}

int main(int argc, char** argv)
{
    if (argc > 1 && strcmp(argv[1], "-time") == 0)
    {
        run_time(argc > 2 ? atoi(argv[2]) : 100);
        return 0;
    }
    srand((unsigned int)time(NULL));
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    GLFWwindow* win = glfwCreateWindow(700, 700, "Sokoban", NULL, NULL);
    glfwMakeContextCurrent(win);
    glfwSwapInterval(1);
    glewExperimental = GL_TRUE;
    glewInit();
    glfwSetKeyCallback(win, key_callback);
    load_level(0);
    clear_history();
    hint_full_reset();
    printf("Controls:\n");
    printf("  Arrows/WASD move | U undo\n");
    printf("  H - hint | B - bot | G - generator\n");
    printf("  F1 - back to level 1 | R restart | N/P level | ESC exit\n");
    while (!glfwWindowShouldClose(win))
    {
        glfwMakeContextCurrent(win);
        int w, h;
        glfwGetFramebufferSize(win, &w, &h);
        glViewport(0, 0, w, h);
        update_bot();
        render();
        glfwSwapBuffers(win);
        glfwPollEvents();
    }
    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
}