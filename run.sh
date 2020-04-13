#/usr/bin/env bash
make
for file in test/*; do
  echo "###### Running "$file" ######"
  sudo dmesg --clear
  sudo ./bin/main < $file > output/$(sed 's/test\///g; s/\.txt//g' <<< $file)_stdout.txt
  sudo dmesg | grep Project > output/$(sed 's/test\///g; s/\.txt//g' <<< $file)_dmesg.txt
done
