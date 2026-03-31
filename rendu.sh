#!/bin/bash
script=${BASH_SOURCE[0]}
# Get the path of this script
SCRIPTPATH=$(realpath $(dirname "$script"))

LAB=$(git branch --show-current)

cd "${SCRIPTPATH}"

# This script tries to update the .rendu.sh script from git
# Then executes the .rendu.sh script, this allows the script
# to be updated, check .rendu.sh to see what it does.
git fetch &> /dev/null
git checkout "origin/${LAB}" .rendu.sh &> /dev/null
# Launch .rendu.sh
./.rendu.sh
