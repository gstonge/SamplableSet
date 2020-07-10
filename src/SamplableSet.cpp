#include "SamplableSet.hpp"


sset::RNGType sset::BaseSamplableSet::gen_ = RNGType(time(NULL));


//seed the RNG
void sset::BaseSamplableSet::seed(unsigned int seed_value)
{
    BaseSamplableSet::gen_.seed(seed_value);
}
