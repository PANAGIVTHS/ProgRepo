#!/bin/bash
myfile=$1

./create_fbn_txt < $myfile | sort | uniq > ${myfile}.uniq
