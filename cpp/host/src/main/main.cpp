#include "../../include/server/Server.hpp"


/**
 * Program entry point
 * @return - 0 upon successful execution
 */
int main()
{
  Server s = Server();
  s.run();
  return 0;
}
