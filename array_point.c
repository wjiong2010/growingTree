#include <stdio.h>

int test_table_32bit[4];

int * get_test_table_point(void)
{
    return test_table_32bit;
}

void print_test_table(char tab[])
{
    printf("print_test_table: %08x, %08x\n", tab[0], tab[1]);
}

void print_test_table2(int* tab_p)
{
    printf("print_test_table2: %08x, %08x\n", tab_p[0], tab_p[1]);
}

int main()
{
    int * p_point_32;
    char * p_point_16;
    int * p_array;

    test_table_32bit[0] = 0x04030201;
    test_table_32bit[1] = 0x14131211;

    p_point_32 = get_test_table_point();
    p_point_16 = get_test_table_point();

    print_test_table(p_point_16);
    printf("point_32: %08x, %08x\n", p_point_32[0], p_point_32[1]);    
    printf("point_16: %08x, %08x\n", p_point_16[0], p_point_16[1]);
    print_test_table(test_table_32bit);
    print_test_table2(p_point_16);
    
}
