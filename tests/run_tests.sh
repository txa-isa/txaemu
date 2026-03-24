#!/bin/sh
# Run all assembler tests and verify expected register values.
# Usage: tests/run_tests.sh  (from repo root or build directory)

set -e

ROOT=$(cd "$(dirname "$0")/.." && pwd)

TXAAS=$ROOT/asm/build/asm-txa
TXAEMU=$ROOT/build/txaemu
TESTS_DIR=$ROOT/tests
PASS=0
FAIL=0

check() {
    local name="$1"
    local asm="$TESTS_DIR/${name}.asm"
    local bin="/tmp/${name}.bin"
    local expected_r0="$2"

    printf "%-20s " "$name"

    if ! "$TXAAS" -o "$bin" "$asm" 2>/dev/null; then
        echo "FAIL (assembly error)"
        FAIL=$((FAIL + 1))
        return
    fi

    dump=$("$TXAEMU" -D "$bin" 2>/dev/null)
    actual=$(echo "$dump" | grep -o "R0=[0-9A-Fa-f]*" | head -1 | cut -d= -f2)

    if [ "$actual" = "$expected_r0" ]; then
        echo "PASS  (R0=0x${actual})"
        PASS=$((PASS + 1))
    else
        echo "FAIL  (expected R0=0x${expected_r0}, got R0=0x${actual})"
        FAIL=$((FAIL + 1))
    fi
}

echo "Running TXA16-1 tests"
echo "---------------------"
check test_alu    "001D"
check test_branch "000A"
check test_stack  "0078"
check test_shift  "0001"
check test_cond   "0007"
echo "---------------------"
echo "Results: $PASS passed, $FAIL failed"

[ "$FAIL" -eq 0 ]
