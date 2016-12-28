#pragma once

#include "Resource.h"
#include "Specifier.h"
#include <list>

namespace darts {

/*
	Class: ShareSpecifier

	A class used to represent shared resources. It provides
	methods query, adding, and removing users from a given resource.

	InstanceOf:
		<Resource>
*/
class ShareSpecifier : public Specifier {
private:
    //Section: private ->

    /*
		Variable: users

		A list of users for the given resource. These people use
		the resource.
	*/
    std::list<IdT> users__;

public:
    //Section: Public ->

    /*
		Variable: classType
			Type of this class.
	*/
    const static typing::ResourceType classType = typing::SharedResource;
    /*
		Variable: classInherit
			Inheritance information for this class.
	*/
    const static unsigned int classInherit = classType * Resource::classInherit;

    /*
		Function: getUsers

		Returns a list of users of the resource.

		Returns:

			A list of <IdTs> using the current resource.
	*/
    std::list<IdT>::iterator
    getUsers(void);

    /*
		Function: addUser

		Adds a user to the list of users of this resource,

		Parameters:

			user - user to add to the list of users.
	*/
    void
    addUser(IdT user);

    /*
		Function: removeUser

		Removes a user to the list of users of this resource,

		Parameters:

			user - user to remove from the list of users.
	*/
    void
    removeUser(IdT user);
};

} // namespace darts
