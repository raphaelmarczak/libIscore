cd libController
sh compileMinuit.sh
make
cp Plugins/*.dylib /Library/Application\ Support/Virage/
sh cleanMinuit.sh