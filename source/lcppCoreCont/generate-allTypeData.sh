#bin/sh

find lcpp/core/typeSystem/types -type f | grep -i ".*data.*\.h" | sed -e 's:\(.*\):#include "\1":g' > lcpp/core/typeSystem/allTypeData.h
