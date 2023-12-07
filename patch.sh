source version.txt
UPDATED=$((PATCH+1))
make clean
make all
version="_$MAJOR"_"$MINOR"_"$UPDATED"
cp strifesisters.pce "strifesisters$version.pce"
rm strifesisters.pce
echo "MAJOR=$MAJOR\nMINOR=$MINOR\nPATCH=$UPDATED" > version.txt