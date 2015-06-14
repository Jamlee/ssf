#ifndef SSF_COMMON_IO_COMPOSED_OP_H_
#define SSF_COMMON_IO_COMPOSED_OP_H_

#include <boost/asio/detail/handler_invoke_helpers.hpp>
#include <boost/asio/strand.hpp>

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

namespace io {

template <class CompHandler, class ContextHandler>
struct ComposedOp {
  ComposedOp(CompHandler h, ContextHandler c)
      : handler(std::move(h)), context(std::move(c)) {}

  inline void operator()(const boost::system::error_code& ec,
                         std::size_t length) {
    handler(ec, length);
  }

  inline void operator()(const boost::system::error_code& ec,
                         std::size_t length) const {
    handler(ec, length);
  }

  inline void operator()(const boost::system::error_code& ec) { handler(ec); }

  inline void operator()(const boost::system::error_code& ec) const {
    handler(ec);
  }

  CompHandler handler;
  ContextHandler context;
};

template <typename CompHandler, typename ContextHandler>
inline void* asio_handler_allocate(
    std::size_t size, ComposedOp<CompHandler, ContextHandler>* this_handler) {
  return boost_asio_handler_alloc_helpers::allocate(size,
                                                    this_handler->context);
}

template <typename CompHandler, typename ContextHandler>
inline void asio_handler_deallocate(
    void* pointer, std::size_t size,
    ComposedOp<CompHandler, ContextHandler>* this_handler) {
  boost_asio_handler_alloc_helpers::deallocate(pointer, size,
                                               this_handler->context);
}

template <typename CompHandler, typename ContextHandler>
inline bool asio_handler_is_continuation(
    ComposedOp<CompHandler, ContextHandler>* this_handler) {
  return boost_asio_handler_cont_helpers::is_continuation(
      this_handler->context);
}

template <typename Function, typename CompHandler, typename ContextHandler>
inline void asio_handler_invoke(
    const Function& function,
    ComposedOp<CompHandler, ContextHandler>* this_handler) {
  boost_asio_handler_invoke_helpers::invoke(function, this_handler->context);
}

template <typename Function, typename CompHandler, typename ContextHandler>
inline void asio_handler_invoke(
    Function& function, ComposedOp<CompHandler, ContextHandler>* this_handler) {
  boost_asio_handler_invoke_helpers::invoke(function, this_handler->context);
}

}  // io

#endif  // SSF_COMMON_IO_COMPOSED_OP_H_
