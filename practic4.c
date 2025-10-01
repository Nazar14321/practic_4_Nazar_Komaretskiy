#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    const char *title;     // book title
    double price;          // price
    int pages;             // pages
    const char *language;  // language
    double weight_kg;      // weight
    int year;              // year
    struct node *next;     // next node
    struct node *prev;     // prev node
} node;

static node* make(
    const char *title, double price, int pages,
    const char *language, double weight_kg, int year
) {
    node *n = (node*)malloc(sizeof *n);   // alloc node
    if (!n) { perror("malloc"); exit(1); }
    n->title = title;                     // init fields
    n->price = price;
    n->pages = pages;
    n->language = language;
    n->weight_kg = weight_kg;
    n->year = year;
    n->next = n->prev = NULL;            // no links yet
    return n;
}

static void print_one(const node *b) {   // print single book
    printf("Title: %s\n"
           "Price: $%.2f\n"
           "Pages: %d\n"
           "Language: %s\n"
           "Weight: %.2f kg\n"
           "Year: %d\n",
           b->title, b->price, b->pages, b->language, b->weight_kg, b->year);
}

static void print_all(const node *head) { // print ring
    if (!head) return;
    const node *p = head;
    do {
        print_one(p);
        puts("----------------------------------------");
        p = p->next;
    } while (p != head);
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);     // unbuffered stdout

    // create nodes
    node *one   = make("Harry Potter and the Philosopher's Stone",  9.99, 223, "English", 0.35, 1997);
    node *two   = make("Harry Potter and the Chamber of Secrets",  10.99, 251, "English", 0.40, 1998);
    node *three = make("Harry Potter and the Prisoner of Azkaban", 11.99, 317, "English", 0.45, 1999);
    node *four  = make("Harry Potter and the Goblet of Fire",      14.99, 636, "English", 0.75, 2000);
    node *five  = make("Harry Potter and the Order of the Phoenix",16.99, 766, "English", 0.85, 2003);
    node *six   = make("Harry Potter and the Half-Blood Prince",   15.99, 607, "English", 0.80, 2005);
    node *seven = make("Harry Potter and the Deathly Hallows",     17.99, 607, "English", 0.82, 2007);

    // link into doubly circular list
    one->prev = seven;  one->next = two;
    two->prev = one;    two->next = three;
    three->prev = two;  three->next = four;
    four->prev = three; four->next = five;
    five->prev = four;  five->next = six;
    six->prev = five;   six->next = seven;
    seven->prev = six;  seven->next = one;

    node *head = one;   // list head
    node *temp = head;  // current pointer
    char input_command; // user command

    printf("if you need help write: h\n");

    // REPL loop
    while (1) {
        printf("Input your command: ");
        if (scanf(" %c", &input_command) != 1) return 1;

        if (input_command=='n') {            // next
            temp = temp->next;
            print_one(temp);
        } else if (input_command=='p') {     // previous
            temp = temp->prev;
            print_one(temp);
        } else if (input_command=='c') {     // current
            printf("current book in series:\n%s\n", temp->title);
            print_one(temp);
        } else if (input_command=='h') {     // help
            printf("Hello, we have a few commands:\n"
                   "  n - next book in series\n"
                   "  p - previous book\n"
                   "  c - current\n"
                   "  a - all\n"
                   "  b - end program\n");
        } else if (input_command=='a') {     // print all
            print_all(head);
        } else if (input_command=='b') {     // break
            printf("Dobby is free\n");
            break;
        } else {
            printf("error\n");               // unknown command
        }
    }

    // free ring
    node *p = head;
    if (p) {
        do {
            node *next = p->next;            // save next
            free(p);                          // free current
            p = next;                         // advance
        } while (p && p != head);
    }

    return 0;
}
