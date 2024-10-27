#!/bin/bash -x
CDUP="$(git rev-parse --show-toplevel)" 

cd "${CDUP}"

git reset --hard
git clean -f

cat ${CDUP}/tmp/repl.sed | while read -r line; do
    sed --in-place -E  ${CDUP}/src/*.{c,h}pp
    git restore src/types.hpp
    cmake --build build && git commit -m "Executing ${line}";
done

git rm  ${CDUP}/tmp/*
git commit --message "Removing temporaring scripts"
