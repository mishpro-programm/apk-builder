# apk-builder
Simple termux package to build apks.

## Example usage:
### Project structure:
##### src/com/example/helloworld/MainActivity.java
##### res/layout/main.xml
##### AndroidManifest.xml
##### main.ks
##### obj/
### Command:
``` shell
apk-builder --j src --r res --m AndroidManifest.xml --i $PREFIX/share/java/android-24.jar --obj obj --k main.ks --kp 12345678
```
### Explanation:
  --j argument (str): path to .java file\
  --r argument (str): path to resources\
  --m argument (str): path to manifest\
  --i argument (str): path to android.jar\
  --obj argument (str): path to folder where you want to store object files\
  --k argument (str): path to keystore file\
  --kp argument (int): keystore password

## Dependencies:
### ecj
##### Eclipse Java Compiler with android-24.jar
### aapt
##### Tool to compile and merge android resources
### dx
##### Dexing tool
### apksigner
##### Signing apk
### boost
##### Big library with many tweaks for C++
###### Used for arguments
### fmt
##### Formatting library
###### Used for formatting command strings
