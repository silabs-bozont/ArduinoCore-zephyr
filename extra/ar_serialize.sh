#!/bin/sh
#
# Serialize `ar` updates of a single archive. Arduino's parallel core build
# invokes `ar rcs core.a file.o` once per object; concurrent writers corrupt
# the archive (non-ELF members), which then surfaces as missing entry_point /
# _exit and other undefined references at link time.
#
# Usage: ar_serialize.sh <ar> [ar-args...] <archive> <object>
#

set -e

if [ "$#" -lt 3 ]; then
	echo "usage: $0 <ar> [ar-args...] <archive> <object>" >&2
	exit 2
fi

AR="$1"
shift

# Archive path is the second-to-last argument.
archive=""
prev=""
for arg in "$@"; do
	if [ -n "$prev" ]; then
		archive="$prev"
	fi
	prev="$arg"
done

if [ -z "$archive" ]; then
	echo "$0: could not determine archive path" >&2
	exit 2
fi

lockdir="${archive}.lock"
while ! mkdir "$lockdir" 2>/dev/null; do
	# Another ar invocation is updating the archive.
	sleep 0.05
done
trap 'rmdir "$lockdir" 2>/dev/null' EXIT INT TERM

"$AR" "$@"
