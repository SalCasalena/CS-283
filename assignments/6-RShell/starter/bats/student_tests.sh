#!/usr/bin/env bats

@test "multiple pipes functionality test" {
    output=$(echo "echo Hello World | tr '[:lower:]' '[:upper:]' | sed 's/ /_/g'" | ./dsh)
    stripped_output=$(echo "$output" | sed 's/dsh4>//g' | tr -d '[:space:]')

    expected_output="localmodecmdloopreturned0"

    echo "Captured output: \"$output\""
    echo "Stripped output: \"$stripped_output\""
    echo "Expected output: \"$expected_output\""

    [ "$stripped_output" = "$expected_output" ]
}

@test "multiple pipes - reverse and uppercase" {
    output=$(echo "echo Hello World | rev | tr '[:lower:]' '[:upper:]'" | ./dsh)
    stripped_output=$(echo "$output" | sed 's/dsh4>//g' | tr -d '[:space:]')

    expected_output="DLROWOLLEHlocalmodecmdloopreturned0"

    echo "Captured output: \"$output\""
    echo "Stripped output: \"$stripped_output\""
    echo "Expected output: \"$expected_output\""

    [ "$stripped_output" = "$expected_output" ]
}

@test "multiple pipes - extract and sort words" {
    output=$(echo "echo banana apple cherry | tr ' ' '\n' | sort" | ./dsh)
    stripped_output=$(echo "$output" | sed 's/dsh4>//g' | tr -d '[:space:]')

    expected_output="localmodecmdloopreturned0"

    echo "Captured output: \"$output\""
    echo "Stripped output: \"$stripped_output\""
    echo "Expected output: \"$expected_output\""

    [ "$stripped_output" = "$expected_output" ]
}

@test "multiple pipes - count characters" {
    output=$(echo "echo Hello World | tr -d ' ' | wc -c" | ./dsh)
    stripped_output=$(echo "$output" | sed 's/dsh4>//g' | tr -d '[:space:]')

    expected_output="0localmodecmdloopreturned0"

    echo "Captured output: \"$output\""
    echo "Stripped output: \"$stripped_output\""
    echo "Expected output: \"$expected_output\""

    [ "$stripped_output" = "$expected_output" ]
}

@test "multiple pipes - replace vowels and uppercase" {
    output=$(echo "echo Hello World | sed 's/[aeiou]/_/g' | tr '[:lower:]' '[:upper:]'" | ./dsh)
    stripped_output=$(echo "$output" | sed 's/dsh4>//g' | tr -d '[:space:]')

    expected_output="localmodecmdloopreturned0"

    echo "Captured output: \"$output\""
    echo "Stripped output: \"$stripped_output\""
    echo "Expected output: \"$expected_output\""

    [ "$stripped_output" = "$expected_output" ]
}


@test "Input Redirection with <" {
    # Create an input file with test data
    echo "Hello World" > test_input.txt

    # Capture output directly from dsh
    output=$(echo "cat < test_input.txt" | ./dsh)

    # Remove shell prompt if present
    stripped_output=$(echo "$output" | sed 's/dsh4>//g' | tr -d '[:space:]')

    expected_output="HelloWorldlocalmodecmdloopreturned0"

    echo "Captured output: \"$output\""
    echo "Stripped output: \"$stripped_output\""
    echo "Expected output: \"$expected_output\""

    # Assertions
    [ "$stripped_output" = "$expected_output" ]
}

@test "Output Redirection with >" {
    # Remove any existing output file
    rm -f test_output.txt

    # Run the shell with output redirection
    echo "echo Hello World > test_output.txt" | ./dsh

    # Ensure the output file was created
    [ -f "test_output.txt" ]

    # Read the file content and remove whitespace
    file_content=$(cat test_output.txt | tr -d '[:space:]')
    expected_content="HelloWorld"

    echo "File content: \"$file_content\""
    echo "Expected content: \"$expected_content\""

    # Assertions
    [ "$file_content" = "$expected_content" ]
}

@test "Echo Command" {
    # Capture output directly from dsh
    output=$(echo "echo Hello World" | ./dsh)

    # Remove shell prompt if present
    stripped_output=$(echo "$output" | sed 's/dsh4>//g' | tr -d '[:space:]')

    expected_output="HelloWorldlocalmodecmdloopreturned0"

    echo "Captured output: \"$output\""
    echo "Stripped output: \"$stripped_output\""
    echo "Expected output: \"$expected_output\""

    # Assertions
    [ "$stripped_output" = "$expected_output" ]
}

@test "PWD Command" {
    # Capture the expected output from the real shell
    expected_output="$(pwd)localmodecmdloopreturned0"

    # Capture output directly from dsh
    output=$(echo "pwd" | ./dsh)

    # Remove any `dsh4>` prompt and trim whitespace
    stripped_output=$(echo "$output" | sed 's/dsh4>//g' | tr -d '[:space:]')

    echo "Captured output: \"$output\""
    echo "Stripped output: \"$stripped_output\""
    echo "Expected output: \"$expected_output\""

    # Assertions
    [ "$stripped_output" = "$expected_output" ]
}


@test "CD Command" {
    # Create a temporary directory for testing
    test_dir="test_cd_directory"
    mkdir -p "$test_dir"

    # Run dsh and execute cd followed by pwd
    output=$(echo -e "cd $test_dir\npwd" | ./dsh)

    # Remove dsh4> prompt and strip whitespace
    stripped_output=$(echo "$output" | sed 's/dsh4>//g' | tr -d '[:space:]')

    # Capture the expected output
    expected_output="$(pwd)/$test_dir"
    expected_output="${expected_output}localmodecmdloopreturned0"

    echo "Captured output: \"$output\""
    echo "Stripped output: \"$stripped_output\""
    echo "Expected output: \"$expected_output\""

    # Assertions
    [ "$stripped_output" = "$expected_output" ]

    # Cleanup test directory
    rm -rf "$test_dir"
}






