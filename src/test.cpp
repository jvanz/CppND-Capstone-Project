#include <iostream>
#include <vector>

#include "common/message.hpp"

class PODMessage {
  int x;
  char data[5];
};

int main() {
  std::vector<char> v;
  std::cout << "v.size(): " << v.size() << std::endl;
  std::cout << "Message is pod: " << std::is_pod<PODMessage>::value
            << std::endl;
}
