#!/bin/bash

# does not account for neg numbers
sci_notation_regex='^[0-9]+([.][0-9]+)?(e[0-9]+|e-[0-9]+)?$'

function test_time {
    # compare 
    if [[ $1 =~ $sci_notation_regex ]] ; 
    then
#        echo time properly formatted 
	echo -n ''
    else
        echo ERROR: time is not on stderr or not formatted properly
        echo
        rm .time
        exit 1
    fi
    # delete tmp file 
    rm .time
}


SUCCESS_FILE=.passed_prefixsum

if [ -e ${SUCCESS_FILE} ] ;
then
    rm ${SUCCESS_FILE} 
fi

for n in 10 1000 1000000;
do 
    for nbt in 1 2 3 4;
    do
        TEST=$(./prefixsum $n $nbt 2> .time)
        if [ "$TEST" = "checked" ] ; 
        then
            test_time $(cat .time)
        else
            echo FAIL: "./prefixsum $n $nbt does not create a correct prefixsum"
            exit 1
        fi
    done
done


touch ${SUCCESS_FILE} 


echo "================================"
echo
echo "Success! All PREFIXSUM tests passed! :)"
