source version.txt
NEW_MAJOR=$MAJOR
NEW_MINOR=$MINOR
NEW_PATCH=$PATCH
if [ $1 = 'minor' ]
then
    NEW_MINOR=$((NEW_MINOR+1))
elif [ $1 = 'major' ]
then
    # UPDATED=$((MAJOR+1))
    NEW_MAJOR=$((NEW_MAJOR+1))
    echo "$NEW_MAJOR"
elif [ $1 = 'patch' ]
then
    # UPDATED=$((PATCH+1))
    NEW_PATCH=$((NEW_PATCH+1))
else
    echo "no change"
fi
version="_$NEW_MAJOR"_"$NEW_MINOR"_"$NEW_PATCH"
make clean
make all
echo $version
cp strifesisters.pce "strifesisters$version.pce"
rm strifesisters.pce
echo "MAJOR=$NEW_MAJOR\nMINOR=$NEW_MINOR\nPATCH=$NEW_PATCH" > version.txt