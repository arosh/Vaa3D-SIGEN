#pragma once
namespace sigen {
template <class U, class T>
U mean(const T &xs) {
  if(xs.size() == 0) return 0.0;
  U sum = 0.0;
  for(int i = 0; i < (int)xs.size(); ++i) {
    sum += xs[i];
  }
  return sum / xs.size();
}
};
