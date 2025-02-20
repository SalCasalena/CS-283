#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suite in this file

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
