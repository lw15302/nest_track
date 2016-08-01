#include <Main.hpp>

/**
 * Entry point into the program
 * @param  argc - number of program args (1 for the current program)
 * @param  argv - program args (no additional args for current program)
 * @return      - returns 0 on successful execution
 */
int main(int argc, char *argv[])
{
  VideoPlayer player;
  player.initVC();
  player.run();
  return 0;
}
