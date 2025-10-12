
home="$(cd $(dirname $0) && pwd)"

export LD_LIBRARY_PATH="${home}/library:${LD_LIBRARY_PATH}"

"${home}/shell" \
	"${home}/kernel" \
	"${home}/script/main.js" \
	"${home}" \
	"$@"
