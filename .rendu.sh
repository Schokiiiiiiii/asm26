#!/bin/bash
script=${BASH_SOURCE[0]}
# Get the path of this script
SCRIPTPATH=$(realpath $(dirname "$script"))

#
# Hello if you want to edit this script here are a few pointers :
# 
# Functions are generic and don't require to be changed for specific labs
# Options are below, normally you don't have to change anything except
# the options, i.e., course & lab as well as the files and directories
# you want in the archive.
#
# LAB_FILES are explicitely checked so that the student is warned
# if files are missing. Files are copied into the archive
# LAB_DIRS are copied into the archive but not checked.
# Notes : Both can overlap, path is relative to script path
#

#
# Options
###########
LAB=$(git branch --show-current)
COURSE=asm

# Lab specific
LAB_FILES_ARRAY=("crackme_reg.txt" "crackme_mod.txt")
LAB_DIRS_ARRAY=("arm/makefiles")

#
# Derived options
###################

rendu_dir="/home/reds/rendus/${COURSE}_${LAB}_rendu_do_not_edit_or_delete"
archive_name="${COURSE}_${LAB}_rendu.tar.gz"

#
# FUNCTIONS
#############
check_lab_files () {
    for FILE in "${LAB_FILES_ARRAY[@]}"
    do
        if [ ! -f "${FILE}" ]
	then
	    echo "File ${FILE} was not found in ${SCRIPTPATH}"
	    echo "This file is graded and part of the lab"
	    read -p "Are you sure you want to continue ? [y/N] : " yn
	    if [ "$yn" != "y" ]
	    then
		echo "Exiting..."
		exit 1
	    else
		echo "Okay."
	    fi
	fi
    done
}

save_lab_files () {
    for FILE in "${LAB_FILES_ARRAY[@]}"
    do
	cp --parents "${FILE}" ${COURSE_TMP_DIR} &> /dev/null
    done
}

save_lab_dirs () {
    for DIR in "${LAB_DIRS_ARRAY[@]}"
    do
	"Adding directory ${DIR} ..."
        cp --parents "${DIR}"/*.c ${COURSE_TMP_DIR} &> /dev/null
	cp --parents "${DIR}"/*.S ${COURSE_TMP_DIR} &> /dev/null
	cp --parents "${DIR}"/*.inc ${COURSE_TMP_DIR} &> /dev/null
	cp --parents "${DIR}"/*.h ${COURSE_TMP_DIR} &> /dev/null
    done
}

diff_lab_files () {
    for FILE in "${LAB_FILES_ARRAY[@]}"
    do
	git diff origin/${LAB} "${FILE}" >> ${COURSE_TMP_DIR}/files.diff  2> /dev/null

    done
}

create_patch () {
    git diff origin/${LAB} > ${COURSE_TMP_DIR}/fin.patch 2> /dev/null
}

create_tmp_dir () {
    COURSE_TMP_DIR=$(mktemp -d -t "${COURSE}"-XXXXXXXX)
}

exit_clean () {
    if [ -z ${COURSE_TMP_DIR+x} ]
    then
	# Unset
	exit 0
    else
	if [ -d ${COURSE_TMP_DIR} ]
	then
	    rm -rf "${COURSE_TMP_DIR}"
	fi
    fi
    if [ -z ${COURSE_TMP_DIR+x} ]
    then
	exit 0
    else
	exit $1
    fi
}

#
# Body of the script
######################

cd "${SCRIPTPATH}"


check_lab_files
# tar -C is for change directory !
tar --exclude='*.o' --exclude='core' --exclude='*.su' --exclude='*.bin' -czf "${archive_name}" ${LAB_FILES_ARRAY[*]} ${LAB_DIRS_ARRAY[*]} &> /dev/null
echo "Archive ${archive_name} ready in ${SCRIPTPATH} !"
