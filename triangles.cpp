#include <iostream>
#include <vector>
#include <random>
#include <array>
#include <time.h>
#include <chrono>

typedef std::vector<std::vector<int>> Matrix;


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

// PART 3
Matrix create_graph(int n, float p)
{
    std::default_random_engine generator;
    generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<float> distribution(0.0, 1.0);
    Matrix graph(n, std::vector<int>(n, 0));
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            float prob = distribution(generator);
            if (prob <= p)
            {
                graph[i][j] = 1;
                graph[j][i] = 1;
            }
        }
    }
    return graph;
}

int main()
{
    
    // PART 3
    int crossoverpt = 124;

    Matrix one = create_graph(1024, 0.01);
    Matrix two = create_graph(1024, 0.02);
    Matrix three = create_graph(1024, 0.03);
    Matrix four = create_graph(1024, 0.04);
    Matrix five = create_graph(1024, 0.05);

    Matrix oneCubed = strassen(strassen(one, one, crossoverpt), one, crossoverpt);
    Matrix twoCubed = strassen(strassen(two, two, crossoverpt), two, crossoverpt);
    Matrix threeCubed = strassen(strassen(three, three, crossoverpt), three, crossoverpt);
    Matrix fourCubed = strassen(strassen(four, four, crossoverpt), four, crossoverpt);
    Matrix fiveCubed = strassen(strassen(five, five, crossoverpt), five, crossoverpt);

    int count1 = 0;
    int count2 = 0;
    int count3 = 0;
    int count4 = 0;
    int count5 = 0;

    for (int i = 0; i < 1024; i++)
    {
        count1 += oneCubed[i][i];
        count2 += twoCubed[i][i];
        count3 += threeCubed[i][i];
        count4 += fourCubed[i][i];
        count5 += fiveCubed[i][i];
    }

    printf("For p = 0.01, the number of triangles is: %i\n", count1 / 6);
    printf("For p = 0.02, the number of triangles is: %i\n", count2 / 6);
    printf("For p = 0.03, the number of triangles is: %i\n", count3 / 6);
    printf("For p = 0.04, the number of triangles is: %i\n", count4 / 6);
    printf("For p = 0.05, the number of triangles is: %i\n", count5 / 6);

    return 0;
}


