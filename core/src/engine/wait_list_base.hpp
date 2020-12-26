#pragma once

#include <boost/smart_ptr/intrusive_ptr.hpp>

namespace engine {
namespace impl {

class TaskContext;

class WaitListBase {
 public:
  class Lock {
   public:
    virtual ~Lock() = default;

    virtual explicit operator bool() = 0;

    virtual void Acquire() = 0;
    virtual void Release() = 0;
  };

  virtual ~WaitListBase() = default;

  virtual bool IsEmpty(Lock&) const = 0;

  virtual void Append(Lock&, boost::intrusive_ptr<impl::TaskContext>) = 0;
  virtual void WakeupOne(Lock&) = 0;
  virtual void WakeupAll(Lock&) = 0;

  virtual void Remove(WaitListBase::Lock&,
                      boost::intrusive_ptr<impl::TaskContext>) = 0;
};

}  // namespace impl
}  // namespace engine
