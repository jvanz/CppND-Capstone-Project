
#include <fstream>
#include <iostream>
#include <string>

// Simple program to generate a file where each line is a message that producer
// will send to the message queue system
int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Invalid command line arguments. Expected file path and "
                 "message count."
              << std::endl;
    return -1;
  }
  std::string file(argv[1]);
  unsigned int message_count = std::stoul(argv[2]);
  std::ofstream outfile;
  std::cout << "Creating " << message_count << " messages in the \"" << file
            << "\" file." << std::endl;

  outfile.open(file);
  if (outfile.is_open()) {
    for (unsigned int i = 0; i < message_count; i++) {
      outfile << "Message " + std::to_string(i) << std::endl;
      std::cout << i + 1 << " messages created." << std::endl;
    }
    outfile.close();
  } else {
    std::cerr << "Cannot open file!" << std::endl;
  }
  std::cout << "Done. Bye!" << std::endl;
  return 0;
}
