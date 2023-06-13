#include <iostream>
#include <vector>
#include <random>
#include <array>
#include <time.h>
#include <chrono>
#include <cmath>

typedef std::vector<std::vector<int>> Matrix;

// PART 2: IMPLEMENT STRASSENS ALGORITHM & CONVENTIONAL ALGORITHM; RUN TESTS TO FIND CROSS OVER POINT

// RANDOMLY GENERATE MATRICES
std::random_device rand_matrix;
std::mt19937 gen(rand_matrix());
std::uniform_int_distribution<int> randNum(1000, 100000);
Matrix random(int n)
{
    Matrix C(n, std::vector<int>(n, 0));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            C[i][j] = randNum(gen);
        }
    }
    return C;
}

// CONVENTIONAL MATRIX-MATRIX MULTIPLICATION
Matrix conventional(Matrix A, Matrix B)
{
    int n = A.size();
    Matrix C(n, std::vector<int>(n, 0));
    // i iterates through the row
    for (int i = 0; i < n; i++)
    {
        // j iterates through the column
        for (int j = 0; j < n; j++)
        {
            // k performs multiplication
            for (int k = 0; k < n; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

// ADD MATRICES HELPER FUNCTION
Matrix add(Matrix A, Matrix B)
{
    int n = A.size();
    Matrix C(n, std::vector<int>(n, 0));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}

// SUBTRACT MATRICES HELPER FUNCTION
Matrix sub(Matrix A, Matrix B)
{
    int n = A.size();
    Matrix C(n, std::vector<int>(n, 0));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return C;
}

// STRASSENS ALGORITHM
Matrix strassen(Matrix A, Matrix B, int cp)
{
    int n = A.size();
    Matrix C(n, std::vector<int>(n, 0));

    // BASE CASE; WILL CHANGE TO SWITCH OVER TO CONVENTIONAL WHEN N IS A CERTAIN SIZE
    // WHEN SIZE = CROSSOVER POINT, CALL CONVENTIONAL
    if (n <= cp)
    {
        C = conventional(A, B);
        return C;
    }

    // INITIALIZE THE SUBMATRICES
    int k = n / 2;
    Matrix A11(k, std::vector<int>(k, 0));
    Matrix A12(k, std::vector<int>(k, 0));
    Matrix A21(k, std::vector<int>(k, 0));
    Matrix A22(k, std::vector<int>(k, 0));
    Matrix B11(k, std::vector<int>(k, 0));
    Matrix B12(k, std::vector<int>(k, 0));
    Matrix B21(k, std::vector<int>(k, 0));
    Matrix B22(k, std::vector<int>(k, 0));

    // INSERT THE VALUES OF THE SUBMATRICES
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + k];
            A21[i][j] = A[i + k][j];
            A22[i][j] = A[i + k][j + k];
            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + k];
            B21[i][j] = B[i + k][j];
            B22[i][j] = B[i + k][j + k];
        }
    }

    // STRASSEN EQUATIONS
    Matrix P1 = strassen(A11, sub(B12, B22), cp);
    Matrix P2 = strassen(add(A11, A12), B22, cp);
    Matrix P3 = strassen(add(A21, A22), B11, cp);
    Matrix P4 = strassen(A22, sub(B21, B11), cp);
    Matrix P5 = strassen(add(A11, A22), add(B11, B22), cp);
    Matrix P6 = strassen(sub(A12, A22), add(B21, B22), cp);
    Matrix P7 = strassen(sub(A11, A21), add(B11, B12), cp);

    // ADD STRASSEN EQUATIONS TO FIND APPROPRIATE TERM
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            C[i][j] = P5[i][j] + P4[i][j] - P2[i][j] + P6[i][j];
            C[i][j + k] = P1[i][j] + P2[i][j];
            C[i + k][j] = P3[i][j] + P4[i][j];
            C[i + k][j + k] = P5[i][j] + P1[i][j] - P3[i][j] - P7[i][j];
        }
    }
    return C;
}

// RANDOMLY TEST DIFFERENT CROSS-OVER POINTS
void crossover(Matrix A, Matrix B)
{
    // DIFFERENT CROSSOVER POINTS TO TEST
    // array crossover[] = {2, 4, 8, 16, 32, 64, 128};
    // int point = 1;
    int size = A.size();
    srand((unsigned)time(NULL));

    for (int i = 1; i <= size; i *= 2)
    {
        printf("Runtime when crossover point is size n = %i: ", i);
        clock_t start = clock();
        strassen(A, B, i);
        clock_t end = clock();
        printf("%f\n", float(end - start) / CLOCKS_PER_SEC);
    }
}

// FUNCTION TO CHECK IF AN INT IS A POWER OF 2
bool powerOf2(int i)
{
    if (i <= 0)
    {
        return false;
    }
    while (i % 2 == 0 && i != 1)
    {
        i /= 2;
    }
    return i == 1;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: ./strassens 0 dimension inputfile");
    }
    int flag = atoi(argv[1]);
    int dimension = atoi(argv[2]);
    char *inputfile = argv[3];

    // PARSING INPUT FILE
    FILE *fp;
    int num;

    Matrix first(dimension, std::vector<int>(dimension, 0));
    Matrix second(dimension, std::vector<int>(dimension, 0));

    fp = fopen(inputfile, "r");
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }
    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            if (fscanf(fp, "%d", &num) == 1)
            {
                first[i][j] = num;
            }
        }
    }
    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            if (fscanf(fp, "%d", &num) == 1)
            {
                second[i][j] = num;
            }
        }
    }

    fclose(fp);

    int crossoverpt = 2;
    int m = 1;

    if (!powerOf2(dimension))
    {
        while (m < dimension)
        {
            m *= 2;
        }

        first.resize(m, std::vector<int>(m, 0));
        second.resize(m, std::vector<int>(m, 0));
    }

    Matrix output = strassen(first, second, crossoverpt);

    for (int i = 0; i < dimension; i++)
    {
        printf("%i\n", output[i][i]);
    }

    // TESTING CROSSOVER POINTS
    // crossover(random(dimension), random(dimension));

    return 0;
}
