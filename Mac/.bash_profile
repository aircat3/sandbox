# terminalの先頭文字設定
PS1="\t \W/ \[\e[1;36m\]\$\[\e[m\]"
function insert_blankline {
    if [[ -z ${IS_NOT_FIRST_LINE} ]]; then
        IS_NOT_FIRST_LINE=true
    else
        printf '\n'
    fi
}
PROMPT_COMMAND='insert_blankline'

# terminalのサジェスト設定 
[ -f /usr/local/etc/bash_completion ] && . /usr/local/etc/bash_completion
