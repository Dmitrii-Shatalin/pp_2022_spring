// Copyright 2022 Chelebaev Artem
#include "../../../modules/task_3/chelebaev_a_graham_scan_tbb/graham_scan_tbb.h"

#include <stack>

std::vector<std::pair<double, double>> gen_random_points(std::size_t size) {
  std::mt19937 rand_r(time(0));
  std::vector<std::pair<double, double>> vec(size);
  for (std::size_t i = 0; i < size; ++i) {
    vec[i].first = rand_r() % 100;
    vec[i].second = rand_r() % 100;
  }
  return vec;
}

double get_polar_radius(const std::pair<double, double>& point) {
  return std::sqrt(point.second * point.second + point.first * point.first);
}

double get_polar_grad(const std::pair<double, double>& point) {
  return std::atan2(point.second, point.first);
}

double calc_det(const std::pair<double, double>& x,
                const std::pair<double, double>& y,
                const std::pair<double, double>& z) {
  return (y.first - x.first) * (z.second - x.second) -
         (z.first - x.first) * (y.second - x.second);
}

std::size_t get_lex_min(std::vector<std::pair<double, double>> v) {
  std::size_t min_idx = 0;
  for (std::size_t i = 1; i < v.size(); ++i) {
    if (v[i] < v[min_idx]) {
      min_idx = i;
    }
  }
  return min_idx;
}

bool is_less(const std::pair<double, double>& a,
             const std::pair<double, double>& b) {
  double grad_a = get_polar_grad(a);
  double grad_b = get_polar_grad(b);

  if (grad_a < grad_b) {
    return true;
  } else if ((std::abs(grad_a - grad_b) <= 1e-15) &&
             (get_polar_radius(a) < get_polar_radius(b))) {
    return true;
  } else {
    return false;
  }
}

std::vector<std::pair<double, double>> graham_conv_hull(
    std::vector<std::pair<double, double>>::iterator begin,
    std::vector<std::pair<double, double>>::iterator end) {
  std::vector<std::pair<double, double>> points(end - begin);
  std::copy(begin, end, points.begin());

  std::size_t min_idx = get_lex_min(points);
  std::swap(points[0], points[min_idx]);
  auto lex_min = points[0];

  for (std::size_t i = 0; i < points.size(); ++i) {
    points[i].first -= lex_min.first;
    points[i].second -= lex_min.second;
  }

  std::sort(points.begin() + 1, points.end(), is_less);

  std::stack<std::pair<double, double>> res;
  std::size_t stack_size = res.size();
  res.push(points[0]);
  res.push(points[1]);

  std::pair<double, double> x, y, z;
  for (std::size_t i = 2; i < points.size(); ++i) {
    stack_size = res.size();
    y = res.top();
    res.pop();
    x = res.top();
    z = points[i];
    double det = calc_det(x, y, z);

    if (det > 0) {
      res.push(y);
      res.push(points[i]);
    } else if (stack_size < 3) {
      res.push(points[i]);
    } else {
      --i;
    }
  }

  std::vector<std::pair<double, double>> res_vec(res.size());
  std::size_t i = res.size() - 1;
  while (!res.empty()) {
    res_vec[i] = res.top();
    res_vec[i].first += lex_min.first;
    res_vec[i].second += lex_min.second;
    res.pop();
    --i;
  }
  return res_vec;
}

std::vector<std::pair<double, double>> tbb_graham_conv_hull(
    std::vector<std::pair<double, double>>::iterator begin,
    std::vector<std::pair<double, double>>::iterator end,
    std::size_t n_threads) {
  if (n_threads == 0) {
    throw "Where are the threads?";
  }
  std::vector<std::pair<double, double>> result;
  int step = (end - begin) / n_threads;
  tbb::task_group threads;
  tbb::spin_mutex mutex;
  tbb::task_scheduler_init init(static_cast<int>(n_threads));
  for (std::size_t i = 0; i < n_threads; i++) {
    if (i == n_threads - 1) {
      threads.run([&result, begin, end, step, n_threads, &mutex]() {
        auto local_res = graham_conv_hull(begin + step * (n_threads - 1), end);
        for (std::size_t j = 0; j < local_res.size(); j++) {
          tbb::spin_mutex::scoped_lock lock;
          lock.acquire(mutex);
          result.push_back(local_res[j]);
          lock.release();
        }
      });
      break;
    }
    threads.run([&result, &mutex, i, begin, step]() {
      auto left = begin + step * i;
      auto right = begin + step * (i + 1);
      auto local_res = graham_conv_hull(left, right);
      for (std::size_t j = 0; j < local_res.size(); ++j) {
        tbb::spin_mutex::scoped_lock lock;
        lock.acquire(mutex);
        result.push_back(local_res[j]);
        lock.release();
      }
    });
  }
  threads.wait();

  return graham_conv_hull(result.begin(), result.end());
}
