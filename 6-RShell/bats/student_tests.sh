#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "valid invalid valid" {
    run ./dsh -s &
    sleep 1
    run ./dsh -c <<EOF                
echo "hello something" | nothing | echo something
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\r' | tr -d '[:cntrl:]' | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>somethingdsh4>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "exit command" {
    run ./dsh -s &
    sleep 1
    run ./dsh -c <<EOF                
exit
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\r' | tr -d '[:cntrl:]' | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>clientexited:gettingnextconnection...cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "grep .c" {
    run ./dsh -s &
    sleep 1
    run ./dsh -c <<EOF                
ls | grep .c
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\r' | tr -d '[:cntrl:]' | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>dsh_cli.cdshlib.crsh_cli.crsh_server.cdsh4>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}


@test "echo command" {
    run ./dsh -s &
    sleep 1
    run ./dsh -c <<EOF                
echo "halo"
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\r' | tr -d '[:cntrl:]' | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>halodsh4>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "invalid CD directory" {
    run ./dsh -s &
    sleep 1
    run ./dsh -c <<EOF                
cd doesntexist
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\r' | tr -d '[:cntrl:]' | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>cd:Nosuchfileordirectorydsh4>clientrequestedservertostop,stopping...cmdloopreturned-7"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "valid cd" {
    run ./dsh -s &
    sleep 1
    run ./dsh -c <<EOF                
cd bats
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\r' | tr -d '[:cntrl:]' | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>dsh4>clientrequestedservertostop,stopping...cmdloopreturned-7"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "cd + cd" {
    run ./dsh -s &
    sleep 1
    run ./dsh -c <<EOF                
cd bats | cd ..
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\r' | tr -d '[:cntrl:]' | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>cd:Nosuchfileordirectorydsh4>clientrequestedservertostop,stopping...cmdloopreturned-7"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "echo qoutes " {
    run ./dsh -s &
    sleep 1
    run ./dsh -c <<EOF                
echo "halloo"
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\r' | tr -d '[:cntrl:]' | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>halloodsh4>clientrequestedservertostop,stopping...cmdloopreturned-7"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "echoing max pipe" {
    run ./dsh -s &
    sleep 1
    run ./dsh -c <<EOF                
echo 1 | echo 2 | echo 3 | echo 4| echo 5| echo 6| echo 7 | echo 8
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\r' | tr -d '[:cntrl:]' | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>8dsh4>clientrequestedservertostop,stopping...cmdloopreturned-7"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "no command" {
    run ./dsh -s &
    sleep 1
    run ./dsh -c <<EOF    

stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\r' | tr -d '[:cntrl:]' | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>dsh4>clientrequestedservertostop,stopping...cmdloopreturned-7"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "dsh -h help" {
    
    run ./dsh -h

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\r[:cntrl:][:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="Usage:./dsh[-c|-s][-iIP][-pPORT][-x][-h]Defaultistorun./dshinlocalmode-cRunasclient-sRunasserver-iIPSetIP/Interfaceaddress(onlyvalidwith-cor-s)-pPORTSetportnumber(onlyvalidwith-cor-s)-xEnablethreadedmode(onlyvalidwith-s)-hShowthishelpmessage"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:"
    echo "Output: $stripped_output"
    echo "Expected Output: $stripped_expected_output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

}


@test "local dsh works" {
    run "./dsh" <<EOF                
exit
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="localmodedsh4>"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "client can exit server" {
    run ./dsh -s &

    sleep 1

    run ./dsh -c <<EOF
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>clientrequestedservertostop,stopping...cmdloopreturned-7"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "multiple commands with pipe" {
    run ./dsh -s &
    sleep 1
    run ./dsh -c <<EOF    

echo bye | echo hello | grep bye | wc -l
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\r' | tr -d '[:cntrl:]' | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>dsh4>0dsh4>clientrequestedservertostop,stopping...cmdloopreturned-7"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}