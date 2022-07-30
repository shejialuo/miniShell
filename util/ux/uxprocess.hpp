/*
  Copyright 2003 by Marc J. Rochkind. All rights reserved.
  May be copied only for purposes and under conditions described
  on the Web page www.basepath.com/aup/copyright.htm.

  The Example Files are provided "as is," without any warranty;
  without even the implied warranty of merchantability or fitness
  for a particular purpose. The author and his publisher are not
  responsible for any damages, direct or incidental, resulting
  from the use or non-use of these Example Files.

  The Example Files may contain defects, and some contain deliberate
  coding mistakes that were included for educational reasons.
  You are responsible for determining if and how the Example Files
  are to be used.
*/

/**
 * @file uxfile.hpp
 * @brief wrap the posix process operation
 */

#ifndef _UTIL_UX_UXPROCESS_HPP_
#define _UTIL_UX_UXPROCESS_HPP_

#include "ux.hpp"

namespace ux {
class Process : public Base {
protected:
  pid_t pid;

public:
  /**
   * @brief Construct a new Process object
   *
   * Construct a new Process object with specified id
   * if the id is not specified, it uses `getpid()`
   * to get the current process id
   *
   * @param id
   */
  Process(pid_t id = -1) : pid(id == -1 ? ::getpid() : id) {}
  /**
   * @brief reset the pid
   *
   * @param id
   */
  void set(pid_t id) { pid = id; }
  /**
   * @brief convert the Process class to `pid_t`
   *
   * @return current pid
   */
  operator pid_t() { return pid; }
};
}  // namespace ux

#endif  // _UTIL_UX_UXPROCESS_HPP_
