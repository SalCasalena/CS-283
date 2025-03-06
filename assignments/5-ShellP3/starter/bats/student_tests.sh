#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file


@test "Check if echo outputs correctly" {
    run ./dsh <<EOF
echo "Hello, world!"
EOF
    [ "$status" -eq 0 ]
    [[ "${lines[0]}" == "Hello, world!" ]]
}

@test "Check if cd changes directory" {
    mkdir -p testdir
    run ./dsh <<EOF
cd testdir
pwd
EOF
    [ "$status" -eq 0 ]
    [[ "${lines[0]}" == "$(pwd)/testdir" ]]
    
    rm -rf testdir/*
    rmdir testdir
}


@test "cd with no arguments does nothing" {
    orig_dir=$(pwd)
    run ./dsh <<EOF
cd
pwd
EOF
    [ "$status" -eq 0 ]
    [[ "${lines[0]}" == "$orig_dir" ]]
}

@test "cd to non-existent directory fails" {
    run ./dsh <<EOF
cd doesnotexist
EOF
    [[ "$output" == *"cd: No such file or directory"* ]]
}


@test "exit command terminates shell" {
    run ./dsh <<EOF
exit
EOF
    [ "$status" -eq 0 ]
}

@test "External command uname -a runs successfully" {
    run ./dsh <<EOF
uname -a
EOF
    [ "$status" -eq 0 ]
}

@test "Echo handles spaces and quotes correctly" {
    run ./dsh <<EOF
echo "   hello     world   "
EOF
    [[ "${lines[0]}" == "   hello     world   " ]]
}

@test "Empty command does nothing" {
    run ./dsh <<EOF

EOF
    [ "$status" -eq 0 ]
}

@test "Invalid command should return an error" {
    run ./dsh <<EOF
thiscommanddoesnotexist
EOF
    [[ "${output}" =~ "dsh: command not found: thiscommanddoesnotexist" ]]
}

@test "Handles multiple spaces correctly" {
    run ./dsh <<EOF
  ls      -l   
EOF
    [ "$status" -eq 0 ]
}

@test "Handles quoted arguments correctly" {
    run ./dsh <<EOF
echo "hello    world"
EOF
    [[ "${lines[0]}" == "hello    world" ]]
}

@test "Forked background process runs successfully" {
    run ./dsh <<EOF
sleep 1 &
EOF
    [ "$status" -eq 0 ]
}

@test "exit with trailing spaces works" {
    run ./dsh <<EOF
exit   
EOF
    [ "$status" -eq 0 ]
}


@test "pwd returns correct directory" {
    run ./dsh <<EOF
pwd
EOF
    [[ "${lines[0]}" == "$(pwd)" ]]
}

@test "cd to parent directory works" {
    mkdir -p testdir/subdir
    parent_dir="$(pwd)/testdir"  # Capture expected path BEFORE changing dir

    cd testdir/subdir
    dsh_path="$(pwd)/../../dsh"  # Get absolute path of `dsh` before running it

    run "$dsh_path" <<EOF
cd ..
pwd
EOF

    [[ "${lines[0]}" == "$parent_dir" ]]  # Ensure correct directory after `cd ..`

    cd ../..
    rmdir testdir/subdir testdir
}



@test "Echo multiple arguments prints correctly" {
    run ./dsh <<EOF
echo Hello World!
EOF
    [[ "${lines[0]}" == "Hello World!" ]]
}

@test "Basic Pipe" {
    run "./dsh" <<EOF                
ls | grep dshlib.c
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dshlib.cdsh3>dsh3>cmdloopreturned0"
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

# Test multiple pipes: 'ls | grep dshlib.c | wc -l'
@test "Multiple Pipes" {
    run "./dsh" <<EOF                
ls | grep dshlib.c | wc -l
EOF

    # Remove shell prompt and extra messages
    filtered_output=$(echo "$output" | grep -vE "dsh3>|cmd loop returned")

    # Trim leading/trailing whitespace and newlines
    stripped_output=$(echo "$filtered_output" | tr -d '[:space:]')

    expected_output="1" # assuming there's only one match for dshlib.c

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Filtered Output: $filtered_output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check if the actual output matches expected output
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}


# Test invalid command in a pipe: 'ls | nonexistentcommand | wc -l'
@test "Invalid Command in Pipe" {
    run "./dsh" <<EOF                
ls | nonexistentcommand | wc -l
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="commandnotfound"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
}

# Test pipe with an empty command: 'ls | | wc -l'
@test "Empty Command in Pipe" {
    run "./dsh" <<EOF                
ls | | wc -l
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="Error: failed to parse command list"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
}

# Test single command with no pipe: 'ls'
@test "Single Command" {
    run "./dsh" <<EOF                
ls
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dshlib.cdsh3>dsh3>cmdloopreturned0"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
}

# Test multiple pipes with valid and invalid commands: 'ls | grep dsh | nonexistentcommand | wc -l'
@test "Multiple Pipes with Invalid Command" {
    run "./dsh" <<EOF                
ls | grep dsh | nonexistentcommand | wc -l
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="commandnotfound"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
}

# Test empty pipe: 'ls || wc -l'
@test "Empty Pipe" {
    run "./dsh" <<EOF                
ls || wc -l
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dshlib.cdsh3>dsh3>cmdloopreturned0"
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
}

# Test: Verify output redirection '>'
@test "Output redirection with > writes to a file" {
    run ./dsh <<EOF
echo "hello, class" > out.txt
EOF
    [ "$status" -eq 0 ]
    [ -f "out.txt" ]  # Ensure the file was created
    grep -q "hello, class" out.txt  # Check if the content is correct
}

# Test: Verify input redirection '<' reads from a file
@test "Input redirection with < reads from a file" {
    echo "hello, class" > out.txt  # Pre-create file with expected content
    run ./dsh <<EOF
cat < out.txt
EOF
    [ "$status" -eq 0 ]
    echo "$output" | tr -d '\r' | grep -q "hello, class"  # Normalize output and check
}


# Test: Verify input and output redirection together
@test "Input and output redirection work together" {
    echo "hello, class" > out.txt
    run ./dsh <<EOF
cat < out.txt > new_output.txt
EOF
    [ "$status" -eq 0 ]
    [ -f "new_output.txt" ]  # Ensure new_output.txt was created
    grep -q "hello, class" new_output.txt  # Check if the content matches
}

@test "Append redirection >> appends to existing file" {
    echo "hello, class" > out.txt  # Pre-create file with initial content
    run ./dsh <<EOF
echo "this is line 2" >> out.txt
EOF
    [ "$status" -eq 0 ]
    grep -q "hello, class" out.txt  # Verify first line still exists
    grep -q "this is line 2" out.txt  # Verify second line was appended
}

@test "Verify entire file content after appending with >>" {
    run cat out.txt
    [ "$status" -eq 0 ]
    [[ "$output" == *"hello, class"* ]]  # Ensure first line exists
    [[ "$output" == *"this is line 2"* ]]  # Ensure second line exists
}