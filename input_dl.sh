#/bin/bash

ROOT_DIR=$(dirname $0)
ENV_PATH=$ROOT_DIR/.env
SESSION_TOKEN=$(grep SESSION_TOKEN "$ENV_PATH" | sed "s/SESSION_TOKEN=//")

DAY=$1
YEAR=$2

[ -z "$DAY" ] && DAY=1
[ -z "$YEAR" ] && YEAR=2015

INPUT="https://adventofcode.com/$YEAR/day/$DAY/input"
OUTPUT_DIR="$ROOT_DIR/$YEAR/day_$(printf %02d $DAY)"
OUTPUT="$OUTPUT_DIR/input.txt"

mkdir -p $OUTPUT_DIR
curl -b session=$SESSION_TOKEN $INPUT --output $OUTPUT

