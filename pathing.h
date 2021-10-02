// #define MAP_WIDTH 128
// #define MAP_HEIGHT 20
// #define NUM_OF_TILES 2560

// char playermap[NUM_OF_TILES];

struct Location{
    int x, y;
};

struct Node{
    struct Location key, value;
    struct Node *next;
};

struct Node *get_path(int x, int y);
struct Node get();
struct Node *get_neighbors(int x, int y);
struct Node *search();
int put_node(int vX, int vY, int fX, int fY);
int cmp(int vX, int vY, int fX, int fY);