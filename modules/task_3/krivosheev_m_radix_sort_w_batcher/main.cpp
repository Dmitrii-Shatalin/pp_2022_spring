// Copyright 2022 Krivosheev Miron
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <numeric>
#include "./batcher.h"

TEST(Radix_Sort_W_Batcher, Test_CorrectSort) {
    std::vector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    ASSERT_EQ(true, checkSort(vec));
}

TEST(Radix_Sort_W_Batcher, Test_Sort) {
    std::vector<int> vec = { 9, 4, 5, 6, 3, 2, 1, 7, 8 };
    radixSort(&vec);
    ASSERT_EQ(true, checkSort(vec));
}

TEST(Radix_Sort_W_Batcher, Test_SortWithSameDigit) {
    std::vector<int> vec = { 3, 5, 1, 1, 7, 5 };
    radixSort(&vec);
    ASSERT_EQ(true, checkSort(vec));
}

TEST(Radix_Sort_W_Batcher, Test_WrangSort) {
    std::vector<int> vec = { 0, 2, 7, 3, 4 };
    ASSERT_EQ(false, checkSort(vec));
}

TEST(Radix_Sort_W_Batcher, Test_Batcher_W_RadixSort) {
    std::vector<int> part1 = GetRandVector(10);
    std::vector<int> part2 = GetRandVector(10);
    radixSort(&part1);
    radixSort(&part2);
    std::vector<int> even = evenBatch(part1, part2);
    std::vector<int> odd = oddBatch(part1, part2);
    std::vector<int> res = EvenOddBatch(even, odd);
    std::vector<int>stl_mege(20);
    std::merge(part1.begin(), part1.end(), part2.begin(),
      part2.end(), stl_mege.begin());
    ASSERT_EQ(stl_mege, res);
}

TEST(Radix_Sort_W_Batcher, Test_RadixSort_w_Batcher_Compare_Time) {
  std::vector<int> part1, part2, even, odd, res, stl_mege(20000);
  std::vector<double> timesOMP, timesSeq;
  double avgTimeOMP, avgTimeSeq;
  clock_t start, end;
  for (int i = 0; i < 3; ++i) {
    part1 = GetRandVector(10000);
    part2 = GetRandVector(10000);
    start = clock();
    radixSort(&part1);
    radixSort(&part2);
    even = evenBatch(part1, part2);
    odd = oddBatch(part1, part2);
    res = EvenOddBatch(even, odd);
    end = clock();
    timesOMP.push_back(end - start);

    part1 = GetRandVector(10000);
    part2 = GetRandVector(10000);
    start = clock();
    radixSortSeq(&part1);
    radixSortSeq(&part2);
    even = evenBatch_seq(part1, part2);
    odd = oddBatch_seq(part1, part2);
    res = EvenOddBatch_seq(even, odd);
    end = clock();
    timesSeq.push_back(end - start);
  }
  std::merge(part1.begin(), part1.end(), part2.begin(),
    part2.end(), stl_mege.begin());
  for (int i = 0; i < 3; i++)
    std::cout << "TBB time (ms): " << timesOMP[i] << std::endl;
  for (int i = 0; i < 3; i++)
    std::cout << "Seq time (ms): " << timesSeq[i] << std::endl;
  std::cout << std::endl;
  avgTimeOMP = std::accumulate(timesOMP.begin(), timesOMP.end(), 0.0) / timesOMP.size();
  avgTimeSeq = std::accumulate(timesSeq.begin(), timesSeq.end(), 0.0) / timesSeq.size();
  std::cout << "Average TBB time (ms): " << avgTimeOMP << std::endl;
  std::cout << "Average seq time (ms): " << avgTimeSeq << std::endl;
  std::cout << "boost rate: " << avgTimeSeq / avgTimeOMP * 100 - 100<< "%" << std::endl;
  for (unsigned int i = 0; i < res.size(); i++)
    ASSERT_EQ(stl_mege[i], res[i]);
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
