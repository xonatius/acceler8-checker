#include <cstdio>

#include "common_tools.h"

#define MAX 20000

// Test input structure
struct TestInput {
    // Matrix sizes
    int n;
    int m;
    // Matrix generation params
    // seed = init_seed
    // seed = (seed * a + b) % c
    int init_seed;
    int a;
    int b;
    int c;
};

// Test output structure
struct TestOutput {
    // Test case_num
    int test;
    // Left upper corner
    int a1;
    int a2;
    // Right lower corner
    int b1;
    int b2;
    // Area and sum
    int area;
    int sum;
};

// Reads test case. Return true if ok.
bool read_input(FILE *finput, TestInput &input)
{
    return fscanf(finput, "%d%d%d%d%d%d", &input.n, &input.m, &input.init_seed, 
                                          &input.a, &input.b, &input.c) == 6;
}

// Reads test output. Return true if ok.
bool read_output(FILE *foutput, TestOutput &output)
{
    return fscanf(foutput, "Case #%d: %d %d %d %d %d %d\n", &output.test, &output.a1, &output.a2, 
                                                            &output.b1, &output.b2, &output.sum, &output.area) == 7;
}

void try_fclose(FILE *f)
{
    if (f != 0)
        fclose(f);
}

int get_sum(TestInput input, TestOutput output, int *data)
{
    int sum=0;
    for (int i=output.a1; i <= output.b1; i++)
        for (int j=output.a2; j <= output.b2; j++)
            sum += data[i*input.m + j];
    return sum;
}

int get_area(TestOutput output)
{
    return (output.b1 - output.a1 + 1) * (output.b2 - output.a2 + 1);
}

int check(TestInput input, TestOutput correct, TestOutput checking)
{
    // Checking input params
    if (input.m < 0 || input.n < 0 || input.init_seed < 0 || input.a < 0 || input.b < 0 || input.c < 0 ||
        input.m > MAX || input.n > MAX || input.init_seed > MAX || input.a > MAX || input.b > MAX || input.c > MAX)
    {
        printf("Input data should be in [%d; %d]\n", 0, MAX);
        return 4;
    }
    int *data = new int[input.n * input.m];
    InitMatrix(data, input.n * input.m, input.init_seed, input.a, input.b, input.c);
    // Checking sum
    int tmp = 0;
    if ((tmp = get_sum(input, correct, data)) != correct.sum)
    {
        printf("Sum in correct file mismatch: %d instead of %d\n", correct.sum, tmp);
        delete [] data;
        return 2;
    }
    if ((tmp = get_sum(input, checking, data)) != checking.sum)
    {
        printf("ERROR: Sum in checking file mismatch: %d instead of %d\n", checking.sum, tmp);
        delete [] data;
        return 1;
    }
    delete [] data;
    if (correct.sum != checking.sum)
    {
        printf("ERROR: Sum in checking file and in correct file mismatch. Correct:%d. Checking:%d\n", correct.sum, checking.sum);
        return 1;
    }
    // Checking area
    if (checking.area != (tmp = get_area(checking)))
    {
        printf("ERROR: Area in checking file mismach. Correct:%d. Checking:%d\n", tmp, checking.area);
        return 1;
    }
    printf("Ok!\n");
    return 0;
}

int check_all(FILE *finput, FILE *fcorrect, FILE *fchecking)
{
    int code=0;
    int test_count;
    fscanf(finput, "%d\n", &test_count);
    printf("Checking %d tests.\n", test_count);
    TestInput input;
    TestOutput correct, checking;
    for (int test=1; test <= test_count; test++)
    {
        printf("Test #%d: ", test);
        if (!read_input(finput, input))
        {
            printf("Wrong input format\n");
            code |= 4;
            continue;
        }
        if (!read_output(fcorrect, correct))
        {
            printf("Wrong correct format\n");
            code |= 2;
            continue;
        }
        if (!read_output(fchecking, checking))
        {
            printf("ERROR: Wrong checking format\n");
            code |= 1;
            continue;
        }
        code |= check(input, correct, checking);
    }
    return code;
}

int main (int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage ./checker test_input correct_output checking_output.\n");
        return -1;
    }
    FILE *input = fopen(argv[1], "r");
    FILE *correct = fopen(argv[2], "r");
    FILE *checking = fopen(argv[3], "r");
    int code = 0;
    if (!input)
    {
        printf("Cannot open input file \"%s\"\n", argv[1]);
        code = -10;
    }
    else if (!correct)
    {
        printf("Cannot open correct output file \"%s\"\n", argv[2]);
        code = -11;
    }
    else if (!checking)
    {
        printf("Cannot open checking output file \"%s\"\n", argv[3]);
        code = -12;
    }
    if (!code)
        code = check_all(input, correct, checking);
    try_fclose(input);
    try_fclose(correct);
    try_fclose(checking);
    return code;
}
