#!/bin/zsh -x
CDUP="$(git rev-parse --show-toplevel)" 

cd "${CDUP}"

git add tmp/update_types.sh tmp/repl.sed
git c -m "Updates to the script" 
git branch START -f HEAD
git reset --hard
git clean -f
touch tmp/started
sed_cmd=""
while read l; do
    # Strip spaces from the line
    sed_cmd+="${(S)l##[[:space:]]%%[[:space:]]}"
    if [[ "${sed_cmd: -1}" == '/' ]]; then 
        continue
    fi

    if [[ -n ${sed_cmd} ]]; then
        echo $sed_cmd
        sed --in-place -E "${sed_cmd}" ${CDUP}/src/*.{c,h}pp || exit 2
        git restore --source=HEAD -- src/types.hpp

        cmake --build build -- clean
        cmake --build build
        if [[ -x build/src/Debug/apple2 ]]; then
            echo SUCCESS ---- 
            git commit src -m "$(printf "Executing:\n\n%s" "${sed_cmd}")"
            git branch SUCCESS -f HEAD
            unset all_sed_cmds
        else
            git reset START
            echo FAIL ------- ${sed_cmd}
            echo Step : ${sed_cmd} failed to build
            rm tmp/started
            exit 1
        fi
        sed_cmd=""
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
