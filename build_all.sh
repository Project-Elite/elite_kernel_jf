echo "Make Clean"
make clean
echo "Make Mrproper"
make mrproper
./build_att_aosp.sh
echo "Make Clean"
make clean
echo "Make Mrproper"
make mrproper
./build_vzw_aosp.sh
