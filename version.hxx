#include <iostream>
namespace cuda {

struct driver_version_t {
    int raw;
    int major() const { return raw / 1000; }
    int minor() const { return (raw % 100); }
    driver_version_t(int raw) : raw(raw) {}
};
std::ostream& operator<<(std::ostream& os, const driver_version_t& v) { return os << v.major() << '.' << v.minor(); }

inline driver_version_t driver_version() noexcept {
  int r;
  cuDriverGetVersion(&r);
  return r; }
} // namespace cuda
