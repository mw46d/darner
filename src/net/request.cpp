#include "darner/net/request.h"

#include <boost/version.hpp>

using namespace boost;
using namespace boost::spirit;
using namespace boost::spirit::ascii;
using boost::spirit::ascii::no_case;
#if BOOST_VERSION < 104100
using namespace boost::spirit::arg_names;
#endif
using namespace darner;

request_grammar::request_grammar()
: request_grammar::base_type(start)
{
   key_name %=
      +((alnum|punct) - '/');

   stats =
      no_case[lit("stats")]     [phoenix::ref(req.type) = request::RT_STATS];

   version =
      no_case[lit("version")]   [phoenix::ref(req.type) = request::RT_VERSION];

   destroy =
      no_case[lit("delete ")]   [phoenix::ref(req.type) = request::RT_DESTROY]
      >> key_name      [phoenix::ref(req.queue) = _1];

   flush =
      no_case[lit("flush ")]    [phoenix::ref(req.type) = request::RT_FLUSH]
      >> key_name      [phoenix::ref(req.queue) = _1];

   flush_all =
      no_case[lit("flush_all")] [phoenix::ref(req.type) = request::RT_FLUSH_ALL];

   set_option =
      no_case[lit("/sync")]     [phoenix::ref(req.set_sync) = true];

   set =
      no_case[lit("set ")]      [phoenix::ref(req.type) = request::RT_SET]
      >> key_name      [phoenix::ref(req.queue) = _1]
      >> *set_option
      >> ' '
      >> uint_ // flags (ignored)
      >> ' '
      >> uint_ // expiration (ignored for now)
      >> ' '
      >> uint_         [phoenix::ref(req.num_bytes) = _1];

   get_option =
      no_case[lit("/open")]     [phoenix::ref(req.get_open) = true]
      | no_case[lit("/peek")]   [phoenix::ref(req.get_peek) = true]
      | no_case[lit("/close")]  [phoenix::ref(req.get_close) = true]
      | no_case[lit("/abort")]  [phoenix::ref(req.get_abort) = true]
      | (
         no_case[lit("/t=")]
         >> uint_      [phoenix::ref(req.wait_ms) = _1]
        );

   get =
      no_case[lit("get")]       [phoenix::ref(req.type) = request::RT_GET]
      >> -no_case[lit('s')] // "gets" is okay too
      >> ' '
      >> key_name      [phoenix::ref(req.queue) = _1]
      >> *get_option
      >> -lit(' '); // be permissive to clients inserting spaces

   fanout =
      no_case[lit("fanout ")]   [phoenix::ref(req.type) = request::RT_FANOUT]
      >> key_name      [phoenix::ref(req.queue) = _1];

   start = (stats | version | destroy | flush | flush_all | set | get | fanout) >> eol;
}
