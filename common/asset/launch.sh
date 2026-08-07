# launch script for sh

twinning="$(cd $(dirname "$0") && pwd)"
twinning="$(printf '%s\n' "$twinning" | sed 's|\\|/|g')"

case "$(uname -s)" in
	CYGWIN*|MINGW*|MSYS*|Windows*)
		export PATH="${twinning}/library:${PATH}"
		export PATHEXT=";${PATHEXT}"
		;;
	*)
		export LD_LIBRARY_PATH="${twinning}/library:${LD_LIBRARY_PATH}"
		;;
esac

"${twinning}/shell" "${twinning}/kernel" "${twinning}/script/main.js" "${twinning}" "$@"

echo "press <ENTER> to exit ..."
read
