// Copyright 2022 Zotov Maxim
#include <gtest/gtest.h>
#include <omp.h>
#include <vector>
#include "./sort_with_batcher_merge.h"

TEST(zotov_parralel_radix_sort, Parralel_Radix_Sort_1) {
    std::vector<int> vec = { 6, 5, -4, 3, -2, 1, -8 };
    std::vector<int> sort_vec = { -8, -4, -2, 1, 3, 5, 6 };
    int number_threads = 2;
    parallelRadixSort(&vec, vec.size(), number_threads);
    ASSERT_EQ(sort_vec, vec);
}

TEST(zotov_parralel_radix_sort, Parralel_Radix_Sort_2) {
    int size = 20;
    std::vector<int> vec = getRandomVector(size);
    std::vector<int> std_sort_vec = vec;
    int number_threads = 2;
    std::sort(std_sort_vec.begin(), std_sort_vec.end());
    parallelRadixSort(&vec, vec.size(), number_threads);
    ASSERT_EQ(std_sort_vec, vec);
}

TEST(zotov_parralel_radix_sort, Parralel_Radix_Sort_3) {
    int size = 20;
    std::vector<int> vec = getRandomVector(size);
    std::vector<int> std_sort_vec = vec;
    int number_threads = 4;
    std::sort(std_sort_vec.begin(), std_sort_vec.end());
    parallelRadixSort(&vec, vec.size(), number_threads);
    ASSERT_EQ(std_sort_vec, vec);
}

TEST(zotov_parralel_radix_sort, Parralel_Radix_Sort_4) {
    int size = 100;
    std::vector<int> vec = getRandomVector(size);
    std::vector<int> seq_vec = vec;
    int number_threads = 2;
    radixSort(&seq_vec, seq_vec.size(), 0);
    parallelRadixSort(&vec, vec.size(), number_threads);
    ASSERT_EQ(seq_vec, vec);
}

TEST(zotov_parralel_radix_sort, Parralel_Radix_Sort_5) {
    int size = 100;
    std::vector<int> vec = getRandomVector(size);
    std::vector<int> seq_vec = vec;
    int number_threads = 4;
    radixSort(&seq_vec, seq_vec.size(), 0);
    parallelRadixSort(&vec, vec.size(), number_threads);
    ASSERT_EQ(seq_vec, vec);
}

TEST(zotov_parralel_radix_sort, Parralel_Radix_Sort_6) {
    int size = 100000;
    std::vector<int> vec = getRandomVector(size);
    std::vector<int> seq_vec = vec;
    int number_threads = 4;
    double a = omp_get_wtime();
    radixSort(&seq_vec, seq_vec.size(), 0);
    double b = omp_get_wtime();
    std::cout << "seq time = " << b - a << std::endl;

    a = omp_get_wtime();
    parallelRadixSort(&vec, vec.size(), number_threads);
    b = omp_get_wtime();
    std::cout << "parallel time = " << b - a << std::endl;
    ASSERT_EQ(seq_vec, vec);
}

TEST(zotov_parralel_radix_sort, Parralel_Radix_Sort_7) {
    int size = 100000;
    std::vector<int> vec = getRandomVector(size);
    std::vector<int> seq_vec = vec;
    int number_threads = 8;
    double a = omp_get_wtime();
    radixSort(&seq_vec, seq_vec.size(), 0);
    double b = omp_get_wtime();
    std::cout << "seq time = " << b - a << std::endl;

    a = omp_get_wtime();
    parallelRadixSort(&vec, vec.size(), number_threads);
    b = omp_get_wtime();
    std::cout << "parallel time = " << b - a << std::endl;
    ASSERT_EQ(seq_vec, vec);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
