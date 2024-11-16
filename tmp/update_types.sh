#!/bin/zsh -x
CDUP="$(git rev-parse --show-toplevel)" 

cd "${CDUP}"

git branch START -f HEAD
git reset --hard
git clean -f
touch tmp/started
while read line; do
    echo $line
    if test -n "${line}"; then 
        sed --in-place -E "${line}" ${CDUP}/src/*.{c,h}pp || exit 2
        git restore --source=HEAD -- src/types.hpp

        cmake --build build -- clean
        cmake --build build
        if [[ -x build/src/Debug/apple2 ]]; then
            echo SUCCESS ---- 
            git commit src -m "Executing: ${line}"
            git branch SUCCESS -f HEAD
            unset all_lines
        else
            git reset START
            echo FAIL ------- ${line}
            echo Step : ${line} failed to build
            rm tmp/started
            exit 1
        fi
    fi
done < ${CDUP}/tmp/repl.sed

if [[ -f tmp/started ]]; then
    echo -------------------

    git rm  ${CDUP}/tmp/*
    git commit --message "Removing temporaring scripts"
    git branch --delete START
else 
    exit 2
fi
