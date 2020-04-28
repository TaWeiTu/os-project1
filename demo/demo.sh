#/usr/bin/env bash
make
mkdir -p demo/output
demo=(TIME_MEASUREMENT.txt FIFO_1.txt PSJF_2.txt RR_3.txt SJF_4.txt)

for file in "${demo[@]}"; do
  echo "###### Running "$file" ######"
  sudo dmesg --clear
  sudo ./bin/main < "test/"$file > ./demo/output/$(sed 's/test\///g; s/\.txt//g' <<< $file)_stdout.txt
  sudo dmesg | grep Project > ./demo/output/$(sed 's/test\///g; s/\.txt//g' <<< $file)_dmesg.txt
done
