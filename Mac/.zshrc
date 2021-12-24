# Gitの補完機能を有効化
fpath=(~/.zsh/completion $fpath)
autoload -Uz compinit
compinit

#pyenvのzshへの反映設定 
eval "$(pyenv init --path)"

# git設定
autoload -Uz vcs_info
setopt prompt_subst
zstyle ':vcs_info:git:*' check-for-changes true
zstyle ':vcs_info:git:*' stagedstr "%F{yellow}!"
zstyle ':vcs_info:git:*' unstagedstr "%F{red}+"
zstyle ':vcs_info:*' formats "%F{green}%c%u[%b]%f"
zstyle ':vcs_info:*' actionformats '[%b|%a]'
PROMPT='%T %n: %c '\$vcs_info_msg_0_' %# '
precmd(){
 vcs_info
 # Print a newline before the prompt, unless it's the
 # first prompt in the process.
 if [ -z "$NEW_LINE_BEFORE_PROMPT" ]; then
     NEW_LINE_BEFORE_PROMPT=1
 elif [ "$NEW_LINE_BEFORE_PROMPT" -eq 1 ]; then
     echo ""
 fi
}

