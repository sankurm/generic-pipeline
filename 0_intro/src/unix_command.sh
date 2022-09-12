#!/bin/bash
#grep "warning: " "$1" | sed -e 's/^.*warning: //1' | wc -l
grep "warning: " "$1" | sed -e 's/^.*warning: //1' | uniq | sort | uniq
grep "warning: " /mnt/c/Sam/UP/logs_for_ankur.txt | sed -e 's/^.*warning: //1' | uniq | sort | uniq