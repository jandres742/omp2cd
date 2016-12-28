#include "ShareSpecifier.h"

using namespace darts;

#ifndef _MSC_VER
const typing::ResourceType ShareSpecifier::classType;
#endif

std::list<IdT>::iterator
ShareSpecifier::getUsers(void)
{
    return users__.begin();
}

void ShareSpecifier::addUser(IdT user)
{
    users__.push_back(user);
}

void ShareSpecifier::removeUser(IdT user)
{
    users__.remove(user);
}
