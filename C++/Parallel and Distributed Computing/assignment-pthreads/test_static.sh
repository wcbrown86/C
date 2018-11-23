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


if [ -e .passed_static ] ; 
then
    rm .passed_static
fi


# static test cases
while read test; 
do

t=($test)
# correctness
ANSW=$(./static_sched ${t[0]} 0 10 ${t[1]} ${t[2]} ${t[3]} ${t[4]} 2> .time)  
if ./approx ${ANSW} ${t[5]};
then
#    echo correct 
# check time format
    test_time $(cat .time)

else # display error and quit
    echo FAIL: "./static_sched ${t[0]} 0 10 ${t[1]} ${t[2]} ${t[3]} ${t[4]}" should give roughly "${t[5]}" not ${ANSW}
    exit 1
fi

done < test_static_cases.txt


touch .passed_static


# all tests passed
echo ================================
echo
echo Success! All STATIC tests passed! ":)"
