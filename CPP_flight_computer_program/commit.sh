#! /bin/bash

current_branch=`git branch 2> /dev/null | sed -e '/^[^*]/d' -e 's/* \(.*\)/(\1)/'`
commit_message=""


if [ $# -ne 1 ]
then
    echo "Usage: commit.sh <commit message>"
    exit 1
fi

if [ $current_branch == "(dev)" ]
then
    echo "You are on dev branch"
    git status
    git add .
    git commit -m "$1"
    git push origin dev
    echo "head on to github and create a pull request"
else
    echo "You are NOT on (dev) branch"
fi
