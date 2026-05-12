#!/bin/bash

sed -n -e 's/Previous link register : \(.*\)/echo $(addr2line -f -p -s -e arm\/backtrace\/backtrace \1$) "<=" \1/ep' \
    -e t \
    -e 's/----/----/p' \
    -e t \
    -e 's/\(Function.*\)/\1/p' < st.log