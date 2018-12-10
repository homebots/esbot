rm tmp/*.bin || true

INCLUDE_PATH="-I $PWD/test/ -I $PWD/src"

for spec in $(find ./src -iname "$SPEC*.spec.cpp" -type f);
do
  specName=$(basename $spec)
  bin=./tmp/$specName.bin

  clang++ $INCLUDE_PATH -std=gnu++11 -o $bin $spec
  [ -f $bin ] && chmod +x $bin && $bin
done