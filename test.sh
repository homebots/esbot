rm tmp/*.bin || true

for spec in $(ls -1 test/bot.spec.cpp);
do
  specName=$(basename $spec)
  bin=./tmp/$specName.bin

  clang++ -std=gnu++11 -o $bin $spec
  [ -f $bin ] && chmod +x $bin && $bin
done