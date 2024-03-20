#!/bin/bash

. /etc/hxloginpref.conf &>/dev/null || :;

if [[ "$HXPREF_ENABLE" == "yes" ]]; then

# --- main big block ---

isroot=0;
if [[ "$UID" -eq 0 ]]; then
	isroot=1;
fi;

hxpref_cd()
{
	local d="$1";
	if [[ -z "$d" ]]; then
		d="$HOME";
		if [[ -z "$d" ]]; then
			command cd;
			return $?;
		fi;
	fi;
	if [[ "$d" == "-" ]]; then
		# The path will be pushed again right away
		popd +1 >/dev/null 2>/dev/null;
	fi;
	pushd "$d" >/dev/null;
	if [[ "$PWD" == "${DIRSTACK[1]}" ]]; then
		popd -0 >/dev/null 2>/dev/null;
	fi;
	if [[ "${#DIRSTACK[@]}" -ge "$HXPREF_DIRSTACK" ]]; then
		popd -0 >/dev/null 2>/dev/null;
	fi;
}

hxpref_popd()
{
	popd >/dev/null || popd +1 >/dev/null 2>/dev/null;
}

hxpref_pushd()
{
	local path;
	if [[ "$#" -eq 0 ]]; then
		pushd .;
	else
		for path in "$@"; do
			pushd "$path";
		done;
	fi;
}

hxpref_beautify_path()
{
	# Show at most the last two path components, if the whole depth of a
	# normalized $PATH is more than 3 levels, e.g.

	#
	#   PATH                                    (NORMALIZED DEPTH LEVEL)
	#
	#   /J/kernel/linux-2.6.17.6                (3)
	#           -> /D/kernel/linux-2.6.17.6     (3)
	#   /J/kernel/linux-2.6.17.6/fs             (4)
	#           -> ../linux-2.6.17.6/fs         (3)
	#   ~/Coding/hxtools                        (3)
	#           -> ~/Coding/hxtools             (3)
	#   ~/Coding/hxtools/bin                    (4)
	#           -> ../hxtools/bin               (3)
	#
	local pw2;
	local wd;
	(
	IFS="/";
	wd="$PWD/"
	# Special treatment for homedir, as ~ is cheaper (shorter) than
	# the full home path ($HOME)
	wd=${wd/#$HOME\//~\/}
	wd=${wd%/};
	set $wd;
	if [ \( -z "$1" -a $# -le "$[$HXPREF_BEAUTIFY_PATH_LEVEL]" \) -o $# -le \
	    "$HXPREF_BEAUTIFY_PATH_LEVEL" ]; then
		echo "$wd";
	else
		eval "echo -en \"../\${$[$#-1]}/\${$#}\"";
	fi;
	);
}

_complete_noop()
{
	return 0;
}

# Use smart directory completion
complete -o nospace -o dirnames -F _complete_noop \
	cd chroot md mkdir pushd rd rmdir;

# Allow truncation via > operator
set +C;

if [[ "$TERM" == "xterm" ]] && [[ -z "$MC_SID" ]]; then
	# Do not use Xterm title inside mc
	PS1_XTERM="\[\e]0;Shell - \h:\w\a\]";
fi;
export PS1_ROOT="$PS1_XTERM\A \h:\$(hxpref_beautify_path) # ";
export PS1_USER="$PS1_XTERM\A \h:\$(hxpref_beautify_path) \$ ";
export PS1_XROOT="$PS1_XTERM\[\e[0;1;30m\]\A \[\e[0;31m\]\h:\$(hxpref_beautify_path) \[\e[1m\]#\[\e[0m\] ";
export PS1_XUSER="$PS1_XTERM\[\e[0;1;30m\]\A \[\e[0;32m\]\h:\$(hxpref_beautify_path) \[\e[0;1;37m\]\$\[\e[0m\] ";
unset PS1_XTERM

eval "$(/usr/bin/dircolors -b /usr/share/hxtools/hxtools_dircolors)";
export QUILT_COLORS="diff_add=32:diff_rem=31:diff_hdr=1;37;44:diff_hunk=35"

# Find out what options this machine's "ls" supports
# Same option finding for "less"
#
LS_OPTIONS="-NT0"
for i in --color=auto --group-dir; do
	/bin/ls "$i" -d >/dev/null 2>&1 && LS_OPTIONS="$LS_OPTIONS $i"
done

export LS_OPTIONS;
# Remove PS1 from export list
unset PS1;

if [[ "$HXPREF_COLORS" == yes ]]; then
	if [[ "$isroot" -eq 1 ]]; then
		PS1="$PS1_XROOT";
	else
		PS1="$PS1_XUSER";
	fi;
else
	unset LS_COLORS;
	if [[ "$isroot" -eq 1 ]]; then
		PS1="$PS1_ROOT";
	else
		PS1="$PS1_USER";
	fi;
fi;

unalias -- + cd.. dir la ll ls-l you 2>/dev/null;
alias -- -="hxpref_popd";
alias ..="cd ../";
alias ...="cd ../../";
alias cd="hxpref_cd";
alias cp="cp -ip";
alias e='$EDITOR';
alias ls='command ls $LS_OPTIONS';
alias l='command ls $LS_OPTIONS -al';
alias md="mkdir -p";
alias mv="mv -i";
alias o="less -S";
alias rd="rmdir";
alias rf="command rm";
alias rm="command rm -i";

# --- end big main block ---

fi; # if [[ "$HXPREF_ENABLE" == "yes" ]];
