
typedef struct listnode listnode;

typedef struct list listout;

struct listnode // linkied list of linked lists for storing postal codes
{
    int postal;
    char name;
    char surname;
    listnode *next;
    listnode *prev;
};

struct list
{
    int postal;
    listnode *node;
    listout *next;
};


listout* list_create();

listnode* list_innerinsert(listnode*,int postal, char name, char surname);

listout* list_insert(listout*,int postal,char name, char surname);

void list_print(listout* list);

listnode* list_search(listout *, int postal,char name);

