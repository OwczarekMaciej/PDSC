#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void *bsearch(const void *key, const void *base, size_t num, size_t size, int (*compar)(const void *, const void *))
{
    size_t left = 0;
    size_t right = num - 1;

    while (left <= right)
    {
        size_t middle = (left + right) / 2;
        void *middle_elem = (char *)base + (middle * size);
        int cmp_result = compar(key, middle_elem);

        if (cmp_result == 0)
        {
            return middle_elem;
        }
        else if (cmp_result < 0)
        {
            right = middle - 1;
        }
        else
        {
            left = middle + 1;
        }
    }

    return NULL;
}

int cmp_int(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

int cmp_float(const void *a, const void *b)
{
    float float_a = *(float *)a;
    float float_b = *(float *)b;

    if (float_a < float_b)
    {
        return -1;
    }
    else if (float_a > float_b)
    {
        return 1;
    }
    else
        return 0;
}

int cmp_string(const void *a, const void *b)
{

    return strcmp(a, b);
}

void test_int(int num_of_elements)
{
    int sorted_array[num_of_elements];

    srand(time(NULL));
    for (int i = 0; i < num_of_elements; i++)
    {
        sorted_array[i] = rand() % 100;
    }

    qsort(sorted_array, num_of_elements, sizeof(int), cmp_int);

    printf("\nSorted Array: ");
    for (int i = 0; i < num_of_elements; i++)
    {
        printf("%d ", sorted_array[i]);
    }
    printf("\n");

    int no_to_find;
    printf("Enter number to find in array: \n");
    scanf("%d", &no_to_find);

    int *result = bsearch(&no_to_find, sorted_array, num_of_elements, sizeof(int), cmp_int);

    int key_index = -1;
    for (int i = 0; i < num_of_elements; i++)
    {
        if (sorted_array[i] == no_to_find)
        {
            key_index = i;
            break;
        }
    }

    if (result != NULL)
    {
        if ((result - sorted_array) == key_index)
        {
            printf("\nBsearch works fine. Number found at %ld index\n", (result - sorted_array));
        }
        else
        {
            printf("\nBsearch does not work. Number found at %ld index.\nIt should be at %d index.\n", (result - sorted_array), key_index);
        }
    }
    else
    {
        printf("\nNumber not found in array\n");
    }
}

void test_float()
{
    float sorted_array[] = {1.5, 2.3, 4.2, 5.1, 6.71, 7.19};
    int num_of_elements = sizeof(sorted_array) / sizeof(sorted_array[0]);
    float no_to_find = 4.2;

    float *result = bsearch(&no_to_find, sorted_array, num_of_elements, sizeof(float), cmp_float);

    printf("\nSorted Array: ");
    for (int i = 0; i < num_of_elements; i++)
    {
        printf("%f ", sorted_array[i]);
    }
    printf("\nNumber to find: %f", no_to_find);

    if (result != NULL)
    {
        int index = result - sorted_array;
        printf("\nBsearch works fine. Number found at %d index.\n", index);
    }
    else
    {
        printf("\nNumber not found in array\n");
    }
}

void test_string()
{
    char* sorted_array[] = {"one", "two", "three", "four"};
    int num_of_elements = sizeof(sorted_array) / sizeof(sorted_array[0]);
    char* no_to_find = "threjje";

    char **result = bsearch(&no_to_find, sorted_array, num_of_elements, sizeof(char*), cmp_string);

    if (result != NULL)
    {
        int index = result - sorted_array;
        printf("\nBsearch works fine. Number found at %d index.\n", index);
    }
    else
    {
        printf("\nNumber not found in array\n");
    }
}

int main()
{
    int size;
    printf("Enter size of array: \n");
    scanf("%d", &size);

    test_int(size);
    test_float();
    test_string();
    return 0;
}
