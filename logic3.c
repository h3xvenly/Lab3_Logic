#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

struct node
{
    char inf[256];          // данные 
    int priority;           // приоритет 
    struct node* next;      // указатель на солед. элем.
};

// отдельный односвязный список
struct list
{
    struct node* head;
    struct node* last;
};

//  структуры данных
struct list queue = { NULL, NULL };   
struct list stack = { NULL, NULL };   
struct list pqueue = { NULL, NULL };  

// чтение целого числа с проверкой
int read_int(const char* prompt)
{
    int x;
    printf("%s", prompt);
    while (scanf("%d", &x) != 1)
    {
        printf("Введите целое число: ");
        while (getchar() != '\n');
    }
    return x;
}

// создание нового элемента
struct node* get_struct(int need_priority)
{
    struct node* p = malloc(sizeof(struct node));

    if (p == NULL)
    {
        printf("Ошибка выделения памяти\n");
        exit(1);
    }

    printf("Введите название объекта (без пробелов): ");
    scanf("%255s", p->inf);

    if (need_priority)
        p->priority = read_int("Введите приоритет (целое число): ");
    else
        p->priority = 0;

    p->next = NULL;
    return p;
}

//очередь 
void queue_add(struct list* l)
{
    struct node* p = get_struct(0);

    if (l->head == NULL)
    {
        l->head = p;
        l->last = p;
    }
    else
    {
        l->last->next = p;
        l->last = p;
    }
    printf("Элемент добавлен в очередь\n");
}

// стек
void stack_push(struct list* l)
{
    struct node* p = get_struct(0);

    p->next = l->head;
    l->head = p;
    if (l->last == NULL)
        l->last = p;

    printf("Элемент добавлен в стек\n");
}

// приоритет очередь
void priority_add(struct list* l)
{
    struct node* p = get_struct(1);

    if (l->head == NULL || p->priority > l->head->priority)
    {
        p->next = l->head;
        l->head = p;
        if (l->last == NULL)
            l->last = p;
    }
    else
    {
        struct node* cur = l->head;

        
        while (cur->next != NULL && cur->next->priority >= p->priority)
            cur = cur->next;

        p->next = cur->next;
        cur->next = p;

        if (p->next == NULL)
            l->last = p;
    }
    printf("Элемент добавлен в приоритетную очередь\n");
}

// извлечение из начала 
void remove_head(struct list* l)
{
    if (l->head == NULL)
    {
        printf("Структура пуста\n");
        return;
    }

    struct node* p = l->head;
    l->head = p->next;

    if (l->head == NULL)
        l->last = NULL;

    printf("Извлечен объект: %s\n", p->inf);
    free(p);
}
int count_by_name(struct list* l, char* name)//поиск и счет
{
    int count = 0;
    struct node* cur = l->head;

    while (cur != NULL)
    {
        if (strcmp(cur->inf, name) == 0)
            count++;
        cur = cur->next;
    }
    return count;
}
// просмотр списка
void review(struct list* l, int show_priority)
{
    struct node* cur = l->head;

    if (cur == NULL)
    {
        printf("Список пуст\n");
        return;
    }

    while (cur != NULL)
    {
        if (show_priority)
            printf("Объект: %s, приоритет: %d\n", cur->inf, cur->priority);
        else
            printf("Объект: %s\n", cur->inf);
        cur = cur->next;
    }
}

// поиск элемента по названию
struct node* find(struct list* l, char* name)
{
    struct node* cur = l->head;

    while (cur != NULL)
    {
        if (strcmp(cur->inf, name) == 0)
            return cur;
        cur = cur->next;
    }
    return NULL;
}

// удаление элемента по названию
void del(struct list* l, char* name)
{
    int count = count_by_name(l, name);

    if (count == 0)
    {
        printf("Элемент не найден\n");
        return;
    }

    struct node* cur = l->head;
    struct node* prev = NULL;
    int seen = 0;      
    int removed = 0;   

    while (cur != NULL)
    {
        if (strcmp(cur->inf, name) == 0)
        {
            seen++;

            
            if (count == 1 || seen > 1)
            {
                struct node* tmp = cur;
                cur = cur->next;

                if (prev == NULL)
                    l->head = cur;
                else
                    prev->next = cur;

                if (tmp == l->last)
                    l->last = prev;

                free(tmp);
                removed++;
                continue;  
            }
        }
        prev = cur;
        cur = cur->next;
    }

    if (count == 1)
        printf("Элемент удален\n");
    else
        printf("Удалено дубликатов: %d, остался один элемент\n", removed);
}

// освобождение памяти списка
void clear_list(struct list* l)
{
    while (l->head != NULL)
    {
        struct node* p = l->head;
        l->head = p->next;
        free(p);
    }
    l->last = NULL;
}


void structure_menu(struct list* l, int kind, const char* title)
{
    int choice;
    char name[256];

    do
    {
        printf("\n--- %s ---\n", title);
        printf("1. Добавить элемент\n");
        printf("2. Извлечь элемент\n");
        printf("3. Просмотреть список\n");
        printf("4. Найти элемент\n");
        printf("5. Удалить элемент по названию\n");
        printf("0. Назад\n");
        choice = read_int("Ваш выбор: ");

        switch (choice)
        {
        case 1:
            if (kind == 1)
                queue_add(l);
            else if (kind == 2)
                stack_push(l);
            else
                priority_add(l);
            break;

        case 2:
            remove_head(l);
            break;

        case 3:
            review(l, kind == 3);
            break;

        case 4:
    printf("Введите название для поиска: ");
    scanf("%255s", name);
    {
        int n = count_by_name(l, name);

        if (n == 0)
            printf("Элемент не найден\n");
        else if (n == 1)
            printf("Элемент найден\n");
        else
            printf("Элемент найден, количество с таким названием: %d\n", n);
    }
    break;

        case 5:
            printf("Введите название для удаления: ");
            scanf("%255s", name);
            del(l, name);
            break;

        case 0:
            break;

        default:
            printf("Нет такого пункта меню\n");
        }
    } while (choice != 0);
}

int main(void)
{
    int choice;
    setlocale(LC_ALL, "ru");

    do
    {
        printf("\n=== ГЛАВНОЕ МЕНЮ ===\n");
        printf("1. Очередь\n");
        printf("2. Стек\n");
        printf("3. Приоритетная очередь\n");
        printf("0. Выход\n");
        choice = read_int("Ваш выбор: ");

        switch (choice)
        {
        case 1:
            structure_menu(&queue, 1, "ОЧЕРЕДЬ");
            break;
        case 2:
            structure_menu(&stack, 2, "СТЕК");
            break;
        case 3:
            structure_menu(&pqueue, 3, "ПРИОРИТЕТНАЯ ОЧЕРЕДЬ");
            break;
        case 0:
            clear_list(&queue);
            clear_list(&stack);
            clear_list(&pqueue);
            printf("Программа завершена\n");
            break;
        default:
            printf("Нет такого пункта меню\n");
        }
    } while (choice != 0);

    return 0;
}