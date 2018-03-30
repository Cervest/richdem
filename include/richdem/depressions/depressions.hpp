#ifndef _richdem_depressions_
#define _richdem_depressions_

#include <richdem/depressions/Barnes2014.hpp>
#include <richdem/depressions/Lindsay2016.hpp>
#include <richdem/depressions/Wei2018.hpp>
#include <richdem/depressions/Zhou2016.hpp>

namespace richdem {

template<class T> void FillDepressions       (Array2D<T> &dem){ PriorityFlood_Zhou2016         (dem); }
template<class T> void FillDepressionsEpsilon(Array2D<T> &dem){ PriorityFloodEpsilon_Barnes2014(dem); }
template<class T> void BreachDepressions     (Array2D<T> &dem){ CompleteBreaching_Lindsay2016  (dem); }

}

#endif
