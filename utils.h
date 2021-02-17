#pragma once
#include <ostream>
#include <cstdlib>
#include <vector>
#include <random>

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec)
{
   auto it = vec.begin();
   auto end = vec.end();

   os << '[';
   if (it != end) {
      os << *it;
      ++it;
   }

   while (it != end) {
      os << ", " << *it;
      ++it;
   }
   os << ']';

   return os;
}

inline bool double_equals(double a, double b, double epsilon = 0.001) {
   return std::abs(a - b) < epsilon;
}

template <typename T>
bool vector_equals(const std::vector<T>& lhs, const std::vector<T>& rhs) {
   if (lhs.size() != rhs.size())
      return false;

   for (size_t i = 0; i < lhs.size(); i++) {
      if (!vector_equals(lhs[i], rhs[i]))
         return false;
   }
   return true;
}

template <typename T>
bool vector_equals(const T& lhs, const T& rhs) {
   return lhs == rhs;
}

template <>
inline bool vector_equals(const std::vector<double>& lhs, const std::vector<double>& rhs) {
   if (lhs.size() != rhs.size())
      return false;

   for (size_t i = 0; i < lhs.size(); i++) {
      if (!double_equals(lhs[i], rhs[i]))
         return false;
   }
   return true;
}

inline double uniform01() {
			static std::default_random_engine generator;
			static std::uniform_real_distribution<double> distribution(0.0, 1.0);
			return distribution(generator);
}

inline double uniformxy(const double x, const double y)
{
   static std::default_random_engine generator;
   std::uniform_real_distribution<double> distribution(x, y);
   return distribution(generator);
}