/*
 *
 * Copyright (c) 1998-2002
 * John Maddock
 *
 * Use, modification and distribution are subject to the 
 * Boost Software License, Version 1.0. (See accompanying file 
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */

 /*
  *   LOCATION:    see http://www.boost.org for most recent version.
  *   FILE         regex_raw_buffer.hpp
  *   VERSION      see <boost/version.hpp>
  *   DESCRIPTION: Raw character buffer for regex code.
  *                Note this is an internal header file included
  *                by regex.hpp, do not include on its own.
  */

#ifndef BOOST_REGEX_RAW_BUFFER_HPP
#define BOOST_REGEX_RAW_BUFFER_HPP

#ifndef BOOST_REGEX_CONFIG_HPP
#include <boost/regex/config.hpp>
#endif

#include <algorithm>
#include <cstddef>

namespace boost{
   namespace re_detail{

#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable: 4103)
#endif
#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif
#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

struct empty_padding{};

union padding
{
   void* p;
   unsigned int i;
};

template <int N>
struct padding3
{
   enum{
      padding_size = 8,
      padding_mask = 7
   };
};

template<>
struct padding3<2>
{
   enum{
      padding_size = 2,
      padding_mask = 1
   };
};

template<>
struct padding3<4>
{
   enum{
      padding_size = 4,
      padding_mask = 3
   };
};

template<>
struct padding3<8>
{
   enum{
      padding_size = 8,
      padding_mask = 7
   };
};

template<>
struct padding3<16>
{
   enum{
      padding_size = 16,
      padding_mask = 15
   };
};

enum{
   padding_size = padding3<sizeof(padding)>::padding_size,
   padding_mask = padding3<sizeof(padding)>::padding_mask
};

//
// class raw_storage
// basically this is a simplified vector<unsigned char>
// this is used by basic_regex for expression storage
//

class BOOST_REGEX_DECL raw_storage
{
public:
   typedef std::size_t           size_type;
   typedef unsigned char*        pointer;
private:
   pointer last, start, end;
public:

   raw_storage();
   raw_storage(size_type n);

   ~raw_storage()
   {
      ::operator delete(start);
   }

   void BOOST_REGEX_CALL resize(size_type n);
   
   void* BOOST_REGEX_CALL extend(size_type n)
   {
      if(size_type(last - end) < n)
         resize(n + (end - start));
	  pointer result = end;
      end += n;
      return result;
   }

   void* BOOST_REGEX_CALL insert(size_type pos, size_type n);

   size_type BOOST_REGEX_CALL size()
   {
      return end - start;
   }

   size_type BOOST_REGEX_CALL capacity()
   {
      return last - start;
   }

   void* BOOST_REGEX_CALL data()const
   {
      return start;
   }

   size_type BOOST_REGEX_CALL index(void* ptr)
   {
      return static_cast<pointer>(ptr) - static_cast<pointer>(data());
   }

   void BOOST_REGEX_CALL clear()
   {
      end = start;
   }

   void BOOST_REGEX_CALL align()
   {
      // move end up to a boundary:
      end = start + (((end - start) + padding_mask) & ~padding_mask);
   }
   void swap(raw_storage& that)
   {
      std::swap(start, that.start);
      std::swap(end, that.end);
      std::swap(last, that.last);
  }
};

inline raw_storage::raw_storage()
{
   last = start = end = 0;
}

inline raw_storage::raw_storage(size_type n)
{
   start = end = static_cast<pointer>(::operator new(n));
   BOOST_REGEX_NOEH_ASSERT(start)
   last = start + n;
}


#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable: 4103)
#endif
#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif
#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

} // namespace re_detail
} // namespace boost

#endif






