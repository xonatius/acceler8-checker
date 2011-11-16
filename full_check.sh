#/bin/bash
CHECKER="./checker"
TEST_DIR="tests"
TMP_FILE="output.tmp"

if [ ! -f $CHECKER ]; then
    echo "Checker \"$CHECKER\" not found. Try to build it with \"./build.sh\""
    exit 1
fi

if [ ! -x $CHECKER ]; then
    echo "Checker \"$CHECKER\" is not executable. Try \"chmod +x $CHECKER\"."
    exit 1
fi

if [ "$1" = "" ]; then
    echo "Usage: ./full_check.sh ./solution_executable_file [tests_directory]"
    exit 1
else
    SOLUTION=$1
fi

if [ ! -f $SOLUTION ]; then
    echo "File \"$SOLUTION\" not found."
    exit 1
fi

if [ ! -x $SOLUTION ]; then
    echo "File \"$SOLUTION\" is not executable. Try \"chmod +x $SOLUTION\"."
    exit 1
fi


if [ "$2" != "" ]; then
    TEST_DIR=$2
fi

for inp in $( echo $TEST_DIR"/*.input" );
do
    input=$inp
    output=$( echo $inp | sed "s/input/output/g")
    if [ ! -f $output ]; then
        echo "Output file \"$output\" not found. Skiping..."
        continue
    fi
    echo "Executing \"time $SOLUTION \"$input\" \"$TMP_FILE\"\"..."
    time $SOLUTION "$input" "$TMP_FILE"
    if [ $? != 0 ]; then
        echo "Error: Solution failed"
    else
        echo ""
        $CHECKER $input $output $TMP_FILE
    fi;
    rm $TMP_FILE > /dev/null
done;

echo "Done"
