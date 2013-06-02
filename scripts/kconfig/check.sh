#!/bin/sh
# Needed for systems without gettext
<<<<<<< HEAD
$* -xc -o /dev/null - > /dev/null 2>&1 << EOF
=======
$* -x c -o /dev/null - > /dev/null 2>&1 << EOF
>>>>>>> remotes/linux2/linux-3.4.y
#include <libintl.h>
int main()
{
	gettext("");
	return 0;
}
EOF
if [ ! "$?" -eq "0"  ]; then
	echo -DKBUILD_NO_NLS;
fi

