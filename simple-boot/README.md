# Overview
This is a very simple proof of concept to build a custom `fip.bin` file that runs on the milkv-duo 64MB. 
This should allow custom code to be ran right at bootup.

The `boot.sd` file currently comes from the resultant .img file built from the (duo-buildroot-sdk)[https://github.com/milkv-duo/duo-buildroot-sdk]. 
I just copied the file and pasted it here. If I can build it seperately, then another build step will be added

To run, `make` should be all that needs to be done. Though some file paths might need to be changed at the moment. The resultant image is placed in the `images` folder.

Most of the code came from these places
```
https://github.com/gtxzsxxk/milkv-duo-firmware-fsbl -> code in the fsbl folder, with many folders removed as to make it specific to my board
https://github.com/milkv-duo/duo-buildroot-sdk -> primarily used for the genimage.cfg file, with the files and sizing changed
```
While the bl33 code came from this sophgo forum page: https://forum.sophgo.com/t/use-opensbi-to-boot-your-own-operating-system/340


# Output
![image](https://github.com/user-attachments/assets/b9049799-ed5b-4612-b94b-12cbf33e1b92)
