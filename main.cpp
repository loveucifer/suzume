#include <cstdlib>
#include <firstapp.hpp>
#include <iostream>
#include <stdexcept>
int main() {
  suzume::firstapp app;
  try {
    app.run();
  } catch (const std::exception &e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }

  return 0;
}
