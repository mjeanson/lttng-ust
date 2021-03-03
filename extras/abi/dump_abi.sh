#!/bin/bash

set -eu

INDIR=$1
OUTDIR=$2

ARGS=(
	"--annotate" # Add comments to the xml output
	"--no-corpus-path" # Do not put the path in the abi-corpus
)

for lib in "${INDIR}"/*.so.?
do
	abidw "${ARGS[@]}" --out-file "${OUTDIR}/$(basename "$lib").xml" "$lib"

	# Clean the full paths
	sed -i "s#$(pwd)##g" "${OUTDIR}/$(basename "$lib").xml"
done

