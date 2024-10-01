//
//  assignment1.c
//  CSCI201
//
//  Created by Anthony Lu on 9/25/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FORWARD 0
#define BACKWARD 1

typedef struct PERSONNEL_REC
{
    char *last_name;
    char *first_name;
    char middle_initial;
    int age;
    int salary;
    long int id_num;
} PERSONNEL_REC;

typedef struct NODE
{
    PERSONNEL_REC *record;
    struct NODE *left;
    struct NODE *right;
} NODE;

typedef struct CELL
{
    PERSONNEL_REC *record;
    struct CELL *next;
    struct CELL *prev;
} CELL;

NODE *name_root = NULL;
NODE *age_root = NULL;
NODE *id_num_root = NULL;
NODE *salary_root = NULL;

CELL *head = NULL;

int compare_id_number(PERSONNEL_REC *p1, PERSONNEL_REC *p2)
{
    long int num1 = p1->id_num;
    long int num2 = p2->id_num;
    if (num1 < num2)
    {
        return -1;
    }
    else
    {
        return (num1 > num2);
    }
}

int compare_name(PERSONNEL_REC *p1, PERSONNEL_REC *p2)
{
    char *last1 = p1->last_name;
    char *last2 = p2->last_name;
    char *first1 = p1->first_name;
    char *first2 = p2->first_name;
    char mid1 = p1->middle_initial;
    char mid2 = p2->middle_initial;
    if (strcmp(last1, last2) != 0)
    {
        return strcmp(last1, last2);
    }
    else if (strcmp(first1, first2) != 0)
    {
        return strcmp(first1, first2);
    }
    else if (mid1 != mid2)
    {
        if (mid1 < mid2)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return compare_id_number(p1, p2);
    }
}

int compare_age(PERSONNEL_REC *p1, PERSONNEL_REC *p2)
{
    int age1 = p1->age;
    int age2 = p2->age;
    if (age1 < age2)
    {
        return -1;
    }
    else if (age1 > age2)
    {
        return 1;
    }
    else
    {
        return compare_name(p1, p2);
    }
}

int compare_salary(PERSONNEL_REC *p1, PERSONNEL_REC *p2)
{
    int salary1 = p1->salary;
    int salary2 = p2->salary;
    if (salary1 < salary2)
    {
        return -1;
    }
    else if (salary1 > salary2)
    {
        return 1;
    }
    else
    {
        return compare_name(p1, p2);
    }
}

PERSONNEL_REC *new_record(char *last_name, char *first_name, char middle_initial, int age, long int id_num, int salary)
{
    PERSONNEL_REC *new_rec = malloc(sizeof(PERSONNEL_REC));
    char *last = malloc(strlen(last_name) * sizeof(char) + 1);
    char *first = malloc(strlen(first_name) * sizeof(char) + 1);
    strcpy(last, last_name);
    strcpy(first, first_name);
    new_rec->last_name = last;
    new_rec->first_name = first;
    new_rec->middle_initial = middle_initial;
    new_rec->age = age;
    new_rec->salary = salary;
    new_rec->id_num = id_num;
    return new_rec;
}

PERSONNEL_REC *read_record(void)
{
    char *last_name = malloc(100 * sizeof(char));
    char *first_name = malloc(100 * sizeof(char));
    char middle_initial;
    int age;
    int salary;
    long int id_num;
    int res = scanf("%99s %99s %c %i %li %i", last_name, first_name, &middle_initial, &age, &id_num, &salary);
    if (res == EOF)
    {
        return NULL;
    }
    PERSONNEL_REC *new_rec = malloc(sizeof(PERSONNEL_REC));
    new_rec = new_record(last_name, first_name, middle_initial, age, id_num, salary);
    return new_rec;
}

void print_record(PERSONNEL_REC *rec)
{
    printf("%s %s %c %i %li %i\n", rec->last_name, rec->first_name, rec->middle_initial, rec->age, rec->id_num, rec->salary);
}

void insert_personnel_record(NODE **root, PERSONNEL_REC *rec, int (*compare)(PERSONNEL_REC *, PERSONNEL_REC *))
{
    if (*root)
    {
        int res = compare(rec, (*root)->record);
        if (res > 0)
        {
            if ((*root)->right)
            {
                insert_personnel_record(&(*root)->right, rec, compare);
            }
            else
            {
                NODE *newNode = malloc(sizeof(NODE));
                newNode->record = rec;
                newNode->left = NULL;
                newNode->right = NULL;
                (*root)->right = newNode;
            }
        }
        else
        {
            if ((*root)->left)
            {
                insert_personnel_record(&(*root)->left, rec, compare);
            }
            else
            {
                NODE *newNode = malloc(sizeof(NODE));
                newNode->record = rec;
                newNode->left = NULL;
                newNode->right = NULL;
                (*root)->left = newNode;
            }
        }
    }
    else
    {
        NODE *newNode = malloc(sizeof(NODE));
        newNode->record = rec;
        newNode->left = NULL;
        newNode->right = NULL;
        *root = newNode;
    }
}

void traverse_and_print_records(NODE *root)
{
    if (root == NULL)
    {
        return;
    }
    else
    {
        traverse_and_print_records(root->left);
        print_record(root->record);
        traverse_and_print_records(root->right);
    }
}

void insert_record_in_list(PERSONNEL_REC *rec)
{
    if (head)
    {
        CELL *new_cell = malloc(sizeof(CELL));
        new_cell->record = rec;
        new_cell->prev = head->prev;
        new_cell->next = head;
        head->prev->next = new_cell;
        head->prev = new_cell;
    }
    else
    {
        head = malloc(sizeof(CELL));
        head->record = rec;
        head->prev = head;
        head->next = head;
    }
}

void insert_from_tree_into_list(NODE *root)
{
    if (root == NULL)
    {
        return;
    }
    else
    {
        insert_from_tree_into_list(root->left);
        insert_record_in_list(root->record);
        insert_from_tree_into_list(root->right);
    }
}

void print_list(int direction)
{
    if (direction == 0)
    {
        CELL *curr = head;
        while (curr->next != head)
        {
            print_record(curr->record);
            curr = curr->next;
        }
        print_record(curr->record);
    }
    else
    {
        CELL *curr = head->prev;
        while (curr != head)
        {
            print_record(curr->record);
            curr = curr->prev;
        }
        print_record(curr->record);
    }
}

void print_n_records(int n)
{
    CELL *curr = head;
    for (int i = 0; i < n; i++)
    {
        print_record(curr->record);
        curr = curr->next;
    }
}

int main(void)
{
    PERSONNEL_REC *rec = read_record();
    while (rec != NULL)
    {
        insert_personnel_record(&name_root, rec, *compare_name);
        insert_personnel_record(&id_num_root, rec, *compare_id_number);
        insert_personnel_record(&age_root, rec, *compare_age);
        insert_personnel_record(&salary_root, rec, *compare_salary);
        rec = read_record();
    }

    printf("SORTED BY NAME\n");
    traverse_and_print_records(name_root);
    printf("SORTED BY ID NUM\n");
    traverse_and_print_records(id_num_root);
    printf("SORTED BY AGE\n");
    traverse_and_print_records(age_root);
    printf("SORTED BY SALARY\n");
    traverse_and_print_records(salary_root);

    insert_from_tree_into_list(name_root);
    printf("Printing list in forward direction:\n");
    print_list(FORWARD);

    printf("Printing list in backwards directions:\n");
    print_list(BACKWARD);

    printf("Printing 120 records\n");
    print_n_records(120);
}
