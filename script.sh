for i in *
do
  mv $i `ls $i | tr [A-Z] [a-z]`
done
