mkdir build
cd build/
rm -rf *
cp ../test.sh .
./test.sh
cd ..
chmod -R 777 build/
cd build/
